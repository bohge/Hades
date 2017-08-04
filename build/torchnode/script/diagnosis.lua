local image = require "image"
require "torch";

local function _ProcessImages(resize, channel, inpath)
  local restable = {};
  local function _imageload(path, channel, datatype)
    LOG(path);
    local input = image.load(path, channel, datatype);
    return input;
  end
  for file in paths.iterfiles(inpath) do
    local path = inpath .. "/" .. file;
    local status, input = pcall(_imageload, path, 3, 'double');
    if true ~= status then
      ERROR("image "..path.." can not loaded!");
    else
      input = image.scale(input,resize,resize);
      if 1 == channel then
        input = image.rgb2gray(input);
      end
      input = input:view(1, channel, resize, resize);
      table.insert(restable, input);
    end
  end
  return restable;
end

local function net_evaluate(net, meanstd, images, trget)
  local loss = 0;
  local mean = meanstd[1];
  local std = meanstd[2];
  for _, input in ipairs(images) do
    for i=1,3 do
      input[{ {},i,{},{} }]:add(-mean[i])
      input[{ {},i,{},{} }]:div(std[i])
    end
    local restensor = net:forward(input);
    loss = loss + math.abs( trget - restensor[1][1] );
  end
  return loss;
end

function net_diagnosis(resize, channel, porn, normal)
  local meanstd = IFileSystem:PathAssembly("docs:save/preprocess.table");
  meanstd = torch.load(meanstd);  
  local pornimages = _ProcessImages(resize, channel, porn);
  local normalimages = _ProcessImages(resize, channel, normal);
  local inpath = IFileSystem:PathAssembly("docs:save");
  for file in paths.iterfiles(inpath) do
    if string.match(file, ".tensor") then
      local path = inpath .. "/" .. file;
      LOG("load file "..file);
      local net = torch.load(path, "ascii");
      net:evaluate();
      local loss = net_evaluate(net, meanstd, pornimages, 1);
      loss = loss + net_evaluate(net, meanstd, normalimages, 0);
    end
  end
  LOG("done");  
end