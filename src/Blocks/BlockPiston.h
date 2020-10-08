
#pragma once

#include "BlockHandler.h"
#include "Mixins.h"
#include "../Item.h"




// fwd:
class cWorld;





class cBlockPistonHandler final :
	public cClearMetaOnDrop<cPitchYawRotator<cBlockHandler, 0x07, 0x03, 0x04, 0x02, 0x05, 0x01, 0x00>>
{
	using Super = cClearMetaOnDrop<cPitchYawRotator<cBlockHandler, 0x07, 0x03, 0x04, 0x02, 0x05, 0x01, 0x00>>;

public:

	using Super::Super;

	static eBlockFace MetaDataToDirection(NIBBLETYPE a_MetaData)
	{
		switch (a_MetaData & 0x7)  // We only want the bottom three bits (4th controls extended-ness))
		{
			case 0x0: return BLOCK_FACE_YM;
			case 0x1: return BLOCK_FACE_YP;
			case 0x2: return BLOCK_FACE_ZM;
			case 0x3: return BLOCK_FACE_ZP;
			case 0x4: return BLOCK_FACE_XM;
			case 0x5: return BLOCK_FACE_XP;
			default:
			{
				ASSERT(!"Invalid Metadata");
				return BLOCK_FACE_NONE;
			}
		}
	}

	/** Converts piston block's metadata into a unit vector representing the direction in which the piston will extend. */
	static Vector3i MetadataToOffset(NIBBLETYPE a_PistonMeta);

	static void ExtendPiston(Vector3i a_BlockPos, cWorld & a_World);
	static void RetractPiston(Vector3i a_BlockPos, cWorld & a_World);

	/** Returns true if the piston (with the specified meta) is extended */
	static inline bool IsExtended(NIBBLETYPE a_PistonMeta) { return ((a_PistonMeta & 0x8) != 0x0); }

private:

	typedef std::unordered_set<Vector3i, VectorHasher<int>> Vector3iSet;

	/** Piston extension block action */
	static const Byte PistonExtendAction = 0U;

	/** Piston retraction block action */
	static const Byte PistonRetractAction = 1U;

	/** Returns true if the piston (specified by blocktype) is a sticky piston */
	static inline bool IsSticky(BLOCKTYPE a_BlockType) { return (a_BlockType == E_BLOCK_STICKY_PISTON); }

	/** Returns true if the specified block can be pushed by a piston (and left intact) */
	static inline bool CanPush(BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta)
	{
		switch (a_BlockType)
		{
			case E_BLOCK_ANVIL:
			case E_BLOCK_BARRIER:
			case E_BLOCK_BEACON:
			case E_BLOCK_BEDROCK:
			case E_BLOCK_BREWING_STAND:
			case E_BLOCK_CHAIN_COMMAND_BLOCK:
			case E_BLOCK_CHEST:
			case E_BLOCK_COMMAND_BLOCK:
			case E_BLOCK_DAYLIGHT_SENSOR:
			case E_BLOCK_DISPENSER:
			case E_BLOCK_DROPPER:
			case E_BLOCK_ENCHANTMENT_TABLE:
			case E_BLOCK_END_GATEWAY:
			case E_BLOCK_END_PORTAL:
			case E_BLOCK_END_PORTAL_FRAME:
			case E_BLOCK_ENDER_CHEST:
			case E_BLOCK_FURNACE:
			case E_BLOCK_LIT_FURNACE:
			case E_BLOCK_INVERTED_DAYLIGHT_SENSOR:
			case E_BLOCK_HOPPER:
			case E_BLOCK_JUKEBOX:
			case E_BLOCK_MOB_SPAWNER:
			case E_BLOCK_NETHER_PORTAL:
			case E_BLOCK_NOTE_BLOCK:
			case E_BLOCK_OBSIDIAN:
			case E_BLOCK_PISTON_EXTENSION:
			case E_BLOCK_REPEATING_COMMAND_BLOCK:
			case E_BLOCK_STANDING_BANNER:
			case E_BLOCK_STRUCTURE_BLOCK:
			case E_BLOCK_TRAPPED_CHEST:
			case E_BLOCK_WALL_BANNER:
			{
				return false;
			}
			case E_BLOCK_STICKY_PISTON:
			case E_BLOCK_PISTON:
			{
				// A piston can only be pushed if retracted:
				return !IsExtended(a_BlockMeta);
			}
		}
		return true;
	}

	/** Tries to push a block and increases the pushed blocks variable. Returns true if the block is pushable */
	static bool CanPushBlock(
		const Vector3i & a_BlockPos, cWorld & a_World, bool a_RequirePushable,
		Vector3iSet & a_BlocksPushed, const Vector3i & a_PushDir
	);

	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) const override
	{
		UNUSED(a_Meta);
		return 11;
	}

	virtual void OnBroken(
		cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface,
		Vector3i a_BlockPos,
		BLOCKTYPE a_OldBlockType, NIBBLETYPE a_OldBlockMeta,
		const cEntity * a_Digger
	) const override;

	/** Moves a list of blocks in a specific direction */
	static void PushBlocks(const Vector3iSet & a_BlocksToPush,
		cWorld & a_World, const Vector3i & a_PushDir
	);
} ;





class cBlockPistonHeadHandler final :
	public cBlockHandler
{
	using Super = cBlockHandler;

public:

	constexpr cBlockPistonHeadHandler(void) :
		Super(E_BLOCK_PISTON_EXTENSION)
	{
	}

	virtual void OnBroken(
		cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface,
		Vector3i a_BlockPos,
		BLOCKTYPE a_OldBlockType, NIBBLETYPE a_OldBlockMeta,
		const cEntity * a_Digger
	) const override;

	virtual cItems ConvertToPickups(NIBBLETYPE a_BlockMeta, const cEntity * a_Digger, const cItem * a_Tool) const override;
} ;
