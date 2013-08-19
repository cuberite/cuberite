
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Pawn.h"





cPawn::cPawn(eEntityType a_EntityType, double a_Width, double a_Height)
	: cEntity(a_EntityType, 0, 0, 0, a_Width, a_Height)
	, m_bBurnable(true)
{
}





