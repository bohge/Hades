



local function Testrequest()
  local A = g_Request:CreateAccessor();
  local B = g_Request:CreateAccessor();
  local C = g_Request:CreateAccessor();
  g_Request:GetAccessors();
  A.test = 100;
  B.test = 200;
  C.test = 300;
  local A_GID = A._GID;
  local B_GID = B._GID;
  local C_GID = C._GID;
  g_Request:FreeAccessors(A_GID);
  g_Request:FreeAccessors(B_GID);
  g_Request:FreeAccessors(C_GID);
  
  
  --测试创建读取
  local A1 = g_Request:ReadonlyAccessorImmediately(A_GID);
  local B1 = g_Request:ReadonlyAccessorImmediately(B_GID);
  local C1 = g_Request:ReadonlyAccessorImmediately(C_GID);
  assert(100 == A1.test);
  assert(200 == B1.test);
  assert(300 == C1.test);
  g_Request:FreeAccessors(A_GID);
  g_Request:FreeAccessors(B_GID);
  g_Request:FreeAccessors(C_GID);
  
  --测试只读读取写入数据无效化
  local A2 = g_Request:ReadonlyAccessorImmediately(A_GID);
  assert(100 == A2.test);
  A2.test = 200;
  g_Request:FreeAccessors(A_GID);
  local A3 = g_Request:ReadonlyAccessorImmediately(A_GID);
  assert(100 == A3.test);
  g_Request:FreeAccessors(A_GID);
  
  --测试嵌套读取
  local A4 = g_Request:ReadwriteAccessorImmediately(A_GID);
  assert(100 == A4.test);
  A4.test = 200;
  local B4 = g_Request:ReadwriteAccessorImmediately(B_GID);
  assert(200 == A4.test);
  B4.test = 300;
  local C4 = g_Request:ReadonlyAccessorImmediately(C_GID);
  C4.test = 400;
  g_Request:FreeAccessors(C_GID);
  A4.temp = 1000;
  g_Request:FreeAccessors(B_GID);
  B4.temp = 2000;
  g_Request:FreeAccessors(A_GID);
  
  local A5 = g_Request:ReadonlyAccessorImmediately(A_GID);
  local B5 = g_Request:ReadonlyAccessorImmediately(B_GID);
  local C5 = g_Request:ReadonlyAccessorImmediately(C_GID);
  assert(200 == A5.test);
  assert(1000 == A5.temp);
  assert(300 == B5.test);
  assert(nil == B5.temp);
  assert(300 == C5.test);
  g_Request:FreeAccessors(A_GID);
  g_Request:FreeAccessors(B_GID);
  g_Request:FreeAccessors(C_GID);
  
  --测试提升读取权限
  local A6 = g_Request:ReadonlyAccessorImmediately(A_GID);
  A6.test = 300;
  local A7 = g_Request:ReadwriteAccessorImmediately(A_GID);
  assert(200 == A6.test);
  A7.test = 400;  
  g_Request:FreeAccessors(A_GID);
  g_Request:FreeAccessors(A_GID);
  
  local A8 = g_Request:ReadonlyAccessorImmediately(A_GID);
  assert(400 == A8.test);
  g_Request:FreeAccessors(A_GID);
  
  --测试一对一释放
  local A9 = g_Request:ReadwriteAccessorImmediately(A_GID);
  local A10 = g_Request:ReadonlyAccessorImmediately(A_GID);
  assert(400 == A10.test);
  g_Request:FreeAccessors(A_GID);
  A9.test = 500;
  g_Request:FreeAccessors(A_GID);
  
  local A11 = g_Request:ReadonlyAccessorImmediately(A_GID);
  assert(500 == A11.test);
  g_Request:FreeAccessors(A_GID);
  
  --测试instance接口
  local I = {};
  I._HandleSelf = g_Machine:RegisteInstance(I);
  g_Request:CreateAccessor(I._HandleSelf);
  g_Request:GetAccessors();
  local I_GID = I._GID;
  I.test = 5;
  g_Request:FreeAccessors(I_GID);
  
  g_Request:ReadwriteAccessor(I_GID, I._HandleSelf);
  g_Request:GetAccessors();
  assert(5 == I.test);
  I.test = 6;
  g_Request:FreeAccessors(I_GID);
  
  g_Request:ReadonlyAccessor(I_GID, I._HandleSelf);
  g_Request:GetAccessors();
  assert(6 == I.test);
  g_Request:FreeAccessors(I_GID);
  
  --测试无instance读取
  g_Request:ReadonlyAccessor(I_GID, I._HandleSelf);
  g_Request:GetAccessors();
  assert(6 == I.test);
  local I1 = g_Request:ReadwriteAccessorImmediately(I_GID);
  assert(6 == I1.test);
  I1.test = 7;
  g_Request:FreeAccessors(I_GID);
  g_Request:FreeAccessors(I_GID);
  
  --测试错误的instance读取
  local I2 = g_Request:ReadwriteAccessorImmediately(I_GID);
  assert(7 == I2.test);  
  g_Request:ReadwriteAccessor(I_GID, I._HandleSelf);--在设计理念上，此处应产生error
  g_Request:GetAccessors();
  assert(7 == I.test);
  g_Request:FreeAccessors(I_GID);
  g_Request:FreeAccessors(I_GID);
  
  
end


return Testrequest;