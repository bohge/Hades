-- Generated by lua-capnproto 0.1.3-1 on Fri Aug 12 16:14:44 2016
-- https://github.com/cloudflare/lua-capnproto.git


local ffi = require "ffi"
local capnp = require "capnp"
local bit = require "bit"

local ceil              = math.ceil
local write_struct_field= capnp.write_struct_field
local read_struct_field = capnp.read_struct_field
local read_text         = capnp.read_text
local write_text        = capnp.write_text
local get_enum_val      = capnp.get_enum_val
local get_enum_name     = capnp.get_enum_name
local get_data_off      = capnp.get_data_off
local write_listp_buf   = capnp.write_listp_buf
local write_structp_buf = capnp.write_structp_buf
local write_structp     = capnp.write_structp
local read_struct_buf   = capnp.read_struct_buf
local read_listp_struct = capnp.read_listp_struct
local read_list_data    = capnp.read_list_data
local write_list        = capnp.write_list
local write_list_data   = capnp.write_list_data
local ffi_new           = ffi.new
local ffi_string        = ffi.string
local ffi_cast          = ffi.cast
local ffi_copy          = ffi.copy
local ffi_fill          = ffi.fill
local ffi_typeof        = ffi.typeof
local band, bor, bxor = bit.band, bit.bor, bit.bxor

local pint8    = ffi_typeof("int8_t *")
local pint16   = ffi_typeof("int16_t *")
local pint32   = ffi_typeof("int32_t *")
local pint64   = ffi_typeof("int64_t *")
local puint8   = ffi_typeof("uint8_t *")
local puint16  = ffi_typeof("uint16_t *")
local puint32  = ffi_typeof("uint32_t *")
local puint64  = ffi_typeof("uint64_t *")
local pbool    = ffi_typeof("uint8_t *")
local pfloat32 = ffi_typeof("float *")
local pfloat64 = ffi_typeof("double *")


local ok, new_tab = pcall(require, "table.new")

if not ok then
    new_tab = function (narr, nrec) return {} end
end

local round8 = function(size)
    return ceil(size / 8) * 8
end

local str_buf
local default_segment_size = 4096

local function get_str_buf(size)
    if size > default_segment_size then
        return ffi_new("char[?]", size)
    end

    if not str_buf then
        str_buf = ffi_new("char[?]", default_segment_size)
    end
    return str_buf
end

-- Estimated from #nodes, not accurate
local _M = new_tab(0, 7)


_M.MsgValueType = {
    ["VT_STRING"] = 0,
    ["VT_BOOL"] = 1,
    ["VT_NUMBER"] = 2,
    ["VT_UINT64"] = 3,
    ["VT_TABLE"] = 4,

}

_M.MsgValueTypeStr = {
    [0] = "VT_STRING",
    [1] = "VT_BOOL",
    [2] = "VT_NUMBER",
    [3] = "VT_UINT64",
    [4] = "VT_TABLE",

}

_M.MsgValueData = {
    id = "15442444624389996504",
    displayName = "msg_generic_job.capnp:MsgValueData",
    dataWordCount = 2,
    pointerCount = 1,
    discriminantCount = 5,
    discriminantOffset = 1,
    field_count = 6,

    fields = {
        { name = "valuetype", default = "vtString", ["type"] = "enum" },
        { name = "stringvalue", default = "", ["type"] = "text" },
        { name = "boolvalue", default = 0, ["type"] = "bool" },
        { name = "numbervalue", default = 0, ["type"] = "float64" },
        { name = "uint64value", default = 0, ["type"] = "uint64" },
        { name = "tablevalue", default = "opaque pointer", ["type"] = "list" },
    },

    calc_size_struct = function(data)
        local size = 24
        local value
        -- text
        value = data["stringvalue"]
        if type(value) == "string" then
            -- size 1, including trailing NULL
            size = size + round8(#value + 1)
        end
        -- list
        if data["tablevalue"] and type(data["tablevalue"]) == "table" then
            size = size + 8
            local num2 = #data["tablevalue"]
            for i2=1, num2 do
                size = size + _M.MsgKeyValuePair.calc_size_struct(data["tablevalue"][i2])
            end
        end
        return size
    end,

    calc_size = function(data)
        local size = 16 -- header + root struct pointer
        return size + _M.MsgValueData.calc_size_struct(data)
    end,

    flat_serialize = function(data, p32, pos)
        pos = pos and pos or 24 -- struct size in bytes
        local start = pos
        local dscrm
        local value

        value = data["valuetype"]
        if type(value) == "string" or type(value) == "number" then
            local val = get_enum_val(value, 0, _M.MsgValueType, "MsgValueData.valuetype")
            write_struct_field(p32, val, "uint16", 16, 0)
        end

        value = data["stringvalue"]
        if value then
            dscrm = 0
        end

        if type(value) == "string" then
            local data_off = get_data_off(_M.MsgValueData, 0, pos)

            local len = #value + 1
            write_listp_buf(p32, _M.MsgValueData, 0, 2, len, data_off)

            ffi_copy(p32 + pos / 4, value)
            pos = pos + round8(len)
        end

        value = data["boolvalue"]
        if value then
            dscrm = 1
        end

        local data_type = type(value)
        if (data_type == "number"
                or data_type == "boolean" ) then

            write_struct_field(p32, value, "bool", 1, 32, 0)
        end

        value = data["numbervalue"]
        if value then
            dscrm = 2
        end

        local data_type = type(value)
        if (data_type == "number"
                or data_type == "boolean" ) then

            write_struct_field(p32, value, "float64", 64, 1, 0)
        end

        value = data["uint64value"]
        if value then
            dscrm = 3
        end

        local data_type = type(value)
        if (data_type == "number"
                or data_type == "boolean" or data_type == "cdata") then

            write_struct_field(p32, value, "uint64", 64, 1, 0)
        end

        value = data["tablevalue"]
        if value then
            dscrm = 4
        end

        if type(value) == "table" then
            local data_off = get_data_off(_M.MsgValueData, 0, pos)
            pos = pos + write_list(p32 + _M.MsgValueData.dataWordCount * 2 + 0 * 2,
                    value, (data_off + 1) * 8, "list", "struct", _M.MsgKeyValuePair)
        end
        if dscrm then
            --buf, discriminantOffset, discriminantValue
            _M.MsgValueData.which(p32, 1, dscrm)
        end

        return pos - start + 24
    end,

    serialize = function(data, p8, size)
        if not p8 then
            size = _M.MsgValueData.calc_size(data)

            p8 = get_str_buf(size)
        end
        ffi_fill(p8, size)
        local p32 = ffi_cast(puint32, p8)

        -- Because needed size has been calculated, only 1 segment is needed
        p32[0] = 0
        p32[1] = (size - 8) / 8

        -- skip header
        write_structp(p32 + 2, _M.MsgValueData, 0)

        -- skip header & struct pointer
        _M.MsgValueData.flat_serialize(data, p32 + 4)

        return ffi_string(p8, size)
    end,

    which = function(buf, offset, n)
        if n then
            -- set value
            write_struct_field(buf, n, "uint16", 16, offset)
        else
            -- get value
            return read_struct_field(buf, "uint16", 16, offset)
        end
    end,

    parse_struct_data = function(p32, data_word_count, pointer_count, header,
            tab)

        local s = tab

        local dscrm = _M.MsgValueData.which(p32, 1)
        -- enum
        local val = read_struct_field(p32, "uint16", 16, 0)
        s["valuetype"] = get_enum_name(val, 0, _M.MsgValueTypeStr)

        -- union
        if dscrm == 0 then

        -- text
        local off, size, num = read_listp_struct(p32, header, _M.MsgValueData, 0)
        if off and num then
            -- dataWordCount + offset + pointerSize + off
            local p8 = ffi_cast(pint8, p32 + (2 + 0 + 1 + off) * 2)
            s["stringvalue"] = ffi_string(p8, num - 1)
        else
            s["stringvalue"] = nil
        end

        else
            s["stringvalue"] = nil
        end

        -- union
        if dscrm == 1 then

        s["boolvalue"] = read_struct_field(p32, "bool", 1, 32, 0)

        else
            s["boolvalue"] = nil
        end

        -- union
        if dscrm == 2 then

        s["numbervalue"] = read_struct_field(p32, "float64", 64, 1, 0)

        else
            s["numbervalue"] = nil
        end

        -- union
        if dscrm == 3 then

        s["uint64value"] = read_struct_field(p32, "uint64", 64, 1, 0)

        else
            s["uint64value"] = nil
        end

        -- union
        if dscrm == 4 then

        -- list
        local off, size, num = read_listp_struct(p32, header, _M.MsgValueData, 0)
        if off and num then
            -- dataWordCount + offset + pointerSize + off
            s["tablevalue"] = read_list_data(p32 + (2 + 0 + 1 + off) * 2, header,
                    num, "struct", _M.MsgKeyValuePair)
        else
            s["tablevalue"] = nil
        end

        else
            s["tablevalue"] = nil
        end

        return s
    end,

    parse = function(bin, tab)
        if #bin < 16 then
            return nil, "message too short"
        end

        local header = new_tab(0, 4)
        local p32 = ffi_cast(puint32, bin)
        header.base = p32

        local nsegs = p32[0] + 1
        header.seg_sizes = {}
        for i=1, nsegs do
            header.seg_sizes[i] = p32[i]
        end
        local pos = round8(4 + nsegs * 4)
        header.header_size = pos / 8
        p32 = p32 + pos / 4

        if not tab then
            tab = new_tab(0, 8)
        end
        local off, dw, pw = read_struct_buf(p32, header)
        if off and dw and pw then
            return _M.MsgValueData.parse_struct_data(p32 + 2 + off * 2, dw, pw,
                    header, tab)
        else
            return nil
        end
    end,

}

_M.MsgKeyValuePair = {
    id = "12292853797542747387",
    displayName = "msg_generic_job.capnp:MsgKeyValuePair",
    dataWordCount = 0,
    pointerCount = 2,
    discriminantCount = 0,
    discriminantOffset = 0,
    field_count = 2,

    fields = {
        { name = "key", default = "", ["type"] = "text" },
        { name = "value", default = "opaque pointer", ["type"] = "struct" },
    },

    calc_size_struct = function(data)
        local size = 16
        local value
        -- text
        value = data["key"]
        if type(value) == "string" then
            -- size 1, including trailing NULL
            size = size + round8(#value + 1)
        end
        -- struct
        value = data["value"]
        if type(value) == "table" then
            size = size + _M.MsgValueData.calc_size_struct(value)
        end
        return size
    end,

    calc_size = function(data)
        local size = 16 -- header + root struct pointer
        return size + _M.MsgKeyValuePair.calc_size_struct(data)
    end,

    flat_serialize = function(data, p32, pos)
        pos = pos and pos or 16 -- struct size in bytes
        local start = pos
        local dscrm
        local value

        value = data["key"]
        if type(value) == "string" then
            local data_off = get_data_off(_M.MsgKeyValuePair, 0, pos)

            local len = #value + 1
            write_listp_buf(p32, _M.MsgKeyValuePair, 0, 2, len, data_off)

            ffi_copy(p32 + pos / 4, value)
            pos = pos + round8(len)
        end

        value = data["value"]
        if type(value) == "table" then
            local data_off = get_data_off(_M.MsgKeyValuePair, 1, pos)
            write_structp_buf(p32, _M.MsgKeyValuePair, _M.MsgValueData, 1, data_off)
            local size = _M.MsgValueData.flat_serialize(value, p32 + pos / 4)
            pos = pos + size
        end
        return pos - start + 16
    end,

    serialize = function(data, p8, size)
        if not p8 then
            size = _M.MsgKeyValuePair.calc_size(data)

            p8 = get_str_buf(size)
        end
        ffi_fill(p8, size)
        local p32 = ffi_cast(puint32, p8)

        -- Because needed size has been calculated, only 1 segment is needed
        p32[0] = 0
        p32[1] = (size - 8) / 8

        -- skip header
        write_structp(p32 + 2, _M.MsgKeyValuePair, 0)

        -- skip header & struct pointer
        _M.MsgKeyValuePair.flat_serialize(data, p32 + 4)

        return ffi_string(p8, size)
    end,

    parse_struct_data = function(p32, data_word_count, pointer_count, header,
            tab)

        local s = tab

        -- text
        local off, size, num = read_listp_struct(p32, header, _M.MsgKeyValuePair, 0)
        if off and num then
            -- dataWordCount + offset + pointerSize + off
            local p8 = ffi_cast(pint8, p32 + (0 + 0 + 1 + off) * 2)
            s["key"] = ffi_string(p8, num - 1)
        else
            s["key"] = nil
        end

        -- struct
        local p = p32 + (0 + 1) * 2 -- p32, dataWordCount, offset
        local off, dw, pw = read_struct_buf(p, header)
        if off and dw and pw then
            if not s["value"] then
                s["value"] = new_tab(0, 2)
            end
            _M.MsgValueData.parse_struct_data(p + 2 + off * 2, dw, pw, header, s["value"])
        else
            s["value"] = nil
        end

        return s
    end,

    parse = function(bin, tab)
        if #bin < 16 then
            return nil, "message too short"
        end

        local header = new_tab(0, 4)
        local p32 = ffi_cast(puint32, bin)
        header.base = p32

        local nsegs = p32[0] + 1
        header.seg_sizes = {}
        for i=1, nsegs do
            header.seg_sizes[i] = p32[i]
        end
        local pos = round8(4 + nsegs * 4)
        header.header_size = pos / 8
        p32 = p32 + pos / 4

        if not tab then
            tab = new_tab(0, 8)
        end
        local off, dw, pw = read_struct_buf(p32, header)
        if off and dw and pw then
            return _M.MsgKeyValuePair.parse_struct_data(p32 + 2 + off * 2, dw, pw,
                    header, tab)
        else
            return nil
        end
    end,

}

_M.MsgGenericMessage = {
    id = "11131339593117287763",
    displayName = "msg_generic_job.capnp:MsgGenericMessage",
    dataWordCount = 1,
    pointerCount = 1,
    discriminantCount = 0,
    discriminantOffset = 0,
    field_count = 2,

    fields = {
        { name = "id", default = "gidInvaid", ["type"] = "enum" },
        { name = "pairs", default = "opaque pointer", ["type"] = "list" },
    },

    calc_size_struct = function(data)
        local size = 16
        local value
        -- list
        if data["pairs"] and type(data["pairs"]) == "table" then
            size = size + 8
            local num2 = #data["pairs"]
            for i2=1, num2 do
                size = size + _M.MsgKeyValuePair.calc_size_struct(data["pairs"][i2])
            end
        end
        return size
    end,

    calc_size = function(data)
        local size = 16 -- header + root struct pointer
        return size + _M.MsgGenericMessage.calc_size_struct(data)
    end,

    flat_serialize = function(data, p32, pos)
        pos = pos and pos or 16 -- struct size in bytes
        local start = pos
        local dscrm
        local value

        value = data["id"]
        if type(value) == "string" or type(value) == "number" then
            local val = get_enum_val(value, 0, _M.MsgGenericID, "MsgGenericMessage.id")
            write_struct_field(p32, val, "uint16", 16, 0)
        end

        value = data["pairs"]
        if type(value) == "table" then
            local data_off = get_data_off(_M.MsgGenericMessage, 0, pos)
            pos = pos + write_list(p32 + _M.MsgGenericMessage.dataWordCount * 2 + 0 * 2,
                    value, (data_off + 1) * 8, "list", "struct", _M.MsgKeyValuePair)
        end
        return pos - start + 16
    end,

    serialize = function(data, p8, size)
        if not p8 then
            size = _M.MsgGenericMessage.calc_size(data)

            p8 = get_str_buf(size)
        end
        ffi_fill(p8, size)
        local p32 = ffi_cast(puint32, p8)

        -- Because needed size has been calculated, only 1 segment is needed
        p32[0] = 0
        p32[1] = (size - 8) / 8

        -- skip header
        write_structp(p32 + 2, _M.MsgGenericMessage, 0)

        -- skip header & struct pointer
        _M.MsgGenericMessage.flat_serialize(data, p32 + 4)

        return ffi_string(p8, size)
    end,

    parse_struct_data = function(p32, data_word_count, pointer_count, header,
            tab)

        local s = tab

        -- enum
        local val = read_struct_field(p32, "uint16", 16, 0)
        s["id"] = get_enum_name(val, 0, _M.MsgGenericIDStr)

        -- list
        local off, size, num = read_listp_struct(p32, header, _M.MsgGenericMessage, 0)
        if off and num then
            -- dataWordCount + offset + pointerSize + off
            s["pairs"] = read_list_data(p32 + (1 + 0 + 1 + off) * 2, header,
                    num, "struct", _M.MsgKeyValuePair)
        else
            s["pairs"] = nil
        end

        return s
    end,

    parse = function(bin, tab)
        if #bin < 16 then
            return nil, "message too short"
        end

        local header = new_tab(0, 4)
        local p32 = ffi_cast(puint32, bin)
        header.base = p32

        local nsegs = p32[0] + 1
        header.seg_sizes = {}
        for i=1, nsegs do
            header.seg_sizes[i] = p32[i]
        end
        local pos = round8(4 + nsegs * 4)
        header.header_size = pos / 8
        p32 = p32 + pos / 4

        if not tab then
            tab = new_tab(0, 8)
        end
        local off, dw, pw = read_struct_buf(p32, header)
        if off and dw and pw then
            return _M.MsgGenericMessage.parse_struct_data(p32 + 2 + off * 2, dw, pw,
                    header, tab)
        else
            return nil
        end
    end,

}

_M.MsgGenericID = {
    ["GID_INVAID"] = 0,
    ["GID_UPDATE_INITIALIZE"] = 1,
    ["GID_BROADCAST_POSITION"] = 2,
    ["GID_ROOM_TICK"] = 3,
    ["GID_ROOM_BROADCAST_MSG_TO_PLAYERS"] = 4,
    ["GID_BROADCAST_MSG_TO_OHTER_PLAYERS"] = 5,
    ["GID_TEST_DATA_TABLE"] = 6,

}

_M.MsgGenericIDStr = {
    [0] = "GID_INVAID",
    [1] = "GID_UPDATE_INITIALIZE",
    [2] = "GID_BROADCAST_POSITION",
    [3] = "GID_ROOM_TICK",
    [4] = "GID_ROOM_BROADCAST_MSG_TO_PLAYERS",
    [5] = "GID_BROADCAST_MSG_TO_OHTER_PLAYERS",
    [6] = "GID_TEST_DATA_TABLE",

}

return _M
