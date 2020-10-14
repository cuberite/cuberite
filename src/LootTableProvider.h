
// cLootTableProvider.h

#pragma once

#include "LootTable.h"

/*
This file describes the LootTable provider. The Provider is initialized per world
and loads default loot tables and custom ones.
The default loot tables are from LootTables/ in the root folder
The custom loot tables are read per world and must be contained in %worldname%/LootTables

The file names follow the cuberite NamingStructure.
The files are in the same format as minecraft loot tables but also support CuberiteNamespace Key, Values.

Notes:
	30.08.2020:
		At the moment only loot tables for item container are supported.
*/




/** The LootTableProvider is used per world to ask for loot tables */
class cLootTableProvider
{
public:
	cLootTableProvider(cWorld & a_World);

	cLootTableProvider(const cLootTableProvider & a_Other) = default;
	cLootTableProvider(cLootTableProvider && a_Other) = default;

	~cLootTableProvider() = default;

	void ReloadLootTables();

	/** Returns a pointer to the loot table with the filename a_Name.
		Try's to load the file from disk if there is none present in memory */
	const cLootTable * GetLootTable(const AString & a_Name);

private:

	/** Function to load a loot table from specified path. */
	void LoadLootTable(const AString & a_Path, const AString & a_Name);

	/** Finds all files in a_Path recursive and tries to load them as loot tables. */
	void LoadLootTablesFromFolder(const AString & a_Path);

	/** Tries to load a loot table with the name a_FileName from disk in folder a_Path recursive. */
	bool FindLootTable(const AString & a_Path, const AString & a_FileName);

	/** Maps containing the loot tables */
	cLootTables m_LootTables;

	/** Empty loot table in case there is an error */
	static cLootTable m_EmptyLootTable;

	cWorld & m_World;
};




