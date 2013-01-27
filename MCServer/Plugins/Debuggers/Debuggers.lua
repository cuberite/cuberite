
-- Global variables
PLUGIN = {}	-- Reference to own plugin object
ShouldDumpFunctions = false  -- If set to true, all available functions are logged upon plugin initialization





function Initialize(Plugin)
	PLUGIN = Plugin
	
	Plugin:SetName("Debuggers")
	Plugin:SetVersion(1)
	
	PluginManager = cRoot:Get():GetPluginManager()
	PluginManager:AddHook(Plugin, cPluginManager.HOOK_PLAYER_USING_ITEM)
	PluginManager:AddHook(Plugin, cPluginManager.HOOK_TAKE_DAMAGE)
	
	LOG("Initialized " .. Plugin:GetName() .. " v." .. Plugin:GetVersion())

	-- dump all available functions to console:
	if (ShouldDumpFunctions) then
		LOG("Dumping all available functions:");
		function dump (prefix, a) 
			for i, v in pairs (a) do
				if (type(v) == "table") then
					if (GetChar(i, 1) ~= ".") then
						if (v == _G) then
							LOG(prefix .. i .. " == _G, CYCLE, ignoring");
						elseif (v == _G.package) then
							LOG(prefix .. i .. " == _G.package, ignoring");
						else
							dump(prefix .. i .. ".", v)
						end
					end
				elseif (type(v) == "function") then
					LOG(prefix .. i .. "()")
				end
			end
		end
		dump("", _G);
	end

	return true
end





function OnPlayerUsingItem(Player, BlockX, BlockY, BlockZ, BlockFace, CursorX, CursorY, CursorZ)

	-- dont check if the direction is in the air
	if (BlockFace == BLOCK_FACE_NONE) then
		return false
	end

	local HeldItem = Player:GetEquippedItem();

	if (HeldItem.m_ItemType == E_ITEM_STICK) then
		-- Magic sTick of ticking: set the pointed block for ticking at the next tick
		Player:SendMessage(cChatColor.LightGray .. "Setting next block tick to {" .. BlockX .. ", " .. BlockY .. ", " .. BlockZ .. "}")
		Player:GetWorld():SetNextBlockTick(BlockX, BlockY, BlockZ);
		return true
	end

	if (HeldItem.m_ItemType == E_ITEM_BLAZE_ROD) then
		-- Magic rod of query: show block types and metas for both neighbors of the pointed face
		local Type = 0;
		local Meta = 0;
		local Valid = false;
		Valid, Type, Meta = Player:GetWorld():GetBlockTypeMeta(BlockX, BlockY, BlockZ, Type, Meta);

		if (Type == E_BLOCK_AIR) then
			Player:SendMessage(cChatColor.LightGray .. "Block {" .. BlockX .. ", " .. BlockY .. ", " .. BlockZ .. "}: air:" .. Meta);
		else
			local TempItem = cItem(Type, 1, Meta);
			Player:SendMessage(cChatColor.LightGray .. "Block {" .. BlockX .. ", " .. BlockY .. ", " .. BlockZ .. "}: " .. ItemToFullString(TempItem) .. " (" .. Type .. ":" .. Meta .. ")");
		end
		
		local X = BlockX;
		local Y = BlockY;
		local Z = BlockZ;
		X, Y, Z = AddFaceDirection(BlockX, BlockY, BlockZ, BlockFace);
		Valid, Type, Meta = Player:GetWorld():GetBlockTypeMeta(X, Y, Z, Type, Meta);
		if (Type == E_BLOCK_AIR) then
			Player:SendMessage(cChatColor.LightGray .. "Block {" .. X .. ", " .. Y .. ", " .. Z .. "}: air:" .. Meta);
		else
			local TempItem = cItem(Type, 1, Meta);
			Player:SendMessage(cChatColor.LightGray .. "Block {" .. X .. ", " .. Y .. ", " .. Z .. "}: " .. ItemToFullString(TempItem) .. " (" .. Type .. ":" .. Meta .. ")");
			return true;
		end
	end
end





function OnTakeDamage(Receiver, TDI)
	-- Receiver is cPawn
	-- TDI is TakeDamageInfo

	LOG(Receiver:GetClass() .. " was dealt RawDamage " .. TDI.RawDamage .. ", FinalDamage " .. TDI.FinalDamage .. " (that is, " .. (TDI.RawDamage - TDI.FinalDamage) .. " HPs covered by armor)");
end



