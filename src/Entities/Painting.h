
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

	cPainting(const AString & a_Name, eBlockFace a_Direction, Vector3d a_Pos, UInt32 a_id);

	/** Returns the protocol name of the painting */
	const AString & GetName(void) const { return m_Name; }  // tolua_export
	UInt32 GetPaintingId(void) const { return m_id; }

private:

	virtual void SpawnOn(cClientHandle & a_Client) override;
	virtual void GetDrops(cItems & a_Items, cEntity * a_Killer) override;
	virtual void KilledBy(TakeDamageInfo & a_TDI) override;

	AString m_Name;
	UInt32 m_id;

};  // tolua_export




