
-- Implements item-repair using the HOOK_CRAFTING_NO_RECIPE hook
-- Based on Fixies plugin v2 by Taugeshtu


-- how much "extra" points are healed per a repair operation (fraction of full health)
BONUS = 0.1





function OnCraftingNoRecipe(Player, Grid, Recipe)
	local _do_fix = false
	local Items = {}
	for x = 0, Grid:GetWidth() - 1 do
		for y = 0, Grid:GetHeight() - 1 do
			local Item = Grid:GetItem(x, y)
			if (Item.m_ItemType ~= E_ITEM_EMPTY) then
				table.insert(Items, Item)
			end
		end
	end
	
	if (#Items ~= 2) then
		-- Only two items together can be fixed
		return false
	end
	
	if (Items[1].m_ItemType ~= Items[2].m_ItemType) then
		-- Only items of the same type may be fixed
		return false
	end
	
	if (
		(Items[1].m_ItemDamage == 0)	or
		(Items[2].m_ItemDamage == 0)
	)
	then
		-- Only damaged items may be fixed
		return false
	end
	
	local _ID = Items[1].m_ItemType
	local _least_hp = math.max(Items[1].m_ItemDamage, Items[2].m_ItemDamage)
	local _most_hp = math.min(Items[1].m_ItemDamage, Items[2].m_ItemDamage)
	local _item_hp = 0
	
	-- TODO: This could be refactored into better code, using an _ID-indexed table for _item_hp
	
	if (
		(_ID == E_ITEM_WOODEN_SHOVEL)  or
		(_ID == E_ITEM_WOODEN_AXE)     or
		(_ID == E_ITEM_WOODEN_PICKAXE) or
		(_ID == E_ITEM_WOODEN_SWORD)   or
		(_ID == E_ITEM_WOODEN_HOE)
	)
	then
		_item_hp = 60
		_do_fix = true
	end
	
	if (
		(_ID == E_ITEM_STONE_SHOVEL)  or
		(_ID == E_ITEM_STONE_AXE)     or
		(_ID == E_ITEM_STONE_PICKAXE) or
		(_ID == E_ITEM_STONE_SWORD)   or
		(_ID == E_ITEM_STONE_HOE)
	)
	then
		_item_hp = 132
		_do_fix = true
	end
	
	if (
		(_ID == E_ITEM_IRON_SHOVEL)  or
		(_ID == E_ITEM_IRON_AXE)     or
		(_ID == E_ITEM_IRON_PICKAXE) or
		(_ID == E_ITEM_IRON_SWORD)   or
		(_ID == E_ITEM_IRON_HOE)
	)
	then
		_item_hp = 251
		_do_fix = true
	end
	
	if (
		(_ID == E_ITEM_GOLD_SHOVEL)  or
		(_ID == E_ITEM_GOLD_AXE)     or
		(_ID == E_ITEM_GOLD_PICKAXE) or
		(_ID == E_ITEM_GOLD_SWORD)   or
		(_ID == E_ITEM_GOLD_HOE)
	)
	then
		_item_hp = 33
		_do_fix = true
	end
	
	if (
		(_ID == E_ITEM_DIAMOND_SHOVEL)  or
		(_ID == E_ITEM_DIAMOND_AXE)     or
		(_ID == E_ITEM_DIAMOND_PICKAXE) or
		(_ID == E_ITEM_DIAMOND_SWORD)   or
		(_ID == E_ITEM_DIAMOND_HOE)
	)
	then
		_item_hp = 1562
		_do_fix = true
	end
	
	if (_ID == E_ITEM_LEATHER_CAP) then
		_item_hp = 56
		_do_fix = true
	end
	if (_ID == E_ITEM_LEATHER_TUNIC) then
		_item_hp = 82
		_do_fix = true
	end
	if (_ID == E_ITEM_LEATHER_PANTS) then
		_item_hp = 76
		_do_fix = true
	end
	if (_ID == E_ITEM_LEATHER_BOOTS) then
		_item_hp = 66
		_do_fix = true
	end
	
	
	if (_ID == E_ITEM_CHAIN_HELMET) then
		_item_hp = 78
		_do_fix = true
	end
	if (_ID == E_ITEM_CHAIN_CHESTPLATE) then
		_item_hp = 114
		_do_fix = true
	end
	if (_ID == E_ITEM_CHAIN_LEGGINGS) then
		_item_hp = 106
		_do_fix = true
	end
	if (_ID == E_ITEM_CHAIN_BOOTS) then
		_item_hp = 92
		_do_fix = true
	end
	
	
	if (_ID == E_ITEM_IRON_HELMET) then
		_item_hp = 166
		_do_fix = true
	end
	if (_ID == E_ITEM_IRON_CHESTPLATE) then
		_item_hp = 242
		_do_fix = true
	end
	if (_ID == E_ITEM_IRON_LEGGINGS) then
		_item_hp = 226
		_do_fix = true
	end
	if (_ID == E_ITEM_IRON_BOOTS) then
		_item_hp = 196
		_do_fix = true
	end
	
	
	if (_ID == E_ITEM_GOLD_HELMET) then
		_item_hp = 78
		_do_fix = true
	end
	if (_ID == E_ITEM_GOLD_CHESTPLATE) then
		_item_hp = 114
		_do_fix = true
	end
	if (_ID == E_ITEM_GOLD_LEGGINGS) then
		_item_hp = 106
		_do_fix = true
	end
	if (_ID == E_ITEM_GOLD_BOOTS) then
		_item_hp = 92
		_do_fix = true
	end
	
	
	if (_ID == E_ITEM_DIAMOND_HELMET)	then
		_item_hp = 364
		_do_fix = true
	end
	if (_ID == E_ITEM_DIAMOND_CHESTPLATE)then
		_item_hp = 529
		_do_fix = true
	end
	if (_ID == E_ITEM_DIAMOND_LEGGINGS)	then
		_item_hp = 496
		_do_fix = true
	end
	if (_ID == E_ITEM_DIAMOND_BOOTS)		then
		_item_hp = 430
		_do_fix = true
	end
	-- /////////////////////////////////////////////////////
	
	if (_do_fix == true) then
		local _hp = _most_hp - (_item_hp - _least_hp) - _item_hp * BONUS
		_hp = math.max(_hp, 0)
		Recipe:SetResult(_ID, 1, _hp)
		Recipe:SetIngredient(Items[1].x, Items[1].y, Items[1]);
		Recipe:SetIngredient(Items[2].x, Items[2].y, Items[2]);
		return true
	end
	return false
end




