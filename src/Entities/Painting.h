
#pragma once

#include "HangingEntity.h"





// tolua_begin
class cPainting :
	public cHangingEntity
{
	typedef cHangingEntity super;

public:

	// tolua_end

	CLASS_PROTODEF(cPainting)

	cPainting(const AString & a_Name, eBlockFace a_Direction, double a_X, double a_Y, double a_Z);

	/** Returns the protocol name of the painting */
	const AString & GetName(void) const { return m_Name; }  // tolua_export

private:

	virtual void SpawnOn(cClientHandle & a_Client) override;
	virtual void GetDrops(cItems & a_Items, cEntity * a_Killer) override;
	virtual void KilledBy(TakeDamageInfo & a_TDI) override
	{
		super::KilledBy(a_TDI);
		Destroy();
	}

	AString m_Name;

};  // tolua_export




