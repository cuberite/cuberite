--[[
General stuff
]]
-- Returns Handy plugin version number
function GetHandyVersion()
	return HANDY_VERSION
end
-- Checks if handy is in proper version
function CheckForRequiedVersion(IN_version)
	if (IN_version > HANDY_VERSION) then		return false	end
	return true
end
--[[
MCS-specific _functions and nasty hacks :D
]]
-- There's a "GetChestHeight" function inside source code, but it's not lua-exported
function GetChestHeightCheat(IN_chest)
	if (IN_chest:GetSlot(28) == nil) then		-- this means we're trying to get double chest slot and FAIL
		LOGWARN("HANDY: single chest checked")
		return 3
	end
	LOGWARN("HANDY: double chest checked")
	return 6
end
-- Those two checks how many items of given IN_itemID chest and player have, and how much they could fit inside them
function ReadChestForItem(IN_chest, IN_itemID)
	local _items_found = 0
	local _free_space = 0
	-- stalk through chest slots...
	local _slot_counter = 0
	local _slot_item
	local _item_max_stack = GetItemMaxStack(IN_itemID)
	while true do
		_slot_item = IN_chest:GetSlot(_slot_counter)
		if (_slot_item ~= nil) then
			if (_slot_item.m_ItemID == IN_itemID) then
				_items_found = _items_found + _slot_item.m_ItemCount
				_free_space = _free_space + (_item_max_stack - _slot_item.m_ItemCount)
			end
			if (_slot_item:IsEmpty() == true) then
				_free_space = _free_space + _item_max_stack
			end
		end
		_slot_counter = _slot_counter + 1
		if (_slot_counter == CHEST_WIDTH*GetChestHeightCheat(IN_chest)) then
			break
		end
	end
	return _items_found, _free_space
end
function ReadPlayerForItem(IN_player, IN_itemID)
	local _items_found = 0
	local _free_space = 0
	-- stalk through IN_player inventory slots...
	local _slot_counter = 9
	if (ItemIsArmor(IN_itemID) == true) then	_slot_counter = 5	end
	local _slot_item
	local _item_max_stack = GetItemMaxStack(IN_itemID)
	while true do
		_slot_item = IN_player:GetInventory():GetSlot(_slot_counter)
		if (_slot_item ~= nil) then
			if (_slot_item.m_ItemID == IN_itemID) then
				_items_found = _items_found + _slot_item.m_ItemCount
				_free_space = _free_space + (_item_max_stack - _slot_item.m_ItemCount)
			end
			if (_slot_item:IsEmpty() == true) then
				_free_space = _free_space + _item_max_stack
			end
		end
		_slot_counter = _slot_counter + 1
		if (_slot_counter == 45) then
			break
		end
	end
	return _items_found, _free_space
end
-- Following functions are for chest-related operations (since noone was bothered writing them in MCS code)
-- BEWARE! Those assume you did checked if chest has items/space in it!
function TakeItemsFromChest(IN_chest, IN_itemID, IN_ammount)	-- UNSAFE! CHECK FOR ITEMS FIRST!!
	-- stalk through chest slots...
	local _slot_counter = 0
	local _slot_item
	local _take_count = IN_ammount
	while true do
		_slot_item = IN_chest:GetSlot(_slot_counter)
		if (_slot_item ~= nil) then
			if (_slot_item.m_ItemID == IN_itemID) then
				-- assuming player have enought money
				if (_take_count > 0) then
					if (_take_count > _slot_item.m_ItemCount) then
						_take_count = _take_count - _slot_item.m_ItemCount
						IN_chest:SetSlot(_slot_counter, cItem())		-- a bit hacky, can't make cItem:Clear() work(
					else
						local _left_count = _slot_item.m_ItemCount - _take_count
						IN_chest:SetSlot(_slot_counter, cItem(_slot_item.m_ItemID, _left_count))		-- a bit hacky
						_take_count = 0
					end
				end
				if (_take_count == 0) then
					break
				end
			end
		end
		_slot_counter = _slot_counter + 1
		if (_slot_counter == CHEST_WIDTH*GetChestHeightCheat(IN_chest)) then
			break
		end
	end
end
function PutItemsToChest(IN_chest, IN_itemID, IN_ammount)	-- UNSAFE! CHECK FOR SPACE FIRST!!
	-- stalk through chest slots...
	local _slot_counter = 0
	local _slot_item
	local _put_count = IN_ammount
	local _max_stack = GetItemMaxStack(IN_itemID)
	while true do
		_slot_item = IN_chest:GetSlot(_slot_counter)
		local _portion = 0
		local _ammount_to_set = 0
		if (_slot_item ~= nil) then
			if (_slot_item:IsEmpty() == true) then
				_portion = math.min(_max_stack, _put_count)
				_ammount_to_set = _portion
			else
				if (_slot_item.m_ItemID == IN_itemID) then
					-- choose between how much we need to put and how much free space left
					_portion = math.min(_put_count, _max_stack - _slot_item.m_ItemCount)
					_ammount_to_set = _slot_item.m_ItemCount + _portion
				end
			end
		end
		IN_chest:SetSlot(_slot_counter, cItem(IN_itemID, _ammount_to_set))	-- we add max stack to chest
		_put_count = _put_count - _portion
		if (_put_count == 0) then		break		end
		_slot_counter = _slot_counter + 1
		if (_slot_counter == CHEST_WIDTH*GetChestHeightCheat(IN_chest)) then
			break
		end
	end
end
-- Similar to chest-related.
function TakeItemsFromPlayer(IN_player, IN_itemID, IN_ammount)	-- UNSAFE! CHECK FIRST!
	local _put_count = IN_ammount
	local _max_stack = GetItemMaxStack(IN_itemID)
	while true do
		local _portion = math.min(_max_stack, _put_count)
		IN_player:GetInventory():RemoveItem(cItem(IN_itemID, _portion))
		_put_count = _put_count - _portion
		if (_put_count == 0) then		break		end
	end
end
function GiveItemsToPlayer(IN_player, IN_itemID, IN_ammount)	-- UNSAFE! CHECK FIRST!
	local _put_count = IN_ammount
	local _max_stack = GetItemMaxStack(IN_itemID)
	while true do
		local _portion = math.min(_max_stack, _put_count)
		IN_player:GetInventory():AddItem(cItem(IN_itemID, _portion))
		_put_count = _put_count - _portion
		if (_put_count == 0) then		break		end
	end
end
-- This function returns item max stack for a given itemID. It uses vanilla max stack size, and uses several non-common items notations;
-- Those are:
-- oneonerecord (because aparently 11record wasn't the best idea in lua scripting application)
-- carrotonastick (because it wasn't added to items.txt yet)
-- waitrecord (for same reason)
-- Feel free to ignore the difference, or to add those to items.txt
function GetItemMaxStack(IN_itemID)
	local _result = 64
	-- Tools and swords
	if (IN_itemID == woodensword)		then	_result = 1	end
	if (IN_itemID == woodenshovel)		then	_result = 1	end
	if (IN_itemID == woodenpickaxe)		then	_result = 1	end
	if (IN_itemID == woodenaxe)			then	_result = 1	end
	if (IN_itemID == woodenhoe)			then	_result = 1	end
	if (IN_itemID == stonesword)		then	_result = 1	end
	if (IN_itemID == stoneshovel)		then	_result = 1	end
	if (IN_itemID == stonepickaxe)		then	_result = 1	end
	if (IN_itemID == stoneaxe)			then	_result = 1	end
	if (IN_itemID == stonehoe)			then	_result = 1	end
	if (IN_itemID == ironsword)			then	_result = 1	end
	if (IN_itemID == ironshovel)		then	_result = 1	end
	if (IN_itemID == ironpickaxe)		then	_result = 1	end
	if (IN_itemID == ironaxe)			then	_result = 1	end
	if (IN_itemID == ironhoe)			then	_result = 1	end
	if (IN_itemID == diamondsword)		then	_result = 1	end
	if (IN_itemID == diamondshovel)		then	_result = 1	end
	if (IN_itemID == diamondpickaxe)	then	_result = 1	end
	if (IN_itemID == diamondaxe)		then	_result = 1	end
	if (IN_itemID == diamondhoe)		then	_result = 1	end
	if (IN_itemID == goldensword)		then	_result = 1	end
	if (IN_itemID == goldenshovel)		then	_result = 1	end
	if (IN_itemID == goldenpickaxe)		then	_result = 1	end
	if (IN_itemID == goldenaxe)			then	_result = 1	end
	if (IN_itemID == goldenhoe)			then	_result = 1	end
	
	if (IN_itemID == flintandsteel)		then	_result = 1	end
	if (IN_itemID == bow)				then	_result = 1	end
	if (IN_itemID == sign)				then	_result = 16	end
	if (IN_itemID == woodendoor)		then	_result = 1	end
	if (IN_itemID == irondoor)			then	_result = 1	end
	if (IN_itemID == cake)				then	_result = 1	end
	if (IN_itemID == cauldron)			then	_result = 1	end
	if (IN_itemID == mushroomstew)		then	_result = 1	end
	if (IN_itemID == painting)			then	_result = 1	end
	if (IN_itemID == bucket)			then	_result = 16	end
	if (IN_itemID == waterbucket)		then	_result = 1	end
	if (IN_itemID == lavabucket)		then	_result = 1	end
	if (IN_itemID == minecart)			then	_result = 1	end
	if (IN_itemID == saddle)			then	_result = 1	end
	if (IN_itemID == snowball)			then	_result = 16	end
	if (IN_itemID == boat)				then	_result = 1	end
	if (IN_itemID == milkbucket)		then	_result = 1	end
	if (IN_itemID == storageminecart)	then	_result = 1	end
	if (IN_itemID == poweredminecart)	then	_result = 1	end
	if (IN_itemID == egg)				then	_result = 16	end
	if (IN_itemID == fishingrod)		then	_result = 1	end
	if (IN_itemID == bed)				then	_result = 1	end
	if (IN_itemID == map)				then	_result = 1	end
	if (IN_itemID == shears)			then	_result = 1	end
	if (IN_itemID == enderpearl)		then	_result = 16	end
	if (IN_itemID == potion)			then	_result = 1	end
	if (IN_itemID == spawnegg)			then	_result = 1	end
	if (IN_itemID == bookandquill)		then	_result = 1	end
	if (IN_itemID == writtenbook)		then	_result = 1	end
	if (IN_itemID == carrotonastick)	then	_result = 1	end
	
	if (IN_itemID == goldrecord)		then	_result = 1	end
	if (IN_itemID == greenrecord)		then	_result = 1	end
	if (IN_itemID == blocksrecord)		then	_result = 1	end
	if (IN_itemID == chirprecord)		then	_result = 1	end
	if (IN_itemID == farrecord)			then	_result = 1	end
	if (IN_itemID == mallrecord)		then	_result = 1	end
	if (IN_itemID == mellohirecord)		then	_result = 1	end
	if (IN_itemID == stalrecord)		then	_result = 1	end
	if (IN_itemID == stradrecord)		then	_result = 1	end
	if (IN_itemID == wardrecord)		then	_result = 1	end
	if (IN_itemID == oneonerecord)		then	_result = 1	end
	if (IN_itemID == waitrecord)		then	_result = 1	end
	
	--if (IN_itemID == xxxxxxxxx)			then	_result = 1	end
	
	if (IN_itemID == leatherhelmet)			then	_result = 1	end
	if (IN_itemID == leatherchestplate)		then	_result = 1	end
	if (IN_itemID == leatherpants)			then	_result = 1	end
	if (IN_itemID == leatherboots)			then	_result = 1	end
	
	if (IN_itemID == chainmailhelmet)		then	_result = 1	end
	if (IN_itemID == chainmailchestplate)	then	_result = 1	end
	if (IN_itemID == chainmailpants)		then	_result = 1	end
	if (IN_itemID == chainmailboots)		then	_result = 1	end
	
	if (IN_itemID == ironhelmet)			then	_result = 1	end
	if (IN_itemID == ironchestplate)		then	_result = 1	end
	if (IN_itemID == ironpants)				then	_result = 1	end
	if (IN_itemID == ironboots)				then	_result = 1	end
	
	if (IN_itemID == diamondhelmet)			then	_result = 1	end
	if (IN_itemID == diamondchestplate)		then	_result = 1	end
	if (IN_itemID == diamondpants)			then	_result = 1	end
	if (IN_itemID == diamondboots)			then	_result = 1	end
	
	if (IN_itemID == goldenhelmet)			then	_result = 1	end
	if (IN_itemID == goldenchestplate)		then	_result = 1	end
	if (IN_itemID == goldenpants)			then	_result = 1	end
	if (IN_itemID == goldenboots)			then	_result = 1	end
	return _result
end
function ItemIsArmor(IN_itemID)
	local _result = false
	if (IN_itemID == leatherhelmet)			then	_result = true	end
	if (IN_itemID == leatherchestplate)		then	_result = true	end
	if (IN_itemID == leatherpants)			then	_result = true	end
	if (IN_itemID == leatherboots)			then	_result = true	end
	
	if (IN_itemID == chainmailhelmet)		then	_result = true	end
	if (IN_itemID == chainmailchestplate)	then	_result = true	end
	if (IN_itemID == chainmailpants)		then	_result = true	end
	if (IN_itemID == chainmailboots)		then	_result = true	end
	
	if (IN_itemID == ironhelmet)			then	_result = true	end
	if (IN_itemID == ironchestplate)		then	_result = true	end
	if (IN_itemID == ironpants)				then	_result = true	end
	if (IN_itemID == ironboots)				then	_result = true	end
	
	if (IN_itemID == diamondhelmet)			then	_result = true	end
	if (IN_itemID == diamondchestplate)		then	_result = true	end
	if (IN_itemID == diamondpants)			then	_result = true	end
	if (IN_itemID == diamondboots)			then	_result = true	end
	
	if (IN_itemID == goldenhelmet)			then	_result = true	end
	if (IN_itemID == goldenchestplate)		then	_result = true	end
	if (IN_itemID == goldenpants)			then	_result = true	end
	if (IN_itemID == goldenboots)			then	_result = true	end
	return _result
end
-- Returns full-length playername for a short name (usefull for parsing commands)
function GetExactPlayername(IN_playername)
	local _result = IN_playername
	local function SetProcessingPlayername(IN_player)
		_result = IN_player:GetName()
	end
	cRoot:Get():FindAndDoWithPlayer(IN_playername, SetProcessingPlayername)
	return _result
end
function GetPlayerByName(IN_playername)
	local _player
	local PlayerSetter = function (Player)
		_player = Player
	end
	cRoot:Get():FindAndDoWithPlayer(IN_playername, PlayerSetter)
	return _player
end
--[[
Not-so-usual math _functions
]]
-- Rounds floating point number. Because lua guys think this function doesn't deserve to be presented in lua's math
function round(IN_x)
  if (IN_x%2 ~= 0.5) then
    return math.floor(IN_x+0.5)
  end
  return IN_x-0.5
end
--[[
Functions I use for filework and stringswork
]]
function PluralString(IN_value, IN_singular_string, IN_plural_string)
	local _value_string = tostring(IN_value)
	if (_value_string[#_value_string] == "1") then
		return IN_singular_string
	end
	return IN_plural_string
end
function PluralItemName(IN_itemID, IN_ammount)	-- BEWARE! TEMPORAL SOLUTION THERE! :D
	local _value_string = tostring(IN_value)
	local _name = ""
	if (_value_string[#_value_string] == "1") then
		-- singular names
		_name = ItemTypeToString(IN_itemID)
	else
		-- plural names
		_name = ItemTypeToString(IN_itemID).."s"
	end
	return _name
end
-- for filewriting purposes. 0 = false, 1 = true
function StringToBool(value)
	if value=="1" then return true end
	return false
end
-- same, but reversal
function BoolToString(value)
	if value==true then return 1 end
	return 0
end