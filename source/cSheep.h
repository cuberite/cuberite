#pragma once

#include "cPassiveMonster.h"

class cSheep : public cPassiveMonster
{
public:
	cSheep();
	~cSheep();

	virtual bool IsA( const char* a_EntityType );

	virtual void KilledBy( cEntity* a_Killer );
};
