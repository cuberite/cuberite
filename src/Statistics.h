
// Statistics.h

#pragma once

#include "Registries/Statistics.h"

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
exported from the server https://wiki.vg/Data_Generators

		Greetings 12xx12 */





/** Class that manages the statistics and achievements of a single player. */
class cStatManager
{
public:

	typedef unsigned StatValue;
	typedef std::unordered_map<Statistic, StatValue> CustomStore;

	/** Set the value of the specified statistic. */
	void SetValue(Statistic a_Stat, StatValue a_Value);

	/** Increments the specified statistic. Returns the new value. */
	StatValue AddValue(Statistic a_Stat, StatValue a_Delta = 1);

	/** Returns whether the prerequisite for awarding an achievement are satisfied. */
	bool SatisfiesPrerequisite(Statistic a_Stat);

	/** Invokes the given callbacks for each category of tracked statistics. */
	template <class CustomCallback>
	void ForEachStatisticType(CustomCallback a_Custom) const
	{
		a_Custom(m_CustomStatistics);
	}

private:

	/** Returns if a statistic is both present and has nonzero value. */
	bool IsStatisticPresent(Statistic a_Stat) const;

	// TODO: Block tallies, entities killed, all the others

	CustomStore m_CustomStatistics;
};
