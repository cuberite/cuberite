function HandlePlayerListCommand( Split, Player )
	local World = Player:GetWorld()
	local PlayerList = World:GetAllPlayers()

	local Message = cChatColor.Green .. "Connected players: (".. cChatColor.White.. #PlayerList .. cChatColor.Green .. ")"
	Player:SendMessage( Message )

	local PlayerTable = {}
	for i, TempPlayer in ipairs( PlayerList ) do
		local PlayerName = TempPlayer:GetName()
		table.insert(PlayerTable, PlayerName )
	end

	Player:SendMessage( table.concat(PlayerTable, " ") )
	return true
end