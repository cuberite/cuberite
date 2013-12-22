
#include "Globals.h"

#include "Floater.h"
#include "Player.h"
#include "../ClientHandle.h"





cFloater::cFloater(double a_X, double a_Y, double a_Z, Vector3d a_Speed, int a_PlayerID, int a_CountDownTime) :
	cEntity(etFloater, a_X, a_Y, a_Z, 0.98, 0.98),
	m_PickupCountDown(0),
	m_PlayerID(a_PlayerID),
	m_CanPickupItem(false),
	m_CountDownTime(a_CountDownTime)
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
	if (IsBlockWater(m_World->GetBlock((int) GetPosX(), (int) GetPosY(), (int) GetPosZ())) && m_World->GetBlockMeta((int) GetPosX(), (int) GetPosY(), (int) GetPosZ()) == 0)
	{
		if (!m_CanPickupItem)
		{
			if (m_CountDownTime <= 0)
			{
				m_World->BroadcastSoundEffect("random.splash", (int) floor(GetPosX() * 8), (int) floor(GetPosY() * 8), (int) floor(GetPosZ() * 8), 1, 1);
				SetPosY(GetPosY() - 1);
				m_CanPickupItem = true;
				m_PickupCountDown = 20;
				m_CountDownTime = 100 + m_World->GetTickRandomNumber(800);
				LOGD("Floater %i can be picked up", GetUniqueID());
			}
			else if (m_CountDownTime == 20) // Calculate the position where the particles should spawn and start producing them.
			{
				LOGD("Started producing particles for floater %i", GetUniqueID());
				m_ParticlePos.Set(GetPosX() + (-4 + m_World->GetTickRandomNumber(8)), GetPosY(), GetPosZ() + (-4 + m_World->GetTickRandomNumber(8)));
				m_World->BroadcastParticleEffect("splash", (float) m_ParticlePos.x, (float) m_ParticlePos.y, (float) m_ParticlePos.z, 0, 0, 0, 0, 15);
			}
			else if (m_CountDownTime < 20)
			{
				m_ParticlePos = (m_ParticlePos + (GetPosition() - m_ParticlePos) / 6);
				m_World->BroadcastParticleEffect("splash", (float) m_ParticlePos.x, (float) m_ParticlePos.y, (float) m_ParticlePos.z, 0, 0, 0, 0, 15);
			}
			m_CountDownTime--;
		}
		SetSpeedY(0.7);
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




