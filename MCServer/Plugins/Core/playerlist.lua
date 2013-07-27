function HandlePlayerListCommand( Split, Player )

	local PlayerTable = {}
	local AppendToTable = function( Player )
		table.insert(PlayerTable, Player:GetName() )
	end
	Player:GetWorld():ForEachPlayer( AppendToTable )

	local Message = cChatColor.Green .. "Connected players: (".. cChatColor.White.. #PlayerTable .. cChatColor.Green .. ")"
	Player:SendMessage( Message )
	
	Player:SendMessage( table.concat(PlayerTable, " ") )
	return true
end