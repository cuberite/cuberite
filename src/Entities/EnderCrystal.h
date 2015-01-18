
#pragma once

#include "Entity.h"





// tolua_begin
class cEnderCrystal :
	public cEntity
{
	// tolua_end
	typedef cEntity super;

public:
	CLASS_PROTODEF(cEnderCrystal)

	cEnderCrystal(double a_X, double a_Y, double a_Z);

private:

	// cEntity overrides:
	virtual void SpawnOn(cClientHandle & a_ClientHandle) override;
	virtual void Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk) override;
	virtual void KilledBy(TakeDamageInfo & a_TDI) override;

};  // tolua_export




