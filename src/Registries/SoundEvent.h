# pragma once
enum class eSoundEvent
{
	NullValue,
	// Weather
	LightningboltThunder,

	// Explosions
	EntityGenericExplode,

	// Mobs
	EntityBlazeAmbient,
	EntityBlazeHurt,
	EntityBlazeDeath,
	EntityBlazeShoot,

	EntityHorseAmbient,
	EntityHorseHurt,
	EntityHorseDeath,
	EntityHorseAngry,
	EntityHorseSaddle,
	EntityHorseArmor,

	EntityGhastAmbient,
	EntityGhastHurt,
	EntityGhastDeath,
	EntityGhastWarn,
	EntityGhastShoot,

	EntityCreeperHurt,
	EntityCreeperDeath,
	EntityCreeperPrimed,

	EntityZombievillagerAmbient,
	EntityZombievillagerHurt,
	EntityZombievillagerDeath,
	EntityZombievillagerCure,

	EntitySheepAmbient,
	EntitySheepHurt,
	EntitySheepDeath,
	EntitySheepShear,

	// Blocks
	BlockChestOpen,
	BlockChestClose,
	BlockLeverClick,
	BlockDispenserFail,
	BlockDispenserDispense,
	BlockPistonExtend,
	BlockPistonContract,
	BlockWoodbtnClickon,
	BlockBrewingstandBrew,

	// Items
	ItemPickup,
	ItemHoeTill,
	ItemFirechargeUse,
	ItemFlintsteelUse,
	ItemChorusfruitTeleport,

	// Entity Items
	EnderEyeCustomSurvive,
	EnderEyeDeath,
	EnderEyeLaunch,
	SplashPotionThrow,
	PaintingBreak,
	PaintingPlace,
	LeashknotPlace,
	LeashknotBreak,
	ArrowHit,
	ArrowShoot,
	ExporbPickup,
	FishingbobberSplash,
	TntPrimed,
};
