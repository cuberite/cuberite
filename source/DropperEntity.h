
// DropperEntity.h

// Declares the cDropperEntity class representing a dropper block entity





#pragma once

#include "DropSpenserEntity.h"





// tolua_begin
class cDropperEntity :
	public cDropSpenserEntity
{
	typedef cDropSpenserEntity super;
	
public:

	/// Constructor used while generating a chunk; sets m_World to NULL
	cDropperEntity(int a_BlockX, int a_BlockY, int a_BlockZ);
	
	// tolua_end
	
	/// Constructor used for normal operation
	cDropperEntity(int a_BlockX, int a_BlockY, int a_BlockZ, cWorld * a_World);

	static const char * GetClassStatic(void) { return "cDropperEntity"; }

protected:
	// cDropSpenserEntity overrides:
	virtual void DropSpenseFromSlot(int a_SlotNum) override;
} ;  // tolua_export




