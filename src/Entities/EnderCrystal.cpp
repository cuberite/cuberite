
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "EnderCrystal.h"
#include "ClientHandle.h"
#include "Player.h"
#include "../Chunk.h"





cEnderCrystal::cEnderCrystal(double a_X, double a_Y, double a_Z)
	: cEntity(etEnderCrystal, a_X, a_Y, a_Z, 1.0, 1.0)
{
	SetMaxHealth(5);
}





void cEnderCrystal::SpawnOn(cClientHandle & a_ClientHandle)
{
	a_ClientHandle.SendSpawnObject(*this, 51, 0, (Byte)GetYaw(), (Byte)GetPitch());
}





void cEnderCrystal::Tick(float a_Dt, cChunk & a_Chunk)
{
	UNUSED(a_Dt);

	a_Chunk.SetBlock(POSX_TOINT, POSY_TOINT, POSZ_TOINT, E_BLOCK_FIRE, 0);

	// No further processing (physics e.t.c.) is needed
}





void cEnderCrystal::KilledBy(TakeDamageInfo & a_TDI)
{
	super::KilledBy(a_TDI);

	m_World->DoExplosionAt(6.0, GetPosX(), GetPosY(), GetPosZ(), true, esEnderCrystal, this);

	Destroy();
}




