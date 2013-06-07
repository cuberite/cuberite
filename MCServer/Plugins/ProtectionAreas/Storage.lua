
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
-- Returns true if successful, false if not
function InitializeStorage()
	g_Storage = cStorage:new();
	if (not(g_Storage:OpenDB())) then
		return false;
	end
	
	return true;
end





function cStorage:new(obj)
	obj = obj or {};
	setmetatable(obj, self);
	self.__index = self;
	return obj;
end




--- Opens the DB and makes sure it has all the columns needed
-- Returns true if successful, false otherwise
function cStorage:OpenDB()
	local ErrCode, ErrMsg;
	self.DB, ErrCode, ErrMsg = sqlite3.open("ProtectionAreas.sqlite");
	if (self.DB == nil) then
		LOGWARNING(PluginPrefix .. "Cannot open ProtectionAreas.sqlite, error " .. ErrCode .. " (" .. ErrMsg ..")");
		return false;
	end
	
	if (
		not(self:CreateTable("Areas", {"ID INTEGER PRIMARY KEY AUTOINCREMENT", "MinX", "MaxX", "MinZ", "MaxZ", "WorldName", "CreatorUserName"})) or
		not(self:CreateTable("AllowedUsers", {"AreaID", "UserName"}))
	) then
		LOGWARNING(PluginPrefix .. "Cannot create DB tables!");
		return false;
	end
	
	return true;
end





--- Executes the SQL command given, calling the a_Callback for each result
-- If the SQL command fails, prints it out on the server console and returns false
-- Returns true on success
function cStorage:DBExec(a_SQL, a_Callback, a_CallbackParam)
	local ErrCode = self.DB:exec(a_SQL, a_Callback, a_CallbackParam);
	if (ErrCode ~= sqlite3.OK) then
		LOGWARNING(PluginPrefix .. "Error " .. ErrCode .. " (" .. self.DB:errmsg() .. 
			") while processing SQL command >>" .. a_SQL .. "<<"
		);
		return false;
	end
	return true;
end





--- Creates the table of the specified name and columns[]
-- If the table exists, any columns missing are added; existing data is kept
function cStorage:CreateTable(a_TableName, a_Columns)

	local sql = "CREATE TABLE IF NOT EXISTS '" .. a_TableName .. "' (";
	sql = sql .. table.concat(a_Columns, ", ");
	sql = sql .. ")";
	if (not(self:DBExec(sql))) then
		LOGWARNING(PluginPrefix .. "Cannot create DB Table " .. a_TableName);
		return false;
	end
	
	-- Check each column whether it exists
	-- Remove all the existing columns from a_Columns:
	local RemoveExistingColumn = function(UserData, NumCols, Values, Names)
		-- Remove the received column from a_Columns. Search for column name in the Names[] / Values[] pairs
		for i = 1, NumCols do
			if (Names[i] == "name") then
				local ColumnName = Values[i]:lower();
				-- Search the a_Columns if they have that column:
				for j = 1, #a_Columns do
					-- Cut away all column specifiers (after the first space), if any:
					local SpaceIdx = string.find(a_Columns[j], " ");
					if (SpaceIdx ~= nil) then
						SpaceIdx = SpaceIdx - 1;
					end
					local ColumnTemplate = string.lower(string.sub(a_Columns[j], 1, SpaceIdx));
					-- If it is a match, remove from a_Columns:
					if (ColumnTemplate == ColumnName) then
						table.remove(a_Columns, j);
						break;  -- for j
					end
				end  -- for j - a_Columns[]
			end
		end  -- for i - Names[] / Values[]
		return 0;
	end
	if (not(self:DBExec("PRAGMA table_info(" .. a_TableName .. ")", RemoveExistingColumn))) then
		LOGWARNING(PluginPrefix .. "Cannot query DB table structure");
		return false;
	end
	
	-- Create the missing columns
	-- a_Columns now contains only those columns that are missing in the DB
	if (#a_Columns > 0) then
		LOGINFO(PluginPrefix .. "Database table \"" .. a_TableName .. "\" is missing " .. #a_Columns .. " columns, fixing now.");
		for idx, ColumnName in ipairs(a_Columns) do
			if (not(self:DBExec("ALTER TABLE '" .. a_TableName .. "' ADD COLUMN " .. ColumnName))) then
				LOGWARNING(PluginPrefix .. "Cannot add DB table \"" .. a_TableName .. "\" column \"" .. ColumnName .. "\"");
				return false;
			end
		end
		LOGINFO(PluginPrefix .. "Database table \"" .. a_TableName .. "\" columns fixed.");
	end
	
	return true;
end





--- Loads cPlayerAreas for the specified player from the DB. Returns a cPlayerAreas object
function cStorage:LoadPlayerAreas(a_PlayerName, a_PlayerX, a_PlayerZ, a_WorldName)
	local res = cPlayerAreas:new();

	-- TODO: Load the areas from the DB, based on the player's location
	-- local sql = "SELECT MinX, MaxX, MinZ, MaxZ FROM Areas WHERE";
	
	LOGWARNING("cStorage:LoadPlayerAreas(): Not implemented yet!");

	-- DEBUG: Insert a dummy area for testing purposes:
	res:AddArea(cCuboid(10, 0, 10, 20, 255, 20), false);
	return res;
end





--- Adds a new area into the DB. a_AllowedNames is a table listing all the players that are allowed in the area
function cStorage:AddArea(a_Cuboid, a_WorldName, a_CreatorName, a_AllowedNames)
	-- Store the area in the DB
	local ID = -1;
	local function RememberID(UserData, NumCols, Values, Names)
		for i = 1, NumCols do
			if (Names[i] == "ID") then
				ID = Values[i];
			end
		end
		return 0;
	end
	local sql = 
		"INSERT INTO Areas (ID, MinX, MaxX, MinZ, MaxZ, WorldName, CreatorUserName) VALUES (NULL, " ..
		a_Cuboid.p1.x .. ", " .. a_Cuboid.p2.x .. ", " .. a_Cuboid.p1.z .. ", " .. a_Cuboid.p2.z .. 
		", '"  .. a_WorldName .. "', '" .. a_CreatorName ..
		"'); SELECT last_insert_rowid() as ID";
	if (not(self:DBExec(sql, RememberID))) then
		LOGWARNING(PluginPrefix .. "SQL Error while inserting new area");
		return false;
	end
	if (ID == -1) then
		LOGWARNING(PluginPrefix .. "SQL Error while retrieving INSERTion ID");
		return false;
	end
	
	-- Store each allowed player in the DB
	for idx, Name in ipairs(a_AllowedNames) do
		local sql = "INSERT INTO AllowedUsers (AreaID, UserName) VALUES (" .. ID .. ", '" .. Name .. "')";
		if (not(self:DBExec(sql))) then
			LOGWARNING(PluginPrefix .. "SQL Error while inserting new area's allowed player " .. Name);
		end
	end
	return true;
end





function cStorage:DelArea(a_WorldName, a_AreaID)
	-- Since all areas are stored in a single DB (for now), the worldname parameter isn't used at all
	-- Later if we change to a per-world DB, we'll need the world name
	
	-- Delete from both tables simultaneously
	local sql = 
		"DELETE FROM Areas          WHERE ID="     .. a_AreaID .. ";" ..
		"DELETE FROM AllowedPlayers WHERE AreaID=" .. a_AreaID;
	if (not(self:DBExec(sql))) then
		LOGWARNING(PluginPrefix .. "SQL error while deleting area " .. a_AreaID .. " from world \"" .. a_WorldName .. "\"");
		return false;
	end
	
	return true;
end





--- Removes the user from the specified area
function cStorage:RemoveUser(a_AreaID, a_UserName, a_WorldName)
	-- TODO
	LOGWARNING("cStorage:RemoveUser(): Not implemented yet!");
end





--- Removes the user from all areas in the specified world
function cStorage:RemoveUserAll(a_UserName, a_WorldName)
	-- TODO
	LOGWARNING("cStorage:RemoveUserAll(): Not implemented yet!");
end




