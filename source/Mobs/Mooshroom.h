#pragma once

#include "PassiveMonster.h"

class cMooshroom : public cPassiveMonster
{
public:
	cMooshroom();
	~cMooshroom();

	virtual bool IsA( const char* a_EntityType );

	virtual void KilledBy( cEntity* a_Killer );
};
