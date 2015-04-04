function Initialize(a_Plugin)
	a_Plugin:SetName("DumpInfo")
	a_Plugin:SetVersion(1)
	
	-- Check if the infodump file exists.
	if (not cFile:Exists("Plugins/InfoDump.lua")) then
		LOGWARN("[DumpInfo] InfoDump.lua was not found.")
		return false
	end
	
	-- Add the webtab.
	a_Plugin:AddWebTab("DumpPlugin", HandleDumpPluginRequest)
	return true
end





function HandleDumpPluginRequest(a_Request)
	local Content = ""
	
	-- Check if it already was requested to dump a plugin.
	if (a_Request.PostParams["DumpInfo"] ~= nil) then
		local F = loadfile("Plugins/InfoDump.lua")
		F("Plugins/" .. a_Request.PostParams["DumpInfo"])
	end
	
	Content = Content .. [[
<table>
	<tr>
		<th colspan="2">DumpInfo</th>
	</tr>]]

	-- Loop through each plugin that is found.
	for PluginName, k in pairs(cPluginManager:Get():GetAllPlugins()) do
		-- Check if there is a file called 'Info.lua' or 'info.lua'
		if (cFile:Exists("Plugins/" .. PluginName .. "/Info.lua")) then
			Content = Content .. "\n<tr>\n"
			Content = Content .. "\t<td>" .. PluginName .. "</td>\n"
			Content = Content .. "\t<td><form method='POST'> <input type='hidden' value='" .. PluginName .. "' name='DumpInfo'> <input type='submit' value='DumpInfo'></form></td>\n"
			Content = Content .. "</tr>\n"
		end
	end
	
	Content = Content .. [[
</table>]]

	return Content
end
