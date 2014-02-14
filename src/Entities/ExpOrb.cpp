#include "Globals.h"

#include "ExpOrb.h"
#include "Player.h"
#include "../ClientHandle.h"

#include <ctime>


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
	cPlayer * a_ClosestPlayer(m_World->FindClosestPlayer(Vector3f(GetPosition()), 5));
	if (a_ClosestPlayer != NULL)
	{
		Vector3f a_PlayerPos(a_ClosestPlayer->GetPosition());
		a_PlayerPos.y++;
		Vector3f a_Distance(a_PlayerPos - GetPosition());
		double Distance(a_Distance.Length());
		if (Distance < 0.1f)
		{
			LOGD("Player %s picked up an ExpOrb. His reward is %i", a_ClosestPlayer->GetName().c_str(), m_Reward);
			a_ClosestPlayer->DeltaExperience(m_Reward);
			
			srand (static_cast <unsigned> (time(0)));
			float r1 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);  // Random Float Value (Java: random.nextFloat())
			float r2 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);  // Random Float Value (Java: random.nextFloat())
			a_ClosestPlayer->PlaySoundEffect("random.orb", 0.1F, 0.5F * ((r1 - r2) * 0.7F + 1.8F));
			
			Destroy(true);
		}
		a_Distance.Normalize();
		a_Distance *= ((float) (5.5 - Distance));
		SetSpeedX( a_Distance.x );
		SetSpeedY( a_Distance.y );
		SetSpeedZ( a_Distance.z );
		BroadcastMovementUpdate();
	}
	HandlePhysics(a_Dt, a_Chunk);
}
