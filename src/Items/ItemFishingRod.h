
// ItemFishingRod.h

// Declares the various fishing rod ItemHandlers





#pragma once

#include "../Entities/Floater.h"
#include "../Entities/Entity.h"
#include "../Item.h"
#include "../Root.h"





////////////////////////////////////////////////////////////////////////////////
// cFloaterCallback
class cFloaterCallback
{
public:
	cFloaterCallback(void) :
		m_CanPickup(false),
		m_AttachedMobID(cEntity::INVALID_ID)
	{
	}

	bool operator () (cEntity & a_Entity)
	{
		auto & Floater = static_cast<cFloater &>(a_Entity);
		m_CanPickup = Floater.CanPickup();
		m_Pos = Floater.GetPosition();
		m_BitePos = Floater.GetBitePos();
		m_AttachedMobID = Floater.GetAttachedMobID();
		Floater.Destroy(true);
		return true;
	}

	bool CanPickup(void)       const { return m_CanPickup; }
	bool IsAttached(void)      const { return (m_AttachedMobID != cEntity::INVALID_ID); }
	UInt32 GetAttachedMobID(void) const { return m_AttachedMobID; }
	Vector3d GetPos(void)      const { return m_Pos; }
	Vector3d GetBitePos(void)  const { return m_BitePos; }

protected:
	bool m_CanPickup;
	UInt32 m_AttachedMobID;
	Vector3d m_Pos;
	Vector3d m_BitePos;
} ;





class cItemFishingRodHandler :
	public cItemHandler
{
	typedef cItemHandler super;

public:
	cItemFishingRodHandler(int a_ItemType) :
		super(a_ItemType)
	{
	}



	virtual bool OnItemUse(
		cWorld * a_World, cPlayer * a_Player, cBlockPluginInterface & a_PluginInterface, const cItem & a_Item,
		int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace
	) override
	{
		if (a_BlockFace != BLOCK_FACE_NONE)
		{
			return false;
		}

		auto & Random = GetRandomProvider();

		if (a_Player->IsFishing())
		{
			cFloaterCallback FloaterInfo;
			a_World->DoWithEntityByID(a_Player->GetFloaterID(), FloaterInfo);
			a_Player->SetIsFishing(false);

			if (FloaterInfo.IsAttached())
			{
				a_World->DoWithEntityByID(FloaterInfo.GetAttachedMobID(), [=](cEntity & a_Entity)
					{
						Vector3d Speed = a_Player->GetPosition() - a_Entity.GetPosition();
						a_Entity.AddSpeed(Speed);
						return true;
					}
				);
			}
			else if (FloaterInfo.CanPickup())
			{
				cItems Drops;
				int ItemCategory = Random.RandInt(99);
				if (ItemCategory <= 4)  // Treasures 5%
				{
					int Treasure = Random.RandInt(5);
					switch (Treasure)
					{
						case 0:
						{
							Drops.Add(cItem(E_ITEM_BOW));  // TODO: Enchantments
							break;
						}
						case 1:
						{
							Drops.Add(cItem(E_ITEM_BOOK));  // TODO: Enchanted book
							break;
						}
						case 2:
						{
							Drops.Add(cItem(E_ITEM_FISHING_ROD, 1, Random.RandInt<short>(50)));  // Fishing rod with durability. TODO: Enchantments on it
							break;
						}
						case 3:
						{
							Drops.Add(cItem(E_ITEM_NAME_TAG));
							break;
						}
						case 4:
						{
							Drops.Add(cItem(E_ITEM_SADDLE));
							break;
						}
						case 5:
						{
							Drops.Add(cItem(E_BLOCK_LILY_PAD));
							break;
						}
					}

					a_Player->GetStatManager().AddValue(statTreasureFished, 1);
				}
				else if (ItemCategory <= 14)  // Junk 10%
				{
					int Junk = Random.RandInt(70);
					if (Junk <= 1)
					{
						Drops.Add(cItem(E_ITEM_DYE, 10, 0));
					}
					else if (Junk <= 4)
					{
						Drops.Add(cItem(E_ITEM_BOW, 1, Random.RandInt<short>(64)));
					}
					else if (Junk <= 9)
					{
						Drops.Add(cItem(E_ITEM_STICK));
					}
					else if (Junk <= 14)
					{
						Drops.Add(cItem(E_ITEM_STRING));
					}
					else if (Junk <= 22)
					{
						Drops.Add(cItem(E_ITEM_BOWL));
					}
					else if (Junk <= 30)
					{
						Drops.Add(cItem(E_ITEM_LEATHER));
					}
					else if (Junk <= 38)
					{
						Drops.Add(cItem(E_ITEM_LEATHER_BOOTS));
					}
					else if (Junk <= 46)
					{
						Drops.Add(cItem(E_ITEM_ROTTEN_FLESH));
					}
					else if (Junk <= 54)
					{
						Drops.Add(cItem(E_ITEM_POTIONS));
					}
					else if (Junk <= 62)
					{
						Drops.Add(cItem(E_ITEM_BONE));
					}
					else if (Junk <= 70)
					{
						Drops.Add(cItem(E_BLOCK_TRIPWIRE_HOOK));
					}

					a_Player->GetStatManager().AddValue(statJunkFished, 1);
				}
				else  // Fish
				{
					int FishType = Random.RandInt(99);
					if (FishType <= 1)  // Clownfish has a 2% chance of spawning
					{
						Drops.Add(cItem(E_ITEM_RAW_FISH, 1, E_META_RAW_FISH_CLOWNFISH));
					}
					else if (FishType <= 12)  // Pufferfish has a 13% chance of spawning
					{
						Drops.Add(cItem(E_ITEM_RAW_FISH, 1, E_META_RAW_FISH_CLOWNFISH));
					}
					else if (FishType <= 24)
					{
						Drops.Add(cItem(E_ITEM_RAW_FISH, 1, E_META_RAW_FISH_SALMON));
					}
					else
					{
						Drops.Add(cItem(E_ITEM_RAW_FISH, 1, E_META_RAW_FISH_FISH));
					}

					a_Player->GetStatManager().AddValue(statFishCaught, 1);
				}

				if (cRoot::Get()->GetPluginManager()->CallHookPlayerFishing(*a_Player, Drops))
				{
					return true;
				}
				Vector3d FloaterPos = FloaterInfo.GetBitePos();
				FloaterPos.y += 0.5f;
				const float FISH_SPEED_MULT = 2.25f;

				Vector3d FlyDirection = a_Player->GetEyePosition() - FloaterPos;
				a_World->SpawnItemPickups(Drops, FloaterPos.x, FloaterPos.y, FloaterPos.z, FlyDirection.x * FISH_SPEED_MULT, (FlyDirection.y + 1.0f) * FISH_SPEED_MULT, FlyDirection.z * FISH_SPEED_MULT);
				cRoot::Get()->GetPluginManager()->CallHookPlayerFished(*a_Player, Drops);
			}
		}
		else
		{
			auto Floater = cpp14::make_unique<cFloater>(a_Player->GetPosX(), a_Player->GetStance(), a_Player->GetPosZ(), a_Player->GetLookVector() * 15, a_Player->GetUniqueID(), (Random.RandInt(100, 900) - static_cast<int>(a_Player->GetEquippedItem().m_Enchantments.GetLevel(cEnchantments::enchLure) * 100)));
			auto FloaterPtr = Floater.get();
			if (!FloaterPtr->Initialize(std::move(Floater), *a_World))
			{
				return false;
			}
			a_Player->SetIsFishing(true, FloaterPtr->GetUniqueID());
		}
		return true;
	}
} ;




