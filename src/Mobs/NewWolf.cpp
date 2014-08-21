
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "NewWolf.h"
#include "../World.h"
#include "../Entities/Player.h"
#include "../Items/ItemHandler.h"





cNewWolf::cNewWolf(void) :
	super("Wolf", mtWolf, "mob.wolf.hurt", "mob.wolf.death", 0.6, 0.8),
	m_IsSitting(false),
	m_IsBegging(false),
	m_IsAngry(false),
	m_CollarColor(14)
{
}




