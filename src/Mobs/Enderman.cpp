
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Enderman.h"
#include "../Entities/Player.h"




///////////////////////////////////////////////////////////////////////////
// cPlayerLookCheck
class cPlayerLookCheck :
	public cPlayerListCallback
{
public:
	cPlayerLookCheck(Vector3d a_EndermanPos) :
		m_EndermanPos(a_EndermanPos),
		m_Player(NULL)
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

		Direction.Normalize();
		
		Vector3d LookVector = a_Player->GetLookVector();
		LookVector.Normalize();
		
		if ((Direction - LookVector).SqrLength() > 0.02)
		{
			return false;
		}
		
		// TODO: Don't attack the player if there is a wall between the player and the enderman.
		m_Player = a_Player;
		return true;
	}
	
	cPlayer * GetPlayer(void) const {return m_Player;}
	bool HasFoundPlayer(void) const {return (m_Player != NULL);}
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




void cEnderman::Tick(float a_Dt, cChunk & a_Chunk)
{
	super::Tick(a_Dt, a_Chunk);
	
	if (m_Target != NULL)
	{
		return;
	}

	cPlayerLookCheck Callback(GetPosition());
	if (!m_World->ForEachPlayer(Callback))
	{
		return;
	}

	if (!Callback.HasFoundPlayer())
	{
		return;
	}

	m_bIsScreaming = true;
	m_Target = Callback.GetPlayer();

	m_World->BroadcastEntityMetadata(*this);
}
	




