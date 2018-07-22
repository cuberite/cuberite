#include "Globals.h"

#include "ExpOrb.h"
#include "Player.h"
#include "../ClientHandle.h"


cExpOrb::cExpOrb(double a_X, double a_Y, double a_Z, int a_Reward, float a_SpeedX, float a_SpeedY, float a_SpeedZ)
	: cEntity(etExpOrb, a_X, a_Y, a_Z, 0.98, 0.98)
	, m_Reward(a_Reward)
	, m_Timer(0)
	, MoveToPlayer(false)
{
	SetMaxHealth(5);
	SetHealth(5);
	SetGravity(-16);
	SetAirDrag(0.02f);
	SetSpeed(a_SpeedX, a_SpeedY, a_SpeedZ);
}





cExpOrb::cExpOrb(const Vector3d & a_Pos, int a_Reward, float a_SpeedX, float a_SpeedY, float a_SpeedZ)
	: cEntity(etExpOrb, a_Pos.x, a_Pos.y, a_Pos.z, 0.98, 0.98)
	, m_Reward(a_Reward)
	, m_Timer(0)
{
	SetMaxHealth(5);
	SetHealth(5);
	SetGravity(-16);
	SetAirDrag(0.02f);
	SetSpeed(a_SpeedX, a_SpeedY, a_SpeedZ);
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
	m_TicksAlive++;

	bool FindPlayer = false;

	// Find closest player within 6.5 meter (slightly increase detect range to have same effect in client)
	m_World->DoWithClosestPlayer(GetPosition(), 6.5, [&](cPlayer & a_Player) -> bool
	{
		FindPlayer = true;
		Vector3f a_PlayerPos(a_Player.GetPosition());
		a_PlayerPos.y++;
		Vector3f a_Distance(a_PlayerPos - GetPosition());
		double Distance(a_Distance.Length());

		if (Distance < 0.7f)
		{
			LOGD("Player %s picked up an ExpOrb. His reward is %i", a_Player.GetName().c_str(), m_Reward);
			a_Player.DeltaExperience(m_Reward);

			m_World->BroadcastSoundEffect("entity.experience_orb.pickup", GetPosition(), 0.5f, (0.75f + (static_cast<float>((GetUniqueID() * 23) % 32)) / 64));

			Destroy(true);
			return true;
		}

		/** Experience orb will "float" or glide toward the player up to a distance of 6 blocks,
		speeding up as they get nearer to the player, Speed range 6 - 10 m per second, accelerate 4 m per second^2 */
		if (MoveToPlayer)  // Already flying to player
		{
			Vector3d SpeedDelta(a_Distance);
			SpeedDelta.Normalize();
			SpeedDelta *= 2;

			Vector3d CurrentSpeed = GetSpeed();

			CurrentSpeed += SpeedDelta;

			if (CurrentSpeed.Length() > 10)
			{
				CurrentSpeed.Normalize();
				CurrentSpeed *= 10;
			}

			SetSpeed(CurrentSpeed);
		}
		else
		{
			// Start flying to player
			MoveToPlayer = true;
			m_Gravity = 0;
			Vector3d Speed(a_Distance);
			Speed.Normalize();
			Speed *= 6;

			SetSpeed(Speed);
		}

		return true;

	}, false, true);  // Don't check line of sight, ignore spectator mode player

	if (!FindPlayer)
	{
		MoveToPlayer = false;
		m_Gravity = -16;
	}

	HandlePhysics(a_Dt, a_Chunk);
	BroadcastMovementUpdate();

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







void cExpOrb::Splite(int a_Reward, std::vector<int> & a_SplitReward)
{
	const static std::array<int, 11> BaseValue = {1, 3, 7, 17, 37, 73, 149, 307, 617, 1237, 2477};
	int Index = BaseValue.size() - 1;  // Last one

	while (a_Reward > 0)
	{
		while (a_Reward < BaseValue[Index])
		{
			Index--;
		}

		a_Reward -= BaseValue[Index];
		a_SplitReward.push_back(BaseValue[Index]);
	}
}


