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

	Log( "Initialized " .. Plugin:GetName() .. " v." .. Plugin:GetVersion() )
	return true
end





function OnDisable()
	Log( PLUGIN:GetName() .. " v." .. PLUGIN:GetVersion() .. " is shutting down..." )
	for i, Carpet in pairs( Carpets ) do
		Carpet:remove()
	end
end





function HandleCarpetCommand( Split, Player )
	Carpet = Carpets[ Player ]
	if( Carpet == nil ) then
		Carpets[ Player ] = cCarpet:new()
		Player:SendMessage("You're on a magic carpet!" )
	else
		Carpet:remove()
		Carpets[ Player ] = nil
		Player:SendMessage("The carpet vanished!" )
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
			LOGINFO("Fell tru mc!")
			Player:TeleportTo( Player:GetPosX(), Carpet:getY(), Player:GetPosZ() )
		end
		Carpet:moveTo( cLocation:new( Player:GetPosX(), Player:GetPosY(), Player:GetPosZ() ) )
	end
end



