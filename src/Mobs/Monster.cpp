
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "IncludeAllMonsters.h"
#include "../Root.h"
#include "../Server.h"
#include "../ClientHandle.h"
#include "../World.h"
#include "../Entities/Player.h"
#include "../Entities/ExpOrb.h"
#include "../MonsterConfig.h"
#include "../MersenneTwister.h"

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
	{cMonster::mtEnderDragon,  "enderdragon"},
	{cMonster::mtGhast,        "ghast"},
	{cMonster::mtHorse,        "horse"},
	{cMonster::mtIronGolem,    "irongolem"},
	{cMonster::mtMagmaCube,    "magmacube"},
	{cMonster::mtMooshroom,    "mooshroom"},
	{cMonster::mtOcelot,       "ocelot"},
	{cMonster::mtPig,          "pig"},
	{cMonster::mtSheep,        "sheep"},
	{cMonster::mtSilverfish,   "silverfish"},
	{cMonster::mtSkeleton,     "skeleton"},
	{cMonster::mtSnowGolem,    "snowgolem"},
	{cMonster::mtSlime,        "slime"},
	{cMonster::mtSpider,       "spider"},
	{cMonster::mtSquid,        "squid"},
	{cMonster::mtVillager,     "villager"},
	{cMonster::mtWitch,        "witch"},
	{cMonster::mtWither,       "wither"},
	{cMonster::mtWolf,         "wolf"},
	{cMonster::mtZombie,       "zombie"},
	{cMonster::mtZombiePigman, "zombiepigman"},
} ;





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cMonster:

cMonster::cMonster(const AString & a_ConfigName, eType a_MobType, const AString & a_SoundHurt, const AString & a_SoundDeath, double a_Width, double a_Height)
	: super(etMonster, a_Width, a_Height)
	, m_EMState(IDLE)
	, m_EMPersonality(AGGRESSIVE)
	, m_Target(NULL)
	, m_bMovingToDestination(false)
	, m_LastGroundHeight(POSY_TOINT)
	, m_IdleInterval(0)
	, m_DestroyTimer(0)
	, m_MobType(a_MobType)
	, m_SoundHurt(a_SoundHurt)
	, m_SoundDeath(a_SoundDeath)
	, m_AttackRate(3)
	, m_AttackDamage(1)
	, m_AttackRange(2)
	, m_AttackInterval(0)
	, m_SightDistance(25)
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





void cMonster::TickPathFinding()
{
	int PosX = (int)floor(GetPosX());
	int PosY = (int)floor(GetPosY());
	int PosZ = (int)floor(GetPosZ());

	m_FinalDestination.y = (double)FindFirstNonAirBlockPosition(m_FinalDestination.x, m_FinalDestination.z);

	std::vector<Vector3d> m_PotentialCoordinates;
	m_TraversedCoordinates.push_back(Vector3i(PosX, PosY, PosZ));

	static const struct // Define which directions to try to move to
	{
		int x, z;
	} gCrossCoords[] =
	{
		{ 1, 0},
		{-1, 0},
		{ 0, 1},
		{ 0,-1},
	} ;

	for (size_t i = 0; i < ARRAYCOUNT(gCrossCoords); i++)
	{
		if ((gCrossCoords[i].x + PosX == PosX) && (gCrossCoords[i].z + PosZ == PosZ))
		{
			continue;
		}

		if (IsCoordinateInTraversedList(Vector3i(gCrossCoords[i].x + PosX, PosY, gCrossCoords[i].z + PosZ)))
		{
			continue;
		}

		BLOCKTYPE BlockAtY = m_World->GetBlock(gCrossCoords[i].x + PosX, PosY, gCrossCoords[i].z + PosZ);
		BLOCKTYPE BlockAtYP = m_World->GetBlock(gCrossCoords[i].x + PosX, PosY + 1, gCrossCoords[i].z + PosZ);
		BLOCKTYPE BlockAtYPP = m_World->GetBlock(gCrossCoords[i].x + PosX, PosY + 2, gCrossCoords[i].z + PosZ);
		BLOCKTYPE BlockAtYM = m_World->GetBlock(gCrossCoords[i].x + PosX, PosY - 1, gCrossCoords[i].z + PosZ);

		if (!g_BlockIsSolid[BlockAtY] && !g_BlockIsSolid[BlockAtYP] && !IsBlockLava(BlockAtYM))
		{
			m_PotentialCoordinates.push_back(Vector3d((gCrossCoords[i].x + PosX), PosY, gCrossCoords[i].z + PosZ));
		}
		else if (g_BlockIsSolid[BlockAtY] && !g_BlockIsSolid[BlockAtYP] && !g_BlockIsSolid[BlockAtYPP] && !IsBlockLava(BlockAtYM))
		{
			m_PotentialCoordinates.push_back(Vector3d((gCrossCoords[i].x + PosX), PosY + 1, gCrossCoords[i].z + PosZ));
		}
	}

	if (!m_PotentialCoordinates.empty())
	{
		Vector3f ShortestCoords = m_PotentialCoordinates.front();
		for (std::vector<Vector3d>::const_iterator itr = m_PotentialCoordinates.begin(); itr != m_PotentialCoordinates.end(); ++itr)
		{
			Vector3f Distance = m_FinalDestination - ShortestCoords;
			Vector3f Distance2 = m_FinalDestination - *itr;
			if (Distance.SqrLength() > Distance2.SqrLength())
			{
				ShortestCoords = *itr;
			}
		}

		m_Destination = ShortestCoords;
		m_Destination.z += 0.5f;
		m_Destination.x += 0.5f;
	}
	else
	{
		FinishPathFinding();
	}
}





void cMonster::MoveToPosition(const Vector3f & a_Position)
{
	FinishPathFinding();

	m_FinalDestination = a_Position;
	m_bMovingToDestination = true;
	TickPathFinding();
}





void cMonster::MoveToPosition(const Vector3d & a_Position)
{
	FinishPathFinding();

	m_FinalDestination = a_Position;
	m_bMovingToDestination = true;
	TickPathFinding();
}



bool cMonster::IsCoordinateInTraversedList(Vector3i a_Coords)
{
	for (std::vector<Vector3i>::const_iterator itr = m_TraversedCoordinates.begin(); itr != m_TraversedCoordinates.end(); ++itr)
	{
		if (itr->Equals(a_Coords))
		{
			return true;
		}
	}

	return false;
}





bool cMonster::ReachedDestination()
{
	if ((m_Destination - GetPosition()).Length() < 0.5f)
	{
		return true;
	}

	return false;
}




bool cMonster::ReachedFinalDestination()
{
	if ((GetPosition() - m_FinalDestination).Length() <= m_AttackRange)
	{
		return true;
	}
	
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

	if ((m_Target != NULL) && m_Target->IsDestroyed())
		m_Target = NULL;

	// Burning in daylight
	HandleDaylightBurning(a_Chunk);

	a_Dt /= 1000;

	if (m_bMovingToDestination)
	{
		if (m_bOnGround)
		{
			m_Destination.y = FindFirstNonAirBlockPosition(m_Destination.x, m_Destination.z);

			if (DoesPosYRequireJump((int)floor(m_Destination.y)))
			{
				m_bOnGround = false;
				AddPosY(1.5); // Jump!!
			}
		}

		Vector3f Distance = m_Destination - GetPosition();
		if(!ReachedDestination() && !ReachedFinalDestination()) // If we haven't reached any sort of destination, move
		{
			Distance.y = 0;
			Distance.Normalize();
			Distance *= 5;
			SetSpeedX(Distance.x);
			SetSpeedZ(Distance.z);

			if (m_EMState == ESCAPING)
			{	//Runs Faster when escaping :D otherwise they just walk away
				SetSpeedX (GetSpeedX() * 2.f);
				SetSpeedZ (GetSpeedZ() * 2.f);
			}
		}
		else
		{
			if (ReachedFinalDestination()) // If we have reached the ultimate, final destination, stop pathfinding and attack if appropriate
			{
				FinishPathFinding();
			}
			else
			{
				TickPathFinding(); // We have reached the next point in our path, calculate another point
			}
		}
	}

	if (ReachedFinalDestination() && (m_Target != NULL))
		Attack(a_Dt);

	SetPitchAndYawFromDestination();
	HandleFalling();

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

	BroadcastMovementUpdate();
}




void cMonster::SetPitchAndYawFromDestination()
{
	Vector3d FinalDestination = m_FinalDestination;
	if (m_Target != NULL)
	{
		if (m_Target->IsPlayer())
		{
			FinalDestination.y = ((cPlayer *)m_Target)->GetStance();
		}
		else
		{
			FinalDestination.y = GetHeight();
		}
	}

	Vector3d Distance = FinalDestination - GetPosition();
	if (Distance.SqrLength() > 0.1f)
	{
		{
			double Rotation, Pitch;
			Distance.Normalize();
			VectorToEuler(Distance.x, Distance.y, Distance.z, Rotation, Pitch);
			SetHeadYaw(Rotation);
			SetPitch(-Pitch);
		}

		{
			Vector3d BodyDistance = m_Destination - GetPosition();
			double Rotation, Pitch;
			Distance.Normalize();
			VectorToEuler(BodyDistance.x, BodyDistance.y, BodyDistance.z, Rotation, Pitch);
			SetYaw(Rotation);
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
			TakeDamage(dtFalling, NULL, Damage, Damage, 0);

			// Fall particles
			GetWorld()->BroadcastSoundParticleEffect(2006, POSX_TOINT, POSY_TOINT - 1, POSZ_TOINT, Damage /* Used as particle effect speed modifier */);
		}

		m_LastGroundHeight = (int)floor(GetPosY());
	}
}




int cMonster::FindFirstNonAirBlockPosition(double a_PosX, double a_PosZ)
{
	int PosY = (int)floor(GetPosY());

	if (PosY < 0)
		PosY = 0;
	else if (PosY > cChunkDef::Height)
		PosY = cChunkDef::Height;

	if (!g_BlockIsSolid[m_World->GetBlock((int)floor(a_PosX), PosY, (int)floor(a_PosZ))])
	{
		while (!g_BlockIsSolid[m_World->GetBlock((int)floor(a_PosX), PosY, (int)floor(a_PosZ))] && (PosY > 0))
		{
			PosY--;
		}

		return PosY + 1;
	}
	else
	{
		while (g_BlockIsSolid[m_World->GetBlock((int)floor(a_PosX), PosY, (int)floor(a_PosZ))] && (PosY < cChunkDef::Height))
		{
			PosY++;
		}

		return PosY;
	}
}






void cMonster::DoTakeDamage(TakeDamageInfo & a_TDI)
{
	super::DoTakeDamage(a_TDI);

	if((m_SoundHurt != "") && (m_Health > 0))
		m_World->BroadcastSoundEffect(m_SoundHurt, (int)(GetPosX() * 8), (int)(GetPosY() * 8), (int)(GetPosZ() * 8), 1.0f, 0.8f);

	if (a_TDI.Attacker != NULL)
	{
		m_Target = a_TDI.Attacker;
	}
}





void cMonster::KilledBy(cEntity * a_Killer)
{
	super::KilledBy(a_Killer);
	if (m_SoundHurt != "")
	{
		m_World->BroadcastSoundEffect(m_SoundDeath, (int)(GetPosX() * 8), (int)(GetPosY() * 8), (int)(GetPosZ() * 8), 1.0f, 0.8f);
	}
	int Reward;
	switch (m_MobType)
	{
		// Animals
		case cMonster::mtChicken:
		case cMonster::mtCow:
		case cMonster::mtHorse:
		case cMonster::mtPig:
		case cMonster::mtSheep:
		case cMonster::mtSquid:
		case cMonster::mtMooshroom:
		case cMonster::mtOcelot:
		case cMonster::mtWolf:
		{
			Reward = m_World->GetTickRandomNumber(2) + 1;
			break;
		}

		// Monsters
		case cMonster::mtCaveSpider:
		case cMonster::mtCreeper:
		case cMonster::mtEnderman:
		case cMonster::mtGhast:
		case cMonster::mtSilverfish:
		case cMonster::mtSkeleton:
		case cMonster::mtSpider:
		case cMonster::mtWitch:
		case cMonster::mtZombie:
		case cMonster::mtZombiePigman:
		case cMonster::mtSlime:
		case cMonster::mtMagmaCube:
		{
			Reward = 6 + (m_World->GetTickRandomNumber(2));
			break;
		}
		case cMonster::mtBlaze:
		{
			Reward = 10;
			break;
		}

		// Bosses
		case cMonster::mtEnderDragon:
		{
			Reward = 12000;
			break;
		}
		case cMonster::mtWither:
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
	m_World->SpawnExperienceOrb(GetPosX(), GetPosY(), GetPosZ(), Reward);
	m_DestroyTimer = 0;
}





//Checks to see if EventSeePlayer should be fired
//monster sez: Do I see the player
void cMonster::CheckEventSeePlayer(void)
{
	// TODO: Rewrite this to use cWorld's DoWithPlayers()
	cPlayer * Closest = m_World->FindClosestPlayer(GetPosition(), (float)m_SightDistance, false);

	if (Closest != NULL)
	{
		EventSeePlayer(Closest);
	}
}





void cMonster::CheckEventLostPlayer(void)
{	
	if (m_Target != NULL)
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
	m_Target = NULL;
	m_EMState = IDLE;
}





void cMonster::InStateIdle(float a_Dt)
{
	if (m_bMovingToDestination)
	{
		return; // Still getting there
	}

	m_IdleInterval += a_Dt;

	if (m_IdleInterval > 1)
	{
		// At this interval the results are predictable
		int rem = m_World->GetTickRandomNumber(6) + 1;
		m_IdleInterval -= 1; // So nothing gets dropped when the server hangs for a few seconds

		Vector3d Dist;
		Dist.x = (double)m_World->GetTickRandomNumber(10) - 5;
		Dist.z = (double)m_World->GetTickRandomNumber(10) - 5;

		if ((Dist.SqrLength() > 2)  && (rem >= 3))
		{
			Vector3d Destination(GetPosX() + Dist.x, 0, GetPosZ() + Dist.z);

			int NextHeight = FindFirstNonAirBlockPosition(Destination.x, Destination.z);

			if (IsNextYPosReachable(NextHeight))
			{
				Destination.y = NextHeight;
				MoveToPosition(Destination);
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
}





void cMonster::GetMonsterConfig(const AString & a_Name)
{
	cRoot::Get()->GetMonsterConfig()->AssignAttributes(this, a_Name);
}





AString cMonster::MobTypeToString(cMonster::eType a_MobType)
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
	// Passive-agressive mobs are counted in mob spawning code as passive

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
		case mtIronGolem:    return mfPassive;
		case mtMagmaCube:    return mfHostile;
		case mtMooshroom:    return mfHostile;
		case mtOcelot:       return mfPassive;
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
	cFastRandom Random;
	cMonster * toReturn = NULL;

	// Create the mob entity
	switch (a_MobType)
	{
		case mtMagmaCube:
		case mtSlime:
		{
			toReturn = new cSlime(Random.NextInt(2) + 1);
			break;
		}
		case mtSkeleton:
		{
			// TODO: Actual detection of spawning in Nether
			toReturn = new cSkeleton(Random.NextInt(1) == 0 ? false : true);
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

			toReturn = new cVillager((cVillager::eVillagerType)VillagerType);
			break;
		}
		case mtHorse:
		{
			// Horses take a type (species), a colour, and a style (dots, stripes, etc.)
			int HorseType = Random.NextInt(7);
			int HorseColor = Random.NextInt(6);
			int HorseStyle = Random.NextInt(6);
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
		case mtCaveSpider:    toReturn = new cCavespider();               break;
		case mtChicken:       toReturn = new cChicken();                  break;
		case mtCow:           toReturn = new cCow();                      break;
		case mtCreeper:       toReturn = new cCreeper();                  break;
		case mtEnderDragon:   toReturn = new cEnderDragon();              break;
		case mtEnderman:      toReturn = new cEnderman();                 break;
		case mtGhast:         toReturn = new cGhast();                    break;
		case mtIronGolem:     toReturn = new cIronGolem();                break;
		case mtMooshroom:     toReturn = new cMooshroom();                break;
		case mtOcelot:        toReturn = new cOcelot();                   break;
		case mtPig:           toReturn = new cPig();                      break;
		case mtSheep:         toReturn = new cSheep (Random.NextInt(15)); break; // Colour parameter
		case mtSilverfish:    toReturn = new cSilverfish();               break;
		case mtSnowGolem:     toReturn = new cSnowGolem();                break;
		case mtSpider:        toReturn = new cSpider();                   break;
		case mtSquid:         toReturn = new cSquid();                    break;
		case mtWitch:         toReturn = new cWitch();                    break;
		case mtWither:	      toReturn = new cWither();                   break;
		case mtWolf:          toReturn = new cWolf();                     break;
		case mtZombie:        toReturn = new cZombie(false);              break; // TODO: Infected zombie parameter
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

	if (!a_Chunk.IsLightValid())
	{
		m_World->QueueLightChunk(GetChunkX(), GetChunkZ());
		return;
	}

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




