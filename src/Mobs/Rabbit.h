
#pragma once

#include "PassiveMonster.h"





enum class eRabbitType : UInt8
{
	Brown          = 0,
	White          = 1,
	Black          = 2,
	BlackAndWhite  = 3,
	Gold           = 4,
	SaltAndPepper  = 5,
	TheKillerBunny = 99
};





class cRabbit :
	public cPassiveMonster
{
	typedef cPassiveMonster super;
	
public:
	cRabbit();
	cRabbit(eRabbitType Type, int MoreCarrotTicks = 0);

	CLASS_PROTODEF(cRabbit)

	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = nullptr) override;
	virtual const cItem GetFollowedItem(void) const override { return cItem(E_ITEM_CARROT); }

	eRabbitType GetRabbitType()         const { return m_Type; }
	UInt8       GetRabbitTypeAsNumber() const { return static_cast<UInt8>(GetRabbitType()); }
	int         GetMoreCarrotTicks()    const { return m_MoreCarrotTicks; }

private:

	eRabbitType m_Type;
	int m_MoreCarrotTicks;  // Ticks until the Rabbit eat planted Carrots
} ;
