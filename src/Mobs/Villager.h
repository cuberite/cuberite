
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

	CLASS_PROTODEF(cVillager);

	// Override functions
	virtual void DoTakeDamage(TakeDamageInfo & a_TDI) override;
	virtual void Tick        (float a_Dt, cChunk & a_Chunk) override;

	// cVillager functions
	void HandleFarmer();
	bool IsBlockFarmable(BLOCKTYPE a_BlockType);

	// Get and set functions.
	int GetVilType(void) const { return m_Type; }	
	Vector3i GetCropsPos(void) const { return m_CropsPos; }
	bool DidFindCrops(void) const { return m_DidFindCrops; }

private:

	int m_ActionCountDown;
	int m_Type;
	bool m_DidFindCrops;
	Vector3i m_CropsPos;

} ;




