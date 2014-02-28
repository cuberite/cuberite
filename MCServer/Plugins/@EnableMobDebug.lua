
-- @EnableMobDebug.lua

-- Enables the MobDebug debugger, used by ZeroBrane Studio, for a plugin
-- Needs to be named with a @ at the start so that it's loaded as the first file of the plugin

--[[
Usage:
Copy this file to your plugin's folder when you want to debug that plugin
You should neither check this file into the plugin's version control system,
nor distribute it in the final release.
--]]





-- Try to load the debugger, be silent about failures:
local IsSuccess, MobDebug = pcall(require, "mobdebug")
if (IsSuccess) then
	MobDebug.start()
	
	-- The debugger will automatically put a breakpoint on this line, use this opportunity to set more breakpoints in your code
	LOG(cPluginManager:GetCurrentPlugin():GetName() .. ": MobDebug enabled")
end




