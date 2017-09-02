local distributions = require "distributions"
require "arclight";
require "utility"
require "torch";
require "nn"

local TINY_EPSILON = 1e-10;
local ENTROPY_BETA = 0.01;
local REWARD_SCALE = 0.1;
local ACTION_BOUND = 0;
local EXPERIENCES_DISCOUNT = 0.9;
local UPDATE_GLOBAL_ITER = 5


local aaacclustenode = torch.class('arclight.aaacclustenode');
aaacclustenode.game = nil;
aaacclustenode.loss = nil;
aaacclustenode.count = nil;


function aaacclustenode:Setup(seed)
  local nThreads = torch.getnumthreads()
  torch.setnumthreads(nThreads)
  LOG("seed is "..seed.."thread num is "..nThreads);
  math.randomseed(seed);
  torch.manualSeed(seed);
  self.game = arclight.openaigym;
end

function aaacclustenode:SetNeuronCriterion(neuronmaker)
  loadstring(neuronmaker)();
  local ss = self.game:GetStateSpace();
  local as = self.game:GetActionSpace();
  ACTION_BOUND = as.high[1];
  self.ActorNeuron, self.CriticNeuron = MakeNeuronCriterion(ss.shape[1], as.shape[1]);  
  collectgarbage();
end

function aaacclustenode:SynchronyNeuronnet(aps, apb, cps, cpb)
  local aow = self.ActorNeuron:getParameters();
  arclight.Message2Tensor(aps, apb, aow);
  local cow = self.CriticNeuron:getParameters();
  arclight.Message2Tensor(cps, cpb, cow);
end

function aaacclustenode:SetCentrcenode(node)
  self.centrcenode = node;
end

function aaacclustenode:SetEnvrionment(gamename)
  LOG("Setup game name's " .. gamename);
  self.game:Setup(gamename);
  local as = self.game:GetActionSpace();
  self.policy_gradient = 
  {
    [1] = torch.Tensor(as.shape[1]),
    [2] = torch.Tensor(as.shape[1]),
  }
  self.value_gradient = torch.Tensor(1);
end

local function _GetMuSigma(outtable)
  return outtable[1] * ACTION_BOUND, outtable[2] + TINY_EPSILON;
end

function aaacclustenode:_ChoiceAction(actions, state)
  local outtable = self.ActorNeuron:forward(state);
  local mu, sigma = _GetMuSigma(outtable);
  local action = arclight.sample_normal_distribution(mu[1], sigma[1]);
  action = torch.Tensor({action});
  action = torch.clamp(action, -ACTION_BOUND, ACTION_BOUND)
  --print("mu "..mu[1].." sigma "..sigma[1].." action "..action[1]);  
  table.insert(actions, action);
  
  --convert to table
  local action_t = {}
  for i=1, action:size(1) do
    table.insert(action_t, action[i]);  
  end  
  return action_t;
end

--开始训练
function aaacclustenode:Trainning(its)
  LOG(string.format("Begin trainning iteritor count %d", its));
  local p = IFileSystem:PathAssembly("docs:save/a3c.log");
  self.game:GameStart(p, true, false, false)
  local state = self.game:Reset();  
  local episodereward = 0    
  for episode = 1, its do
    local states = {}
    local rewards = {}
    local actions = {}
    local step = 0;
    local reward = 0;
    local isterminal = false;
    repeat
      step = step + 1;
      local action_t = self:_ChoiceAction(actions, state);
      table.insert(states, state);
      state, reward, isterminal = self.game:GameStep(action_t, true);
      episodereward = episodereward + reward;
      reward = reward * REWARD_SCALE;      
      table.insert(rewards, reward);
      if isterminal or step % UPDATE_GLOBAL_ITER == 0 then
        local valueloss = 0;
        local policyloss = 0
        self.CriticNeuron:zeroGradParameters();
        self.ActorNeuron:zeroGradParameters();
        local R = 0;
        if isterminal then
          LOG("episode reward is "..episodereward);
          episodereward = 0;
          state = self.game:Reset();
        else          
          R = self.CriticNeuron:forward(state)[1];
        end        
        for i=step, 1, -1 do
          local state = states[i];      
          R = rewards[i] + EXPERIENCES_DISCOUNT * R;
          
          --caculate td-error
          local value = self.CriticNeuron:forward(state);
          local td = R - value[1];
          valueloss = valueloss + math.pow(td, 2);
          
          local outtable = self.ActorNeuron:forward(state);
          local mu, sigma = _GetMuSigma(outtable);
          mu = mu[1];
          sigma = sigma[1];          
          local action = actions[i][1];
          
          --derivative of value error (x-y)^2dt(x-y) = 2*(x-y) = -2*td
          self.value_gradient[1] = - 2 * td;
          
          local entropy = 0.5 * math.log(2 * math.pi * 2.718281828459) + math.log(sigma);
          local log_prob = -0.5 * (action - mu)*(action - mu)/(sigma*sigma) - 0.5 * math.log(2 * math.pi) - math.log(sigma);
          policyloss = -(log_prob * td + ENTROPY_BETA * entropy);
          
          local gradmu = -td*(action-mu)/(sigma*sigma);
          local gradsigma = td*(1/sigma - (action-mu)*(action-mu)/(sigma*sigma*sigma)) - ENTROPY_BETA*1/sigma;
             
          self.policy_gradient[1][1] = ACTION_BOUND*gradmu;
          self.policy_gradient[2][1] = gradsigma;  
          
          --print("mu "..mu.." sigma "..sigma.." action "..action.." gradmu "..gradmu);
         
          self.CriticNeuron:backward(state, self.value_gradient);
          self.ActorNeuron:backward(state, self.policy_gradient);
        end
        LOG(string.format("end %d iterator reward is %d td error %.2f policy loss %.2f", episode, episodereward, valueloss, policyloss));
        local asb, abb, csb, cbb = self.centrcenode:ApplyGradient(self:_SerializeGradient());
        self:SynchronyNeuronnet(asb, abb, csb, cbb);
        collectgarbage();
      end      
    until isterminal or step % UPDATE_GLOBAL_ITER == 0;
  end
  self.game:GameStop();
end

function aaacclustenode:_SerializeGradient()
  local _, agw = self.ActorNeuron:getParameters();
  local asb, abb = arclight.Tensor2Message(agw);
  local _, cgw = self.CriticNeuron:getParameters();
  local csb, cbb = arclight.Tensor2Message(cgw);  
  return asb, abb, csb, cbb;
end

--[[ gaussian negative log likelihood
          --policy loss
          local entropy = 0.5 * ( math.log(2*math.pi*sigma) + 1);
          local guassnnl = math.log(sigma) + 0.5 * math.log(2*math.pi) + 0.5 * (action - mu)*(action - mu)*math.exp(-math.log(sigma));
          policyloss = guassnnl * td + ENTROPY_BETA * entropy;
          
          --derivative of policy error
          local gradmu = -td*(action-mu)/sigma;
          local gradsigma = td*(0.5/sigma - (action-mu)*(action-mu)/(2*sigma*sigma)) + ENTROPY_BETA*0.5/sigma;
          
          self.policy_gradient[1][1] = 2*gradmu;
          self.policy_gradient[2][1] = gradsigma; 
          
          print("mu "..mu.." sigma "..sigma.." action "..action.." log_prob "..log_prob.." gradmu "..gradmu);
]]--

--[[ gaussian log pdf
          local entropy = 0.5 * math.log(2 * math.pi * 2.718281828459) + math.log(sigma);
          local log_prob = -0.5 * (action - mu)*(action - mu)/(sigma*sigma) - 0.5 * math.log(2 * math.pi) - math.log(sigma);
          policyloss = log_prob * td + ENTROPY_BETA * entropy;
          
          local gradmu = -td*(action-mu)/(sigma*sigma);
          local gradsigma = td*(1/sigma - (action-mu)*(action-mu)/(sigma*sigma*sigma)) - ENTROPY_BETA*1/sigma;
             
          self.policy_gradient[1][1] = 2*gradmu;
          self.policy_gradient[2][1] = gradsigma;  
          
          print("mu "..mu.." sigma "..sigma.." action "..action.." gradmu "..gradmu);
]]--

return aaacclustenode;