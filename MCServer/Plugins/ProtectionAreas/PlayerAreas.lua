
-- PlayerAreas.lua
-- Implements the cPlayerAreas class representing the per-player area storage object

--[[
Each player instance is expected to have a separate object of type cPlayerAreas.
Each object has an array of {cuboid, IsAllowed} tables, one for each area that is "within reach"
The code can then ask each object, whether the player can interact with a certain block or not.
A player can interact with a block if either one of these is true:
1, There are no areas covering the block
2, There is at least one area covering the block with IsAllowed set to true
The OOP class implementation follows the PiL 16.1

Also, a global table g_PlayerAreas is the actual map of PlayerID -> cPlayerAreas
--]]




cPlayerAreas = {};

g_PlayerAreas = {};





function cPlayerAreas:new(obj)
	obj = obj or {};
	setmetatable(obj, self);
	self.__index = self;
	return obj;
end




-- Adds a new cuboid to the area list, where the player is either allowed or not, depending on the IsAllowed param
function cPlayerAreas:AddArea(a_Cuboid, a_IsAllowed)
	table.insert(self, {Cuboid = a_Cuboid, IsAllowed = a_IsAllowed});
end





--- returns true if the player owning this object can interact with the specified block
function cPlayerAreas:CanInteractWithBlock(a_BlockX, a_BlockY, a_BlockZ)
	-- iterate through all the stored areas:
	local IsInsideAnyArea = false;
	for idx, Area in ipairs(self) do
		if (Area.Cuboid:IsInside(a_BlockX, a_BlockY, a_BlockZ)) then
			if (Area.IsAllowed) then
				return true;
			end
			-- The coords are inside a cuboid for which the player doesn't have access, take a note of it
			IsInsideAnyArea = true;
		end
	end
	
	if (IsInsideAnyArea) then
		-- The specified coords are inside at least one area, but none of them allow the player to interact
		return false;
	end
	
	-- The coords are not inside any area
	-- TODO: Have a config saying whether a player can build in the non-areated space or not
	return true;
end




