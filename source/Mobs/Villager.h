#pragma once

#include "PassiveMonster.h"

class cVillager : public cPassiveMonster
{
public:
	cVillager();
	~cVillager();

	virtual bool IsA( const char* a_EntityType );

	virtual void KilledBy( cEntity* a_Killer );
};
