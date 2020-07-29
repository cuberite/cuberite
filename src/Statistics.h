
// Statistics.h




#pragma once




// tolua_begin
enum eStatistic
{
	// The order must match the order of cStatInfo::ms_Info

	statInvalid = -1,

	/* Achievements */
	achOpenInv,           /* Taking Inventory     */
	achMineWood,          /* Getting Wood         */
	achCraftWorkbench,    /* Benchmarking         */
	achCraftPickaxe,      /* Time to Mine!        */
	achCraftFurnace,      /* Hot Topic            */
	achAcquireIron,       /* Acquire Hardware     */
	achCraftHoe,          /* Time to Farm!        */
	achMakeBread,         /* Bake Bread           */
	achBakeCake,          /* The Lie              */
	achCraftBetterPick,   /* Getting an Upgrade   */
	achCookFish,          /* Delicious Fish       */
	achOnARail,           /* On A Rail            */
	achCraftSword,        /* Time to Strike!      */
	achKillMonster,       /* Monster Hunter       */
	achKillCow,           /* Cow Tipper           */
	achFlyPig,            /* When Pigs Fly        */
	achSnipeSkeleton,     /* Sniper Duel          */
	achDiamonds,          /* DIAMONDS!            */
	achEnterPortal,       /* We Need to Go Deeper */
	achReturnToSender,    /* Return to Sender     */
	achBlazeRod,          /* Into Fire            */
	achBrewPotion,        /* Local Brewery        */
	achEnterTheEnd,       /* The End?             */
	achDefeatDragon,      /* The End.             */
	achCraftEnchantTable, /* Enchanter            */
	achOverkill,          /* Overkill             */
	achBookshelf,         /* Librarian            */
	achExploreAllBiomes,  /* Adventuring Time     */
	achSpawnWither,       /* The Beginning?       */
	achKillWither,        /* The Beginning.       */
	achFullBeacon,        /* Beaconator           */
	achBreedCow,          /* Repopulation         */
	achThrowDiamonds,     /* Diamonds to you!     */

	/* Statistics */
	statGamesQuit,
	statMinutesPlayed,
	statDistWalked,
	statDistSwum,
	statDistFallen,
	statDistClimbed,
	statDistFlown,
	statDistDove,
	statDistMinecart,
	statDistBoat,
	statDistPig,
	statDistHorse,
	statJumps,
	statItemsDropped,
	statDamageDealt,
	statDamageTaken,
	statDeaths,
	statMobKills,
	statAnimalsBred,
	statPlayerKills,
	statFishCaught,
	statJunkFished,
	statTreasureFished,

	statCount
};
// tolua_end

// 12xx12_begin
/*
 * https://wiki.vg/Protocol#Statistics
 * added for better readability in transmission code
 */
enum statisticType{
	statMined,
	statCrafted,
	statUsed,
	statBroken,
	statPickedUp,
	statDropped,
	statKilled,
	statKilledBy,
	statCustom
};
	/*
	 * a list of all avaiable stats as of 1.13.2 ordered by id for transmission
	enum stat {
		statGamesQuit,					// 0
		statMinutesPlayed,				// 1
		statTimeSinceDeath,				// 2 Todo: implement this
	 	statTimeSinceRest,				// 3 Todo: implement this
	 	statSneakTime,					// 4 Todo: implement this
	 	statDistWalked,					// 5
		statDistCrouched,				// 6 Todo: implement this
		statDistSprinted,				// 7 Todo: implement this
		statDistWalkedOnWater,			// 8 Todo: implement this
	 	statDistFallen,					// 9
		statDistClimbed,				// 10
	 	statDistFlown,					// 11
	 	statDistDove,					// 12
		statDistMinecart,				// 13
	 	statDistBoat,					// 14
	 	statDistPig,					// 15
		statDistHorse,					// 16
	 	statDistElytra,					// 17 Todo: implement this
		statDistSwum,					// 18
	 	statJumps,						// 19
	 	statItemsDropped,				// 20
	 	statDamageDealt,				// 21
		statDamageDealtAbsorbed,		// 22 Todo: implement this
		statDamageDealtResisted,		// 23 Todo: implement this
		statDamageTaken,				// 24 Todo: implement this
		statDamageBlockedShield,		// 25 Todo: implement this
	 	statDamageAbsorbed, 			// 26 Todo: implement this
	 	statDamageResisted,				// 27 Todo: implement this
	 	statDeaths,						// 28 Todo: implement this
		statMobKills,					// 29
	 	statAnimalsBred,				// 30
	 	statPlayerKills,				// 31
		statFishCaught,					// 32
		statTalkedToVillager,			// 33 Todo: implement this
	 	statTradedWithVillager,			// 34 Todo: implement this
		statAteCakeSlice,				// 35 Todo: implement this
	 	statFillCauldron,				// 36 Todo: implement this
		statUseCauldron,				// 37 Todo: implement this
		statCleanedArmor,				// 38 Todo: implement this
		statCleanedBanner,				// 39 Todo: implement this
		statCleanedShulker,				// 40 Todo: implement this
	 	statInteractBrewingstand,		// 41 Todo: implement this
		statInteractBeacon,				// 42 Todo: implement this
		statInpectDropper,				// 43 Todo: implement this
		statInpectHopper				// 44 Todo: implement this
		statInpectDispenser,			// 45 Todo: implement this
		statPlayNoteblock,				// 46 Todo: implement this
	 	statTuneNoteblock,				// 47 Todo: implement this
		statPotFlower,					// 48 Todo: implement this
	 	statTriggeredTrappedChests,		// 49 Todo: implement this
	 	statOpenEnderchest,				// 50 Todo: implement this
		statEnchantItem,				// 51 Todo: implement this
		statPlayedRecord,				// 52 Todo: implement this
	 	statInteractFurnace,			// 53 Todo: implement this
	 	statInteractCraftingTable,		// 54 Todo: implement this
		statOpenChest,					// 55 Todo: implement this
	 	statSleepInBed,					// 56 Todo: implement this
	 	statOpenShulkerBox,				// 57 Todo: implement this
	};
	*/
// 12xx12_end



/** Class used to store and query statistic-related information. */
class cStatInfo
{
public:

	cStatInfo();

	cStatInfo(const eStatistic a_Type, const AString & a_Name, const eStatistic a_Depends = statInvalid);

	/** Type -> Name */
	static const AString & GetName(const eStatistic a_Type);

	/** Name -> Type */
	static eStatistic GetType(const AString & a_Name);

	/** Returns stat prerequisite. (Used for achievements) */
	static eStatistic GetPrerequisite(const eStatistic a_Type);

private:

	eStatistic m_Type;

	AString m_Name;

	eStatistic m_Depends;

	static cStatInfo ms_Info[statCount];
};




/* Signed (?) integral value. */
typedef int StatValue;  // tolua_export




/** Class that manages the statistics and achievements of a single player. */
// tolua_begin
class cStatManager
{
public:
	// tolua_end

	cStatManager();

	// tolua_begin

	/** Return the value of the specified stat. */
	StatValue GetValue(const eStatistic a_Stat) const;

	/** Set the value of the specified stat. */
	void SetValue(const eStatistic a_Stat, const StatValue a_Value);

	/** Reset everything. */
	void Reset();

	/** Increments the specified stat.
	Returns the new value.
	*/
	StatValue AddValue(const eStatistic a_Stat, const StatValue a_Delta = 1);

	// tolua_end

private:

	StatValue m_MainStats[statCount];

	// TODO 10-05-2014 xdot: Use, mine, craft statistics


};  // tolua_export



