#pragma once

//fwds
class cMonster;
class cItems;
class cEntity;
struct TakeDamageInfo;

class cTraitCoward
{
public:
	cTraitCoward(cMonster * a_Parent);

	// Functions our host Monster should invoke:
	bool ActiveTick();
	void DoTakeDamage(TakeDamageInfo & a_TDI);


private:
	cMonster * m_Parent;  // Our Parent
	cEntity * m_Attacker;  // The entity we're running away from
};
