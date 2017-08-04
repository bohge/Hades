require('torch')
require('nn')


arclight = {}

arclight.commandparser = require('arclight.tools.commandline')
arclight.stringsplit = require('arclight.tools.stringsplit')
arclight.Tensor2Message = require('arclight.tools.tensormessage')
arclight.Message2Tensor = require('arclight.tools.messagetensor')
arclight.sumtree = require('arclight.tools.sumtree')

require('arclight.clustertorch.Image')
require('arclight.clustertorch.Module')
require('arclight.clustertorch.Container')
require('arclight.clustertorch.BatchNormalization')




return arclight;