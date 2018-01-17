
#pragma once

#include "Entity.h"





// tolua_begin

class cFallingBlock :
	public cEntity
{
	typedef cEntity super;

public:
	// tolua_end

	CLASS_PROTODEF(cFallingBlock)

	/** Creates a new falling block. a_BlockPosition is expected in world coords */
	cFallingBlock(const Vector3i & a_BlockPosition, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta);

	// tolua_begin

	BLOCKTYPE  GetBlockType(void) const { return m_BlockType; }
	NIBBLETYPE GetBlockMeta(void) const { return m_BlockMeta; }

	// tolua_end

	// cEntity overrides:
	virtual void SpawnOn(cClientHandle & a_ClientHandle) override;
	virtual void Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk) override;

private:
	BLOCKTYPE  m_BlockType;
	NIBBLETYPE m_BlockMeta;
	Vector3i   m_OriginalPosition;  // Position where the falling block has started, in world coords
} ;  // tolua_export




