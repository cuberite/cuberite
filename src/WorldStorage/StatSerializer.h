
// StatSerializer.h

// Declares the cStatSerializer class that is used for saving stats into JSON





#pragma once





// fwd:
class cStatManager;
namespace Json { class Value; }





namespace StatSerializer
{
	/* Try to load the player statistics. */
	void Load(cStatManager & Manager, const std::string & WorldPath, std::string && FileName);

	/* Try to save the player statistics. */
	void Save(const cStatManager & Manager, const std::string & WorldPath, std::string && FileName);
}
