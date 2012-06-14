#pragma once

#include "cAggressiveMonster.h"

class cSkeleton : public cAggressiveMonster
{
public:
	cSkeleton();
	~cSkeleton();

	virtual bool IsA( const char* a_EntityType );

	virtual void Tick(float a_Dt);
	virtual void KilledBy( cEntity* a_Killer );
	
};
