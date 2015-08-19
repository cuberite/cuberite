
#pragma once

#include "../Defines.h"





// fwd:
class cPlayer;
class cChunk;
class cBlockPluginInterface;
class cChunkInterface;
class cWorldInterface;
class cItems;





class cBlockHandler
{
public:
	cBlockHandler(BLOCKTYPE a_BlockType);
	
	virtual ~cBlockHandler() {}

	/** Called when the block gets ticked either by a random tick or by a queued tick.
	Note that the coords are chunk-relative! */
	virtual void OnUpdate(cChunkInterface & cChunkInterface, cWorldInterface & a_WorldInterface, cBlockPluginInterface & a_BlockPluginInterface, cChunk & a_Chunk, int a_RelX, int a_RelY, int a_RelZ);

	/** Called before a block is placed into a world.
	The handler should return true to allow placement, false to refuse.
	Also, the handler should set a_BlockType and a_BlockMeta to correct values for the newly placed block.
	Called by cItemHandler::GetPlacementBlockTypeMeta() if the item is a block */
	virtual bool GetPlacementBlockTypeMeta(
		cChunkInterface & a_ChunkInterface, cPlayer * a_Player,
		int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace,
		int a_CursorX, int a_CursorY, int a_CursorZ,
		BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta
	);

	/** Called by cWorld::SetBlock() after the block has been set */
	virtual void OnPlaced(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta);
	
	/** Called by cPlayer::PlaceBlocks() for each block after it has been set to the world. Called after OnPlaced(). */
	virtual void OnPlacedByPlayer(
		cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, cPlayer * a_Player, const sSetBlock & a_BlockChange
	);
	
	/** Called before the player has destroyed a block */
	virtual void OnDestroyedByPlayer(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, cPlayer * a_Player, int a_BlockX, int a_BlockY, int a_BlockZ);
	
	/** Called before a block gets destroyed / replaced with air */
	virtual void OnDestroyed(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, int a_BlockX, int a_BlockY, int a_BlockZ);
	
	/** Called when a direct neighbor of this block has been changed (The position is the block's own position, not the changing neighbor's position)
	a_WhichNeighbor indicates which neighbor has changed. For example, BLOCK_FACE_YP meant the neighbor above has changed.
	BLOCK_FACE_NONE means that it is a neighbor not directly adjacent (diagonal, etc.) */
	virtual void OnNeighborChanged(cChunkInterface & a_ChunkInterface, int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_WhichNeighbor) {}
	
	/** Notifies the specified neighbor that the current block has changed.
	a_NeighborXYZ coords are the coords of the neighbor
	a_WhichNeighbor specifies which neighbor (relative to a_NeighborXYZ) has changed.
	For example BLOCK_FACE_YP means that the block at {a_NeighborX, a_NeighborY + 1, a_NeighborZ} has changed.
	BLOCK_FACE_NONE means that it is a neighbor not directly adjacent (diagonal, etc.) */
	static void NeighborChanged(cChunkInterface & a_ChunkInterface, int a_NeighborX, int a_NeighborY, int a_NeighborZ, eBlockFace a_WhichNeighbor);
	
	/** Called when the player starts digging the block. */
	virtual void OnDigging(cChunkInterface & cChunkInterface, cWorldInterface & a_WorldInterface, cPlayer * a_Player, int a_BlockX, int a_BlockY, int a_BlockZ) {}
	
	/** Called if the user right clicks the block and the block is useable */
	virtual void OnUse(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, cPlayer * a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ) {}
	
	/** Called when a right click to this block is cancelled */
	virtual void OnCancelRightClick(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, cPlayer * a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace) {}
	
	/** Called when the item is mined to convert it into pickups. Pickups may specify multiple items. Appends items to a_Pickups, preserves its original contents */
	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta);
	
	/** Handles the dropping, but not destruction, of a block based on what ConvertTo(Verbatim)Pickups() returns, including the spawning of pickups and alertion of plugins
	@param a_Digger The entity causing the drop; it may be nullptr
	@param a_CanDrop Informs the handler whether the block should be dropped at all. One example when this is false is when stone is destroyed by hand
	@param a_DropVerbatim Calls ConvertToVerbatimPickups() instead of its counterpart, meaning the block itself is dropped by default (due to a speical tool or enchantment)
	*/
	virtual void DropBlock(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, cBlockPluginInterface & a_BlockPluginInterface, cEntity * a_Digger, int a_BlockX, int a_BlockY, int a_BlockZ, bool a_CanDrop = true);
	
	/** Checks if the block can stay at the specified relative coords in the chunk */
	virtual bool CanBeAt(cChunkInterface & a_ChunkInterface, int a_RelX, int a_RelY, int a_RelZ, const cChunk & a_Chunk);

	/** Checks whether the block has an effect on growing the plant */
	virtual bool CanSustainPlant(BLOCKTYPE a_Plant) { return false; }
	
	/** Checks if the block can be placed at this point.
	Default: CanBeAt(...)
	NOTE: This call doesn't actually place the block
	*/
	// virtual bool CanBePlacedAt(cWorld * a_World, int a_BlockX, int a_BlockY, int a_BlockZ, char a_Dir);
		
	/** Called to check whether this block supports a rclk action.
	If it returns true, OnUse() is called */
	virtual bool IsUseable(void);
	
	/** Indicates whether the client will click through this block.
	For example digging a fire will hit the block below the fire so fire is clicked through
	*/
	virtual bool IsClickedThrough(void);
	
	/** Checks if the player can build "inside" this block.
	For example blocks placed "on" snow will be placed at the same position. So: Snow ignores Build collision
	*/
	virtual bool DoesIgnoreBuildCollision(void);

	/** Similar to DoesIgnoreBuildCollision(void), but is used for cases where block's meta or
	player's item-in-hand is needed to determine collision (thin snow) */
	virtual bool DoesIgnoreBuildCollision(cPlayer *, NIBBLETYPE a_Meta)
	{
		UNUSED(a_Meta);
		return DoesIgnoreBuildCollision();
	}

	/** Returns if this block drops if it gets destroyed by an unsuitable situation.
	Default: true */
	virtual bool DoesDropOnUnsuitable(void);
	
	/** Called when one of the neighbors gets set; equivalent to MC block update.
	By default drops if position no more suitable (CanBeAt(), DoesDropOnUnsuitable(), Drop()),
	and wakes up all simulators on the block. */
	virtual void Check(cChunkInterface & ChunkInterface, cBlockPluginInterface & a_PluginInterface, int a_RelX, int a_RelY, int a_RelZ, cChunk & a_Chunk);

	/** Returns the base colour ID of the block, as will be represented on a map, as per documentation: http://minecraft.gamepedia.com/Map_item_format */
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
	
protected:
	BLOCKTYPE m_BlockType;
	
	// Creates a new blockhandler for the given block type. For internal use only, use ::GetBlockHandler() instead.
	static cBlockHandler * CreateBlockHandler(BLOCKTYPE a_BlockType);

	friend class cBlockInfo;
};




