#include "Globals.h"

#include "ExpOrb.h"
#include "Player.h"
#include "../ClientHandle.h"


cExpOrb::cExpOrb(Vector3d a_Pos, int a_Reward):
	Super(etExpOrb, a_Pos, 0.98, 0.98),  // TODO: Check size
	m_Reward(a_Reward),
	m_Timer(0)
{
	SetMaxHealth(5);
	SetHealth(5);
	SetGravity(-16);
	SetAirDrag(0.02f);
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

	// Find closest player within 6.5 meter (slightly increase detect range to have same effect in client)
	bool FoundPlayer = m_World->DoWithNearestPlayer(GetPosition(), 6.5, [&](cPlayer & a_Player) -> bool
	{
		Vector3f a_PlayerPos(a_Player.GetPosition());
		a_PlayerPos.y += 0.8f;
		Vector3f a_Distance = a_PlayerPos - GetPosition();
		double Distance = a_Distance.Length();

		if (Distance < 0.7f)
		{
			a_Player.DeltaExperience(m_Reward);

			m_World->BroadcastSoundEffect("entity.experience_orb.pickup", GetPosition(), 0.5f, (0.75f + (static_cast<float>((GetUniqueID() * 23) % 32)) / 64));
			Destroy();
			return true;
		}

		// Experience orb will "float" or glide toward the player up to a distance of 6 blocks.
		// speeding up as they get nearer to the player, Speed range 6 - 12 m per second, accelerate 60 m per second^2
		Vector3d SpeedDelta(a_Distance);
		SpeedDelta.Normalize();
		SpeedDelta *= 3;

		Vector3d CurrentSpeed = GetSpeed();
		CurrentSpeed += SpeedDelta;
		if (CurrentSpeed.Length() > 12)
		{
			CurrentSpeed.Normalize();
			CurrentSpeed *= 12;
		}

		SetSpeed(CurrentSpeed);
		m_Gravity = 0;

		return true;
	}, false, true);  // Don't check line of sight, ignore spectator mode player

	if (!FoundPlayer)
	{
		m_Gravity = -16;
	}

	HandlePhysics(a_Dt, a_Chunk);
	BroadcastMovementUpdate();

	m_Timer += a_Dt;
	if (m_Timer >= std::chrono::minutes(5))
	{
		Destroy();
	}
}





bool cExpOrb::DoTakeDamage(TakeDamageInfo & a_TDI)
{
	if (a_TDI.DamageType == dtCactusContact)
	{
		Destroy();
		return true;
	}

	return Super::DoTakeDamage(a_TDI);
}





std::vector<int> cExpOrb::Split(int a_Reward)
{
	const static std::array<int, 11> BaseValue = {{1, 3, 7, 17, 37, 73, 149, 307, 617, 1237, 2477}};

	std::vector<int> Rewards;
	size_t Index = BaseValue.size() - 1;  // Last one

	while (a_Reward > 0)
	{
		while (a_Reward < BaseValue[Index])
		{
			Index--;
		}

		a_Reward -= BaseValue[Index];
		Rewards.push_back(BaseValue[Index]);
	}

	return Rewards;
}


