local GlobalDataList = require "globaldatalist"

local DataTable = {}
require "os"
require "io"
require "socket"
require "utility/json"

DataTable.Tables = {test = 0,scene = 1 }

function DataTable:new( o )
    o = o or {};
    self.__index = self;    
    setmetatable(o,self);

    return o;   
end

function DataTable:Initialize()
    self._HandleSelf = g_Machine:RegisteInstance(self)
    self.RootPath = IFileSystem:PathAssembly("docs:jsontable/")
    print(self.RootPath)
end

function DataTable:OpenAll()
    for k, v in pairs(self.Tables) do
        self.Tables[k] = json.load(self.RootPath ..k..".json")
    end
end

function DataTable:GetRowByIndex(tableKey, index, value)    
    for _, v in pairs(self.Tables[tableKey]) do        
        if v[index] == value then            
            return v
        end
    end
end
function DataTable:GetRowCount(tablename)
    return #self.Tables[tablename];
end
function DataTable:GetSceneConfig(sceneid)
    local SceneConfig = {};    
    local newtable = g_Request:ReadonlyAccessorImmediately(GlobalDataList.DataTableAccessor.GID);     
    local Data  = DataTable:new(newtable);
    local TableName = "scene";    
    
    local Row = Data:GetRowByIndex(TableName, "ID", sceneid)
    SceneConfig.ID = Row.ID;
    SceneConfig.Desc = Row.Desc;
    SceneConfig.Name = Row.Name;
    SceneConfig.MonsterPositions = {};
    table.insert(SceneConfig.MonsterPositions,self:CastStringToVector3(Row.MonsterPos1)) ;
    table.insert(SceneConfig.MonsterPositions,self:CastStringToVector3(Row.MonsterPos2)) ;
    table.insert(SceneConfig.MonsterPositions,self:CastStringToVector3(Row.MonsterPos3)) ;
    -- Print_Table(SceneConfig);
    return SceneConfig;
end
function DataTable:CastStringToVector3( str)
    
    t={}
    for w in string.gmatch(str,"([^',']+)") do     --按照“,”分割字符串        
        table.insert(t,w) 
    end
    local Vector = {}
    Vector.posx = t[1];
    Vector.posy = t[2];
    Vector.posz = t[3];
    return Vector;

end

return DataTable
