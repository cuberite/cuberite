function HandleItemCommand( Split, Player )
	if( #Split ~= 2 and #Split ~=3 ) then
		Player:SendMessage( cChatColor.Green .. "Usage: /item [ItemID/Name:Dmg] <Amount>" )
		return true
	end

	local Item = cItem(E_ITEM_EMPTY, 1)
	local FoundItem = StringToItem( Split[2], Item )
	
	if( IsValidItem( Item.m_ItemType ) == false ) then -- StringToItem does not check if item is valid
		FoundItem = false
	end

	if( FoundItem == false ) then
		Player:SendMessage( cChatColor.Green .. "Invalid Item ID / Name !" )
		return true
	end

	if( #Split == 3 ) then
		ItemAmount = tonumber( Split[3] )
		if( ItemAmount == nil or ItemAmount < 1 or ItemAmount > 512 ) then
			Player:SendMessage( cChatColor.Green .. "Invalid Amount !" )
			return true
		else
			Item.m_ItemCount = ItemAmount
		end
	end

	if( Player:GetInventory():AddItem( Item ) == true ) then
		Player:SendMessage( cChatColor.Green .. "There you go !" )
		LOG("Gave " .. Player:GetName() .. " " .. Item.m_ItemCount .. " times " .. Item.m_ItemType .. ":" .. Item.m_ItemDamage)
	else
		Player:SendMessage( cChatColor.Green .. "Not enough space in inventory !" )
	end
	return true
end