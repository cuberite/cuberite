#pragma once

#include "PassiveMonster.h"

class cOcelot : public cPassiveMonster
{
public:
	cOcelot();
	~cOcelot();

	virtual bool IsA( const char* a_EntityType );

	virtual void KilledBy( cEntity* a_Killer );
};
