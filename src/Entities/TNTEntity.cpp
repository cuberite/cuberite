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

	// TODO centre everything else too
	m_World->DoExplosionAt(4.0, GetPosX(), GetPosY() + GetHeight() / 2, GetPosZ(), true, esPrimedTNT, this);
	Destroy();
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

	m_FuseTicks -= std::chrono::duration_cast<cTickTime>(a_Dt);
	if (m_FuseTicks.count() <= 0)
	{
		Explode();
	}
}




