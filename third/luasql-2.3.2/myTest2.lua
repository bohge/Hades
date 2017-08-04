-- 示例脚本功能：创建一个名为“sample”的表

luasql = require "luasql.mysql"

--创建环境对象
local env  = luasql.mysql()
assert(env)
print(env)

--连接数据库（参数：数据库名 用户名 密码）
local conn = env:connect('test','root','')
assert(conn)
print(conn)

--操作数据库
status,errorString = conn:execute([[CREATE TABLE sample (id INTEGER, name TEXT)]])
print(status,errorString )

status,errorString = conn:execute([[INSERT INTO sample values('12','Raj')]])
print(status,errorString )

cursor,errorString = conn:execute([[select * from sample]])
print(cursor,errorString)

row = cursor:fetch ({}, "a")
while row do
  print(string.format("Id: %s, Name: %s", row.id, row.name))
  row = cursor:fetch (row, "a")
end

-- close everything
cursor:close()
conn:close()
env:close()