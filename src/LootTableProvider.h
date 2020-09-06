
// cLootTableProvider.h

#pragma once

#include "LootTable.h"

/*
This file describes the LootTable provider. The Provider is initialized per world
and loads default loot tables and custom ones.
The default loot tables are from LootTables/ in the root folder
The custom loot tables are read per world and must be contained in %worldname%/LootTables
They follow the vanilla file structure so any possible entry should be respected

The file names follow the cuberite NamingStructure.

Notes:
	30.08.2020:
		At the moment only loot tables for item container are supported.
*/




/** The LootTableProvider is used per world to ask for loot tables */
class cLootTableProvider
{
public:
	cLootTableProvider() = default;

	cLootTableProvider(AString & a_Path, cWorld * a_World);

	cLootTableProvider(const cLootTableProvider & a_Other) = default;
	cLootTableProvider(cLootTableProvider && a_Other) = default;
	cLootTableProvider & operator = (const cLootTableProvider & a_Other) = default;
	cLootTableProvider & operator = (cLootTableProvider && a_Other) = default;

	~cLootTableProvider() = default;

	/** Functions to load loot tables. Custom loot tables are also checked */
	/*
	Further information on the string based function:
	Format:    Type|FurtherInfo
	example:   Chest|AbandonedMineshaft
	example:   Monster|Skeleton
	example:   Block|Stone
	This is not case sensitive, and removes all spaces */
	const cLootTable * GetLootTable(const AString & a_Name);
	cLootTable * GetLootTable(enum LootTable::eChestType a_Type);

private:

	/** Maps containing the loot tables */
	cChestLootTableMap m_ChestLootTables;
	// cMonsterLootTableMap m_CustomMonsterLootTables = cMonsterLootTableMap();

	/** Function to load a loot table from specified path */
	void LoadLootTable(const AString & a_String, const AString & a_Type);

	/** Empty loot table in case there is an error */
	static cLootTable m_EmptyLootTable;

	cWorld * m_World;
};




