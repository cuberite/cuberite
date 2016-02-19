
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "IncludeAllMonsters.h"
#include "../Root.h"
#include "../Server.h"
#include "../ClientHandle.h"
#include "../World.h"
#include "../EffectID.h"
#include "../Entities/Player.h"
#include "../Entities/ExpOrb.h"
#include "../MonsterConfig.h"

#include "../Chunk.h"
#include "../FastRandom.h"

#include "PathFinder.h"



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
	, m_PathFinder(a_Width, a_Height)
	, m_PathfinderActivated(false)
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
	, m_AttackCoolDownTicksLeft(0)
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
	, m_AgingTimer(20 * 60 * 20)  // about 20 minutes
	, m_Target(nullptr)
{
	if (!a_ConfigName.empty())
	{
		GetMonsterConfig(a_ConfigName);
	}
}





cMonster::~cMonster()
{
	ASSERT(GetTarget() == nullptr);
}





void cMonster::Destroyed()
{
	SetTarget(nullptr);  // Tell them we're no longer targeting them.
	super::Destroyed();
}





void cMonster::SpawnOn(cClientHandle & a_Client)
{
	a_Client.SendSpawnMob(*this);
}





void cMonster::MoveToWayPoint(cChunk & a_Chunk)
{
	if ((m_NextWayPointPosition - GetPosition()).SqrLength() < WAYPOINT_RADIUS * WAYPOINT_RADIUS)
	{
		return;
	}


	if (m_JumpCoolDown == 0)
	{
		if (DoesPosYRequireJump(FloorC(m_NextWayPointPosition.y)))
		{
			if (((IsOnGround()) && (GetSpeed().SqrLength() == 0.0f)) ||
			(IsSwimming()))
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











void cMonster::MoveToPosition(const Vector3d & a_Position)
{
	m_FinalDestination = a_Position;
	m_PathfinderActivated = true;
}





void cMonster::StopMovingToPosition()
{
	m_PathfinderActivated = false;
}





void cMonster::Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	super::Tick(a_Dt, a_Chunk);
	GET_AND_VERIFY_CURRENT_CHUNK(Chunk, POSX_TOINT, POSZ_TOINT);

	ASSERT((GetTarget() == nullptr) || (GetTarget()->IsPawn() && (GetTarget()->GetWorld() == GetWorld())));
	if (m_AttackCoolDownTicksLeft > 0)
	{
		m_AttackCoolDownTicksLeft -= 1;
	}

	if (m_Health <= 0)
	{
		// The mob is dead, but we're still animating the "puff" they leave when they die
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
	if ((GetTarget() != nullptr))
	{
		ASSERT(GetTarget()->IsTicking());

		if (GetTarget()->IsPlayer())
		{
			if (static_cast<cPlayer *>(GetTarget())->IsGameModeCreative())
			{
				SetTarget(nullptr);
				m_EMState = IDLE;
			}
		}
	}

	// Process the undead burning in daylight.
	HandleDaylightBurning(*Chunk, WouldBurnAt(GetPosition(), *Chunk));

	bool a_IsFollowingPath = false;
	if (m_PathfinderActivated)
	{
		if (ReachedFinalDestination())
		{
			StopMovingToPosition();  // Simply sets m_PathfinderActivated to false.
		}
		else
		{
			// Note that m_NextWayPointPosition is actually returned by GetNextWayPoint)
			switch (m_PathFinder.GetNextWayPoint(*Chunk, GetPosition(), &m_FinalDestination, &m_NextWayPointPosition, m_EMState == IDLE ? true : false))
			{
				case ePathFinderStatus::PATH_FOUND:
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
					}
					else
					{
						a_IsFollowingPath = true;  // Used for proper body / head orientation only.
						MoveToWayPoint(*Chunk);
					}
					break;
				}
				case ePathFinderStatus::PATH_NOT_FOUND:
				{
					StopMovingToPosition();
					break;
				}
				default:
				{

				}
			}
		}
	}

	SetPitchAndYawFromDestination(a_IsFollowingPath);

	switch (m_EMState)
	{
		case IDLE:
		{
			// If enemy passive we ignore checks for player visibility.
			InStateIdle(a_Dt, a_Chunk);
			break;
		}
		case CHASING:
		{
			// If we do not see a player anymore skip chasing action.
			InStateChasing(a_Dt, a_Chunk);
			break;
		}
		case ESCAPING:
		{
			InStateEscaping(a_Dt, a_Chunk);
			break;
		}
		case ATTACKING: break;
	}  // switch (m_EMState)

	BroadcastMovementUpdate();

	if (m_AgingTimer > 0)
	{
		m_AgingTimer--;
		if ((m_AgingTimer <= 0) && IsBaby())
		{
			SetAge(1);
			m_World->BroadcastEntityMetadata(*this);
		}
	}
}





void cMonster::SetPitchAndYawFromDestination(bool a_IsFollowingPath)
{
	Vector3d BodyDistance;
	if (!a_IsFollowingPath && (GetTarget() != nullptr))
	{
		BodyDistance = GetTarget()->GetPosition() - GetPosition();
	}
	else
	{
		BodyDistance = m_NextWayPointPosition - GetPosition();
	}
	double BodyRotation, BodyPitch;
	BodyDistance.Normalize();
	VectorToEuler(BodyDistance.x, BodyDistance.y, BodyDistance.z, BodyRotation, BodyPitch);
	SetYaw(BodyRotation);

	Vector3d HeadDistance;
	if (GetTarget() != nullptr)
	{
		if (GetTarget()->IsPlayer())  // Look at a player
		{
			HeadDistance = GetTarget()->GetPosition() - GetPosition();
		}
		else  // Look at some other entity
		{
			HeadDistance = GetTarget()->GetPosition() - GetPosition();
			// HeadDistance.y = GetTarget()->GetPosY() + GetHeight();
		}
	}
	else  // Look straight
	{
		HeadDistance = BodyDistance;
		HeadDistance.y = 0;
	}

	double HeadRotation, HeadPitch;
	HeadDistance.Normalize();
	VectorToEuler(HeadDistance.x, HeadDistance.y, HeadDistance.z, HeadRotation, HeadPitch);
	if ((std::abs(BodyRotation - HeadRotation) < 70) && (std::abs(HeadPitch) < 60))
	{
		SetHeadYaw(HeadRotation);
		SetPitch(-HeadPitch);
	}
	else
	{
		SetHeadYaw(BodyRotation);
		SetPitch(0);
	}
}





void cMonster::HandleFalling()
{
	m_bTouchGround = IsOnGround();
	super::HandleFalling();
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

	if ((a_TDI.Attacker != nullptr) && a_TDI.Attacker->IsPawn())
	{
		SetTarget(static_cast<cPawn*>(a_TDI.Attacker));
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
void cMonster::CheckEventSeePlayer(cChunk & a_Chunk)
{
	// TODO: Rewrite this to use cWorld's DoWithPlayers()
	cPlayer * Closest = m_World->FindClosestPlayer(GetPosition(), static_cast<float>(m_SightDistance), false);

	if (Closest != nullptr)
	{
		EventSeePlayer(Closest, a_Chunk);
	}
}





void cMonster::CheckEventLostPlayer(void)
{
	if (GetTarget() != nullptr)
	{
		if ((GetTarget()->GetPosition() - GetPosition()).Length() > m_SightDistance)
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
void cMonster::EventSeePlayer(cEntity * a_SeenPlayer, cChunk & a_Chunk)
{
	UNUSED(a_Chunk);
	ASSERT(a_SeenPlayer->IsPlayer());
	SetTarget(static_cast<cPawn*>(a_SeenPlayer));
}





void cMonster::EventLosePlayer(void)
{
	SetTarget(nullptr);
	m_EMState = IDLE;
}





void cMonster::InStateIdle(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	if (m_PathfinderActivated)
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

			Vector3d Destination(GetPosX() + Dist.x, GetPosition().y, GetPosZ() + Dist.z);

			cChunk * Chunk = a_Chunk.GetNeighborChunk(static_cast<int>(Destination.x), static_cast<int>(Destination.z));
			if ((Chunk == nullptr) || !Chunk->IsValid())
			{
				return;
			}

			BLOCKTYPE BlockType;
			NIBBLETYPE BlockMeta;
			int RelX = static_cast<int>(Destination.x) - Chunk->GetPosX() * cChunkDef::Width;
			int RelZ = static_cast<int>(Destination.z) - Chunk->GetPosZ() * cChunkDef::Width;
			Chunk->GetBlockTypeMeta(RelX, static_cast<int>(Destination.y) - 1, RelZ, BlockType, BlockMeta);
			if (BlockType != E_BLOCK_STATIONARY_WATER)  // Idle mobs shouldn't enter water on purpose
			{
				MoveToPosition(Destination);
			}
		}
	}
}





// What to do if in Chasing State
// This state should always be defined in each child class
void cMonster::InStateChasing(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	UNUSED(a_Dt);
}





// What to do if in Escaping State
void cMonster::InStateEscaping(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	UNUSED(a_Dt);

	if (GetTarget() != nullptr)
	{
		Vector3d newloc = GetPosition();
		newloc.x = (GetTarget()->GetPosition().x < newloc.x)? (newloc.x + m_SightDistance): (newloc.x - m_SightDistance);
		newloc.z = (GetTarget()->GetPosition().z < newloc.z)? (newloc.z + m_SightDistance): (newloc.z - m_SightDistance);
		MoveToPosition(newloc);
	}
	else
	{
		m_EMState = IDLE;  // This shouldnt be required but just to be safe
	}
}





void cMonster::ResetAttackCooldown()
{
	m_AttackCoolDownTicksLeft = static_cast<int>(3 * 20 * m_AttackRate);  // A second has 20 ticks, an attack rate of 1 means 1 hit every 3 seconds
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

	// Search Cuberite name:
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






/** Sets the target. */
void cMonster::SetTarget (cPawn * a_NewTarget)
{
	ASSERT((a_NewTarget == nullptr) || (IsTicking()));
	if (m_Target == a_NewTarget)
	{
		return;
	}
	cPawn * OldTarget = m_Target;
	m_Target = a_NewTarget;

	if (OldTarget != nullptr)
	{
		// Notify the old target that we are no longer targeting it.
		OldTarget->NoLongerTargetingMe(this);
	}

	if (a_NewTarget != nullptr)
	{
		ASSERT(a_NewTarget->IsTicking());
		// Notify the new target that we are now targeting it.
		m_Target->TargetingMe(this);
	}

}





void cMonster::UnsafeUnsetTarget()
{
	m_Target = nullptr;
}





cPawn * cMonster::GetTarget ()
{
	return m_Target;
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
	// If the Y coord is out of range, return the most logical result without considering anything else:
	int RelY = FloorC(a_Location.y);
	if (RelY < 0)
	{
		// Never burn under the world
		return false;
	}
	if (RelY >= cChunkDef::Height)
	{
		// Always burn above the world
		return true;
	}

	cChunk * Chunk = a_Chunk.GetNeighborChunk(FloorC(a_Location.x), FloorC(a_Location.z));
	if ((Chunk == nullptr) || (!Chunk->IsValid()))
	{
		return false;
	}

	int RelX = FloorC(a_Location.x) - Chunk->GetPosX() * cChunkDef::Width;
	int RelZ = FloorC(a_Location.z) - Chunk->GetPosZ() * cChunkDef::Width;

	if (
		(Chunk->GetSkyLight(RelX, RelY, RelZ) == 15) &&             // In the daylight
		(Chunk->GetBlock(RelX, RelY, RelZ) != E_BLOCK_SOULSAND) &&  // Not on soulsand
		(GetWorld()->GetTimeOfDay() < 12000 + 1000) &&              // Daytime
		GetWorld()->IsWeatherSunnyAt(POSX_TOINT, POSZ_TOINT)        // Not raining
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
