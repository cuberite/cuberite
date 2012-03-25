
#include "Globals.h"

#include "cFileFormatUpdater.h"
#include "Vector3d.h"
#include "Vector3f.h"

#include "cItem.h"
#include <json/json.h>





void cFileFormatUpdater::UpdateFileFormat()
{
	UpdatePlayersOfWorld("world");
}





// Convert player .bin files to JSON
void cFileFormatUpdater::UpdatePlayersOfWorld( const char* a_WorldName )
{
	AString PlayerDir = AString( a_WorldName ) + "/player/";

	AStringList AllFiles = GetDirectoryContents( PlayerDir.c_str() );
	for (AStringList::iterator itr = AllFiles.begin(); itr != AllFiles.end(); ++itr )
	{
		AString & FileName = *itr;
		if (FileName.rfind(".bin") != AString::npos) // Get only the files ending in .bin
		{
			PlayerBINtoJSON( (PlayerDir + FileName).c_str() );
		}
	}
}





#define READ(File, Var) \
	if (File.Read(&Var, sizeof(Var)) != sizeof(Var)) \
	{ \
		LOGERROR("ERROR READING \"%s\" FROM FILE \"%s\"", #Var, a_FileName); \
		return; \
	}

// Converts player binary files to human readable JSON
void cFileFormatUpdater::PlayerBINtoJSON( const char* a_FileName )
{
	Vector3d	PlayerPos;
	Vector3f	PlayerRot;
	short		PlayerHealth = 0;

	const unsigned int NumInventorySlots = 45; // At this time the player inventory has/had 45 slots
	cItem IventoryItems[ NumInventorySlots ];

	cFile f;
	if (!f.Open(a_FileName, cFile::fmRead))
	{
		return;
	}

	// First read player position, rotation and health
	READ(f, PlayerPos.x);
	READ(f, PlayerPos.y);
	READ(f, PlayerPos.z);
	READ(f, PlayerRot.x);
	READ(f, PlayerRot.y);
	READ(f, PlayerRot.z);
	READ(f, PlayerHealth);


	for(unsigned int i = 0; i < NumInventorySlots; i++)
	{
		cItem & Item = IventoryItems[i];
		READ(f, Item.m_ItemID);
		READ(f, Item.m_ItemCount);
		READ(f, Item.m_ItemHealth);
	}
	f.Close();

	// Loaded all the data, now create the JSON data
	Json::Value JSON_PlayerPosition;
	JSON_PlayerPosition.append( Json::Value( PlayerPos.x ) );
	JSON_PlayerPosition.append( Json::Value( PlayerPos.y ) );
	JSON_PlayerPosition.append( Json::Value( PlayerPos.z ) );

	Json::Value JSON_PlayerRotation;
	JSON_PlayerRotation.append( Json::Value( PlayerRot.x ) );
	JSON_PlayerRotation.append( Json::Value( PlayerRot.y ) );
	JSON_PlayerRotation.append( Json::Value( PlayerRot.z ) );

	Json::Value JSON_Inventory;
	for(unsigned int i = 0; i < NumInventorySlots; i++)
	{
		Json::Value JSON_Item;
		IventoryItems[i].GetJson( JSON_Item );
		JSON_Inventory.append( JSON_Item );
	}

	Json::Value root;
	root["position"] = JSON_PlayerPosition;
	root["rotation"] = JSON_PlayerRotation;
	root["inventory"] = JSON_Inventory;
	root["health"] = PlayerHealth;

	Json::StyledWriter writer;
	std::string JsonData = writer.write( root );

	// Get correct filename
	std::string FileName = a_FileName;
	std::string FileNameWithoutExt = FileName.substr(0, FileName.find_last_of(".") );
	std::string FileNameJson = FileNameWithoutExt + ".json";

	// Write to file
	if (!f.Open(FileNameJson.c_str(), cFile::fmWrite))
	{
		return;
	}
	if (f.Write(JsonData.c_str(), JsonData.size()) != JsonData.size())
	{
		LOGERROR("ERROR WRITING PLAYER JSON TO FILE \"%s\"", FileNameJson.c_str());
		return;
	}
	f.Close();

	// Delete old format file, only do this when conversion has succeeded
	if (std::remove(a_FileName) != 0)
	{
		LOGERROR("COULD NOT DELETE old format file \"%s\"", a_FileName);
		return;
	}

	LOGINFO("Successfully converted binary to Json \"%s\"", FileNameJson.c_str() );
}




