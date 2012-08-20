local CurrentlyEditingIni = nil

-- Some HTML helper functions
local function HTML_Option( value, text, selected )
	if( selected == true ) then
		return "<option value=\"" .. value .. "\" selected>" .. text .. "</option>"
	else
		return "<option value=\"" .. value .. "\">" .. text .. "</option>"
	end
end

local function HTML_Select_On_Off( name, defaultValue )
	local Content = ""
	Content = Content .. "<select name=\"" .. name .. "\">"
	Content = Content .. HTML_Option("1", "On",  defaultValue == 1 )
	Content = Content .. HTML_Option("0", "Off", defaultValue == 0 )
	Content = Content .. "</select>"
	return Content
end


local function ShowGeneralSettings( Request )
	local Content = ""
	local InfoMsg = nil
	
	local SettingsIni = cIniFile("settings.ini")
	if( SettingsIni:ReadFile() == false ) then
		InfoMsg = "<b style=\"color: red;\">ERROR: Could not read settings.ini!</b>"
	end
	
	if( Request.PostParams["general_submit"] ~= nil ) then
		
		SettingsIni:SetValue("Server", "Description",Request.PostParams["Server_Description"],false )
		if( tonumber( Request.PostParams["Server_MaxPlayers"] ) ~= nil ) then
			SettingsIni:SetValue("Server", "MaxPlayers", Request.PostParams["Server_MaxPlayers"], false )
		end
		if( tonumber( Request.PostParams["Server_Port"] ) ~= nil ) then
			SettingsIni:SetValue("Server", "Port",       Request.PostParams["Server_Port"],       false )
		end
		if( tonumber( Request.PostParams["Authentication_Authenticate"] ) ~= nil ) then
			SettingsIni:SetValue("Authentication", "Authenticate", Request.PostParams["Authentication_Authenticate"], false )
		end
		if( SettingsIni:WriteFile() == false ) then
			InfoMsg =  "<b style=\"color: red;\">ERROR: Could not write to settings.ini!</b>"
		else
			InfoMsg = "<b style=\"color: green;\">INFO: Successfully saved changes to settings.ini</b>"
		end
	end
	
	
	Content = Content .. "<form method=\"POST\">"
	
	Content = Content .. "<p><b>General Settings</b></p>"
	if( InfoMsg ~= nil ) then
		Content = Content .. "<p>" .. InfoMsg .. "</p>"
	end
	Content = Content .. "<table>"
	Content = Content .. "<th colspan=\"2\">Server</th>"
	Content = Content .. "<tr><td style=\"width: 50%;\">Description:</td>"
	Content = Content .. "<td><input type=\"text\" name=\"Server_Description\" value=\"" .. SettingsIni:GetValue("Server", "Description") .. "\"></td></tr>"
	Content = Content .. "<tr><td>Max Players:</td>"
	Content = Content .. "<td><input type=\"text\" name=\"Server_MaxPlayers\" value=\"" .. SettingsIni:GetValue("Server", "MaxPlayers") .. "\"></td></tr>"
	Content = Content .. "<tr><td>Port:</td>"
	Content = Content .. "<td><input type=\"text\" name=\"Server_Port\" value=\"" .. SettingsIni:GetValue("Server", "Port") .. "\"></td></tr>"
	Content = Content .. "</table><br>"
	
	Content = Content .. "<table>"
	Content = Content .. "<th colspan=\"2\">Authentication</th>"
	Content = Content .. "<tr><td style=\"width: 50%;\">Authenticate:</td>"
	Content = Content .. "<td>" .. HTML_Select_On_Off("Authentication_Authenticate", SettingsIni:GetValueI("Authentication", "Authenticate") ) .. "</td></tr>"
	Content = Content .. "</table>"
	
	Content = Content .. "<input type=\"submit\" value=\"Save Settings\" name=\"general_submit\"> WARNING: Any changes made here might require a server restart to be applied!"
	Content = Content .. "</form>"
	
	return Content
end


local function ShowMonstersSettings( Request )
	return "<p><b>Monsters Settings</b></p>"
end

local function ShowWorldsSettings( Request )
	return "<p><b>Worlds Settings</b></p>"
end

function HandleRequest_ServerSettings( Request )
	local Content = ""

	Content = Content .. "<p><b>Server Settings</b></p>"
	Content = Content .. "<table>"
	Content = Content .. "<tr>"
	Content = Content .. "<td><a href=\"?tab=General\">General</a></td>"
	Content = Content .. "<td><a href=\"?tab=Monsters\">Monsters</a></td>"
	Content = Content .. "<td><a href=\"?tab=Worlds\">Worlds</a></td>"
	Content = Content .. "</tr>"
	Content = Content .. "</table>"
	Content = Content .. "<br>"
	
	if( Request.Params["tab"] == "Monsters" ) then
		Content = Content .. ShowMonstersSettings( Request )
	elseif( Request.Params["tab"] == "Worlds" ) then
		Content = Content .. ShowWorldsSettings( Request )
	else
		Content = Content .. ShowGeneralSettings( Request ) -- Default to general settings
	end
	
	return Content
end