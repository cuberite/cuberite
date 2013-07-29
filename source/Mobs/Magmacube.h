
#pragma once

#include "AggressiveMonster.h"





class cMagmacube :
	public cAggressiveMonster
{
	typedef cAggressiveMonster super;
	
public:
	/// Creates a magmacube of the specified size; size is 1 .. 3, with 1 being the smallest
	cMagmacube(int a_Size);

	CLASS_PROTODEF(cMagmacube);

	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = NULL) override;
	
protected:

	/// Size of the magmacube, 1 .. 3, with 1 being the smallest
	int m_Size;
} ;




