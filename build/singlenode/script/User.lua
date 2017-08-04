

user = {};
local LocalData = {};
Chilen = {}
Chilen.test = 100;

function Chilen:Testfunc()
  print(self.test);
end



function user:Initialize(machine, instance)
  self.HandleLocalData = machine:RegisteInstance(LocalData);
  
  self.HandleCreateUser = instance:SignFuncitonWithReturn(TNative, self.InitializeUser, TBool, TString);
  self.HandleRequestUser = instance:SignFuncitonWithReturn(TNative, self.CalculatetUser, TBool, TString);
end

function user:CreateUser()
  Accessor:CreateAccessor(self.HandleLocalData);
  return self.HandleCreateUser;
end

function user:RequestUser()
  requestcode = tonumber(stringcode);
  Accessor:ReadwriteAccessor(requestcode, self.HandleLocalData);
  return self.HandleRequestUser;
end

function user:InitializeUser(issucces)
  if issucces then
    LocalData.Data = 0;
    LocalData.Chiled = Chilen;
    LocalData.Chiled.CC = {};
    LocalData.Chiled.CC.Test = 200;
    LocalData.Position = vector3(1,2,3);    
    strres = tostring(LocalData._GID)
    .. ";"
    .. tostring(LocalData._UUID)
    .. ";"
    .. tostring(LocalData.Data);
    Client:Sendback(strres);
  end
  return nil;
end

function user:CalculatetUser(issucces)
  if issucces then
    LocalData.Data = LocalData.Data + 1;
    strres = tostring(LocalData._GID)
    .. ";"
    .. tostring(LocalData._UUID)
    .. ";"
    .. tostring(LocalData.Data);
    Client:Sendback(strres);
  end
  return nil;
end