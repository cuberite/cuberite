
// Utils.h

// Interfaces to utility functions





#pragma once





enum eEntityType
{
	entBat,
	entBlaze,
	entCaveSpider,
	entChicken,
	entCow,
	entCreeper,
	entEnderDragon,
	entEnderman,
	entGhast,
	entGiant,
	entLavaSlime,
	entMushroomCow,
	entOzelot,
	entPig,
	entPigZombie,
	entSheep,
	entSilverfish,
	entSkeleton,
	entSlime,
	entSnowMan,
	entSpider,
	entSquid,
	entVillager,
	entVillagerGolem,
	entWitch,
	entWitherBoss,
	entWolf,
	entZombie,
	entUnknown,
	entMax = entUnknown,
} ;





extern const char * GetBiomeString(unsigned char a_Biome);
extern const char * GetBlockTypeString(unsigned char a_BlockType);
extern eEntityType GetEntityType(const AString & a_EntityTypeString);
extern const char * GetEntityTypeString(eEntityType a_EntityType);
extern int GetNumCores(void);



