local json = require ("dkjson")
local luasql = require "luasql.mysql"
require "utility"

local mysqltools = {};

function mysqltools:Initialize()
  self.env = luasql.mysql();
end

function mysqltools:_Connect()
  local conn = self.env:connect('gs','root','root1234');
  if conn == nil then
    ERROR("database connection failed");
  end
  return conn;
end


function mysqltools:insert(gameid, roleid)
  LOG("insert gameid "..gameid.." roleid "..roleid);
  local res = true;
  local conn = self:_Connect();
  res1 = assert (conn:execute(string.format(" SELECT * from vips where gameid = '%s' and roleid = '%s'", gameid, roleid)))
  if not res1:fetch() then
      res = assert (conn:execute(string.format(" INSERT INTO vips VALUES ('%s', '%s')", gameid, roleid)));
  end
  conn:close();
  return res;
end

function mysqltools:delete(gameid, roleid)
  LOG("delete gameid "..gameid.." roleid "..roleid);
  local conn = self:_Connect();
  res = assert (conn:execute(string.format(" DELETE from vips where gameid = '%s' and roleid = '%s'", gameid, roleid)));
  conn:close();
  return res
end

function mysqltools:GetAllPotential()
  function rows (connection, sql_statement)
    local cursor = assert (connection:execute (sql_statement))
    return function ()
      return cursor:fetch()
    end
  end

  local res = {}
  local conn = self:_Connect();
  for gameid, roleid in rows(conn ,"SELECT * from vips") do
    local gametable = res[gameid];
    if nil == gametable then
      gametable = {}
      res[gameid] = gametable;
    end
    --table.insert(res[gameid],roleid)
    gametable[roleid] = true;
  end
  conn:close();
  return res;
end

return mysqltools;