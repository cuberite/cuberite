
#pragma once

#ifndef _WIN32
#include "BlockID.h"
#else
enum ENUM_BLOCK_ID;
#endif





class cClientHandle;
class cPlayer;
class cWorld;





class cBlockEntity
{
protected:
	cBlockEntity(ENUM_BLOCK_ID a_BlockType, int a_X, int a_Y, int a_Z, cWorld * a_World) 
		: m_PosX( a_X )
		, m_PosY( a_Y )
		, m_PosZ( a_Z )
		, m_BlockType( a_BlockType ) 
	{}
public:
	virtual ~cBlockEntity() {};
	virtual void Destroy() {};
	
	int GetPosX() { return m_PosX; }
	int GetPosY() { return m_PosY; }
	int GetPosZ() { return m_PosZ; }

	ENUM_BLOCK_ID GetBlockType() { return m_BlockType; }
	
	cWorld * GetWorld(void) const {return m_World; }

	virtual void UsedBy( cPlayer & a_Player ) = 0;
	virtual void SendTo( cClientHandle* a_Client ) { (void)a_Client; }

protected:
	int m_PosX; // Position in block coordinates
	int m_PosY;
	int m_PosZ;

	ENUM_BLOCK_ID m_BlockType;
	
	cWorld * m_World;
};




