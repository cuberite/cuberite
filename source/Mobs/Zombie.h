#pragma once

#include "AggressiveMonster.h"

class cZombie : public cAggressiveMonster
{
public:
	cZombie();
	~cZombie();

	virtual bool IsA( const char* a_EntityType );

	virtual void Tick(float a_Dt);
	virtual void KilledBy( cEntity* a_Killer );
};
