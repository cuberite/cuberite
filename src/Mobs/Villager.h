
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

	virtual void DoTakeDamage(TakeDamageInfo & a_TDI) override;
	virtual void Tick        (float a_Dt, cChunk & a_Chunk) override;

	void HandleFarmer();
	bool IsBlockFarmable(BLOCKTYPE a_BlockType);
	int GetVilType(void) const { return m_Type; }	

private:

	int m_Type;
	bool m_DidFindCrops;
	Vector3i m_CropsPos;

} ;




