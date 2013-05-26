
#pragma once

#include "DropSpenserEntity.h"





// tolua_begin
class cDispenserEntity :
	public cDropSpenserEntity
{
	typedef cDropSpenserEntity super;
	
public:

	/// Constructor used while generating a chunk; sets m_World to NULL
	cDispenserEntity(int a_BlockX, int a_BlockY, int a_BlockZ);
	
	// tolua_end
	
	/// Constructor used for normal operation
	cDispenserEntity(int a_BlockX, int a_BlockY, int a_BlockZ, cWorld * a_World);

	static const char * GetClassStatic(void) { return "cDispenserEntity"; }

private:
	// cDropSpenser overrides:
	virtual void DropSpenseFromSlot(int a_SlotNum) override;
} ;  // tolua_export




