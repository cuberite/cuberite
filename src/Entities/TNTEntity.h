
#pragma once

#include "Entity.h"




// tolua_begin
class cTNTEntity :
	public cEntity
{
	typedef cEntity super;
	
public:
	// tolua_end
	CLASS_PROTODEF(cTNTEntity);

	cTNTEntity(double a_X, double a_Y, double a_Z, double a_FuseTimeInSec = 4);
	cTNTEntity(const Vector3d & a_Pos, double a_FuseTimeInSec = 4);
	
	// cEntity overrides:
	virtual void SpawnOn(cClientHandle & a_ClientHandle) override;
	virtual void Tick(float a_Dt, cChunk & a_Chunk) override;
	
	// tolua_begin
	
	/** Explode the tnt */
	void Explode(void);
	
	/** Returns the fuse ticks until the tnt will explode */
	double GetFuseTicks(void) const { return m_FuseTicks; }
	
	/** Set the fuse ticks until the tnt will explode */
	void SetFuseTicks(double a_FuseTicks) { m_FuseTicks = a_FuseTicks; }
	
	// tolua_end
	
protected:
	double m_FuseTicks;      ///< How much time in seconds is left, while the tnt will explode
}; // tolua_export




