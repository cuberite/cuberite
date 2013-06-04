
-- CommandState.lua

-- Implements the cCommandState class representing a command state for each VIP player

--[[
The command state holds internal info, such as the coords they selected using the wand
The command state needs to be held in a per-entity manner, so that we can support multiple logins
from the same account (just for the fun of it)
The OOP class implementation follows the PiL 16.1

Also, a global table g_CommandStates is the map of PlayerEntityID -> cCommandState
--]]





cCommandState = {
	-- Default coords
	m_Coords1 = {x = 0, y = 0, z = 0};
	m_Coords2 = {x = 0, y = 0, z = 0};
};

g_CommandStates = {};





function cCommandState:new(obj)
	obj = obj or {};
	setmetatable(obj, self);
	self.__index = self;
	return obj;
end





--- Returns the current coord pair as a cCuboid object
function cCommandState:GetCurrentCuboid()
	local res = cCuboid(
		self.Coords1.x, self.Coords1.y, self.Coords1.z,
		self.Coords2.x, self.Coords2.y, self.Coords2.z
	);
	res:Sort();
	return res;
end




--- Sets the first set of coords (upon rclk with a wand)
function cCommandState:SetCoords1(a_BlockX, a_BlockY, a_BlockZ)
	self.m_Coords1.x = a_BlockX;
	self.m_Coords1.y = a_BlockY;
	self.m_Coords1.z = a_BlockZ;
end





--- Sets the second set of coords (upon lclk with a wand)
function cCommandState:SetCoords2(a_BlockX, a_BlockY, a_BlockZ)
	self.m_Coords2.x = a_BlockX;
	self.m_Coords2.y = a_BlockY;
	self.m_Coords2.z = a_BlockZ;
end





--- Returns the cCommandState for the specified player; creates one if not existant
function GetCommandStateForPlayer(a_Player)
	local res = g_CommandStates[a_Player:GetUniqueID()];
	if (res == nil) then
		res = cCommandState:new();
		g_CommandStates[a_Player:GetUniqueID()] = res;
	end
	return res;
end;




