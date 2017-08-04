-- Generated by lua-capnproto 0.1.3-1 on Fri Aug 12 16:14:45 2016
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
local _M = new_tab(0, 2)


_M.MsgMessageID = {
    ["MID_LOGIN"] = 0,
    ["MID_LOGOUT"] = 1,
    ["MID_CONFIRM_DELAY"] = 2,
    ["MID_GENERIC_JOB"] = 3,
    ["MID_PLAYER_FACE"] = 4,
    ["MID_PLAYER_FACE_RESULT"] = 5,
    ["MID_PLAYER_MOVE_MEET_OBSTACLE"] = 6,
    ["MID_PLAYER_MOVE_MEET_OBSTACLE_RESULT"] = 7,
    ["MID_PLAYER_MOVE_START"] = 8,
    ["MID_PLAYER_MOVE_START_RESULT"] = 9,
    ["MID_PLAYER_MOVE_STOP"] = 10,
    ["MID_PLAYER_MOVE_STOP_RESULT"] = 11,
    ["MID_TIMEVALUE"] = 12,
    ["MID_MSG_ENTRY_SCENE_REQUEST"] = 13,
    ["MID_MSG_BROADCAST_PLAYER_INFO"] = 14,
    ["MID_MSG_LEAVE_SCENE"] = 15,
    ["MID_MSG_BROADCAST_PLAYER_LEAVE"] = 16,
    ["MIN_MSG_PLAYER_VIVE_DEVICE_INFO"] = 17,
    ["MIN_MSG_PLAYER_VIVE_DEVICE_INFO_RESULT"] = 18,
    ["MID_MSG_PLAYER_FIRE"] = 19,
    ["MID_MSG_PLAYER_FIRE_RESULT"] = 20,
    ["MID_MSG_PLAYER_RELOAD"] = 21,
    ["MID_MSG_PLAYER_RELOAD_RESULT"] = 22,
    ["MID_MSG_PLAYER_CHANGE_WEAPON"] = 23,
    ["MID_MSG_PLAYER_CHANGE_WEAPON_RESULT"] = 24,
    ["MID_MSG_PLAYER_REVIVE"] = 25,
    ["MID_MSG_PLAYER_REVIVE_RESULT"] = 26,
    ["MID_MSG_PLAYER_PROJECTILE_DAMAGE"] = 27,
    ["MID_MSG_PLAYER_PROJECTILE_DAMAGE_RESULT"] = 28,
    ["MID_MSG_PVP_GAME_START_COUNDOWN"] = 29,
    ["MID_MSG_PVP_GAME_START"] = 30,
    ["MID_MSG_PVP_GAME_TICK_TIME_MEET"] = 31,
    ["MID_MSG_PVP_NOTIFY_SPAWN_MONSTER"] = 32,
    ["MID_MAX"] = 33,

}

_M.MsgMessageIDStr = {
    [0] = "MID_LOGIN",
    [1] = "MID_LOGOUT",
    [2] = "MID_CONFIRM_DELAY",
    [3] = "MID_GENERIC_JOB",
    [4] = "MID_PLAYER_FACE",
    [5] = "MID_PLAYER_FACE_RESULT",
    [6] = "MID_PLAYER_MOVE_MEET_OBSTACLE",
    [7] = "MID_PLAYER_MOVE_MEET_OBSTACLE_RESULT",
    [8] = "MID_PLAYER_MOVE_START",
    [9] = "MID_PLAYER_MOVE_START_RESULT",
    [10] = "MID_PLAYER_MOVE_STOP",
    [11] = "MID_PLAYER_MOVE_STOP_RESULT",
    [12] = "MID_TIMEVALUE",
    [13] = "MID_MSG_ENTRY_SCENE_REQUEST",
    [14] = "MID_MSG_BROADCAST_PLAYER_INFO",
    [15] = "MID_MSG_LEAVE_SCENE",
    [16] = "MID_MSG_BROADCAST_PLAYER_LEAVE",
    [17] = "MIN_MSG_PLAYER_VIVE_DEVICE_INFO",
    [18] = "MIN_MSG_PLAYER_VIVE_DEVICE_INFO_RESULT",
    [19] = "MID_MSG_PLAYER_FIRE",
    [20] = "MID_MSG_PLAYER_FIRE_RESULT",
    [21] = "MID_MSG_PLAYER_RELOAD",
    [22] = "MID_MSG_PLAYER_RELOAD_RESULT",
    [23] = "MID_MSG_PLAYER_CHANGE_WEAPON",
    [24] = "MID_MSG_PLAYER_CHANGE_WEAPON_RESULT",
    [25] = "MID_MSG_PLAYER_REVIVE",
    [26] = "MID_MSG_PLAYER_REVIVE_RESULT",
    [27] = "MID_MSG_PLAYER_PROJECTILE_DAMAGE",
    [28] = "MID_MSG_PLAYER_PROJECTILE_DAMAGE_RESULT",
    [29] = "MID_MSG_PVP_GAME_START_COUNDOWN",
    [30] = "MID_MSG_PVP_GAME_START",
    [31] = "MID_MSG_PVP_GAME_TICK_TIME_MEET",
    [32] = "MID_MSG_PVP_NOTIFY_SPAWN_MONSTER",
    [33] = "MID_MAX",

}

return _M
