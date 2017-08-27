
#pragma once

#include "PassiveMonster.h"
#include "Blocks/ChunkInterface.h"
#include "../UI/WindowOwner.h"





struct VillagerTradeOffer
{
	VillagerTradeOffer(const cItem & PrimaryTradeDesire, const cItem & TradeRecompense) :
		PrimaryDesire(PrimaryTradeDesire),
		Recompense(TradeRecompense),
		Transactions(0),
		MaximumTransactions(10)
	{
	}

	VillagerTradeOffer(const cItem & PrimaryTradeDesire, const cItem & SecondaryTradeDesire, const cItem & TradeRecompense) :
		PrimaryDesire(PrimaryTradeDesire),
		SecondaryDesire(SecondaryTradeDesire),
		Recompense(TradeRecompense),
		Transactions(0),
		MaximumTransactions(10)
	{
	}

	cItem PrimaryDesire;
	cItem SecondaryDesire;
	cItem Recompense;

	bool IsTradeExhausted() const
	{
		return Transactions == MaximumTransactions;
	}

	unsigned Transactions;
	unsigned MaximumTransactions;
};





class cVillager :
	public cPassiveMonster,
	public cEntityWindowOwner
{
	typedef cPassiveMonster super;

public:

	enum class VillagerProfession
	{
		Farmer = 0,
		Librarian = 1,
		Priest = 2,
		Blacksmith = 3,
		Butcher = 4,
		Nitwit = 5
	};

	enum class VillagerCareer
	{
		Farmer,
		Fisherman,
		Shepherd,
		Fletcher,
		Librarian,
		Cartographer,
		Cleric,
		Armorer,
		WeaponSmith,
		ToolSmith,
		Butcher,
		Leatherworker,
		Nitwit
	};

	cVillager(VillagerCareer, unsigned);
	CLASS_PROTODEF(cVillager)

	/** Converts internal career to equivalent Vanilla profession.
	Returns an enumeration with Vanilla-compatible values. */
	static VillagerProfession VillagerCareerToProfession(VillagerCareer);

	/** Returns the career of the villager. */
	VillagerCareer GetCareer() const { return m_Career; }

	/** Returns a trading offer of the villager subject to criteria.
	If the player has selected the first trade offer, the first available trade offer matching the input is returned, or the first trade offer if nothing was found.
	For any other offer selection, the selected offer is returned. */
	VillagerTradeOffer & GetTradeOffer(unsigned, const cItem &, const cItem &);

	/** Returns number of transactions of a single trade can be completed, given input and output items. */
	static unsigned GetTransactionMultiplier(const VillagerTradeOffer &, const cItem &, const cItem &);

	/** Handles events where players interact with an available trade.
	This includes adding a desired item to fulfil or exceed the offer, or otherwise interacting with a trade when it can be performed. */
	void HandleTradeAvailable() const;

	/** Handles events where players interact with the trading window.
	This event does not include interactions with the currently presented trade itself. */
	void HandleTradeInProgress() const;

	/** Handles events where players interact with an unavailable trade.
	This includes adding a desired item to fulfil or exceed the offer, or otherwise interacting with a trade when it can be performed. */
	void HandleTradeUnavailable() const;

	/** Convenience function for setting correct item counts and upgrading trade tiers if necessary. */
	void HandleTransaction(VillagerTradeOffer &, unsigned TransactionMultiplier);

	// cEntity overrides
	virtual bool DoTakeDamage(TakeDamageInfo & a_TDI) override;
	virtual void Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk) override;
	virtual void OnRightClicked(cPlayer &) override;

private:

	/** return true if the given blocktype are: crops, potatoes or carrots. */
	bool IsBlockFarmable(BLOCKTYPE a_BlockType);

	// Farmer functions
	/** Searches in a 11x7x11 area for crops. If it found some it will navigate to them. */
	void HandleFarmerPrepareFarmCrops();

	/** Looks if the farmer has reached it's destination, and if it's still crops and the destination is closer then 2 blocks it will harvest them. */
	void HandleFarmerTryHarvestCrops();

	/** Replaces the crops he harvested. */
	void HandleFarmerPlaceCrops();

	// Get and set functions.
	Vector3i GetCropsPos(void) const { return m_CropsPos; }
	bool DoesHaveActionActivated(void) const { return m_VillagerAction; }

	/** Levels-up the villager to the given tier, if not already at maximum for their career.
	Levelling-up unlocks additional trades available at that level, and re-enables all previously exhausted trade offers. */
	void UpdateTradeTier(size_t);

	int m_ActionCountDown;
	VillagerCareer m_Career;
	size_t m_TradeTier;
	std::vector<VillagerTradeOffer> m_TradeOffers;
	bool m_VillagerAction;
	Vector3i m_CropsPos;

};
