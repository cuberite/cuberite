#include "Globals.h"

#include "ExpOrb.h"
#include "Player.h"
#include "../ClientHandle.h"


cExpOrb::cExpOrb(double a_X, double a_Y, double a_Z, int a_Reward)
	: cEntity(etExpOrb, a_X, a_Y, a_Z, 0.98, 0.98)
	, m_Reward(a_Reward)
	, m_Timer(0)
{
	SetMaxHealth(5);
	SetHealth(5);
	SetGravity(0);
}





cExpOrb::cExpOrb(const Vector3d & a_Pos, int a_Reward)
	: cEntity(etExpOrb, a_Pos.x, a_Pos.y, a_Pos.z, 0.98, 0.98)
	, m_Reward(a_Reward)
	, m_Timer(0)
{
	SetMaxHealth(5);
	SetHealth(5);
	SetGravity(0);
}





void cExpOrb::SpawnOn(cClientHandle & a_Client)
{
	a_Client.SendExperienceOrb(*this);
	m_bDirtyOrientation = false;
	m_bDirtyHead = false;
}





void cExpOrb::Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	DetectCacti();

	// Check player proximity no more than twice per second
	if ((m_TicksAlive % 10) == 0)
	{
		cPlayer * a_ClosestPlayer(m_World->FindClosestPlayer(Vector3f(GetPosition()), 5, false));
		if ((a_ClosestPlayer != nullptr) && (!a_ClosestPlayer->IsGameModeSpectator()))
		{
			Vector3f a_PlayerPos(a_ClosestPlayer->GetPosition());
			a_PlayerPos.y++;
			Vector3f a_Distance(a_PlayerPos - GetPosition());
			double Distance(a_Distance.Length());
			if (Distance < 0.5f)
			{
				LOGD("Player %s picked up an ExpOrb. His reward is %i", a_ClosestPlayer->GetName().c_str(), m_Reward);
				a_ClosestPlayer->DeltaExperience(m_Reward);

				m_World->BroadcastSoundEffect("entity.experience_orb.pickup", GetPosition(), 0.5f, (0.75f + (static_cast<float>((GetUniqueID() * 23) % 32)) / 64));

				Destroy(true);
				return;
			}
			SetSpeedX((a_PlayerPos.x - GetPosition().x) * 2.0);
			SetSpeedY((a_PlayerPos.y - GetPosition().y) * 2.0);
			SetSpeedZ((a_PlayerPos.z - GetPosition().z) * 2.0);
		}
	}
	HandlePhysics(a_Dt, a_Chunk);

	m_Timer += a_Dt;
	if (m_Timer >= std::chrono::minutes(5))
	{
		Destroy(true);
	}
}

bool cExpOrb::DoTakeDamage(TakeDamageInfo & a_TDI)
{
	if (a_TDI.DamageType == dtCactusContact)
	{
		Destroy(true);
		return true;
	}

	return super::DoTakeDamage(a_TDI);
}
