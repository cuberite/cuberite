
#pragma once

#include "ClientHandle.h"
#include "World.h"





namespace Json
{
	class Value;
};

class cPlayer;
class cPacket;





class cBlockEntity
{
protected:
	cBlockEntity(BLOCKTYPE a_BlockType, int a_BlockX, int a_BlockY, int a_BlockZ) :  // Used when generating
		m_PosX(a_BlockX),
		m_PosY(a_BlockY),
		m_PosZ(a_BlockZ),
		m_BlockType(a_BlockType),
		m_World(NULL)
	{
	}
	
	
	cBlockEntity(BLOCKTYPE a_BlockType, int a_BlockX, int a_BlockY, int a_BlockZ, cWorld * a_World) :
		m_PosX(a_BlockX),
		m_PosY(a_BlockY),
		m_PosZ(a_BlockZ),
		m_BlockType(a_BlockType),
		m_World(a_World)
	{
	}

public:
	virtual ~cBlockEntity() {};  // force a virtual destructor in all descendants
	
	virtual void Destroy(void) {};
	
	void SetWorld(cWorld * a_World)
	{
		m_World = a_World;
	}
	
	// Position, in absolute block coordinates:
	int GetPosX(void) const { return m_PosX; }
	int GetPosY(void) const { return m_PosY; }
	int GetPosZ(void) const { return m_PosZ; }

	BLOCKTYPE GetBlockType(void) const { return m_BlockType; }
	
	cWorld * GetWorld(void) const {return m_World; }

	virtual void SaveToJson  (Json::Value & a_Value) = 0;
	
	virtual void UsedBy( cPlayer * a_Player ) = 0;
	
	/** Sends the packet defining the block entity to the client specified.
	To send to all eligible clients, use cWorld::BroadcastBlockEntity()
	*/
	virtual void SendTo(cClientHandle & a_Client) = 0;
	
	/// Ticks the entity; returns true if the chunk should be marked as dirty as a result of this ticking. By default does nothing.
	virtual bool Tick(float a_Dt) { return false; }

protected:
	int m_PosX; // Position in absolute block coordinates
	int m_PosY;
	int m_PosZ;

	BLOCKTYPE m_BlockType;
	
	cWorld * m_World;
};




