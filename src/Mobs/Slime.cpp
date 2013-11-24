
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Slime.h"





/// Creates a slime of the specified size; size is 1 .. 3, with 1 being the smallest
cSlime::cSlime(int a_Size) :
	super("Slime", mtSlime, "mob.slime.attack", "mob.slime.attack", 0.6 * a_Size, 0.6 * a_Size),
	m_Size(a_Size)
{
}





void cSlime::GetDrops(cItems & a_Drops, cEntity * a_Killer)
{
	// TODO: only when tiny
	AddRandomDropItem(a_Drops, 0, 2, E_ITEM_SLIMEBALL);
}




