
--全局的保存accid的一个表
local GlobalDataList = {};

--c++数据的句柄
GlobalDataList._HandleInstance = nil;

--玩家队列
GlobalDataList.PlayerListAccessor = {};
GlobalDataList.PlayerListAccessor.GID = 0;
GlobalDataList.PlayerListAccessor.ObjMgrGID = 0;
GlobalDataList.PlayerListAccessor.RoomGID = 0;

--DataTable
GlobalDataList.DataTableAccessor = {}
GlobalDataList.DataTableAccessor.GID = 0


GlobalDataList.RoomInst = {}

function GlobalDataList:Initialize()
  self._HandleInstance = g_Machine:RegisteInstance(GlobalDataList, "GlobalDataList");
end

return GlobalDataList;