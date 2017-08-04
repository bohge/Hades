require "luabit.bit"

local sumtree = torch.class('arclight.sumtree');
sumtree.root = nil; --根节点
sumtree.size = nil; --可用个数
sumtree.capacity = nil; --容量
sumtree.insertindex = nil; --插入索引
sumtree.experiences = nil; --插入数据池


function sumtree:_MakeTree(parent, me, depth)
  me.available = false; --是否可访问
  me.priority = 0;
  me.parent = parent;
  if self.capacity == 2^depth then
    me.exp = {};
    local len = #self.experiences;
    me.index = len + 1;
    table.insert(self.experiences, me);
  else
    me.left = {};
    me.right = {};
    self:_MakeTree(me, me.left, depth+1);
    self:_MakeTree(me, me.right, depth+1);
  end
end

function sumtree:Initialization(capacity)
  LOG("sum tree's capacity is "..capacity);
  assert(bit.band(capacity, capacity-1) == 0);--power of 2
  self.capacity = capacity;
  self.root = {};
  self.size = 0;
  self.experiences = {};
  self.insertindex = 1;
  self:_MakeTree(nil, self.root, 0);
end

function sumtree:_IterateUpdate(node, diffpriority)
  node.available = true;
  node.priority = node.priority + diffpriority;
  if node.parent then
    self:_IterateUpdate(node.parent, diffpriority);
  end  
end

function sumtree:_UpdateNode(node, priority)
  local diffpriority = priority - node.priority;
  self:_IterateUpdate(node, diffpriority);
end

function sumtree:UpdatePriority(indices, prioritys)
  assert( #indices == #prioritys );
  for i=1, #indices do
    local index = indices[i];
    local node = self.experiences[index];
    local priority = prioritys[i];
    self:_UpdateNode(node, priority);
  end 
end

function sumtree:InsertExperience(priority, current_state, next_state, action, reward, isterminal)
  assert(priority >= 0);
  priority = priority == 0 and 1e-5 or priority;
  local node = self.experiences[self.insertindex];
  node.exp.reward = reward;
  node.exp.action = action;
  node.exp.isterminal = isterminal;
  node.exp.next_state = next_state;
  node.exp.current_state = current_state;
  self:_UpdateNode(node, priority);
  self.insertindex = self.insertindex + 1 > self.capacity and 1 or self.insertindex + 1;
  self.size = self.size + 1 > self.capacity and self.capacity or self.size + 1;
end

function sumtree:TotalPriority()
  return self.root.priority ~= 0 and self.root.priority or 1;
end

function sumtree:_IterateAvaliable(node)
  if not node.left.available and not node.right.available then
    node.available = false;        
    if node.parent then
      self:_IterateAvaliable(node.parent);
    end 
  end 
end


function sumtree:_Retrieve(node, seed)
  assert(node.available == true);
  if node.exp then
    node.available = false;
    self:_IterateAvaliable(node.parent);
    --LOG(string.format("i%d\t%.6f\t%.4f",node.index,node.priority,node.exp.reward));
    return node.index, node.exp;
  end
  if node.left.priority >= seed and node.left.available or not node.right.available then
    return self:_Retrieve(node.left, seed);
  else
    return self:_Retrieve(node.right, seed - node.left.priority);
  end  
  return nil;
end

function sumtree:Sampling(count)
  local indices = {}
  local experience = {}
  local interval = self.root.priority / count;
  for c=1, count do
    local seed = math.random();
    seed = interval * (c + seed - 1);
    local index, exp = self:_Retrieve(self.root, seed);
    table.insert(indices, index);
    table.insert(experience, exp);
  end
  return indices,experience;
end

function sumtree:Size()
  return self.size;
end

return sumtree;