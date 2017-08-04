require "pacmancenter"
require "arclight";
require "utility"
require "torch";
require "nn"


local qlearclustenode = torch.class('arclight.qlearclustenode');
qlearclustenode.neuronnet = nil;
qlearclustenode.target_q = nil;
qlearclustenode.criterion = nil;
qlearclustenode.loss = nil;
qlearclustenode.count = nil;
qlearclustenode.adamstate = nil;--adam状态


function qlearclustenode:Setup(seed)
  local nThreads = torch.getnumthreads()
  torch.setnumthreads(nThreads)
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
  self.sumtree = arclight.sumtree;
end

function qlearclustenode:SetLearningRate(lr)
  self.learningRate = lr;
end

function qlearclustenode:SetExplorationRate(er)
  self.explorate = er;
end

function qlearclustenode:SetTrainingTimes(t)
  training_times = t;
end

function qlearclustenode:SetNeuronCriterion(neuronmaker)
  loadstring(neuronmaker)();
  self.neuronnet, self.criterion = MakeNeuronCriterion();
  self.target_q = self.neuronnet:clone();
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
  self.target_q = self.neuronnet:clone();
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
    local d, w, h = get_pac_state_size();
    local newstate = state:view(1,d,w,h);
    local out = self.neuronnet:forward(newstate);--通过神经网络
    if nil == self.outstring then
      self.outstring = string.format("%.4f\t%.4f\t%.4f\t%.4f", out[1][1],out[1][2],out[1][3],out[1][4]);
    end
    local y, i = torch.max(out, 2);
    return i[1][1];
  else
    return math.random(action_count());
  end
end

--开始训练
function qlearclustenode:Trainning(its, tf, tbc, tbs, sf, ls, mep, ed, ss)
  training_frequency = tf;
  training_batch_count = tbc;
  training_batch_size = tbs;
  switch_frequency = sf;
  learn_start = ls;
  max_experience_pool = mep;
  experiences_discount = ed;
  synchronize_step = ss;
  self.grad_scale = 0;
  self.step_size = 0;
  self.part_loss = 0;
  self.learntimes = 0;  
  self.sumtree:Initialization(max_experience_pool);
  self.current_state = get_pac_state(self.meanstd);
  for episode = self.episode, its do
    local episodereward = 0
    self.printout = true;
    WARNING(string.format("Begin %d episode explorate %.3f", episode, self.explorate));
    for stepcount=1, synchronize_step do
      local action = self:_GetAction(self.current_state);
      local reward, terminal = keypressed_playing(action);
      episodereward = episodereward + reward;
      local next_state = get_pac_state(self.meanstd);
      self:_InsertExperience(self.current_state, next_state, action, reward, terminal);
      self.current_state = next_state;
      self:_TryLearning(stepcount);
      if stepcount % 100 == 0 then
        collectgarbage();
      end
    end
    local ps, pb = self:_SerializeParameters();
    local ss, sb = self:_GetSubBuffer()
    self.centrcenode:EpsiodeDone(episode, episodereward, ps, pb, ss, sb, self.outstring);
    self.outstring = nil;
    self.resumer = Context:GetServiceContext();
    self.resumer:Pasue();
    WARNING(string.format("end %d episode reward %d", episode, episodereward));
  end
end

function qlearclustenode:_TryLearning(step)
  if self.sumtree:Size() >= learn_start then
    if step % training_frequency == 0 then
      self.learntimes = self.learntimes + 1;
      local indices, transition = self.sumtree:Sampling(training_batch_count * training_batch_size);
      local experience, prioritys = self:_CaculateExperiences(transition);
      self.sumtree:UpdatePriority(indices, prioritys);
      local pl, ss, gs = self:_doTrainning(experience);
      LOG(string.format("part %d training grad scale %.8f step size %.6f loss %.8f", step,gs,ss,pl));
      self.grad_scale = math.max(self.grad_scale, gs);
      self.step_size = math.max(self.step_size, ss);
      self.part_loss = self.part_loss + pl;
      if 0 == (step % switch_frequency) then
        self.part_loss = self.part_loss / self.learntimes;
        self.centrcenode:StepLoss(self.part_loss);
        LOG(string.format("switch net training grad scale %.8f step size %.6f loss %.8f", self.grad_scale,self.step_size,self.part_loss));
        self.learntimes = 0;
        self.grad_scale = 0;
        self.step_size = 0;
        self.part_loss = 0;
        self.target_q = self.neuronnet:clone();
      end
    end    
  end  
end

function qlearclustenode:_CaculateExperiences(transition)
  local prioritys = {}
  local experiences = {};
  local a = action_count();
  local d, w, h = get_pac_state_size();
  assert(1==d);  
  self.target_q:evaluate();
  self.neuronnet:evaluate();
  for batchcount=1, training_batch_count do
    local current_state = torch.Tensor(training_batch_size, d, w, h);
    local next_state = torch.Tensor(training_batch_size, d, w, h);
    local action = {};
    local reward = {};
    local isterminal = {};
    for index=1, training_batch_size do
      local expindex = (batchcount-1)*training_batch_size+index;
      local expdata = transition[expindex];
      current_state[{index, {}, {}, {}}]:copy(expdata.current_state);
      next_state[{index, {}, {}, {}}]:copy(expdata.next_state);
      table.insert(action, expdata.action);
      table.insert(reward, expdata.reward);
      table.insert(isterminal, expdata.isterminal);
    end
    
    local current_out = self.neuronnet:forward(current_state):clone();
    local next_out = self.target_q:forward(next_state):clone();
    local next_eval = self.neuronnet:forward(next_state);--double dqn
    local _, next_action = torch.max(next_eval, 2);
    local updatereward = current_out:clone();
    for index=1, training_batch_size do
      if isterminal[index] then
        updatereward[{index, action[index], 1}] = reward[index];
      else
        updatereward[{index, action[index], 1}]
          = reward[index]+ experiences_discount
            * next_out[{index, next_action[{index, 1, 1}], 1}];
      end
      local priority = math.abs(current_out[{index, action[index], 1}] - updatereward[{index, action[index], 1}]);
      table.insert( prioritys, priority );
    end
    updatereward = torch.clamp(updatereward, -1.5, 1.5);
    batch = { input = current_state, target = updatereward }
    table.insert( experiences, batch );
  end
  return experiences, prioritys;
end

function qlearclustenode:_doTrainning(partdata)
  local loss = 0;
  local batchsize = #partdata;
  local accscale = 1 / batchsize;
  self.neuronnet:training();
  self.neuronnet:zeroGradParameters();
  for i, data in ipairs(partdata) do
    self.neuronnet:forward(data.input);
    local lloss = self.criterion:forward(self.neuronnet.output, data.target) * accscale;--计算误差;
    --[[if self.printout or lloss > 1 then
      self.printout = false;
      LOG(tostring(self.neuronnet.output));
      LOG(tostring(data.target));
    end]]--
    loss = loss + lloss
    local grad = self.criterion:backward(self.neuronnet.output, data.target);--计算梯度
    self.neuronnet:backward(data.input, grad, accscale)
  end
  
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
  local w_scale = torch.norm(ow);
  local g_scale = stepSize*torch.norm(ogw);
  local grad_scale = g_scale/w_scale;
  ow:addcdiv(-stepSize, self.adamstate.m, self.adamstate.denom); 

  return loss, stepSize, grad_scale;
end

function qlearclustenode:_SerializeParameters()
  local w = self.neuronnet:getParameters();
  local struct, buffer = arclight.Tensor2Message(w);
  return struct, buffer;
end

function qlearclustenode:_GetSubBuffer()
  local sp = self.neuronnet:getSubparameters();
  local struct, buffer = arclight.Tensor2Message(sp);
  return struct, buffer;
end

return qlearclustenode;