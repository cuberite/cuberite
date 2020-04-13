
#pragma once

#include "HangingEntity.h"





// tolua_begin
class cPainting :
	public cHangingEntity
{
	// tolua_end

	using Super = cHangingEntity;

public:  // tolua_export

	CLASS_PROTODEF(cPainting)

	cPainting(const AString & a_Name, eBlockFace a_Direction, Vector3d a_Pos);

	/** Returns the protocol name of the painting */
	const AString & GetName(void) const { return m_Name; }  // tolua_export

private:

	virtual void SpawnOn(cClientHandle & a_Client) override;
	virtual void GetDrops(cItems & a_Items, cEntity * a_Killer) override;
	virtual void KilledBy(TakeDamageInfo & a_TDI) override
	{
		Super::KilledBy(a_TDI);
		Destroy();
	}

	AString m_Name;

};  // tolua_export




