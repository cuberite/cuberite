
-- Global variables
g_DropSpensersToActivate = {};  -- A list of dispensers and droppers (as {World, X, Y Z} quadruplets) that are to be activated every tick
g_HungerReportTick = 10;
g_ShowFoodStats = false;  -- When true, each player's food stats are sent to them every 10 ticks






function Initialize(Plugin)
	Plugin:SetName("Debuggers")
	Plugin:SetVersion(1)
	
	--[[
	-- Test multiple hook handlers:
	cPluginManager.AddHook(cPluginManager.HOOK_TICK,                         OnTick1);
	cPluginManager.AddHook(cPluginManager.HOOK_TICK,                         OnTick2);
	--]]
	
	cPluginManager:AddHook(cPluginManager.HOOK_PLAYER_USING_BLOCK,           OnPlayerUsingBlock);
	cPluginManager:AddHook(cPluginManager.HOOK_PLAYER_USING_ITEM,            OnPlayerUsingItem);
	cPluginManager:AddHook(cPluginManager.HOOK_TAKE_DAMAGE,                  OnTakeDamage);
	cPluginManager:AddHook(cPluginManager.HOOK_TICK,                         OnTick);
	cPluginManager:AddHook(cPluginManager.HOOK_CHAT,                         OnChat);
	cPluginManager:AddHook(cPluginManager.HOOK_PLAYER_RIGHT_CLICKING_ENTITY, OnPlayerRightClickingEntity);
	cPluginManager:AddHook(cPluginManager.HOOK_WORLD_TICK,                   OnWorldTick);
	cPluginManager:AddHook(cPluginManager.HOOK_CHUNK_GENERATED,              OnChunkGenerated);
	cPluginManager:AddHook(cPluginManager.HOOK_PLUGINS_LOADED,               OnPluginsLoaded);
	cPluginManager:AddHook(cPluginManager.HOOK_PLUGIN_MESSAGE,               OnPluginMessage);

	PM = cRoot:Get():GetPluginManager();
	PM:BindCommand("/le",      "debuggers", HandleListEntitiesCmd, "- Shows a list of all the loaded entities");
	PM:BindCommand("/ke",      "debuggers", HandleKillEntitiesCmd, "- Kills all the loaded entities");
	PM:BindCommand("/wool",    "debuggers", HandleWoolCmd,         "- Sets all your armor to blue wool");
	PM:BindCommand("/testwnd", "debuggers", HandleTestWndCmd,      "- Opens up a window using plugin API");
	PM:BindCommand("/gc",      "debuggers", HandleGCCmd,           "- Activates the Lua garbage collector");
	PM:BindCommand("/fast",    "debuggers", HandleFastCmd,         "- Switches between fast and normal movement speed");
	PM:BindCommand("/dash",    "debuggers", HandleDashCmd,         "- Switches between fast and normal sprinting speed");
	PM:BindCommand("/hunger",  "debuggers", HandleHungerCmd,       "- Lists the current hunger-related variables");
	PM:BindCommand("/poison",  "debuggers", HandlePoisonCmd,       "- Sets food-poisoning for 15 seconds");
	PM:BindCommand("/starve",  "debuggers", HandleStarveCmd,       "- Sets the food level to zero");
	PM:BindCommand("/fl",      "debuggers", HandleFoodLevelCmd,    "- Sets the food level to the given value");
	PM:BindCommand("/spidey",  "debuggers", HandleSpideyCmd,       "- Shoots a line of web blocks until it hits non-air");
	PM:BindCommand("/ench",    "debuggers", HandleEnchCmd,         "- Provides an instant dummy enchantment window");
	PM:BindCommand("/fs",      "debuggers", HandleFoodStatsCmd,    "- Turns regular foodstats message on or off");
	PM:BindCommand("/arr",     "debuggers", HandleArrowCmd,        "- Creates an arrow going away from the player");
	PM:BindCommand("/fb",      "debuggers", HandleFireballCmd,     "- Creates a ghast fireball as if shot by the player");
	PM:BindCommand("/xpa",     "debuggers", HandleAddExperience,   "- Adds 200 experience to the player");
	PM:BindCommand("/xpr",     "debuggers", HandleRemoveXp,        "- Remove all xp");
	PM:BindCommand("/fill",    "debuggers", HandleFill,            "- Fills all block entities in current chunk with junk");
	PM:BindCommand("/fr",      "debuggers", HandleFurnaceRecipe,   "- Shows the furnace recipe for the currently held item");
	PM:BindCommand("/ff",      "debuggers", HandleFurnaceFuel,     "- Shows how long the currently held item would burn in a furnace");

	Plugin:AddWebTab("Debuggers", HandleRequest_Debuggers);

	-- Enable the following line for BlockArea / Generator interface testing:
	-- PluginManager:AddHook(Plugin, cPluginManager.HOOK_CHUNK_GENERATED);
	
	LOG("Initialized " .. Plugin:GetName() .. " v." .. Plugin:GetVersion())

	-- TestBlockAreas();
	-- TestSQLiteBindings();
	-- TestExpatBindings();

	return true
end;






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





function OnTick1()
	-- For testing multiple hook handlers per plugin
	LOGINFO("Tick1");
end





function OnTick2()
	-- For testing multiple hook handlers per plugin
	LOGINFO("Tick2");
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
	
	return false;
end





function OnWorldTick(a_World, a_Dt)
	-- Report food stats, if switched on:
	local Tick = a_World:GetWorldAge();
	if (not(g_ShowFoodStats) or (math.mod(Tick, 10) ~= 0)) then
		return false;
	end
	a_World:ForEachPlayer(
		function(a_Player)
			a_Player:SendMessage(
				tostring(Tick / 10) .. 
				" > FS: fl " .. a_Player:GetFoodLevel() .. 
				"; sat " .. a_Player:GetFoodSaturationLevel() ..
				"; exh " .. a_Player:GetFoodExhaustionLevel()
			);
		end
	);
end





function OnChat(a_Player, a_Message)
	return false, "blabla " .. a_Message;
end





function OnPlayerRightClickingEntity(a_Player, a_Entity)
	LOG("Player " .. a_Player:GetName() .. " right-clicking entity ID " .. a_Entity:GetUniqueID() .. ", a " .. a_Entity:GetClass());
	return false;
end





function OnPluginsLoaded()
	LOG("All plugins loaded");
end





function OnChunkGenerated(a_World, a_ChunkX, a_ChunkZ, a_ChunkDesc)
	-- Get the topmost block coord:
	local Height = a_ChunkDesc:GetHeight(0, 0);
	
	-- Create a sign there:
	a_ChunkDesc:SetBlockTypeMeta(0, Height + 1, 0, E_BLOCK_SIGN_POST, 0);
	local BlockEntity = a_ChunkDesc:GetBlockEntity(0, Height + 1, 0);
	if (BlockEntity ~= nil) then
		local SignEntity = tolua.cast(BlockEntity, "cSignEntity");
		SignEntity:SetLines("Chunk:", tonumber(a_ChunkX) .. ", " .. tonumber(a_ChunkZ), "", "(Debuggers)");
	end

	-- Update the heightmap:
	a_ChunkDesc:SetHeight(0, 0, Height + 1);
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
		local cls = Entity:GetClass();
		Player:SendMessage("  " .. Entity:GetUniqueID() .. ": " .. cls .. "   {" .. round(Entity:GetPosX(), 2) .. ", " .. round(Entity:GetPosY(), 2) .. ", " .. round(Entity:GetPosZ(), 2) .."}");
		if (cls == "cPickup") then
			local Pickup = Entity;
			tolua.cast(Pickup, "cPickup");
			Player:SendMessage("    Age: " .. Pickup:GetAge() .. ", IsCollected: " .. tostring(Pickup:IsCollected()));
		end
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
	local WindowType  = cWindow.wtHopper;
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
	a_Player:SetFoodSaturationLevel(5);
	a_Player:SetFoodExhaustionLevel(0);
	a_Player:SendMessage(
		"Food level set to " .. a_Player:GetFoodLevel() .. 
		", saturation reset to " .. a_Player:GetFoodSaturationLevel() ..
		" and exhaustion reset to " .. a_Player:GetFoodExhaustionLevel()
	);
	return true;
end





function HandleSpideyCmd(a_Split, a_Player)
	-- Place a line of cobwebs from the player's eyes until non-air block, in the line-of-sight of the player
	local World = a_Player:GetWorld();

	local Callbacks = {
		OnNextBlock = function(a_BlockX, a_BlockY, a_BlockZ, a_BlockType, a_BlockMeta)
			if (a_BlockType ~= E_BLOCK_AIR) then
				-- abort the trace
				return true;
			end
			World:SetBlock(a_BlockX, a_BlockY, a_BlockZ, E_BLOCK_COBWEB, 0);
		end
	};
	
	local EyePos = a_Player:GetEyePosition();
	local LookVector = a_Player:GetLookVector();
	LookVector:Normalize();
	
	-- Start cca 2 blocks away from the eyes
	local Start = EyePos + LookVector + LookVector;
	local End = EyePos + LookVector * 50;
	
	cLineBlockTracer.Trace(World, Callbacks, Start.x, Start.y, Start.z, End.x, End.y, End.z);
	
	return true;
end





function HandleEnchCmd(a_Split, a_Player)
	local Wnd = cLuaWindow(cWindow.wtEnchantment, 1, 1, "Ench");
	a_Player:OpenWindow(Wnd);
	Wnd:SetProperty(0, 10);
	Wnd:SetProperty(1, 15);
	Wnd:SetProperty(2, 25);
	return true;
end





function HandleFoodStatsCmd(a_Split, a_Player)
	g_ShowFoodStats = not(g_ShowFoodStats);
	return true;
end





function HandleArrowCmd(a_Split, a_Player)
	local World = a_Player:GetWorld();
	local Pos = a_Player:GetEyePosition();
	local Speed = a_Player:GetLookVector();
	Speed:Normalize();
	Pos = Pos + Speed;
	
	World:CreateProjectile(Pos.x, Pos.y, Pos.z, cProjectileEntity.pkArrow, a_Player, Speed * 10);
	return true;
end





function HandleFireballCmd(a_Split, a_Player)
	local World = a_Player:GetWorld();
	local Pos = a_Player:GetEyePosition();
	local Speed = a_Player:GetLookVector();
	Speed:Normalize();
	Pos = Pos + Speed * 2;
	
	World:CreateProjectile(Pos.x, Pos.y, Pos.z, cProjectileEntity.pkGhastFireball, a_Player, Speed * 10);
	return true;
end




function HandleAddExperience(a_Split, a_Player)
	a_Player:DeltaExperience(200);

	return true;
end





function HandleRemoveXp(a_Split, a_Player)
	a_Player:SetCurrentExperience(0);
	
	return true;
end





function HandleFill(a_Split, a_Player)
	local World = a_Player:GetWorld();
	local ChunkX = a_Player:GetChunkX();
	local ChunkZ = a_Player:GetChunkZ();
	World:ForEachBlockEntityInChunk(ChunkX, ChunkZ,
		function(a_BlockEntity)
			local BlockType = a_BlockEntity:GetBlockType();
			if (
				(BlockType == E_BLOCK_CHEST) or
				(BlockType == E_BLOCK_DISPENSER) or
				(BlockType == E_BLOCK_DROPPER) or
				(BlockType == E_BLOCK_FURNACE) or
				(BlockType == E_BLOCK_HOPPER)
			) then
				-- This block entity has items (inherits from cBlockEntityWithItems), fill it:
				-- Note that we're not touching lit furnaces, don't wanna mess them up
				local EntityWithItems = tolua.cast(a_BlockEntity, "cBlockEntityWithItems");
				local ItemGrid = EntityWithItems:GetContents();
				local NumSlots = ItemGrid:GetNumSlots();
				local ItemToSet = cItem(E_ITEM_GOLD_NUGGET);
				for i = 0, NumSlots - 1 do
					if (ItemGrid:GetSlot(i):IsEmpty()) then
						ItemGrid:SetSlot(i, ItemToSet);
					end
				end
			end
		end
	);
	return true;
end





function HandleFurnaceRecipe(a_Split, a_Player)
	local HeldItem = a_Player:GetEquippedItem();
	local Out, NumTicks, In = cRoot:GetFurnaceRecipe(HeldItem);
	if (Out ~= nil) then
		a_Player:SendMessage(
			"Furnace turns " .. ItemToFullString(In) ..
			" to " .. ItemToFullString(Out) ..
			" in " .. NumTicks .. " ticks (" ..
			tostring(NumTicks / 20) .. " seconds)."
		);
	else
		a_Player:SendMessage("There is no furnace recipe that would smelt " .. ItemToString(HeldItem));
	end
	return true;
end





function HandleFurnaceFuel(a_Split, a_Player)
	local HeldItem = a_Player:GetEquippedItem();
	local NumTicks = cRoot:GetFurnaceFuelBurnTime(HeldItem);
	if (NumTicks > 0) then
		a_Player:SendMessage(
			ItemToFullString(HeldItem) .. " would power a furnace for " .. NumTicks ..
			" ticks (" .. tostring(NumTicks / 20) .. " seconds)."
		);
	else
		a_Player:SendMessage(ItemToString(HeldItem) .. " will not power furnaces.");
	end
	return true;
end





function HandleRequest_Debuggers(a_Request)
	local FolderContents = cFile:GetFolderContents("./");
	return "<p>The following objects have been returned by cFile:GetFolderContents():<ul><li>" .. table.concat(FolderContents, "</li><li>") .. "</li></ul></p>";
end





function OnPluginMessage(a_Client, a_Channel, a_Message)
	LOGINFO("Received a plugin message from client " .. a_Client:GetUsername() .. ": channel '" .. a_Channel .. "', message '" .. a_Message .. "'");
	
	if (a_Channel == "REGISTER") then
		if (a_Message:find("WECUI")) then
			-- The client has WorldEditCUI mod installed, test the comm by sending a few WECUI messages:
			--[[
			WECUI messages have the following generic format:
			<shape>|<params>
			If shape is p (cuboid selection), the params are sent individually for each corner click and have the following format:
			<point-index>|<x>|<y>|<z>|<volume>
			point-index is 0 or 1 (lclk / rclk)
			volume is the 3D volume of the current cuboid selected (all three coords' deltas multiplied), including the edge blocks; -1 if N/A
			--]]
			-- Select a 51 * 51 * 51 block cuboid:
			a_Client:SendPluginMessage("WECUI", "p|0|50|50|50|-1");
			a_Client:SendPluginMessage("WECUI", "p|1|100|100|100|132651");  -- 132651 = 51 * 51 * 51
		end
	end
end




