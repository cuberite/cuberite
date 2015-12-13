
// SignEntity.cpp

// Implements the cSignEntity class representing a single sign in the world

#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules
#include "json/value.h"
#include "SignEntity.h"
#include "../ClientHandle.h"





cSignEntity::cSignEntity(BLOCKTYPE a_BlockType, int a_X, int a_Y, int a_Z, cWorld * a_World) :
	super(a_BlockType, a_X, a_Y, a_Z, a_World)
{
	ASSERT((a_Y >= 0) && (a_Y < cChunkDef::Height));
}





bool cSignEntity::UsedBy(cPlayer * a_Player)
{
	UNUSED(a_Player);
	return true;
}





void cSignEntity::SetLines(const AString & a_Line1, const AString & a_Line2, const AString & a_Line3, const AString & a_Line4)
{
	m_Line[0] = a_Line1;
	m_Line[1] = a_Line2;
	m_Line[2] = a_Line3;
	m_Line[3] = a_Line4;
}





void cSignEntity::SetLine(int a_Index, const AString & a_Line)
{
	if ((a_Index < 0) || (a_Index >= static_cast<int>(ARRAYCOUNT(m_Line))))
	{
		LOGWARNING("%s: setting a non-existent line %d (value \"%s\"", __FUNCTION__, a_Index, a_Line.c_str());
		return;
	}
	m_Line[a_Index] = a_Line;
}





AString cSignEntity::GetLine(int a_Index) const
{
	if ((a_Index < 0) || (a_Index >= static_cast<int>(ARRAYCOUNT(m_Line))))
	{
		LOGWARNING("%s: requesting a non-existent line %d", __FUNCTION__, a_Index);
		return "";
	}
	return m_Line[a_Index];
}





void cSignEntity::SendTo(cClientHandle & a_Client)
{
	a_Client.SendUpdateSign(m_PosX, m_PosY, m_PosZ, m_Line[0], m_Line[1], m_Line[2], m_Line[3]);
}
