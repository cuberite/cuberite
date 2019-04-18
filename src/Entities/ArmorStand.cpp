
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "ArmorStand.h"
#include "../ClientHandle.h"
#include "../Chunk.h"
#include "../World.h"





cArmorStand::cArmorStand(Vector3d a_Pos)
	: cEntity(etArmorStand, a_Pos.x, a_Pos.y, a_Pos.z, 0.5, 0.9875)
{
	SetMaxHealth(1);
}





void cArmorStand::SpawnOn(cClientHandle & a_ClientHandle)  // Should got any rotation and should replace any of non-solid blocks like snow
{
	a_ClientHandle.SendSpawnObject(*this, 78, 0, static_cast<Byte>(GetYaw()), static_cast<Byte>(GetPitch()));
}





void cArmorStand::Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	UNUSED(a_Dt);
	// No further processing (physics e.t.c.) is needed
}





void cArmorStand::KilledBy(TakeDamageInfo & a_TDI)
{
	super::KilledBy(a_TDI);
	Destroy();
}
