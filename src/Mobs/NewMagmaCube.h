#pragma once

#include "NewMonster.h"





class cNewMagmaCube :
	public cNewMonster
{
	typedef cNewMonster super;
	
public:
	/// Creates a MagmaCube of the specified size; size is 1 .. 3, with 1 being the smallest
	cNewMagmaCube(int a_Size);

	CLASS_PROTODEF(cNewMagmaCube)

	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = NULL) override;
	int GetSize(void) const { return m_Size; }
	
protected:

	/// Size of the MagmaCube, 1 .. 3, with 1 being the smallest
	int m_Size;
} ;




