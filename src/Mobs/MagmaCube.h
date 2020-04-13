#pragma once

#include "AggressiveMonster.h"





class cMagmaCube:
	public cAggressiveMonster
{
	using Super = cAggressiveMonster;

public:

	/** Creates a MagmaCube of the specified size; with 1 being the smallest */
	cMagmaCube(int a_Size);

	CLASS_PROTODEF(cMagmaCube)

	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = nullptr) override;
	int GetSize(void) const { return m_Size; }

	/** Returns the text describing the slime's size, as used by the client's resource subsystem for sounds.
	Returns either "big" or "small". */
	static AString GetSizeName(int a_Size);

protected:

	/** Size of the MagmaCube, with 1 being the smallest */
	int m_Size;
} ;




