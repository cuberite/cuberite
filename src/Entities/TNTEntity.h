
#pragma once

#include "Entity.h"




// tolua_begin
class cTNTEntity :
	public cEntity
{
	typedef cEntity super;

public:
	// tolua_end
	CLASS_PROTODEF(cTNTEntity)

	cTNTEntity(Vector3d a_Pos, int a_FuseTicks = 80);

	// cEntity overrides:
	virtual void SpawnOn(cClientHandle & a_ClientHandle) override;
	virtual void Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk) override;

	// tolua_begin

	/** Explode the tnt */
	void Explode(void);

	/** Returns the fuse ticks until the tnt will explode */
	int GetFuseTicks(void) const { return m_FuseTicks; }

	/** Set the fuse ticks until the tnt will explode */
	void SetFuseTicks(int a_FuseTicks) { m_FuseTicks = a_FuseTicks; }

	// tolua_end

protected:
	int m_FuseTicks;      ///< How much ticks is left, while the tnt will explode
};  // tolua_export




