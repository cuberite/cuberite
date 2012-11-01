#pragma once

#include "PassiveMonster.h"

class cBat : public cPassiveMonster
{
public:
	cBat();
	~cBat();

	virtual bool IsA( const char* a_EntityType );

	virtual void KilledBy( cEntity* a_Killer );
};
