
#pragma once

#include "Entity.h"





// tolua_begin
class cPawn :
	public cEntity
{
	// tolua_end
	typedef cEntity super;
	
public:
	CLASS_PROTODEF(cPawn);

	cPawn(eEntityType a_EntityType, double a_Width, double a_Height);

protected:
	bool m_bBurnable;
} ;  // tolua_export




