
#include "Globals.h"
#include "BlockPiston.h"
#include "BlockAir.h"
#include "../BlockInfo.h"
#include "../Item.h"
#include "../World.h"
#include "../Entities/Player.h"
#include "../BlockInServerPluginInterface.h"
#include "ChunkInterface.h"





#define PISTON_MAX_PUSH_DISTANCE 12





inline Vector3i cBlockPistonHandler::GetExtensionDirection(BlockState a_Block)
{
	eBlockFace Facing = BLOCK_FACE_NONE;
	switch (a_Block.Type())
	{
		case BlockType::Piston:       Facing = Block::Piston::Facing(a_Block); break;
		case BlockType::StickyPiston: Facing = Block::StickyPiston::Facing(a_Block); break;
		default: return Vector3i();
	}

	switch (Facing)
	{
		case BLOCK_FACE_NONE: return Vector3i();
		case BLOCK_FACE_XM:   return Vector3i(-1,  0,  0);
		case BLOCK_FACE_XP:   return Vector3i( 1,  0,  0);
		case BLOCK_FACE_YM:   return Vector3i( 0, -1,  0);
		case BLOCK_FACE_YP:   return Vector3i( 0,  1,  0);
		case BLOCK_FACE_ZM:   return Vector3i( 0,  0, -1);
		case BLOCK_FACE_ZP:   return Vector3i( 0,  0,  1);
	}
	return Vector3i();
}





inline Byte cBlockPistonHandler::GetExtensionByte(BlockState a_Block)
{
	eBlockFace Facing;
	switch (a_Block.Type())
	{
		case BlockType::Piston:       Facing = Block::Piston::Facing(a_Block); break;
		case BlockType::StickyPiston: Facing = Block::StickyPiston::Facing(a_Block); break;
		default: return std::numeric_limits<Byte>::max();
	}

	switch (Facing)
	{
		case BLOCK_FACE_NONE: return std::numeric_limits<Byte>::max();
		case BLOCK_FACE_XM:   return 3;  // West
		case BLOCK_FACE_XP:   return 5;  // East
		case BLOCK_FACE_YM:   return 0;  // Down
		case BLOCK_FACE_YP:   return 1;  // Up
		case BLOCK_FACE_ZM:   return 4;  // North
		case BLOCK_FACE_ZP:   return 2;  // South
	}
	return std::numeric_limits<Byte>::max();
}





void cBlockPistonHandler::ExtendPiston(Vector3i a_BlockPos, cWorld & a_World)
{
	// Broadcast block action first. Will do nothing if piston cannot in fact push

	{
		auto Self = a_World.GetBlock(a_BlockPos);
		auto DirectionByte = GetExtensionByte(Self);
		if (DirectionByte == std::numeric_limits<Byte>::max())
		{
			return;
		}
		a_World.BroadcastBlockAction(a_BlockPos, PistonExtendAction, DirectionByte, Self.Type());
	}
	// Client expects the server to "play" the animation before setting the final blocks
	// However, we don't confuse animation with the underlying state of the world, so emulate by delaying 1 tick
	// (Probably why vanilla has so many dupe glitches with sand and pistons lolol)

	a_World.ScheduleTask(1_tick, [a_BlockPos](cWorld & World)
		{

			auto Self = World.GetBlock(a_BlockPos);

			if (!IsBlockPiston(Self) && !IsSticky(Self))
			{
				// Ensure we operate on a piston to avoid spurious behaviour
				// Note that the scheduled task may result in the block type of a_BlockPos changing
				return;
			}

			if (IsExtended(Self))
			{
				// Already extended, bail out
				return;
			}

			Vector3i PushDir = GetExtensionDirection(Self);
			Vector3iSet BlocksPushed;
			if (!CanPushBlock(a_BlockPos + PushDir, World, true, BlocksPushed, PushDir))
			{
				// Can't push anything, bail out
				return;
			}
			PushBlocks(BlocksPushed, World, PushDir);

			// Set the extension and the piston base correctly
			Vector3i ExtensionPos = a_BlockPos + PushDir;
			using namespace Block;
			switch (Self.Type())
			{
				case BlockType::Piston:
				{
					World.SetBlock(a_BlockPos,   Piston::Piston(true, Piston::Facing(Self)));
					World.SetBlock(ExtensionPos, PistonHead::PistonHead(Piston::Facing(Self), false, PistonHead::Type::Normal));
					break;
				}
				case BlockType::StickyPiston:
				{
					World.SetBlock(a_BlockPos,   StickyPiston::StickyPiston(true, StickyPiston::Facing(Self)));
					World.SetBlock(ExtensionPos, PistonHead::PistonHead(StickyPiston::Facing(Self), false, PistonHead::Type::Sticky));
					break;
				}
				default: return;
			}

			// Play sound effect only if extended successfully
			World.BroadcastSoundEffect("block.piston.extend", a_BlockPos, 0.5f, 0.7f);
		}
	);
}





void cBlockPistonHandler::RetractPiston(Vector3i a_BlockPos, cWorld & a_World)
{
	{
		auto Self = a_World.GetBlock(a_BlockPos);
		auto DirectionByte = GetExtensionByte(Self);
		if (DirectionByte == std::numeric_limits<Byte>::max())
		{
			return;
		}
		a_World.BroadcastBlockAction(a_BlockPos, PistonRetractAction, DirectionByte, Self.Type());
	}

	a_World.ScheduleTask(1_tick, [a_BlockPos](cWorld & World)
	{
		auto Self = World.GetBlock(a_BlockPos);

		if (!IsBlockPiston(Self))
		{
			// Ensure we operate on a piston to avoid spurious behaviour
			// Note that the scheduled task may result in the block type of a_BlockPos changing
			return;
		}

		if (!IsExtended(Self))
		{
			// Already retracted, bail out
			return;
		}

		auto PushDir = GetExtensionDirection(Self);

		// Check the extension:
		Vector3i ExtensionPos = a_BlockPos + PushDir;
		if (World.GetBlock(ExtensionPos).Type() != BlockType::PistonHead)
		{
			LOGD("%s: Piston without an extension - still extending, or just in an invalid state?", __FUNCTION__);
			return;
		}

		using namespace Block;
		// Remove extension, update base state:
		World.SetBlock(ExtensionPos, Air::Air());
		switch (Self.Type())
		{
			case BlockType::Piston:
			{
				World.SetBlock(a_BlockPos,   Piston::Piston(false, Piston::Facing(Self)));
				break;
			}
			case BlockType::StickyPiston:
			{
				World.SetBlock(a_BlockPos,   StickyPiston::StickyPiston(true, StickyPiston::Facing(Self)));
				break;
			}
			default: return;
		}

		// (Retraction is always successful, but play in the task for consistency)
		World.BroadcastSoundEffect("block.piston.contract", a_BlockPos, 0.5f, 0.7f);

		if (!IsSticky(Self))
		{
			// No need for block pulling, bail out
			return;
		}

		// Get the block to pull
		Vector3i AdjustedPosition = a_BlockPos + PushDir * 2;
		// Try to "push" the pulling block in the opposite direction
		PushDir *= -1;

		Vector3iSet PushedBlocks;
		if (!CanPushBlock(AdjustedPosition, World, false, PushedBlocks, PushDir))
		{
			// Not pushable, bail out
			return;
		}

		PushBlocks(PushedBlocks, World, PushDir);
	});
}





void cBlockPistonHandler::PushBlocks(
	const Vector3iSet & a_BlocksToPush,
	cWorld & a_World, const Vector3i & a_PushDir
)
{
	// Sort blocks to move the blocks first, which are farthest away from the piston
	// This prevents the overwriting of existing blocks
	std::vector<Vector3i> SortedBlocks(a_BlocksToPush.begin(), a_BlocksToPush.end());
	std::sort(SortedBlocks.begin(), SortedBlocks.end(), [a_PushDir](const Vector3i & a, const Vector3i & b)
	{
		return (a.Dot(a_PushDir) > b.Dot(a_PushDir));
	});

	// Move every block
	for (auto & MoveBlockPos : SortedBlocks)
	{
		auto BlockToMove = a_World.GetBlock(MoveBlockPos);

		if (cBlockInfo::IsPistonBreakable(BlockToMove))
		{
			// Block is breakable, drop it:
			a_World.DropBlockAsPickups(MoveBlockPos, nullptr, nullptr);
		}
		else
		{
			// Not breakable, just move it
			a_World.SetBlock(MoveBlockPos, Block::Air::Air());
			MoveBlockPos += a_PushDir;
			a_World.SetBlock(MoveBlockPos, BlockToMove);
		}
	}
}





bool cBlockPistonHandler::CanPushBlock(
	const Vector3i & a_BlockPos, cWorld & a_World, bool a_RequirePushable,
	Vector3iSet & a_BlocksPushed, const Vector3i & a_PushDir
)
{
	if (!cChunkDef::IsValidHeight(a_BlockPos))
	{
		// Can't push a void block.
		return false;
	}

	const static std::array<Vector3i, 6> pushingDirs =
	{
			Vector3i(-1,  0,  0),
			Vector3i( 1,  0,  0),
			Vector3i( 0, -1,  0),
			Vector3i( 0,  1,  0),
			Vector3i( 0,  0, -1),
			Vector3i( 0,  0,  1)
	};

	auto BlockToCheck = a_World.GetBlock(a_BlockPos);

	if (cBlockAirHandler::IsBlockAir(BlockToCheck))
	{
		// Air can be pushed
		return true;
	}

	if (!a_RequirePushable && cBlockInfo::IsPistonBreakable(BlockToCheck))
	{
		// Block should not be broken, when it's not in the pushing direction
		return true;
	}

	if (!CanPush(BlockToCheck))
	{
		// When it's not required to push this block, don't fail
		return !a_RequirePushable;
	}

	if (a_BlocksPushed.size() >= PISTON_MAX_PUSH_DISTANCE)
	{
		// Do not allow to push too much blocks
		return false;
	}

	if (!a_BlocksPushed.insert(a_BlockPos).second || cBlockInfo::IsPistonBreakable(BlockToCheck))
	{
		return true;  // Element exist already
	}

	if (BlockToCheck.Type() == BlockType::SlimeBlock)
	{
		// Try to push the other directions
		for (const auto & testDir : pushingDirs)
		{
			if (!CanPushBlock(a_BlockPos + testDir, a_World, false, a_BlocksPushed, a_PushDir))
			{
				// When it's not possible for a direction, then fail
				return false;
			}
		}
	}

	// Try to push the block in front of this block
	return CanPushBlock(a_BlockPos + a_PushDir, a_World, true, a_BlocksPushed, a_PushDir);
}





void cBlockPistonHandler::OnBroken(
	cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface,
	Vector3i a_BlockPos,
	BlockState a_OldBlock,
	const cEntity * a_Digger
) const
{
	UNUSED(a_Digger);
	if (!IsExtended(a_OldBlock))
	{
		return;
	}

	const auto Extension = a_BlockPos + GetExtensionDirection(a_OldBlock);
	if (
		cChunkDef::IsValidHeight(Extension) &&
		(a_ChunkInterface.GetBlock(Extension).Type() == BlockType::PistonHead)
	)
	{
		// If the piston is extended, destroy the extension as well:
		a_ChunkInterface.SetBlock(Extension, Block::Air::Air());
	}
}





////////////////////////////////////////////////////////////////////////////////
// cBlockPistonHeadHandler:

void cBlockPistonHeadHandler::OnBroken(
	cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface,
	Vector3i a_BlockPos,
	BlockState a_OldBlock,
	const cEntity * a_Digger
) const
{
	UNUSED(a_Digger);
	const auto Base = a_BlockPos - cBlockPistonHandler::GetExtensionDirection(a_OldBlock);
	if (!cChunkDef::IsValidHeight(Base))
	{
		return;
	}

	const auto Block = a_ChunkInterface.GetBlock(Base);
	if ((Block.Type() == BlockType::Piston) || (Block == BlockType::StickyPiston))
	{
		// Remove the base of the piston:
		a_ChunkInterface.SetBlock(Base, Block::Air::Air());
	}
}
