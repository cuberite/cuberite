#pragma once

#include "cPassiveMonster.h"

class cPig : public cPassiveMonster
{
public:
	cPig();
	~cPig();

	virtual bool IsA( const char* a_EntityType );

	virtual void KilledBy( cEntity* a_Killer );
};
