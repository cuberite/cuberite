
#include "Globals.h"

#include "cFileFormatUpdater.h"
#include "cMCLogger.h"
#include "Vector3d.h"
#include "Vector3f.h"

#include "cItem.h"
#include <json/json.h>

typedef std::list< std::string > StringList;
StringList GetDirectoryContents( const char* a_Directory );

void cFileFormatUpdater::UpdateFileFormat()
{
	UpdatePlayersOfWorld("world");
}

// Convert player .bin files to JSON
void cFileFormatUpdater::UpdatePlayersOfWorld( const char* a_WorldName )
{
	std::string PlayerDir = std::string( a_WorldName ) + "/player/";

	StringList AllFiles = GetDirectoryContents( PlayerDir.c_str() );
	for( StringList::iterator itr = AllFiles.begin(); itr != AllFiles.end(); ++itr )
	{
		std::string & FileName = *itr;
		if( FileName.rfind(".bin") != std::string::npos ) // Get only the files ending in .bin
		{
			PlayerBINtoJSON( (PlayerDir + FileName).c_str() );
		}
	}
}

// Converts player binary files to human readable JSON
void cFileFormatUpdater::PlayerBINtoJSON( const char* a_FileName )
{
	Vector3d	PlayerPos;
	Vector3f	PlayerRot;
	short		PlayerHealth = 0;

	const unsigned int NumInventorySlots = 45; // At this time the player inventory has/had 45 slots
	cItem IventoryItems[ NumInventorySlots ];

	FILE* f;
#ifdef _WIN32
	if( fopen_s(&f, a_FileName, "rb" ) == 0 )	// no error
#else
	if( (f = fopen(a_FileName, "rb" ) ) != 0 )	// no error
#endif
	{
		// First read player position, rotation and health
		if( fread( &PlayerPos.x, sizeof(double), 1, f) != 1 )	{ LOGERROR("ERROR READING FROM FILE %s", a_FileName); fclose(f); return; }
		if( fread( &PlayerPos.y, sizeof(double), 1, f) != 1 )	{ LOGERROR("ERROR READING FROM FILE %s", a_FileName); fclose(f); return; }
		if( fread( &PlayerPos.z, sizeof(double), 1, f) != 1 )	{ LOGERROR("ERROR READING FROM FILE %s", a_FileName); fclose(f); return; }
		if( fread( &PlayerRot.x, sizeof(float), 1, f) != 1 )	{ LOGERROR("ERROR READING FROM FILE %s", a_FileName); fclose(f); return; }
		if( fread( &PlayerRot.y, sizeof(float), 1, f) != 1 )	{ LOGERROR("ERROR READING FROM FILE %s", a_FileName); fclose(f); return; }
		if( fread( &PlayerRot.z, sizeof(float), 1, f) != 1 )	{ LOGERROR("ERROR READING FROM FILE %s", a_FileName); fclose(f); return; }
		if( fread( &PlayerHealth, sizeof(short), 1, f) != 1 )	{ LOGERROR("ERROR READING FROM FILE %s", a_FileName); fclose(f); return; }


		for(unsigned int i = 0; i < NumInventorySlots; i++)
		{
			cItem & Item = IventoryItems[i];
			if( fread( &Item.m_ItemID,		sizeof(Item.m_ItemID), 1, f)	 != 1 ) { LOGERROR("ERROR READING INVENTORY FROM FILE"); return; }
			if( fread( &Item.m_ItemCount,	sizeof(Item.m_ItemCount), 1, f) != 1 )	{ LOGERROR("ERROR READING INVENTORY FROM FILE"); return; }
			if( fread( &Item.m_ItemHealth,	sizeof(Item.m_ItemHealth), 1, f)!= 1 )	{ LOGERROR("ERROR READING INVENTORY FROM FILE"); return; }
		}

		fclose(f);
	}

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
#ifdef _WIN32
	if( fopen_s(&f, FileNameJson.c_str(), "wb" ) == 0 )	// no error
#else
	if( (f = fopen(FileNameJson.c_str(), "wb" ) ) != 0 )	// no error
#endif
	{
		if( fwrite( JsonData.c_str(), JsonData.size(), 1, f ) != 1 )	{ LOGERROR("ERROR WRITING PLAYER JSON TO FILE %s", FileNameJson.c_str() ); return; }
		fclose( f );
	}

	// Delete old format file, only do this when conversion has succeeded
	if( std::remove( a_FileName ) != 0 )
	{
		LOGERROR("COULD NOT DELETE FILE %s", a_FileName );
		return;
	}

	LOGINFO("Successfully converted binary to Json %s", FileNameJson.c_str() );
}









// Helper function
StringList GetDirectoryContents( const char* a_Directory )
{
	StringList AllFiles;
#ifdef _WIN32
	std::string FileFilter = std::string( a_Directory ) + "*.*";
	HANDLE hFind;
	WIN32_FIND_DATA FindFileData;

	if( ( hFind = FindFirstFile(FileFilter.c_str(), &FindFileData) ) != INVALID_HANDLE_VALUE)
	{
		do
		{
			AllFiles.push_back( FindFileData.cFileName );
		} while( FindNextFile(hFind, &FindFileData) );
		FindClose(hFind);
	}
#else
	DIR *dp;
	struct dirent *dirp;
	if( (dp  = opendir( a_Directory ) ) == NULL) 
	{
		LOGERROR("Error (%i) opening %s\n", errno, a_Directory );
	}
	else
	{
		while ((dirp = readdir(dp)) != NULL) 
		{
			AllFiles.push_back( dirp->d_name );
		}
		closedir(dp);
	}
#endif

	return AllFiles;
}