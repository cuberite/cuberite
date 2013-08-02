function HandleBanCommand( Split, Player )
	if( #Split < 2 ) then
		Player:SendMessage(cChatColor.Yellow .. "[INFO] " .. cChatColor.White .. "Usage: /ban [Player] <Reason>" )
		return true
	end

	local Reason = "You have been banned"
	if( #Split > 2 ) then
		Reason = table.concat(Split, " ", 3)
	end
		
	if( BanPlayer(Split[2], Reason) == false ) then
		Player:SendMessage(cChatColor.Rose .. "[INFO] " .. cChatColor.White .. "Could not find player " .. Split[2] )
		return true
	end

	return true
end

function BanPlayer(PlayerName, Reason)
	-- Ban the player in the banned.ini:
	BannedPlayersIni:SetValueB("Banned", PlayerName, true)
	BannedPlayersIni:WriteFile()
	
	-- Kick the player:
	if (Reason == nil) then
		Reason = "You have been banned"
	end
	local Success = KickPlayer(PlayerName, Reason)
	if (not(Success)) then
		return false;
	end
	
	LOGINFO("'" .. PlayerName .. "' has been banned (\"" .. Reason .. "\") ");
	local Server = cRoot:Get():GetServer();
	Server:SendMessage("Banned " .. PlayerName);
	
	return true
end

function HandleUnbanCommand( Split, Player )
	if( #Split < 2 ) then
		Player:SendMessage(cChatColor.Yellow .. "[INFO] " .. cChatColor.White .. "Usage: /unban [Player]" )
		return true
	end
	
	if( BannedPlayersIni:GetValueB("Banned", Split[2], false) == false ) then
		Player:SendMessage(cChatColor.Rose .. "[INFO] " .. cChatColor.White .. " is not banned!" )
		return true
	end
	
	BannedPlayersIni:SetValueB("Banned", Split[2], false, false)
	BannedPlayersIni:WriteFile()

	local Server = cRoot:Get():GetServer()
	LOGINFO( Player:GetName() .. " is unbanning " .. Split[2] )
	Server:SendMessage( "Unbanning " .. Split[2] )

	return true
end