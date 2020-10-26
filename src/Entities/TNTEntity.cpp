#include "Globals.h"

#include "TNTEntity.h"
#include "../World.h"
#include "../ClientHandle.h"





cTNTEntity::cTNTEntity(Vector3d a_Pos, unsigned a_FuseTicks) :
	Super(etTNT, a_Pos, 0.98, 0.98),
	m_FuseTicks(a_FuseTicks)
{
	SetGravity(-16.0f);
	SetAirDrag(0.02f);
}





void cTNTEntity::SpawnOn(cClientHandle & a_ClientHandle)
{
	a_ClientHandle.SendSpawnEntity(*this);
	m_bDirtyOrientation = false;  // TODO: why?
	m_bDirtyHead = false;
}





void cTNTEntity::Explode(void)
{
	FLOGD("BOOM at {0}", GetPosition());

	// Destroy first so the Explodinator doesn't find us (when iterating through entities):
	Destroy();

	// TODO: provided centred coordinates to all calls to DoExplosionAt, from entities and blocks
	// This is to ensure maximum efficiency of explosions
	m_World->DoExplosionAt(4.0, GetPosX(), GetPosY() + GetHeight() / 2, GetPosZ(), true, esPrimedTNT, this);
}





void cTNTEntity::Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	Super::Tick(a_Dt, a_Chunk);
	if (!IsTicking())
	{
		// The base class tick destroyed us
		return;
	}

	BroadcastMovementUpdate();

	if (m_FuseTicks > 0)
	{
		--m_FuseTicks;
	}

	if (m_FuseTicks == 0)
	{
		Explode();
	}
}




