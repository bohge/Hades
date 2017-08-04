require "pacmancenter"
require "arclight";
require "utility"
require "torch";
require "nn"


local qlearclustenode = torch.class('arclight.qlearclustenode');
qlearclustenode.centrcenode = nil;
qlearclustenode.neuronnet = nil;
qlearclustenode.criterion = nil;
qlearclustenode.loss = nil;
qlearclustenode.count = nil;
qlearclustenode.adamstate = nil;--adam状态


function qlearclustenode:Setup(seed, myself)  
  self.caculaters = {};
  local nThreads = torch.getnumthreads()
  torch.setnumthreads(nThreads)
  self.nodescount = nThreads;
  LOG("seed is "..seed.."thread num is "..nThreads);
  math.randomseed(seed);
  torch.manualSeed(seed);
  start_new_game();--初始化游戏
  
  --初始化adam
  self.adamstate = {};
  self.adamstate.t = 0;
  self.adamstate.beta1 = 0.9;
  self.adamstate.beta2 = 0.995;
  self.adamstate.epsilon = 1e-7;
    
  self.episode = 1;
  self.switchtimes = 0;
  self.grad_scale = 0;
  self.step_size = 0;
  self.part_loss = 0;
  self.sumtree = arclight.sumtree;
  
  
  for i=1,self.nodescount do    
    local caculater = IServicesSystem:Create("arclight.pddqn_grad_qlearcaculatenode_cnn");
    caculater:Setup(myself);
    table.insert(self.caculaters, caculater);
  end
end

function qlearclustenode:SetLearningRate(lr)
  self.learningRate = lr;
end

function qlearclustenode:SetExplorationRate(er)
  self.explorate = er;
end

function qlearclustenode:SetNeuronCriterion(neuronmaker)
  loadstring(neuronmaker)();
  self.neuronnet, self.criterion = MakeNeuronCriterion();
  for _, node in pairs(self.caculaters) do
    node:SetNeuronCriterion(neuronmaker);
  end
  collectgarbage();
end

function qlearclustenode:SetMeanstd(meanstd)
  self.meanstd = meanstd;
end

function qlearclustenode:SetEpisode(e)
  self.episode = e;
end

function qlearclustenode:SynchronyNeuronnet(ps, pb)
  local ow = self.neuronnet:getParameters();
  arclight.Message2Tensor(ps, pb, ow);
end

function qlearclustenode:Resume()
  self.resumer:Resume();
end

function qlearclustenode:SetCentrcenode(node)
  self.centrcenode = node;
end

--插入经验
function qlearclustenode:_InsertExperience(current_state, next_state, action, reward, isterminal)
  self.sumtree:InsertExperience(math.abs(reward), current_state, next_state, action, reward, isterminal)
end

--得到动作
function qlearclustenode:_GetAction(state)
   if math.random() > self.explorate then
    self.neuronnet:evaluate();
    local out = self.neuronnet:forward(state);--通过神经网络
    if nil == self.outstring then
      self.outstring = string.format("%.4f\t%.4f\t%.4f\t%.4f", out[1],out[2],out[3],out[4]);
    end    
    local y, i = torch.max(out, 1);
    return i[1];
  else
    return math.random(action_count());
  end
end

--开始训练
function qlearclustenode:Trainning(its, tf, tbs, sf, ls, mep, ed, ss)
  training_frequency = tf;
  training_batch_size = tbs;
  switch_frequency = sf;
  learn_start = ls;
  max_experience_pool = mep;
  synchronize_step = ss;
  for _, node in pairs(self.caculaters) do
    node:SetDiscount(ed);
  end
  self.sumtree:Initialization(max_experience_pool);
  self.current_state = get_pac_state(self.meanstd);
  for episode = self.episode, its do
    local episodereward = 0
    WARNING(string.format("Begin %d episode explorate %.3f", episode, self.explorate));
    for stepcount=1, synchronize_step do
      local action = self:_GetAction(self.current_state);
      local reward, terminal = keypressed_playing(action);
      episodereward = episodereward + reward;
      local next_state = get_pac_state(self.meanstd);
      self:_InsertExperience(self.current_state, next_state, action, reward, terminal);
      self.current_state = next_state;
      self:_TryLearning(stepcount);
      if stepcount % 1000 == 0 then
        for _, node in pairs(self.caculaters) do
          node:CollectGarbage();
        end
        collectgarbage();
      end 
    end
    local ps, pb = self:_SerializeParameters();
    self.centrcenode:EpsiodeDone(episode, episodereward, ps, pb, self.outstring);
    self.outstring = nil;
    self.resumer = Context:GetServiceContext();
    self.resumer:Pasue();
    WARNING(string.format("end %d episode reward %d", episode, episodereward));
  end
end

function qlearclustenode:_TryLearning(step)
  if self.sumtree:Size() >= learn_start then
    if step % training_frequency == 0 then
      local indices, transition = self.sumtree:Sampling(training_batch_size);
      local ps,pb = self:_SerializeParameters();
      for _, node in pairs(self.caculaters) do
        node:SynchronyNeuronnet(ps, pb);
      end
      local prioritys = self:_CaculateExperiences(transition);
      self.sumtree:UpdatePriority(indices, prioritys);
      local pl, ss, gs = self:_doTrainning();
      LOG(string.format("part training grad scale %.8f step size %.6f loss %.8f", gs,ss,pl));
      self.grad_scale = math.max(self.grad_scale, gs);
      self.step_size = math.max(self.step_size, ss);
      self.part_loss = self.part_loss + pl / switch_frequency;
      self.switchtimes = self.switchtimes + 1;
      if 0 == (self.switchtimes % switch_frequency) then
        --LOG("_TrySwitchTargetQ "..self.switchtimes);
        self.centrcenode:StepLoss(self.part_loss);
        LOG(string.format("switch net training grad scale %.8f step size %.6f loss %.8f", self.grad_scale,self.step_size,self.part_loss));
        self.switchtimes = 0;
        self.grad_scale = 0;
        self.step_size = 0;
        self.part_loss = 0;
        for _, node in pairs(self.caculaters) do
          node:SwitchTargetQ();
        end
      end
    end    
  end  
end

function qlearclustenode:_CaculateExperiences(transition)
  self.prioritys = {}
  self.countcollect = 0;  
  local ni = 1;  
  for i,v in ipairs(transition) do
    local css, csb = arclight.Tensor2Message(v.current_state);
    local nss, nsb = arclight.Tensor2Message(v.next_state);
    local node = self.caculaters[ni];
    ni = ni + 1 > self.nodescount and 1 or ni + 1;    
    node:CaculateExperiences(i, css, csb, nss, nsb, v.action, v.reward, v.isterminal);
    table.insert( self.prioritys, 1 );
  end
  self.resumer = Context:GetServiceContext();
  self.resumer:Pasue();
  return self.prioritys;
end

function qlearclustenode:CollectExperiences(index, p)
  self.prioritys[index] = p;
  self.countcollect = self.countcollect + 1;
  if self.countcollect == training_batch_size then
    self.resumer:Resume();
  end  
end

function qlearclustenode:_doTrainning()
  self.loss = 0;
  local accscale = 1 / training_batch_size;
  self.countcollect = 0;  
  self.neuronnet:training();
  self.neuronnet:zeroGradParameters();  
  for _, node in pairs(self.caculaters) do
    node:CaculateGradient(accscale);
  end
  self.resumer = Context:GetServiceContext();
  local loss, stepSize, grad_scale = self.resumer:Pasue();
  return loss, stepSize, grad_scale;
end

function qlearclustenode:CollectGradient(loss, gs, gb)
  self.loss = self.loss + loss;
  self.countcollect = self.countcollect + 1;
  local ow, ogw = self.neuronnet:getParameters();
  local nogw = arclight.Message2Tensor(gs, gb);
  ogw:add(nogw);
  if self.countcollect == self.nodescount then
    local ow, ogw = self.neuronnet:getParameters();
    self.adamstate.t = self.adamstate.t + 1;
    self.adamstate.m = self.adamstate.m or torch.Tensor(ogw:size()):zero();
    self.adamstate.v = self.adamstate.v or torch.Tensor(ogw:size()):zero();
    self.adamstate.denom = self.adamstate.denom or torch.Tensor(ogw:size()):zero();
    self.adamstate.m:mul(self.adamstate.beta1):add(1-self.adamstate.beta1, ogw);
    self.adamstate.v:mul(self.adamstate.beta2):addcmul(1-self.adamstate.beta2, ogw, ogw);
    self.adamstate.denom:copy(self.adamstate.v):sqrt():add(self.adamstate.epsilon);
    local biasCorrection1 = 1 - self.adamstate.beta1^self.adamstate.t
    local biasCorrection2 = 1 - self.adamstate.beta2^self.adamstate.t
    local stepSize = self.learningRate * math.sqrt(biasCorrection2)/biasCorrection1;
    ogw:copy(self.adamstate.m):cdiv(self.adamstate.denom);
    ow:addcdiv(-stepSize, self.adamstate.m, self.adamstate.denom); 
    
    local w_scale = torch.norm(ow);
    local g_scale = stepSize*torch.norm(ogw);
    local grad_scale = g_scale/w_scale;
    self.resumer:Resume(self.loss, stepSize, grad_scale);
  end
end

function qlearclustenode:_SerializeParameters()
  local w = self.neuronnet:getParameters();
  local struct, buffer = arclight.Tensor2Message(w);
  return struct, buffer;
end

return qlearclustenode;