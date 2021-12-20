
#pragma once

#include "PassiveMonster.h"
#include "../Blocks/ChunkInterface.h"
#include "../Inventory.h"




class cVillager:
	public cPassiveMonster
{
	using Super = cPassiveMonster;

public:

	enum eVillagerType
	{
		vtFarmer     = 0,
		vtLibrarian  = 1,
		vtPriest     = 2,
		vtBlacksmith = 3,
		vtButcher    = 4,
		vtGeneric    = 5,
		vtMax
	} ;

	cVillager(eVillagerType VillagerType);

	CLASS_PROTODEF(cVillager)

	/** Returns a random Profession. */
	static eVillagerType GetRandomProfession();

	// cEntity overrides
	virtual bool DoTakeDamage(TakeDamageInfo & a_TDI) override;
	virtual void Tick        (std::chrono::milliseconds a_Dt, cChunk & a_Chunk) override;
	virtual void KilledBy    (TakeDamageInfo & a_TDI) override;

	// cVillager functions
	/** Returns the villager hidden inventory (8 slots). */
	cItemGrid & GetInventory(void) { return m_Inventory; };

	/** return true if the given blocktype are: crops, potatoes or carrots. */
	bool IsBlockFarmable(BLOCKTYPE a_BlockType);

	// Farmer functions
	/** Searches in a 11x7x11 area for crops. If it found some it will navigate to them. */
	void HandleFarmerPrepareFarmCrops();

	/** Looks if the farmer has reached it's destination, and if it's still crops and the destination is closer then 2 blocks it will harvest them.
	Returns true if the villager must keep walking to the crop. */
	bool HandleFarmerTryHarvestCrops();

	/** Replaces the crops he harvested. */
	void HandleFarmerPlaceCrops();

	/** Harvesting nearby crops */
	void CheckForNearbyCrops();

	/** Returns whether the farmer has crops in his inventory to plant. */
	bool CanPlantCrops();

	// Get and set functions.
	int      GetVilType(void)              const { return m_Type; }
	Vector3i GetCropsPos(void)             const { return m_CropsPos; }
	bool     DoesHaveActionActivated(void) const { return m_VillagerAction; }

private:

	int m_ActionCountDown;
	int m_Type;
	bool m_VillagerAction;
	bool m_Harvesting;
	Vector3i m_CropsPos;
	cItemGrid m_Inventory;

} ;




