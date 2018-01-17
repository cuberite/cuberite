
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

	/** Called after the baby is born, allows the baby to inherit the parents' properties (color, etc.) */
	virtual void InheritFromParents(cPassiveMonster * a_Parent1, cPassiveMonster * a_Parent2) { }

	/** Returns the partner which the monster is currently mating with. */
	cPassiveMonster * GetPartner(void) const { return m_LovePartner; }

	/** Start the mating process. Causes the monster to keep bumping into the partner until m_MatingTimer reaches zero. */
	void EngageLoveMode(cPassiveMonster * a_Partner);

	/** Finish the mating process. Called after a baby is born. Resets all breeding related timers and sets m_LoveCooldown to 20 minutes. */
	void ResetLoveMode();

	/** Returns whether the monster has just been fed and is ready to mate. If this is "true" and GetPartner isn't "nullptr", then the monster is mating. */
	bool IsInLove() const { return (m_LoveTimer > 0); }

	/** Returns whether the monster is tired of breeding and is in the cooldown state. */
	bool IsInLoveCooldown() const { return (m_LoveCooldown > 0); }

	virtual void Destroyed(void) override;

protected:
	/** The monster's breeding partner. */
	cPassiveMonster * m_LovePartner;

	/** If above 0, the monster is in love mode, and will breed if a nearby monster is also in love mode. Decrements by 1 per tick till reaching zero. */
	int m_LoveTimer;

	/** If above 0, the monster is in cooldown mode and will refuse to breed. Decrements by 1 per tick till reaching zero. */
	int m_LoveCooldown;

	/** The monster is engaged in mating, once this reaches zero, a baby will be born. Decrements by 1 per tick till reaching zero, then a baby is made and ResetLoveMode() is called. */
	int m_MatingTimer;
};




