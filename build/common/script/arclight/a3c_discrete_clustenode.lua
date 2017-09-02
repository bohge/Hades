local distributions = require "distributions"
require "arclight";
require "utility"
require "torch";
require "dpnn"
require "nn"

local TINY_EPSILON = 1e-10;
local ENTROPY_BETA = 0.001;
local REWARD_SCALE = 0.1;
local EXPERIENCES_DISCOUNT = 0.9;


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
  self.ActorNeuron, self.CriticNeuron = MakeNeuronCriterion(ss.shape[1], as.n);  
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
  self.policy_gradient = torch.Tensor(as.n);
  self.value_gradient = torch.Tensor(1);
end

function aaacclustenode:_ChoiceAction(actions, state)
  local probability = self.ActorNeuron:forward(state);
  local action = torch.multinomial(probability, 1):squeeze()
  table.insert(actions, action);
  return action - 1; -- because different start of index, so there minus one to mapping to game space
end

--开始训练
function aaacclustenode:Trainning(its)
  LOG("Begin trainning iteritor count ".. its);
  local p = IFileSystem:PathAssembly("docs:save/a3c.log");
  self.game:GameStart(p, true, false, false)
  for episode = 1, its do
    local states = {}
    local rewards = {}
    local actions = {}
    local episodereward = 0
    local state = self.game:Reset();
    local step = 0;
    local reward = 0;
    local isterminal = false;    
    repeat
      step = step + 1;
      local action_g = self:_ChoiceAction(actions, state);
      table.insert(states, state);
      state, reward, isterminal = self.game:GameStep(action_g, true);
      episodereward = episodereward + reward;
      reward = reward * REWARD_SCALE;
      table.insert(rewards, reward);
      if isterminal then
        local valueloss = 0;
        local policyloss = 0
        self.CriticNeuron:zeroGradParameters();
        self.ActorNeuron:zeroGradParameters();
        local R = 0;
        for i=step, 1, -1 do
          local state = states[i];
          local action = actions[i];
          
          R = rewards[i] + EXPERIENCES_DISCOUNT * R;
          
          local value = self.CriticNeuron:forward(state);
          local probability = self.ActorNeuron:forward(state);
          
          --caculate td-error
          local td = R - value[1];
          valueloss = valueloss + td * td;
          
          --entropy = - p * log(p);
          local entropy = - ( probability[action] * torch.log(probability[action]) );
          --policy loss = log(p)*td + beta * entropy;
          policyloss = torch.log(probability[action]) * td + ENTROPY_BETA * entropy;
          
          self.value_gradient[1] = - 0.5 * td;          
          -- ∇θ logp(s) = 1/p(a) for chosen a, 0 otherwise
          self.policy_gradient:zero()
          -- f(s) ∇θ logp(s)
          self.policy_gradient[action] = -td / probability[action] -- Negative target for gradient descent
          
          -- Calculate (negative of) gradient of entropy of policy (for gradient descent): -(-logp(s) - 1)
          local gradEntropy = torch.log(probability) + 1
          -- Add to target to improve exploration (prevent convergence to suboptimal deterministic policy)
          self.policy_gradient:add(ENTROPY_BETA, gradEntropy)
          
          self.CriticNeuron:backward(state, self.value_gradient);
          self.ActorNeuron:backward(state, self.policy_gradient);
        end
        LOG(string.format("end %d iterator rewards %.0f td error %.2f policy loss %.2f", episode, episodereward, valueloss, policyloss));
        local asb, abb, csb, cbb = self.centrcenode:ApplyGradient(self:_SerializeGradient());
        self:SynchronyNeuronnet(asb, abb, csb, cbb);
        collectgarbage();
      end
    until isterminal;
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

return aaacclustenode;