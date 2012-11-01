#pragma once

#include "AggressiveMonster.h"

class cWitch : public cAggressiveMonster
{
public:
	cWitch();
	~cWitch();

	virtual bool IsA( const char* a_EntityType );

	virtual void KilledBy( cEntity* a_Killer );
};
