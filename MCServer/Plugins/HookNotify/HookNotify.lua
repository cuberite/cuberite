
-- Global variables
PLUGIN = {}	-- Reference to own plugin object





function Initialize(Plugin)
	PLUGIN = Plugin
	
	Plugin:SetName("HookNotify");
	Plugin:SetVersion(1);
	
	PluginManager = cPluginManager:Get();
	cPluginManager.AddHook(cPluginManager.HOOK_BLOCK_TO_PICKUPS,      OnBlockToPickups);
	cPluginManager.AddHook(cPluginManager.HOOK_CHAT,                  OnChat);
	cPluginManager.AddHook(cPluginManager.HOOK_CHUNK_AVAILABLE,       OnChunkAvailable);
	cPluginManager.AddHook(cPluginManager.HOOK_CHUNK_GENERATED,       OnChunkGenerated);
	cPluginManager.AddHook(cPluginManager.HOOK_CHUNK_GENERATING,      OnChunkGenerating);
	cPluginManager.AddHook(cPluginManager.HOOK_CHUNK_UNLOADED,        OnChunkUnloaded);
	cPluginManager.AddHook(cPluginManager.HOOK_CHUNK_UNLOADING,       OnChunkUnloading);
	cPluginManager.AddHook(cPluginManager.HOOK_COLLECTING_PICKUP,     OnCollectingPickup);
	cPluginManager.AddHook(cPluginManager.HOOK_CRAFTING_NO_RECIPE,    OnCraftingNoRecipe);
	cPluginManager.AddHook(cPluginManager.HOOK_DISCONNECT,            OnDisconnect);
	cPluginManager.AddHook(cPluginManager.HOOK_ENTITY_TELEPORT,       OnEntityTeleport);
	cPluginManager.AddHook(cPluginManager.HOOK_EXECUTE_COMMAND,       OnExecuteCommand);
	cPluginManager.AddHook(cPluginManager.HOOK_HANDSHAKE,             OnHandshake);
	cPluginManager.AddHook(cPluginManager.HOOK_KILLING,               OnKilling);
	cPluginManager.AddHook(cPluginManager.HOOK_LOGIN,                 OnLogin);
	cPluginManager.AddHook(cPluginManager.HOOK_PLAYER_BREAKING_BLOCK, OnPlayerBreakingBlock);
	cPluginManager.AddHook(cPluginManager.HOOK_PLAYER_BROKEN_BLOCK,   OnPlayerBrokenBlock);
	cPluginManager.AddHook(cPluginManager.HOOK_PLAYER_EATING,         OnPlayerEating);
	cPluginManager.AddHook(cPluginManager.HOOK_PLAYER_JOINED,         OnPlayerJoined);
	cPluginManager.AddHook(cPluginManager.HOOK_PLAYER_LEFT_CLICK,     OnPlayerLeftClick);
	cPluginManager.AddHook(cPluginManager.HOOK_PLAYER_MOVING,         OnPlayerMoving);
	cPluginManager.AddHook(cPluginManager.HOOK_PLAYER_PLACED_BLOCK,   OnPlayerPlacedBlock);
	cPluginManager.AddHook(cPluginManager.HOOK_PLAYER_PLACING_BLOCK,  OnPlayerPlacingBlock);
	cPluginManager.AddHook(cPluginManager.HOOK_PLAYER_RIGHT_CLICK,    OnPlayerRightClick);
	cPluginManager.AddHook(cPluginManager.HOOK_PLAYER_SHOOTING,       OnPlayerShooting);
	cPluginManager.AddHook(cPluginManager.HOOK_PLAYER_SPAWNED,        OnPlayerSpawned);
	cPluginManager.AddHook(cPluginManager.HOOK_PLAYER_TOSSING_ITEM,   OnPlayerTossingItem);
	cPluginManager.AddHook(cPluginManager.HOOK_PLAYER_USED_BLOCK,     OnPlayerUsedBlock);
	cPluginManager.AddHook(cPluginManager.HOOK_PLAYER_USED_ITEM,      OnPlayerUsedItem);
	cPluginManager.AddHook(cPluginManager.HOOK_PLAYER_USING_BLOCK,    OnPlayerUsingBlock);
	cPluginManager.AddHook(cPluginManager.HOOK_PLAYER_USING_ITEM,     OnPlayerUsingItem);
	cPluginManager.AddHook(cPluginManager.HOOK_POST_CRAFTING,         OnPostCrafting);
	cPluginManager.AddHook(cPluginManager.HOOK_PRE_CRAFTING,          OnPreCrafting);
	cPluginManager.AddHook(cPluginManager.HOOK_SPAWNED_ENTITY,        OnSpawnedEntity);
	cPluginManager.AddHook(cPluginManager.HOOK_SPAWNED_MONSTER,       OnSpawnedMonster);
	cPluginManager.AddHook(cPluginManager.HOOK_SPAWNING_ENTITY,       OnSpawningEntity);
	cPluginManager.AddHook(cPluginManager.HOOK_SPAWNING_MONSTER,      OnSpawningMonster);
	cPluginManager.AddHook(cPluginManager.HOOK_TAKE_DAMAGE,           OnTakeDamage);
	cPluginManager.AddHook(cPluginManager.HOOK_UPDATED_SIGN,          OnUpdatedSign);
	cPluginManager.AddHook(cPluginManager.HOOK_UPDATING_SIGN,         OnUpdatingSign);
	cPluginManager.AddHook(cPluginManager.HOOK_WEATHER_CHANGED,       OnWeatherChanged);
	cPluginManager.AddHook(cPluginManager.HOOK_WEATHER_CHANGING,      OnWeatherChanging);
	
	LOGINFO("HookNotify plugin is installed, beware, the log output may be quite large!");
	LOGINFO("You want this plugin enabled only when developing another plugin, not for regular gameplay.");
	
	return true
end





function LogHook(FnName, ...)
	LOG(FnName .. "(");
	for i, v in ipairs(arg) do
		local vt = tostring(v);
		local TypeString = type(v);
		if (type(v) == "userdata") then
			TypeString = tolua.type(v);
		end;
		LOG("  " .. tostring(i) .. ": " .. TypeString .. ": " .. tostring(v));
	end
	LOG(")");
end





function OnBlockToPickups(...)
	LogHook("OnBlockToPickups", unpack(arg));
	local World, Digger, BlockX, BlockY, BlockZ, BlockType, BlockMeta, Pickups = unpack(arg);
	if (Pickups ~= nil) then
		local Name = "NULL";
		if (Digger ~= nil) then
			Name = Digger:GetName()
		end
		LOG("Got cItems from " .. Name .. ", trying to manipulate them.");
		Pickups:Add(cItem:new(E_ITEM_DIAMOND_SHOVEL, 1));
		LOG("Current size: " .. Pickups:Size());
	end;
end;





function OnChat(...)
	LogHook("OnChat", unpack(arg));
end





function OnChunkAvailable(...)
	LogHook("OnChunkAvailable", unpack(arg));
end





function OnChunkGenerated(...)
	LogHook("OnChunkGenerated", unpack(arg));
end





function OnChunkGenerating(...)
	LogHook("OnChunkGenerating", unpack(arg));
end





function OnChunkUnloaded(...)
	LogHook("OnChunkUnloaded", unpack(arg));
end





function OnChunkUnloading(...)
	LogHook("OnChunkUnloading", unpack(arg));
end





function OnPlayerUsingItem(...)
	LogHook("OnPlayerUsingItem", unpack(arg));
end





function OnCollectingPickup(...)
	LogHook("OnCollectingPickup", unpack(arg));
end




function OnCraftingNoRecipe(...)
	LogHook("OnCraftingNoRecipe", unpack(arg));
end





function OnDisconnect(...)
	LogHook("OnDisconnect", unpack(arg));
end





function OnEntityTeleport(arg1,arg2,arg3)
	if arg1.IsPlayer() then
		-- if it's a player, get his name
		LOG("OnEntityTeleport: Player: " .. arg1.GetName());
	else
		-- if it's a entity, get its type
		LOG("OnEntityTeleport: EntityType: " .. arg1.GetEntityType());
	end
	LOG("OldPos: " .. arg2.x .. " / " .. arg2.y .. " / " .. arg2.z);
	LOG("NewPos: " .. arg3.x .. " / " .. arg3.y .. " / " .. arg3.z);
end





function OnExecuteCommand(...)
	LogHook("OnExecuteCommand", unpack(arg));
	
	-- For some reason logging doesn't work for this callback, so list some stuff manually to verify:
	LOG("arg1 type: " .. type(arg[1]));
	if (arg[1] ~= nil) then
		LOG("Player name: " .. arg[1]:GetName());
	end
	LOG("Command: " .. arg[2][1]);
end





function OnHandshake(...)
	LogHook("OnHandshake", unpack(arg));
end





function OnKilling(...)
	LogHook("OnKilling", unpack(arg));
end





function OnLogin(...)
	LogHook("OnLogin", unpack(arg));
end





function OnPlayerBreakingBlock(...)
	LogHook("OnPlayerBreakingBlock", unpack(arg));
end





function OnPlayerBrokenBlock(...)
	LogHook("OnPlayerBrokenBlock", unpack(arg));
end





function OnPlayerEating(...)
	LogHook("OnPlayerEating", unpack(arg));
end





function OnPlayerJoined(...)
	LogHook("OnPlayerJoined", unpack(arg));
end





function OnPlayerLeftClick(...)
	LogHook("OnPlayerLeftClick", unpack(arg));
end





function OnPlayerMoving(...)
	LogHook("OnPlayerMoving", unpack(arg));
end





function OnPlayerPlacedBlock(...)
	LogHook("OnPlayerPlacedBlock", unpack(arg));
end





function OnPlayerPlacingBlock(...)
	LogHook("OnPlayerPlacingBlock", unpack(arg));
end





function OnPlayerRightClick(...)
	LogHook("OnPlayerRightClick", unpack(arg));
end





function OnPlayerShooting(...)
	LogHook("OnPlayerShooting", unpack(arg));
end





function OnPlayerSpawned(...)
	LogHook("OnPlayerSpawned", unpack(arg));
end





function OnPlayerTossingItem(...)
	LogHook("OnPlayerTossingItem", unpack(arg));
end





function OnPlayerUsedBlock(...)
	LogHook("OnPlayerUsedBlock", unpack(arg));
end





function OnPlayerUsedItem(...)
	LogHook("OnPlayerUsedItem", unpack(arg));
end





function OnPlayerUsingBlock(...)
	LogHook("OnPlayerUsingBlock", unpack(arg));
end





function OnPlayerUsingItem(...)
	LogHook("OnPlayerUsingItem", unpack(arg));
end





function OnPostCrafting(...)
	LogHook("OnPostCrafting", unpack(arg));
end





function OnPreCrafting(...)
	LogHook("OnPreCrafting", unpack(arg));
end





function OnSpawnedEntity(...)
	LogHook("OnSpawnedEntity", unpack(arg));
end





function OnSpawnedMonster(...)
	LogHook("OnSpawnedMonster", unpack(arg));
end





function OnSpawningEntity(...)
	LogHook("OnSpawningEntity", unpack(arg));
end





function OnSpawningMonster(...)
	LogHook("OnSpawningMonster", unpack(arg));
end





function OnUpdatedSign(...)
	LogHook("OnUpdatedSign", unpack(arg));
end





function OnUpdatingSign(...)
	LogHook("OnUpdatingSign", unpack(arg));
end





function OnWeatherChanged(...)
	LogHook("OnWeatherChanged", unpack(arg));
end





function OnWeatherChanging(...)
	LogHook("OnWeatherChanging", unpack(arg));
end





------------------------------------------------------------------
-- Special handling for OnTakeDamage to print the contents of TDI:

function OnTakeDamage(Receiver, TDI)
	-- Receiver is cPawn
	-- TDI is TakeDamageInfo

	LOG("OnTakeDamage(): " .. Receiver:GetClass() .. " was dealt RawDamage " .. TDI.RawDamage .. ", FinalDamage " .. TDI.FinalDamage .. " (that is, " .. (TDI.RawDamage - TDI.FinalDamage) .. " HPs covered by armor)");
end



