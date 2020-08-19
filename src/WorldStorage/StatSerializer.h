
// StatSerializer.h

// Declares the cStatSerializer class that is used for saving stats into JSON





#pragma once





// fwd:
class cStatManager;
namespace Json { class Value; }





class cStatSerializer
{
public:

	cStatSerializer(cStatManager & Manager, const std::string & WorldPath, std::string FileName);

	/* Try to load the player statistics. */
	void Load(void);

	/* Try to save the player statistics. */
	void Save(void);

private:

	void SaveStatToJSON(Json::Value & a_Out);

	void LoadLegacyFromJSON(const Json::Value & In);

	void LoadCustomStatFromJSON(const Json::Value & a_In);

	cStatManager & m_Manager;

	std::string m_Path;
} ;
