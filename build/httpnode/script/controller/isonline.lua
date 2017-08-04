local json = require ("dkjson")
local luasql = require "luasql.mysql"

local isonline = {};
isonline.games = nil;

function isonline:Initialize()
  self.env = luasql.mysql();
  self.conn = self.env:connect('feedback','feedback','feedback@laravel');
end

function isonline:index(p)
  function rows (connection, sql_statement)
    local cursor = assert (connection:execute (sql_statement))
    return function ()
      return cursor:fetch()
    end
  end
  
  for id, name, degree in rows(self.conn ,"SELECT * from feedback_contents") do
    print(string.format("%d  %s  %f", id, name, degree))
  end
  return res;
end

function isonline:create(p)
end

return isonline;