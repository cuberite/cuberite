#pragma once

class cFileFormatUpdater
{
public:
	static void UpdateFileFormat();
private:
	static void UpdatePlayersOfWorld( const char* a_WorldName );
	
	static void PlayerBINtoJSON( const char* a_FileName );
};