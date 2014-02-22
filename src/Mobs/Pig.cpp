
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Pig.h"
#include "../Entities/Player.h"
#include "../World.h"





cPig::cPig(void) :
	super("Pig", mtPig, "mob.pig.say", "mob.pig.death", 0.9, 0.9),
	m_bIsSaddled(false)
{
}





void cPig::GetDrops(cItems & a_Drops, cEntity * a_Killer)
{
	int LootingLevel = a_Killer->GetEquippedWeapon().m_Enchantments.GetLevel(E_ENCHANTMENT_LOOTING);
	AddRandomDropItem(a_Drops, 1, 3 + LootingLevel, IsOnFire() ? E_ITEM_COOKED_PORKCHOP : E_ITEM_RAW_PORKCHOP);
	if (m_bIsSaddled)
	{
		a_Drops.push_back(cItem(E_ITEM_SADDLE, 1));
	}
}





void cPig::OnRightClicked(cPlayer & a_Player)
{
	if (m_bIsSaddled)
	{
		if (m_Attachee != NULL)
		{
			if (m_Attachee->GetUniqueID() == a_Player.GetUniqueID())
			{
				// This player is already sitting in, they want out.
				a_Player.Detach();
				return;
			}
		
			if (m_Attachee->IsPlayer())
			{
				// Another player is already sitting in here, cannot attach
				return;
			}
		
			// Detach whatever is sitting in this pig now:
			m_Attachee->Detach();
		}
	
		// Attach the player to this pig
		a_Player.AttachTo(this);
	}
	else if (a_Player.GetEquippedItem().m_ItemType == E_ITEM_SADDLE)
	{
		if (!a_Player.IsGameModeCreative())
		{
			a_Player.GetInventory().RemoveOneEquippedItem();
		}

		// Set saddle state & broadcast metadata
		m_bIsSaddled = true;
		m_World->BroadcastEntityMetadata(*this);
	}
}



