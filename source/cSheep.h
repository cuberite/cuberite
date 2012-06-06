#pragma once

#include "cPassiveMonster.h"

class cSheep : public cPassiveMonster
{
public:
	cSheep();
	~cSheep();
	
	bool       m_IsSheared;
	NIBBLETYPE m_WoolColor;  // Uses E_META_WOOL_ constants for colors

	virtual bool IsA(const char * a_EntityType);

	virtual void KilledBy(cEntity * a_Killer);
};
