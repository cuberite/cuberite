
#pragma once

#include "Monster.h"





class cPassiveMonster :
	public cMonster
{
	typedef cMonster super;
	
public:
	cPassiveMonster(const AString & a_ConfigName, char a_ProtocolMobType, const AString & a_SoundHurt, const AString & a_SoundDeath, double a_Width, double a_Height);

	virtual void Tick(float a_Dt, cChunk & a_Chunk) override;

	/// When hit by someone, run away
	virtual void DoTakeDamage(TakeDamageInfo & a_TDI) override;

} ;




