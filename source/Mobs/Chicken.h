#pragma once

#include "PassiveMonster.h"

class cChicken : public cPassiveMonster
{
public:
	cChicken();
	~cChicken();

	virtual bool IsA( const char* a_EntityType );

	virtual void KilledBy( cEntity* a_Killer );
};
