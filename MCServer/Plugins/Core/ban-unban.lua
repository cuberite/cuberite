function HandleBanCommand( Split, Player )
	if( #Split < 2 ) then
		Player:SendMessage(cChatColor.Yellow .. "[INFO] " .. cChatColor.White .. "Usage: /ban [Player] <Reason>" )
		return true
	end

	local Reason = cChatColor.Red .. "You have been banned." .. cChatColor.White .. " Did you do something illegal?"
	if( #Split > 2 ) then
		Reason = table.concat(Split, " ", 3)
	end
	
	if KickPlayer(Split[2], Reason) == false then
		BannedPlayersIni:DeleteValue("Banned", Split[2])
		BannedPlayersIni:SetValueB("Banned", Split[2], true)
		BannedPlayersIni:WriteFile()
		Player:SendMessage(cChatColor.Green .. "[INFO] " .. cChatColor.White .. "Could not find player, but banned anyway" )
	else
		BannedPlayersIni:DeleteValue("Banned", Split[2])
		BannedPlayersIni:SetValueB("Banned", Split[2], true)
		BannedPlayersIni:WriteFile()
		Player:SendMessage(cChatColor.Green .. "[INFO] " .. cChatColor.White .. "Successfully kicked and banned player" )
	end
	return true
end

function HandleUnbanCommand( Split, Player )
	if( #Split < 2 ) then
		Player:SendMessage(cChatColor.Yellow .. "[INFO] " .. cChatColor.White .. "Usage: /unban [Player]" )
		return true
	end
	
	if( BannedPlayersIni:GetValueB("Banned", Split[2], false) == false ) then
		Player:SendMessage(cChatColor.Rose .. "[INFO] " .. cChatColor.White .. Split[2] .. " is not banned!" )
		return true
	end
	
	BannedPlayersIni:DeleteValue("Banned", Split[2])
	BannedPlayersIni:SetValueB("Banned", Split[2], false)
	BannedPlayersIni:WriteFile()

	LOGINFO( Player:GetName() .. " is unbanning " .. Split[2] )
	Player:SendMessage(cChatColor.Green .. "[INFO] " .. cChatColor.White .. "Unbanning " .. Split[2] )

	return true
end