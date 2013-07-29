#include "Globals.h"

#include "TNTEntity.h"
#include "World.h"
#include "ClientHandle.h"





cTNTEntity::cTNTEntity(double a_X, double a_Y, double a_Z, float a_FuseTimeInSec) :
	super(etTNT, a_X, a_Y, a_Z, 0.98, 0.98),
	m_Counter(0),
	m_MaxFuseTime(a_FuseTimeInSec)
{
}





cTNTEntity::cTNTEntity(const Vector3d & a_Pos, float a_FuseTimeInSec) :
	super(etTNT, a_Pos.x, a_Pos.y, a_Pos.z, 0.98, 0.98),
	m_Counter(0),
	m_MaxFuseTime(a_FuseTimeInSec)
{
}




void cTNTEntity::Initialize(cWorld * a_World)
{
	super::Initialize(a_World);
	a_World->BroadcastSpawnEntity(*this);
}





void cTNTEntity::SpawnOn(cClientHandle & a_ClientHandle)
{
	a_ClientHandle.SendSpawnObject(*this, 50, 1, 0, 0);  // 50 means TNT
	m_bDirtyPosition = false;
	m_bDirtySpeed = false;
	m_bDirtyOrientation = false;
	m_bDirtyHead = false;
}





void cTNTEntity::Tick(float a_Dt, cChunk & a_Chunk)
{
	super::Tick(a_Dt, a_Chunk);
	BroadcastMovementUpdate();
	float delta_time = a_Dt / 1000;  // Convert miliseconds to seconds
	m_Counter += delta_time;
	if (m_Counter > m_MaxFuseTime)  // Check if we go KABOOOM
	{
		Destroy(true);
		LOGD("BOOM at {%f,%f,%f}", GetPosX(), GetPosY(), GetPosZ());
		m_World->DoExplosiontAt(4.0, (int)floor(GetPosX()), (int)floor(GetPosY()), (int)floor(GetPosZ()));
		return;
	}
}




