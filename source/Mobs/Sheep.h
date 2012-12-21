
#pragma once

#include "PassiveMonster.h"





class cSheep :
	public cPassiveMonster
{
	typedef cPassiveMonster super;
	
public:
	cSheep(void);
	
	bool       m_IsSheared;
	NIBBLETYPE m_WoolColor;  // Uses E_META_WOOL_ constants for colors

	virtual bool IsA(const char * a_EntityType) override;

	virtual void GetDrops(cItems & a_Drops, cPawn * a_Killer = NULL) override;
} ;




