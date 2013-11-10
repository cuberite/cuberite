
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "IncludeAllMonsters.h"
#include "../Root.h"
#include "../Server.h"
#include "../ClientHandle.h"
#include "../World.h"
#include "../Entities/Player.h"
#include "../Defines.h"
#include "../MonsterConfig.h"
#include "../MersenneTwister.h"

#include "../Vector3f.h"
#include "../Vector3i.h"
#include "../Vector3d.h"
#include "../Tracer.h"
#include "../Chunk.h"
#include "../FastRandom.h"





/** Map for eType <-> string
Needs to be alpha-sorted by the strings, because binary search is used in StringToMobType()
The strings need to be lowercase (for more efficient comparisons in StringToMobType())
*/
static const struct
{
	cMonster::eType m_Type;
	const char * m_lcName;
} g_MobTypeNames[] =
{
	{cMonster::mtBat,          "bat"},
	{cMonster::mtBlaze,        "blaze"},
	{cMonster::mtCaveSpider,   "cavespider"},
	{cMonster::mtChicken,      "chicken"},
	{cMonster::mtCow,          "cow"},
	{cMonster::mtCreeper,      "creeper"},
	{cMonster::mtEnderman,     "enderman"},
	{cMonster::mtGhast,        "ghast"},
	{cMonster::mtHorse,        "horse"},
	{cMonster::mtMagmaCube,    "magmacube"},
	{cMonster::mtMooshroom,    "mooshroom"},
	{cMonster::mtOcelot,       "ocelot"},
	{cMonster::mtPig,          "pig"},
	{cMonster::mtSheep,        "sheep"},
	{cMonster::mtSilverfish,   "silverfish"},
	{cMonster::mtSkeleton,     "skeleton"},
	{cMonster::mtSlime,        "slime"},
	{cMonster::mtSpider,       "spider"},
	{cMonster::mtSquid,        "squid"},
	{cMonster::mtVillager,     "villager"},
	{cMonster::mtWitch,        "witch"},
	{cMonster::mtWolf,         "wolf"},
	{cMonster::mtZombie,       "zombie"},
	{cMonster::mtZombiePigman, "zombiepigman"},
} ;





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cMonster:

cMonster::cMonster(const AString & a_ConfigName, eType a_MobType, const AString & a_SoundHurt, const AString & a_SoundDeath, double a_Width, double a_Height)
	: super(etMonster, a_Width, a_Height)
	, m_Target(NULL)
	, m_AttackRate(3)
	, idle_interval(0)
	, m_bMovingToDestination(false)
	, m_DestinationTime( 0 )
	, m_DestroyTimer( 0 )
	, m_Jump(0)
	, m_MobType(a_MobType)
	, m_SoundHurt(a_SoundHurt)
	, m_SoundDeath(a_SoundDeath)
	, m_EMState(IDLE)
	, m_SightDistance(25)
	, m_SeePlayerInterval (0)
	, m_EMPersonality(AGGRESSIVE)
	, m_AttackDamage(1.0f)
	, m_AttackRange(2.0f)
	, m_AttackInterval(0)
	, m_BurnsInDaylight(false)
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





void cMonster::MoveToPosition( const Vector3f & a_Position )
{
	m_bMovingToDestination = true;

	m_Destination = a_Position;
}





bool cMonster::ReachedDestination()
{
	Vector3f Distance = (m_Destination) - GetPosition();
	if( Distance.SqrLength() < 2.f )
		return true;

	return false;
}





void cMonster::Tick(float a_Dt, cChunk & a_Chunk)
{
	super::Tick(a_Dt, a_Chunk);

	if (m_Health <= 0)
	{
		// The mob is dead, but we're still animating the "puff" they leave when they die
		m_DestroyTimer += a_Dt / 1000;
		if (m_DestroyTimer > 1)
		{
			Destroy(true);
		}
		return;
	}

	// Burning in daylight
	HandleDaylightBurning(a_Chunk);
	
	HandlePhysics(a_Dt,a_Chunk);
	BroadcastMovementUpdate();

	a_Dt /= 1000;

	if (m_bMovingToDestination)
	{
		Vector3f Pos( GetPosition() );
		Vector3f Distance = m_Destination - Pos;
		if( !ReachedDestination() )
		{
			Distance.y = 0;
			Distance.Normalize();
			Distance *= 3;
			SetSpeedX( Distance.x );
			SetSpeedZ( Distance.z );

			if (m_EMState == ESCAPING)
			{	//Runs Faster when escaping :D otherwise they just walk away
				SetSpeedX (GetSpeedX() * 2.f);
				SetSpeedZ (GetSpeedZ() * 2.f);
			}
		}
		else
		{
			m_bMovingToDestination = false;
		}

		if( GetSpeed().SqrLength() > 0.f )
		{
			if( m_bOnGround )
			{
				Vector3f NormSpeed = Vector3f(GetSpeed()).NormalizeCopy();
				Vector3f NextBlock = Vector3f( GetPosition() ) + NormSpeed;
				int NextHeight;
				if (!m_World->TryGetHeight((int)NextBlock.x, (int)NextBlock.z, NextHeight))
				{
					// The chunk at NextBlock is not loaded
					return;
				}
				if( NextHeight > (GetPosY() - 1.0) && (NextHeight - GetPosY()) < 2.5 )
				{
					m_bOnGround = false;
					SetSpeedY(5.f); // Jump!!
				}
			}
		}
	}

	Vector3d Distance = m_Destination - GetPosition();
	if (Distance.SqrLength() > 0.1f)
	{
		double Rotation, Pitch;
		Distance.Normalize();
		VectorToEuler( Distance.x, Distance.y, Distance.z, Rotation, Pitch );
		SetHeadYaw (Rotation);
		SetRotation( Rotation );
		SetPitch( -Pitch );
	}

	switch (m_EMState)
	{
		case IDLE:
		{
			// If enemy passive we ignore checks for player visibility
			InStateIdle(a_Dt);
			break;
		}
	
		case CHASING:
		{
			// If we do not see a player anymore skip chasing action
			InStateChasing(a_Dt);
			break;
		}
	
		case ESCAPING:
		{
			InStateEscaping(a_Dt);
			break;
		}
	}  // switch (m_EMState)
}






void cMonster::DoTakeDamage(TakeDamageInfo & a_TDI)
{
	super::DoTakeDamage(a_TDI);
	if((m_SoundHurt != "") && (m_Health > 0)) m_World->BroadcastSoundEffect(m_SoundHurt, (int)(GetPosX() * 8), (int)(GetPosY() * 8), (int)(GetPosZ() * 8), 1.0f, 0.8f);
	if (a_TDI.Attacker != NULL)
	{
		m_Target = a_TDI.Attacker;
		AddReference(m_Target);
	}
}





void cMonster::KilledBy(cEntity * a_Killer)
{
	super::KilledBy(a_Killer);
	if (m_SoundHurt != "")
	{
		m_World->BroadcastSoundEffect(m_SoundDeath, (int)(GetPosX() * 8), (int)(GetPosY() * 8), (int)(GetPosZ() * 8), 1.0f, 0.8f);
	}
	m_DestroyTimer = 0;
}





//----State Logic

const char *cMonster::GetState()
{
	switch(m_EMState)
	{
		case IDLE:      return "Idle";
		case ATTACKING: return "Attacking";
		case CHASING:   return "Chasing";
		default:        return "Unknown";
	}
}





// for debugging
void cMonster::SetState(const AString & a_State)
{
	if (a_State.compare("Idle") == 0)
	{
		m_EMState = IDLE;
	}
	else if (a_State.compare("Attacking") == 0)
	{
		m_EMState = ATTACKING;
	}
	else if (a_State.compare("Chasing") == 0)
	{
		m_EMState = CHASING;
	}
	else
	{
		LOGD("cMonster::SetState(): Invalid state");
		ASSERT(!"Invalid state");
	}
}





//Checks to see if EventSeePlayer should be fired
//monster sez: Do I see the player
void cMonster::CheckEventSeePlayer(void)
{
	// TODO: Rewrite this to use cWorld's DoWithPlayers()
	cPlayer * Closest = FindClosestPlayer();

	if (Closest != NULL)
	{
		EventSeePlayer(Closest);
	}
}





void cMonster::CheckEventLostPlayer(void)
{
	Vector3f pos;
	cTracer LineOfSight(GetWorld());
	
	if (m_Target != NULL)
	{
		pos = m_Target->GetPosition();
		if ((pos - GetPosition()).Length() > m_SightDistance || LineOfSight.Trace(GetPosition(),(pos - GetPosition()), (int)(pos - GetPosition()).Length()))
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
	AddReference(m_Target);
}





void cMonster::EventLosePlayer(void)
{
	Dereference(m_Target);
	m_Target = NULL;
	m_EMState = IDLE;
}





// What to do if in Idle State
void cMonster::InStateIdle(float a_Dt)
{
	idle_interval += a_Dt;
	if (idle_interval > 1)
	{
		// at this interval the results are predictable
		int rem = m_World->GetTickRandomNumber(6) + 1;
		// LOGD("Moving: int: %3.3f rem: %i",idle_interval,rem);
		idle_interval -= 1;		// So nothing gets dropped when the server hangs for a few seconds
		Vector3f Dist;
		Dist.x = (float)(m_World->GetTickRandomNumber(10) - 5);
		Dist.z = (float)(m_World->GetTickRandomNumber(10) - 5);
		if ((Dist.SqrLength() > 2)  && (rem >= 3))
		{
			m_Destination.x = (float)(GetPosX() + Dist.x);
			m_Destination.z = (float)(GetPosZ() + Dist.z);
			int PosY;
			if (m_World->TryGetHeight((int)m_Destination.x, (int)m_Destination.z, PosY))
			{
				m_Destination.y = (float)PosY + 1.2f;
				MoveToPosition(m_Destination);
			}
		}
	}
}





// What to do if in Chasing State
// This state should always be defined in each child class
void cMonster::InStateChasing(float a_Dt)
{
	UNUSED(a_Dt);
}





// What to do if in Escaping State
void cMonster::InStateEscaping(float a_Dt)
{
	UNUSED(a_Dt);
	
	if (m_Target != NULL)
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





// Do attack here
// a_Dt is passed so we can set attack rate
void cMonster::Attack(float a_Dt)
{
	m_AttackInterval += a_Dt * m_AttackRate;
    if ((m_Target != NULL) && (m_AttackInterval > 3.0))
    {
        // Setting this higher gives us more wiggle room for attackrate
        m_AttackInterval = 0.0;
        ((cPawn *)m_Target)->TakeDamage(*this);
    }
}





// Checks for Players close by and if they are visible return the closest
cPlayer * cMonster::FindClosestPlayer(void)
{
	return m_World->FindClosestPlayer(GetPosition(), m_SightDistance);
}





void cMonster::GetMonsterConfig(const AString & a_Name)
{
	cRoot::Get()->GetMonsterConfig()->AssignAttributes(this, a_Name);
}





void cMonster::SetAttackRate(int ar)
{
	m_AttackRate = (float)ar;
}





void cMonster::SetAttackRange(float ar)
{
	m_AttackRange = ar;
}





void cMonster::SetAttackDamage(float ad)
{
	m_AttackDamage = ad;
}





void cMonster::SetSightDistance(float sd)
{
	m_SightDistance = sd;
}





AString cMonster::MobTypeToString(cMonster::eType a_MobType)
{
	// Mob types aren't sorted, so we need to search linearly:
	for (int i = 0; i < ARRAYCOUNT(g_MobTypeNames); i++)
	{
		if (g_MobTypeNames[i].m_Type == a_MobType)
		{
			return g_MobTypeNames[i].m_lcName;
		}
	}
	
	// Not found:
	return "";
}





cMonster::eType cMonster::StringToMobType(const AString & a_Name)
{
	AString lcName(a_Name);
	StrToLower(lcName);
	
	// Binary-search for the lowercase name:
	int lo = 0, hi = ARRAYCOUNT(g_MobTypeNames) - 1;
	while (hi - lo > 1)
	{
		int mid = (lo + hi) / 2;
		int res = strcmp(g_MobTypeNames[mid].m_lcName, lcName.c_str());
		if (res == 0)
		{
			return g_MobTypeNames[mid].m_Type;
		}
		if (res < 0)
		{
			lo = mid;
		}
		else
		{
			hi = mid;
		}
	}
	// Range has collapsed to at most two elements, compare each:
	if (strcmp(g_MobTypeNames[lo].m_lcName, lcName.c_str()) == 0)
	{
		return g_MobTypeNames[lo].m_Type;
	}
	if ((lo != hi) && (strcmp(g_MobTypeNames[hi].m_lcName, lcName.c_str()) == 0))
	{
		return g_MobTypeNames[hi].m_Type;
	}
	
	// Not found:
	return mtInvalidType;
}





cMonster::eFamily cMonster::FamilyFromType(eType a_Type)
{
	switch (a_Type)
	{
		case mtBat:          return mfAmbient;
		case mtBlaze:        return mfHostile;
		case mtCaveSpider:   return mfHostile;
		case mtChicken:      return mfPassive;
		case mtCow:          return mfPassive;
		case mtCreeper:      return mfHostile;
		case mtEnderman:     return mfHostile;
		case mtGhast:        return mfHostile;
		case mtHorse:        return mfPassive;
		case mtMagmaCube:    return mfHostile;
		case mtMooshroom:    return mfHostile;
		case mtOcelot:       return mfHostile;
		case mtPig:          return mfPassive;
		case mtSheep:        return mfPassive;
		case mtSilverfish:   return mfHostile;
		case mtSkeleton:     return mfHostile;
		case mtSlime:        return mfHostile;
		case mtSpider:       return mfHostile;
		case mtSquid:        return mfWater;
		case mtVillager:     return mfPassive;
		case mtWitch:        return mfHostile;
		case mtWolf:         return mfHostile;
		case mtZombie:       return mfHostile;
		case mtZombiePigman: return mfHostile;
	} ;
	ASSERT(!"Unhandled mob type");
	return mfMaxplusone;
}





int cMonster::GetSpawnDelay(cMonster::eFamily a_MobFamily)
{
	switch (a_MobFamily)
	{
		case mfHostile: return 40;
		case mfPassive: return 40;
		case mfAmbient: return 40;
		case mfWater:   return 400;
	}
	ASSERT(!"Unhandled mob family");
	return -1;
}





cMonster * cMonster::NewMonsterFromType(cMonster::eType a_MobType)
{
	cMonster * toReturn = NULL;
	cFastRandom RandomDerps;

	// Create the mob entity
	switch (a_MobType)
	{
		case mtMagmaCube:
		case mtSlime:         toReturn = new cSlime    (RandomDerps.NextInt(2) + 1);             break; // Size parameter
		case mtSheep:         toReturn = new cSheep    (RandomDerps.NextInt(15));                break; // Colour parameter
		case mtZombie:        toReturn = new cZombie   (false);                                  break; // TODO: Infected zombie parameter
		case mtSkeleton:
		{
			// TODO: Actual detection of spawning in Nether
			toReturn = new cSkeleton(RandomDerps.NextInt(1) == 0 ? false : true);
			break;
		}
		case mtVillager:
		{
			int VilType = RandomDerps.NextInt(6);
			if (VilType == 6) { VilType = 0; } // Give farmers a better chance of spawning

			toReturn = new cVillager(cVillager::eVillagerType(VilType)); // Type (blacksmith, butcher, etc.) parameter
			break;
		}
		case mtHorse:
		{
			// Horses take a type (species), a colour, and a style (dots, stripes, etc.)
			int HseType = RandomDerps.NextInt(7);
			int HseColor = RandomDerps.NextInt(6);
			int HseStyle = RandomDerps.NextInt(6);
			int HseTameTimes = RandomDerps.NextInt(6) + 1;

			if ((HseType == 5) || (HseType == 6) || (HseType == 7)) { HseType = 0; } // Increase chances of normal horse (zero)

			toReturn = new cHorse(HseType, HseColor, HseStyle, HseTameTimes);
			break;
		}

		case mtBat:           toReturn  = new cBat();                         break;
		case mtBlaze:         toReturn  = new cBlaze();                       break;
		case mtCaveSpider:    toReturn  = new cCavespider();                  break;
		case mtChicken:       toReturn  = new cChicken();                     break;
		case mtCow:           toReturn  = new cCow();                         break;
		case mtCreeper:       toReturn  = new cCreeper();                     break;
		case mtEnderman:      toReturn  = new cEnderman();                    break;
		case mtGhast:         toReturn  = new cGhast();                       break;
		case mtMooshroom:     toReturn  = new cMooshroom();                   break;
		case mtOcelot:        toReturn  = new cOcelot();                      break;
		case mtPig:           toReturn  = new cPig();                         break;
		case mtSilverfish:    toReturn  = new cSilverfish();                  break;
		case mtSpider:        toReturn  = new cSpider();                      break;
		case mtSquid:         toReturn  = new cSquid();                       break;
		case mtWitch:         toReturn  = new cWitch();                       break;
		case mtWolf:          toReturn  = new cWolf();                        break;
		case mtZombiePigman:  toReturn  = new cZombiePigman();                break;
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
	int Count = r1.randInt() % (a_Max + 1 - a_Min) + a_Min;
	if (Count > 0)
	{
		a_Drops.push_back(cItem(a_Item, Count, a_ItemHealth));
	}
}





void cMonster::HandleDaylightBurning(cChunk & a_Chunk)
{
	if (!m_BurnsInDaylight)
	{
		return;
	}
	
	int RelY = (int)floor(GetPosY());
	if ((RelY < 0) || (RelY >= cChunkDef::Height))
	{
		// Outside the world
		return;
	}
	
	int RelX = (int)floor(GetPosX()) - GetChunkX() * cChunkDef::Width;
	int RelZ = (int)floor(GetPosZ()) - GetChunkZ() * cChunkDef::Width;
	if (
		(a_Chunk.GetSkyLight(RelX, RelY, RelZ) == 15) &&             // In the daylight
		(a_Chunk.GetBlock(RelX, RelY, RelZ) != E_BLOCK_SOULSAND) &&  // Not on soulsand
		(GetWorld()->GetTimeOfDay() < (12000 + 1000)) &&             // It is nighttime
		!IsOnFire()                                                  // Not already burning
	)
	{
		// Burn for 100 ticks, then decide again
		StartBurning(100);
	}
}




cMonster::eFamily cMonster::GetMobFamily(void) const
{
	return FamilyFromType(m_MobType);
}




