
-- Storage.lua
-- Implements the storage access object, shielding the rest of the code away from the DB

--[[
The cStorage class is the interface to the underlying storage, the SQLite database.
This class knows how to load player areas from the DB, how to add or remove areas in the DB
and other such operations.

Also, a g_Storage global variable is declared, it holds the single instance of the storage.
--]]





cStorage = {};

g_Storage = {};





--- Initializes the storage subsystem, creates the g_Storage object
function InitializeStorage()
	g_Storage = cStorage:new();
end





function cStorage:new(obj)
	obj = obj or {};
	setmetatable(obj, self);
	self.__index = self;
	return obj;
end




--- Loads cPlayerAreas for the specified player from the DB. Returns a cPlayerAreas object
function cStorage:LoadPlayerAreas(PlayerName)
	local res = cPlayerAreas:new();
	-- TODO: Load the areas from the DB, based on the player's location
	
	-- DEBUG: Insert a dummy area for testing purposes:
	res:AddArea(cCuboid(10, 0, 10, 20, 255, 20), false);
	return res;
end




