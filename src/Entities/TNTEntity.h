
#pragma once

#include "Entity.h"




// tolua_begin
class cTNTEntity:
	public cEntity
{

	// tolua_end

	using Super = cEntity;

public:  // tolua_export

	CLASS_PROTODEF(cTNTEntity)

	cTNTEntity(Vector3d a_Pos, unsigned a_FuseTicks = 80);

	// cEntity overrides:
	virtual void SpawnOn(cClientHandle & a_ClientHandle) override;
	virtual void Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk) override;

	// tolua_begin

	/** Explode the tnt */
	void Explode(void);

	/** Returns the fuse ticks until the tnt will explode */
	unsigned GetFuseTicks(void) const { return m_FuseTicks; }

	/** Set the fuse ticks until the tnt will explode */
	void SetFuseTicks(unsigned a_FuseTicks) { m_FuseTicks = a_FuseTicks; }

	// tolua_end

protected:
	unsigned m_FuseTicks;      ///< How much ticks is left, while the tnt will explode
};  // tolua_export
