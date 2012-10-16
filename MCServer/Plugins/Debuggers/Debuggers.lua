
-- Global variables
PLUGIN = {}	-- Reference to own plugin object





function Initialize(Plugin)
	PLUGIN = Plugin
	
	Plugin:SetName("Debuggers")
	Plugin:SetVersion(1)
	
	PluginManager = cRoot:Get():GetPluginManager()
	PluginManager:AddHook(Plugin, cPluginManager.E_PLUGIN_BLOCK_PLACE)
	
	LOG("Initialized " .. Plugin:GetName() .. " v." .. Plugin:GetVersion())
	return true
end





function OnBlockPlace(Player, BlockX, BlockY, BlockZ, BlockFace, HeldItem)

	-- dont check if the direction is in the air
	if BlockFace == BLOCK_FACE_NONE then
		return false
	end

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
		Type, Meta = Player:GetWorld():GetBlockTypeMeta(BlockX, BlockY, BlockZ, Type, Meta);
		if (Type == E_BLOCK_AIR) then
			Player:SendMessage(cChatColor.LightGray .. "Block {" .. BlockX .. ", " .. BlockY .. ", " .. BlockZ .. "}: air:" .. Meta);
		else
			local TempItem = cItem(Type, 1, Meta);
			Player:SendMessage(cChatColor.LightGray .. "Block {" .. BlockX .. ", " .. BlockY .. ", " .. BlockZ .. "}: " .. ItemToFullString(TempItem) .. " (" .. Type .. ":" .. Meta .. ")");
		end
		
		local X = BlockX;
		local Y = BlockY;
		local Z = BlockZ;
		X, Y, Z = AddDirection(BlockX, BlockY, BlockZ, BlockFace);
		Type, Meta = Player:GetWorld():GetBlockTypeMeta(X, Y, Z, Type, Meta);
		if (Type == E_BLOCK_AIR) then
			Player:SendMessage(cChatColor.LightGray .. "Block {" .. X .. ", " .. Y .. ", " .. Z .. "}: air:" .. Meta);
		else
			local TempItem = cItem(Type, 1, Meta);
			Player:SendMessage(cChatColor.LightGray .. "Block {" .. X .. ", " .. Y .. ", " .. Z .. "}: " .. ItemToFullString(TempItem) .. " (" .. Type .. ":" .. Meta .. ")");
			return true;
		end
	end
end




