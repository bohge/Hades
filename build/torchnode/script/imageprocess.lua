local image = require "image"
require "torch";

--独立存储为单独的tensor数据
function process_single(inpath, outpath)
  local list = {};
  local pics = paths.dir(inpath);
  for _, file in ipairs(pics) do
    local fullin = inpath .. "/" .. file;
    if string.find(fullin, ".jpg") then
      print("process ".. file);
      table.insert(list, file);
      local input = image.load(fullin,3,'double');
      input = image.scale(input,resize,resize);
      local fullout = outpath .. "/" .. file;
      torch.save(fullout, input);
      --image.save(fullout, input);
    end    
  end
  torch.save(outpath.. "/list.table", list);
  print(inpath .. " done");
end

--打包为一个完整的tensor
function process_full(inpath, outfile)
  local list = {};
  local pics = paths.dir(inpath);
  local restensor = nil;
  local index = 0;
  for _, file in ipairs(pics) do
    local fullin = inpath .. "/" .. file;
    if string.find(fullin, ".jpg") then
      print("process ".. file);
      table.insert(list, file);
      local input = image.load(fullin,3,'double');
      input = image.scale(input,resize,resize);
      if nil == restensor then
        restensor = input;
      else
        restensor = restensor:cat(input, 1);
      end
      index = index + 1;
    end    
  end
  restensor = restensor:view(index,3,resize,resize);
  torch.save(outfile, restensor);
  print(inpath .. " done");
end

local function _imageload(path, channel, datatype)
  local input = image.load(path, channel, datatype);
  return input;
end

--针对ResNet网络需要将N个图片打包在一个文件中，验证集单纯增加一个维度
function resizeImage(resize, channel, inpath, outpath)
  local index = 1;
  LOG("Resize images");
  for file in paths.iterfiles(inpath) do
    local path = inpath .. "/" .. file;
    local fullout = outpath .. "/" .. index .. ".tensor";
    local imageout = outpath .. "/" .. index .. ".jpg";
    if paths.filep(fullout) then
      LOG("file exits "..fullout);
    else
      --local input = image.load(path,3,'float');
      local status, input = pcall(_imageload, path, 3, 'float');
      if true ~= status then
        ERROR("image "..path.." can not loaded!");
      else
        input = image.scale(input,resize,resize);
        if 1 == channel then
          input = image.rgb2gray(input);
        end
        input = input:view(channel, resize, resize);
        LOG("load "..path.." save to "..fullout);
        torch.save(fullout, input);
        --image.save(imageout, input);
      end
    end
    index = index + 1;
  end
  LOG("Work done count "..index);
end

local function _findfiles(filetragets, traget, path)
  for file in paths.iterfiles(path) do
    local t = {};
    t.path = path.."/"..file;
    t.traget = traget;
    table.insert(filetragets, t);
  end
end

local function _splittable(input, count)
  local res = {};
  local inputsize = #input;
  for i=1, inputsize, count do
    local remain = inputsize - i + 1;
    remain = remain > count and count or remain;
    local t = {};
    table.insert(res, t);
    for j=1, remain do
      local index = i + j - 1;
      table.insert(t, input[index]);
    end
  end
  return res;
end

local function _shuffletable(input)
  local inputsize = #input;
  local shuffle = torch.randperm(inputsize, 'torch.LongTensor');
  local res = {}
  for i=1, inputsize do
    table.insert(res, input[shuffle[i]]);
  end
  return res;
end

local function _loadfiles(channel, size, filestable)
  local inputsize = #filestable;
  local mixdata = torch.LongTensor(inputsize,channel,size,size):float();
  local mixlable = torch.LongTensor(inputsize):byte();
  for i, filetarget in ipairs(filestable) do
    LOG("load "..filetarget.path);
    local input = torch.load(filetarget.path);
    mixdata[i]:copy(input);
    mixlable[i] = filetarget.traget;
  end
  return mixdata, mixlable;
end


--stdv近似
local function _caculatemeanstdv(channel, size, splittables, meanstdpath)
  local mean = {};
  local stdv = {};
  for i=1,channel do
    mean[i] = 0;
    stdv[i] = 0;
  end  
  for _, t in ipairs(splittables) do
    mixdata = _loadfiles(channel, size, t);
    for i=1,channel do
      mean[i] = mean[i] + mixdata[{ {},i,{},{} }]:mean()
      stdv[i] = stdv[i] + mixdata[{ {},i,{},{} }]:std()
    end
  end
  for i=1,channel do
    mean[i] = mean[i] / #splittables;
    stdv[i] = stdv[i] / #splittables;
    LOG("channel " .. i .. "mean " .. mean[i] .. " stdv " .. stdv[i]);
  end
  local preprocess = {mean, stdv};
  torch.save(meanstdpath, preprocess);
  return mean, stdv;
end

local function _splittableset(splittables, verifyprecit)
  local fullsize = #splittables;
  local verifysize = math.ceil(fullsize * verifyprecit);
  local trainsize = fullsize - verifysize;
  local trainset = {}
  local verifyset = {}
  for i=1, trainsize do
    table.insert(trainset, splittables[i]);
  end
  for i=trainsize+1, fullsize do
    table.insert(verifyset, splittables[i]);
  end
  return trainset, verifyset;
end

local function _savefiles(tableset, channel, size, mean, stdv, outpath)
  local filelist={};
  for i, t in ipairs(tableset) do
    outputdata, outputlabel = _loadfiles(channel, size, t);
    for i=1,channel do
    -- normalize each channel globally:
      outputdata[{ {},i,{},{} }]:add(-mean[i])
      outputdata[{ {},i,{},{} }]:div(stdv[i])
    end    
    local filename = i .. ".float.tensor";
    local fullout = outpath .. filename;
    local tosave ={outputdata,outputlabel};
    torch.save(fullout, tosave);
    LOG("save "..fullout);
    table.insert(filelist, filename);
  end
    local info = {
    ["size"]=#tableset,
    ["list"]=filelist };
  local fullout = outpath .. "data.info";
  torch.save(fullout, info);
end

local function _savetoimage(mean, std, channel, size, infile, outpath)
  local datatable = torch.load(infile);
  local imagetensor = datatable[1];
  local label = datatable[2];
  for i=1,channel do
    imagetensor[{ {},i,{},{} }]:mul(std[i])
    imagetensor[{ {},i,{},{} }]:add(mean[i])
  end
  local tensortable = imagetensor:split(1,1);
  for index, pic in ipairs(tensortable) do
    pic = pic:view(channel,size,size);
    local fullout = outpath .. "/" .. index .."_".. label[index] .. ".jpg";
    image.save(fullout, pic);
  end
end

--针对ResNet网络需要将N个图片打包在一个文件中，验证集单纯增加一个维度
function process_batch(batchsize, verifyprecit, channel, size, censoredpath, unqualifiedpath, meanstdpath, trainpath, verifypath)
  local mixfiles = {};
  local filetragets = {};
  LOG("discover tensors");
  _findfiles(filetragets, 0, censoredpath);
  _findfiles(filetragets, 1, unqualifiedpath);
  local fullsize = #filetragets;
  
  LOG("shuff and split tensors");
  filetragets = _shuffletable(filetragets);
  local splittables = _splittable(filetragets, batchsize);
  
  LOG("caculate mean and stdv");
  mean, stdv = _caculatemeanstdv(channel, size, splittables, meanstdpath);
  
  LOG("split into trainingset and verifyset");
  trainingset, verifyset = _splittableset(splittables, verifyprecit);
  
  LOG("batch and save trainingset");
  _savefiles(trainingset, channel, size, mean, stdv, trainpath);
  
  LOG("batch and save verifyset");
  _savefiles(verifyset, channel, size, mean, stdv, verifypath);
  
  LOG("resave test");
  _savetoimage(mean, stdv, channel, size, trainpath.."1.float.tensor", "E:/head/test/resave");
  
  LOG("done!");
end



