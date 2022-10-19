#pragma once





enum class ePoiType
{
	// Villagers
	poiArmorer,  // Blast furnace
	poiButcher,  // Smoker
	poiCartographer,  // Cartography table
	poiCleric,  // Brewing stand
	poiFarmer,  // Composter
	poiFisherman,  // Barrel
	poiFletcher,  // Fletching table
	poiLetherworker,  // Cauldron
	poiLibrairian,  // Lectern
	poiMason,  // Stonecutter
	poiShepherd,  // Loom
	poiToolsmith,  // Smithing table
	poiWeaponsmith,  // Grindstone
	poiHome,  // Bed
	poiMeeting,  // Bell

	// Bees
	poiBeehive,  // Beehive
	poiBeeNest,  // Bee nest

	// Others
	poiNetherPortal,  // Nether portal (used for faster portal travel)
	poiLodestone,  // Lodestone (used for player respawn)
	poiLightningRod,  // Lightning rod (used for lightnings)

	poiNone
};
