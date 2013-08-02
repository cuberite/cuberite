function HandlePortalCommand( Split, Player )
	if( #Split ~= 2 ) then
		Player:SendMessage(cChatColor.Yellow .. "[INFO] " .. cChatColor.White .. "Usage: /portal [WorldName]" )
		return true	
	end
	
	if( Player:MoveToWorld(Split[2]) == false ) then
		Player:SendMessage(cChatColor.Rose .. "[INFO] " .. cChatColor.White .. "Could not move to world " .. Split[2] .. "!" )
		return true
	end
	
	
	Player:SendMessage(cChatColor.Green .. "[INFO] " .. cChatColor.White .. "Moved successfully to '" .. Split[2] .. "'! :D" )
	return true
end

function HandleWorldsCommand( Split, Player )
	local SettingsIni = cIniFile("settings.ini")
	if SettingsIni:ReadFile() == false then
		Player:SendMessage(cChatColor.Yellow .. "[INFO] " .. cChatColor.White .. "No worlds found" )
	end
	Number = SettingsIni:NumValues("Worlds") - 1
	Worlds = {}
	for i=0, SettingsIni:GetNumKeys() - 1 do
		if SettingsIni:GetKeyName(i) == "Worlds" then
			Key = i
			break
		end
	end
	for i=0, Number	do
		table.insert( Worlds, SettingsIni:GetValue( Key, i) )
	end
	Player:SendMessage(cChatColor.Yellow .. "[INFO] " .. cChatColor.White .. "Found " .. #Worlds .. " worlds" )
	Player:SendMessage(cChatColor.Gold .. table.concat( Worlds, ", " ) )
	return true
end