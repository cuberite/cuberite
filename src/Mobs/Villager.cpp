
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Villager.h"
#include "../UI/VillagerTradeWindow.h"
#include "../Entities/Player.h"
#include "../World.h"
#include "../BlockArea.h"
#include "../Blocks/BlockHandler.h"
#include "../BlockInServerPluginInterface.h"
#include "../ClientHandle.h"





cVillager::cVillager(VillagerCareer Career, unsigned TradeTier) :
	super("Villager", mtVillager, "entity.villager.hurt", "entity.villager.death", 0.6, 1.8),
	cEntityWindowOwner(this),
	m_ActionCountDown(-1),
	m_Career(Career),
	m_TradeTier(TradeTier),
	m_VillagerAction(false)
{
	UpdateTradeTier(m_TradeTier);
}





cVillager::VillagerProfession cVillager::VillagerCareerToProfession(VillagerCareer Career)
{
	switch (Career)
	{
		case VillagerCareer::Farmer:
		case VillagerCareer::Fisherman:
		case VillagerCareer::Shepherd:
		case VillagerCareer::Fletcher: return cVillager::VillagerProfession::Farmer;
		case VillagerCareer::Librarian:
		case VillagerCareer::Cartographer: return cVillager::VillagerProfession::Librarian;
		case VillagerCareer::Cleric:  return cVillager::VillagerProfession::Priest;
		case VillagerCareer::Armorer:
		case VillagerCareer::WeaponSmith:
		case VillagerCareer::ToolSmith: return cVillager::VillagerProfession::Blacksmith;
		case VillagerCareer::Butcher:
		case VillagerCareer::Leatherworker: return cVillager::VillagerProfession::Butcher;
		case VillagerCareer::Nitwit: return cVillager::VillagerProfession::Nitwit;
	}

	throw std::runtime_error("Unhandled career");
}





bool cVillager::DoTakeDamage(TakeDamageInfo & a_TDI)
{
	if (!super::DoTakeDamage(a_TDI))
	{
		return false;
	}

	if ((a_TDI.Attacker != nullptr) && a_TDI.Attacker->IsPlayer())
	{
		m_World->BroadcastEntityStatus(*this, esVillagerAngry);
	}

	if (a_TDI.DamageType == dtLightning)
	{
		Destroy();
		m_World->SpawnMob(GetPosX(), GetPosY(), GetPosZ(), mtWitch, false);
		return true;
	}
	return true;
}





void cVillager::Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	super::Tick(a_Dt, a_Chunk);
	if (!IsTicking())
	{
		// The base class tick destroyed us
		return;
	}

	if (m_ActionCountDown > -1)
	{
		m_ActionCountDown--;
		if ((m_ActionCountDown == 0) && (m_Career == VillagerCareer::Farmer))
		{
			HandleFarmerPlaceCrops();
		}
		return;
	}

	if (m_VillagerAction)
	{
		if (m_Career == VillagerCareer::Farmer)
		{
			HandleFarmerTryHarvestCrops();
		}
		m_VillagerAction = false;
		return;
	}

	// Don't always try to do a special action. Each tick has 1% to do a special action.
	if (GetRandomProvider().RandBool(0.99))
	{
		return;
	}

	if (m_Career == VillagerCareer::Farmer)
	{
		HandleFarmerPrepareFarmCrops();
	}
}





void cVillager::OnRightClicked(cPlayer & InteractingPlayer)
{
	if (m_TradeOffers.empty())
	{
		// Client handles this ok, but the contract of GetTradeOffer says it must return something
		return;
	}

	// If the window is not created, open it anew:
	if (GetWindow() == nullptr)
	{
		OpenWindow(new cVillagerTradeWindow(*this));
	}

	InteractingPlayer.OpenWindow(*GetWindow());
	InteractingPlayer.GetClientHandle()->SendVillagerTradeList(*GetWindow(), m_TradeOffers);
	GetWorld()->BroadcastSoundEffect("entity.villager.trading", GetPosX(), GetPosY(), GetPosZ(), 1, 1);
}





VillagerTradeOffer & cVillager::GetTradeOffer(unsigned PlayerOfferIndex, const cItem & PrimarySlot, const cItem & SecondarySlot)
{
	if ((PlayerOfferIndex != 0) || GetTransactionMultiplier(m_TradeOffers[PlayerOfferIndex], PrimarySlot, SecondarySlot) != 0)
	{
		return m_TradeOffers[PlayerOfferIndex];
	}

	for (auto & Offer : m_TradeOffers)
	{
		if (GetTransactionMultiplier(Offer, PrimarySlot, SecondarySlot) != 0)
		{
			return Offer;
		}
	}

	return m_TradeOffers[PlayerOfferIndex];
}





unsigned cVillager::GetTransactionMultiplier(const VillagerTradeOffer & Trade, const cItem & PrimarySlot, const cItem & SecondarySlot)
{
	if (Trade.IsTradeExhausted())
	{
		return 0;
	}

	if (
		PrimarySlot.IsEqual(Trade.PrimaryDesire) &&
		(PrimarySlot.m_ItemCount >= Trade.PrimaryDesire.m_ItemCount) &&
		SecondarySlot.IsEqual(Trade.SecondaryDesire) &&
		(SecondarySlot.m_ItemCount >= Trade.SecondaryDesire.m_ItemCount)
	)
	{
		// Slots matched normally: find common denominator of transaction multiplier
		return std::min(
			{
				static_cast<unsigned>(PrimarySlot.m_ItemCount / Trade.PrimaryDesire.m_ItemCount),
				Trade.SecondaryDesire.IsEmpty() ? std::numeric_limits<unsigned>::max() : static_cast<unsigned>(SecondarySlot.m_ItemCount / Trade.SecondaryDesire.m_ItemCount),
				static_cast<unsigned>(Trade.Recompense.GetMaxStackSize() / Trade.Recompense.m_ItemCount)
			}
		);
	}
	else if (
		Trade.SecondaryDesire.IsEmpty() &&
		PrimarySlot.IsEmpty() &&
		SecondarySlot.IsEqual(Trade.PrimaryDesire) &&
		(SecondarySlot.m_ItemCount >= Trade.PrimaryDesire.m_ItemCount)
	)
	{
		// Slots crossed - primary desire in secondary slot: swap comparison accordingly
		return std::min(
			static_cast<unsigned>(SecondarySlot.m_ItemCount / Trade.PrimaryDesire.m_ItemCount),
			static_cast<unsigned>(Trade.Recompense.GetMaxStackSize() / Trade.Recompense.m_ItemCount)
		);
	}

	return 0;
}





void cVillager::HandleTradeAvailable() const
{
	GetWorld()->BroadcastSoundEffect("entity.villager.yes", GetPosX(), GetPosY(), GetPosZ(), 1, 1);
}





void cVillager::HandleTradeInProgress() const
{
	GetWorld()->BroadcastSoundEffect("entity.villager.trading", GetPosX(), GetPosY(), GetPosZ(), 1, 1);
}





void cVillager::HandleTradeUnavailable() const
{
	GetWorld()->BroadcastSoundEffect("entity.villager.no", GetPosX(), GetPosY(), GetPosZ(), 1, 1);
}





void cVillager::HandleTransaction(VillagerTradeOffer & Trade, unsigned TransactionMultiplier)
{
	GetWorld()->BroadcastSoundEffect("entity.villager.yes", GetPosX(), GetPosY(), GetPosZ(), 1, 1);
	Trade.Transactions += TransactionMultiplier;

	for (unsigned Try = 0; Try != TransactionMultiplier; Try++)
	{
		// One singular transaction has a 10% chance of upgrading the villager's trading tier

		if (GetRandomProvider().RandBool(0.1))
		{
			UpdateTradeTier(m_TradeTier + 1);

			class SendNewTradesCallback : public cItemCallback<cClientHandle>
			{
			public:
				SendNewTradesCallback(const cWindow & VillagerTradeWindow, const std::vector<VillagerTradeOffer> & VillagerTradeOffers) :
					TradeWindow(VillagerTradeWindow),
					TradeOffers(VillagerTradeOffers)
				{
				}

				bool Item(cClientHandle * Handle)
				{
					Handle->SendVillagerTradeList(TradeWindow, TradeOffers);
					return false;
				}

			private:
				const cWindow & TradeWindow;
				const std::vector<VillagerTradeOffer> & TradeOffers;

			} SNTC(*GetWindow(), m_TradeOffers);
			GetWindow()->ForEachClient(SNTC);

			m_World->BroadcastEntityStatus(*this, esVillagerHappy);
			break;
		}
	}
}





////////////////////////////////////////////////////////////////////////////////
// Farmer functions:

void cVillager::HandleFarmerPrepareFarmCrops()
{
	if (!m_World->VillagersShouldHarvestCrops())
	{
		return;
	}

	cBlockArea Surrounding;

	// Read a 11x7x11 area:
	Surrounding.Read(
		*m_World,
		FloorC(GetPosX()) - 5,
		FloorC(GetPosX()) + 6,
		FloorC(GetPosY()) - 3,
		FloorC(GetPosY()) + 4,
		FloorC(GetPosZ()) - 5,
		FloorC(GetPosZ()) + 6
	);

	for (int I = 0; I < 5; I++)
	{
		for (int Y = 0; Y < 6; Y++)
		{
			// Pick random coordinates and check for crops.
			int X = m_World->GetTickRandomNumber(11);
			int Z = m_World->GetTickRandomNumber(11);

			// A villager can't farm this.
			if (!IsBlockFarmable(Surrounding.GetRelBlockType(X, Y, Z)))
			{
				continue;
			}
			if (Surrounding.GetRelBlockMeta(X, Y, Z) != 0x7)
			{
				continue;
			}

			m_VillagerAction = true;
			m_CropsPos = Vector3i(static_cast<int>(GetPosX()) + X - 5, static_cast<int>(GetPosY()) + Y - 3, static_cast<int>(GetPosZ()) + Z - 5);
			MoveToPosition(Vector3d(m_CropsPos.x + 0.5, m_CropsPos.y + 0.0, m_CropsPos.z + 0.5));
			return;
		}  // for Y loop.
	}  // Repeat the procces 5 times.
}





void cVillager::HandleFarmerTryHarvestCrops()
{
	// Harvest the crops if the villager isn't moving and if the crops are closer then 2 blocks.
	if (!m_PathfinderActivated && (GetPosition() - m_CropsPos).Length() < 2)
	{
		// Check if the blocks didn't change while the villager was walking to the coordinates.
		BLOCKTYPE CropBlock = m_World->GetBlock(m_CropsPos.x, m_CropsPos.y, m_CropsPos.z);
		if (IsBlockFarmable(CropBlock) && m_World->GetBlockMeta(m_CropsPos.x, m_CropsPos.y, m_CropsPos.z) == 0x7)
		{
			cBlockHandler * Handler = cBlockInfo::GetHandler(CropBlock);
			cChunkInterface ChunkInterface(m_World->GetChunkMap());
			cBlockInServerPluginInterface PluginInterface(*m_World);
			Handler->DropBlock(ChunkInterface, *m_World, PluginInterface, this, m_CropsPos.x, m_CropsPos.y, m_CropsPos.z);
			m_World->SetBlock(m_CropsPos.x, m_CropsPos.y, m_CropsPos.z, E_BLOCK_AIR, 0);
			m_ActionCountDown = 20;
		}
	}
}




void cVillager::HandleFarmerPlaceCrops()
{
	// Check if there is still farmland at the spot where the crops were.
	if (m_World->GetBlock(m_CropsPos.x, m_CropsPos.y - 1, m_CropsPos.z) == E_BLOCK_FARMLAND)
	{
		m_World->SetBlock(m_CropsPos.x, m_CropsPos.y, m_CropsPos.z, E_BLOCK_CROPS, 0);
	}
}





void cVillager::UpdateTradeTier(size_t DesiredTier)
{
	struct CareerHash
	{
		size_t operator()(const VillagerCareer & Career) const
		{
			return static_cast<size_t>(Career);
		}
	};
	using Offer = VillagerTradeOffer;

	static const std::unordered_map<
		VillagerCareer,
		std::vector<
			// Element index: trading tier (0-based)
			std::vector<Offer>  // Trade offer
		>,
		CareerHash  // TODO for C++14: unneeded
	> TradeMatrix =
	{
		{
			VillagerCareer::Farmer,
			{
				{  // Tier 1 (equivalent)
					Offer(cItem(E_ITEM_WHEAT, 18), cItem(E_ITEM_EMERALD)),
					Offer(cItem(E_ITEM_POTATO, 15), cItem(E_ITEM_EMERALD)),
					Offer(cItem(E_ITEM_CARROT, 15), cItem(E_ITEM_EMERALD))
				},
				{  // Tier 2
					Offer(cItem(E_BLOCK_PUMPKIN, 8), cItem(E_ITEM_EMERALD)),
					Offer(cItem(E_ITEM_EMERALD), cItem(E_ITEM_PUMPKIN_PIE, 2))
				},
				{  // Tier 3
					Offer(cItem(E_BLOCK_MELON, 7), cItem(E_ITEM_EMERALD)),
					Offer(cItem(E_ITEM_EMERALD), cItem(E_ITEM_RED_APPLE, 5))
				},
				{  // Tier 4
					Offer(cItem(E_ITEM_EMERALD), cItem(E_ITEM_COOKIE, 6)),
					Offer(cItem(E_ITEM_EMERALD), cItem(E_ITEM_CAKE))
				}
			}
		},
		{
			VillagerCareer::Fisherman, {}
		},
		{
			VillagerCareer::Shepherd, {}
		},
		{
			VillagerCareer::Fletcher, {}
		},
		{
			VillagerCareer::Librarian, {}
		},
		{
			VillagerCareer::Cartographer, {}
		},
		{
			VillagerCareer::Cleric, {}
		},
		{
			VillagerCareer::Armorer, {}
		},
		{
			VillagerCareer::WeaponSmith, {}
		},
		{
			VillagerCareer::ToolSmith, {}
		},
		{
			VillagerCareer::Butcher, {}
		},
		{
			VillagerCareer::Leatherworker, {}
		},
		{
			VillagerCareer::Nitwit, {}
		}
	};

	auto Tiers = TradeMatrix.find(m_Career)->second;

	m_TradeTier = std::min(Tiers.size(), DesiredTier);
	m_TradeOffers.clear();

	for (unsigned Tier = 0; Tier < m_TradeTier; Tier++)
	{
		m_TradeOffers.insert(m_TradeOffers.end(), Tiers[Tier].cbegin(), Tiers[Tier].cend());
	}
}





bool cVillager::IsBlockFarmable(BLOCKTYPE a_BlockType)
{
	switch (a_BlockType)
	{
		case E_BLOCK_BEETROOTS:
		case E_BLOCK_CROPS:
		case E_BLOCK_POTATOES:
		case E_BLOCK_CARROTS:
		{
			return true;
		}
	}
	return false;
}

