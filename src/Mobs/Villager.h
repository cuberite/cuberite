
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
	cItemGrid & GetInventory(void) { return m_Inventory; }
	const cItemGrid & GetInventory(void) const { return m_Inventory; }

	/** Returns true if the given blocktype are: crops, potatoes or carrots and they have full grown up. */
	bool IsBlockFarmable(BlockState a_Block);

	/** Returns true if the block at the given location is a fully grown up crop. */
	bool IsHarvestable(Vector3i a_CropsPos);

	/** Returns true if seeds can be planted at a given location. */
	bool IsPlantable(Vector3i a_CropsPos);

	// Farmer functions
	enum eFarmerAction
	{
		faIdling,
		faPlanting,
		faHarvesting,
	} ;

	static const int FARMER_RANDOM_TICK_SPEED = 5;
	/** With 10% chance, it takes about 20 seconds to find a spot. */
	static constexpr double FARMER_SPECIAL_ACTION_CHANCE = 0.1;
	/** This distance from the Villager makes for a 31x3x31 area. */
	static constexpr Vector3i FARMER_SCAN_CROPS_DIST {15, 1, 15};

	/** Tick function for farmers. */
	void TickFarmer();

	/** Searches in a 31x3x31 area to harvest crops or spaces to plant crops. If it found some it will navigate to them. */
	void ScanAreaForWork();

	/** Looks if the farmer has reached it's destination, and if it's still crops and the destination is closer then 1 block it will harvest them. */
	void HandleFarmerTryHarvestCrops();

	/** Looks if the farmer has reached it's destination, and if it's still non obstructed farmland and the destination is closer then 1 block it will plant crops. */
	void HandleFarmerTryPlaceCrops();

	/** Checking for harvesting or planting nearby crops */
	void CheckForNearbyCrops();

	/** Returns whether the farmer has crops in his inventory to plant. */
	bool CanPlantCrops();

	/** Returns whether the farmer is not working. */
	bool IsIdling()
	{
		return m_FarmerAction == faIdling;
	}

	// Get and set functions.
	int           GetVilType(void)      const { return m_Type; }
	eFarmerAction GetFarmerAction(void) const { return m_FarmerAction; }

private:

	int m_ActionCountDown;
	int m_Type;
	eFarmerAction m_FarmerAction;
	Vector3i m_CropsPos;
	cItemGrid m_Inventory;

} ;




