
#pragma once

#include "Entity.h"
#include "Defines.h"




class cPlayer;
class cItem;






class cFallingBlock :
	public cEntity
{
	typedef cEntity super;
	
public:
	CLASS_PROTODEF(cFallingBlock);

	cFallingBlock(const Vector3i & a_BlockPosition, BLOCKTYPE a_BlockType);

	virtual void Initialize(cWorld * a_World) override;
	virtual void SpawnOn(cClientHandle & a_ClientHandle) override;
	virtual void Tick(float a_Dt) override;
	
private:
	BLOCKTYPE m_BlockType;
	Vector3i m_OriginalPosition;

	float m_SpeedY;

	static bool IsPassable(BLOCKTYPE a_BlockType)
	{
		return ((a_BlockType == E_BLOCK_AIR) || IsBlockLiquid(a_BlockType));
	}
} ;




