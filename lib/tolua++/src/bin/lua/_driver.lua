
-- Allow debugging by ZBS, if run under the IDE:
local mobdebugfound, mobdebug = pcall(require, "mobdebug")
if mobdebugfound then mobdebug.start() end

-- The list of valid arguments that the ToLua scripts can process:
local KnownArgs = {
	['v'] = true,
	['h'] = true,
	['p'] = true,
	['P'] = true,
	['o'] = true,
	['n'] = true,
	['H'] = true,
	['S'] = true,
	['1'] = true,
	['L'] = true,
	['D'] = true,
	['W'] = true,
	['C'] = true,
	['E'] = true,
	['t'] = true,
	['q'] = true,
}





-- The flags table used by ToLua scripts, to be filled from the cmdline params:
flags = {}

-- Te extra parameters used by ToLua scripts:
_extra_parameters = {}

-- ToLua version required by the scripts:
TOLUA_VERSION = "tolua++-1.0.92"

-- Lua version used by ToLua, required by the scripts:
TOLUA_LUA_VERSION = "Lua 5.1"






-- Process the cmdline params into the flags table:
local args = arg or {}
local argc = #args
local i = 1
while (i <= argc) do
	local argv = args[i]
	if (argv:sub(1, 1) == "-") then
		if (KnownArgs[argv:sub(2)]) then
			print("Setting flag \"" .. argv:sub(2) .. "\" to \"" .. args[i + 1] .. "\".")
			flags[argv:sub(2)] = args[i + 1]
			i = i + 1
		else
			print("Unknown option (" .. i .. "): " .. argv)
			print("Aborting.")
			os.exit(1)
		end
	else
		print("Setting flag \"f\" to \"" .. argv .. "\".")
		flags['f'] = argv
		break
	end
	i = i + 1
end

-- Get the path where the scripts are located:
path = args[0] or ""
local index = path:find("/[^/]*$")
if (index == nil) then
	index = path:find("\\[^\\]*$")
end
if (index ~= nil) then
	path = path:sub(1, index)
end

print("path is set to \"" .. path .. "\".")





-- Call the ToLua processor:
dofile(path .. "all.lua")





