-- Location object
cLocation = {}
function cLocation:new( x, y, z )
	local object = { x = x, y = y, z = z }
	setmetatable(object, { __index = cLocation })
	return object
end

-- Offsets
cFibers = { }
function cFibers:new()
	local object = {
						cLocation:new( 2, -1, 2 ),
						cLocation:new( 2, -1, 1 ),
						cLocation:new( 2, -1, 0 ),
						cLocation:new( 2, -1, -1 ),
						cLocation:new( 2, -1, -2 ),
						cLocation:new( 1, -1, 2 ),
						cLocation:new( 1, -1, 1 ),
						cLocation:new( 1, -1, 0 ),
						cLocation:new( 1, -1, -1 ),
						cLocation:new( 1, -1, -2 ),
						cLocation:new( 0, -1, 2 ),
						cLocation:new( 0, -1, 1 ),
						cLocation:new( 0, -1, 0 ),
						cLocation:new( 0, -1, -1 ),
						cLocation:new( 0, -1, -2 ),
						cLocation:new( -1, -1, 2 ),
						cLocation:new( -1, -1, 1 ),
						cLocation:new( -1, -1, 0 ),
						cLocation:new( -1, -1, -1 ),
						cLocation:new( -1, -1, -2 ),
						cLocation:new( -2, -1, 2 ),
						cLocation:new( -2, -1, 1 ),
						cLocation:new( -2, -1, 0 ),
						cLocation:new( -2, -1, -1 ),
						cLocation:new( -2, -1, -2 ),
						imadeit = false,
					}
	setmetatable(object, { __index = cFibers })
	return object;
end

-- Carpet object
cCarpet = {}
function cCarpet:new()
	local object = {	Location = cLocation:new(0,0,0),
						Fibers = cFibers:new(),
					}
	setmetatable(object, { __index = cCarpet })
	return object
end

function cCarpet:remove()
	local World = cRoot:Get():GetWorld()
	for i, fib in ipairs( self.Fibers ) do
		local x = self.Location.x + fib.x
		local y = self.Location.y + fib.y
		local z = self.Location.z + fib.z
		local BlockID = World:GetBlock( x, y, z )
		if( fib.imadeit == true and BlockID == E_BLOCK_GLASS ) then
			World:SetBlock( x, y, z, 0, 0 )
			fib.imadeit = false
		end
	end
end

function cCarpet:draw()
	local World = cRoot:Get():GetWorld()
	for i, fib in ipairs( self.Fibers ) do
		local x = self.Location.x + fib.x
		local y = self.Location.y + fib.y
		local z = self.Location.z + fib.z
		local BlockID = World:GetBlock( x, y, z )
		if( BlockID == 0  ) then
			fib.imadeit = true
			World:SetBlock( x, y, z, E_BLOCK_GLASS, 0 )
		else
			fib.imadeit = false
		end
	end
end

function cCarpet:moveTo( NewPos )
	local x = math.floor( NewPos.x )
	local y = math.floor( NewPos.y )
	local z = math.floor( NewPos.z )
	if( self.Location.x ~= x or self.Location.y ~= y or self.Location.z ~= z ) then
		self:remove()
		self.Location = cLocation:new( x, y, z )
		self:draw()
	end
end


MagicCarpetPlugin = {}
MagicCarpetPlugin.__index = MagicCarpetPlugin

function MagicCarpetPlugin:new()
   local t = {}
   setmetatable(t, MagicCarpetPlugin)
   local w = Lua__cPlugin:new()
   tolua.setpeer(w, t)
   w:tolua__set_instance(w)
   return w
end

function MagicCarpetPlugin:Initialize()
	self:SetName( "MagicCarpet" )
	self:SetVersion( 1 )

	PluginManager = cRoot:Get():GetPluginManager()
	PluginManager:AddHook( self, cPluginManager.E_PLUGIN_PLAYER_MOVE)
	PluginManager:AddHook( self, cPluginManager.E_PLUGIN_DISCONNECT)
	
	self:AddCommand("/mc", " - Spawns a magical carpet!", 	"magiccarpet")
	self:BindCommand( "/mc", 		"magiccarpet",		HandleCarpetCommand )

	self.Carpets = {}

	Log( "Initialized " .. self:GetName() .. " v." .. self:GetVersion() )
	return true
end

function MagicCarpetPlugin:OnDisable()
	Log( self:GetName() .. " v." .. self:GetVersion() .. " is shutting down..." )
	for i, Carpet in pairs( self.Carpets ) do
		Carpet:remove()
	end
end

function HandleCarpetCommand( Split, Player )
	Carpet = self.Carpets[ Player ]
	if( Carpet == nil ) then
		self.Carpets[ Player ] = cCarpet:new()
		Player:SendMessage("You're on a magic carpet!" )
	else
		Carpet:remove()
		self.Carpets[ Player ] = nil
		Player:SendMessage("The carpet vanished!" )
	end

	return true
end

function MagicCarpetPlugin:OnDisconnect( Reason, Player )
	local Carpet = self.Carpets[ Player ]
	if( Carpet ~= nil )	 then
		Carpet:remove()
	end
	self.Carpets[ Player ] = nil
end

function MagicCarpetPlugin:OnPlayerMove( Player )
	local Carpet = self.Carpets[ Player ]
	if( Carpet == nil ) then
		return
	end

	if( Player:GetPitch() == 90 ) then
		Carpet:moveTo( cLocation:new( Player:GetPosX(), Player:GetPosY()-1, Player:GetPosZ() ) )
	else
		Carpet:moveTo( cLocation:new( Player:GetPosX(), Player:GetPosY(), Player:GetPosZ() ) )
	end
end

Plugin = MagicCarpetPlugin:new()
cRoot:Get():GetPluginManager():AddPlugin( Plugin )

