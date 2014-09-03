
#pragma once

#include "PassiveMonster.h"





class cVillager :
	public cPassiveMonster
{
	typedef cPassiveMonster super;
	
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

	// cEntity overrides
	virtual bool DoTakeDamage(TakeDamageInfo & a_TDI) override;
	virtual void Tick        (float a_Dt, cChunk & a_Chunk) override;

	// cVillager functions
	/** return true if the given blocktype are: crops, potatoes or carrots.*/
	bool IsBlockFarmable(BLOCKTYPE a_BlockType);

	// Farmer functions
	/** Searches in a 11x7x11 area for crops. If it found some it will navigate to them.*/
	void HandleFarmerPrepareFarmCrops();

	/** Looks if the farmer has reached it's destination, and if it's still crops and the destination is closer then 2 blocks it will harvest them.*/
	void HandleFarmerTryHarvestCrops();

	/** Replaces the crops he harvested.*/
	void HandleFarmerPlaceCrops();

	// Get and set functions.
	int      GetVilType(void)              const { return m_Type; }
	Vector3i GetCropsPos(void)             const { return m_CropsPos; }
	bool     DoesHaveActionActivated(void) const { return m_VillagerAction; }

private:

	int m_ActionCountDown;
	int m_Type;
	bool m_VillagerAction;
	Vector3i m_CropsPos;

} ;




