
#pragma once

#include "Entity.h"





// tolua_begin
class cItemFrame :
	public cEntity
{
	// tolua_end
	typedef cEntity super;

public:

	CLASS_PROTODEF(cItemFrame);

	cItemFrame(int a_BlockFace, double a_X, double a_Y, double a_Z);

	const cItem & GetItem(void) { return m_Item; }
	Byte GetRotation(void) const { return m_Rotation; }

private:

	virtual void SpawnOn(cClientHandle & a_ClientHandle) override;
	virtual void OnRightClicked(cPlayer & a_Player) override;
	virtual void Tick(float a_Dt, cChunk & a_Chunk) override {};
	virtual void KilledBy(cEntity * a_Killer) override;
	virtual void GetDrops(cItems & a_Items, cEntity * a_Killer) override;

	int m_BlockFace;
	cItem m_Item;
	Byte m_Rotation;

};  // tolua_export




