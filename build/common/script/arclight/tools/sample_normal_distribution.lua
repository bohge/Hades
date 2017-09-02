

local function gen_random_obye_normal_distribution(mu, sigam)
  local function _central_limit_theorem()
    sum = 0;
    for i=1, 12 do
      sum = sum + math.random();
    end
    return sum - 6;
  end
  return mu + _central_limit_theorem()*sigam;    
end

return gen_random_obye_normal_distribution;