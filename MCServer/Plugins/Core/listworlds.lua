function HandleListWorldsCommand( Split, Player )
	local SettingsIni = cIniFile("settings.ini")
	if SettingsIni:ReadFile() == false then
		Player:SendMessage( cChatColor.Green .. "No worlds found" )
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
	Player:SendMessage( cChatColor.Green .. "Worlds:" )
	Player:SendMessage( cChatColor.Green .. table.concat( Worlds, ", " ) )
	return true
end