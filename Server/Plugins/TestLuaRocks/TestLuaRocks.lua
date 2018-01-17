
-- TestLuaRocks.lua

-- This is a mockup plugin that does a quick test of LuaRocks capability in Cuberite

-- "Success" is when the plugin loads, downloads the forum webpage and displays the headers and length and then displays both libs as loaded.
-- "Failure" usually manifests as one of the "require" lines failing, although you have the luarock installed.
-- Note that the plugin deliberately never fully loads, so that it can be reloaded fast by pressing its Enable button in the webadmin's plugin list.






local log30 = require("30log");
local socket = require("socket");
local http = require("socket.http");





LOGINFO("Trying to download a webpage...")
local body, code, headers = http.request('https://forum.cuberite.org/')
LOG("code: " .. tostring(code))
LOG("headers: ")
for k, v in pairs(headers or {}) do
	LOG("  " .. k .. ": " .. v)
end
LOG("body length: " .. string.len(body))





function Initialize(a_Plugin)
	if (socket == nil) then
		LOGWARNING("LuaSocket not found")
	else
		LOG("LuaSocket loaded")
	end
	if (log30 == nil) then
		LOGWARNING("30log not found")
	else
		LOG("30log loaded")
	end
	LOGINFO("Reload plugin from console or webadmin to rerun tests.")
	return false
end
