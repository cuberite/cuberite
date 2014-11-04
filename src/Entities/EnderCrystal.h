
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

	cEnderCrystal(CreateEntityInfo a_Info);

private:

	// cEntity overrides:
	virtual void SpawnOn(cClientHandle & a_ClientHandle) override;
	virtual void Tick(float a_Dt, cChunk & a_Chunk) override;
	virtual void KilledBy(TakeDamageInfo & a_TDI) override;

};  // tolua_export




