#pragma once

#include "AggressiveMonster.h"

class cCavespider : public cAggressiveMonster
{
public:
	cCavespider();
	~cCavespider();

	virtual bool IsA( const char* a_EntityType );

	virtual void Tick(float a_Dt);
	virtual void KilledBy( cEntity* a_Killer );
};
