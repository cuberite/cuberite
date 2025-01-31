
#pragma once
#include "Registries/Items.h"




/** List of slot numbers, used for inventory-painting */
typedef std::vector<int> cSlotNums;





/** Constant to calculate ticks from seconds "ticks per second" */
constexpr inline const int TPS = 20;
// This is not added to the lua API because it broke the build




constexpr auto MAIN_HAND = 0;  // Temporary




// tolua_begin

/** Experience Orb setup */
enum
{
	// Open to suggestion on naming convention here :)
	MAX_EXPERIENCE_ORB_SIZE = 2000
};





enum class eParticleModeSetting
{
	All,
	Decreased,
	Minimal
};





/** Block face constants, used in PlayerDigging and PlayerBlockPlacement packets and bbox collision calc */
enum eBlockFace : char
{
	BLOCK_FACE_NONE = -1,  // Interacting with no block face - swinging the item in the air
	BLOCK_FACE_XM   =  4,  // Interacting with the X- face of the block
	BLOCK_FACE_XP   =  5,  // Interacting with the X+ face of the block
	BLOCK_FACE_YM   =  0,  // Interacting with the Y- face of the block
	BLOCK_FACE_YP   =  1,  // Interacting with the Y+ face of the block
	BLOCK_FACE_ZM   =  2,  // Interacting with the Z- face of the block
	BLOCK_FACE_ZP   =  3,  // Interacting with the Z+ face of the block

	// Synonyms using the (deprecated) world directions:
	BLOCK_FACE_BOTTOM = BLOCK_FACE_YM,  // Interacting with the bottom   face of the block
	BLOCK_FACE_TOP    = BLOCK_FACE_YP,  // Interacting with the top      face of the block
	BLOCK_FACE_NORTH  = BLOCK_FACE_ZM,  // Interacting with the northern face of the block
	BLOCK_FACE_SOUTH  = BLOCK_FACE_ZP,  // Interacting with the southern face of the block
	BLOCK_FACE_WEST   = BLOCK_FACE_XM,  // Interacting with the western  face of the block
	BLOCK_FACE_EAST   = BLOCK_FACE_XP,  // Interacting with the eastern  face of the block

	// Bounds, used for range-checking:
	BLOCK_FACE_MIN = -1,
	BLOCK_FACE_MAX =  5,
} ;





/** PlayerDigging status constants */
enum
{
	DIG_STATUS_STARTED           = 0,
	DIG_STATUS_CANCELLED         = 1,
	DIG_STATUS_FINISHED          = 2,
	DIG_STATUS_DROP_STACK        = 3,
	DIG_STATUS_DROP_HELD         = 4,
	DIG_STATUS_SHOOT_EAT         = 5,
	DIG_STATUS_SWAP_ITEM_IN_HAND = 6,
} ;





/** Individual actions sent in the WindowClick packet */
enum eClickAction
{
	// Sorted by occurrence in the 1.5 protocol
	caLeftClick,
	caRightClick,
	caShiftLeftClick,
	caShiftRightClick,
	caNumber1,
	caNumber2,
	caNumber3,
	caNumber4,
	caNumber5,
	caNumber6,
	caNumber7,
	caNumber8,
	caNumber9,
	caMiddleClick,
	caDropKey,
	caCtrlDropKey,
	caLeftClickOutside,
	caRightClickOutside,
	caLeftClickOutsideHoldNothing,
	caRightClickOutsideHoldNothing,
	caLeftPaintBegin,
	caRightPaintBegin,
	caMiddlePaintBegin,
	caLeftPaintProgress,
	caRightPaintProgress,
	caMiddlePaintProgress,
	caLeftPaintEnd,
	caRightPaintEnd,
	caMiddlePaintEnd,
	caDblClick,
	// Add new actions here
	caUnknown = 255,

	// Keep this list in sync with ClickActionToString() function below!
} ;





enum eGameMode
{
	eGameMode_NotSet    = -1,
	eGameMode_Survival  = 0,
	eGameMode_Creative  = 1,
	eGameMode_Adventure = 2,
	eGameMode_Spectator = 3,

	// Easier-to-use synonyms:
	gmNotSet    = eGameMode_NotSet,
	gmSurvival  = eGameMode_Survival,
	gmCreative  = eGameMode_Creative,
	gmAdventure = eGameMode_Adventure,
	gmSpectator = eGameMode_Spectator,

	// These two are used to check GameMode for validity when converting from integers.
	gmMax,  // Gets automatically assigned
	gmMin = 0,
} ;





enum eChatType
{
	ctChatBox        = 0,
	ctSystem         = 1,
	ctAboveActionBar = 2,
} ;





enum eWeather
{
	eWeather_Sunny        = 0,
	eWeather_Rain         = 1,
	eWeather_ThunderStorm = 2,

	// Easier-to-use synonyms:
	wSunny        = eWeather_Sunny,
	wRain         = eWeather_Rain,
	wThunderstorm = eWeather_ThunderStorm,
	wStorm        = wThunderstorm,
} ;





enum eMobHeadType  // TODO: remove this no longer used
{
	SKULL_TYPE_SKELETON    = 0,
	SKULL_TYPE_WITHER      = 1,
	SKULL_TYPE_ZOMBIE      = 2,
	SKULL_TYPE_PLAYER      = 3,
	SKULL_TYPE_CREEPER     = 4,
	SKULL_TYPE_DRAGON      = 5,
};





enum eMobHeadRotation
{
	SKULL_ROTATION_NORTH = 0,
	SKULL_ROTATION_NORTH_NORTH_EAST = 1,
	SKULL_ROTATION_NORTH_EAST = 2,
	SKULL_ROTATION_EAST_NORTH_EAST = 3,
	SKULL_ROTATION_EAST = 4,
	SKULL_ROTATION_EAST_SOUTH_EAST = 5,
	SKULL_ROTATION_SOUTH_EAST = 6,
	SKULL_ROTATION_SOUTH_SOUTH_EAST = 7,
	SKULL_ROTATION_SOUTH = 8,
	SKULL_ROTATION_SOUTH_SOUTH_WEST = 9,
	SKULL_ROTATION_SOUTH_WEST = 10,
	SKULL_ROTATION_WEST_SOUTH_WEST = 11,
	SKULL_ROTATION_WEST = 12,
	SKULL_ROTATION_WEST_NORTH_WEST = 13,
	SKULL_ROTATION_NORTH_WEST = 14,
	SKULL_ROTATION_NORTH_NORTH_WEST = 15,
} ;





enum eSkinPart
{
	spCape = 0x01,
	spJacket = 0x02,
	spLeftSleeve = 0x04,
	spRightSleeve = 0x08,
	spLeftPants = 0x10,
	spRightPants = 0x20,
	spHat = 0x40,
	spMask = 0x7F,
};




/** Dimension of a world */
enum eDimension
{
	dimNether    = -1,
	dimOverworld = 0,
	dimEnd       = 1,
	dimNotSet    = 255,  // For things that need an "indeterminate" state, such as cProtocol's LastSentDimension
} ;





/** Damage type, used in the TakeDamageInfo structure and related functions */
enum eDamageType
{
	// Canonical names for the types (as documented in the plugin wiki):
	dtAttack,           // Being attacked by a mob
	dtRangedAttack,     // Being attacked by a projectile, possibly from a mob
	dtLightning,        // Hit by a lightning strike
	dtFalling,          // Falling down; dealt when hitting the ground
	dtDrowning,         // Drowning in water / lava
	dtSuffocating,      // Suffocating inside a block
	dtStarving,         // Hunger
	dtCactusContact,    // Contact with a cactus block
	dtMagmaContact,     // Contact with a magma block
	dtLavaContact,      // Contact with a lava block
	dtPoisoning,        // Having the poison effect
	dtWithering,        // Having the wither effect
	dtOnFire,           // Being on fire
	dtFireContact,      // Standing inside a fire block
	dtInVoid,           // Falling into the Void (Y < 0)
	dtPotionOfHarming,
	dtEnderPearl,       // Thrown an ender pearl, teleported by it
	dtAdmin,            // Damage applied by an admin command
	dtExplosion,        // Damage applied by an explosion
	dtEnvironment,      // Damage dealt to mobs from environment: enderman in rain, snow golem in desert

	// Some common synonyms:
	dtPawnAttack   = dtAttack,
	dtEntityAttack = dtAttack,
	dtMob          = dtAttack,
	dtMobAttack    = dtAttack,
	dtArrowAttack  = dtRangedAttack,
	dtArrow        = dtRangedAttack,
	dtProjectile   = dtRangedAttack,
	dtFall         = dtFalling,
	dtDrown        = dtDrowning,
	dtSuffocation  = dtSuffocating,
	dtStarvation   = dtStarving,
	dtHunger       = dtStarving,
	dtCactus       = dtCactusContact,
	dtCactuses     = dtCactusContact,
	dtCacti        = dtCactusContact,
	dtMagma        = dtMagmaContact,
	dtLava         = dtLavaContact,
	dtPoison       = dtPoisoning,
	dtWither       = dtWithering,
	dtBurning      = dtOnFire,
	dtInFire       = dtFireContact,
	dtPlugin       = dtAdmin,
} ;





/** The source of an explosion.
Also dictates the type of the additional data passed to the explosion handlers:
| esBed           | Vector3i *             | Bed exploding in the Nether or in the End
| esEnderCrystal  | cEnderCrystal *        |
| esGhastFireball | cGhastFireballEntity * |
| esMonster       | cMonster *             |
| esOther         | nullptr                | Any other explosion unaccounted for
| esPlugin        | nullptr                | Explosion primarily attributed to a plugin
| esPrimedTNT     | cTNTEntity *           |
| esWitherBirth   | cMonster *             |
| esWitherSkull   | cProjectileEntity *    |
*/
enum eExplosionSource
{
	esBed,
	esEnderCrystal,
	esGhastFireball,
	esMonster,
	esOther,
	esPlugin,
	esPrimedTNT,
	esTNTMinecart,
	esWitherBirth,
	esWitherSkull,
	esMax,
} ;





enum eShrapnelLevel
{
	slNone,
	slGravityAffectedOnly,
	slAll
} ;





enum eSpreadSource
{
	ssFireSpread,
	ssGrassSpread,
	ssMushroomSpread,
	ssMycelSpread,
	ssVineSpread,
} ;





enum eMessageType
{
	// https://forum.cuberite.org/thread-1212.html
	// MessageType...

	mtCustom,          // Send raw data without any processing
	mtFailure,         // Something could not be done (i.e. command not executed due to insufficient privilege)
	mtInformation,     // Informational message (i.e. command usage)
	mtSuccess,         // Something executed successfully
	mtWarning,         // Something concerning (i.e. reload) is about to happen
	mtFatal,           // Something catastrophic occured (i.e. plugin crash)
	mtDeath,           // Denotes death of player
	mtPrivateMessage,  // Player to player messaging identifier
	mtJoin,            // A player has joined the server
	mtLeave,           // A player has left the server
	mtMaxPlusOne,      // The first invalid type, used for checking on LuaAPI boundaries

	// Common aliases:
	mtFail  = mtFailure,
	mtError = mtFailure,
	mtInfo  = mtInformation,
	mtPM    = mtPrivateMessage,
};





enum class BannerPattern
{
	BottomStripe,
	TopStripe,
	LeftStripe,
	RightStripe,
	CenterStripeVertical,
	MiddleStripeHorizontal,
	DownRightStripe,
	DownLeftStripe,
	SmallVerticalStripes,
	DiagonalCross,
	SquareCross,
	LeftOfDiagonal,
	RightOfUpsideDownDiagonal,
	LeftOfUpsideDownDiagonal,
	RightOfDiagonal,
	VerticalHalfLeft,
	VerticalHalfRight,
	HorizontalHalfTop,
	HorizontalHalfBottom,
	BottomLeftCorner,
	BottomRightCorner,
	TopLeftCorner,
	TopRightCorner,
	BottomTriangle,
	TopTriangle,
	BottomTriangleSawtooth,
	TopTriangleSawtooth,
	MiddleCircle,
	MiddleRhombus,
	Border,
	CurlyBorder,
	Brick,
	Gradient,
	GradientUpsideDown,
	Creeper,
	Skull,
	Flower,
	Mojang,
	Globe,
	Piglin
};





enum class BossBarColor
{
	Pink,
	Blue,
	Red,
	Green,
	Yellow,
	Purple,
	White
};





enum class BossBarDivisionType
{
	None,
	SixNotches,
	TenNotches,
	TwelveNotches,
	TwentyNotches
};



// tolua_end

enum class CommandBlockType
{
	Sequence,
	Auto,
	Redstone
};





enum class eGameStateReason
{
	NoRespawnBlock,
	RainStarted,
	RainStopped,
	GamemodeChanged,
	WinGame,
	DemoMessageShown,
	ProjectileHitPlayer,
	RainGradientChanged,
	ThunderGradientChanged,
	PufferfishString,
	ElderGuardianEffect,
	ImmediateRespawn,
	LimitedCraftingToggled,
	InitialChunksComing
};





enum class EntityAnimation
{
	AnimalFallsInLove,
	ArmorStandGetsHit,
	ArrowTipSparkles,
	DolphinShowsHappiness,
	EggCracks,
	EntityGetsCriticalHit,
	EntityGetsMagicalCriticalHit,
	EntityTrailsHoney,
	EvokerFangsAttacks,
	FireworkRocketExplodes,
	// FishingHookReels,
	FoxChews,
	GuardianAttacks,
	HoglinAttacks,
	HorseTamingFails,
	HorseTamingSucceeds,
	IronGolemAttacks,
	IronGolemOffersGift,
	IronGolemStashesGift,
	MinecartSpawnerDelayResets,
	MinecartTNTIgnites,
	MobSpawns,
	OcelotTrusts,
	OcelotDistrusts,
	PawnBerryBushPricks,
	PawnBurns,
	PawnChestEquipmentBreaks,
	PawnDies,
	PawnDrowns,
	PawnFeetEquipmentBreaks,
	PawnHandItemSwaps,
	PawnHeadEquipmentBreaks,
	PawnHurts,
	PawnLegsEquipmentBreaks,
	PawnMainHandEquipmentBreaks,
	PawnOffHandEquipmentBreaks,
	PawnShieldBlocks,
	PawnShieldBreaks,
	PawnTeleports,
	PawnThornsPricks,
	PawnTotemActivates,
	PlayerBadOmenActivates,
	PlayerEntersBed,
	PlayerFinishesEating,
	PlayerLeavesBed,
	PlayerMainHandSwings,
	// PlayerReducedDebugScreenDisables,
	// PlayerReducedDebugScreenEnables,
	PlayerOffHandSwings,
	RabbitJumps,
	RavagerAttacks,
	RavagerBecomesStunned,
	SheepEatsGrass,
	SnowballPoofs,
	// SquidResetsRotation,
	VillagerKisses,
	VillagerShowsAnger,
	VillagerShowsHappiness,
	VillagerSweats,
	WitchMagicks,
	WolfShakesWater,
	WolfTamingFails,
	WolfTamingSucceeds,
	ZoglinAttacks,
	ZombieVillagerCureFinishes
};





enum class eCommandParserType : std::int8_t
{
	None = -1,
	Bool = 0,
	Float,
	Double,
	Integer,
	Long,
	String,
	Entity,
	GameProfile,
	BlockPos,
	ColumnPos,
	Vec3,
	Vec2,
	BlockState,
	BlockPredicate,
	ItemStack,
	ItemPredicate,
	Color,
	Component,
	Style,
	Message,
	NbtCompoundTag,
	NbtTag,
	NbtPath,
	Objective,
	ObjectiveCriteria,
	Operation,
	Particle,
	Angle,
	Rotation,
	ScoreboardSlot,
	ScoreHolder,
	Swizzle,
	Team,
	ItemSlot,
	ItemSlots,
	ResourceLocation,
	Function,
	EntityAnchor,
	IntRange,
	FloatRange,
	Dimension,
	Gamemode,
	Time,
	ResourceOrTag,
	ResourceOrTagKey,
	Resource,
	ResourceKey,
	TemplateMirror,
	TemplateRotation,
	Heightmap,
	LootTable,
	LootPredicate,
	LootModifier,
	Uuid
	// TODO: actually implement these parsers
};





// tolua_begin

/** Returns a textual representation of the click action. */
const char * ClickActionToString(int a_ClickAction);

/** Returns a blockface mirrored around the Y axis (doesn't change up / down). */
eBlockFace MirrorBlockFaceY(eBlockFace a_BlockFace);

/** Returns a blockface mirrored around the XY plane. */
eBlockFace MirrorBlockFaceXY(eBlockFace a_BlockFace);

/** Returns a blockface mirrored around the YZ plane. */
eBlockFace MirrorBlockFaceYZ(eBlockFace a_BlockFace);

/** Returns a blockface rotated around the Y axis counter-clockwise. */
eBlockFace RotateBlockFaceCCW(eBlockFace a_BlockFace);

/** Returns a blockface rotated around the Y axis clockwise. */
eBlockFace RotateBlockFaceCW(eBlockFace a_BlockFace);

/** Returns a blockface opposite to the one specified. */
eBlockFace ReverseBlockFace(eBlockFace  a_BlockFace);

/** Returns the textual representation of the BlockFace constant. */
AString BlockFaceToString(eBlockFace a_BlockFace);

bool IsValidItem(Item a_ItemType);

eBlockFace RotationToBlockFace(double a_Rotation, bool a_Inverse = false);

unsigned char RotationToFineFace(double a_Rotation, bool a_Invert = false);

eBlockFace DisplacementYawToFacing(Vector3d a_PlacePosition, Vector3d a_EyePosition, double a_Yaw);

/** Translates a dimension string to dimension enum. Takes either a number or a dimension alias (built-in). Returns dimOverworld on failure */
extern eDimension StringToDimension(const AString & a_DimensionString);

/** Translates a dimension enum to dimension string.
Takes an eDimension enum value and returns "Overworld" on failure. */
extern AString DimensionToString(eDimension a_Dimension);

/** Translates damage type constant to a string representation (built-in). */
extern AString DamageTypeToString(eDamageType a_DamageType);

/** Translates a damage type string to damage type. Takes either a number or a damage type alias (built-in). Returns -1 on failure */
extern eDamageType StringToDamageType(const AString & a_DamageString);

/** Modifies the specified coords so that they point to the block adjacent to the one specified through its specified face.
If a_Inverse is true, the opposite direction is used instead. */
void AddFaceDirection(int & a_BlockX, int & a_BlockY, int & a_BlockZ, eBlockFace a_BlockFace, bool a_bInverse = false);

/** Returns the coords of a block that is neighboring the specified position through its specified face.
If a_InvertDirection is true, the opposite direction is used instead. */
Vector3i AddFaceDirection(Vector3i a_Pos, eBlockFace a_BlockFace, bool a_InvertDirection = false);

// tolua_end





inline void EulerToVector(double a_Pan, double a_Pitch, double & a_X, double & a_Y, double & a_Z)
{
	// 	a_X = sinf ( a_Pan / 180 * PI) * cosf ( a_Pitch / 180 * PI);
	// 	a_Y = -sinf ( a_Pitch / 180 * PI);
	// 	a_Z = -cosf ( a_Pan / 180 * PI) * cosf ( a_Pitch / 180 * PI);
	a_X = cos(a_Pan / 180 * M_PI) * cos(a_Pitch / 180 * M_PI);
	a_Y = sin(a_Pan / 180 * M_PI) * cos(a_Pitch / 180 * M_PI);
	a_Z = sin(a_Pitch / 180 * M_PI);
}





inline void VectorToEuler(double a_X, double a_Y, double a_Z, double & a_Pan, double & a_Pitch)
{
	double r = sqrt((a_X * a_X) + (a_Z * a_Z));
	if (r < std::numeric_limits<double>::epsilon())
	{
		a_Pan = 0;
	}
	else
	{
		a_Pan = atan2(a_Z, a_X) * 180 / M_PI - 90;
	}

	a_Pitch = atan2(a_Y, r) * 180 / M_PI;
}





template <class T>
inline T Diff(T a_Val1, T a_Val2)
{
	return std::abs(a_Val1 - a_Val2);
}





// tolua_begin

/** Normalizes an angle in degrees to the [-180, +180) range: */
inline double NormalizeAngleDegrees(const double a_Degrees)
{
	double Norm = fmod(a_Degrees + 180, 360);
	if (Norm < 0)
	{
		Norm += 360;
	}
	return Norm - 180;
}





namespace ItemCategory
{
	bool IsPickaxe(Item a_ItemType);

	bool IsAxe(Item a_ItemType);

	bool IsSword(Item a_ItemType);

	bool IsHoe(Item a_ItemType);

	bool IsShovel(Item a_ItemType);

	bool IsTool(Item a_ItemType);

	bool IsHelmet(Item a_ItemType);

	bool IsChestPlate(Item a_ItemType);

	bool IsLeggings(Item a_ItemType);

	bool IsBoots(Item a_ItemType);

	bool IsMinecart(Item a_ItemType);

	bool IsArmor(Item a_ItemType);

	bool IsHorseArmor(Item a_ItemType);

	bool IsVillagerFood(Item a_ItemType);
}

// tolua_end
