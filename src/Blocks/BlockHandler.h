
#pragma once

#include "../Defines.h"
#include "../Item.h"
#include "../BoundingBox.h"





// fwd:
class cPlayer;
class cChunk;
class cBlockPluginInterface;
class cChunkInterface;
class cWorld;
class cWorldInterface;
class cItems;





class cBlockHandler
{
public:

	constexpr cBlockHandler(BlockType a_Block) :
		m_BlockType(a_Block)
	{
	}

	/** Called when the block gets ticked either by a random tick or by a queued tick.
	Note that the coords in a_RelPos are chunk-relative! */
	virtual void OnUpdate(
		cChunkInterface & a_ChunkInterface,
		cWorldInterface & a_WorldInterface,
		cBlockPluginInterface & a_BlockPluginInterface,
		cChunk & a_Chunk,
		const Vector3i a_RelPos
	) const;

	/** Returns the relative bounding box that must be entity-free in
	order for the block to be placed. a_XM, a_XP, etc. stand for the
	blocktype of the minus-X neighbor, the positive-X neighbor, etc. */
	virtual cBoundingBox GetPlacementCollisionBox(BlockState a_XM, BlockState a_XP, BlockState a_YM, BlockState a_YP, BlockState a_ZM, BlockState a_ZP) const;

	/** Called by cWorld::SetBlock() after the block has been set */
	virtual void OnPlaced(
		cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface,
		Vector3i a_BlockPos,
		BlockState a_Block
	) const {}

	/** Called after a block gets broken (replaced with air), by natural means.
	The block is already dug up in the world, the original block type and meta is passed in a_OldBlockType and a_OldBlockMeta.
	By default notifies all direct neighbors via their OnNeighborChanged() callbacks.
	You can determine what kind of entity broke the block (e.g. player) by checking a_Digger! */
	virtual void OnBroken(
		cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface,
		Vector3i a_BlockPos,
		BlockState a_OldBlock,
		const cEntity * a_Digger
	) const {}

	/** Called when a direct neighbor of this block has been changed.
	The position is the block's own position, NOT the changed neighbor's position.
	a_WhichNeighbor indicates which neighbor has changed. For example, BLOCK_FACE_YP meant the neighbor above has changed.
	BLOCK_FACE_NONE means that it is a neighbor not directly adjacent (diagonal, etc.) */
	virtual void OnNeighborChanged(cChunkInterface & a_ChunkInterface, Vector3i a_BlockPos, eBlockFace a_WhichNeighbor) const;

	/** Notifies the specified neighbor that the current block has changed.
	a_NeighborPos are the coords of the neighbor to be notified
	a_WhichNeighbor specifies which neighbor (relative to a_NeighborPos) has changed.
	For example BLOCK_FACE_YP means that the block at {a_NeighborX, a_NeighborY + 1, a_NeighborZ} has changed.
	BLOCK_FACE_NONE means that it is a neighbor not directly adjacent (diagonal, etc.) */
	static void NeighborChanged(cChunkInterface & a_ChunkInterface, Vector3i a_NeighborPos, eBlockFace a_WhichNeighbor);

	/** Called when the player starts digging the block. */
	virtual void OnDigging(
		cChunkInterface & a_ChunkInterface,
		cWorldInterface & a_WorldInterface,
		cPlayer & a_Player,
		const Vector3i a_BlockPos
	) const
	{
	}

	/** Called when the user right clicks the block and the block is useable.
	a_CursorPos is the cursor position within the clicked block face.
	Returns true if the use was successful, return false to use the block as a "normal" block */
	virtual bool OnUse(
		cChunkInterface & a_ChunkInterface,
		cWorldInterface & a_WorldInterface,
		cPlayer & a_Player,
		const Vector3i a_BlockPos,
		eBlockFace a_BlockFace,
		const Vector3i a_CursorPos
	) const
	{
		return false;
	}

	/** Called when a right click to this block is cancelled.
	Descendants should force the server to send the real state of a block to the client to prevent client assuming the operation was successfull. */
	virtual void OnCancelRightClick(
		cChunkInterface & a_ChunkInterface,
		cWorldInterface & a_WorldInterface,
		cPlayer & a_Player,
		const Vector3i a_BlockPos,
		eBlockFace a_BlockFace
	) const
	{
	}

	/** Returns the pickups that would result if the block was mined by a_Digger using a_Tool.
	Doesn't do any actual block change / mining, only calculates the pickups.
	a_Digger is the entity that caused the conversion, usually the player digging.
	a_Tool is the tool used for the digging.
	The default implementation drops a single item created from m_BlockType and the current meta. */
	virtual cItems ConvertToPickups(BlockState a_Block, const cItem * a_Tool = nullptr) const;

	/** Checks if the block can stay at the specified relative coords in the chunk */
	virtual bool CanBeAt(const cChunk & a_Chunk, Vector3i a_Position, BlockState a_Self) const;

	/** Checks whether the block has an effect on growing the plant */
	virtual bool CanSustainPlant(BlockState a_Block) const { return false; }

	/** Called to check whether this block supports a rclk action.
	If it returns true, OnUse() is called */
	virtual bool IsUseable(void) const;

	/** Checks if the player can build "inside" this block.
	For example blocks placed "on" snow will be placed at the same position. So: Snow ignores Build collision
	@param a_Pos Position of the block
	@param a_Player Player trying to build on the block
	@param a_Meta Meta value of the block currently at a_Pos */
	virtual bool DoesIgnoreBuildCollision(const cWorld & a_World, const cItem & a_HeldItem, Vector3i a_Position, BlockState a_ClickedBlock, eBlockFace a_ClickedBlockFace, bool a_ClickedDirectly) const;

	/** Tests if a_RelPosition is inside the block, where a_RelPosition is relative to the origin of the block.
	Coords in a_RelPosition are guaranteed to be in the [0..1] range. */
	virtual bool IsInsideBlock(const Vector3d a_RelPosition, const BlockState a_Block) const;

	/** Called when one of the neighbors gets set; equivalent to MC block update.
	By default drops (DropBlockAsPickup() / SetBlock()) if the position is no longer suitable (CanBeAt(), DoesDropOnUnsuitable()),
	otherwise wakes up all simulators on the block. */
	void Check(
		cChunkInterface & ChunkInterface, cBlockPluginInterface & a_PluginInterface,
		Vector3i a_RelPos,
		cChunk & a_Chunk
	) const;

	/** Returns the base colour ID of the block, as will be represented on a map, as per documentation: https://minecraft.gamepedia.com/Map_item_format */
	virtual ColourID GetMapBaseColourID() const;

	/** Rotates a given block meta counter-clockwise. Default: no change
	Returns block meta following rotation */
	virtual BlockState RotateCCW(BlockState a_Block) const { return a_Block; }

	/** Rotates a given block meta clockwise. Default: no change
	Returns block meta following rotation */
	virtual BlockState RotateCW(BlockState a_Block) const { return a_Block; }

	/** Mirrors a given block meta around the XY plane. Default: no change
	Returns block meta following rotation */
	virtual BlockState MirrorXY(BlockState a_Block) const { return a_Block; }

	/** Mirros a given block meta around the XZ plane. Default: no change
	Returns block meta following rotation */
	virtual BlockState MirrorXZ(BlockState a_Block) const { return a_Block; }

	/** Mirros a given block meta around the YZ plane. Default: no change
	Returns block meta following rotation */
	virtual BlockState MirrorYZ(BlockState a_Block) const { return a_Block; }

	/** Grows this block, if it supports growing, by the specified amount of stages (at most).
	Returns the number of stages actually grown, zero if not supported (default). */
	virtual int Grow(cChunk & a_Chunk, Vector3i a_RelPos, char a_NumStages = 1) const { return 0; }

	virtual bool FullyOccupiesVoxel(const BlockState a_Block) const { return true; }

	/** Returns true if the specified tool is valid and has a non-zero silk-touch enchantment.
	Helper used in many ConvertToPickups() implementations. */
	static bool ToolHasSilkTouch(const cItem * a_Tool);

	/** Returns the fortune level of a tool, if it is a valid tool.
	Can be used in ConvertToPickups() implementations. */
	static unsigned char ToolFortuneLevel(const cItem * a_Tool);

	/** Returns a random number of drops taking into account fortune.
	Only applies to drops following clamped discrete random distribution.
	a_DefaultMax is the maximum items from one block without fortune.
	a_BonusMax is the amount to increase the max of randInt by, usually the fortune level (but not always)
	a_DropCap is the maximum items from one block with fortune,
	if unspecified set to 25 to prevent lag or crash with high level tools.
	Similar to uniform_bonus_count at https://minecraft.wiki/w/Loot_table#Functions */
	static char FortuneDiscreteRandom(char a_MinDrop, char a_DefaultMax, unsigned char a_BonusMax, char a_DropCap = 25);

	// Gets the blockhandler for the given block type.
	static const cBlockHandler & For(BlockType a_Block);

protected:

	~cBlockHandler() = default;

	const BlockType m_BlockType;
};
