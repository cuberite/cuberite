
// ItemFishingRod.h

// Declares the various fishing rod ItemHandlers





#pragma once

#include "../Entities/Floater.h"
#include "../Entities/Entity.h"
#include "../Item.h"




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
			class cFloaterCallback :
				public cEntityCallback
			{
			public:
				cFloaterCallback(void) : 
					m_CanPickup(false) 
				{
				}

				bool CanPickup(void) const { return m_CanPickup; }
				Vector3d GetPos(void) const { return m_Pos; }

				virtual bool Item(cEntity * a_Entity) override
				{
					m_CanPickup = ((cFloater *)a_Entity)->CanPickup();
					m_Pos = Vector3d(a_Entity->GetPosX(), a_Entity->GetPosY(), a_Entity->GetPosZ());
					a_Entity->Destroy(true);
					return true;
				}
			protected:
				bool m_CanPickup;
				Vector3d m_Pos;
			} Callbacks;
			a_World->DoWithEntityByID(a_Player->GetFloaterID(), Callbacks);
			a_Player->SetIsFishing(false);

			if (Callbacks.CanPickup())
			{
				cItems Drops;
				Drops.Add(cItem(E_ITEM_RAW_FISH));
				Vector3d FloaterPos(Callbacks.GetPos());
				Vector3d FlyDirection(a_Player->GetPosition() - FloaterPos);
				a_World->SpawnItemPickups(Drops, FloaterPos.x, FloaterPos.y, FloaterPos.z, FlyDirection.x, FlyDirection.Length() / (FlyDirection.y * 2), FlyDirection.z);
				// TODO: More types of pickups.
			}
		}
		else
		{
			cFloater * Floater = new cFloater(a_Player->GetPosX(), a_Player->GetStance(), a_Player->GetPosZ(), a_Player->GetLookVector() * 7, a_Player->GetUniqueID());
			Floater->Initialize(a_World);
			a_Player->SetIsFishing(true, Floater->GetUniqueID());
		}
		return true;
	}
} ;