
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "IncludeAllMonsters.h"
#include "../BlockInfo.h"
#include "../Root.h"
#include "../Server.h"
#include "../ClientHandle.h"
#include "../Items/ItemHandler.h"
#include "../World.h"
#include "../EffectID.h"
#include "../Entities/Player.h"
#include "../Entities/ExpOrb.h"
#include "../MonsterConfig.h"
#include "../BoundingBox.h"

#include "Items/ItemSpawnEgg.h"

#include "../Chunk.h"
#include "../FastRandom.h"

#include "PathFinder.h"
#include "../Entities/LeashKnot.h"



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
	const char * m_VanillaNameNBT;
} g_MobTypeNames[] =
{
	{mtBat,            "bat",            "Bat",            "bat"},
	{mtBlaze,          "blaze",          "Blaze",          "blaze"},
	{mtCaveSpider,     "cavespider",     "CaveSpider",     "cave_spider"},
	{mtChicken,        "chicken",        "Chicken",        "chicken"},
	{mtCow,            "cow",            "Cow",            "cow"},
	{mtCreeper,        "creeper",        "Creeper",        "creeper"},
	{mtEnderman,       "enderman",       "Enderman",       "enderman"},
	{mtEnderDragon,    "enderdragon",    "EnderDragon",    "ender_dragon"},
	{mtGhast,          "ghast",          "Ghast",          "ghast"},
	{mtGiant,          "giant",          "Giant",          "giant"},
	{mtGuardian,       "guardian",       "Guardian",       "guardian"},
	{mtHorse,          "horse",          "EntityHorse",    "horse"},
	{mtIronGolem,      "irongolem",      "VillagerGolem",  "iron_golem"},
	{mtMagmaCube,      "magmacube",      "LavaSlime",      "magma_cube"},
	{mtMooshroom,      "mooshroom",      "MushroomCow",    "mooshroom"},
	{mtOcelot,         "ocelot",         "Ozelot",         "ocelot"},
	{mtPig,            "pig",            "Pig",            "pig"},
	{mtRabbit,         "rabbit",         "Rabbit",         "rabbit"},
	{mtSheep,          "sheep",          "Sheep",          "sheep"},
	{mtSilverfish,     "silverfish",     "Silverfish",     "silverfish"},
	{mtSkeleton,       "skeleton",       "Skeleton",       "skeleton"},
	{mtSlime,          "slime",          "Slime",          "slime"},
	{mtSnowGolem,      "snowgolem",      "SnowMan",        "snow_golem"},
	{mtSpider,         "spider",         "Spider",         "spider"},
	{mtSquid,          "squid",          "Squid",          "squid"},
	{mtVillager,       "villager",       "Villager",       "villager"},
	{mtWitch,          "witch",          "Witch",          "witch"},
	{mtWither,         "wither",         "WitherBoss",     "wither"},
	{mtWitherSkeleton, "witherskeleton", "WitherSkeleton", "wither_skeleton"},
	{mtWolf,           "wolf",           "Wolf",           "wolf"},
	{mtZombie,         "zombie",         "Zombie",         "zombie"},
	{mtZombiePigman,   "zombiepigman",   "PigZombie",      "zombie_pigman"},
	{mtZombieVillager, "zombievillager", "ZombieVillager", "zombie_villager"},
} ;





////////////////////////////////////////////////////////////////////////////////
// cMonster:

cMonster::cMonster(const AString & a_ConfigName, eMonsterType a_MobType, const AString & a_SoundHurt, const AString & a_SoundDeath, const AString & a_SoundAmbient, double a_Width, double a_Height)
	: Super(etMonster, a_Width, a_Height)
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
	, m_SoundAmbient(a_SoundAmbient)
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
	, m_AgingTimer(TPS * 60 * 20)  // about 20 minutes
	, m_WasLastTargetAPlayer(false)
	, m_LeashedTo(nullptr)
	, m_LeashToPos(nullptr)
	, m_IsLeashActionJustDone(false)
	, m_CanBeLeashed(GetMobFamily() == eFamily::mfPassive)
	, m_LovePartner(nullptr)
	, m_LoveTimer(0)
	, m_LoveCooldown(0)
	, m_MatingTimer(0)
	, m_Target(nullptr)
{
	if (!a_ConfigName.empty())
	{
		GetMonsterConfig(a_ConfigName);
	}

	// Prevent mobs spawning at the same time from making sounds simultaneously
	m_AmbientSoundTimer = GetRandomProvider().RandInt(0, 100);
}





cMonster::~cMonster()
{
	ASSERT(GetTarget() == nullptr);
}





void cMonster::OnRemoveFromWorld(cWorld & a_World)
{
	if (IsLeashed())
	{
		cEntity * LeashedTo = GetLeashedTo();
		Unleash(false, true);

		// Remove leash knot if there are no more mobs leashed to
		if (!LeashedTo->HasAnyMobLeashed() && LeashedTo->IsLeashKnot())
		{
			LeashedTo->Destroy();
		}
	}

	Super::OnRemoveFromWorld(a_World);
}





void cMonster::Destroyed()
{
	SetTarget(nullptr);  // Tell them we're no longer targeting them.
	if (m_LovePartner != nullptr)
	{
		m_LovePartner->ResetLoveMode();
	}
	Super::Destroyed();
}





void cMonster::SpawnOn(cClientHandle & a_Client)
{
	a_Client.SendSpawnMob(*this);

	if (IsLeashed())
	{
		a_Client.SendLeashEntity(*this, *this->GetLeashedTo());
	}
}





void cMonster::MoveToWayPoint(cChunk & a_Chunk)
{
	if ((m_NextWayPointPosition - GetPosition()).SqrLength() < WAYPOINT_RADIUS * WAYPOINT_RADIUS)
	{
		return;
	}

	if (m_JumpCoolDown <= 0)
	{
		if (DoesPosYRequireJump(FloorC(m_NextWayPointPosition.y)))
		{
			if (
				(IsOnGround() && (GetSpeed().SqrLength() <= 0.5)) ||  // If walking on the ground, we need to slow down first, otherwise we miss the jump
				IsInWater()
			)
			{
				m_bOnGround = false;
				m_JumpCoolDown = 20;
				AddPosY(1.6);  // Jump!!
				SetSpeedY(1);
				SetSpeedX(3.2 * (m_NextWayPointPosition.x - GetPosition().x));  // Move forward in a preset speed.
				SetSpeedZ(3.2 * (m_NextWayPointPosition.z - GetPosition().z));  // The numbers were picked based on trial and error
			}
		}
	}
	else
	{
		--m_JumpCoolDown;
	}

	Vector3d Distance = m_NextWayPointPosition - GetPosition();
	if ((std::abs(Distance.x) > 0.05) || (std::abs(Distance.z) > 0.05))
	{
		Distance.y = 0;
		Distance.Normalize();

		if (m_bOnGround)
		{
			Distance *= 2.5f;
		}
		else if (IsInWater())
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
	Super::Tick(a_Dt, a_Chunk);
	if (!IsTicking())
	{
		// The base class tick destroyed us
		return;
	}
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
			Destroy();
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
			if (!static_cast<cPlayer *>(GetTarget())->CanMobsTarget())
			{
				SetTarget(nullptr);
				m_EMState = IDLE;
			}
		}
	}

	// Process the undead burning in daylight.
	HandleDaylightBurning(*Chunk, WouldBurnAt(GetPosition(), *Chunk));

	bool a_IsFollowingPath = false;
	if (m_PathfinderActivated && (GetMobType() != mtGhast))  // Pathfinder is currently disabled for ghasts, which have their own flying mechanism
	{
		if (ReachedFinalDestination() || (m_LeashToPos != nullptr))
		{
			StopMovingToPosition();  // Simply sets m_PathfinderActivated to false.
		}
		else
		{
			// Note that m_NextWayPointPosition is actually returned by GetNextWayPoint)
			switch (m_PathFinder.GetNextWayPoint(*Chunk, GetPosition(), &m_FinalDestination, &m_NextWayPointPosition, m_EMState == IDLE))
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

	// Leash calculations
	CalcLeashActions(a_Dt);

	BroadcastMovementUpdate();

	// Ambient mob sounds
	if (!m_SoundAmbient.empty() && (--m_AmbientSoundTimer <= 0))
	{
		auto & Random = GetRandomProvider();
		auto ShouldPlaySound = Random.RandBool();
		if (ShouldPlaySound)
		{
			auto SoundPitchMultiplier = 1.0f + (Random.RandReal(1.0f) - Random.RandReal(1.0f)) * 0.2f;
			m_World->BroadcastSoundEffect(m_SoundAmbient, GetPosition(), 1.0f, SoundPitchMultiplier * 1.0f);
		}
		m_AmbientSoundTimer = 100;
	}

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





void cMonster::CalcLeashActions(std::chrono::milliseconds a_Dt)
{
	// This mob just spotted in the world and [m_LeashToPos not null] shows that should be leashed to a leash knot at m_LeashToPos.
	// This keeps trying until knot is found. Leash knot may be in a different chunk that needn't or can't be loaded yet.
	if (!IsLeashed() && (m_LeashToPos != nullptr))
	{
		auto LeashKnot = cLeashKnot::FindKnotAtPos(*m_World, { FloorC(m_LeashToPos->x), FloorC(m_LeashToPos->y), FloorC(m_LeashToPos->z) });
		if (LeashKnot != nullptr)
		{
			LeashTo(*LeashKnot);
			SetLeashToPos(nullptr);
		}
	}

	if (!IsLeashed())
	{
		return;
	}

	static const double CloseFollowDistance = 1.8;   // The closest the mob will path towards the leashed to entity
	static const double LeashNaturalLength  = 5.0;   // The closest the mob is actively pulled towards the entity
	static const double LeashMaximumLength  = 10.0;  // Length where the leash breaks
	static const double LeashSpringConstant = 20.0;  // How stiff the leash is

	const auto LeashedToPos = m_LeashedTo->GetPosition();
	const auto Displacement = LeashedToPos - GetPosition();
	const auto Distance = Displacement.Length();
	const auto Direction = Displacement.NormalizeCopy();

	// If the leash is over-extended, break the leash:
	if (Distance > LeashMaximumLength)
	{
		LOGD("Leash broken (distance)");
		Unleash(false);
		return;
	}

	// If the mob isn't following close enough, pull the mob towards the leashed to entity:
	if (Distance > LeashNaturalLength)
	{
		// Accelerate monster towards the leashed to entity:
		const auto Extension = Distance - LeashNaturalLength;
		auto Acceleration = Direction * (Extension * LeashSpringConstant);

		// Stop mobs from floating up when on the ground
		if (IsOnGround() && (Acceleration.y < std::abs(GetGravity())))
		{
			Acceleration.y = 0.0;
		}

		// Apply the acceleration
		using namespace std::chrono;
		AddSpeed(Acceleration * duration_cast<duration<double>>(a_Dt).count());
	}

	// Passively follow the leashed to entity:
	if (Distance > CloseFollowDistance)
	{
		const Vector3d TargetBlock((LeashedToPos - Direction * CloseFollowDistance).Floor());
		// Move to centre of target block face
		MoveToPosition(TargetBlock + Vector3d{ 0.5, 0.0, 0.5 });
	}
}





void cMonster::SetPitchAndYawFromDestination(bool a_IsFollowingPath)
{
	Vector3d BodyDistance;
	if (!a_IsFollowingPath)
	{
		if (GetTarget() == nullptr)
		{
			// Avoid dirtying head position when nothing will change
			// Thus avoids creating unnecessary network traffic
			return;
		}

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
	Super::HandleFalling();
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
	if (!Super::DoTakeDamage(a_TDI))
	{
		return false;
	}

	if (!m_SoundHurt.empty() && (m_Health > 0))
	{
		m_World->BroadcastSoundEffect(m_SoundHurt, GetPosition(), 1.0f, 0.8f);
	}

	if ((a_TDI.Attacker != nullptr) && a_TDI.Attacker->IsPawn())
	{
		if (
			(!a_TDI.Attacker->IsPlayer()) ||
			(static_cast<cPlayer *>(a_TDI.Attacker)->CanMobsTarget())
		)
		{
			SetTarget(static_cast<cPawn*>(a_TDI.Attacker));
		}
		m_TicksSinceLastDamaged = 0;
	}
	return true;
}





void cMonster::DoMoveToWorld(const cEntity::sWorldChangeInfo & a_WorldChangeInfo)
{
	// Stop all mobs from targeting this entity
	// Stop this entity from targeting other mobs
	SetTarget(nullptr);
	StopEveryoneFromTargetingMe();

	Super::DoMoveToWorld(a_WorldChangeInfo);
}





void cMonster::KilledBy(TakeDamageInfo & a_TDI)
{
	Super::KilledBy(a_TDI);
	if (m_SoundHurt != "")
	{
		m_World->BroadcastSoundEffect(m_SoundDeath, GetPosition(), 1.0f, 0.8f);
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
			Reward = GetRandomProvider().RandInt(1, 3);
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
		case mtWitherSkeleton:
		case mtZombie:
		case mtZombiePigman:
		case mtZombieVillager:
		case mtSlime:
		case mtMagmaCube:
		{
			Reward = GetRandomProvider().RandInt(6, 8);
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
		m_World->SpawnSplitExperienceOrbs(GetPosX(), GetPosY(), GetPosZ(), Reward);
	}
	m_DestroyTimer = std::chrono::milliseconds(0);
}





void cMonster::OnRightClicked(cPlayer & a_Player)
{
	Super::OnRightClicked(a_Player);

	const cItem & EquippedItem = a_Player.GetEquippedItem();
	if ((EquippedItem.m_ItemType == E_ITEM_NAME_TAG) && !EquippedItem.m_CustomName.empty())
	{
		SetCustomName(EquippedItem.m_CustomName);
		if (!a_Player.IsGameModeCreative())
		{
			a_Player.GetInventory().RemoveOneEquippedItem();
		}
	}

	// Using leashes
	m_IsLeashActionJustDone = false;
	if (IsLeashed() && (GetLeashedTo() == &a_Player))  // a player can only unleash a mob leashed to him
	{
		Unleash(!a_Player.IsGameModeCreative());
	}
	else if (IsLeashed())
	{
		// Mob is already leashed but client anticipates the server action and draws a leash link, so we need to send current leash to cancel it
		m_World->BroadcastLeashEntity(*this, *this->GetLeashedTo());
	}
	else if (CanBeLeashed() && (EquippedItem.m_ItemType == E_ITEM_LEASH))
	{
		if (!a_Player.IsGameModeCreative())
		{
			a_Player.GetInventory().RemoveOneEquippedItem();
		}
		LeashTo(a_Player);
	}
}





// Checks to see if EventSeePlayer should be fired
// monster sez: Do I see the player
void cMonster::CheckEventSeePlayer(cChunk & a_Chunk)
{
	m_World->DoWithNearestPlayer(GetPosition(), static_cast<float>(m_SightDistance), [&](cPlayer & a_Player) -> bool
	{
		EventSeePlayer(&a_Player, a_Chunk);
		return true;
	}, false);
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
void cMonster::EventSeePlayer(cPlayer * a_SeenPlayer, cChunk & a_Chunk)
{
	UNUSED(a_Chunk);
	SetTarget(a_SeenPlayer);
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
		auto & Random = GetRandomProvider();

		// At this interval the results are predictable
		int rem = Random.RandInt(1, 7);
		m_IdleInterval -= std::chrono::seconds(1);  // So nothing gets dropped when the server hangs for a few seconds

		Vector3d Dist;
		Dist.x = static_cast<double>(Random.RandInt(-5, 5));
		Dist.z = static_cast<double>(Random.RandInt(-5, 5));

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
			int YBelowUs = static_cast<int>(Destination.y) - 1;
			if (YBelowUs >= 0)
			{
				Chunk->GetBlockTypeMeta(RelX, YBelowUs, RelZ, BlockType, BlockMeta);
				if (BlockType != E_BLOCK_STATIONARY_WATER)  // Idle mobs shouldn't enter water on purpose
				{
					MoveToPosition(Destination);
				}
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
	m_AttackCoolDownTicksLeft = static_cast<int>(TPS * m_AttackRate);  // A second has 20 ticks, an attack rate of 1 means 1 hit every second
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





AString cMonster::MobTypeToVanillaNBT(eMonsterType a_MobType)
{
	// Mob types aren't sorted, so we need to search linearly:
	for (size_t i = 0; i < ARRAYCOUNT(g_MobTypeNames); i++)
	{
		if (g_MobTypeNames[i].m_Type == a_MobType)
		{
			return g_MobTypeNames[i].m_VanillaNameNBT;
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

	// Search in NBT name
	for (size_t i = 0; i < ARRAYCOUNT(g_MobTypeNames); i++)
	{
		if (strcmp(StrToLower(g_MobTypeNames[i].m_VanillaNameNBT).c_str(), lcName.c_str()) == 0)
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
		case mtBat:            return mfAmbient;
		case mtBlaze:          return mfHostile;
		case mtCaveSpider:     return mfHostile;
		case mtChicken:        return mfPassive;
		case mtCow:            return mfPassive;
		case mtCreeper:        return mfHostile;
		case mtEnderDragon:    return mfNoSpawn;
		case mtEnderman:       return mfHostile;
		case mtGhast:          return mfHostile;
		case mtGiant:          return mfNoSpawn;
		case mtGuardian:       return mfWater;  // Just because they have special spawning conditions. If Watertemples have been added, this needs to be edited!
		case mtHorse:          return mfPassive;
		case mtIronGolem:      return mfPassive;
		case mtMagmaCube:      return mfHostile;
		case mtMooshroom:      return mfPassive;
		case mtOcelot:         return mfPassive;
		case mtPig:            return mfPassive;
		case mtRabbit:         return mfPassive;
		case mtSheep:          return mfPassive;
		case mtSilverfish:     return mfHostile;
		case mtSkeleton:       return mfHostile;
		case mtSlime:          return mfHostile;
		case mtSnowGolem:      return mfNoSpawn;
		case mtSpider:         return mfHostile;
		case mtSquid:          return mfWater;
		case mtVillager:       return mfPassive;
		case mtWitch:          return mfHostile;
		case mtWither:         return mfNoSpawn;
		case mtWitherSkeleton: return mfHostile;
		case mtWolf:           return mfPassive;
		case mtZombie:         return mfHostile;
		case mtZombiePigman:   return mfHostile;
		case mtZombieVillager: return mfHostile;

		default:
		{
			ASSERT(!"Unhandled mob type");
			return mfUnhandled;
		}
	}
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
		default:
		{
			ASSERT(!"Unhandled mob family");
			return -1;
		}
	}
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
		m_WasLastTargetAPlayer = m_Target->IsPlayer();
	}

}





void cMonster::UnsafeUnsetTarget()
{
	m_Target = nullptr;
}





cPawn * cMonster::GetTarget()
{
	return m_Target;
}





std::unique_ptr<cMonster> cMonster::NewMonsterFromType(eMonsterType a_MobType)
{
	auto & Random = GetRandomProvider();

	// Create the mob entity
	switch (a_MobType)
	{
		case mtMagmaCube:
		{
			return std::make_unique<cMagmaCube>(1 << Random.RandInt(2));  // Size 1, 2 or 4
		}
		case mtSlime:
		{
			return std::make_unique<cSlime>(1 << Random.RandInt(2));  // Size 1, 2 or 4
		}
		case mtVillager: return std::make_unique<cVillager>(cVillager::GetRandomProfession());
		case mtHorse:
		{
			// Horses take a type (species), a colour, and a style (dots, stripes, etc.)
			int HorseType = Random.RandInt(7);
			int HorseColor = Random.RandInt(6);
			int HorseStyle = Random.RandInt(4);
			int HorseTameTimes = Random.RandInt(1, 6);

			if ((HorseType == 5) || (HorseType == 6) || (HorseType == 7))
			{
				// Increase chances of normal horse (zero)
				HorseType = 0;
			}

			return std::make_unique<cHorse>(HorseType, HorseColor, HorseStyle, HorseTameTimes);
		}
		case mtZombieVillager:
		{
			return std::make_unique<cZombieVillager>(cVillager::GetRandomProfession());
		}
		case mtBat:            return std::make_unique<cBat>();
		case mtBlaze:          return std::make_unique<cBlaze>();
		case mtCaveSpider:     return std::make_unique<cCaveSpider>();
		case mtChicken:        return std::make_unique<cChicken>();
		case mtCow:            return std::make_unique<cCow>();
		case mtCreeper:        return std::make_unique<cCreeper>();
		case mtEnderDragon:    return std::make_unique<cEnderDragon>();
		case mtEnderman:       return std::make_unique<cEnderman>();
		case mtGhast:          return std::make_unique<cGhast>();
		case mtGiant:          return std::make_unique<cGiant>();
		case mtGuardian:       return std::make_unique<cGuardian>();
		case mtIronGolem:      return std::make_unique<cIronGolem>();
		case mtMooshroom:      return std::make_unique<cMooshroom>();
		case mtOcelot:         return std::make_unique<cOcelot>();
		case mtPig:            return std::make_unique<cPig>();
		case mtRabbit:         return std::make_unique<cRabbit>();
		case mtSheep:          return std::make_unique<cSheep>();
		case mtSilverfish:     return std::make_unique<cSilverfish>();
		case mtSkeleton:       return std::make_unique<cSkeleton>();
		case mtSnowGolem:      return std::make_unique<cSnowGolem>();
		case mtSpider:         return std::make_unique<cSpider>();
		case mtSquid:          return std::make_unique<cSquid>();
		case mtWitch:          return std::make_unique<cWitch>();
		case mtWither:         return std::make_unique<cWither>();
		case mtWitherSkeleton: return std::make_unique<cWitherSkeleton>();
		case mtWolf:           return std::make_unique<cWolf>();
		case mtZombie:         return std::make_unique<cZombie>();
		case mtZombiePigman:   return std::make_unique<cZombiePigman>();
		default:
		{
			ASSERT(!"Unhandled mob type whilst trying to spawn mob!");
			return nullptr;
		}
	}
}





void cMonster::EngageLoveMode(cMonster *a_Partner)
{
	m_LovePartner = a_Partner;
	m_MatingTimer = 50;  // about 3 seconds of mating
}





void cMonster::ResetLoveMode()
{
	m_LovePartner = nullptr;
	m_LoveTimer = 0;
	m_MatingTimer = 0;
	m_LoveCooldown = TPS * 60 * 5;  // 5 minutes

	// when an animal is in love mode, the client only stops sending the hearts if we let them know it's in cooldown, which is done with the "age" metadata
	m_World->BroadcastEntityMetadata(*this);
}





void cMonster::LoveTick(void)
{
	// if we have a partner, mate
	if (m_LovePartner != nullptr)
	{

		if (m_MatingTimer > 0)
		{
			// If we should still mate, keep bumping into them until baby is made
			Vector3d Pos = m_LovePartner->GetPosition();
			MoveToPosition(Pos);
		}
		else
		{
			// Mating finished. Spawn baby
			Vector3f Pos = (GetPosition() + m_LovePartner->GetPosition()) * 0.5;
			UInt32 BabyID = m_World->SpawnMob(Pos.x, Pos.y, Pos.z, GetMobType(), true);

			cMonster * Baby = nullptr;

			m_World->DoWithEntityByID(BabyID, [&](cEntity & a_Entity)
			{
				Baby = static_cast<cMonster *>(&a_Entity);
				return true;
			});

			if (Baby != nullptr)
			{
				Baby->InheritFromParents(this, m_LovePartner);
			}

			m_World->SpawnExperienceOrb(Pos.x, Pos.y, Pos.z, GetRandomProvider().RandInt(1, 6));

			m_World->DoWithPlayerByUUID(m_Feeder, [&] (cPlayer & a_Player)
			{
				a_Player.GetStatManager().AddValue(Statistic::AnimalsBred);
				if (GetMobType() == eMonsterType::mtCow)
				{
					a_Player.AwardAchievement(Statistic::AchBreedCow);
				}
				return true;
			});
			m_LovePartner->ResetLoveMode();
			ResetLoveMode();
		}
	}
	else
	{
		// We have no partner, so we just chase the player if they have our breeding item
		cItems FollowedItems;
		GetFollowedItems(FollowedItems);
		if (FollowedItems.Size() > 0)
		{
			m_World->DoWithNearestPlayer(GetPosition(), static_cast<float>(m_SightDistance), [&](cPlayer & a_Player) -> bool
			{
				const cItem & EquippedItem = a_Player.GetEquippedItem();
				if (FollowedItems.ContainsType(EquippedItem))
				{
					Vector3d PlayerPos = a_Player.GetPosition();
					MoveToPosition(PlayerPos);
				}

				return true;
			});
		}
	}

	// If we are in love mode but we have no partner, search for a partner neabry
	if (m_LoveTimer > 0)
	{
		if (m_LovePartner == nullptr)
		{
			m_World->ForEachEntityInBox(cBoundingBox(GetPosition(), 8, 8), [=](cEntity & a_Entity)
			{
				// If the entity is not a monster, don't breed with it
				// Also, do not self-breed
				if ((a_Entity.GetEntityType() != etMonster) || (&a_Entity == this))
				{
					return false;
				}

				auto & Me = static_cast<cMonster &>(*this);
				auto & PotentialPartner = static_cast<cMonster &>(a_Entity);

				// If the potential partner is not of the same species, don't breed with it
				if (PotentialPartner.GetMobType() != Me.GetMobType())
				{
					return false;
				}

				// If the potential partner is not in love
				// Or they already have a mate, do not breed with them
				if ((!PotentialPartner.IsInLove()) || (PotentialPartner.GetPartner() != nullptr))
				{
					return false;
				}

				// All conditions met, let's breed!
				PotentialPartner.EngageLoveMode(&Me);
				Me.EngageLoveMode(&PotentialPartner);
				return true;
			});
		}

		m_LoveTimer--;
	}
	if (m_MatingTimer > 0)
	{
		m_MatingTimer--;
	}
	if (m_LoveCooldown > 0)
	{
		m_LoveCooldown--;
	}
}





void cMonster::RightClickFeed(cPlayer & a_Player)
{

	const cItem & EquippedItem = a_Player.GetEquippedItem();

	// If a player holding breeding items right-clicked me, go into love mode
	if ((m_LoveCooldown == 0) && !IsInLove() && !IsBaby())
	{
		cItems Items;
		GetBreedingItems(Items);
		if (Items.ContainsType(EquippedItem.m_ItemType))
		{
			if (!a_Player.IsGameModeCreative())
			{
				a_Player.GetInventory().RemoveOneEquippedItem();
			}
			m_LoveTimer = TPS * 30;  // half a minute
			m_World->BroadcastEntityStatus(*this, esMobInLove);
		}
	}
	// If a player holding my spawn egg right-clicked me, spawn a new baby
	if (EquippedItem.m_ItemType == E_ITEM_SPAWN_EGG)
	{
		eMonsterType MonsterType = cItemSpawnEggHandler::ItemDamageToMonsterType(EquippedItem.m_ItemDamage);
		if (
			(MonsterType == m_MobType) &&
			(m_World->SpawnMob(GetPosX(), GetPosY(), GetPosZ(), m_MobType, true) != cEntity::INVALID_ID)  // Spawning succeeded
			)
		{
			if (!a_Player.IsGameModeCreative())
			{
				// The mob was spawned, "use" the item:
				a_Player.GetInventory().RemoveOneEquippedItem();
			}
		}
	}
	// Stores feeder UUID for statistic tracking
	m_Feeder = a_Player.GetUUID();
}





void cMonster::AddRandomDropItem(cItems & a_Drops, unsigned int a_Min, unsigned int a_Max, short a_Item, short a_ItemHealth)
{
	auto Count = GetRandomProvider().RandInt<unsigned int>(a_Min, a_Max);
	auto MaxStackSize = static_cast<unsigned char>(ItemHandler(a_Item)->GetMaxStackSize());
	while (Count > MaxStackSize)
	{
		a_Drops.emplace_back(a_Item, MaxStackSize, a_ItemHealth);
		Count -= MaxStackSize;
	}
	if (Count > 0)
	{
		a_Drops.emplace_back(a_Item, Count, a_ItemHealth);
	}
}





void cMonster::AddRandomUncommonDropItem(cItems & a_Drops, float a_Chance, short a_Item, short a_ItemHealth)
{
	if (GetRandomProvider().RandBool(a_Chance / 100.0))
	{
		a_Drops.push_back(cItem(a_Item, 1, a_ItemHealth));
	}
}





void cMonster::AddRandomRareDropItem(cItems & a_Drops, cItems & a_Items, unsigned int a_LootingLevel)
{
	auto & r1 = GetRandomProvider();
	if (r1.RandBool((5 + a_LootingLevel) / 200.0))
	{
		size_t Rare = r1.RandInt<size_t>(a_Items.Size() - 1);
		a_Drops.push_back(a_Items.at(Rare));
	}
}





void cMonster::AddRandomArmorDropItem(cItems & a_Drops, unsigned int a_LootingLevel)
{
	auto & r1 = GetRandomProvider();

	double LootingBonus = a_LootingLevel / 100.0;

	if (r1.RandBool(m_DropChanceHelmet + LootingBonus))
	{
		if (!GetEquippedHelmet().IsEmpty())
		{
			a_Drops.push_back(GetEquippedHelmet());
		}
	}

	if (r1.RandBool(m_DropChanceChestplate + LootingBonus))
	{
		if (!GetEquippedChestplate().IsEmpty())
		{
			a_Drops.push_back(GetEquippedChestplate());
		}
	}

	if (r1.RandBool(m_DropChanceLeggings + LootingBonus))
	{
		if (!GetEquippedLeggings().IsEmpty())
		{
			a_Drops.push_back(GetEquippedLeggings());
		}
	}

	if (r1.RandBool(m_DropChanceBoots + LootingBonus))
	{
		if (!GetEquippedBoots().IsEmpty())
		{
			a_Drops.push_back(GetEquippedBoots());
		}
	}
}





void cMonster::AddRandomWeaponDropItem(cItems & a_Drops, unsigned int a_LootingLevel)
{
	if (GetRandomProvider().RandBool(m_DropChanceWeapon + (a_LootingLevel / 100.0)))
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
	if (RelY >= cChunkDef::Height)
	{
		// Always burn above the world
		return true;
	}
	if (RelY <= 0)
	{
		// The mob is about to die, no point in burning
		return false;
	}

	PREPARE_REL_AND_CHUNK(a_Location, a_Chunk);
	if (!RelSuccess)
	{
		return false;
	}

	if (
		(Chunk->GetBlock(Rel.x, Rel.y, Rel.z) != E_BLOCK_SOULSAND) &&  // Not on soulsand
		(GetWorld()->GetTimeOfDay() < 12000 + 1000) &&              // Daytime
		GetWorld()->IsWeatherSunnyAt(POSX_TOINT, POSZ_TOINT) &&     // Not raining
		!IsInWater()                                                // Isn't swimming
	)
	{
		int MobHeight = CeilC(a_Location.y + GetHeight()) - 1;  // The block Y coord of the mob's head
		if (MobHeight >= cChunkDef::Height)
		{
			return true;
		}
		// Start with the highest block and scan down to just above the mob's head.
		// If a non transparent is found, return false (do not burn). Otherwise return true.
		// Note that this loop is not a performance concern as transparent blocks are rare and the loop almost always bailes out
		// instantly.(An exception is e.g. standing under a long column of glass).
		int CurrentBlock = Chunk->GetHeight(Rel.x, Rel.z);
		while (CurrentBlock > MobHeight)
		{
			BLOCKTYPE Block = Chunk->GetBlock(Rel.x, CurrentBlock, Rel.z);
			if (
				// Do not burn if a block above us meets one of the following conditions:
				(!cBlockInfo::IsTransparent(Block)) ||
				(Block == E_BLOCK_LEAVES) ||
				(Block == E_BLOCK_NEW_LEAVES) ||
				(IsBlockWater(Block))
			)
			{
				return false;
			}
			--CurrentBlock;
		}
		return true;

	}
	return false;
}





cMonster::eFamily cMonster::GetMobFamily(void) const
{
	return FamilyFromType(m_MobType);
}





void cMonster::LeashTo(cEntity & a_Entity, bool a_ShouldBroadcast)
{
	// Do nothing if already leashed
	if (m_LeashedTo != nullptr)
	{
		return;
	}

	m_LeashedTo = &a_Entity;

	a_Entity.AddLeashedMob(this);

	if (a_ShouldBroadcast)
	{
		m_World->BroadcastLeashEntity(*this, a_Entity);
	}

	m_IsLeashActionJustDone = true;
}





void cMonster::Unleash(bool a_ShouldDropLeashPickup, bool a_ShouldBroadcast)
{
	// Do nothing if not leashed
	if (m_LeashedTo == nullptr)
	{
		return;
	}

	m_LeashedTo->RemoveLeashedMob(this);

	m_LeashedTo = nullptr;

	if (a_ShouldDropLeashPickup)
	{
		cItems Pickups;
		Pickups.Add(cItem(E_ITEM_LEASH, 1, 0));
		GetWorld()->SpawnItemPickups(Pickups, GetPosX() + 0.5, GetPosY() + 0.5, GetPosZ() + 0.5);
	}

	if (a_ShouldBroadcast)
	{
		m_World->BroadcastUnleashEntity(*this);
	}

	m_IsLeashActionJustDone = true;
}





void cMonster::Unleash(bool a_ShouldDropLeashPickup)
{
	Unleash(a_ShouldDropLeashPickup, true);
}
