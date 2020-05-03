#include "Globals.h"

#include "TNTEntity.h"
#include "../World.h"
#include "../ClientHandle.h"





cTNTEntity::cTNTEntity(Vector3d a_Pos, int a_FuseTicks) :
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
	m_FuseTicks = 0;
	Destroy();
	FLOGD("BOOM at {0}", GetPosition());
	m_World->DoExplosionAt(4.0, GetPosX(), GetPosY(), GetPosZ(), true, esPrimedTNT, this);
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

	m_FuseTicks -= 1;
	if (m_FuseTicks <= 0)
	{
		Explode();
	}
}




