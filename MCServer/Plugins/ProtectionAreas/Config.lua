
-- Config.lua

-- Implements the cConfig class that holds the general plugin configuration





cConfig = {
	m_Wand = cItem(E_ITEM_STICK, 1, 1);  -- TODO: Make this configurable by loading it from an INI file
};





--- Returns true if a_Item is the wand tool item
function cConfig:IsWand(a_Item)
	return (
		(a_Item.m_ItemType   == self.m_Wand.m_ItemType) and
		(a_Item.m_ItemDamage == self.m_Wand.m_ItemDamage)
	);
end





--- Returns the wand tool item as a cItem object
function cConfig:GetWandItem()
	return self.m_Wand;
end