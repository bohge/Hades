--创建环境对象
luasql = require "luasql.mysql"
local env = luasql.mysql()
assert(env)
print(env)

--连接数据库（参数：数据库名 用户名 密码）
local conn = env:connect('test','root','')
assert(conn)
print(conn)

--操作数据库
conn:execute"SET NAMES GB2312"

--操作数据库文法
function rows (connection, sql_statement)
  local cursor = assert (connection:execute (sql_statement))
  return function ()
    return cursor:fetch()
  end
end

for id, name, degree in rows(conn ,"SELECT * from MyClass") do
    print(string.format("%d  %s  %f", id, name, degree))
end

conn:close()  --关闭数据库连接
env:close()   --关闭数据库环境