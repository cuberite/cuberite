
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "EnderCrystal.h"
#include "ClientHandle.h"
#include "../Chunk.h"
#include "../World.h"





cEnderCrystal::cEnderCrystal(double a_X, double a_Y, double a_Z)
	: cEntity(etEnderCrystal, a_X, a_Y, a_Z, 1.0, 1.0)
{
	SetMaxHealth(5);
}





void cEnderCrystal::SpawnOn(cClientHandle & a_ClientHandle)
{
	a_ClientHandle.SendSpawnObject(*this, 51, 0, static_cast<Byte>(GetYaw()), static_cast<Byte>(GetPitch()));
}





void cEnderCrystal::Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	UNUSED(a_Dt);
	// No further processing (physics e.t.c.) is needed
}





void cEnderCrystal::KilledBy(TakeDamageInfo & a_TDI)
{
	super::KilledBy(a_TDI);

	m_World->DoExplosionAt(6.0, GetPosX(), GetPosY(), GetPosZ(), true, esEnderCrystal, this);

	Destroy();

	m_World->SetBlock(POSX_TOINT, POSY_TOINT,     POSZ_TOINT, E_BLOCK_BEDROCK, 0);
	m_World->SetBlock(POSX_TOINT, POSY_TOINT + 1, POSZ_TOINT, E_BLOCK_FIRE,    0);
}




