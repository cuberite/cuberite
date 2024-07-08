
-- _preload.lua

-- First thing executed when the plugin loads. Replaces the global environment (_G) with an empty table
-- with __index set to the old environment. This way any function or variable that is created globally by the plugin
-- won't be reported as new or undocumented.





local newEnv, oldEnv = {}, _G
local setmetatable = setmetatable
for k, v in pairs(_G) do
	newEnv[k] = v;
	oldEnv[k] = nil;
end
_G = setmetatable(oldEnv, {__index = newEnv});




