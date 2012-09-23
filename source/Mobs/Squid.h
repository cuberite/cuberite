#pragma once

#include "PassiveMonster.h"

class cSquid : public cPassiveMonster
{
public:
	cSquid();
	~cSquid();

	virtual void Tick(float a_Dt);

	virtual bool IsA( const char* a_EntityType );
	virtual void KilledBy( cEntity* a_Killer );
	
protected:
	float m_NoWater;
};
