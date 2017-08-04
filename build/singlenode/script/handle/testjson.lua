local ffi = require "ffi"
local GenID = require "protocol/msg_generic_id_capnp".MsgGenericID;
local GlobalDataList = require "globaldatalist"
local Dispatcher = require "dispatchhandle";
local DataTable = require "datatable"

local TestJson = {};
function TestJson:Initialize()
    Dispatcher:RegisteHandle( GenID["GID_TEST_ONLY"], self, self.OnGenericJob );
end


function TestJson:OnRequest()
end

function TestJson:OnGenericJob( datas)
    print("TestJson:OnGenericJob()")
    g_Request:ReadonlyAccessor(GlobalDataList.DataTableAccessor.GID, DataTable._HandleSelf)
    g_Request:GetAccessors()
    colum = DataTable:GetColumByIndex("test", "index", 2)
    print(colum.product_id)
end

return TestJson;
