#pragma once

#include "AggressiveMonster.h"

class cBlaze : public cAggressiveMonster
{
public:
	cBlaze();
	~cBlaze();

	virtual bool IsA( const char* a_EntityType );

	virtual void KilledBy( cEntity* a_Killer );
};
