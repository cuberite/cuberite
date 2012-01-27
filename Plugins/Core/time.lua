function HandleTimeCommand( Split, Player )
	if( #Split ~= 2 ) then
		Player:SendMessage( cChatColor.Green .. "Usage: /time [Day/Night]" )
		return true;
	end

	local Server = cRoot:Get():GetServer()
	if( string.upper( Split[2] ) == "DAY") then
		Player:GetWorld():SetWorldTime( 0 )
		Server:SendMessage( cChatColor.Green .. Player:GetName() .. " set the time to Day.")
	elseif( string.upper( Split[2] ) == "NIGHT") then
		Player:GetWorld():SetWorldTime( 12000 + 1000 )
		Server:SendMessage( cChatColor.Green .. Player:GetName() .. " set the time to Night.")
	else
		Player:SendMessage( cChatColor.Green .. "Usage: /time [Day/Night]" )
	end
	return true
end