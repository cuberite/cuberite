
#pragma once





/** List of slot numbers, used for inventory-painting */
typedef std::vector<int> cSlotNums;





#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-variable-declarations"
#endif
/** Constant to calculate ticks from seconds "ticks per second" */
constexpr inline const int TPS = 20;
// This is not added to the lua API because it broke the build
#ifdef __clang__
#pragma clang diagnostic pop
#endif





// tolua_begin

/** Experience Orb setup */
enum
{
	// Open to suggestion on naming convention here :)
	MAX_EXPERIENCE_ORB_SIZE = 2000
} ;





/** Block face constants, used in PlayerDigging and PlayerBlockPlacement packets and bbox collision calc */
enum eBlockFace
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





enum eMobHeadType
{
	SKULL_TYPE_SKELETON    = 0,
	SKULL_TYPE_WITHER      = 1,
	SKULL_TYPE_ZOMBIE      = 2,
	SKULL_TYPE_PLAYER      = 3,
	SKULL_TYPE_CREEPER     = 4,
	SKULL_TYPE_DRAGON      = 5,
} ;





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





enum eHand
{
	hMain = 0,
	hOff = 1,
} ;





enum eMainHand
{
	mhLeft = 0,
	mhRight = 1,
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




/** Returns a textual representation of the click action. */
const char * ClickActionToString(int a_ClickAction);

/** Returns a blockface mirrored around the Y axis (doesn't change up / down). */
eBlockFace MirrorBlockFaceY(eBlockFace a_BlockFace);

/** Returns a blockface rotated around the Y axis counter-clockwise. */
eBlockFace RotateBlockFaceCCW(eBlockFace a_BlockFace);

/** Returns a blockface rotated around the Y axis clockwise. */
eBlockFace RotateBlockFaceCW(eBlockFace a_BlockFace);

/** Returns a blockface opposite to the one specified. */
eBlockFace ReverseBlockFace(eBlockFace  a_BlockFace);

/** Returns the textual representation of the BlockFace constant. */
AString BlockFaceToString(eBlockFace a_BlockFace);

/** Returns true if the specified block type is valid (known). */
bool IsValidBlock(int a_BlockType);

/** Returns true if the specified item type is valid (known). */
bool IsValidItem(int a_ItemType);

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
If a_IsInverse is true, the opposite direction is used instead. */
inline Vector3i AddFaceDirection(Vector3i a_Pos, eBlockFace a_BlockFace, bool a_bInverse = false)
{
	AddFaceDirection(a_Pos.x, a_Pos.y, a_Pos.z, a_BlockFace, a_bInverse);
	return a_Pos;
}

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





template <class T> inline T Diff(T a_Val1, T a_Val2)
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
	bool IsPickaxe(short a_ItemType);

	bool IsAxe(short a_ItemType);

	bool IsSword(short a_ItemType);

	bool IsHoe(short a_ItemType);

	bool IsShovel(short a_ItemType);

	bool IsTool(short a_ItemType);

	bool IsHelmet(short a_ItemType);

	bool IsChestPlate(short a_ItemType);

	bool IsLeggings(short a_ItemType);

	bool IsBoots(short a_ItemType);

	bool IsMinecart(short a_ItemType);

	bool IsArmor(short a_ItemType);

	bool IsHorseArmor(short a_ItemType);
}

// tolua_end
