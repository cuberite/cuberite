
// ItemFishingRod.h

// Declares the various fishing rod ItemHandlers





#pragma once

#include "../Entities/Floater.h"
#include "../Entities/Entity.h"





class cItemFishingRodHandler :
	public cItemHandler
{
	typedef cItemHandler super;
	
public:
	cItemFishingRodHandler(int a_ItemType) :
		super(a_ItemType)
	{
	}

	virtual bool OnItemUse(cWorld * a_World, cPlayer * a_Player, const cItem & a_Item, int a_BlockX, int a_BlockY, int a_BlockZ, char a_Dir) override
	{
		if (a_Player->IsFishing())
		{
			class Callbacks : public cEntityCallback
			{
			public:
				bool CanPickup;
				virtual bool Item(cEntity * a_Entity) override
				{
					CanPickup = ((cFloater *)a_Entity)->CanPickup();
					a_Entity->Destroy(true);
					return true;
				}
				Callbacks(void) : CanPickup(false) {}
			} Callback;
			a_World->DoWithEntityByID(a_Player->GetFloaterID(), Callback);
			a_Player->SetIsFishing(false);
			if (Callback.CanPickup)
			{
				a_Player->SendMessage("TODO: Spawn Items.");
				// TODO: Pickups if fishing went correct.
			}
			
		}
		else
		{
			cFloater * Floater = new cFloater(a_Player->GetPosX(), a_Player->GetStance(), a_Player->GetPosZ(), a_Player->GetLookVector() * 5, a_Player->GetUniqueID());
			Floater->Initialize(a_World);
			a_Player->SetIsFishing(true, Floater->GetUniqueID());
		}
		return true;
	}
} ;