nativelibrary("nativesystem");
nativelibrary("application");

Client = {};
Client.count = 0;

function Client:Initialize()
  self.Caculate = IServicesSystem:Create("Caculate");
  return true;
end

function Client:OnReciveRequest(gid, buffer)
  print("OnReciveRequest");
  self.Caculate:Send(gid, buffer);
  --g_Server:SendMessage(gid, buffer);
  --Client.count = self.Caculate:Add(1,Client.count);
  print(self.Caculate.Test);
end


function Client:OnTimerTriggered()	
 print("OnTimerTriggered");
end


