
// SignEntity.cpp

// Implements the cSignEntity class representing a single sign in the world

#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules
#include "json/value.h"
#include "SignEntity.h"
#include "../ClientHandle.h"





cSignEntity::cSignEntity(BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, Vector3i a_Pos, cWorld * a_World):
	Super(a_BlockType, a_BlockMeta, a_Pos, a_World)
{
	ASSERT((a_BlockType ==  E_BLOCK_WALLSIGN) || (a_BlockType == E_BLOCK_SIGN_POST));
	ASSERT(cChunkDef::IsValidHeight(a_Pos.y));
}





void cSignEntity::CopyFrom(const cBlockEntity & a_Src)
{
	Super::CopyFrom(a_Src);
	auto & src = static_cast<const cSignEntity &>(a_Src);
	for (size_t i = 0; i < ARRAYCOUNT(m_Line); ++i)
	{
		m_Line[i] = src.m_Line[i];
	}
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
	a_Client.SendUpdateSign(m_Pos.x, m_Pos.y, m_Pos.z, m_Line[0], m_Line[1], m_Line[2], m_Line[3]);
}
