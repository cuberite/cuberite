function HandleItemCommand( Split, Player )
	if( #Split ~= 2 and #Split ~=3 ) then
		Player:SendMessage( cChatColor.Green .. "Usage: /item [ItemID/Name:Dmg] <Amount>" )
		return true
	end

	local FoundItem = false
	
	local ItemSyntax = Split[2]	-- Contains item string with optional metadata
	local ItemData = StringSplit( Split[2], ":" )

	-- Default item values
	local ItemID = 0
	local ItemMeta = 0
	local ItemAmount = 1
	
	if( #ItemData > 0 ) then
		ItemID = ItemData[1]
	end
	
	if( tonumber(ItemID) ~= nil ) then -- Definitely a number
		ItemID = tonumber(ItemID)
		if( IsValidItem( ItemID ) ) then
			FoundItem = true
		end
	end

	if( FoundItem == false ) then
		if ( HAVE_ITEM_NAMES == true ) then
			local Item = ItemsTable[ ItemID ]
			if( Item ~= nil ) then
				ItemID = Item.m_ItemID
				ItemMeta = Item.m_ItemHealth
				FoundItem = true
			end
		end
	end
	
	-- Override metadata from item in list, if metadata was given
	if( #ItemData > 1 and tonumber( ItemData[2] ) ~= nil ) then	-- Metadata is given, and is a number
		ItemMeta = tonumber( ItemData[2] )
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
		end
	end

	local NewItem = cItem( ItemID, ItemAmount, ItemMeta )
	if( Player:GetInventory():AddItem( NewItem ) == true ) then
		Player:SendMessage( cChatColor.Green .. "There you go !" )
		LOG("Gave " .. Player:GetName() .. " " .. ItemAmount .. " times " .. ItemID .. ":" .. ItemMeta)
	else
		Player:SendMessage( cChatColor.Green .. "Not enough space in inventory !" )
	end
	return true
end