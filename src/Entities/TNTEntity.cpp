#include "Globals.h"

#include "TNTEntity.h"
#include "../World.h"
#include "../ClientHandle.h"





cTNTEntity::cTNTEntity(Vector3d a_Pos, int a_FuseTicks) :
	super(etTNT, a_Pos.x, a_Pos.y, a_Pos.z, 0.98, 0.98),
	m_FuseTicks(a_FuseTicks)
{
	SetGravity(-16.0f);
	SetAirDrag(0.4f);
}




void cTNTEntity::SpawnOn(cClientHandle & a_ClientHandle)
{
	a_ClientHandle.SendSpawnObject(*this, 50, 1, 0, 0);  // 50 means TNT
	m_bDirtyOrientation = false;
	m_bDirtyHead = false;
}





void cTNTEntity::Explode(void)
{
	m_FuseTicks = 0;
	Destroy(true);
	LOGD("BOOM at {%f, %f, %f}", GetPosX(), GetPosY(), GetPosZ());
	m_World->DoExplosionAt(4.0, GetPosX() + 0.49, GetPosY() + 0.49, GetPosZ() + 0.49, true, esPrimedTNT, this);
}





void cTNTEntity::Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	super::Tick(a_Dt, a_Chunk);
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




