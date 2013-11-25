#include "Globals.h"

#include "ExpOrb.h"
#include "Player.h"
#include "../ClientHandle.h"


cExpOrb::cExpOrb(double a_X, double a_Y, double a_Z, int a_Reward) :
	cEntity(etExpOrb, a_X, a_Y, a_Z, 0.98, 0.98),
	m_Reward(a_Reward)
{
}





cExpOrb::cExpOrb(const Vector3d & a_Pos, int a_Reward) :
	cEntity(etExpOrb, a_Pos.x, a_Pos.y, a_Pos.z, 0.98, 0.98),
	m_Reward(a_Reward)
{
}





void cExpOrb::SpawnOn(cClientHandle & a_Client)
{
	a_Client.SendExperienceOrb(*this);
	m_bDirtyPosition = false;
	m_bDirtySpeed = false;
	m_bDirtyOrientation = false;
	m_bDirtyHead = false;
}





void cExpOrb::Tick(float a_Dt, cChunk & a_Chunk)
{
	cPlayer * a_ClosestPlayer(m_World->FindClosestPlayer(Vector3f(GetPosition()), 3));
	if (a_ClosestPlayer)
	{
		Vector3f a_PlayerPos(a_ClosestPlayer->GetPosition());
		Vector3f a_Distance(a_PlayerPos - GetPosition());
		if (a_Distance.Length() < 0.1f)
		{
			a_ClosestPlayer->DeltaExperience(m_Reward);
			a_ClosestPlayer->SendExperience();
			Destroy(true);
		}
		a_Distance.y = 0;
		a_Distance.Normalize();
		a_Distance *= 3;
		SetSpeedX( a_Distance.x );
		SetSpeedZ( a_Distance.z );
	}
}