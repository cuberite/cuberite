function HandleViewDistanceCommand( Split, Player )
	if( #Split ~= 2 ) then
		Player:SendMessage( cChatColor.Green .. "Usage: /viewdistance [".. cClientHandle.MIN_VIEW_DISTANCE .."-".. cClientHandle.MAX_VIEW_DISTANCE .."]" )
		return true
	end
	
	Player:GetClientHandle():SetViewDistance( Split[2] )
	Player:SendMessage(cChatColor.Green .. "Your viewdistance has been set to " .. Player:GetClientHandle():GetViewDistance() )
	return true
end