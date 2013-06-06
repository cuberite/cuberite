
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




--- Loads cPlayerAreas for the specified player from the DB. Returns a cPlayerAreas object
function cStorage:LoadPlayerAreas(PlayerName)
	local res = cPlayerAreas:new();
	-- TODO: Load the areas from the DB, based on the player's location
	
	-- DEBUG: Insert a dummy area for testing purposes:
	res:AddArea(cCuboid(10, 0, 10, 20, 255, 20), false);
	return res;
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
		not(self:CreateTable("Areas", {"ID INTEGER PRIMARY KEY AUTOINCREMENT", "MinX", "MaxX", "MinZ", "MaxZ", "CreatorUserName"})) or
		not(self:CreateTable("AllowedUsers", {"AreaID", "UserName"}))
	) then
		LOGWARNING(PluginPrefix .. "Cannot create DB tables!");
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
	local ErrCode = self.DB:exec(sql);
	if (ErrCode ~= sqlite3.OK) then
		LOGWARNING(PluginPrefix .. "Cannot create DB Table, error " .. ErrCode .. " (" .. self.DB:errmsg() .. ")");
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
	local ErrCode = self.DB:exec("PRAGMA table_info(" .. a_TableName .. ")", RemoveExistingColumn);
	if (ErrCode ~= sqlite3.OK) then
		LOGWARNING(PluginPrefix .. "Cannot query DB table structure, error " .. ErrCode .. " (" .. self.DB:errmsg() ..")");
		return false;
	end
	
	-- Create the missing columns
	-- a_Columns now contains only those columns that are missing in the DB
	if (#a_Columns > 0) then
		LOGINFO(PluginPrefix .. "Database table \"" .. a_TableName .. "\" is missing " .. #a_Columns .. " columns, fixing now.");
		for idx, ColumnName in ipairs(a_Columns) do
			local ErrCode = self.DB:exec("ALTER TABLE '" .. a_TableName .. "' ADD COLUMN " .. ColumnName);
			if (ErrCode ~= sqlite3.OK) then
				LOGWARNING(PluginPrefix .. "Cannot add DB table \"" .. a_TableName .. "\" column \"" .. ColumnName .. "\", error " .. ErrCode .. " (" .. self.DB:errmsg() ..")");
				return false;
			end
		end
		LOGINFO(PluginPrefix .. "Database table \"" .. a_TableName .. "\" columns fixed.");
	end
	
	return true;
end





--- Adds a new area into the DB. a_AllowedNames is a table listing all the players that are allowed in the area
function cStorage:AddArea(a_Cuboid, a_CreatorName, a_AllowedNames)
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
		"INSERT INTO Areas (ID, MinX, MaxX, MinZ, MaxZ, CreatorUserName) VALUES (NULL, " ..
		a_Cuboid.p1.x .. ", " .. a_Cuboid.p2.x .. ", " .. a_Cuboid.p1.z .. ", " .. a_Cuboid.p2.z .. ", '" .. a_CreatorName .. 
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
		end;
	end
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





