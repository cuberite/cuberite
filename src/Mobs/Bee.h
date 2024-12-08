
#pragma once

#include "PassiveAggressiveMonster.h"





class cBee:
	public cPassiveAggressiveMonster
{
	using Super = cPassiveAggressiveMonster;

public:

	cBee();

	CLASS_PROTODEF(cBee)
/*
private:
	int m_angertime;
	int m_CannotEnterHiveTicks;
	int m_CropsGrownSincePollination; */


};
