#pragma once

#include "AggressiveMonster.h"

class cCreeper : public cAggressiveMonster
{
public:
	cCreeper();
	~cCreeper();

	virtual bool IsA( const char* a_EntityType );

	virtual void KilledBy( cEntity* a_Killer );
};
