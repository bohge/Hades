local GymClient = require "gym_http_api.gym_http_client"
require 'torch';


local base = 'http://127.0.0.1:5000';
local openaigym = torch.class('arclight.openaigym');


function openaigym:Setup(gamename)
  self.client = GymClient.new(base);
  self.instance_id =  self.client:env_create(gamename);
end

function openaigym:GetActionSpace()
  return self.client:env_action_space_info(self.instance_id);
end

function openaigym:GetStateSpace()
  return self.client:env_observation_space_info(self.instance_id);
end

function openaigym:GameStart(outdir, force, resume, video_callable)
  self.client:env_monitor_start(self.instance_id, outdir, force, resume, video_callable);
end

function openaigym:GameStop()
  self.client:env_monitor_close(self.instance_id);
end

function openaigym:Reset()
  local ts = self.client:env_reset(self.instance_id)
  return torch.Tensor(ts);
end

function openaigym:GameStep(action, render)
  local ts, reward, done = self.client:env_step(self.instance_id, action, render)
  return torch.Tensor(ts), reward, done;
end

function openaigym:RandomAction()
  return self.client:env_action_space_sample(self.instance_id);
end


return openaigym;

