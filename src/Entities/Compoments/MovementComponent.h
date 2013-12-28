#pragma once

#include "../Entity.h"

class cMovementComponent
{
protected:
	cEntity * m_Self;
public:
	cMovementComponent(cEntity * a_Entity) : m_Self(a_Entity){}

};