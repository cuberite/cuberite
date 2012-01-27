function OnKilled( Killed, Killer )
	if( Killer == nil ) then
		local KilledPlayer = tolua.cast( Killed, "cPlayer")
		if( not KilledPlayer:IsA("cPlayer") or KilledPlayer == nil ) then
			return false
		end

		local Server = cRoot:Get():GetServer()
		Server:SendMessage( cChatColor.Red .. KilledPlayer:GetName() .. " died" )
	else
		local KilledPlayer = tolua.cast( Killed, "cPlayer")
		if( not KilledPlayer:IsA("cPlayer") or KilledPlayer == nil ) then
			return false
		end
		local KillerPlayer = tolua.cast( Killer, "cPlayer")
		if( not KillerPlayer:IsA("cPlayer") or KillerPlayer == nil ) then
			return false
		end

		local Server = cRoot:Get():GetServer()
		Server:SendMessage( cChatColor.Red .. KilledPlayer:GetName() .. " was killed by " .. KillerPlayer:GetName() .. "!" )
	end
	return false
end