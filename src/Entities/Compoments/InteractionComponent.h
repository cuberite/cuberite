#pragma once

#include "../Entity.h"

class cInteractionComponent
{
protected:
	cEntity * m_Self;
public:
	cInteractionComponent(cEntity * a_Entity) : m_Self(a_Entity){}

	virtual void OnCollisionWithEntity(cEntity * a_Entity){}
	
	virtual void OnTakeDamage(TakeDamageInfo & a_TDI){}
	virtual void OnRightClicked(){}
	virtual void OnKilled(cEntity * a_Killer = NULL){}
	// virtual void OnPickup(){}
	// virtual void OnDestroy(){}

};
