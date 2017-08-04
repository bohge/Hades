--[[ main.lua

     Main code for PacPac, a lua-based pac-man clone.
     There are many pac-man clones. This one is mine.
  ]]
require "utility"
require "torch";
require "dpnn"
require "rnn"
require "nn"

local Character = require('pacman.Character')
local events = require('pacman.events')
local levelreader = require('pacman.levelreader')
local util = require('pacman.util')

--ai reward
reward_ghost_score = 400;
reward_dots_score = 20
reward_superdots_score = 60
reward_alive = -10;
reward_hit_wall = -100;
reward_dead = -400;
reward_scaler = 400;
active_reward = 0;
active_isterminal = false;

--obj defiend
defiend_ghost = -3
defiend_wall = -2
defiend_trunghost = -1
defiend_hero = 0
defiend_road = 1
defiend_dot = 2
defiend_superdot = 3
defiend_weakghost = 4

--state defiend
state_d = 1
state_w = 15
state_h = 15


-------------------------------------------------------------------------------
-- Declare all globals here.
-------------------------------------------------------------------------------

version = "0.301"

map = nil
num_levels = 3

-- This can be 'start screen' or 'playing'.
game_mode = nil

superdots = {} -- Value given below.
num_dots = 0

tile_size = 28

man_x = 10.5
man_y = 17.5
man_dir = {-1, 0}
pending_dir = nil
clock = 0

message = ''
show_message_till = -1
pause_till = -1
game_over = false
super_mode_till = -1
death_anim_till = -1

man = nil  -- A Character object for the hero.
red = nil
characters = {}  -- All moving Character objects = man + ghosts.

ghost_mode = 'scatter'

lives_left = 0
life_start_time = 0
next_music_speedup = -1
score = 0
hi_score = 0
ghost_eaten_scores = {}
next_ghost_score = reward_ghost_score
death_anim_time = 2

jstick = nil
jstick_img = nil
jstick_overlay = nil
keybd_img = nil

logo = nil
large_font = nil
small_font = nil

say_ready_till = 0

-- Sound-related variables.

wata = nil
play_wata_till = -1
weeoo = nil
bwop = nil
death_noise = nil
nomnomz = {}
nomnomz_index = 1
runny = nil
open_noise = nil
thunder = {}
thunder_index = 1

start_song_id = nil

start_lives = 3
dots_at_end = 0
easy_mode = false
is_invincible = false

if easy_mode then
  start_lives = 10
  dots_at_end = 250
end

-- Potential debug settings.
--[[
dots_at_end = -1
is_invincible = true
]]--


-------------------------------------------------------------------------------
-- Non-love functions.
-------------------------------------------------------------------------------

function superdot_eaten()
  for k, c in pairs(characters) do
    if c.shape == 'ghost' then c.eaten = false end
  end
  super_mode_till = clock + 6.0
  add_to_score(reward_superdots_score)  -- An additional +10 is given for every dot.
  next_ghost_score = reward_ghost_score
end

-- Sets ghost_mode to either 'scatter' or 'pursue', based on a 26-second cycle,
-- where the first 6 seconds are scatter, and the next 20 are pursue.
function update_ghost_mode()
  local cycle_point = clock % 26
  if cycle_point < 6 then
    ghost_mode = 'scatter'
  else
    ghost_mode = 'pursue'
  end
end

function pts_hit_by_man_at_xy(x, y)
  local h = 0.45  -- Less than 0.5 to allow turns near intersections.
  local pts = {}
  for dx = -1, 1, 2 do for dy = -1, 1, 2 do
    table.insert(pts, {math.floor(x + dx * h), math.floor(y + dy * h)})
  end end
  return pts
end

-- Returns a hash set of the dot pts nearby, whether or not a dot is there.
function dots_hit_by_man_at_xy(x, y)
  local pts = pts_hit_by_man_at_xy(2 * x + 0.5, 2 * y + 0.5)
  local dots = {}
  for k, v in pairs(pts) do
    local pt = {v[1] / 2, v[2] / 2}
    dots[util.str(pt)] = pt
  end
  return dots
end

function xy_hits_a_wall(x, y, can_pass_hotel_door)
  local pts = pts_hit_by_man_at_xy(x, y)
  for k, v in pairs(pts) do
    if v[1] >= 1 and v[1] <= #map then
      local m = map[v[1]][v[2]]
      if m == 1 then return true end
      if m == 3 and not can_pass_hotel_door then return true end
    end
  end
  return false
end

-- There's a function for this since we might want to play up to 4 overlapping
-- instances at once.
function play_nomnom()
  local n = nomnomz[nomnomz_index]
  nomnomz_index  = (nomnomz_index % 4) + 1
end

function check_for_hit()
  local dead = false;
  for k, character in pairs(characters) do
    if character ~= man and man:dist(character) < 0.5 then
      if character:is_weak() then
        play_nomnom()
        character.dead_till = math.huge
        character.eaten = true
        add_ghost_eaten_score(next_ghost_score, character.x, character.y)
        next_ghost_score = next_ghost_score-- * 2
      else --bohge:dead
        dead = true;
        local x,y = random_road_point();
        man:setPosition(x,y);
        add_to_score(reward_dead);
      end
    end
  end
  if not dead then
    add_to_score(reward_alive);
  end  
end

function add_ghost_eaten_score(points, x, y)
  add_to_score(points)
  local s = {score = points,
             x = (x - 0.4) * tile_size,
             y = (y - 0.3) * tile_size}
  local event_id = nil

  -- Register the score to disappear in 2 seconds.
  function remove_ghost_eaten_score()
    ghost_eaten_scores[event_id] = nil
  end
  event_id = events.add(2, remove_ghost_eaten_score)

  ghost_eaten_scores[event_id] = s
end

function add_to_score(points)
  if points == reward_dead or points >= reward_ghost_score then
    active_isterminal = true;
  end  
  active_reward = active_reward + points;
  score = score + points
  if score > hi_score then hi_score = score end
  --print(string.format("add %d active reward %d total %d", points, active_reward, score));  
end

-- Input is similar to {0, 1}, which would be a request to go right.
function dir_request(dir)
  --是否左右转
  if dir == nil then return end
  if man == nil then return end
  if man:can_turn_right_now(dir) then
    man.dir = dir
  else
    add_to_score(reward_hit_wall);
  end
end

function have_dots(x,y)
  function have_one_dot(x,y)
    return nil ~= dots[util.str({x, y})];
  end
  return have_one_dot(x + 0.5, y + 0.5)
end

function is_superdot(x,y)
  function is_one_superdot(x,y)
    return nil ~= superdots[util.str({x, y})];
  end
  return is_one_superdot(x + 0.5, y + 0.5)
end


-- Inner functions to help find the dot locations.
-- The input x, y is the integer square location in tile coordinates.
function add_dots(x, y)
  if map[x][y] ~= 0 then return end
  add_one_dot(x + 0.5, y + 0.5)
  if x + 1 <= #map and map[x + 1][y] == 0 then
    add_one_dot(x + 1, y + 0.5)
  end
  if y + 1 <= #(map[1]) and map[x][y + 1] == 0 then
    add_one_dot(x + 0.5, y + 1)
  end
end
function add_one_dot(x, y)
  if nil == dots[util.str({x, y})] then
    dots[util.str({x, y})] = {x, y}
    num_dots = num_dots + 1
  end  
end

function random_road_point()
  local maxx = #map;
  local maxy = #(map[1]);
  
  local flag, x, y;
  repeat
    x = math.random(maxx);
    y = math.random(maxy);  
    flag = map[x][y];
  until 0 == flag;
  return x,y;
end

-- Loads the level corresponding to level_num.
function setup_level()
  local filename = 'pacman_' .. level_num .. '.txt'
  level = levelreader.read(filename)
  map = level.map

  show_message_till = 0
  num_dots = 0

  superdots = util.hash_from_list(level.superdots)

  -- This will be a hash set of all dot locations.
  dots = {}

  --for x = 1, #map do for y = 1, #(map[1]) do add_dots(x, y) end end
  generate_points()
  generate_points()
  generate_points()
  generate_points()
  characters = {}
  begin_play()
end

function start_new_game()
  lives_left = start_lives
  game_over = false
  score = 0

  level_num = 1
  setup_level()
  setup_tensor();
end

function setup_characters()
  characters = {}
  man = Character.new('hero', 'yellow')
  table.insert(characters, man)

  red = Character.new('ghost', 'red')
  table.insert(characters, red)

  table.insert(characters, Character.new('ghost', 'pink'))
  table.insert(characters, Character.new('ghost', 'blue'))
  table.insert(characters, Character.new('ghost', 'orange'))
end

function begin_play()
  setup_characters()
  pause_till = 0
  show_message_till = 0
end



function generate_points(dt)
  if num_dots <= 150 then
    local count = math.random(10, 30);
    for i=1, count do
      local x,y = random_road_point();
      add_dots(x,y);
    end
  end  
end

local time_el = 0;
function update_playing(dt)

end

function real_update_playing(dt)
  --bohge:更新
  clock = clock + dt
  update_ghost_mode()
  for k, character in pairs(characters) do
    character:update(dt)
  end
  check_for_hit()
  events.update(dt)
  generate_points()
end

local mixdata;
local collectcount = 2000;
local collectindex;

function keypressed_playing(key)
  --bohge:输入
  active_reward = 0;
  active_isterminal = false;
  --local dirs = {up = {0, -1}, down = {0, 1}, left = {-1, 0}, right = {1, 0}}
  local dirs = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}}
  dir_request(dirs[key])
  real_update_playing(0.25);
  
  --local info = get_pac_state()
  --caculate_mean_std(info);
  active_reward = active_reward / reward_scaler;
  return active_reward, active_isterminal;
end


function get_pac_state_size()
  return state_d, state_w, state_h;
end

function action_count()
  return 4;
end

function caculate_mean_std(info)
  mixdata[{collectindex, {}, {}, {}}] = info;
  if collectindex == collectcount then
    local mean = mixdata[{ {}, {}, {}, {}}]:mean()
    local stdv = mixdata[{ {}, {}, {}, {}  }]:std()
    mixdata[{ {}, {}, {}, {}  }]:add(-mean)    
    mixdata[{ {}, {}, {}, {}  }]:div(stdv)
    print("mead "..mean);
    print("stdv "..stdv);
    preprocess = {mean, stdv};
    local meanstdpath = IFileSystem:PathAssembly("docs:save/pacman_lite.table");
    print("save mean std to "..meanstdpath);
    torch.save(meanstdpath, preprocess);
    
    mean = mixdata[{ {}, {}, {}, {}}]:mean()
    stdv = mixdata[{ {}, {}, {}, {}  }]:std()
    print("new mead "..mean);
    print("new stdv "..stdv);
    --[[for i=1, collectindex do
      local p = mixdata[{i, {}, {}, {}}];
      print(p);
      print("---------------------");
    end]]--
  end
  print(collectindex);
  collectindex = collectindex + 1;
end

function setup_tensor()
  local d,w,h = get_pac_state_size();
  mixdata = torch.FloatTensor(collectcount, d, w, h);
  collectindex = 1;
end

function is_inside_map(x, y)  
  local w = #map
  local h = #(map[1]);
  return x >= 1 and x <= w and y >= 1 and y <= h;
end

function get_pac_state(meanstd)
  local d,w,h = get_pac_state_size();
  local manx = math.floor(man.x);
  local oy = math.floor(man.y);
  local many = math.min(19, math.max(1, oy));
  local beginx = manx - math.floor(w*0.5) - 1;
  local beginy = many - math.floor(h*0.5) - 1;
  
  local info = torch.Tensor(w, h):fill(defiend_wall);
  for iy = 1, h do
    for ix = 1, w do
      local x = beginx + ix;
      local y = beginy + iy;
      if is_inside_map(x, y) then
        local define;
        local gf = map[x][y];
        if gf == 1 or gf == 2 or gf == 3 then
          define = defiend_wall;
        elseif gf == 0 then
          if have_dots(x,y) then
            if is_superdot(x,y) then
              define = defiend_superdot;
            else
              define = defiend_dot;
            end        
          else          
            define = defiend_road;
          end      
        else
          ERROR("error unkonw flag type");
        end
        info[iy][ix] = define;
      end    
    end
  end
  
    --处理角色
  for k, character in pairs(characters) do
    if not character:is_dead() then
      local x = math.floor(character.x);
      local oy = math.floor(character.y);
      local y = math.min(19, math.max(1, oy));
      local ix = x - beginx;
      local iy = y - beginy;
      if ix >=1 and ix <= w
        and iy >= 1 and iy <= h then
        if character.shape == 'hero' then
          info[iy][ix] = defiend_hero;
        else
          if character:is_weak() then
            if character:is_inverted_weak() then
              info[iy][ix] = defiend_trunghost;
            else
              info[iy][ix] = defiend_weakghost;
            end            
          else
            info[iy][ix] = defiend_ghost;
          end
        end
      end  
    end  
  end
  --print(info);
  info[{{},{}}]:add(-meanstd[1]);
  info[{{},{}}]:div(meanstd[2]);
  return info:view(d,w,h);
end