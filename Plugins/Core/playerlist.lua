local PlayerTable = {}

function HandlePlayerListCommand( Split, Player )
	PlayerTable = {}
	Player:GetWorld():ForEachPlayer( AppendToTable )

	local Message = cChatColor.Green .. "Connected players: (".. cChatColor.White.. #PlayerTable .. cChatColor.Green .. ")"
	Player:SendMessage( Message )
	
	Player:SendMessage( table.concat(PlayerTable, " ") )
	return true
end

function AppendToTable( Player )
	table.insert(PlayerTable, Player:GetName() )
end