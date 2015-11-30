
#pragma once

#include "Monster.h"





class cPassiveMonster :
	public cMonster
{
	typedef cMonster super;
	
public:
	cPassiveMonster(const AString & a_ConfigName, eMonsterType a_MobType, const AString & a_SoundHurt, const AString & a_SoundDeath, double a_Width, double a_Height);

	virtual void Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk) override;
	virtual void OnRightClicked(cPlayer & a_Player) override;

	/** When hit by someone, run away */
	virtual bool DoTakeDamage(TakeDamageInfo & a_TDI) override;

	/** Returns the items that the animal of this class follows when a player holds it in hand. */
	virtual void GetFollowedItems(cItems & a_Items) { }

	/** Returns the items that make the animal breed - this is usually the same as the ones that make the animal follow, but not necessarily. */
	virtual void GetBreedingItems(cItems & a_Items) { GetFollowedItems(a_Items); }

	cPassiveMonster * GetPartner(void) const { return m_LovePartner; }
	void EngageLoveMode(cPassiveMonster * a_Partner);
	void ResetLoveMode();

	bool IsInLove() const { return (m_LoveTimer > 0); }
	bool IsInLoveCooldown() const { return (m_LoveCooldown > 0); }

protected:
	cPassiveMonster * m_LovePartner;
	int m_LoveTimer;
	int m_LoveCooldown;
	int m_MatingTimer;
};




