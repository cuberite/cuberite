
#pragma once

#include "AggressiveMonster.h"





class cSlime:
	public cAggressiveMonster
{
	using Super = cAggressiveMonster;

public:

	/** Creates a slime of the specified size; size can be 1, 2 or 4, with 1 is the smallest and 4 is the tallest. */
	cSlime(int a_Size);

	CLASS_PROTODEF(cSlime)

	// cAggressiveMonster overrides:
	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = nullptr) override;
	virtual bool Attack(std::chrono::milliseconds a_Dt) override;
	virtual void KilledBy(TakeDamageInfo & a_TDI) override;

	int GetSize(void) const { return m_Size; }

	/** Returns the text describing the slime's size, as used by the client's resource subsystem for sounds.
	Returns either "big" or "small". */
	static AString GetSizeName(int a_Size);

protected:

	/** Size of the slime, with 1 being the smallest.
	Vanilla uses sizes 1, 2 and 4 only. */
	int m_Size;
} ;




