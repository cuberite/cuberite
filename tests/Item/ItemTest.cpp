
#include "Globals.h"
#include "../TestHelpers.h"
#include "Item.h"

static void TestCreateAndAccess()
{
	// Create empty Item
	auto Item = cItem();
	TEST_EQUAL(Item.m_ItemType, E_ITEM_EMPTY);
	TEST_EQUAL(Item.m_ItemCount, 0);
	TEST_EQUAL(Item.m_ItemDamage, 0);
	TEST_EQUAL(Item.m_RepairCost, 0);
	TEST_TRUE(Item.IsBothNameAndLoreEmpty());

	auto Enchantments = Item.get<cEnchantments>();
	TEST_FALSE(Enchantments.has_value());

	auto FireworkItem = Item.get<cFireworkItem>();
	TEST_FALSE(FireworkItem.has_value());

	auto DisplayProperties = Item.get<cItem::cDisplayProperties>();
	TEST_FALSE(DisplayProperties.has_value());

	Item.Clear();

	// Create Item with just type
	Item = cItem(E_ITEM_BAKED_POTATO);
	TEST_EQUAL(Item.m_ItemType, E_ITEM_BAKED_POTATO);
	TEST_EQUAL(Item.m_ItemCount, 1);
	TEST_EQUAL(Item.m_ItemDamage, 0);
	TEST_EQUAL(Item.m_RepairCost, 0);
	TEST_TRUE(Item.IsBothNameAndLoreEmpty());

	Enchantments = Item.get<cEnchantments>();
	TEST_FALSE(Enchantments.has_value());

	FireworkItem = Item.get<cFireworkItem>();
	TEST_FALSE(FireworkItem.has_value());

	DisplayProperties = Item.get<cItem::cDisplayProperties>();
	TEST_FALSE(DisplayProperties.has_value());

	Item.Clear();

	// Create Item with count
	Item = cItem(E_ITEM_BAKED_POTATO, 20);
	TEST_EQUAL(Item.m_ItemType, E_ITEM_BAKED_POTATO);
	TEST_EQUAL(Item.m_ItemCount, 20);
	TEST_EQUAL(Item.m_ItemDamage, 0);
	TEST_EQUAL(Item.m_RepairCost, 0);
	TEST_TRUE(Item.IsBothNameAndLoreEmpty());

	Enchantments = Item.get<cEnchantments>();
	TEST_FALSE(Enchantments.has_value());

	FireworkItem = Item.get<cFireworkItem>();
	TEST_FALSE(FireworkItem.has_value());

	DisplayProperties = Item.get<cItem::cDisplayProperties>();
	TEST_FALSE(DisplayProperties.has_value());
	
	Item.Clear();
	
	// Create with item, count and damage
	Item = cItem(E_ITEM_BAKED_POTATO, 20, 12);
	TEST_EQUAL(Item.m_ItemType, E_ITEM_BAKED_POTATO);
	TEST_EQUAL(Item.m_ItemCount, 20);
	TEST_EQUAL(Item.m_ItemDamage, 12);
	TEST_EQUAL(Item.m_RepairCost, 0);
	TEST_TRUE(Item.IsBothNameAndLoreEmpty());

	Enchantments = Item.get<cEnchantments>();
	TEST_FALSE(Enchantments.has_value());

	FireworkItem = Item.get<cFireworkItem>();
	TEST_FALSE(FireworkItem.has_value());

	DisplayProperties = Item.get<cItem::cDisplayProperties>();
	TEST_FALSE(DisplayProperties.has_value());

	Item.Clear();
	
	// Create with item, count, damage, custom name
	Item = cItem(E_ITEM_BAKED_POTATO, 20, 12, "", "custom test name");
	TEST_EQUAL(Item.m_ItemType, E_ITEM_BAKED_POTATO);
	TEST_EQUAL(Item.m_ItemCount, 20);
	TEST_EQUAL(Item.m_ItemDamage, 12);
	TEST_EQUAL(Item.m_RepairCost, 0);
	TEST_TRUE(Item.IsLoreEmpty());
	TEST_FALSE(Item.IsCustomNameEmpty());

	Enchantments = Item.get<cEnchantments>();
	TEST_FALSE(Enchantments.has_value());

	FireworkItem = Item.get<cFireworkItem>();
	TEST_FALSE(FireworkItem.has_value());

	DisplayProperties = Item.get<cItem::cDisplayProperties>();
	TEST_TRUE(DisplayProperties.has_value());
	TEST_EQUAL(DisplayProperties->m_CustomName, "custom test name");

	// Create with item, count, damage, custom name
	auto LoreTable = AStringVector {"this", "is", "a", "lore", "table"};
	Item = cItem(E_ITEM_BAKED_POTATO, 20, 12, "", "custom test name", LoreTable);
	TEST_EQUAL(Item.m_ItemType, E_ITEM_BAKED_POTATO);
	TEST_EQUAL(Item.m_ItemCount, 20);
	TEST_EQUAL(Item.m_ItemDamage, 12);
	TEST_EQUAL(Item.m_RepairCost, 0);

	TEST_FALSE(Item.IsBothNameAndLoreEmpty());

	Enchantments = Item.get<cEnchantments>();
	TEST_FALSE(Enchantments.has_value());

	FireworkItem = Item.get<cFireworkItem>();
	TEST_FALSE(FireworkItem.has_value());

	DisplayProperties = Item.get<cItem::cDisplayProperties>();
	TEST_TRUE(DisplayProperties.has_value());
	TEST_EQUAL(DisplayProperties->m_CustomName, "custom test name");
	TEST_EQUAL(DisplayProperties->m_LoreTable.size(), LoreTable.size());
	for (std::size_t i = 0; i < LoreTable.size(); i++)
	{
		TEST_EQUAL(DisplayProperties->m_LoreTable[i], LoreTable[i]);
	}

	// Create with item, count, damage, custom name, lore table and enchantments
	Item = cItem(E_ITEM_BAKED_POTATO, 20, 12, "Efficiency=5;SilkTouch=5", "custom test name", LoreTable);
	TEST_EQUAL(Item.m_ItemType, E_ITEM_BAKED_POTATO);
	TEST_EQUAL(Item.m_ItemCount, 20);
	TEST_EQUAL(Item.m_ItemDamage, 12);
	TEST_EQUAL(Item.m_RepairCost, 0);

	TEST_FALSE(Item.IsBothNameAndLoreEmpty());

	Enchantments = Item.get<cEnchantments>();
	TEST_TRUE(Enchantments.has_value());

	TEST_EQUAL(std::distance(Enchantments->begin(), Enchantments->end()), 2);
	TEST_EQUAL(Enchantments->GetLevel(cEnchantments::enchEfficiency), 5);
	TEST_EQUAL(Enchantments->GetLevel(cEnchantments::enchSilkTouch), 5);

	FireworkItem = Item.get<cFireworkItem>();
	TEST_FALSE(FireworkItem.has_value());

	DisplayProperties = Item.get<cItem::cDisplayProperties>();
	TEST_TRUE(DisplayProperties.has_value());
	TEST_EQUAL(DisplayProperties->m_CustomName, "custom test name");
	TEST_EQUAL(DisplayProperties->m_LoreTable.size(), LoreTable.size());
	for (std::size_t i = 0; i < LoreTable.size(); i++)
	{
		TEST_EQUAL(DisplayProperties->m_LoreTable[i], LoreTable[i]);
	}
}

static void TestClear()
{
	auto Lore = AStringVector({"one", "two", "three"});
	auto Item = cItem(E_ITEM_BAKED_POTATO, 20, 12, "Efficiency=5", "lalala", Lore);

	TEST_EQUAL(Item.m_ItemType, E_ITEM_BAKED_POTATO);
	TEST_EQUAL(Item.m_ItemCount, 20);
	TEST_EQUAL(Item.m_ItemDamage, 12);
	TEST_EQUAL(Item.m_RepairCost, 0);
	TEST_FALSE(Item.IsBothNameAndLoreEmpty());

	auto Enchantments = Item.get<cEnchantments>();
	TEST_TRUE(Enchantments.has_value());
	TEST_EQUAL(Enchantments->GetLevel(cEnchantments::enchEfficiency), 5);

	auto FireworkItem = Item.get<cFireworkItem>();
	TEST_FALSE(FireworkItem.has_value());

	auto DisplayProperties = Item.get<cItem::cDisplayProperties>();
	TEST_TRUE(DisplayProperties.has_value());
	TEST_EQUAL(DisplayProperties->m_CustomName, "lalala");
	for (std::size_t i = 0; i < Lore.size(); i++)
	{
		TEST_EQUAL(DisplayProperties->m_LoreTable[i], Lore[i]);
	}

	Item.Clear();

	TEST_EQUAL(Item.m_ItemType, E_ITEM_EMPTY);
	TEST_EQUAL(Item.m_ItemCount, 0);
	TEST_EQUAL(Item.m_ItemDamage, 0);
	TEST_EQUAL(Item.m_RepairCost, 0);
	TEST_TRUE(Item.IsBothNameAndLoreEmpty());

	Enchantments = Item.get<cEnchantments>();
	TEST_FALSE(Enchantments.has_value());

	FireworkItem = Item.get<cFireworkItem>();
	TEST_FALSE(FireworkItem.has_value());

	DisplayProperties = Item.get<cItem::cDisplayProperties>();
	TEST_FALSE(DisplayProperties.has_value());
}

static void TestSet()
{
	auto Item = cItem();
	auto LoreTable = AStringVector {"Lore", "Table", "Test"};
	cItem::cDisplayProperties DisplayProperties;
	DisplayProperties.m_LoreTable = LoreTable;
	DisplayProperties.m_CustomName = "Custom Name";
	Item.set<cItem::cDisplayProperties>(DisplayProperties);

	auto NewDisplayProperties = Item.get<cItem::cDisplayProperties>();
	TEST_TRUE(NewDisplayProperties.has_value());
	TEST_EQUAL(NewDisplayProperties->m_CustomName, "Custom Name");
	TEST_EQUAL(NewDisplayProperties->m_LoreTable.size(), LoreTable.size());
	for (std::size_t i = 0; i < LoreTable.size(); i++)
	{
		TEST_EQUAL(NewDisplayProperties->m_LoreTable[i], LoreTable[i]);
	}

	auto Enchantments = cEnchantments("Efficiency=5;SilkTouch=5");
	Item.set<cEnchantments>(Enchantments);

	auto NewEnchantments = Item.get<cEnchantments>();
	TEST_TRUE(NewEnchantments.has_value());
	TEST_EQUAL(NewEnchantments->GetLevel(cEnchantments::enchEfficiency), Enchantments.GetLevel(cEnchantments::enchEfficiency));
	TEST_EQUAL(NewEnchantments->GetLevel(cEnchantments::enchSilkTouch), Enchantments.GetLevel(cEnchantments::enchSilkTouch));
	TEST_EQUAL(std::distance(NewEnchantments->begin(), NewEnchantments->end()), std::distance(Enchantments.begin(), Enchantments.end()));
}

IMPLEMENT_TEST_MAIN("Item",
	TestCreateAndAccess();
	TestClear();
	TestSet();
)