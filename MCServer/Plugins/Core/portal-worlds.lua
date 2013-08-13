function HandlePortalCommand( Split, Player )

	if( #Split ~= 2 ) then
		SendMessage( Player, "Usage: /portal [WorldName]" )
		return true
	end

	if( Player:MoveToWorld(Split[2]) == false ) then
		SendMessageFailure( Player, "Could not move to world " .. Split[2] .. "!" )
		return true
	end

	SendMessageSuccess( Player, "Moved successfully to '" .. Split[2] .. "'! :D" )
	return true

end

function HandleWorldsCommand( Split, Player )

	local SettingsIni = cIniFile("settings.ini")
	if SettingsIni:ReadFile() == false then
		SendMessageFailure( Player, "No worlds found" )
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
		table.insert( Worlds, SettingsIni:GetValue( Key, i ) )
	end
	SendMessage( Player, "Found " .. #Worlds .. " worlds" )
	SendMessage( Player, table.concat( Worlds, ", " ) )
	return true

end
