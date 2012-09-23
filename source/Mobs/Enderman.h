#pragma once

#include "PassiveAggressiveMonster.h"

class cEnderman : public cPassiveAggressiveMonster
{
public:
	cEnderman();
	~cEnderman();

	virtual bool IsA( const char* a_EntityType );

	virtual void Tick(float a_Dt);
	virtual void KilledBy( cEntity* a_Killer );
};
