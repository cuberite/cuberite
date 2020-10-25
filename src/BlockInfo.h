
#pragma once

#include "ChunkDef.h"





// tolua_begin
class cBlockInfo
{
public:

	// tolua_end

	/** Returns how much of an explosion Destruction Lazor's (tm) intensity the given block attenuates.
	See Physics\Explodinator.cpp for details of explosion block destruction. */
	static float GetExplosionAbsorption(BLOCKTYPE Block);

	// tolua_begin

	/** How much light do the blocks emit on their own? */
	static NIBBLETYPE GetLightValue(BLOCKTYPE Block);

	/** How much light do the blocks consume? */
	static NIBBLETYPE GetSpreadLightFalloff(BLOCKTYPE Block);

	/** Can a finisher change it? */
	static bool CanBeTerraformed(BLOCKTYPE Block);

	/** Does this block fully occupy its voxel - is it a 'full' block? */
	static bool FullyOccupiesVoxel(BLOCKTYPE Block);

	/** Is a block destroyed after a single hit?
	Warning: IsOneHitDig does not take into account enchantments / status effects / swim state / floating state
	and therefore may be incorrect. Only use to check if hardness is 0.
	If you want to check if a player would instantly mine a_Block use cPlayer::CanInstantlyMine(a_Block) */
	static bool IsOneHitDig(BLOCKTYPE Block);

	/** Can a piston break this block? */
	static bool IsPistonBreakable(BLOCKTYPE Block);

	/** Does this block block the passage of rain? */
	static bool IsRainBlocker(BLOCKTYPE Block);

	/** Does this block disperse sky light? (only relevant for transparent blocks) */
	static bool IsSkylightDispersant(BLOCKTYPE Block);

	static bool IsSnowable(BLOCKTYPE Block);

	/** Is this block solid (player cannot walk through)? */
	static bool IsSolid(BLOCKTYPE Block);

	/** Is a block transparent? (https://minecraft.gamepedia.com/Opacity) */
	static bool IsTransparent(BLOCKTYPE Block);

	/** Can a spectator interact with this block? */
	static bool IsUseableBySpectator(BLOCKTYPE Block);

	/** Block's height. */
	static float GetBlockHeight(BLOCKTYPE Block);

	/** Block's hardness. The greater the value the longer the player needs to break the block. */
	static float GetHardness(BLOCKTYPE Block);
};
// tolua_end





bool IsBlockWater(BLOCKTYPE a_BlockType);

bool IsBlockIce(BLOCKTYPE a_BlockType);

bool IsBlockWaterOrIce(BLOCKTYPE a_BlockType);

bool IsBlockLava(BLOCKTYPE a_BlockType);

bool IsBlockLiquid(BLOCKTYPE a_BlockType);

bool IsBlockRail(BLOCKTYPE a_BlockType);

bool IsBlockTypeOfDirt(BLOCKTYPE a_BlockType);

bool IsBlockFence(BLOCKTYPE a_BlockType);

bool IsBlockShulkerBox(BLOCKTYPE a_BlockType);

bool IsBlockMaterialWood(BLOCKTYPE a_BlockType);

bool IsBlockMaterialPlants(BLOCKTYPE a_BlockType);

bool IsBlockMaterialVine(BLOCKTYPE a_BlockType);

bool IsBlockMaterialIron(BLOCKTYPE a_BlockType);

bool IsBlockMaterialLeaves(BLOCKTYPE a_BlockType);

bool IsBlockMaterialGourd(BLOCKTYPE a_BlockType);

bool IsBlockMaterialRock(BLOCKTYPE a_BlockType);
