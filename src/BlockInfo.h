
#pragma once

#include "ChunkDef.h"





// tolua_begin
class cBlockInfo
{
public:

	/** How much light do the blocks emit on their own? */
	static LIGHTTYPE GetLightValue(BlockType Block);

	/** How much light do the blocks consume? */
	static LIGHTTYPE GetSpreadLightFalloff(BlockType Block);

	/** Can a finisher change it? */
	static bool CanBeTerraformed(BlockType Block);

	/** Does this block fully occupy its voxel - is it a 'full' block? */
	static bool FullyOccupiesVoxel(BlockState Block);

	/** Is a block destroyed after a single hit?
	Warning: IsOneHitDig does not take into account enchantments / status effects / swim state / floating state
	and therefore may be incorrect. Only use to check if hardness is 0.
	If you want to check if a player would instantly mine a_Block use cPlayer::CanInstantlyMine(a_Block) */
	static bool IsOneHitDig(BlockType Block);

	/** Can a piston break this block? */
	static bool IsPistonBreakable(BlockType Block);

	/** Does this block block the passage of rain? */
	static bool IsRainBlocker(BlockType Block);

	/** Does this block disperse sky light? (only relevant for transparent blocks) */
	static bool IsSkylightDispersant(BlockType Block);

	static bool IsSnowable(BlockType Block);

	/** Is this block solid (player cannot walk through)? */
	static bool IsSolid(BlockType Block);

	/** Is a block transparent? (https://minecraft.gamepedia.com/Opacity) */
	static bool IsTransparent(BlockType Block);

	/** Can a spectator interact with this block? */
	static bool IsUseableBySpectator(BlockType Block);

	/** Block's height. */
	static float GetBlockHeight(BlockType Block);

	/** Block's hardness. The greater the value the longer the player needs to break the block. */
	static float GetHardness(BlockType Block);
};
// tolua_end





bool IsBlockWater(BlockType a_BlockType);

bool IsBlockIce(BlockType a_BlockType);

bool IsBlockWaterOrIce(BlockType a_BlockType);

bool IsBlockLava(BlockType a_BlockType);

bool IsBlockLiquid(BlockType a_BlockType);

bool IsBlockRail(BlockType a_BlockType);

bool IsBlockTypeOfDirt(BlockType a_BlockType);

bool IsBlockFence(BlockType a_BlockType);

bool IsBlockShulkerBox(BlockType a_BlockType);

bool IsBlockMaterialWood(BlockType a_BlockType);

bool IsBlockMaterialPlants(BlockType a_BlockType);

bool IsBlockMaterialVine(BlockType a_BlockType);

bool IsBlockMaterialIron(BlockType a_BlockType);

bool IsBlockMaterialLeaves(BlockType a_BlockType);

bool IsBlockMaterialGourd(BlockType a_BlockType);

bool IsBlockMaterialRock(BlockType a_BlockType);

bool IsBed(BlockType a_BlockType);

bool IsHead(BlockType a_BlockType);

bool IsSign(BlockType a_BlockType);

bool IsAnvil(BlockType a_BlockType);
