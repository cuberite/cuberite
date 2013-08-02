local PLUGIN = {}
local Carpets = {}

function Initialize( Plugin )
	PLUGIN = Plugin

	Plugin:SetName( "MagicCarpet" )
	Plugin:SetVersion( 1 )

	PluginManager = cRoot:Get():GetPluginManager()
	PluginManager:AddHook(Plugin, cPluginManager.HOOK_PLAYER_MOVING)
	PluginManager:AddHook(Plugin, cPluginManager.HOOK_DISCONNECT)
	
	PluginManager:BindCommand("/mc", "magiccarpet", HandleCarpetCommand, " - Spawns a magical carpet");

	LOG( "Initialized " .. Plugin:GetName() .. " v." .. Plugin:GetVersion() )
	return true
end





function OnDisable()
	LOG( PLUGIN:GetName() .. " v." .. PLUGIN:GetVersion() .. " is shutting down..." )
	for i, Carpet in pairs( Carpets ) do
		Carpet:remove()
	end
end





function HandleCarpetCommand( Split, Player )
	Carpet = Carpets[ Player ]
	if( Carpet == nil ) then
		Carpets[ Player ] = cCarpet:new()
		Player:SendMessage(cChatColor.Green .. "[INFO] " .. cChatColor.White .. "You're on a magic carpet!" )
		Player:SendMessage(cChatColor.Yellow .. "[INFO] " .. cChatColor.White .. "Look straight down to descend. Jump to ascend!" )
	else
		Carpet:remove()
		Carpets[ Player ] = nil
		Player:SendMessage(cChatColor.Green .. "[INFO] " .. cChatColor.White .. "The carpet vanished!" )
	end

	return true
end





function OnDisconnect( Reason, Player )
	local Carpet = Carpets[ Player ]
	if( Carpet ~= nil )	 then
		Carpet:remove()
	end
	Carpets[ Player ] = nil
end





function OnPlayerMoving(Player)
	local Carpet = Carpets[ Player ]
	if( Carpet == nil ) then
		return
	end

	if( Player:GetPitch() == 90 ) then
		Carpet:moveTo( cLocation:new( Player:GetPosX(), Player:GetPosY() - 1, Player:GetPosZ() ) )
	else
		if( Player:GetPosY() < Carpet:getY() ) then
			Player:TeleportToCoords(Player:GetPosX(), Carpet:getY() + 0.2, Player:GetPosZ())
		end
		Carpet:moveTo( cLocation:new( Player:GetPosX(), Player:GetPosY(), Player:GetPosZ() ) )
	end
end