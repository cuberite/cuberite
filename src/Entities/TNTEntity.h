
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

	cTNTEntity(double a_X, double a_Y, double a_Z, double a_FuseTimeInSec);
	cTNTEntity(const Vector3d & a_Pos, double a_FuseTimeInSec);
	
	// cEntity overrides:
	virtual void SpawnOn(cClientHandle & a_ClientHandle) override;
	virtual void Tick(float a_Dt, cChunk & a_Chunk) override;

	double GetCounterTime(void) const { return m_Counter; } // tolua_export
	double GetMaxFuseTime(void) const { return m_MaxFuseTime; } // tolua_export
	
protected:
	double m_Counter;      ///< How much time has elapsed since the object was created, in seconds
	double m_MaxFuseTime;  ///< How long the fuse is, in seconds
}; // tolua_export




