
#pragma once

#include "AggressiveMonster.h"





// tolua_begin
class cMagmaCube :
	public cAggressiveMonster
{
	typedef cAggressiveMonster super;
	
public:
	// tolua_end

	/// Creates a MagmaCube of the specified size; size is 1 .. 3, with 1 being the smallest
	cMagmaCube(int a_Size);

	CLASS_PROTODEF(cMagmaCube);

	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = NULL) override;
	
	// tolua_begin
	int GetSize(void) const { return m_Size; }
	// tolua_end
	
protected:

	/// Size of the MagmaCube, 1 .. 3, with 1 being the smallest
	int m_Size;
} ; // tolua_export



