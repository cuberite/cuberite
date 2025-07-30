
// Statistics.h

/* Hello fellow developer !
In case you are trying to add new statistics to Cuberite you need to do a few things:
---------------------------------------------------------------------------
1. add a new entry to the enum class Statistic in Registries\Statistics.h file
2. add this to serialization functions in WorldStorage\NamespaceSerializer.cpp
	The String in the above is used for saving on disk!
	so use the same string!

In case you want to add a mapping of network IDs to the used stats
you will find a lua script in ../Tools/BlockTypePaletteGenerator/ExportStatMapping.lua
it will provide you with information how to use it. you need a registries.json
exported from the server https://minecraft.wiki/w/Minecraft_Wiki:Projects/wiki.vg_merge/Data_Generators

		Greetings 12xx12 */





#pragma once

#include "Registries/CustomStatistics.h"





/** Class that manages the statistics and achievements of a single player. */
struct StatisticsManager
{
	typedef unsigned StatValue;

	// TODO: Block tallies, entities killed, all the others

	std::unordered_map<CustomStatistic, StatValue> Custom;

	/** Returns whether the prerequisite for awarding an achievement are satisfied. */
	bool SatisfiesPrerequisite(CustomStatistic a_Stat) const;

private:

	/** Returns if a statistic is both present and has nonzero value. */
	bool IsStatisticPresent(CustomStatistic a_Stat) const;
};
