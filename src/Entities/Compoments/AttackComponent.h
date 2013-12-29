#pragma once

#include "../Entity.h"

class cAttackComponent
{
protected:
	cEntity * m_Self;
public:
	cAttackComponent(cEntity * a_Entity) : m_Self(a_Entity){}
	
	virtual void OnAttackEntity(cEntity * a_Entity){}

};
