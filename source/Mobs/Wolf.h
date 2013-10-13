
#pragma once

#include "PassiveAggressiveMonster.h"





class cWolf :
	public cPassiveAggressiveMonster
{
	typedef cPassiveAggressiveMonster super;
	
public:
	cWolf(void);

	CLASS_PROTODEF(cWolf);

	virtual void DoTakeDamage(TakeDamageInfo & a_TDI) override;
	virtual void OnRightClicked(cPlayer & a_Player) override;

	bool IsSitting(void) const { return m_bIsSitting; }
	bool IsTame(void) const { return m_bIsTame; }
	bool IsBegging(void) const { return m_bIsBegging; }
	bool IsAngry(void) const { return m_bIsAngry; }

private:

	bool m_bIsSitting;
	bool m_bIsTame;
	bool m_bIsBegging;
	bool m_bIsAngry;

} ;




