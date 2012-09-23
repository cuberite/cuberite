#pragma once

#include "AggressiveMonster.h"

class cSlime : public cAggressiveMonster
{
public:
	cSlime();
	~cSlime();

	virtual bool IsA( const char* a_EntityType );

	virtual void KilledBy( cEntity* a_Killer );
};
