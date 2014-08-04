
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Enderman.h"
#include "../Entities/Player.h"
#include "../Tracer.h"




////////////////////////////////////////////////////////////////////////////////
// cPlayerLookCheck
class cPlayerLookCheck :
	public cPlayerListCallback
{
public:
	cPlayerLookCheck(Vector3d a_EndermanPos, int a_SightDistance) :
		m_Player(NULL),
		m_EndermanPos(a_EndermanPos),
		m_SightDistance(a_SightDistance)
	{
	}

	virtual bool Item(cPlayer * a_Player) override
	{
		// Don't check players who are in creative gamemode
		if (a_Player->IsGameModeCreative())
		{
			return false;
		}
		
		Vector3d Direction = m_EndermanPos - a_Player->GetPosition();
		
		// Don't check players who are more then SightDistance (64) blocks away
		if (Direction.Length() > m_SightDistance)
		{
			return false;
		}
		
		// Don't check if the player has a pumpkin on his head
		if (a_Player->GetEquippedHelmet().m_ItemType == E_BLOCK_PUMPKIN)
		{
			return false;
		}

		
		Vector3d LookVector = a_Player->GetLookVector();
		double dot = Direction.Dot(LookVector);
		
		// 0.09 rad ~ 5 degrees
		// If the player's crosshair is within 5 degrees of the enderman, it counts as looking
		if (dot <= cos(0.09))
		{
			return false;
		}
		
		cTracer LineOfSight(a_Player->GetWorld());
		if (LineOfSight.Trace(m_EndermanPos, Direction, (int)Direction.Length()))
		{
			// No direct line of sight
			return false;
		}

		m_Player = a_Player;
		return true;
	}
	
	cPlayer * GetPlayer(void) const { return m_Player; }

protected:
	cPlayer * m_Player;
	Vector3d m_EndermanPos;
	int m_SightDistance;
} ;





cEnderman::cEnderman(void) :
	super("Enderman", mtEnderman, "mob.endermen.hit", "mob.endermen.death", 0.5, 2.9),
	m_bIsScreaming(false),
	CarriedBlock(E_BLOCK_AIR),
	CarriedMeta(0)
{
}





void cEnderman::GetDrops(cItems & a_Drops, cEntity * a_Killer)
{
	int LootingLevel = 0;
	if (a_Killer != NULL)
	{
		LootingLevel = a_Killer->GetEquippedWeapon().m_Enchantments.GetLevel(cEnchantments::enchLooting);
	}
	AddRandomDropItem(a_Drops, 0, 1 + LootingLevel, E_ITEM_ENDER_PEARL);
}




void cEnderman::CheckEventSeePlayer()
{
	if (m_Target != NULL)
	{
		return;
	}

	cPlayerLookCheck Callback(GetPosition(), m_SightDistance);
	if (m_World->ForEachPlayer(Callback))
	{
		return;
	}
	
	ASSERT(Callback.GetPlayer() != NULL);

	if (!CheckLight())
	{
		// Insufficient light for enderman to become aggravated
		// TODO: Teleport to a suitable location
		return;
	}

	if (!Callback.GetPlayer()->IsGameModeCreative())
	{
		super::EventSeePlayer(Callback.GetPlayer());
		m_EMState = CHASING;
		m_bIsScreaming = true;
		GetWorld()->BroadcastEntityMetadata(*this);
	}
}





void cEnderman::CheckEventLostPlayer(void)
{
	super::CheckEventLostPlayer();
	if (!CheckLight())
	{
		EventLosePlayer();
	}
}
	




void cEnderman::EventLosePlayer()
{
	super::EventLosePlayer();
	m_bIsScreaming = false;
	GetWorld()->BroadcastEntityMetadata(*this);
}





bool cEnderman::CheckLight()
{
	int ChunkX, ChunkZ;
	cChunkDef::BlockToChunk(POSX_TOINT, POSZ_TOINT, ChunkX, ChunkZ);

	// Check if the chunk the enderman is in is lit
	if (!m_World->IsChunkLighted(ChunkX, ChunkZ))
	{
		m_World->QueueLightChunk(ChunkX, ChunkZ);
		return true;
	}

	// Enderman only attack if the skylight is lower or equal to 8
	if (m_World->GetBlockSkyLight(POSX_TOINT, POSY_TOINT, POSZ_TOINT) - GetWorld()->GetSkyDarkness() > 8)
	{
		return false;
	}

	return true;
}
