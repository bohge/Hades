require "utility"

_DEBUG = false;

if _DEBUG then
  local debuger = require "debug/mobdebug";
  debuger.start();
  _COROUTINES_ON = debuger.on;
  _COROUTINES_OFF = debuger.off;
else
  _COROUTINES_ON = function() end;
  _COROUTINES_OFF = function() end;
end
  

nativelibrary("nativesystem");

local main = {};

require "pacman"
require "dqntester"
--require "torchdemo"
require "arclight"
--require "gnuplot"
--require "torchx";
require "torch";
--require "cutorch";
require "dpnn"
require "rnn"
require "nn"
--require "cunn"
--require "ul_serial"
--require "rnndemo.train"
require "imageprocess"
require "diagnosis"
local sumtree = arclight.sumtree;
local argparse = require "argparse";
local net = require "arclight.centrcenode_pron_new";

local function trainnet()
  opt = {
    vocabfile = IFileSystem:PathAssembly("docs:data/vocab.t7"),
    datafile = IFileSystem:PathAssembly("docs:data/train.t7"),
    savefile = IFileSystem:PathAssembly("docs:save/lstm_model.t7"),
    batch_size = 16,
    seq_length = 16,
    rnn_size = 256,
    max_epochs = 1,
    save_every = 100,
    print_every = 10,
    seed = 85423357};
  
  _COROUTINES_ON();
  lstmtrain(opt);
  _COROUTINES_OFF();
end

function main:_CmdLineInitialize()
  self.cmd = argparse("script", "command line");
  self.cmd:argument("input", "Input file.")
  self.cmd:option("-o --output", "Output file.", "a.out")
  self.cmd:option("-I --include", "Include locations."):count("*")
end

function main:Initialize()
  _COROUTINES_ON();
  local seed = 851022;
  local stepcounts = 1000;
  dqntester("docs:dqn/dqn_cnn_1", seed, stepcounts);
  dqntester("docs:dqn/ddodqn_cnn_1", seed, stepcounts);
  dqntester("docs:dqn/ddodqn_cnn_2", seed, stepcounts);
  dqntester("docs:dqn/ddodqn_cnn_3", seed, stepcounts);
  
  LOG("\n\n\n************************************************");
  stepcounts = 3000;
  dqntester("docs:dqn/dqn_cnn_1", seed, stepcounts);
  dqntester("docs:dqn/ddodqn_cnn_1", seed, stepcounts);
  dqntester("docs:dqn/ddodqn_cnn_2", seed, stepcounts);
  dqntester("docs:dqn/ddodqn_cnn_3", seed, stepcounts);
  
  LOG("\n\n\n************************************************");
  stepcounts = 5000;
  dqntester("docs:dqn/dqn_cnn_1", seed, stepcounts);
  dqntester("docs:dqn/ddodqn_cnn_1", seed, stepcounts);
  dqntester("docs:dqn/ddodqn_cnn_2", seed, stepcounts);
  dqntester("docs:dqn/ddodqn_cnn_3", seed, stepcounts);
  
  --neuronmaker = IFileSystem:ReadFile("comm:script/arclight/neuronnets/pac_dueling_cnn_lite_1_15_15.lua");
  --loadstring(neuronmaker)();
  --neuronnet, criterion = MakeNeuronCriterion();
  --local t = torch.Tensor(1,15,15);
  --local o = neuronnet:forward(t);
  --print(o);
  --torch.manualSeed(851022);
  --[[sumtree:Initialization(2^6);
  sumtree:InsertExperience(1, 1, 1, 1, 1, 1);
  sumtree:InsertExperience(5, 2, 2, 2, 2, 2);
  sumtree:InsertExperience(2, 3, 3, 3, 3, 3);
  sumtree:InsertExperience(1, 4, 4, 4, 4, 4);
  sumtree:InsertExperience(1, 5, 5, 5, 5, 5);
  sumtree:InsertExperience(1, 5, 5, 5, 5, 5);
  sumtree:InsertExperience(1, 5, 5, 5, 5, 5);
  sumtree:InsertExperience(1, 5, 5, 5, 5, 5);
  sumtree:InsertExperience(10, 5, 5, 5, 5, 5);
  sumtree:InsertExperience(1, 5, 5, 5, 5, 5);
  sumtree:InsertExperience(1, 5, 5, 5, 5, 5);
  sumtree:InsertExperience(1, 5, 5, 5, 5, 5);
  sumtree:InsertExperience(1, 5, 5, 5, 5, 5);
  sumtree:InsertExperience(1, 5, 5, 5, 5, 5);
  sumtree:InsertExperience(1, 5, 5, 5, 5, 5);
  sumtree:InsertExperience(1, 5, 5, 5, 5, 5);
  sumtree:InsertExperience(1, 5, 5, 5, 5, 5);
  sumtree:InsertExperience(1, 5, 5, 5, 5, 5);
  sumtree:InsertExperience(1, 5, 5, 5, 5, 5);
  sumtree:InsertExperience(1, 5, 5, 5, 5, 5);
  sumtree:InsertExperience(1, 5, 5, 5, 5, 5);
  sumtree:InsertExperience(1, 5, 5, 5, 5, 5);
  sumtree:InsertExperience(1, 5, 5, 5, 5, 5);
  sumtree:InsertExperience(1, 5, 5, 5, 5, 5);
  sumtree:InsertExperience(1, 5, 5, 5, 5, 5);
  sumtree:InsertExperience(1, 5, 5, 5, 5, 5);
  sumtree:InsertExperience(1, 5, 5, 5, 5, 5);
  sumtree:InsertExperience(1, 5, 5, 5, 5, 5);
  sumtree:InsertExperience(1, 5, 5, 5, 5, 5);
  sumtree:InsertExperience(1, 5, 5, 5, 5, 5);
  sumtree:InsertExperience(1, 5, 5, 5, 5, 5);
  sumtree:InsertExperience(1, 5, 5, 5, 5, 5);
  sumtree:InsertExperience(1, 5, 5, 5, 5, 5);
  sumtree:InsertExperience(1, 5, 5, 5, 5, 5);
  sumtree:InsertExperience(1, 5, 5, 5, 5, 5);
  sumtree:InsertExperience(1, 5, 5, 5, 5, 5);
  sumtree:InsertExperience(1, 5, 5, 5, 5, 5);
  sumtree:InsertExperience(1, 5, 5, 5, 5, 5);
  sumtree:InsertExperience(1, 5, 5, 5, 5, 5);
  sumtree:InsertExperience(1, 5, 5, 5, 5, 5);
  sumtree:InsertExperience(1, 5, 5, 5, 5, 5);
  sumtree:InsertExperience(1, 5, 5, 5, 5, 5);
  
  local indices,experience = sumtree:Sampling(16);]]--
  --self:_CmdLineInitialize();
  --torch.test();
  --nn.test();
  --cunn.test();
  --dpnn.test();
  --rnn.test();
  --torchx.test();
  --cutorch.test();
  --torchdemo.run();
  --torchdemo.test();
  --net:Setup();
  --net:Load();
  --net:Verification();
  --net_diagnosis(96,3,"C:/Users/zhaopeng_wj/Desktop/sp", "C:/Users/zhaopeng_wj/Desktop/sn");
  --[[resizeImage(96, 3,"E:/head/original/censored","E:/head/intermediate/censored");
  resizeImage(96, 3,"E:/head/original/pron","E:/head/intermediate/pron");
  process_batch(10, 0.01, 3, 96,
          "E:/head/intermediate/censored",
          "E:/head/intermediate/pron",
          --"E:/head/test/censored",
          --"E:/head/test/pron",
          "E:/head/tensor/preprocess.table",
          "E:/head/tensor/trainset/",
          "E:/head/tensor/verifyset/");
          --"E:/head/test/preprocess.table",
          --"E:/head/test/trainset/",
          --"E:/head/test/verifyset/");]]--
  --test2("E:/head/head/tensor/unqualified.tensor", "E:/head/head/tensor");
  --self.t = torch.Tensor(2000000, 1, 21, 19);
  _COROUTINES_OFF();
end

function main:OnCommandline(args)
  _COROUTINES_ON();
  --self.cmd:parse(args);
  if "w" == args then
    keypressed_playing("up");
  elseif "a" == args then
    keypressed_playing("left");
  elseif "s" == args then
    keypressed_playing("down");
  elseif "d" == args then
    keypressed_playing("right");
  end  
  local t = get_info_map();
  print(t);
  _COROUTINES_OFF();
end

return main;