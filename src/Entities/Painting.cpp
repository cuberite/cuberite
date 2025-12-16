
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Painting.h"
#include "Player.h"
#include "../ClientHandle.h"
#include "../Chunk.h"





cPainting::cPainting(const AString & a_Name, eBlockFace a_Direction, Vector3d a_Pos):
	Super(etPainting, a_Direction, a_Pos),
	m_Name(a_Name)
{
}





void cPainting::SpawnOn(cClientHandle & a_Client)
{
	Super::SpawnOn(a_Client);
	a_Client.SendPaintingSpawn(*this);

	m_World->BroadcastSoundEffect(eSoundEvent::PaintingPlace, GetPosition(), 1, 1);
}





void cPainting::GetDrops(cItems & a_Items, cEntity * a_Killer)
{
	a_Items.emplace_back(E_ITEM_PAINTING);
}





void cPainting::KilledBy(TakeDamageInfo & a_TDI)
{
	Super::KilledBy(a_TDI);

	m_World->BroadcastSoundEffect(eSoundEvent::PaintingBreak, GetPosition(), 1, 1);
}
