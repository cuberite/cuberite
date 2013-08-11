function HandleChangeGMCommand( Split, Player )

	if( #Split ~= 2 ) then
		SendMessage( Player, "Usage: /gm [0|1]" )
		return true
	end

	Player:SetGameMode( Split[2] )
	return true

end
