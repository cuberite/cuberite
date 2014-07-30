
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Enderman.h"
#include "../Entities/Player.h"
#include "../Tracer.h"




///////////////////////////////////////////////////////////////////////////
// cPlayerLookCheck
class cPlayerLookCheck :
	public cPlayerListCallback
{
public:
	cPlayerLookCheck(Vector3d a_EndermanPos) :
		m_Player(NULL),
		m_EndermanPos(a_EndermanPos)
	{
	}

	virtual bool Item(cPlayer * a_Player) override
	{
		// Don't check players who are in creative gamemode.
		if (a_Player->IsGameModeCreative())
		{
			return false;
		}
		
		Vector3d Direction = m_EndermanPos - a_Player->GetPosition();
		
		// Don't check players who are more then 64 blocks away.
		if (Direction.SqrLength() > 64)
		{
			return false;
		}
		
		// Don't check if the player has a pumpkin on his head.
		if (a_Player->GetEquippedHelmet().m_ItemType == E_BLOCK_PUMPKIN)
		{
			return false;
		}

		
		Vector3d LookVector = a_Player->GetLookVector();
		double dot = Direction.Dot(LookVector);
		
		// 0.09 rad ~ 5 degrees.
		// If the players crosshair is within 5 degrees of the endermen
		// It counts as looking.
		if (dot > cos(0.09))
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

	cPlayerLookCheck Callback(GetPosition());
	if (m_World->ForEachPlayer(Callback))
	{
		return;
	}
	
	ASSERT(Callback.GetPlayer() != NULL);

	if (!Callback.GetPlayer()->IsGameModeCreative())
	{
		super::EventSeePlayer(Callback.GetPlayer());
		m_EMState = CHASING;
		m_bIsScreaming = true;
		GetWorld()->BroadcastEntityMetadata(*this);
	}
}
	




void cEnderman::EventLosePlayer()
{
	super::EventLosePlayer();
	m_bIsScreaming = false;
	GetWorld()->BroadcastEntityMetadata(*this);
}
