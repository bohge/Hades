require("torch")
require("nn")

nnsparse = {}

include('src/SparseTools.lua')

include('src/SDAECriterion.lua')
include('src/SDAESparseCriterion.lua')
include('src/MaskCriterion.lua')
include('src/SparseCriterion.lua')

include('src/SparseLinearBatch.lua')

include('src/SparseSorting.lua')
include('src/Batchifier.lua')
include('src/DynamicSparseTensor.lua')



return nnsparse
