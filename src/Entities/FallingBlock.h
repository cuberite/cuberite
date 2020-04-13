
#pragma once

#include "Entity.h"





// tolua_begin
class cFallingBlock :
	public cEntity
{
	// tolua_end

	using Super = cEntity;

public:  // tolua_export

	CLASS_PROTODEF(cFallingBlock)

	/** Creates a new falling block.
	a_Position is expected in world coords */
	cFallingBlock(Vector3d a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta);

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
} ;  // tolua_export




