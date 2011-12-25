#pragma once

#include "cPassiveAggressiveMonster.h"

class cZombiepigman : public cPassiveAggressiveMonster
{
public:
	cZombiepigman();
	~cZombiepigman();

	virtual bool IsA( const char* a_EntityType );

	virtual void Tick(float a_Dt);
	virtual void KilledBy( cEntity* a_Killer );
};
