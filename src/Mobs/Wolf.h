
#pragma once

#include "Monster.h"
#include "../Entities/Entity.h"





class cWolf :
	public cMonster
{
	typedef cMonster super;
	
public:
	cWolf(void);

	CLASS_PROTODEF(cWolf)

	// Get functions
	bool    IsSitting     (void) const { return m_IsSitting; }
	bool    IsBegging     (void) const { return m_IsBegging; }
	bool    IsAngry       (void) const { return m_IsAngry; }
	int     GetCollarColor(void) const { return m_CollarColor; }

	// Set functions
	void SetIsSitting  (bool a_IsSitting)   { m_IsSitting = a_IsSitting; }
	void SetIsBegging  (bool a_IsBegging)   { m_IsBegging = a_IsBegging; }
	void SetIsAngry    (bool a_IsAngry)     { m_IsAngry = a_IsAngry; }
	void SetCollarColor(int a_CollarColor)  { m_CollarColor = a_CollarColor; }

protected:

	bool    m_IsSitting;
	bool    m_IsBegging;
	bool    m_IsAngry;
	int     m_CollarColor;
} ;




