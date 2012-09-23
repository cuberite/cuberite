#pragma once

#include "AggressiveMonster.h"

class cGhast : public cAggressiveMonster
{
public:
	cGhast();
	~cGhast();

	virtual bool IsA( const char* a_EntityType );

	virtual void KilledBy( cEntity* a_Killer );
};
