
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "IncludeAllMonsters.h"
#include "../Root.h"
#include "../Server.h"
#include "../ClientHandle.h"
#include "../World.h"
#include "../Entities/Player.h"
#include "../Entities/ExpOrb.h"
#include "../MonsterConfig.h"

#include "../Chunk.h"
#include "../FastRandom.h"

#include "Path.h"



/** Map for eType <-> string
Needs to be alpha-sorted by the strings, because binary search is used in StringToMobType()
The strings need to be lowercase (for more efficient comparisons in StringToMobType())
m_VanillaName is the name that vanilla use for this mob.
*/
static const struct
{
	eMonsterType m_Type;
	const char * m_lcName;
	const char * m_VanillaName;
} g_MobTypeNames[] =
{
	{mtBat,          "bat",          "Bat"},
	{mtBlaze,        "blaze",        "Blaze"},
	{mtCaveSpider,   "cavespider",   "CaveSpider"},
	{mtChicken,      "chicken",      "Chicken"},
	{mtCow,          "cow",          "Cow"},
	{mtCreeper,      "creeper",      "Creeper"},
	{mtEnderman,     "enderman",     "Enderman"},
	{mtEnderDragon,  "enderdragon",  "EnderDragon"},
	{mtGhast,        "ghast",        "Ghast"},
	{mtGiant,        "giant",        "Giant"},
	{mtGuardian,     "guardian",     "Guardian"},
	{mtHorse,        "horse",        "EntityHorse"},
	{mtIronGolem,    "irongolem",    "VillagerGolem"},
	{mtMagmaCube,    "magmacube",    "LavaSlime"},
	{mtMooshroom,    "mooshroom",    "MushroomCow"},
	{mtOcelot,       "ocelot",       "Ozelot"},
	{mtPig,          "pig",          "Pig"},
	{mtRabbit,       "rabbit",       "Rabbit"},
	{mtSheep,        "sheep",        "Sheep"},
	{mtSilverfish,   "silverfish",   "Silverfish"},
	{mtSkeleton,     "skeleton",     "Skeleton"},
	{mtSlime,        "slime",        "Slime"},
	{mtSnowGolem,    "snowgolem",    "SnowMan"},
	{mtSpider,       "spider",       "Spider"},
	{mtSquid,        "squid",        "Squid"},
	{mtVillager,     "villager",     "Villager"},
	{mtWitch,        "witch",        "Witch"},
	{mtWither,       "wither",       "WitherBoss"},
	{mtWolf,         "wolf",         "Wolf"},
	{mtZombie,       "zombie",       "Zombie"},
	{mtZombiePigman, "zombiepigman", "PigZombie"},
} ;





////////////////////////////////////////////////////////////////////////////////
// cMonster:

cMonster::cMonster(const AString & a_ConfigName, eMonsterType a_MobType, const AString & a_SoundHurt, const AString & a_SoundDeath, double a_Width, double a_Height)
	: super(etMonster, a_Width, a_Height)
	, m_EMState(IDLE)
	, m_EMPersonality(AGGRESSIVE)
	, m_Target(nullptr)
	, m_Path(nullptr)
	, m_IsFollowingPath(false)
	, m_PathfinderActivated(false)
	, m_GiveUpCounter(0)
	, m_LastGroundHeight(POSY_TOINT)
	, m_JumpCoolDown(0)
	, m_IdleInterval(0)
	, m_DestroyTimer(0)
	, m_MobType(a_MobType)
	, m_CustomName("")
	, m_CustomNameAlwaysVisible(false)
	, m_SoundHurt(a_SoundHurt)
	, m_SoundDeath(a_SoundDeath)
	, m_AttackRate(3)
	, m_AttackDamage(1)
	, m_AttackRange(1)
	, m_AttackInterval(0)
	, m_SightDistance(25)
	, m_DropChanceWeapon(0.085f)
	, m_DropChanceHelmet(0.085f)
	, m_DropChanceChestplate(0.085f)
	, m_DropChanceLeggings(0.085f)
	, m_DropChanceBoots(0.085f)
	, m_CanPickUpLoot(true)
	, m_TicksSinceLastDamaged(100)
	, m_BurnsInDaylight(false)
	, m_RelativeWalkSpeed(1)
	, m_Age(1)
{
	if (!a_ConfigName.empty())
	{
		GetMonsterConfig(a_ConfigName);
	}
}





void cMonster::SpawnOn(cClientHandle & a_Client)
{
	a_Client.SendSpawnMob(*this);
}





bool cMonster::TickPathFinding(cChunk & a_Chunk)
{
	if (!m_PathfinderActivated)
	{
		return false;
	}

	if (ReachedFinalDestination())
	{
		StopMovingToPosition();
		return false;
	}

	if (m_Path == nullptr)
	{
		if (!EnsureProperDestination(a_Chunk))
		{
			StopMovingToPosition();  // Invalid chunks, probably world is loading or something, cancel movement.
			return false;
		}
		m_GiveUpCounter = 40;
		m_Path = new cPath(a_Chunk, GetPosition(), m_FinalDestination, 20, GetWidth(), GetHeight());
	}

	switch (m_Path->Step(a_Chunk))
	{
		case ePathFinderStatus::NEARBY_FOUND:
		{
			m_FinalDestination = m_Path->AcceptNearbyPath();
			break;
		}
		case ePathFinderStatus::PATH_NOT_FOUND:
		{
			StopMovingToPosition();  // Try to calculate a path again.
			// Note that the next time may succeed, e.g. if a player breaks a barrier.
			break;
		}
		case ePathFinderStatus::CALCULATING:
		{
			// Pathfinder needs more time
			break;
		}
		case ePathFinderStatus::PATH_FOUND:
		{
			if ((--m_GiveUpCounter) == 0)
			{
				// Failed to reach a waypoint - that's a failure condition whichever point we're at
				if (m_EMState == CHASING)
				{
					ResetPathFinding();  // Try to calculate a path again.
					// This results in mobs hanging around an unreachable target (player).
				}
				else
				{
					StopMovingToPosition();  // Find a different place to go to.
				}
				return false;
			}
			else if (!m_Path->IsLastPoint())  // Have we arrived at the next cell, as denoted by m_NextWayPointPosition?
			{
				if ((m_Path->IsFirstPoint() || ReachedNextWaypoint()))
				{
					m_NextWayPointPosition = m_Path->GetNextPoint();
					m_GiveUpCounter = 40;  // Give up after 40 ticks (2 seconds) if failed to reach m_NextWayPointPosition.
				}
			}

			m_IsFollowingPath = true;
			return true;
		}
	}

	return false;
}





void cMonster::MoveToWayPoint(cChunk & a_Chunk)
{
	if (m_JumpCoolDown == 0)
	{
		if (DoesPosYRequireJump(FloorC(m_NextWayPointPosition.y)))
		{
			if (
				(IsOnGround() && (GetSpeedX() == 0.0f) && (GetSpeedY() == 0.0f)) ||
				(IsSwimming() && (m_GiveUpCounter < 15))
			)
			{
				m_bOnGround = false;
				m_JumpCoolDown = 20;
				// TODO: Change to AddSpeedY once collision detection is fixed - currently, mobs will go into blocks attempting to jump without a teleport
				AddPosY(1.6);  // Jump!!
				SetSpeedX(3.2 * (m_NextWayPointPosition.x - GetPosition().x));  // Move forward in a preset speed.
				SetSpeedZ(3.2 * (m_NextWayPointPosition.z - GetPosition().z));  // The numbers were picked based on trial and error and 1.6 and 3.2 are perfect.
			}
		}
	}
	else
	{
		--m_JumpCoolDown;
	}

	Vector3d Distance = m_NextWayPointPosition - GetPosition();
	if ((Distance.x != 0.0f) || (Distance.z != 0.0f))
	{
		Distance.y = 0;
		Distance.Normalize();

		if (m_bOnGround)
		{
			Distance *= 2.5f;
		}
		else if (IsSwimming())
		{
			Distance *= 1.3f;
		}
		else
		{
			// Don't let the mob move too much if he's falling.
			Distance *= 0.25f;
		}
		// Apply walk speed:
		Distance *= m_RelativeWalkSpeed;
		/* Reduced default speed.
		Close to Vanilla, easier for mobs to follow m_NextWayPointPositions, hence
		better pathfinding. */
		Distance *= 0.5;
		AddSpeedX(Distance.x);
		AddSpeedZ(Distance.z);
	}
}





bool cMonster::EnsureProperDestination(cChunk & a_Chunk)
{
	cChunk * Chunk = a_Chunk.GetNeighborChunk(FloorC(m_FinalDestination.x), FloorC(m_FinalDestination.z));
	BLOCKTYPE BlockType;
	NIBBLETYPE BlockMeta;

	if ((Chunk == nullptr) || !Chunk->IsValid())
	{
		return false;
	}

	int RelX = FloorC(m_FinalDestination.x) - Chunk->GetPosX() * cChunkDef::Width;
	int RelZ = FloorC(m_FinalDestination.z) - Chunk->GetPosZ() * cChunkDef::Width;

	// If destination in the air, first try to go 1 block north, or east, or west.
	// This fixes the player leaning issue.
	// If that failed, we instead go down to the lowest air block.
	Chunk->GetBlockTypeMeta(RelX, FloorC(m_FinalDestination.y) - 1, RelZ, BlockType, BlockMeta);
	if (!cBlockInfo::IsSolid(BlockType))
	{
		bool InTheAir = true;
		int x, z;
		for (z = -1; z <= 1; ++z)
		{
			for (x = -1; x <= 1; ++x)
			{
				if ((x==0) && (z==0))
				{
					continue;
				}
				Chunk = a_Chunk.GetNeighborChunk(FloorC(m_FinalDestination.x+x), FloorC(m_FinalDestination.z+z));
				if ((Chunk == nullptr) || !Chunk->IsValid())
				{
					return false;
				}
				RelX = FloorC(m_FinalDestination.x+x) - Chunk->GetPosX() * cChunkDef::Width;
				RelZ = FloorC(m_FinalDestination.z+z) - Chunk->GetPosZ() * cChunkDef::Width;
				Chunk->GetBlockTypeMeta(RelX, FloorC(m_FinalDestination.y) - 1, RelZ, BlockType, BlockMeta);
				if (cBlockInfo::IsSolid(BlockType))
				{
					m_FinalDestination.x += x;
					m_FinalDestination.z += z;
					InTheAir = false;
					goto breakBothLoops;
				}
			}
		}
		breakBothLoops:

		// Go down to the lowest air block.
		if (InTheAir)
		{
			while (m_FinalDestination.y > 0)
			{
				Chunk->GetBlockTypeMeta(RelX, FloorC(m_FinalDestination.y) - 1, RelZ, BlockType, BlockMeta);
				if (cBlockInfo::IsSolid(BlockType))
				{
					break;
				}
				m_FinalDestination.y -= 1;
			}
		}
	}

	// If destination in water, go up to the highest water block.
	// If destination in solid, go up to first air block.
	bool InWater = false;
	while (m_FinalDestination.y < cChunkDef::Height)
	{
		Chunk->GetBlockTypeMeta(RelX, FloorC(m_FinalDestination.y), RelZ, BlockType, BlockMeta);
		if (BlockType == E_BLOCK_STATIONARY_WATER)
		{
			InWater = true;
		}
		else if (cBlockInfo::IsSolid(BlockType))
		{
			InWater = false;
		}
		else
		{
			break;
		}
		m_FinalDestination.y += 1;
	}
	if (InWater)
	{
		m_FinalDestination.y -= 1;
	}


	return true;
}





void cMonster::MoveToPosition(const Vector3d & a_Position)
{
	m_FinalDestination = a_Position;
	m_PathfinderActivated = true;
}





void cMonster::StopMovingToPosition()
{
	m_PathfinderActivated = false;
	ResetPathFinding();
}





void cMonster::ResetPathFinding(void)
{
	m_IsFollowingPath = false;
	if (m_Path != nullptr)
	{
		delete m_Path;
		m_Path = nullptr;
	}
}





void cMonster::Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	super::Tick(a_Dt, a_Chunk);
	GET_AND_VERIFY_CURRENT_CHUNK(Chunk, POSX_TOINT, POSZ_TOINT);

	if (m_Health <= 0)
	{
		// The mob is dead, but we're still animating the "puff" they leave when they die.
		m_DestroyTimer += a_Dt;
		if (m_DestroyTimer > std::chrono::seconds(1))
		{
			Destroy(true);
		}
		return;
	}

	if (m_TicksSinceLastDamaged < 100)
	{
		++m_TicksSinceLastDamaged;
	}
	if ((m_Target != nullptr) && m_Target->IsDestroyed())
	{
		m_Target = nullptr;
	}

	// Process the undead burning in daylight.
	HandleDaylightBurning(*Chunk, WouldBurnAt(GetPosition(), *Chunk));
	if (TickPathFinding(*Chunk))
	{
		/* If I burn in daylight, and I won't burn where I'm standing, and I'll burn in my next position, and at least one of those is true:
		1. I am idle
		2. I was not hurt by a player recently.
		Then STOP. */
		if (
			m_BurnsInDaylight && ((m_TicksSinceLastDamaged >= 100) || (m_EMState == IDLE)) &&
			WouldBurnAt(m_NextWayPointPosition, *Chunk) &&
			!WouldBurnAt(GetPosition(), *Chunk)
		)
		{
			// If we burn in daylight, and we would burn at the next step, and we won't burn where we are right now, and we weren't provoked recently:
			StopMovingToPosition();
			m_GiveUpCounter = 40;  // This doesn't count as giving up, keep the giveup timer as is.
		}
		else
		{
			MoveToWayPoint(*Chunk);
		}
	}

	SetPitchAndYawFromDestination();
	HandleFalling();

	switch (m_EMState)
	{
		case IDLE:
		{
			// If enemy passive we ignore checks for player visibility.
			InStateIdle(a_Dt);
			break;
		}
		case CHASING:
		{
			// If we do not see a player anymore skip chasing action.
			InStateChasing(a_Dt);
			break;
		}
		case ESCAPING:
		{
			InStateEscaping(a_Dt);
			break;
		}
		case ATTACKING: break;
	}  // switch (m_EMState)

	BroadcastMovementUpdate();
}





void cMonster::SetPitchAndYawFromDestination()
{
	Vector3d FinalDestination = m_FinalDestination;
	if (m_Target != nullptr)
	{
		if (m_Target->IsPlayer())
		{
			FinalDestination.y = static_cast<cPlayer *>(m_Target)->GetStance() - 1;
		}
		else
		{
			FinalDestination.y = m_Target->GetPosY() + GetHeight();
		}
	}


	Vector3d BodyDistance;
	if (!m_IsFollowingPath && (m_Target != nullptr))
	{
		BodyDistance = m_Target->GetPosition() - GetPosition();
	}
	else
	{
		BodyDistance = m_NextWayPointPosition - GetPosition();
	}
	double BodyRotation, BodyPitch;
	BodyDistance.Normalize();
	VectorToEuler(BodyDistance.x, BodyDistance.y, BodyDistance.z, BodyRotation, BodyPitch);
	SetYaw(BodyRotation);

	Vector3d Distance = FinalDestination - GetPosition();
	{
		double HeadRotation, HeadPitch;
		Distance.Normalize();
		VectorToEuler(Distance.x, Distance.y, Distance.z, HeadRotation, HeadPitch);
		if (std::abs(BodyRotation - HeadRotation) < 90)
		{
			SetHeadYaw(HeadRotation);
			SetPitch(-HeadPitch);
		}
		else  // We're not an owl. If it's more than 120, don't look behind and instead look at where you're walking.
		{
			SetHeadYaw(BodyRotation);
			SetPitch(-BodyPitch);
		}
	}
}





void cMonster::HandleFalling()
{
	if (m_bOnGround)
	{
		int Damage = (m_LastGroundHeight - POSY_TOINT) - 3;

		if (Damage > 0)
		{
			TakeDamage(dtFalling, nullptr, Damage, Damage, 0);

			// Fall particles
			GetWorld()->BroadcastSoundParticleEffect(2006, POSX_TOINT, POSY_TOINT - 1, POSZ_TOINT, Damage /* Used as particle effect speed modifier */);
		}

		m_LastGroundHeight = POSY_TOINT;
	}
}





int cMonster::FindFirstNonAirBlockPosition(double a_PosX, double a_PosZ)
{
	int PosY = POSY_TOINT;
	PosY = Clamp(PosY, 0, cChunkDef::Height);

	if (!cBlockInfo::IsSolid(m_World->GetBlock(FloorC(a_PosX), PosY, FloorC(a_PosZ))))
	{
		while (!cBlockInfo::IsSolid(m_World->GetBlock(FloorC(a_PosX), PosY, FloorC(a_PosZ))) && (PosY > 0))
		{
			PosY--;
		}

		return PosY + 1;
	}
	else
	{
		while ((PosY < cChunkDef::Height) && cBlockInfo::IsSolid(m_World->GetBlock(static_cast<int>(floor(a_PosX)), PosY, static_cast<int>(floor(a_PosZ)))))
		{
			PosY++;
		}

		return PosY;
	}
}





bool cMonster::DoTakeDamage(TakeDamageInfo & a_TDI)
{
	if (!super::DoTakeDamage(a_TDI))
	{
		return false;
	}

	if (!m_SoundHurt.empty() && (m_Health > 0))
	{
		m_World->BroadcastSoundEffect(m_SoundHurt, GetPosX(), GetPosY(), GetPosZ(), 1.0f, 0.8f);
	}

	if (a_TDI.Attacker != nullptr)
	{
		m_Target = a_TDI.Attacker;
		m_TicksSinceLastDamaged = 0;
	}
	return true;
}





void cMonster::KilledBy(TakeDamageInfo & a_TDI)
{
	super::KilledBy(a_TDI);
	if (m_SoundHurt != "")
	{
		m_World->BroadcastSoundEffect(m_SoundDeath, GetPosX(), GetPosY(), GetPosZ(), 1.0f, 0.8f);
	}
	int Reward;
	switch (m_MobType)
	{
		// Animals
		case mtChicken:
		case mtCow:
		case mtHorse:
		case mtPig:
		case mtRabbit:
		case mtSheep:
		case mtSquid:
		case mtMooshroom:
		case mtOcelot:
		case mtWolf:
		{
			Reward = m_World->GetTickRandomNumber(2) + 1;
			break;
		}

		// Monsters
		case mtCaveSpider:
		case mtCreeper:
		case mtEnderman:
		case mtGhast:
		case mtGuardian:
		case mtSilverfish:
		case mtSkeleton:
		case mtSpider:
		case mtWitch:
		case mtZombie:
		case mtZombiePigman:
		case mtSlime:
		case mtMagmaCube:
		{
			Reward = 6 + (m_World->GetTickRandomNumber(2));
			break;
		}
		case mtBlaze:
		{
			Reward = 10;
			break;
		}

		// Bosses
		case mtEnderDragon:
		{
			Reward = 12000;
			break;
		}
		case mtWither:
		{
			Reward = 50;
			break;
		}

		default:
		{
			Reward = 0;
			break;
		}
	}
	if ((a_TDI.Attacker != nullptr) && (!IsBaby()))
	{
		m_World->SpawnExperienceOrb(GetPosX(), GetPosY(), GetPosZ(), Reward);
	}
	m_DestroyTimer = std::chrono::milliseconds(0);
}





void cMonster::OnRightClicked(cPlayer & a_Player)
{
	super::OnRightClicked(a_Player);

	const cItem & EquippedItem = a_Player.GetEquippedItem();
	if ((EquippedItem.m_ItemType == E_ITEM_NAME_TAG) && !EquippedItem.m_CustomName.empty())
	{
		SetCustomName(EquippedItem.m_CustomName);
		if (!a_Player.IsGameModeCreative())
		{
			a_Player.GetInventory().RemoveOneEquippedItem();
		}
	}
}





// Checks to see if EventSeePlayer should be fired
// monster sez: Do I see the player
void cMonster::CheckEventSeePlayer(void)
{
	// TODO: Rewrite this to use cWorld's DoWithPlayers()
	cPlayer * Closest = m_World->FindClosestPlayer(GetPosition(), static_cast<float>(m_SightDistance), false);

	if (Closest != nullptr)
	{
		EventSeePlayer(Closest);
	}
}





void cMonster::CheckEventLostPlayer(void)
{
	if (m_Target != nullptr)
	{
		if ((m_Target->GetPosition() - GetPosition()).Length() > m_SightDistance)
		{
			EventLosePlayer();
		}
	}
	else
	{
		EventLosePlayer();
	}
}





// What to do if player is seen
// default to change state to chasing
void cMonster::EventSeePlayer(cEntity * a_SeenPlayer)
{
	m_Target = a_SeenPlayer;
}





void cMonster::EventLosePlayer(void)
{
	m_Target = nullptr;
	m_EMState = IDLE;
}





void cMonster::InStateIdle(std::chrono::milliseconds a_Dt)
{
	if (m_IsFollowingPath)
	{
		return;  // Still getting there
	}

	m_IdleInterval += a_Dt;

	if (m_IdleInterval > std::chrono::seconds(1))
	{
		// At this interval the results are predictable
		int rem = m_World->GetTickRandomNumber(6) + 1;
		m_IdleInterval -= std::chrono::seconds(1);  // So nothing gets dropped when the server hangs for a few seconds

		Vector3d Dist;
		Dist.x = static_cast<double>(m_World->GetTickRandomNumber(10)) - 5.0;
		Dist.z = static_cast<double>(m_World->GetTickRandomNumber(10)) - 5.0;

		if ((Dist.SqrLength() > 2)  && (rem >= 3))
		{
			Vector3d Destination(GetPosX() + Dist.x, 0, GetPosZ() + Dist.z);
			Destination.y = FindFirstNonAirBlockPosition(Destination.x, Destination.z);
			MoveToPosition(Destination);
		}
	}
}





// What to do if in Chasing State
// This state should always be defined in each child class
void cMonster::InStateChasing(std::chrono::milliseconds a_Dt)
{
	UNUSED(a_Dt);
}





// What to do if in Escaping State
void cMonster::InStateEscaping(std::chrono::milliseconds a_Dt)
{
	UNUSED(a_Dt);

	if (m_Target != nullptr)
	{
		Vector3d newloc = GetPosition();
		newloc.x = (m_Target->GetPosition().x < newloc.x)? (newloc.x + m_SightDistance): (newloc.x - m_SightDistance);
		newloc.z = (m_Target->GetPosition().z < newloc.z)? (newloc.z + m_SightDistance): (newloc.z - m_SightDistance);
		MoveToPosition(newloc);
	}
	else
	{
		m_EMState = IDLE;  // This shouldnt be required but just to be safe
	}
}





void cMonster::SetCustomName(const AString & a_CustomName)
{
	m_CustomName = a_CustomName;

	// The maximal length is 64
	if (a_CustomName.length() > 64)
	{
		m_CustomName = a_CustomName.substr(0, 64);
	}

	if (m_World != nullptr)
	{
		m_World->BroadcastEntityMetadata(*this);
	}
}





void cMonster::SetCustomNameAlwaysVisible(bool a_CustomNameAlwaysVisible)
{
	m_CustomNameAlwaysVisible = a_CustomNameAlwaysVisible;
	if (m_World != nullptr)
	{
		m_World->BroadcastEntityMetadata(*this);
	}
}





void cMonster::GetMonsterConfig(const AString & a_Name)
{
	cRoot::Get()->GetMonsterConfig()->AssignAttributes(this, a_Name);
}





bool cMonster::IsUndead(void)
{
	return false;
}





AString cMonster::MobTypeToString(eMonsterType a_MobType)
{
	// Mob types aren't sorted, so we need to search linearly:
	for (size_t i = 0; i < ARRAYCOUNT(g_MobTypeNames); i++)
	{
		if (g_MobTypeNames[i].m_Type == a_MobType)
		{
			return g_MobTypeNames[i].m_lcName;
		}
	}

	// Not found:
	return "";
}





AString cMonster::MobTypeToVanillaName(eMonsterType a_MobType)
{
	// Mob types aren't sorted, so we need to search linearly:
	for (size_t i = 0; i < ARRAYCOUNT(g_MobTypeNames); i++)
	{
		if (g_MobTypeNames[i].m_Type == a_MobType)
		{
			return g_MobTypeNames[i].m_VanillaName;
		}
	}

	// Not found:
	return "";
}





eMonsterType cMonster::StringToMobType(const AString & a_Name)
{
	AString lcName = StrToLower(a_Name);

	// Search MCServer name:
	for (size_t i = 0; i < ARRAYCOUNT(g_MobTypeNames); i++)
	{
		if (strcmp(g_MobTypeNames[i].m_lcName, lcName.c_str()) == 0)
		{
			return g_MobTypeNames[i].m_Type;
		}
	}

	// Not found. Search Vanilla name:
	for (size_t i = 0; i < ARRAYCOUNT(g_MobTypeNames); i++)
	{
		if (strcmp(StrToLower(g_MobTypeNames[i].m_VanillaName).c_str(), lcName.c_str()) == 0)
		{
			return g_MobTypeNames[i].m_Type;
		}
	}

	// Not found:
	return mtInvalidType;
}





cMonster::eFamily cMonster::FamilyFromType(eMonsterType a_Type)
{
	// Passive-agressive mobs are counted in mob spawning code as passive

	switch (a_Type)
	{
		case mtBat:          return mfAmbient;
		case mtBlaze:        return mfHostile;
		case mtCaveSpider:   return mfHostile;
		case mtChicken:      return mfPassive;
		case mtCow:          return mfPassive;
		case mtCreeper:      return mfHostile;
		case mtEnderDragon:  return mfNoSpawn;
		case mtEnderman:     return mfHostile;
		case mtGhast:        return mfHostile;
		case mtGiant:        return mfNoSpawn;
		case mtGuardian:     return mfWater;  // Just because they have special spawning conditions. If Watertemples have been added, this needs to be edited!
		case mtHorse:        return mfPassive;
		case mtIronGolem:    return mfPassive;
		case mtMagmaCube:    return mfHostile;
		case mtMooshroom:    return mfHostile;
		case mtOcelot:       return mfPassive;
		case mtPig:          return mfPassive;
		case mtRabbit:       return mfPassive;
		case mtSheep:        return mfPassive;
		case mtSilverfish:   return mfHostile;
		case mtSkeleton:     return mfHostile;
		case mtSlime:        return mfHostile;
		case mtSnowGolem:    return mfNoSpawn;
		case mtSpider:       return mfHostile;
		case mtSquid:        return mfWater;
		case mtVillager:     return mfPassive;
		case mtWitch:        return mfHostile;
		case mtWither:       return mfNoSpawn;
		case mtWolf:         return mfHostile;
		case mtZombie:       return mfHostile;
		case mtZombiePigman: return mfHostile;

		case mtInvalidType:  break;
	}
	ASSERT(!"Unhandled mob type");
	return mfUnhandled;
}





int cMonster::GetSpawnDelay(cMonster::eFamily a_MobFamily)
{
	switch (a_MobFamily)
	{
		case mfHostile:   return 40;
		case mfPassive:   return 40;
		case mfAmbient:   return 40;
		case mfWater:     return 400;
		case mfNoSpawn:   return -1;
		case mfUnhandled: break;
	}
	ASSERT(!"Unhandled mob family");
	return -1;
}





cMonster * cMonster::NewMonsterFromType(eMonsterType a_MobType)
{
	cFastRandom Random;
	cMonster * toReturn = nullptr;

	// Create the mob entity
	switch (a_MobType)
	{
		case mtMagmaCube:
		{
			toReturn = new cMagmaCube(1 << Random.NextInt(3));  // Size 1, 2 or 4
			break;
		}
		case mtSlime:
		{
			toReturn = new cSlime(1 << Random.NextInt(3));  // Size 1, 2 or 4
			break;
		}
		case mtSkeleton:
		{
			// TODO: Actual detection of spawning in Nether
			toReturn = new cSkeleton((Random.NextInt(1) == 0) ? false : true);
			break;
		}
		case mtVillager:
		{
			int VillagerType = Random.NextInt(6);
			if (VillagerType == 6)
			{
				// Give farmers a better chance of spawning
				VillagerType = 0;
			}

			toReturn = new cVillager(static_cast<cVillager::eVillagerType>(VillagerType));
			break;
		}
		case mtHorse:
		{
			// Horses take a type (species), a colour, and a style (dots, stripes, etc.)
			int HorseType = Random.NextInt(8);
			int HorseColor = Random.NextInt(7);
			int HorseStyle = Random.NextInt(5);
			int HorseTameTimes = Random.NextInt(6) + 1;

			if ((HorseType == 5) || (HorseType == 6) || (HorseType == 7))
			{
				// Increase chances of normal horse (zero)
				HorseType = 0;
			}

			toReturn = new cHorse(HorseType, HorseColor, HorseStyle, HorseTameTimes);
			break;
		}

		case mtBat:           toReturn = new cBat();                      break;
		case mtBlaze:         toReturn = new cBlaze();                    break;
		case mtCaveSpider:    toReturn = new cCaveSpider();               break;
		case mtChicken:       toReturn = new cChicken();                  break;
		case mtCow:           toReturn = new cCow();                      break;
		case mtCreeper:       toReturn = new cCreeper();                  break;
		case mtEnderDragon:   toReturn = new cEnderDragon();              break;
		case mtEnderman:      toReturn = new cEnderman();                 break;
		case mtGhast:         toReturn = new cGhast();                    break;
		case mtGiant:         toReturn = new cGiant();                    break;
		case mtGuardian:      toReturn = new cGuardian();                 break;
		case mtIronGolem:     toReturn = new cIronGolem();                break;
		case mtMooshroom:     toReturn = new cMooshroom();                break;
		case mtOcelot:        toReturn = new cOcelot();                   break;
		case mtPig:           toReturn = new cPig();                      break;
		case mtRabbit:        toReturn = new cRabbit();                   break;
		case mtSheep:         toReturn = new cSheep();                    break;
		case mtSilverfish:    toReturn = new cSilverfish();               break;
		case mtSnowGolem:     toReturn = new cSnowGolem();                break;
		case mtSpider:        toReturn = new cSpider();                   break;
		case mtSquid:         toReturn = new cSquid();                    break;
		case mtWitch:         toReturn = new cWitch();                    break;
		case mtWither:	      toReturn = new cWither();                   break;
		case mtWolf:          toReturn = new cWolf();                     break;
		case mtZombie:        toReturn = new cZombie(false);              break;  // TODO: Infected zombie parameter
		case mtZombiePigman:  toReturn = new cZombiePigman();             break;
		default:
		{
			ASSERT(!"Unhandled mob type whilst trying to spawn mob!");
		}
	}
	return toReturn;
}





void cMonster::AddRandomDropItem(cItems & a_Drops, unsigned int a_Min, unsigned int a_Max, short a_Item, short a_ItemHealth)
{
	MTRand r1;
	int Count = static_cast<int>(static_cast<unsigned int>(r1.randInt()) % (a_Max + 1 - a_Min) + a_Min);
	if (Count > 0)
	{
		a_Drops.push_back(cItem(a_Item, static_cast<char>(Count), a_ItemHealth));
	}
}





void cMonster::AddRandomUncommonDropItem(cItems & a_Drops, float a_Chance, short a_Item, short a_ItemHealth)
{
	MTRand r1;
	int Count = r1.randInt() % 1000;
	if (Count < (a_Chance * 10))
	{
		a_Drops.push_back(cItem(a_Item, 1, a_ItemHealth));
	}
}





void cMonster::AddRandomRareDropItem(cItems & a_Drops, cItems & a_Items, unsigned int a_LootingLevel)
{
	MTRand r1;
	unsigned int Count = static_cast<unsigned int>(static_cast<unsigned long>(r1.randInt()) % 200);
	if (Count < (5 + a_LootingLevel))
	{
		size_t Rare = static_cast<size_t>(r1.randInt()) % a_Items.Size();
		a_Drops.push_back(a_Items.at(Rare));
	}
}





void cMonster::AddRandomArmorDropItem(cItems & a_Drops, unsigned int a_LootingLevel)
{
	MTRand r1;
	if (r1.randInt() % 200 < ((m_DropChanceHelmet * 200) + (a_LootingLevel * 2)))
	{
		if (!GetEquippedHelmet().IsEmpty())
		{
			a_Drops.push_back(GetEquippedHelmet());
		}
	}

	if (r1.randInt() % 200 < ((m_DropChanceChestplate * 200) + (a_LootingLevel * 2)))
	{
		if (!GetEquippedChestplate().IsEmpty())
		{
			a_Drops.push_back(GetEquippedChestplate());
		}
	}

	if (r1.randInt() % 200 < ((m_DropChanceLeggings * 200) + (a_LootingLevel * 2)))
	{
		if (!GetEquippedLeggings().IsEmpty())
		{
			a_Drops.push_back(GetEquippedLeggings());
		}
	}

	if (r1.randInt() % 200 < ((m_DropChanceBoots * 200) + (a_LootingLevel * 2)))
	{
		if (!GetEquippedBoots().IsEmpty())
		{
			a_Drops.push_back(GetEquippedBoots());
		}
	}
}





void cMonster::AddRandomWeaponDropItem(cItems & a_Drops, unsigned int a_LootingLevel)
{
	MTRand r1;
	if (r1.randInt() % 200 < ((m_DropChanceWeapon * 200) + (a_LootingLevel * 2)))
	{
		if (!GetEquippedWeapon().IsEmpty())
		{
			a_Drops.push_back(GetEquippedWeapon());
		}
	}
}





void cMonster::HandleDaylightBurning(cChunk & a_Chunk, bool WouldBurn)
{
	if (!m_BurnsInDaylight)
	{
		return;
	}

	int RelY = POSY_TOINT;
	if ((RelY < 0) || (RelY >= cChunkDef::Height))
	{
		// Outside the world
		return;
	}
	if (!a_Chunk.IsLightValid())
	{
		m_World->QueueLightChunk(GetChunkX(), GetChunkZ());
		return;
	}

	if (!IsOnFire() && WouldBurn)
	{
		// Burn for 100 ticks, then decide again
		StartBurning(100);
	}
}




bool cMonster::WouldBurnAt(Vector3d a_Location, cChunk & a_Chunk)
{
	cChunk * Chunk = a_Chunk.GetNeighborChunk(FloorC(a_Location.x), FloorC(a_Location.z));
	if ((Chunk == nullptr) || (!Chunk->IsValid()))
	{
		return false;
	}

	int RelX = FloorC(a_Location.x) - Chunk->GetPosX() * cChunkDef::Width;
	int RelY = FloorC(a_Location.y);
	int RelZ = FloorC(a_Location.z) - Chunk->GetPosZ() * cChunkDef::Width;

	if (
		(Chunk->GetSkyLight(RelX, RelY, RelZ) == 15) &&             // In the daylight
		(Chunk->GetBlock(RelX, RelY, RelZ) != E_BLOCK_SOULSAND) &&  // Not on soulsand
		(GetWorld()->GetTimeOfDay() < (12000 + 1000)) &&             // It is nighttime
		GetWorld()->IsWeatherSunnyAt(POSX_TOINT, POSZ_TOINT)         // Not raining
	)
	{
		return true;
	}
	return false;
}





cMonster::eFamily cMonster::GetMobFamily(void) const
{
	return FamilyFromType(m_MobType);
}
