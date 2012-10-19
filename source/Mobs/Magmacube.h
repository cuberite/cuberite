#pragma once

#include "AggressiveMonster.h"

class cMagmacube : public cAggressiveMonster
{
public:
	cMagmacube();
	~cMagmacube();

	virtual bool IsA( const char* a_EntityType );

	virtual void KilledBy( cEntity* a_Killer );
};
