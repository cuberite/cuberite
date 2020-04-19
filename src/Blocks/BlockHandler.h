
#pragma once

#include "../Defines.h"
#include "../Item.h"
#include "../BoundingBox.h"





// fwd:
class cPlayer;
class cChunk;
class cBlockPluginInterface;
class cChunkInterface;
class cWorldInterface;
class cItems;
class BlockTypeRegistry;





class cBlockHandler
{
public:
	cBlockHandler(BLOCKTYPE a_BlockType);

	virtual ~cBlockHandler() {}

	/** Called when the block gets ticked either by a random tick or by a queued tick.
	Note that the coords in a_RelPos are chunk-relative! */
	virtual void OnUpdate(
		cChunkInterface & a_ChunkInterface,
		cWorldInterface & a_WorldInterface,
		cBlockPluginInterface & a_BlockPluginInterface,
		cChunk & a_Chunk,
		const Vector3i a_RelPos
	);

	/** Returns the relative bounding box that must be entity-free in
	order for the block to be placed. a_XM, a_XP, etc. stand for the
	blocktype of the minus-X neighbor, the positive-X neighbor, etc. */
	virtual cBoundingBox GetPlacementCollisionBox(BLOCKTYPE a_XM, BLOCKTYPE a_XP, BLOCKTYPE a_YM, BLOCKTYPE a_YP, BLOCKTYPE a_ZM, BLOCKTYPE a_ZP);

	/** Called before a block is placed into a world by player, by cItemHandler::GetPlacementBlockTypeMeta().
	The handler should return true to allow placement, false to refuse.
	a_PlacedBlockPos is the coords of the block being placed
	a_ClickedBlockFace is the face of the neighbor block clicked by the client to place this block.
	a_CursorPos is the position of the cursor within the neighbor's face
	The descendant handler should set a_BlockType and a_BlockMeta to correct values for the newly placed block.
	The default handler uses the stored block type and meta copied from the lowest 4 bits of the player's equipped item's damage value. */
	virtual bool GetPlacementBlockTypeMeta(
		cChunkInterface & a_ChunkInterface,
		cPlayer & a_Player,
		const Vector3i a_PlacedBlockPos,
		eBlockFace a_ClickedBlockFace,
		const Vector3i a_CursorPos,
		BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta
	);

	/** Called by cWorld::SetBlock() after the block has been set */
	virtual void OnPlaced(
		cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface,
		Vector3i a_BlockPos,
		BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta
	);

	/** Called by cPlayer::PlaceBlocks() for each block after it has been set to the world. Called after OnPlaced(). */
	virtual void OnPlacedByPlayer(
		cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, cPlayer & a_Player, const sSetBlock & a_BlockChange
	);

	/** Called just before the player breaks the block.
	The block is still valid in the world.
	By default does nothing special; descendants may provide further behavior. */
	virtual void OnPlayerBreakingBlock(
		cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface,
		cPlayer & a_Player,
		Vector3i a_BlockPos
	) {}

	/** Called just after the player breaks the block.
	The block is already dug up in the world, the original block type and meta is passed in a_OldBlockType and a_OldBlockMeta.
	By default does nothing special, descendants may provide further behavior. */
	virtual void OnPlayerBrokeBlock(
		cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface,
		cPlayer & a_Player,
		Vector3i a_BlockPos,
		BLOCKTYPE a_OldBlockType, NIBBLETYPE a_OldBlockMeta
	) {}

	/** Called before a block gets broken (replaced with air), either by player or by natural means.
	If by player, it is called after the OnPlayerBreakingBlock() callback.
	By default does nothing. */
	virtual void OnBreaking(
		cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface,
		Vector3i a_BlockPos
	) {}

	/** Called after a block gets broken (replaced with air), either by player or by natural means.
	If by player, it is called before the OnPlayerBrokeBlock() callback.
	The block is already dug up in the world, the original block type and meta is passed in a_OldBlockType and a_OldBlockMeta.
	By default notifies all direct neighbors via their OnNeighborChanged() callbacks. */
	virtual void OnBroken(
		cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface,
		Vector3i a_BlockPos,
		BLOCKTYPE a_OldBlockType, NIBBLETYPE a_OldBlockMeta
	);

	/** Called when a direct neighbor of this block has been changed.
	The position is the block's own position, NOT the changed neighbor's position.
	a_WhichNeighbor indicates which neighbor has changed. For example, BLOCK_FACE_YP meant the neighbor above has changed.
	BLOCK_FACE_NONE means that it is a neighbor not directly adjacent (diagonal, etc.) */
	virtual void OnNeighborChanged(cChunkInterface & a_ChunkInterface, Vector3i a_BlockPos, eBlockFace a_WhichNeighbor) {}

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
	)
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
	)
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
	)
	{
	}

	/** Returns the pickups that would result if the block was mined by a_Digger using a_Tool.
	Doesn't do any actual block change / mining, only calculates the pickups.
	a_BlockEntity is the block entity present at the block, if any, nullptr if none.
	a_Digger is the entity that caused the conversion, usually the player digging.
	a_Tool is the tool used for the digging.
	The default implementation drops a single item created from m_BlockType and the current meta. */
	virtual cItems ConvertToPickups(
		NIBBLETYPE a_BlockMeta,
		cBlockEntity * a_BlockEntity,
		const cEntity * a_Digger = nullptr,
		const cItem * a_Tool = nullptr
	);

	/** Checks if the block can stay at the specified relative coords in the chunk */
	virtual bool CanBeAt(
		cChunkInterface & a_ChunkInterface,
		const Vector3i a_RelPos,
		const cChunk & a_Chunk
	);

	/** Checks whether the block has an effect on growing the plant */
	virtual bool CanSustainPlant(BLOCKTYPE a_Plant) { return false; }

	/** Called to check whether this block supports a rclk action.
	If it returns true, OnUse() is called */
	virtual bool IsUseable(void);

	/** Indicates whether the client will click through this block.
	For example digging a fire will hit the block below the fire so fire is clicked through. */
	virtual bool IsClickedThrough(void);

	/** Checks if the player can build "inside" this block.
	For example blocks placed "on" snow will be placed at the same position. So: Snow ignores Build collision
	@param a_Pos Position of the block
	@param a_Player Player trying to build on the block
	@param a_Meta Meta value of the block currently at a_Pos */
	virtual bool DoesIgnoreBuildCollision(cChunkInterface & ChunkInterface, const Vector3i a_Pos, cPlayer & a_Player, NIBBLETYPE a_Meta);

	/** Returns if this block drops if it gets destroyed by an unsuitable situation.
	Default: true */
	virtual bool DoesDropOnUnsuitable(void);

	/** Tests if a_RelPosition is inside the block, where a_RelPosition is relative to the origin of the block.
	Coords in a_RelPosition are guaranteed to be in the [0..1] range. */
	virtual bool IsInsideBlock(const Vector3d a_RelPosition, const NIBBLETYPE a_BlockMeta);

	/** Called when one of the neighbors gets set; equivalent to MC block update.
	By default drops (DropBlockAsPickup() / SetBlock()) if the position is no longer suitable (CanBeAt(), DoesDropOnUnsuitable()),
	otherwise wakes up all simulators on the block. */
	virtual void Check(
		cChunkInterface & ChunkInterface, cBlockPluginInterface & a_PluginInterface,
		Vector3i a_RelPos,
		cChunk & a_Chunk
	);

	/** Returns the base colour ID of the block, as will be represented on a map, as per documentation: https://minecraft.gamepedia.com/Map_item_format */
	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta);

	/** Rotates a given block meta counter-clockwise. Default: no change
	Returns block meta following rotation */
	virtual NIBBLETYPE MetaRotateCCW(NIBBLETYPE a_Meta) { return a_Meta; }

	/** Rotates a given block meta clockwise. Default: no change
	Returns block meta following rotation */
	virtual NIBBLETYPE MetaRotateCW(NIBBLETYPE a_Meta) { return a_Meta; }

	/** Mirrors a given block meta around the XY plane. Default: no change
	Returns block meta following rotation */
	virtual NIBBLETYPE MetaMirrorXY(NIBBLETYPE a_Meta) { return a_Meta; }

	/** Mirros a given block meta around the XZ plane. Default: no change
	Returns block meta following rotation */
	virtual NIBBLETYPE MetaMirrorXZ(NIBBLETYPE a_Meta) { return a_Meta; }

	/** Mirros a given block meta around the YZ plane. Default: no change
	Returns block meta following rotation */
	virtual NIBBLETYPE MetaMirrorYZ(NIBBLETYPE a_Meta) { return a_Meta; }

	/** Grows this block, if it supports growing, by the specified amount of stages (at most).
	Returns the number of stages actually grown, zero if not supported (default). */
	virtual int Grow(cChunk & a_Chunk, Vector3i a_RelPos, int a_NumStages = 1) { return 0; }

	/** Returns true if the specified tool is valid and has a non-zero silk-touch enchantment.
	Helper used in many ConvertToPickups() implementations. */
	static bool ToolHasSilkTouch(const cItem * a_Tool);


protected:
	BLOCKTYPE m_BlockType;

	// Creates a new blockhandler for the given block type. For internal use only, use ::GetBlockHandler() instead.
	static cBlockHandler * CreateBlockHandler(BLOCKTYPE a_BlockType);

	friend class cBlockInfo;
};




namespace Temporary
{
	/** Registers all the BlockHandler descendants in the specified registry.
	Temporary, since this will later be performed in a plugin that provides the vanilla blocks. */
	void RegisterAllBlockHandlers(BlockTypeRegistry & aRegistry);
};
