
#include "Globals.h"

#include "Floater.h"
#include "Player.h"
#include "../ClientHandle.h"





cFloater::cFloater(double a_X, double a_Y, double a_Z, Vector3d a_Speed, int a_PlayerID) :
	cEntity(etFloater, a_X, a_Y, a_Z, 0.98, 0.98),
	m_PickupCountDown(0),
	m_PlayerID(a_PlayerID),
	m_CanPickupItem(false)
{
	SetSpeed(a_Speed);
}





void cFloater::SpawnOn(cClientHandle & a_Client)
{
	a_Client.SendSpawnObject(*this, 90, m_PlayerID, 0, 0);
}





void cFloater::Tick(float a_Dt, cChunk & a_Chunk)
{
	HandlePhysics(a_Dt, a_Chunk);
	if (IsBlockWater(m_World->GetBlock((int) GetPosX(), (int) GetPosY(), (int) GetPosZ())) && m_World->GetBlockMeta((int) GetPosX(), (int) GetPosY(), (int) GetPosZ()) != 0)
	{
		if ((!m_CanPickupItem) && (m_World->GetTickRandomNumber(100) == 0))
		{
			SetPosY(GetPosY() - 1);
			m_CanPickupItem = true;
			m_PickupCountDown = 20;
			LOGD("Floater %i can be picked up", GetUniqueID());
		}
		else
		{
			SetSpeedY(0.7);
		}
	}
	SetSpeedX(GetSpeedX() * 0.95);
	SetSpeedZ(GetSpeedZ() * 0.95);
	if (CanPickup())
	{
		m_PickupCountDown--;
		if (m_PickupCountDown == 0)
		{
			m_CanPickupItem = false;
			LOGD("The fish is gone. Floater %i can not pick an item up.", GetUniqueID());
		}
	}
	BroadcastMovementUpdate();
}




