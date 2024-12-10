
#pragma once

#include "ChunkDef.h"





// tolua_begin
class cBlockInfo
{
public:

	/** How much light do the blocks emit on their own? */
	static LIGHTTYPE GetLightValue(BlockState a_Block);

	/** How much light do the blocks consume? */
	static LIGHTTYPE GetSpreadLightFalloff(BlockState a_Block);

	/** Can a finisher change it? */
	static bool CanBeTerraformed(BlockState a_Block);

	/** Does this block fully occupy its voxel - is it a 'full' block? */
	static bool FullyOccupiesVoxel(BlockState a_Block);

	/** Does the client pretend the block doesn't exist when clicking?
	For example, digging a fire will hit the block below the fire, so fire is "clicked through". */
	static bool IsClickedThrough(BlockState a_Block);

	/** Is a block destroyed after a single hit?
	Warning: IsOneHitDig does not take into account enchantments / status effects / swim state / floating state
	and therefore may be incorrect. Only use to check if hardness is 0.
	If you want to check if a player would instantly mine a_Block use cPlayer::CanInstantlyMine(a_Block) */
	static bool IsOneHitDig(BlockState a_Block);

	/** Can a piston break this block? */
	static bool IsPistonBreakable(BlockState a_Block);

	/** Does this block block the passage of rain? */
	static bool IsRainBlocker(BlockState a_Block);

	/** Does this block disperse sky light? (only relevant for transparent blocks) */
	static bool IsSkylightDispersant(BlockState a_Block);

	static bool IsSnowable(BlockState a_Block);

	/** Is this block solid (player cannot walk through)? */
	static bool IsSolid(BlockState a_Block);

	/** Is a block transparent? (https://minecraft.gamepedia.com/Opacity) */
	static bool IsTransparent(BlockState a_Block);

	/** Can a spectator interact with this block? */
	static bool IsUseableBySpectator(BlockState a_Block);

	/** Block's height. */
	static float GetBlockHeight(BlockState a_Block);

	/** Block's hardness. The greater the value the longer the player needs to break the block. */
	static float GetHardness(BlockState a_Block);
};
// tolua_end

bool IsBlockAir(BlockState a_Block);

bool IsBlockAnvil(BlockState a_Block);

bool IsBlockIce(BlockState a_Block);

bool IsBlockLiquid(BlockState a_Block);

bool IsBlockMaterialDirt(BlockState a_Block);

bool IsBlockMaterialGourd(BlockState a_Block);

bool IsBlockMaterialIron(BlockState a_Block);

bool IsBlockMaterialPlants(BlockState a_Block);

bool IsBlockMaterialRock(BlockState a_Block);

bool IsBlockMaterialVine(BlockState a_Block);

bool IsBlockMaterialWood(BlockState a_Block);

bool IsBlockMobHead(BlockState a_Block);

bool IsBlockShulkerBox(BlockState a_Block);

bool IsBlockWaterOrIce(BlockState a_Block);
