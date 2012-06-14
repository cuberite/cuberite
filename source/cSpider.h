#pragma once

#include "cAggressiveMonster.h"

class cSpider : public cAggressiveMonster
{
public:
	cSpider();
	~cSpider();

	virtual bool IsA( const char* a_EntityType );

	virtual void KilledBy( cEntity* a_Killer );
};
