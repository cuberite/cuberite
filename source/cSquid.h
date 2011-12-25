#pragma once

#include "cPassiveMonster.h"

class cSquid : public cPassiveMonster
{
public:
	cSquid();
	~cSquid();

	virtual bool IsA( const char* a_EntityType );
	virtual void KilledBy( cEntity* a_Killer );
};
