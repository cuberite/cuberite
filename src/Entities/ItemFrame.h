
#pragma once

#include "HangingEntity.h"





// tolua_begin
class cItemFrame :
	public cHangingEntity
{
	// tolua_end

	using Super = cHangingEntity;

public:  // tolua_export

	CLASS_PROTODEF(cItemFrame)

	cItemFrame(eBlockFace a_BlockFace, Vector3d a_Pos);

	// tolua_begin

	/** Returns the item in the frame */
	const cItem & GetItem(void) const { return m_Item; }

	/** Set the item in the frame */
	void SetItem(cItem & a_Item) { m_Item = a_Item; }

	/** Returns the rotation from the item in the frame */
	Byte GetItemRotation(void) const { return m_ItemRotation; }

	/** Set the rotation from the item in the frame */
	void SetItemRotation(Byte a_ItemRotation) { m_ItemRotation = a_ItemRotation; }

	// tolua_end

private:

	virtual void OnRightClicked(cPlayer & a_Player) override;
	virtual void KilledBy(TakeDamageInfo & a_TDI) override;
	virtual void GetDrops(cItems & a_Items, cEntity * a_Killer) override;
	virtual void SpawnOn(cClientHandle & a_ClientHandle) override;

	cItem m_Item;
	Byte m_ItemRotation;

};  // tolua_export




