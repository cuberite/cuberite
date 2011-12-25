#pragma once

#include "cPassiveAggressiveMonster.h"

class cWolf : public cPassiveAggressiveMonster
{
public:
	cWolf();
	~cWolf();

	virtual bool IsA( const char* a_EntityType );
};
