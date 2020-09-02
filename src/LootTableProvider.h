
// cLootTableProvider.h

#pragma once

#include "LootTable.h"

/*
This file contains all classes, types, ... used in the loot table functions.
The default loot tables are from LootTables/ in the root folder
The custom loot tables are read per world and must be contained in %worldname%/LootTables
They follow the vanilla file structure so any possible entry should be respected

The file names follow the cuberite NamingStructure.

Notes:
	30.08.2020:
		At the moment only loot tables for item container are supported.
	01.09.2020:
		Vanilla Minecraft uses a luck value per player that influences the outcome.
		This is not added to cuberite at the moment.

		When adding you need to:
		- add the luck processing to GetItem. The value is already parsed and stored
*/




/** The LootTableProvider is used per world to ask for loot tables */
class cLootTableProvider
{
public:
	cLootTableProvider(AString & a_Path, cWorld * a_World);

	/** Function to load a loot table from specified path */
	void LoadLootTable(const AString & a_String, const AString & a_Type);

	/** Functions to load loot tables. Custom loot tables are also checked */
	/*
	Further information on the string based function:
	Format:    Type|FurtherInfo
	example:   Chest|AbandonedMineshaft
	example:   Monster|Skeleton
	example:   Block|Stone
	This is not case sensitive, and removes all spaces */
	const cLootTable * GetLootTable(const AString & a_Name);
	const cLootTable * GetLootTable(const enum LootTable::eChestType a_Type);

private:

	/** Maps containing custom loot tables */
	cChestLootTableMap m_ChestLootTables;
	// cMonsterLootTableMap m_CustomMonsterLootTables = cMonsterLootTableMap();

	/** Empty loot table in case there is an error */
	static const cLootTable m_EmptyLootTable;

	cWorld * m_World;
};




