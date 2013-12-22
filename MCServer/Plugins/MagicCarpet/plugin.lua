local Carpets = {}
local PLUGIN

function Initialize( Plugin )
	Plugin:SetName( "MagicCarpet" )
	Plugin:SetVersion( 2 )

	cPluginManager.AddHook(cPluginManager.HOOK_PLAYER_MOVING, OnPlayerMoving)
	cPluginManager.AddHook(cPluginManager.HOOK_DISCONNECT,    OnDisconnect)
	
	local PluginManager = cPluginManager:Get()
	PluginManager:BindCommand("/mc", "magiccarpet", HandleCarpetCommand, " - Spawns a magical carpet");
	
	PLUGIN = Plugin
	
	LOG( "Initialised " .. Plugin:GetName() .. " v." .. Plugin:GetVersion() )
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
		SendMessageSuccess(Player, "You're on a magic carpet!")
		SendMessage(Player, "Look straight down to descend. Jump to ascend.")
	else
		Carpet:remove()
		Carpets[ Player ] = nil
		SendMessageSuccess(Player, "The carpet vanished!")
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
