
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

	cFallingBlock(const Vector3i & a_BlockPosition, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta);

	BLOCKTYPE  GetBlockType(void) const { return m_BlockType; }
	NIBBLETYPE GetBlockMeta(void) const { return m_BlockMeta; }
	
	// cEntity overrides:
	virtual void Initialize(cWorld * a_World) override;
	virtual void SpawnOn(cClientHandle & a_ClientHandle) override;
	virtual void Tick(float a_Dt, cChunk & a_Chunk) override;
	
private:
	BLOCKTYPE  m_BlockType;
	NIBBLETYPE m_BlockMeta;
	Vector3i   m_OriginalPosition;
} ;




