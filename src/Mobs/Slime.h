
#pragma once

#include "AggressiveMonster.h"





// tolua_begin
class cSlime :
	public cAggressiveMonster
{
	typedef cAggressiveMonster super;
	
public:
	// tolua_end

	/// Creates a slime of the specified size; size is 1 .. 3, with 1 being the smallest
	cSlime(int a_Size);

	CLASS_PROTODEF(cSlime);

	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = NULL) override;
	
	// tolua_begin
	int GetSize(void) const { return m_Size; }
	// tolua_end
	
protected:

	/// Size of the slime, 1 .. 3, with 1 being the smallest
	int m_Size;
} ; // tolua_export




