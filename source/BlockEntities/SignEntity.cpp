
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "SignEntity.h"

#include "../Entities/Player.h"
// #include "ClientHandle.h"
// #include "World.h"
// #include "Root.h"

#include <json/json.h>





cSignEntity::cSignEntity(BLOCKTYPE a_BlockType, int a_X, int a_Y, int a_Z, cWorld * a_World)
	: cBlockEntity(a_BlockType, a_X, a_Y, a_Z, a_World)
{
}





cSignEntity::~cSignEntity()
{
}





// It don't do anything when 'used'
void cSignEntity::UsedBy( cPlayer * a_Player )
{
	(void)a_Player;
}





void cSignEntity::SetLines( const AString & a_Line1, const AString & a_Line2, const AString & a_Line3, const AString & a_Line4 )
{
	m_Line[0] = a_Line1;
	m_Line[1] = a_Line2;
	m_Line[2] = a_Line3;
	m_Line[3] = a_Line4;
}





void cSignEntity::SetLine( int a_Index, const AString & a_Line )
{
	if( a_Index < 4 && a_Index > -1 )
	{
		m_Line[a_Index] = a_Line;
	}
}





AString cSignEntity::GetLine( int a_Index ) const
{
	if( a_Index < 4 && a_Index > -1 )
	{
		return m_Line[a_Index];
	}
	return "";
}





void cSignEntity::SendTo(cClientHandle & a_Client)
{
	a_Client.SendUpdateSign(m_PosX, m_PosY, m_PosZ, m_Line[0], m_Line[1], m_Line[2], m_Line[3]);
}





#define READ(File, Var) \
	if (File.Read(&Var, sizeof(Var)) != sizeof(Var)) \
	{ \
		LOGERROR("ERROR READING cSignEntity %s FROM FILE (line %d)", #Var, __LINE__); \
		return false; \
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




