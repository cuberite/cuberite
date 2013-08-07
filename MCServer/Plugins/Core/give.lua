function HandleGiveCommand(Split, Player)

	-- Make sure there are a correct number of arguments.
	if #Split ~= 3 and #Split ~= 4 and #Split ~= 5 then
		Player:SendMessage( cChatColor.Yellow .. "[INFO] " .. cChatColor.White .. "Usage: /give <player> <item> [amount] [meta]" )
		return true
	end

	-- Get the item from the arguments and check it's valid.
	local Item = cItem()
	if #Split == 5 then
		local FoundItem = StringToItem(Split[3] .. ":" .. Split[5], Item)
	else
		local FoundItem = StringToItem(Split[3], Item)
	end	
	if not IsValidItem(Item.m_ItemType) then  -- StringToItem does not check if item is valid
		FoundItem = false
	end

	if not FoundItem  then
		Player:SendMessage(cChatColor.Rose .. "[INFO] " .. cChatColor.White .. "Invalid item id or name!" )
		return true
	end

	-- Work out how many items the user wants.
	local ItemAmount = 1
	if #Split > 3 then
		ItemAmount = tonumber(Split[4])
		if ItemAmount == nil or ItemAmount < 1 or ItemAmount > 512 then
			Player:SendMessage(cChatColor.Rose .. "[INFO] " .. cChatColor.White .. "Invalid amount!" )
			return true
		end
	end

	Item.m_ItemCount = ItemAmount

	-- Get the playername from the split.
	local playerName = Split[2]

	local function giveItems(newPlayer)
		local ItemsGiven = newPlayer:GetInventory():AddItem(Item)
		if ItemsGiven == ItemAmount then
			newPlayer:SendMessage(cChatColor.Green .. "[INFO] " .. cChatColor.White .. "You were given " .. Item.m_ItemCount .. " of " .. Item.m_ItemType .. "." )
			if not newPlayer == Player then
				Player:SendMessage(cChatColor.Green .. "[INFO] " .. cChatColor.White .. "Items given!" )
			end
			LOG("Gave " .. newPlayer:GetName() .. " " .. Item.m_ItemCount .. " times " .. Item.m_ItemType .. ":" .. Item.m_ItemDamage)
		else
			Player:SendMessage( cChatColor.Rose .. "[INFO] " .. cChatColor.White .. "Not enough space in inventory, only gave " .. ItemsGiven)
			LOG( "Player " .. Player:GetName() .. " asked for " .. Item.m_ItemCount .. " times " .. Item.m_ItemType .. ":" .. Item.m_ItemDamage ..", but only could fit " .. ItemsGiven )
		end
		return true
	end

	-- Finally give the items to the player.
	itemStatus = cRoot:Get():FindAndDoWithPlayer(playerName, giveItems)

	-- Check to make sure that giving items was successful.
	if not itemStatus then
		Player:SendMessage( cChatColor.Rose .. "[INFO] " .. cChatColor.White .. "There was no player that matched your query.")
	end

	return true

end
