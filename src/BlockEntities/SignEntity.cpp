
// SignEntity.cpp

// Implements the cSignEntity class representing a single sign in the world

#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules
#include "json/value.h"
#include "SignEntity.h"
#include "../ClientHandle.h"
#include "../Blocks/BlockWallSign.h"





cSignEntity::cSignEntity(BlockState a_Block, Vector3i a_Pos, cWorld * a_World):
	Super(a_Block, a_Pos, a_World)
{
	ASSERT(cBlockWallSignHandler::IsBlockSignPost(a_Block) ||cBlockWallSignHandler::IsBlockWallSign(a_Block));
	ASSERT(cChunkDef::IsValidHeight(a_Pos));
}





void cSignEntity::CopyFrom(const cBlockEntity & a_Src)
{
	Super::CopyFrom(a_Src);
	auto & src = static_cast<const cSignEntity &>(a_Src);
	m_Line = src.m_Line;
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





void cSignEntity::SetLine(size_t a_Index, const AString & a_Line)
{
	if (a_Index >= m_Line.size())
	{
		LOGWARNING("%s: setting a non-existent line %d (value \"%s\"", __FUNCTION__, a_Index, a_Line.c_str());
		return;
	}

	m_Line[a_Index] = a_Line;
}





AString cSignEntity::GetLine(size_t a_Index) const
{
	if (a_Index >= m_Line.size())
	{
		LOGWARNING("%s: requesting a non-existent line %d", __FUNCTION__, a_Index);
		return "";
	}

	return m_Line[a_Index];
}





void cSignEntity::SendTo(cClientHandle & a_Client)
{
	a_Client.SendUpdateSign(m_Pos, m_Line[0], m_Line[1], m_Line[2], m_Line[3]);
}
