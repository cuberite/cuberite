#pragma once

#include "cAggressiveMonster.h"

class cSilverfish : public cAggressiveMonster
{
public:
	cSilverfish();
	~cSilverfish();

	virtual bool IsA( const char* a_EntityType );
};
