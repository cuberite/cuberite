
#pragma once

#include "cClientHandle.h"
#include "cWorld.h"





#ifndef _WIN32
#include "BlockID.h"
#else
enum ENUM_BLOCK_ID;
#endif





namespace Json
{
	class Value;
};

class cPlayer;
class cWorld;
class cPacket;





class cBlockEntity
{
protected:
	cBlockEntity(ENUM_BLOCK_ID a_BlockType, int a_BlockX, int a_BlockY, int a_BlockZ, cWorld * a_World) 
		: m_PosX( a_BlockX )
		, m_PosY( a_BlockY )
		, m_PosZ( a_BlockZ )
		, m_BlockType( a_BlockType ) 
		, m_World( a_World )
	{}
public:
	virtual ~cBlockEntity() {};
	virtual void Destroy() {};
	
	// Position, in absolute block coordinates:
	int GetPosX() { return m_PosX; }
	int GetPosY() { return m_PosY; }
	int GetPosZ() { return m_PosZ; }

	ENUM_BLOCK_ID GetBlockType() { return m_BlockType; }
	
	cWorld * GetWorld(void) const {return m_World; }

	virtual void SaveToJson  (Json::Value & a_Value ) = 0;
	
	virtual void UsedBy( cPlayer * a_Player ) = 0;
	
	/** Sends the packet defining the block entity to the client specified.
	To send to all eligible clients, use cWorld::BroadcastBlockEntity()
	*/
	virtual void SendTo(cClientHandle & a_Client) = 0;

protected:
	int m_PosX; // Position in absolute block coordinates
	int m_PosY;
	int m_PosZ;

	ENUM_BLOCK_ID m_BlockType;
	
	cWorld * m_World;
};




