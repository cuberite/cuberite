
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "IncludeAllMonsters.h"
#include "../Root.h"
#include "../Server.h"
#include "../ClientHandle.h"
#include "../Items/ItemHandler.h"
#include "../World.h"
#include "../EffectID.h"
#include "../Entities/Player.h"
#include "../Entities/ExpOrb.h"
#include "../MonsterConfig.h"
#include "BoundingBox.h"

#include "../Chunk.h"
#include "../FastRandom.h"

#include "PathFinder.h"
#include "../Entities/LeashKnot.h"

// Temporary pathfinder hack
#include "Behaviors/BehaviorDayLightBurner.h"




// Ticks to wait to do leash calculations
#define LEASH_ACTIONS_TICK_STEP 10



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
	{mtBat,          "bat",          "Bat",             "bat"},
	{mtBlaze,        "blaze",        "Blaze",           "blaze"},
	{mtCaveSpider,   "cavespider",   "CaveSpider",      "cave_spider"},
	{mtChicken,      "chicken",      "Chicken",         "chicken"},
	{mtCow,          "cow",          "Cow",             "cow"},
	{mtCreeper,      "creeper",      "Creeper",         "creeper"},
	{mtEnderman,     "enderman",     "Enderman",        "enderman"},
	{mtEnderDragon,  "enderdragon",  "EnderDragon",     "ender_dragon"},
	{mtGhast,        "ghast",        "Ghast",           "ghast"},
	{mtGiant,        "giant",        "Giant",           "giant"},
	{mtGuardian,     "guardian",     "Guardian",        "guardian"},
	{mtHorse,        "horse",        "EntityHorse",     "horse"},
	{mtIronGolem,    "irongolem",    "VillagerGolem",   "iron_golem"},
	{mtMagmaCube,    "magmacube",    "LavaSlime",       "magma_cube"},
	{mtMooshroom,    "mooshroom",    "MushroomCow",     "mooshroom"},
	{mtOcelot,       "ocelot",       "Ozelot",          "ocelot"},
	{mtPig,          "pig",          "Pig",             "pig"},
	{mtRabbit,       "rabbit",       "Rabbit",          "rabbit"},
	{mtSheep,        "sheep",        "Sheep",           "sheep"},
	{mtSilverfish,   "silverfish",   "Silverfish",      "silverfish"},
	{mtSkeleton,     "skeleton",     "Skeleton",        "skeleton"},
	{mtSlime,        "slime",        "Slime",           "slime"},
	{mtSnowGolem,    "snowgolem",    "SnowMan",         "snow_golem"},
	{mtSpider,       "spider",       "Spider",          "spider"},
	{mtSquid,        "squid",        "Squid",           "squid"},
	{mtVillager,     "villager",     "Villager",        "villager"},
	{mtWitch,        "witch",        "Witch",           "witch"},
	{mtWither,       "wither",       "WitherBoss",      "wither"},
	{mtWolf,         "wolf",         "Wolf",            "wolf"},
	{mtZombie,       "zombie",       "Zombie",          "zombie"},
	{mtZombiePigman, "zombiepigman", "PigZombie",       "zombie_pigman"},
} ;





////////////////////////////////////////////////////////////////////////////////
// cMonster:

cMonster::cMonster(const AString & a_ConfigName, eMonsterType a_MobType, const AString & a_SoundHurt, const AString & a_SoundDeath, double a_Width, double a_Height)
	: super(etMonster, a_Width, a_Height)
	, m_EMPersonality(AGGRESSIVE)
	, m_NearestPlayerIsStale(true)
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
	, m_SightDistance(25)
	, m_DropChanceWeapon(0.085f)
	, m_DropChanceHelmet(0.085f)
	, m_DropChanceChestplate(0.085f)
	, m_DropChanceLeggings(0.085f)
	, m_DropChanceBoots(0.085f)
	, m_CanPickUpLoot(true)
	, m_RelativeWalkSpeed(1)
	, m_Age(1)
	, m_AgingTimer(20 * 60 * 20)  // about 20 minutes
	, m_WasLastTargetAPlayer(false)
	, m_LeashedTo(nullptr)
	, m_LeashToPos(nullptr)
	, m_IsLeashActionJustDone(false)
	, m_CanBeLeashed(GetMobFamily() == eFamily::mfPassive)
	, m_LookingAt(nullptr)
	, m_CurrentTickControllingBehavior(nullptr)
	, m_NewTickControllingBehavior(nullptr)
	, m_TickControllingBehaviorState(Normal)
{
	if (!a_ConfigName.empty())
	{
		GetMonsterConfig(a_ConfigName);
	}
}





cMonster::~cMonster()
{

}





void cMonster::Destroy(bool a_ShouldBroadcast)
{
	//mobtodo Destroy vs Destroyed

	// mobTodo behavior for leash
	if (IsLeashed())
	{
		cEntity * LeashedTo = GetLeashedTo();
		Unleash(false, a_ShouldBroadcast);

		// Remove leash knot if there are no more mobs leashed to
		if (!LeashedTo->HasAnyMobLeashed() && LeashedTo->IsLeashKnot())
		{
			LeashedTo->Destroy();
		}
	}

	super::Destroy(a_ShouldBroadcast);
}





void cMonster::Destroyed()
{
	for (cBehavior * Behavior : m_AttachedDestroyBehaviors)
	{
		Behavior->Destroyed();
	}

	super::Destroyed();
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
	UNUSED(a_Chunk);
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
				IsSwimming()
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

	// Speed up leashed mobs getting far from player
	if (IsLeashed() && GetLeashedTo()->IsPlayer())
	{
		Distance = GetLeashedTo()->GetPosition() - GetPosition();
		Distance.Normalize();
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
	// LOGD("mobDebug - Monster tick begins");
	m_NearestPlayerIsStale = true;
	super::Tick(a_Dt, a_Chunk);
	if (!IsTicking())
	{
		// The base class tick destroyed us
		return;
	}
	GET_AND_VERIFY_CURRENT_CHUNK(Chunk, POSX_TOINT, POSZ_TOINT);

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

	// All behaviors can execute PostTick and PreTick.
	// These are for bookkeeping or passive actions like laying eggs.
	// They MUST NOT control mob movement or interefere with the main Tick.
	for (cBehavior * Behavior : m_AttachedPreTickBehaviors)
	{
		// LOGD("mobDebug - preTick");
		ASSERT(Behavior != nullptr);
		Behavior->PreTick(a_Dt, a_Chunk);
	}

	// Note 1: Each monster tick, at most one Behavior executes its Tick method.
	// Note 2: Each monster tick, exactly one of these is executed:
	// ControlStarting, Tick, ControlEnding

	// If we're in a regular tick cycle
	if (m_TickControllingBehaviorState == Normal)
	{
		// ask the behaviors sequentially if they are interested in controlling this mob
		// Stop at the first one that says yes.
		m_NewTickControllingBehavior = nullptr;
		for (cBehavior * Behavior : m_AttachedTickBehaviors)
		{
			if (Behavior->IsControlDesired(a_Dt, a_Chunk))
			{
				m_NewTickControllingBehavior = Behavior;
				break;
			}
		}
		ASSERT(m_NewTickControllingBehavior != nullptr); // it's not OK if no one asks for control
		if (m_CurrentTickControllingBehavior == m_NewTickControllingBehavior)
		{
			// The Behavior asking for control is the same as the behavior from last tick.
			// Nothing special, just tick it.
			// LOGD("mobDebug - Tick");
			m_CurrentTickControllingBehavior->Tick(a_Dt, a_Chunk);
		}
		else if (m_CurrentTickControllingBehavior == nullptr)
		{
			// first behavior to ever control
			m_TickControllingBehaviorState = NewControlStarting;
		}
		else
		{
			// The behavior asking for control is not the same as the behavior from last tick.
			// Begin the control swapping process.
			m_TickControllingBehaviorState = OldControlEnding;
		}

	}

	// Make the current controlling behavior clean up
	if (m_TickControllingBehaviorState == OldControlEnding)
	{
		ASSERT(m_CurrentTickControllingBehavior != nullptr);
		if (m_CurrentTickControllingBehavior->ControlEnding(a_Dt, a_Chunk))
		{
			// The current behavior told us it is ready for letting go of control
			m_TickControllingBehaviorState = NewControlStarting;
		}
		else
		{
			// The current behavior is not ready for releasing control. We'll execute ControlEnding
			// next tick too.
			m_TickControllingBehaviorState = OldControlEnding;
		}
	}
	// Make the new controlling behavior set up
	else if (m_TickControllingBehaviorState == NewControlStarting)
	{
		ASSERT(m_NewTickControllingBehavior != nullptr);
		if (m_NewTickControllingBehavior->ControlStarting(a_Dt, a_Chunk))
		{
			// The new behavior told us it is ready for taking control
			// The new behavior is now the current behavior. Next tick it will execute its Tick.
			m_TickControllingBehaviorState = Normal;
			m_CurrentTickControllingBehavior = m_NewTickControllingBehavior;
		}
		else
		{
			// The new behavior is not ready for taking control.
			// We'll execute ControlStarting next tick too.
			m_TickControllingBehaviorState = NewControlStarting;
		}
	}

	// All behaviors can execute PostTick and PreTick.
	// These are for bookkeeping or passive actions like laying eggs.
	// They MUST NOT control mob movement or interefere with the main Tick.
	for (cBehavior * Behavior : m_AttachedPostTickBehaviors)
	{
		// LOGD("mobDebug - PostTick");
		Behavior->PostTick(a_Dt, a_Chunk);
	}

	bool a_IsFollowingPath = false;
	if (m_PathfinderActivated)
	{
		if (ReachedFinalDestination() || (m_LeashToPos != nullptr))
		{
			StopMovingToPosition();  // Simply sets m_PathfinderActivated to false.
		}
		else
		{
			// mobToDo fix dont care
			// Note that m_NextWayPointPosition is actually returned by GetNextWayPoint)
			switch (m_PathFinder.GetNextWayPoint(*Chunk, GetPosition(), &m_FinalDestination, &m_NextWayPointPosition, true))
			{
				case ePathFinderStatus::PATH_FOUND:
				{
					/* If I burn in daylight, and I won't burn where I'm standing, and I'll burn in my next position, and at least one of those is true:
					1. I am idle
					2. I was not hurt by a player recently.
					Then STOP. */
					if (
							//mobTodo emstate
						/* (GetBehaviorDayLightBurner() != nullptr) && (m_TicksSinceLastDamaged >= 100) &&
						GetBehaviorDayLightBurner()->WouldBurnAt(m_NextWayPointPosition, *Chunk) &&
						!(GetBehaviorDayLightBurner()->WouldBurnAt(GetPosition(), *Chunk)) */
							1 == 0

							// This logic should probably be in chaser
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

	// Leash calculations
	if ((m_TicksAlive % LEASH_ACTIONS_TICK_STEP) == 0)
	{
		CalcLeashActions();
	}

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





void cMonster::CalcLeashActions()
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
	else if (IsLeashed())  // Mob is already leashed to an entity: follow it.
	{
		// TODO: leashed mobs in vanilla can move around up to 5 blocks distance from leash origin
		MoveToPosition(m_LeashedTo->GetPosition());

		// If distance to target > 10 break leash
		Vector3f a_Distance(m_LeashedTo->GetPosition() - GetPosition());
		double Distance(a_Distance.Length());
		if (Distance > 10.0)
		{
			LOGD("Leash broken (distance)");
			Unleash(false);
		}
	}
}





void cMonster::SetPitchAndYawFromDestination(bool a_IsFollowingPath)
{
	Vector3d BodyDistance;
	cPawn * LookingAt = m_LookingAt.GetPointer();
	if (!a_IsFollowingPath && (LookingAt != nullptr))
	{
		BodyDistance = LookingAt->GetPosition() - GetPosition();
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
	if (LookingAt != nullptr)
	{
		if (LookingAt->IsPlayer())  // Look at a player
		{
			HeadDistance = LookingAt->GetPosition() - GetPosition();
		}
		else  // Look at some other entity
		{
			HeadDistance = LookingAt->GetPosition() - GetPosition();
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

	for (cBehavior * Behavior : m_AttachedDoTakeDamageBehaviors)
	{
		ASSERT(Behavior != nullptr);
		Behavior->DoTakeDamage(a_TDI);
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
		case mtZombie:
		case mtZombiePigman:
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
		m_World->SpawnExperienceOrb(GetPosX(), GetPosY(), GetPosZ(), Reward);
	}
	m_DestroyTimer = std::chrono::milliseconds(0);
}





void cMonster::OnRightClicked(cPlayer & a_Player)
{
	super::OnRightClicked(a_Player);

	// mobTodo put this in a behavior?
	const cItem & EquippedItem = a_Player.GetEquippedItem();
	if ((EquippedItem.m_ItemType == E_ITEM_NAME_TAG) && !EquippedItem.m_CustomName.empty())
	{
		SetCustomName(EquippedItem.m_CustomName);
		if (!a_Player.IsGameModeCreative())
		{
			a_Player.GetInventory().RemoveOneEquippedItem();
		}
	}

	for (cBehavior * Behavior : m_AttachedOnRightClickBehaviors)
	{
		Behavior->OnRightClicked(a_Player);
	}

	// mobTodo put this in a behavior?
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







void cMonster::SetLookingAt(cPawn * a_NewTarget)
{
	m_LookingAt = a_NewTarget;

	/*
	ASSERT((a_NewTarget == nullptr) || (IsTicking()));
	if (m_LookingAt == a_NewTarget)
	{
		return;
	}
	cPawn * OldTarget = m_LookingAt;
	m_LookingAt = a_NewTarget;

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
	}*/

}




bool cMonster::IsPathFinderActivated() const
{
	return m_PathfinderActivated;
}





cBehaviorBreeder * cMonster::GetBehaviorBreeder()
{
	return nullptr;
}





const cBehaviorBreeder * cMonster::GetBehaviorBreeder() const
{
	return nullptr;
}





cBehaviorChaser * cMonster::GetBehaviorChaser()
{
	return nullptr;
}





cBehaviorDayLightBurner * cMonster::GetBehaviorDayLightBurner()
{
	return nullptr;
}





void cMonster::InheritFromParents(cMonster * a_Parent1, cMonster * a_Parent2)
{
	UNUSED(a_Parent1);
	UNUSED(a_Parent2);
	return;
}





void cMonster::GetFollowedItems(cItems & a_Items)
{
	return;
}





void cMonster::GetBreedingItems(cItems & a_Items)
{
	return GetFollowedItems(a_Items);
}





cPlayer * cMonster::GetNearestPlayer()
{
	if (m_NearestPlayerIsStale)
	{
		// TODO: Rewrite this to use cWorld's DoWithPlayers()
		m_NearestPlayer = GetWorld()->FindClosestPlayer(GetPosition(), static_cast<float>(GetSightDistance()));
		m_NearestPlayerIsStale = false;
	}
	if ((m_NearestPlayer != nullptr) && (!m_NearestPlayer->IsTicking()))
	{
		m_NearestPlayer = nullptr;
	}
	return m_NearestPlayer;
}





std::unique_ptr<cMonster> cMonster::NewMonsterFromType(eMonsterType a_MobType)
{
	auto & Random = GetRandomProvider();

	// Create the mob entity
	switch (a_MobType)
	{
		case mtMagmaCube:
		{
			return cpp14::make_unique<cMagmaCube>(1 << Random.RandInt(2));  // Size 1, 2 or 4
		}
		case mtSlime:
		{
			return cpp14::make_unique<cSlime>(1 << Random.RandInt(2));  // Size 1, 2 or 4
		}
		case mtSkeleton:
		{
			// TODO: Actual detection of spawning in Nether
			return cpp14::make_unique<cSkeleton>(false);
		}
		case mtVillager:
		{
			int VillagerType = Random.RandInt(6);
			if (VillagerType == 6)
			{
				// Give farmers a better chance of spawning
				VillagerType = 0;
			}

			return cpp14::make_unique<cVillager>(static_cast<cVillager::eVillagerType>(VillagerType));
		}
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

			return cpp14::make_unique<cHorse>(HorseType, HorseColor, HorseStyle, HorseTameTimes);
		}

		case mtBat:           return cpp14::make_unique<cBat>();
		case mtBlaze:         return cpp14::make_unique<cBlaze>();
		case mtCaveSpider:    return cpp14::make_unique<cCaveSpider>();
		case mtChicken:       return cpp14::make_unique<cChicken>();
		case mtCow:           return cpp14::make_unique<cCow>();
		case mtCreeper:       return cpp14::make_unique < cCreeper>();
		case mtEnderDragon:   return cpp14::make_unique<cEnderDragon>();
		case mtEnderman:      return cpp14::make_unique<cEnderman>();
		case mtGhast:         return cpp14::make_unique<cGhast>();
		case mtGiant:         return cpp14::make_unique<cGiant>();
		case mtGuardian:      return cpp14::make_unique<cGuardian>();
		case mtIronGolem:     return cpp14::make_unique<cIronGolem>();
		case mtMooshroom:     return cpp14::make_unique<cMooshroom>();
		case mtOcelot:        return cpp14::make_unique<cOcelot>();
		case mtPig:           return cpp14::make_unique<cPig>();
		case mtRabbit:        return cpp14::make_unique<cRabbit>();
		case mtSheep:         return cpp14::make_unique<cSheep>();
		case mtSilverfish:    return cpp14::make_unique<cSilverfish>();
		case mtSnowGolem:     return cpp14::make_unique<cSnowGolem>();
		case mtSpider:        return cpp14::make_unique<cSpider>();
		case mtSquid:         return cpp14::make_unique<cSquid>();
		case mtWitch:         return cpp14::make_unique<cWitch>();
		case mtWither:        return cpp14::make_unique<cWither>();
		case mtWolf:          return cpp14::make_unique<cWolf>();
		case mtZombie:        return cpp14::make_unique<cZombie>(false);  // TODO: Infected zombie parameter
		case mtZombiePigman:  return cpp14::make_unique<cZombiePigman>();
		default:
		{
			ASSERT(!"Unhandled mob type whilst trying to spawn mob!");
			return nullptr;
		}
	}
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



void cMonster::AttachPreTickBehavior(cBehavior * a_Behavior)
{
	ASSERT(a_Behavior != nullptr);
	m_AttachedPreTickBehaviors.push_back(a_Behavior);
}





void cMonster::AttachPostTickBehavior(cBehavior * a_Behavior)
{
	ASSERT(a_Behavior != nullptr);
	m_AttachedPostTickBehaviors.push_back(a_Behavior);
}





void cMonster::AttachTickBehavior(cBehavior * a_Behavior)
{
	ASSERT(a_Behavior != nullptr);
	m_AttachedTickBehaviors.push_back(a_Behavior);
}





void cMonster::AttachDestroyBehavior(cBehavior * a_Behavior)
{
	ASSERT(a_Behavior != nullptr);
	m_AttachedDestroyBehaviors.push_back(a_Behavior);
}





void cMonster::AttachRightClickBehavior(cBehavior * a_Behavior)
{
	ASSERT(a_Behavior != nullptr);
	m_AttachedOnRightClickBehaviors.push_back(a_Behavior);
}





void cMonster::AttachDoTakeDamageBehavior(cBehavior * a_Behavior)
{
	m_AttachedDoTakeDamageBehaviors.push_back(a_Behavior);
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
