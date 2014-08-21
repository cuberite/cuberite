
#pragma once

#include "NewMonster.h"





class cNewHorse :
	public cNewMonster
{
	typedef cNewMonster super;
	
public:
	cNewHorse(int Type, int Color, int Style, int TameTimes);

	CLASS_PROTODEF(cNewHorse)
	
	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = NULL) override;
	virtual void Tick(float a_Dt, cChunk & a_Chunk) override;

	bool IsSaddled      (void) const  {return m_bIsSaddled; }
	bool IsChested      (void) const  {return m_bHasChest; }
	bool IsEating       (void) const  {return m_bIsEating; }
	bool IsRearing      (void) const  {return m_bIsRearing; }
	bool IsMthOpen      (void) const  {return m_bIsMouthOpen; }
	bool IsTame         (void) const  {return m_bIsTame; }
	int  GetHorseType   (void) const  {return m_Type; }
	int  GetHorseColor  (void) const  {return m_Color; }
	int  GetHorseStyle  (void) const  {return m_Style; }
	int  GetHorseArmour (void) const  {return m_Armour;}

private:

	bool m_bHasChest, m_bIsEating, m_bIsRearing, m_bIsMouthOpen, m_bIsTame, m_bIsSaddled;
	int m_Type, m_Color, m_Style, m_Armour, m_TimesToTame, m_TameAttemptTimes, m_RearTickCount;

} ;




