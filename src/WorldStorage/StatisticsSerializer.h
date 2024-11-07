
// StatisticsSerializer.h

// Declares the cStatSerializer class that is used for saving stats into JSON





#pragma once





struct StatisticsManager;
namespace Json { class Value; }





namespace StatisticsSerializer
{
	/* Try to load the player statistics. */
	void Load(StatisticsManager & Manager, const std::string & WorldPath, std::string && FileName);

	/* Try to save the player statistics. */
	void Save(const StatisticsManager & Manager, const std::string & WorldPath, std::string && FileName);
}
