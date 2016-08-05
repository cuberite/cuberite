
#pragma once

#include "Monster.h"
#include "Traits/TraitBreeder.h"
#include "Traits/TraitItemFollower.h"
#include "Traits/TraitCoward.h"



class cPassiveMonster : public cMonster, public iTraitBreeder, public iTraitItemFollower
{
	typedef cMonster super;

public:
	cPassiveMonster(const AString & a_ConfigName, eMonsterType a_MobType, const AString & a_SoundHurt, const AString & a_SoundDeath, double a_Width, double a_Height);

	virtual void Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk) override;
	virtual void OnRightClicked(cPlayer & a_Player) override;

	/** When hit by someone, run away */
	virtual bool DoTakeDamage(TakeDamageInfo & a_TDI) override;

	virtual void Destroyed(void) override;

	// Implementing the iTraitBreeder interface
	/** Returns the items that make the animal breed - this is usually the same as the ones that make the animal follow, but not necessarily. */
	virtual void GetBreedingItems(cItems & a_Items) const override { GetFollowedItems(a_Items); }

	virtual void InheritFromParents(cMonster * a_Parent1, cMonster * a_Parent2) override { }

	cTraitBreeder & GetTraitBreeder() override;

	const cTraitBreeder & GetTraitBreeder() const override;

	// Implementing the iTraitItemFollower interface

	/** Returns the items that the animal of this class follows when a player holds it in hand. */
	virtual void GetFollowedItems(cItems & a_Items) const override { }

private:
	cTraitBreeder m_TraitBreeder;
	cTraitItemFollower m_TraitItemFollower;
	cTraitCoward m_TraitCoward;
};
