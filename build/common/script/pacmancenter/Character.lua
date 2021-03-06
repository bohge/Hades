-- Character.lua
--
-- A class to work with the hero and ghosts.
--
-- Use like this:
--     local Character = require('Character')
--
local interval = 20;
local util = require('pacman.util')

-- TODO Remove this once we have more confidence the bug is fixed.
function ask_to_report_error()
  s = 'Please report this error and version number: ' .. version
  s = s .. ' on this thread: '
  s = s .. 'https://github.com/tylerneylon/pacpac/issues/7'
  ERROR(s)
end

local Character = {} ; Character.__index = Character

-- shape is 'hero' or 'ghost'; color is in {'red', 'pink', 'blue', 'orange'}.
function Character.new(shape, color)
  local c = setmetatable({shape = shape, color = color}, Character)
  c.is_fake = false
  c:reset()
  return c
end

function Character:is_dead()
  return self.dead_till > clock
end

function Character:is_weak()
  return super_mode_till > clock and not self.eaten
end

function Character:setPosition(x,y)
  self.x = x + 0.5;
  self.y = y + 0.5;
  self.lx = self.x;
  self.ly = self.y;
  self.dir = {0, 0}
end

function Character:reset()
  self.dead_till = -1
  self.mode = 'normal'  -- Can be 'freemove' for ghosts through the hotel door.
  self.eaten = false  -- To avoid ghosts being double eaten.
  self.past_turns = {}
  local start_pos = level.start_pos[self.color]
  self.x = start_pos[1]
  self.y = start_pos[2]
  self.lx = self.x;
  self.ly = self.y;
  if self.shape == 'hero' then
    self.dir = {-1, 0}
  else
    -- It's a ghost.
    if self.color == 'red' then
      self.dir = {1, 0}
      self.exit_time = math.huge
    elseif self.color == 'pink' then
      self.dir = {0, 0}
      self.exit_time = clock + 6
    elseif self.color == 'blue' then
      self.dir = {0, 0}
      self.exit_time = clock + 12
    elseif self.color == 'orange' then
      self.dir = {0, 0}
      self.exit_time = clock + 18
    end
  end
end

function Character:speed()
  local hotel_pos = level.ghost_hotel.outside
  if self.shape == 'hero' then return 4 end
  if self:is_dead() then
    -- The dead move fast, except near the hotel so they don't miss the door.
    if self:dist_to_pt(hotel_pos) < 1 then return 4 end
    return 8
  end
  if self:is_weak() then
    return 2.5
  else
    return 4
  end
end

function Character:target()
  local hotel = level.ghost_hotel
  if self.shape == 'hero' then return {} end
  if self:is_dead() then return hotel.inside end
  if self.mode == 'freemove' then return hotel.outside end
  if self:is_weak() then
    return {math.random() * 19, math.random() * 22}
  end
  if self.color == 'red' then
    if ghost_mode == 'scatter' then return {18.5, 2.5} end
    if ghost_mode == 'pursue' then
      return {man.x, man.y}
    end
  elseif self.color == 'pink' then
    if ghost_mode == 'scatter' then return {2.5, 2.5} end
    if ghost_mode == 'pursue' then
      return {man.x + man.dir[1] * 2, man.y + man.dir[2] * 2}
    end
  elseif self.color == 'blue' then
    if ghost_mode == 'scatter' then return {18.5, 21.5} end
    if ghost_mode == 'pursue' then
      local v1 = {man.x + man.dir[1], man.y + man.dir[2]}
      local v2 = {v1[1] - red.x, v1[2] - red.y}
      return {v1[1] + v2[1], v1[2] + v2[2]}
    end
  elseif self.color == 'orange' then
    local default = {2.5, 21.5}
    if ghost_mode == 'scatter' then return default end
    if ghost_mode == 'pursue' then
      local dist_v = {self.x - man.x, self.y - man.y}
      local dist_sq = dist_v[1] * dist_v[1] + dist_v[2] * dist_v[2]
      if dist_sq > 16 then
        return {man.x, man.y}
      else
        return default
      end
    end
  end
end

-- TODO Is this needed anymore? Check.
function Character:snap_into_place()
  if self.dir[1] == 0 then
    self.x = math.floor(2 * self.x + 0.5) / 2
  end
  if self.dir[2] == 0 then
    self.y = math.floor(2 * self.y + 0.5) / 2
  end
end

function Character:can_go_in_dir(dir)
  if dir == nil then return false end
  local new_x, new_y = self.x + dir[1], self.y + dir[2]
  local can_pass_hotel_door = (self.mode == 'freemove' or self:is_dead())
  return not xy_hits_a_wall(new_x, new_y, can_pass_hotel_door)
end

-- Only returns true for u-turns or when pac-pac is at a grid point, which is
-- common when he is stopped.
function Character:can_turn_right_now(dir)
  if not self:can_go_in_dir(dir) then return false end

  -- Allow legit turns at grid points.
  if self.x - math.floor(self.x) == 0.5 and
     self.y - math.floor(self.y) == 0.5 then
    return true
  end

  -- Allow u-turns anywhere.
  for i = 1, 2 do
    if dir[i] ~= -self.dir[i] then return false end
  end
  return true
end

function Character:turn_score(dir)
  local target = self:target()
  local target_dir = {target[1] - self.x, target[2] - self.y}
  local score = target_dir[1] * dir[1] + target_dir[2] * dir[2]
  local past = self.past_turns[util.str({self.x, self.y})]
  if past and util.str(past.dir) == util.str(dir) then
    score = score - 5 * past.times
  end
  return score
end

-- Returns available directions, skipping u-turns unless it's the only choice.
-- This is for use by ghosts.
function Character:available_dirs()
  local turn = {self.dir[2], self.dir[1]}
  local turns = {}
  if self:can_go_in_dir(self.dir) then turns = {self.dir} end
  for sign = -1, 1, 2 do
    local t = {turn[1] * sign, turn[2] * sign}
    if self:can_go_in_dir(t) then table.insert(turns, t) end
  end
  if #turns == 0 then table.insert(turns, {-self.dir[1], -self.dir[2]}) end
  return turns
end

-- Switch self.dir to dir if it is more aligned with getting to the target.
function Character:turn_if_better(turn)
  if self:turn_score(turn) > self:turn_score(self.dir) then
    self.dir = turn
    self.last_turn = {self.x, self.y}
  end
end

function Character:next_grid_point()
  local pt = {self.x, self.y}

  -- TODO Remove this once we have more confidence it doesn't happen.
  local num_bad = 0
  for i = 1, 2 do
    if pt[i] - math.floor(pt[i]) ~= 0.5 then num_bad = num_bad + 1 end
  end
  if num_bad == 2 then
    ERROR('Error: bad coordinate <' .. self.shape .. '> (' .. self.x .. ', ' .. self.y .. ')')
    ask_to_report_error()
  end

  for i = 1, 2 do
    -- This if block is more readable than a short but opaque mathy summary.
    if self.dir[i] == 1 then
      pt[i] = math.floor(pt[i] + 0.5) + 0.5
    elseif self.dir[i] == -1 then
      pt[i] = math.ceil(pt[i] - 0.5) - 0.5
    elseif self.dir[i] ~= 0 then
      ERROR('Error: Unexpected dir value (' .. self.dir[i] .. ') found.')
      ask_to_report_error()
    end
  end
  return pt
end

function Character:update(dt)
  if pause_till > clock then return end

  -- Check if it's time for a ghost to exit the ghost hotel.
  if self.shape == 'ghost' and self.exit_time < clock then

    -- TODO Remove this once we have more confidence it doesn't happen.
    local pt = {self.x, self.y}
    local num_bad = 0
    for i = 1, 2 do
      if pt[i] - math.floor(pt[i]) ~= 0.5 then num_bad = num_bad + 1 end
    end
    if num_bad > 0 then
      ERROR('Error: ghost turn triggered when not on a grid point')
      ERROR('Current position=(' .. self.x .. ', ' .. self.y .. ')')
      ERROR('Current dir=(' .. self.dir[1] .. ', ' .. self.dir[2] .. ')')
      ask_to_report_error()
    end
    
    self.mode = 'freemove'
    self.exit_time = math.huge
    self.dir = {0, -1}
  end

  local movement = dt * self:speed()
  while movement > 0 do
    -- This is inside the loop because the hero can hit a wall and stop.
    if self.dir[1] == 0 and self.dir[2] == 0 then break end
    
    self.lx = self.x;
    self.ly = self.y;    

    local pt = self:next_grid_point()
    local dist = self:dist_to_pt(pt)
    if dist <= movement then
      self.x, self.y = pt[1], pt[2]
      self:reached_grid_point()
    else
      self.x = self.x + self.dir[1] * movement
      self.y = self.y + self.dir[2] * movement
    end
    movement = movement - dist  -- May end up below 0; that's ok.
  end

  self:check_for_side_warps()
  self:register_dots_eaten()
end

function Character:reached_grid_point()
  if self.x < 1 or self.x > (#map + 1) then return end

  if self.shape == 'hero' then
    if not self:can_go_in_dir(self.dir) then
      self.dir = {0, 0}
    end
  end

  if self.shape == 'ghost' then

    -- Handle movements around the ghost hotel door.
    local can_pass_hotel_door = (self.mode == 'freemove' or self:is_dead())
    local t = self:target()
    if can_pass_hotel_door and self.x == t[1] and self.y == t[2] then
      if self:is_dead() then
        self.dir = {0, -1}
        self.dead_till = clock
        self.mode = 'freemove'
        self.eaten = true
      else
        self.mode = 'normal'
      end
    end

    -- Handle standard movements (not into/out of the ghost hotel).
    local dirs = self:available_dirs()
    self.dir = dirs[1]
    for k, t in pairs(dirs) do self:turn_if_better(t) end
    self:register_turn()
  end
end

-- Records that we turned to self.dir at self.xy.
function Character:register_turn()
  local key = util.str({self.x, self.y})
  local value = self.past_turns[key]
  if value and util.str(value.dir) == util.str(self.dir) then
    value.times = value.times + 1
  else
    self.past_turns[key] = {dir = self.dir, times = 1}
  end
end

-- If a character is far to the left right, they jump across the map.
function Character:check_for_side_warps()--bohge:这个必须修改有bug
  if self.x <= 0.5 then
    self.x = #map + 0.5
    self.dir = {-1, 0}
    self.lx = self.x;
  elseif self.x >= #map + 1.5 then
    self.x = 1.5
    self.dir = {1, 0}
    self.lx = self.x;
  end
end

function Character:register_dots_eaten()
  local invers = 1 / interval;
  if self.shape ~= 'hero' then return end
  local ix = (self.x - self.lx) * invers;
  local iy = (self.y - self.ly) * invers;
  for i=1,interval do
    local px = self.lx + i * ix;
    local py = self.ly + i * iy;
    local dots_hit = dots_hit_by_man_at_xy(px, py)
    for k, v in pairs(dots_hit) do
      if dots[k] then
        if superdots[k] then superdot_eaten() end
        dots[k] = nil
        num_dots = num_dots - 1
        add_to_score(reward_dots_score)
        play_wata_till = clock + 0.2
        if num_dots <= dots_at_end then
          generate_points()
        end
      end
    end    
  end  
end

function Character:draw_death_anim()
  local t = death_anim_time - (death_anim_till - clock)
  local start, stop = 1.0, 2 * math.pi
  local erase_time = 0.5
  -- From time_left, map 3 -> start, 1 -> stop.
  local angle = (t / erase_time) * (stop - start) + start
  if angle > stop then
    if (death_anim_time - t) < 1.1 then
      love.graphics.setLineWidth(1)
      draw.setColor(255, 255, 255)
      local n = 7
      local r1, r2 = 0.15 * tile_size, 0.3 * tile_size
      for a = 0, 2 * math.pi, 2 * math.pi / n do
        local c, s = math.cos(a), math.sin(a)
        local x, y = self.x * tile_size, self.y * tile_size
        love.graphics.line(c * r1 + x, s * r1 + y,
                           c * r2 + x, s * r2 + y)
      end
    end
    return
  end
  draw.setColor(255, 255, 0)
  local offset = math.atan2(-1, 0)
  local r = 0.45
  love.graphics.arc('fill', self.x * tile_size, self.y * tile_size,
                    tile_size * r,
                    offset + angle / 2,
                    offset + 2 * math.pi - angle / 2, 16)
end

function Character:is_inverted_weak()
  local time_left = super_mode_till - clock
  return time_left < 2;
end


function Character:dist_to_pt(pt)
  local dist_v = {self.x - pt[1], self.y - pt[2]}
  -- Using L1 makes it easier to survive close-pursuit turns.
  return math.abs(dist_v[1]) + math.abs(dist_v[2])
end

function Character:dist(other)
  if other:is_dead() then return math.huge end
  function i_dist_to_pt(pt)
    local dist = math.huge;
    local invers = 1 / interval;
    local ix = (self.x - self.lx) * invers;
    local iy = (self.y - self.ly) * invers;
    for i=1,interval do
      local px = self.lx + i * ix;
      local py = self.ly + i * iy;
      
      local dist_v = {px - pt[1], py - pt[2]}
      -- Using L1 makes it easier to survive close-pursuit turns.
      local ndist = math.abs(dist_v[1]) + math.abs(dist_v[2])
      dist = dist > ndist and ndist or dist;
    end 
    return dist;
  end
  
  local dist = math.huge;
  local invers = 1 / interval;
  local ix = (other.x - other.lx) * invers;
  local iy = (other.y - other.ly) * invers;
  for i=1,interval do
    local px = other.lx + i * ix;
    local py = other.ly + i * iy;
    local ndist = i_dist_to_pt({px,py});
    dist = dist > ndist and ndist or dist;
  end
  return dist;
end

return Character
