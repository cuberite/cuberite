#pragma once

#include "AggressiveMonster.h"





class cMagmaCube :
	public cAggressiveMonster
{
	typedef cAggressiveMonster super;
	
public:
	/// Creates a MagmaCube of the specified size; size is 1 .. 3, with 1 being the smallest
	cMagmaCube(int a_Size);

	CLASS_PROTODEF(cMagmaCube)

	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = nullptr) override;
	int GetSize(void) const { return m_Size; }

	/** Returns the text describing the slime's size, as used by the client's resource subsystem for sounds.
	Returns either "big" or "small". */
	const AString GetSizeName(int a_Size) const;
	
protected:

	/// Size of the MagmaCube, 1, 2 and 4, with 1 being the smallest
	int m_Size;
} ;




