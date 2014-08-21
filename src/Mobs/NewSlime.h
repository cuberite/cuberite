
#pragma once

#include "NewMonster.h"





class cNewSlime :
	public cNewMonster
{
	typedef cNewMonster super;
	
public:
	/** Creates a slime of the specified size; size can be 1, 2 or 4, with 1 is the smallest and 4 is the tallest. */
	cNewSlime(int a_Size);

	CLASS_PROTODEF(cNewSlime)

	// cAggressiveMonster overrides:
	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = NULL) override;

	int GetSize(void) const { return m_Size; }
	
	/** Returns the text describing the slime's size, as used by the client's resource subsystem for sounds.
	Returns either "big" or "small". */
	const AString GetSizeName(int a_Size) const;
	
protected:

	/** Size of the slime, with 1 being the smallest.
	Vanilla uses sizes 1, 2 and 4 only. */
	int m_Size;
} ;




