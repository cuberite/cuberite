#pragma once

#include "cPassiveMonster.h"

class cCow : public cPassiveMonster
{
public:
	cCow();
	~cCow();

	virtual bool IsA( const char* a_EntityType );

	virtual void KilledBy( cEntity* a_Killer );
};
