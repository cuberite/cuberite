
-- Global variables
PLUGIN = {};	-- Reference to own plugin object
ShouldDumpFunctions = true;  -- If set to true, all available functions are written to the API.txt file upon plugin initialization

g_DropSpensersToActivate = {};  -- A list of dispensers and droppers (as {World, X, Y Z} quadruplets) that are to be activated every tick

g_HungerReportTick = 10;





function Initialize(Plugin)
	PLUGIN = Plugin
	
	Plugin:SetName("Debuggers")
	Plugin:SetVersion(1)
	
	PluginManager = cRoot:Get():GetPluginManager()
	PluginManager:AddHook(Plugin, cPluginManager.HOOK_PLAYER_USING_BLOCK);
	PluginManager:AddHook(Plugin, cPluginManager.HOOK_PLAYER_USING_ITEM);
	PluginManager:AddHook(Plugin, cPluginManager.HOOK_TAKE_DAMAGE);
	PluginManager:AddHook(Plugin, cPluginManager.HOOK_TICK);
	PluginManager:AddHook(Plugin, cPluginManager.HOOK_CHAT);
	
	PluginManager:BindCommand("/le",      "debuggers", HandleListEntitiesCmd, "Shows a list of all the loaded entities");
	PluginManager:BindCommand("/ke",      "debuggers", HandleKillEntitiesCmd, "Kills all the loaded entities");
	PluginManager:BindCommand("/wool",    "debuggers", HandleWoolCmd,         "Sets all your armor to blue wool");
	PluginManager:BindCommand("/testwnd", "debuggers", HandleTestWndCmd,      "Opens up a window using plugin API");
	PluginManager:BindCommand("/gc",      "debuggers", HandleGCCmd,           "Activates the Lua garbage collector");
	PluginManager:BindCommand("/fast",    "debuggers", HandleFastCmd,         "Switches between fast and normal movement speed");
	PluginManager:BindCommand("/dash",    "debuggers", HandleDashCmd,         "Switches between fast and normal sprinting speed");
	PluginManager:BindCommand("/hunger",  "debuggers", HandleHungerCmd,       "Lists the current hunger-related variables");
	PluginManager:BindCommand("/poison",  "debuggers", HandlePoisonCmd,       "Sets food-poisoning for 15 seconds");
	PluginManager:BindCommand("/starve",  "debuggers", HandleStarveCmd,       "Sets the food level to zero");
	PluginManager:BindCommand("/fl",      "debuggers", HandleFoodLevelCmd,    "Sets the food level to the given value");

	-- Enable the following line for BlockArea / Generator interface testing:
	-- PluginManager:AddHook(Plugin, cPluginManager.HOOK_CHUNK_GENERATED);
	
	LOG("Initialized " .. Plugin:GetName() .. " v." .. Plugin:GetVersion())

	-- dump all available API functions and objects:
	if (ShouldDumpFunctions) then
		DumpAPI();
	end
	
	
	-- TestBlockAreas();
	-- TestSQLiteBindings();
	-- TestExpatBindings();

	return true
end;





function DumpAPI()
	LOG("Dumping all available functions to API.txt...");
	function dump (prefix, a, Output)
		for i, v in pairs (a) do
			if (type(v) == "table") then
				if (GetChar(i, 1) ~= ".") then
					if (v == _G) then
						LOG(prefix .. i .. " == _G, CYCLE, ignoring");
					elseif (v == _G.package) then
						LOG(prefix .. i .. " == _G.package, ignoring");
					else
						dump(prefix .. i .. ".", v, Output)
					end
				end
			elseif (type(v) == "function") then
				if (string.sub(i, 1, 2) ~= "__") then
					table.insert(Output, prefix .. i .. "()");
				end
			end
		end
	end

	local Output = {};
	dump("", _G, Output);

	table.sort(Output);
	local f = io.open("API.txt", "w");
	for i, n in ipairs(Output) do
		f:write(n, "\n");
	end
	f:close();
	LOG("API.txt written.");
end





function TestBlockAreas()
	LOG("Testing block areas...");
	
	-- Debug block area merging:
	local BA1 = cBlockArea();
	local BA2 = cBlockArea();
	if (BA1:LoadFromSchematicFile("schematics/test.schematic")) then
		if (BA2:LoadFromSchematicFile("schematics/fountain.schematic")) then
			BA2:SetRelBlockType(0, 0, 0, E_BLOCK_LAPIS_BLOCK);
			BA2:SetRelBlockType(1, 0, 0, E_BLOCK_LAPIS_BLOCK);
			BA2:SetRelBlockType(2, 0, 0, E_BLOCK_LAPIS_BLOCK);
			BA1:Merge(BA2, 1, 10, 1, cBlockArea.msImprint);
			BA1:SaveToSchematicFile("schematics/merge.schematic");
		end
	else
		BA1:Create(16, 16, 16);
	end
	
	-- Debug block area cuboid filling:
	BA1:FillRelCuboid(2, 9, 2, 8, 2, 8, cBlockArea.baTypes, E_BLOCK_GOLD_BLOCK);
	BA1:RelLine(2, 2, 2, 9, 8, 8, cBlockArea.baTypes or cBlockArea.baMetas, E_BLOCK_SAPLING, E_META_SAPLING_BIRCH);
	BA1:SaveToSchematicFile("schematics/fillrel.schematic");

	-- Debug block area mirroring:
	if (BA1:LoadFromSchematicFile("schematics/lt.schematic")) then
		BA1:MirrorXYNoMeta();
		BA1:SaveToSchematicFile("schematics/lt_XY.schematic");
		BA1:MirrorXYNoMeta();
		BA1:SaveToSchematicFile("schematics/lt_XY2.schematic");
		
		BA1:MirrorXZNoMeta();
		BA1:SaveToSchematicFile("schematics/lt_XZ.schematic");
		BA1:MirrorXZNoMeta();
		BA1:SaveToSchematicFile("schematics/lt_XZ2.schematic");
		
		BA1:MirrorYZNoMeta();
		BA1:SaveToSchematicFile("schematics/lt_YZ.schematic");
		BA1:MirrorYZNoMeta();
		BA1:SaveToSchematicFile("schematics/lt_YZ2.schematic");
	end
	
	-- Debug block area rotation:
	if (BA1:LoadFromSchematicFile("schematics/rot.schematic")) then
		BA1:RotateCWNoMeta();
		BA1:SaveToSchematicFile("schematics/rot1.schematic");
		BA1:RotateCWNoMeta();
		BA1:SaveToSchematicFile("schematics/rot2.schematic");
		BA1:RotateCWNoMeta();
		BA1:SaveToSchematicFile("schematics/rot3.schematic");
		BA1:RotateCWNoMeta();
		BA1:SaveToSchematicFile("schematics/rot4.schematic");
	end

	-- Debug block area rotation:
	if (BA1:LoadFromSchematicFile("schematics/rotm.schematic")) then
		BA1:RotateCCW();
		BA1:SaveToSchematicFile("schematics/rotm1.schematic");
		BA1:RotateCCW();
		BA1:SaveToSchematicFile("schematics/rotm2.schematic");
		BA1:RotateCCW();
		BA1:SaveToSchematicFile("schematics/rotm3.schematic");
		BA1:RotateCCW();
		BA1:SaveToSchematicFile("schematics/rotm4.schematic");
	end

	-- Debug block area mirroring:
	if (BA1:LoadFromSchematicFile("schematics/ltm.schematic")) then
		BA1:MirrorXY();
		BA1:SaveToSchematicFile("schematics/ltm_XY.schematic");
		BA1:MirrorXY();
		BA1:SaveToSchematicFile("schematics/ltm_XY2.schematic");
		
		BA1:MirrorXZ();
		BA1:SaveToSchematicFile("schematics/ltm_XZ.schematic");
		BA1:MirrorXZ();
		BA1:SaveToSchematicFile("schematics/ltm_XZ2.schematic");
		
		BA1:MirrorYZ();
		BA1:SaveToSchematicFile("schematics/ltm_YZ.schematic");
		BA1:MirrorYZ();
		BA1:SaveToSchematicFile("schematics/ltm_YZ2.schematic");
	end
	
	LOG("Block areas test ended");
end


	



function TestSQLiteBindings()
	LOG("Testing SQLite bindings...");
	
	-- Debug SQLite binding
	local TestDB, ErrCode, ErrMsg = sqlite3.open("test.sqlite");
	if (TestDB ~= nil) then
		local function ShowRow(UserData, NumCols, Values, Names)
			assert(UserData == 'UserData');
			LOG("New row");
			for i = 1, NumCols do
				LOG("  " .. Names[i] .. " = " .. Values[i]);
			end
			return 0;
		end
		local sql = [=[
			CREATE TABLE numbers(num1,num2,str);
			INSERT INTO numbers VALUES(1, 11, "ABC");
			INSERT INTO numbers VALUES(2, 22, "DEF");
			INSERT INTO numbers VALUES(3, 33, "UVW");
			INSERT INTO numbers VALUES(4, 44, "XYZ");
			SELECT * FROM numbers;
		]=]
		local Res = TestDB:exec(sql, ShowRow, 'UserData');
		if (Res ~= sqlite3.OK) then
			LOG("TestDB:exec() failed: " .. Res .. " (" .. TestDB:errmsg() .. ")");
		end;
		TestDB:close();
	else
		-- This happens if for example SQLite cannot open the file (eg. a folder with the same name exists)
		LOG("SQLite3 failed to open DB! (" .. ErrCode .. ", " .. ErrMsg ..")");
	end
	
	LOG("SQLite bindings test ended");
end





function TestExpatBindings()
	LOG("Testing Expat bindings...");
	
	-- Debug LuaExpat bindings:
	local count = 0
	callbacks = {
	    StartElement = function (parser, name)
	        LOG("+ " .. string.rep(" ", count) .. name);
	        count = count + 1;
	    end,
	    EndElement = function (parser, name)
	        count = count - 1;
	        LOG("- " .. string.rep(" ", count) .. name);
	    end
	}

	local p = lxp.new(callbacks);
	p:parse("<elem1>\nnext line\nanother line");
	p:parse("text\n");
	p:parse("<elem2/>\n");
	p:parse("more text");
	p:parse("</elem1>");
	p:parse("\n");
	p:parse();  -- finishes the document
	p:close();  -- closes the parser
	
	LOG("Expat bindings test ended");
end





function OnUsingBlazeRod(Player, BlockX, BlockY, BlockZ, BlockFace, CursorX, CursorY, CursorZ)
	-- Magic rod of query: show block types and metas for both neighbors of the pointed face
	local Type, Meta, Valid = Player:GetWorld():GetBlockTypeMeta(BlockX, BlockY, BlockZ, Type, Meta);

	if (Type == E_BLOCK_AIR) then
		Player:SendMessage(cChatColor.LightGray .. "Block {" .. BlockX .. ", " .. BlockY .. ", " .. BlockZ .. "}: air:" .. Meta);
	else
		local TempItem = cItem(Type, 1, Meta);
		Player:SendMessage(cChatColor.LightGray .. "Block {" .. BlockX .. ", " .. BlockY .. ", " .. BlockZ .. "}: " .. ItemToFullString(TempItem) .. " (" .. Type .. ":" .. Meta .. ")");
	end
	
	local X, Y, Z = AddFaceDirection(BlockX, BlockY, BlockZ, BlockFace);
	Valid, Type, Meta = Player:GetWorld():GetBlockTypeMeta(X, Y, Z, Type, Meta);
	if (Type == E_BLOCK_AIR) then
		Player:SendMessage(cChatColor.LightGray .. "Block {" .. X .. ", " .. Y .. ", " .. Z .. "}: air:" .. Meta);
	else
		local TempItem = cItem(Type, 1, Meta);
		Player:SendMessage(cChatColor.LightGray .. "Block {" .. X .. ", " .. Y .. ", " .. Z .. "}: " .. ItemToFullString(TempItem) .. " (" .. Type .. ":" .. Meta .. ")");
	end
	return false;
end





function OnUsingDiamond(Player, BlockX, BlockY, BlockZ, BlockFace, CursorX, CursorY, CursorZ)
	-- Rclk with a diamond to test block area cropping and expanding
	local Area = cBlockArea();
	Area:Read(Player:GetWorld(),
		BlockX - 19, BlockX + 19,
		BlockY - 7, BlockY + 7,
		BlockZ - 19, BlockZ + 19
	);

	LOG("Size before cropping: " .. Area:GetSizeX() .. " x " .. Area:GetSizeY() .. " x " .. Area:GetSizeZ());
	Area:DumpToRawFile("crop0.dat");
	
	Area:Crop(2, 3, 0, 0, 0, 0);
	LOG("Size after cropping 1: " .. Area:GetSizeX() .. " x " .. Area:GetSizeY() .. " x " .. Area:GetSizeZ());
	Area:DumpToRawFile("crop1.dat");
	
	Area:Crop(2, 3, 0, 0, 0, 0);
	LOG("Size after cropping 2: " .. Area:GetSizeX() .. " x " .. Area:GetSizeY() .. " x " .. Area:GetSizeZ());
	Area:DumpToRawFile("crop2.dat");
	
	Area:Expand(2, 3, 0, 0, 0, 0);
	LOG("Size after expanding 1: " .. Area:GetSizeX() .. " x " .. Area:GetSizeY() .. " x " .. Area:GetSizeZ());
	Area:DumpToRawFile("expand1.dat");
	
	Area:Expand(3, 2, 1, 1, 0, 0);
	LOG("Size after expanding 2: " .. Area:GetSizeX() .. " x " .. Area:GetSizeY() .. " x " .. Area:GetSizeZ());
	Area:DumpToRawFile("expand2.dat");
	
	Area:Crop(0, 0, 0, 0, 3, 2);
	LOG("Size after cropping 3: " .. Area:GetSizeX() .. " x " .. Area:GetSizeY() .. " x " .. Area:GetSizeZ());
	Area:DumpToRawFile("crop3.dat");
	
	Area:Crop(0, 0, 3, 2, 0, 0);
	LOG("Size after cropping 4: " .. Area:GetSizeX() .. " x " .. Area:GetSizeY() .. " x " .. Area:GetSizeZ());
	Area:DumpToRawFile("crop4.dat");

	LOG("Crop test done");
	Player:SendMessage("Crop / expand test done.");
	return false;
end





function OnUsingEyeOfEnder(Player, BlockX, BlockY, BlockZ)
	-- Rclk with an eye of ender places a predefined schematic at the cursor
	local Area = cBlockArea();
	if not(Area:LoadFromSchematicFile("schematics/test.schematic")) then
		LOG("Loading failed");
		return false;
	end
	LOG("Schematic loaded, placing now.");
	Area:Write(Player:GetWorld(), BlockX, BlockY, BlockZ);
	LOG("Done.");
	return false;
end





function OnUsingEnderPearl(Player, BlockX, BlockY, BlockZ, BlockFace, CursorX, CursorY, CursorZ)
	-- Rclk with an ender pearl saves a predefined area around the cursor into a .schematic file. Also tests area copying
	local Area = cBlockArea();
	if not(Area:Read(Player:GetWorld(), 
		BlockX - 8, BlockX + 8, BlockY - 8, BlockY + 8, BlockZ - 8, BlockZ + 8)
	) then
		LOG("LUA: Area couldn't be read");
		return false;
	end
	LOG("LUA: Area read, copying now.");
	local Area2 = cBlockArea();
	Area2:CopyFrom(Area);
	LOG("LUA: Copied, now saving.");
	if not(Area2:SaveToSchematicFile("schematics/test.schematic")) then
		LOG("LUA: Cannot save schematic file.");
		return false;
	end
	LOG("LUA: Done.");
	return false;
end





function OnUsingRedstoneTorch(Player, BlockX, BlockY, BlockZ, BlockFace, CursorX, CursorY, CursorZ)
	-- Redstone torch activates a rapid dispenser / dropper discharge (at every tick):
	local BlockType = Player:GetWorld():GetBlock(BlockX, BlockY, BlockZ);
	if (BlockType == E_BLOCK_DISPENSER) then
		table.insert(g_DropSpensersToActivate, {World = Player:GetWorld(), x = BlockX, y = BlockY, z = BlockZ});
		Player:SendMessage("Dispenser at {" .. BlockX .. ", " .. BlockY .. ", " .. BlockZ .. "} discharging");
		return true;
	elseif (BlockType == E_BLOCK_DROPPER) then
		table.insert(g_DropSpensersToActivate, {World = Player:GetWorld(), x = BlockX, y = BlockY, z = BlockZ});
		Player:SendMessage("Dropper at {" .. BlockX .. ", " .. BlockY .. ", " .. BlockZ .. "} discharging");
		return true;
	else
		Player:SendMessage("Neither a dispenser nor a dropper at {" .. BlockX .. ", " .. BlockY .. ", " .. BlockZ .. "}: " .. BlockType);
	end
	return false;
end





function OnPlayerUsingItem(Player, BlockX, BlockY, BlockZ, BlockFace, CursorX, CursorY, CursorZ)

	-- dont check if the direction is in the air
	if (BlockFace == BLOCK_FACE_NONE) then
		return false
	end

	local HeldItem = Player:GetEquippedItem();
	local HeldItemType = HeldItem.m_ItemType;

	if (HeldItemType == E_ITEM_STICK) then
		-- Magic sTick of ticking: set the pointed block for ticking at the next tick
		Player:SendMessage(cChatColor.LightGray .. "Setting next block tick to {" .. BlockX .. ", " .. BlockY .. ", " .. BlockZ .. "}")
		Player:GetWorld():SetNextBlockTick(BlockX, BlockY, BlockZ);
		return true
	elseif (HeldItemType == E_ITEM_BLAZE_ROD) then
		return OnUsingBlazeRod(Player, BlockX, BlockY, BlockZ, BlockFace, CursorX, CursorY, CursorZ);
	elseif (HeldItemType == E_ITEM_DIAMOND) then
		return OnUsingDiamond(Player, BlockX, BlockY, BlockZ, BlockFace, CursorX, CursorY, CursorZ);
	elseif (HeldItemType == E_ITEM_EYE_OF_ENDER) then
		return OnUsingEyeOfEnder(Player, BlockX, BlockY, BlockZ, BlockFace, CursorX, CursorY, CursorZ);
	elseif (HeldItemType == E_ITEM_ENDER_PEARL) then
		return OnUsingEnderPearl(Player, BlockX, BlockY, BlockZ, BlockFace, CursorX, CursorY, CursorZ);
	end
	return false;
end





function OnPlayerUsingBlock(Player, BlockX, BlockY, BlockZ, BlockFace, CursorX, CursorY, CursorZ, BlockType, BlockMeta)
	-- dont check if the direction is in the air
	if (BlockFace == BLOCK_FACE_NONE) then
		return false
	end

	local HeldItem = Player:GetEquippedItem();
	local HeldItemType = HeldItem.m_ItemType;

	if (HeldItemType == E_BLOCK_REDSTONE_TORCH_ON) then
		return OnUsingRedstoneTorch(Player, BlockX, BlockY, BlockZ, BlockFace, CursorX, CursorY, CursorZ);
	end
end





function OnTakeDamage(Receiver, TDI)
	-- Receiver is cPawn
	-- TDI is TakeDamageInfo

	LOG(Receiver:GetClass() .. " was dealt " .. DamageTypeToString(TDI.DamageType) .. " damage: Raw " .. TDI.RawDamage .. ", Final " .. TDI.FinalDamage .. " (" .. (TDI.RawDamage - TDI.FinalDamage) .. " covered by armor)");
	return false;
end





--- When set to a positive number, the following OnTick() will perform GC and decrease until 0 again
GCOnTick = 0;





function OnTick()
	-- Activate all dropspensers in the g_DropSpensersToActivate list:
	local ActivateDrSp = function(DropSpenser)
		if (DropSpenser:GetContents():GetFirstUsedSlot() == -1) then
			return true;
		end
		DropSpenser:Activate();
		return false;
	end
	-- Walk the list backwards, because we're removing some items
	local idx = #g_DropSpensersToActivate;
	for i = idx, 1, -1 do
		local DrSp = g_DropSpensersToActivate[i];
		if not(DrSp.World:DoWithDropSpenserAt(DrSp.x, DrSp.y, DrSp.z, ActivateDrSp)) then
			table.remove(g_DropSpensersToActivate, i);
		end
	end
	
	
	-- If GCOnTick > 0, do a garbage-collect and decrease by one
	if (GCOnTick > 0) then
		collectgarbage();
		GCOnTick = GCOnTick - 1;
	end
	
	--[[
	if (g_HungerReportTick > 0) then
		g_HungerReportTick = g_HungerReportTick - 1;
	else
		g_HungerReportTick = 10;
		cRoot:Get():GetDefaultWorld():ForEachPlayer(
			function(a_Player)
				a_Player:SendMessage("FoodStat: " .. a_Player:GetFoodLevel() .. " / " .. a_Player:GetFoodExhaustionLevel());
			end
		);
	end
	]]
	
	return false;
end





function OnChunkGenerated(World, ChunkX, ChunkZ, ChunkDesc)
	-- Test ChunkDesc / BlockArea interaction
	local BlockArea = cBlockArea();
	ChunkDesc:ReadBlockArea(BlockArea, 0, 15, 50, 70, 0, 15);
	
	-- BlockArea:SaveToSchematicFile("ChunkBlocks_" .. ChunkX .. "_" .. ChunkZ .. ".schematic");

	ChunkDesc:WriteBlockArea(BlockArea, 5, 115, 5);
	return false;
end





function OnChat(a_Player, a_Message)
	return false, "blabla " .. a_Message;
end





-- Function "round" copied from http://lua-users.org/wiki/SimpleRound
function round(num, idp)
	local mult = 10^(idp or 0)
	if num >= 0 then return math.floor(num * mult + 0.5) / mult
	else return math.ceil(num * mult - 0.5) / mult end
end





function HandleListEntitiesCmd(Split, Player)
	local NumEntities = 0;
	
	local ListEntity = function(Entity)
		if (Entity:IsDestroyed()) then
			-- The entity has already been destroyed, don't list it
			return false;
		end;
		Player:SendMessage("  " .. Entity:GetUniqueID() .. ": " .. Entity:GetClass() .. "   {" .. round(Entity:GetPosX(), 2) .. ", " .. round(Entity:GetPosY(), 2) .. ", " .. round(Entity:GetPosZ(), 2) .."}");
		NumEntities = NumEntities + 1;
	end
	
	Player:SendMessage("Listing all entities...");
	Player:GetWorld():ForEachEntity(ListEntity);
	Player:SendMessage("List finished, " .. NumEntities .. " entities listed");
	return true;
end





function HandleKillEntitiesCmd(Split, Player)
	local NumEntities = 0;
	
	local KillEntity = function(Entity)
		-- kill everything except for players:
		if (Entity:GetEntityType() ~= cEntity.etPlayer) then
			Entity:Destroy();
			NumEntities = NumEntities + 1;
		end;
	end
	
	Player:SendMessage("Killing all entities...");
	Player:GetWorld():ForEachEntity(KillEntity);
	Player:SendMessage("Killed " .. NumEntities .. " entities.");
	return true;
end





function HandleWoolCmd(Split, Player)
	local Wool = cItem(E_BLOCK_WOOL, 1, E_META_WOOL_BLUE);
	Player:GetInventory():SetArmorSlot(0, Wool);
	Player:GetInventory():SetArmorSlot(1, Wool);
	Player:GetInventory():SetArmorSlot(2, Wool);
	Player:GetInventory():SetArmorSlot(3, Wool);
	Player:SendMessage("You have been bluewooled :)");
	return true;
end





function HandleTestWndCmd(a_Split, a_Player)
	local WindowType  = cWindow.Hopper;
	local WindowSizeX = 5;
	local WindowSizeY = 1;
	if (#a_Split == 4) then
		WindowType  = tonumber(a_Split[2]);
		WindowSizeX = tonumber(a_Split[3]);
		WindowSizeY = tonumber(a_Split[4]);
	elseif (#a_Split ~= 1) then
		a_Player:SendMessage("Usage: /testwnd [WindowType WindowSizeX WindowSizeY]");
		return true;
	end
	
	-- Test out the OnClosing callback's ability to refuse to close the window
	local attempt = 1;
	local OnClosing = function(Window, Player, CanRefuse)
		Player:SendMessage("Window closing attempt #" .. attempt .. "; CanRefuse = " .. tostring(CanRefuse));
		attempt = attempt + 1;
		return CanRefuse and (attempt <= 3);  -- refuse twice, then allow, unless CanRefuse is set to true
	end
	
	-- Log the slot changes
	local OnSlotChanged = function(Window, SlotNum)
		LOG("Window \"" .. Window:GetWindowTitle() .. "\" slot " .. SlotNum .. " changed.");
	end
	
	local Window = cLuaWindow(WindowType, WindowSizeX, WindowSizeY, "TestWnd");
	local Item2 = cItem(E_ITEM_DIAMOND_SWORD, 1, 0, "1=1");
	local Item3 = cItem(E_ITEM_DIAMOND_SHOVEL);
	Item3.m_Enchantments:SetLevel(cEnchantments.enchUnbreaking, 4);
	local Item4 = cItem(Item3);  -- Copy
	Item4.m_Enchantments:SetLevel(cEnchantments.enchEfficiency, 3);  -- Add enchantment
	Item4.m_Enchantments:SetLevel(cEnchantments.enchUnbreaking, 5);  -- Overwrite existing level
	local Item5 = cItem(E_ITEM_DIAMOND_CHESTPLATE, 1, 0, "thorns=1;unbreaking=3");
	Window:SetSlot(a_Player, 0, cItem(E_ITEM_DIAMOND, 64));
	Window:SetSlot(a_Player, 1, Item2);
	Window:SetSlot(a_Player, 2, Item3);
	Window:SetSlot(a_Player, 3, Item4);
	Window:SetSlot(a_Player, 4, Item5);
	Window:SetOnClosing(OnClosing);
	Window:SetOnSlotChanged(OnSlotChanged);
	
	a_Player:OpenWindow(Window);
	
	-- To make sure that the object has the correct life-management in Lua,
	-- let's garbage-collect in the following few ticks
	GCOnTick = 10;
	
	return true;
end





function HandleGCCmd(a_Split, a_Player)
	collectgarbage();
	return true;
end






function HandleFastCmd(a_Split, a_Player)
	if (a_Player:GetNormalMaxSpeed() <= 0.11) then
		-- The player has normal speed, set double speed:
		a_Player:SetNormalMaxSpeed(0.2);
		a_Player:SendMessage("You are now fast");
	else
		-- The player has fast speed, set normal speed:
		a_Player:SetNormalMaxSpeed(0.1);
		a_Player:SendMessage("Back to normal speed");
	end
	return true;
end





function HandleDashCmd(a_Split, a_Player)
	if (a_Player:GetSprintingMaxSpeed() <= 0.14) then
		-- The player has normal sprinting speed, set double Sprintingspeed:
		a_Player:SetSprintingMaxSpeed(0.4);
		a_Player:SendMessage("You can now sprint very fast");
	else
		-- The player has fast sprinting speed, set normal sprinting speed:
		a_Player:SetSprintingMaxSpeed(0.13);
		a_Player:SendMessage("Back to normal sprinting");
	end
	return true;
end;





function HandleHungerCmd(a_Split, a_Player)
	a_Player:SendMessage("FoodLevel: " .. a_Player:GetFoodLevel());
	a_Player:SendMessage("FoodSaturationLevel: " .. a_Player:GetFoodSaturationLevel());
	a_Player:SendMessage("FoodTickTimer: " .. a_Player:GetFoodTickTimer());
	a_Player:SendMessage("FoodExhaustionLevel: " .. a_Player:GetFoodExhaustionLevel());
	a_Player:SendMessage("FoodPoisonedTicksRemaining: " .. a_Player:GetFoodPoisonedTicksRemaining());
	return true;
end





function HandlePoisonCmd(a_Split, a_Player)
	a_Player:FoodPoison(15 * 20);
	return true;
end





function HandleStarveCmd(a_Split, a_Player)
	a_Player:SetFoodLevel(0);
	a_Player:SendMessage("You are now starving");
	return true;
end





function HandleFoodLevelCmd(a_Split, a_Player)
	if (#a_Split ~= 2) then
		a_Player:SendMessage("Missing an argument: the food level to set");
		return true;
	end
	
	a_Player:SetFoodLevel(tonumber(a_Split[2]));
	a_Player:SendMessage("Food level set to " .. a_Player:GetFoodLevel());
	return true;
end




