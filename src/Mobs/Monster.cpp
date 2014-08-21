#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules
#include "Monster.h"
#include "../ClientHandle.h"
#include "../MersenneTwister.h"

cMonster::cMonster(const AString & a_ConfigName, eType a_MobType, const AString & a_SoundHurt, const AString & a_SoundDeath, double a_Width, double a_Height)
	: super(etMonster, a_Width, a_Height)
	, m_MobType(a_MobType)
	, m_SoundHurt(a_SoundHurt)
	, m_SoundDeath(a_SoundDeath)
{
	m_AI = new cAIComponent(this);
	m_Attack = new cAttackComponent(this);
	m_Environment = new cEnvironmentComponent(this);
	m_Movement = new cMovementComponent(this);

	// Temporary placement till I figure out where to put it
	m_DropChanceWeapon = 0.0f;
	m_DropChanceHelmet = 0.0f;
	m_DropChanceChestplate = 0.0f;
	m_DropChanceLeggings = 0.0f;
	m_DropChanceBoots = 0.0f;
}





void cMonster::SpawnOn(cClientHandle & a_Client)
{
	a_Client.SendSpawnMob(*this);
}





void cMonster::AddRandomDropItem(cItems & a_Drops, unsigned int a_Min, unsigned int a_Max, short a_Item, short a_ItemHealth)
{
	MTRand r1;
	int Count = r1.randInt() % (a_Max + 1 - a_Min) + a_Min;
	if (Count > 0)
	{
		a_Drops.push_back(cItem(a_Item, Count, a_ItemHealth));
	}
}





void cMonster::AddRandomUncommonDropItem(cItems & a_Drops, float a_Chance, short a_Item, short a_ItemHealth)
{
	MTRand r1;
	int Count = r1.randInt() % 1000;
	if (Count < (a_Chance * 10))
	{
		a_Drops.push_back(cItem(a_Item, 1, a_ItemHealth));
	}
}





void cMonster::AddRandomRareDropItem(cItems & a_Drops, cItems & a_Items, short a_LootingLevel)
{
	MTRand r1;
	int Count = r1.randInt() % 200;
	if (Count < (5 + a_LootingLevel))
	{
		int Rare = r1.randInt() % a_Items.Size();
		a_Drops.push_back(a_Items.at(Rare));
	}
}





void cMonster::AddRandomArmorDropItem(cItems & a_Drops, short a_LootingLevel)
{
	MTRand r1;
	if (r1.randInt() % 200 < ((m_DropChanceHelmet * 200) + (a_LootingLevel * 2)))
	{
		if (!GetEquippedHelmet().IsEmpty()) a_Drops.push_back(GetEquippedHelmet());
	}
	
	if (r1.randInt() % 200 < ((m_DropChanceChestplate * 200) + (a_LootingLevel * 2)))
	{
		if (!GetEquippedChestplate().IsEmpty()) a_Drops.push_back(GetEquippedChestplate());
	}
	
	if (r1.randInt() % 200 < ((m_DropChanceLeggings * 200) + (a_LootingLevel * 2)))
	{
		if (!GetEquippedLeggings().IsEmpty()) a_Drops.push_back(GetEquippedLeggings());
	}
	
	if (r1.randInt() % 200 < ((m_DropChanceBoots * 200) + (a_LootingLevel * 2)))
	{
		if (!GetEquippedBoots().IsEmpty()) a_Drops.push_back(GetEquippedBoots());
	}
}





void cMonster::AddRandomWeaponDropItem(cItems & a_Drops, short a_LootingLevel)
{
	MTRand r1;
	if (r1.randInt() % 200 < ((m_DropChanceWeapon * 200) + (a_LootingLevel * 2)))
	{
		if (!GetEquippedWeapon().IsEmpty()) a_Drops.push_back(GetEquippedWeapon());
	}
}
