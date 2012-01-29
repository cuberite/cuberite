
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cSignEntity.h"

#include "cPlayer.h"
#include "cClientHandle.h"
#include "cWorld.h"
#include "cChunk.h"
#include "cRoot.h"

#include "packets/cPacket_UpdateSign.h"

#include <json/json.h>





cSignEntity::cSignEntity(ENUM_BLOCK_ID a_BlockType, int a_X, int a_Y, int a_Z, cChunk* a_Chunk)
	: cBlockEntity(a_BlockType, a_X, a_Y, a_Z, a_Chunk)
{
}

cSignEntity::~cSignEntity()
{
}

// It don't do anything when 'used'
void cSignEntity::UsedBy( cPlayer & a_Player )
{
	(void)a_Player;
}

void cSignEntity::SetLines( const std::string & a_Line1, const std::string & a_Line2, const std::string & a_Line3, const std::string & a_Line4 )
{
	m_Line[0] = a_Line1;
	m_Line[1] = a_Line2;
	m_Line[2] = a_Line3;
	m_Line[3] = a_Line4;
}

void cSignEntity::SetLine( int a_Index, std::string a_Line )
{
	if( a_Index < 4 && a_Index > -1 )
	{
		m_Line[a_Index] = a_Line;
	}
}

std::string cSignEntity::GetLine( int a_Index )
{
	if( a_Index < 4 && a_Index > -1 )
	{
		return m_Line[a_Index];
	}
	return "";
}

void cSignEntity::SendTo( cClientHandle* a_Client )
{
	cPacket_UpdateSign Sign;
	Sign.m_PosX = m_PosX;
	Sign.m_PosY = (short)m_PosY;
	Sign.m_PosZ = m_PosZ;
	Sign.m_Line1 = m_Line[0];
	Sign.m_Line2 = m_Line[1];
	Sign.m_Line3 = m_Line[2];
	Sign.m_Line4 = m_Line[3];

	if( a_Client ) a_Client->Send( Sign );
	else // broadcast of a_Client == 0
	{
		GetChunk()->Broadcast( Sign );
	}
}

void cSignEntity::WriteToFile(FILE* a_File)
{
	fwrite( &m_BlockType, sizeof( ENUM_BLOCK_ID ), 1, a_File );
	fwrite( &m_PosX, sizeof( int ), 1, a_File );
	fwrite( &m_PosY, sizeof( int ), 1, a_File );
	fwrite( &m_PosZ, sizeof( int ), 1, a_File );

	for( int i = 0; i < 4; i++ )
	{
		short Size = (short)m_Line[i].size();
		fwrite( &Size, sizeof(short), 1, a_File );
		fwrite( m_Line[i].c_str(), Size * sizeof(char), 1, a_File );
	}
}

bool cSignEntity::LoadFromFile(FILE* a_File)
{
	if( fread( &m_PosX, sizeof(int), 1, a_File) != 1 ) { LOGERROR("ERROR READING SIGN FROM FILE"); return false; }
	if( fread( &m_PosY, sizeof(int), 1, a_File) != 1 ) { LOGERROR("ERROR READING SIGN FROM FILE"); return false; }
	if( fread( &m_PosZ, sizeof(int), 1, a_File) != 1 ) { LOGERROR("ERROR READING SIGN FROM FILE"); return false; }

	for( int i = 0; i < 4; i++ )
	{
		short Size = 0;
		if( fread( &Size, sizeof(short), 1, a_File) != 1 ) { LOGERROR("ERROR READING SIGN FROM FILE"); return false; }
		if( Size > 0 )
		{
			char* c_Str = new char[Size];
			if( fread( c_Str, Size * sizeof(char), 1, a_File) != 1 ) { LOGERROR("ERROR READING SIGN FROM FILE"); delete [] c_Str; return false; }
			m_Line[i].assign( c_Str, Size );
			delete [] c_Str;
		}
		LOG("Line %i: %s", i+1, m_Line[i].c_str() );
	}

	return true;
}

bool cSignEntity::LoadFromJson( const Json::Value & a_Value )
{
	m_PosX = a_Value.get("x", 0).asInt();
	m_PosY = a_Value.get("y", 0).asInt();
	m_PosZ = a_Value.get("z", 0).asInt();

	m_Line[0] = a_Value.get("Line1", "").asString();
	m_Line[1] = a_Value.get("Line2", "").asString();
	m_Line[2] = a_Value.get("Line3", "").asString();
	m_Line[3] = a_Value.get("Line4", "").asString();

	return true;
}

void cSignEntity::SaveToJson( Json::Value & a_Value )
{
	a_Value["x"] = m_PosX;
	a_Value["y"] = m_PosY;
	a_Value["z"] = m_PosZ;

	a_Value["Line1"] = m_Line[0];
	a_Value["Line2"] = m_Line[1];
	a_Value["Line3"] = m_Line[2];
	a_Value["Line4"] = m_Line[3];
}