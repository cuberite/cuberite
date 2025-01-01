
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
	cFallingBlock(Vector3d a_Position, BlockState a_Block);

	// tolua_begin

	BlockState GetBlock(void) const { return m_Block; }

	// tolua_end

	// cEntity overrides:
	virtual void SpawnOn(cClientHandle & a_ClientHandle) override;
	virtual void Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk) override;

private:
	BlockState m_Block;
} ;  // tolua_export




