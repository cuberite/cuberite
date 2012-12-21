
#pragma once

#include "AggressiveMonster.h"





class cSkeleton :
	public cAggressiveMonster
{
	typedef cAggressiveMonster super;
	
public:
	cSkeleton();

	virtual bool IsA(const char * a_EntityType) override;

	virtual void Tick(float a_Dt) override;
	virtual void GetDrops(cItems & a_Drops, cPawn * a_Killer = NULL) override;
} ;




