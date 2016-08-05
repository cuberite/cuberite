#pragma once

class cTraitBreeder;
class iTraitBreeder;

class cWorld;
class cMonster;
class cPlayer;
class cItems;

// To use this trait, a Monster must implement the following interface.
// The monster must also have a cTraitBreeder as a member variable.

class iTraitBreeder
{
public:
	virtual void InheritFromParents(cMonster * a_Parent1, cMonster * a_Parent2) = 0;

	virtual void GetBreedingItems(cItems & a_Items) const = 0;

	virtual cTraitBreeder & GetTraitBreeder() = 0;

	virtual const cTraitBreeder & GetTraitBreeder() const = 0;
	virtual ~iTraitBreeder();
};





class cTraitBreeder
{

public:
	cTraitBreeder(iTraitBreeder * a_Parent);

	// Functions our host Monster should invoke:
	void Tick();
	bool ActiveTick();
	void OnRightClicked(cPlayer & a_Player);
	void Destroyed();

	/** Returns the partner which the monster is currently mating with. */
	cMonster * GetPartner(void) const { return m_LovePartner; }

	/** Start the mating process. Causes the monster to keep bumping into the partner until m_MatingTimer reaches zero. */
	void EngageLoveMode(cMonster * a_Partner);

	/** Finish the mating process. Called after a baby is born. Resets all breeding related timers and sets m_LoveCooldown to 20 minutes. */
	void ResetLoveMode();

	/** Returns whether the monster has just been fed and is ready to mate. If this is "true" and GetPartner isn't "nullptr", then the monster is mating. */
	bool IsInLove() const;

	/** Returns whether the monster is tired of breeding and is in the cooldown state. */
	bool IsInLoveCooldown() const;

private:
	static iTraitBreeder * ToInterface(cMonster * a_Monster);
	static cTraitBreeder * ToTrait(cMonster * a_Monster);

	/** Our parent */
	iTraitBreeder * m_ParentInterface;
	cMonster * m_Parent;

	/** The monster's breeding partner. */
	cMonster * m_LovePartner;

	/** If above 0, the monster is in love mode, and will breed if a nearby monster is also in love mode. Decrements by 1 per tick till reaching zero. */
	int m_LoveTimer;

	/** If above 0, the monster is in cooldown mode and will refuse to breed. Decrements by 1 per tick till reaching zero. */
	int m_LoveCooldown;

	/** The monster is engaged in mating, once this reaches zero, a baby will be born. Decrements by 1 per tick till reaching zero, then a baby is made and ResetLoveMode() is called. */
	int m_MatingTimer;
};
