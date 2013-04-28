#include "Globals.h"

#include "TNTEntity.h"
#include "World.h"
#include "ClientHandle.h"





cTNTEntity::cTNTEntity(int a_X,int a_Y,int a_Z,float a_MaxFuseTime) :
	super(etTNT, a_X + 0.5f, a_Y + 0.5f, a_Z + 0.5f)
{
	m_MaxFuseTime = a_MaxFuseTime;
	m_Counter = 0;
}





cTNTEntity::cTNTEntity(const Vector3i a_Pos,float a_MaxFuseTime) :
	super(etTNT, a_Pos.x,a_Pos.y,a_Pos.z)
{
	m_MaxFuseTime = a_MaxFuseTime;
	m_Counter = 0;
}




void cTNTEntity::Initialize(cWorld * a_World)
{
	super::Initialize(a_World);
	a_World->BroadcastSpawn(*this);
}





void cTNTEntity::SpawnOn(cClientHandle & a_ClientHandle)
{
	a_ClientHandle.SendSpawnObject(*this,50,1,0,0); //50 means TNT
	m_bDirtyPosition = false;
	m_bDirtySpeed = false;
	m_bDirtyOrientation = false;
	m_bDirtyHead = false;
}





void cTNTEntity::Tick(float a_Dt, cChunk & a_Chunk)
{
	super::Tick(a_Dt,a_Chunk);
	BroadcastMovementUpdate();
	float delta_time = a_Dt / 1000; //Convert miliseconds to seconds
	m_Counter += delta_time;
	if (m_Counter > m_MaxFuseTime) //Check if we go KABOOOM
	{
		Destroy();
		LOGD("BOOM at {%f,%f,%f}",GetPosX(),GetPosY(),GetPosZ());
		m_World->DoExplosiontAt(4.0,(int)floor(GetPosX()),(int)floor(GetPosY()),(int)floor(GetPosZ()));
		return;
	}
}




