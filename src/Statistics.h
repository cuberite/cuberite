
// Statistics.h




#pragma once




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



