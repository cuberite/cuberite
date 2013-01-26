
-- Global variables
PLUGIN = {}	-- Reference to own plugin object





function Initialize(Plugin)
	PLUGIN = Plugin
	
	Plugin:SetName("HookNotify")
	Plugin:SetVersion(1)
	
	PluginManager = cRoot:Get():GetPluginManager()
	PluginManager:AddHook(Plugin, cPluginManager.HOOK_CHAT);
	PluginManager:AddHook(Plugin, cPluginManager.HOOK_CHUNK_GENERATED);
	PluginManager:AddHook(Plugin, cPluginManager.HOOK_CHUNK_GENERATING);
	PluginManager:AddHook(Plugin, cPluginManager.HOOK_COLLECT_PICKUP);
	PluginManager:AddHook(Plugin, cPluginManager.HOOK_CRAFTING_NO_RECIPE);
	PluginManager:AddHook(Plugin, cPluginManager.HOOK_DISCONNECT);
	PluginManager:AddHook(Plugin, cPluginManager.HOOK_HANDSHAKE);
	PluginManager:AddHook(Plugin, cPluginManager.HOOK_KILLED);
	PluginManager:AddHook(Plugin, cPluginManager.HOOK_LOGIN);
	PluginManager:AddHook(Plugin, cPluginManager.HOOK_PLAYER_BREAKING_BLOCK);
	PluginManager:AddHook(Plugin, cPluginManager.HOOK_PLAYER_BROKEN_BLOCK);
	PluginManager:AddHook(Plugin, cPluginManager.HOOK_PLAYER_EATING);
	PluginManager:AddHook(Plugin, cPluginManager.HOOK_PLAYER_JOINED);
	PluginManager:AddHook(Plugin, cPluginManager.HOOK_PLAYER_LEFT_CLICK);
	PluginManager:AddHook(Plugin, cPluginManager.HOOK_PLAYER_MOVING);
	PluginManager:AddHook(Plugin, cPluginManager.HOOK_PLAYER_PLACED_BLOCK);
	PluginManager:AddHook(Plugin, cPluginManager.HOOK_PLAYER_PLACING_BLOCK);
	PluginManager:AddHook(Plugin, cPluginManager.HOOK_PLAYER_RIGHT_CLICK);
	PluginManager:AddHook(Plugin, cPluginManager.HOOK_PLAYER_SHOOTING);
	PluginManager:AddHook(Plugin, cPluginManager.HOOK_PLAYER_SPAWNED);
	PluginManager:AddHook(Plugin, cPluginManager.HOOK_PLAYER_TOSSING_ITEM);
	PluginManager:AddHook(Plugin, cPluginManager.HOOK_PLAYER_USED_BLOCK);
	PluginManager:AddHook(Plugin, cPluginManager.HOOK_PLAYER_USED_ITEM);
	PluginManager:AddHook(Plugin, cPluginManager.HOOK_PLAYER_USING_BLOCK);
	PluginManager:AddHook(Plugin, cPluginManager.HOOK_PLAYER_USING_ITEM);
	PluginManager:AddHook(Plugin, cPluginManager.HOOK_POST_CRAFTING);
	PluginManager:AddHook(Plugin, cPluginManager.HOOK_PRE_CRAFTING);
	PluginManager:AddHook(Plugin, cPluginManager.HOOK_TAKE_DAMAGE);
	PluginManager:AddHook(Plugin, cPluginManager.HOOK_UPDATED_SIGN);
	PluginManager:AddHook(Plugin, cPluginManager.HOOK_UPDATING_SIGN);
	PluginManager:AddHook(Plugin, cPluginManager.HOOK_WEATHER_CHANGED);
	
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





function OnChat(...)
	LogHook("OnChat", unpack(arg));
end





function OnChunkGenerated(...)
	LogHook("OnChunkGenerated", unpack(arg));
end





function OnChunkGenerating(...)
	LogHook("OnChunkGenerating", unpack(arg));
end





function OnPlayerUsingItem(...)
	LogHook("OnPlayerUsingItem", unpack(arg));
end





function OnCollectPickup(...)
	LogHook("OnCollectPickup", unpack(arg));
end




function OnCraftingNoRecipe(...)
	LogHook("OnCraftingNoRecipe", unpack(arg));
end





function OnDisconnect(...)
	LogHook("OnDisconnect", unpack(arg));
end





function OnHandshake(...)
	LogHook("OnHandshake", unpack(arg));
end





function OnKilled(...)
	LogHook("OnKilled", unpack(arg));
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





function OnUpdatedSign(...)
	LogHook("OnUpdatedSign", unpack(arg));
end





function OnUpdatingSign(...)
	LogHook("OnUpdatingSign", unpack(arg));
end





function OnWeatherChanged(...)
	LogHook("OnWeatherChanged", unpack(arg));
end





------------------------------------------------------------------
-- Special handling for OnTakeDamage to print the contents of TDI:

function OnTakeDamage(Receiver, TDI)
	-- Receiver is cPawn
	-- TDI is TakeDamageInfo

	LOG("OnTakeDamage(): " .. Receiver:GetClass() .. " was dealt RawDamage " .. TDI.RawDamage .. ", FinalDamage " .. TDI.FinalDamage .. " (that is, " .. (TDI.RawDamage - TDI.FinalDamage) .. " HPs covered by armor)");
end



