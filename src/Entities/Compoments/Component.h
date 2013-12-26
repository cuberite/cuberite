#include "Entity.h"

class cComponent
{
protected:
	cEntity * m_Self;
public:
	cComponent(cEntity * a_Entity) : m_Self(a_Entity){}
};