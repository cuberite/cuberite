function HandleGotoWorldCommand( Split, Player )
	if( #Split ~= 2 ) then
		Player:SendMessage( cChatColor.Green .. "Usage: /gotoworld [WorldName]" )
		return true	
	end
	
	if( Player:MoveToWorld(Split[2]) == false ) then
		Player:SendMessage( cChatColor.Green .. "Could not move to world '" .. Split[2] .. "'!" )
		return true
	end
	
	
	Player:SendMessage( cChatColor.Green .. "Moved successfully to '" .. Split[2] .. "'! :D" )
	return true
end