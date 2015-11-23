
#pragma once

#include "Monster.h"





class cPassiveMonster :
	public cMonster
{
	typedef cMonster super;
	
public:
	cPassiveMonster(const AString & a_ConfigName, eMonsterType a_MobType, const AString & a_SoundHurt, const AString & a_SoundDeath, double a_Width, double a_Height);

	virtual void Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk) override;

	/** When hit by someone, run away */
	virtual bool DoTakeDamage(TakeDamageInfo & a_TDI) override;

	/** Returns the items that the animal of this class follows when a player holds it in hand. */
	virtual void GetFollowedItems(cItems & a_Items) { }

} ;




