
// StatSerializer.h

// Declares the cStatSerializer class that is used for saving stats into JSON





#pragma once

#include "json/json.h"





// fwd:
class cStatManager;




class cStatSerializer
{
public:

	cStatSerializer(const AString & a_WorldName, const AString & a_PlayerName, cStatManager * a_Manager);

	/* Try to load the player statistics. Returns whether the operation was successful or not. */
	bool Load(void);

	/* Try to save the player statistics. Returns whether the operation was successful or not. */
	bool Save(void);


protected:

	void SaveStatToJSON(Json::Value & a_Out);

	bool LoadStatFromJSON(const Json::Value & a_In);


private:

	cStatManager* m_Manager;

	AString m_Path;


} ;




