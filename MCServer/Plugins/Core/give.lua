function HandleGiveCommand(Split, Player)
	if ((#Split ~= 2) and (#Split ~=3)) then
		Player:SendMessage(cChatColor.Green .. "Usage: /give [ItemType/Name:Dmg] <Amount>");
		return true;
	end

	local Item = cItem();
	local FoundItem = StringToItem(Split[2], Item);
	
	if not(IsValidItem(Item.m_ItemType)) then  -- StringToItem does not check if item is valid
		FoundItem = false
	end

	if not(FoundItem) then
		Player:SendMessage( cChatColor.Green .. "Invalid item id or name!" )
		return true
	end

	local ItemAmount = 1;
	if (#Split == 3) then
		ItemAmount = tonumber(Split[3]);
		if ((ItemAmount == nil) or (ItemAmount < 1) or (ItemAmount > 512)) then
			Player:SendMessage(cChatColor.Green .. "Invalid amount!");
			return true;
		end
	end

	Item.m_ItemCount = ItemAmount;

	local ItemsGiven = Player:GetInventory():AddItem(Item);
	if (ItemsGiven == ItemAmount) then
		Player:SendMessage( cChatColor.Green .. "There you go!");
		LOG("Gave " .. Player:GetName() .. " " .. Item.m_ItemCount .. " times " .. Item.m_ItemType .. ":" .. Item.m_ItemDamage);
	else
		Player:SendMessage(cChatColor.Green .. "Not enough space in inventory, only gave " .. ItemsGiven);
		LOG("Player " .. Player:GetName() .. " asked for " .. Item.m_ItemCount .. " times " .. Item.m_ItemType .. ":" .. Item.m_ItemDamage ..", but only could fit " .. ItemsGiven);
	end
	return true;
end