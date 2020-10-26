
// Utils.cpp

// Implements utility functions

#include "Globals.h"
#include "Utils.h"





struct
{
	eEntityType  Type;
	const char * String;
} g_EntityTypes[] =
{
	{entBat,            "Bat"},
	{entBlaze,          "Blaze"},
	{entCaveSpider,     "CaveSpider"},
	{entChicken,        "Chicken"},
	{entCow,            "Cow"},
	{entCreeper,        "Creeper"},
	{entEnderDragon,    "EnderDragon"},
	{entEnderman,       "Enderman"},
	{entGhast,          "Ghast"},
	{entGiant,          "Giant"},
	{entGuardian,       "Guardian"},
	{entLavaSlime,      "LavaSlime"},
	{entMushroomCow,    "MushroomCow"},
	{entOzelot,         "Ozelot"},
	{entPig,            "Pig"},
	{entPigZombie,      "PigZombie"},
	{entRabbit,         "Rabbit"},
	{entSheep,          "Sheep"},
	{entSilverfish,     "Slverfish"},
	{entSkeleton,       "Skeleton"},
	{entSlime,          "Slime"},
	{entSnowMan,        "SnowMan"},
	{entSpider,         "Spider"},
	{entSquid,          "Squid"},
	{entVillager,       "Villager"},
	{entVillagerGolem,  "VillagerGolem"},
	{entWitch,          "Witch"},
	{entWitherBoss,     "WitherBoss"},
	{entWolf,           "Wolf"},
	{entZombie,         "Zombie"},
	{entZombieVillager, "ZombieVillager"},
	{entUnknown,        "Unknown"},
} ;





const char * GetBiomeString(unsigned char a_Biome)
{
	static const char * BiomeNames[] =  // Biome names, as equivalent to their index
	{
		"Ocean",
		"Plains",
		"Desert",
		"Extreme Hills",
		"Forest",
		"Taiga",
		"Swampland",
		"River",
		"Hell",
		"Sky",
		"Frozen Ocean",
		"Frozen River",
		"Ice Plains",
		"Ice Mountains",
		"Mushroom Island",
		"Mushroom Island Shore",
		"Beach",
		"Desert Hills",
		"Forest Hills",
		"Taiga Hills",
		"Extreme Hills Edge",
		"Jungle",
		"Jungle Hills",
	} ;
	return (a_Biome < ARRAYCOUNT(BiomeNames)) ? BiomeNames[a_Biome] : "";
}





const char * GetBlockTypeString(unsigned char a_BlockType)
{
	static const char * BlockTypeNames[] =  // Block type names, as equivalent to their index
	{
		"air",
		"stone",
		"grass",
		"dirt",
		"cobblestone",
		"planks",
		"sapling",
		"bedrock",
		"water",
		"stillwater",
		"lava",
		"stilllava",
		"sand",
		"gravel",
		"goldore",
		"ironore",
		"coalore",
		"log",
		"leaves",
		"sponge",
		"glass",
		"lapisore",
		"lapisblock",
		"dispenser",
		"sandstone",
		"noteblock",
		"bedblock",
		"poweredrail",
		"detectorrail",
		"stickypiston",
		"cobweb",
		"tallgrass",
		"deadbush",
		"piston",
		"pistonhead",
		"wool",
		"pistonmovedblock",
		"flower",
		"rose",
		"brownmushroom",
		"redmushroom",
		"goldblock",
		"ironblock",
		"doubleslab",
		"slab",
		"brickblock",
		"tnt",
		"bookcase",
		"mossycobblestone",
		"obsidian",
		"torch",
		"fire",
		"mobspawner",
		"woodstairs",
		"chest",
		"redstonedust",
		"diamondore",
		"diamondblock",
		"workbench",
		"crops",
		"soil",
		"furnace",
		"litfurnace",
		"signblock",
		"wooddoorblock",
		"ladder",
		"tracks",
		"cobblestonestairs",
		"wallsign",
		"lever",
		"stoneplate",
		"irondoorblock",
		"woodplate",
		"redstoneore",
		"redstoneorealt",
		"redstonetorchoff",
		"redstonetorchon",
		"button",
		"snow",
		"ice",
		"snowblock",
		"cactus",
		"clayblock",
		"reedblock",
		"jukebox",
		"fence",
		"pumpkin",
		"netherrack",
		"soulsand",
		"glowstone",
		"portal",
		"jack-o-lantern",
		"cakeblock",
		"repeateroff",
		"repeateron",
		"lockedchest",
		"trapdoor",
		"silverfishblock",
		"stonebricks",
		"hugebrownmushroom",
		"hugeredmushroom",
		"ironbars",
		"glasspane",
		"melon",
		"pumpkinstem",
		"melonstem",
		"vines",
		"fencegate",
		"brickstairs",
		"stonebrickstairs",
		"mycelium",
		"lilypad",
		"netherbrick",
		"netherbrickfence",
		"netherbrickstairs",
		"netherwartblock",
		"enchantmenttable",
		"brewingstandblock",
		"cauldronblock",
		"endportal",
		"endportalframe",
		"endstone",
		"dragonegg",
		"redstonelampoff",
		"redstonelampon",
		"woodendoubleslab",
		"woodenslab",
		"cocoapod",
		"sandstonestairs",  /* 128 */
		"Emerald Ore",
		"Ender Chest",
		"Tripwire Hook",
		"Tripwire",
		"Block of Emerald",
		"Spruce Wood Stairs",
		"Birch Wood Stairs",
		"Jungle Wood Stairs",
		"Command Block",
		"Beacon",
		"Cobblestone Wall",
		"Flower Pot",
		"Carrots",
		"Potatoes",
		"Wooden Button",
		"Head",
	} ;

	return (a_BlockType < ARRAYCOUNT(BlockTypeNames)) ? BlockTypeNames[a_BlockType] : "";
}





eEntityType GetEntityType(const AString & a_EntityTypeString)
{
	for (int i = 0; i < ARRAYCOUNT(g_EntityTypes); i++)
	{
		if (a_EntityTypeString == g_EntityTypes[i].String)
		{
			return g_EntityTypes[i].Type;
		}
	}
	return entUnknown;
}





extern const char * GetEntityTypeString(eEntityType a_EntityType)
{
	return g_EntityTypes[a_EntityType].String;
}





int GetNumCores(void)
{
	// Get number of cores by querying the system process affinity mask (Windows-specific)
	DWORD_PTR Affinity, ProcAffinity;
	GetProcessAffinityMask(GetCurrentProcess(), &ProcAffinity, &Affinity);
	int NumCores = 0;
	while (Affinity > 0)
	{
		if ((Affinity & 1) == 1)
		{
			++NumCores;
		}
		Affinity >>= 1;
	}  // while (Affinity > 0)
	return NumCores;
}
