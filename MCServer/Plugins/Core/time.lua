function HandleTimeCommand( Split, Player )
	if Split[2] == nil then 
		Player:SendMessage( cChatColor.Green .. "Usage: /time [Day/Night/Set/Add]" )
		return true
	end
	local Server = cRoot:Get():GetServer()
	if( string.upper( Split[2] ) == "DAY") then
		Player:GetWorld():SetTimeOfDay( 0 )
		Server:SendMessage( cChatColor.Green .. Player:GetName() .. " set the time to Day.")
	elseif( string.upper( Split[2] ) == "NIGHT") then
		Player:GetWorld():SetTimeOfDay( 12000 + 1000 )
		Server:SendMessage( cChatColor.Green .. Player:GetName() .. " set the time to Night.")
	elseif( string.upper(Split[2]) == "SET" ) and ( tonumber(Split[3]) ~= nil) then
		Player:GetWorld():SetTimeOfDay( tonumber(Split[3]) )
		Server:SendMessage( cChatColor.Green .. Player:GetName() .. " set the time to " .. Split[3] )
	elseif( string.upper(Split[2]) == "ADD" ) and ( tonumber(Split[3]) ~= nil) then
		Player:GetWorld():SetTimeOfDay( Player:GetWorld():GetTimeOfDay() + Split[3] )
		Server:SendMessage( cChatColor.Green .. Player:GetName() .. " Added " .. Split[3] .. " to the time" )
	else
		Player:SendMessage( cChatColor.Green .. "Usage: /time [Day/Night/Set/Add]" )
	end
	return true
end