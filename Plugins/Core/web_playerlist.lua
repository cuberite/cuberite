local PlayerHTML = ""
local PlayerNum = 0

function HandleRequest_PlayerList( Request )
	local World = cRoot:Get():GetWorld()
	local Content = ""
	
	if( Request.Params["playerlist-kick"] ~= nil ) then
		local KickPlayerName = Request.Params["playerlist-kick"]
		local Player = World:GetPlayer( KickPlayerName )
		if( Player == nil ) then
			Content = Content .. "<p>Could not find player " .. KickPlayerName .. " !</p>"
		elseif( Player:GetName() == KickPlayerName ) then
			Player:GetClientHandle():Kick("You were kicked from the game!")
			Content = Content .. "<p>" .. KickPlayerName .. " has been kicked from the game!</p>"
		end
	end
	
	Content = Content .. "<p>Connected Players: <b>" .. World:GetNumPlayers() .. "</b></p>"
	Content = Content .. "<table>"
	
	PlayerNum = 0
	PlayerHTML = ""
	World:ForEachPlayer( CreatePlayerList )

	if( PlayerHTML ~= "" ) then
		Content = Content .. PlayerHTML
	else
		Content = Content .. "<tr><td>None</td></tr>"
	end
	Content = Content .. "</table>"
	Content = Content .. "<br>"
	return Content
end

function CreatePlayerList( Player, Data )
	PlayerNum = PlayerNum + 1
	PlayerHTML = PlayerHTML .. "<tr>"
	PlayerHTML = PlayerHTML .. "<td style='width: 10px;'>" .. PlayerNum .. ".</td>"
	PlayerHTML = PlayerHTML .. "<td>" .. Player:GetName() .. "</td>"
	PlayerHTML = PlayerHTML .. "<td><a href='?playerlist-kick=" .. Player:GetName() .. "'>Kick</a></td>"
	PlayerHTML = PlayerHTML .. "</tr>"
end