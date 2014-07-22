
#pragma once

#include "../Defines.h"
#include "../Item.h"
#include "WorldInterface.h"
#include "ChunkInterface.h"
#include "BlockPluginInterface.h"





// fwd:
class cPlayer;
class cChunk;





class cBlockHandler
{
public:
	cBlockHandler(BLOCKTYPE a_BlockType);
	
	virtual ~cBlockHandler() {}

	/// Called when the block gets ticked either by a random tick or by a queued tick.
	/// Note that the coords are chunk-relative!
	virtual void OnUpdate(cChunkInterface & cChunkInterface, cWorldInterface & a_WorldInterface, cBlockPluginInterface & a_BlockPluginInterface, cChunk & a_Chunk, int a_RelX, int a_RelY, int a_RelZ);

	/** Called before a block is placed into a world.
	The handler should return true to allow placement, false to refuse.
	Also, the handler should set a_BlockType and a_BlockMeta to correct values for the newly placed block.
	Called by cItemHandler::GetPlacementBlockTypeMeta() if the item is a block
	*/
	virtual bool GetPlacementBlockTypeMeta(
		cChunkInterface & a_ChunkInterface, cPlayer * a_Player,
		int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace,
		int a_CursorX, int a_CursorY, int a_CursorZ,
		BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta
	);

	/// Called by cWorld::SetBlock() after the block has been set
	virtual void OnPlaced(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta);
	
	/// Called by cClientHandle::HandlePlaceBlock() after the player has placed a new block. Called after OnPlaced().
	virtual void OnPlacedByPlayer(
		cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, cPlayer * a_Player,
		int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace,
		int a_CursorX, int a_CursorY, int a_CursorZ,
		BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta
	);
	
	/// Called before the player has destroyed a block
	virtual void OnDestroyedByPlayer(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, cPlayer * a_Player, int a_BlockX, int a_BlockY, int a_BlockZ);
	
	/// Called before a block gets destroyed / replaced with air
	virtual void OnDestroyed(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, int a_BlockX, int a_BlockY, int a_BlockZ);
	
	/// Called when a direct neighbor of this block has been changed (The position is the own position, not the neighbor position)
	virtual void OnNeighborChanged(cChunkInterface & a_ChunkInterface, int a_BlockX, int a_BlockY, int a_BlockZ) {};
	
	/// Notifies all neighbors of the given block about a change
	static void NeighborChanged(cChunkInterface & a_ChunkInterface, int a_BlockX, int a_BlockY, int a_BlockZ);
	
	/// Called while the player diggs the block.
	virtual void OnDigging(cChunkInterface & cChunkInterface, cWorldInterface & a_WorldInterface, cPlayer * a_Player, int a_BlockX, int a_BlockY, int a_BlockZ) {};
	
	/// Called if the user right clicks the block and the block is useable
	virtual void OnUse(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, cPlayer * a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ) {};
	
	/** Called when a right click to this block is cancelled */
	virtual void OnCancelRightClick(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, cPlayer * a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace) {};
	
	/// <summary>Called when the item is mined to convert it into pickups. Pickups may specify multiple items. Appends items to a_Pickups, preserves its original contents</summary>
	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta);
	
	/** Handles the dropping, but not destruction, of a block based on what ConvertTo(Verbatim)Pickups() returns, including the spawning of pickups and alertion of plugins
	@param a_Digger The entity causing the drop; it may be NULL
	@param a_DropVerbatim Calls ConvertToVerbatimPickups() instead of its counterpart, meaning the block itself is dropped by default (due to a speical tool or enchantment)
	*/
	virtual void DropBlock(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, cBlockPluginInterface & a_BlockPluginInterface, cEntity * a_Digger, int a_BlockX, int a_BlockY, int a_BlockZ, bool a_DropVerbatim = false);
	
	/// Returns step sound name of block
	virtual const char * GetStepSound(void);
	
	/// Checks if the block can stay at the specified relative coords in the chunk
	virtual bool CanBeAt(cChunkInterface & a_ChunkInterface, int a_RelX, int a_RelY, int a_RelZ, const cChunk & a_Chunk);

	/** Can the dirt under this block grow to grass? */
	virtual bool CanDirtGrowGrass(NIBBLETYPE a_Meta);
	
	/** Checks if the block can be placed at this point.
	Default: CanBeAt(...)
	NOTE: This call doesn't actually place the block
	*/
	// virtual bool CanBePlacedAt(cWorld * a_World, int a_BlockX, int a_BlockY, int a_BlockZ, char a_Dir);
		
	/// Called to check whether this block supports a rclk action. If it returns true, OnUse() is called
	virtual bool IsUseable(void);
	
	/** Indicates whether the client will click through this block.
	For example digging a fire will hit the block below the fire so fire is clicked through
	*/
	virtual bool IsClickedThrough(void);
	
	/** Checks if the player can build "inside" this block.
	For example blocks placed "on" snow will be placed at the same position. So: Snow ignores Build collision
	*/
	virtual bool DoesIgnoreBuildCollision(void);

	/// <summary>Similar to DoesIgnoreBuildCollision(void), but is used for cases where block meta/player item-in-hand is needed to determine collision (thin snow)</summary>
	virtual bool DoesIgnoreBuildCollision(cPlayer *, NIBBLETYPE a_Meta)
	{
		UNUSED(a_Meta);
		return DoesIgnoreBuildCollision();
	}

	/// <summary>Returns if this block drops if it gets destroyed by an unsuitable situation. Default: true</summary>
	virtual bool DoesDropOnUnsuitable(void);
	
	/** Called when one of the neighbors gets set; equivalent to MC block update.
	By default drops if position no more suitable (CanBeAt(), DoesDropOnUnsuitable(), Drop()),
	and wakes up all simulators on the block.
	*/
	virtual void Check(cChunkInterface & ChunkInterface, cBlockPluginInterface & a_PluginInterface, int a_RelX, int a_RelY, int a_RelZ, cChunk & a_Chunk);
	
	/// <summary>Rotates a given block meta counter-clockwise. Default: no change</summary>
	/// <returns>Block meta following rotation</returns>
	virtual NIBBLETYPE MetaRotateCCW(NIBBLETYPE a_Meta) { return a_Meta; }
	
	/// <summary>Rotates a given block meta clockwise. Default: no change</summary>
	/// <returns>Block meta following rotation</returns>
	virtual NIBBLETYPE MetaRotateCW(NIBBLETYPE a_Meta) { return a_Meta; }
	
	/// <summary>Mirros a given block meta around the XY plane. Default: no change</summary>
	/// <returns>Block meta following mirroring</returns>
	virtual NIBBLETYPE MetaMirrorXY(NIBBLETYPE a_Meta) { return a_Meta; }

	/// <summary>Mirros a given block meta around the XZ plane. Default: no change</summary>
	/// <returns>Block meta following mirroring</returns>
	virtual NIBBLETYPE MetaMirrorXZ(NIBBLETYPE a_Meta) { return a_Meta; }

	/// <summary>Mirros a given block meta around the YZ plane. Default: no change</summary>
	/// <returns>Block meta following mirroring</returns>
	virtual NIBBLETYPE MetaMirrorYZ(NIBBLETYPE a_Meta) { return a_Meta; }
	
protected:
	BLOCKTYPE m_BlockType;
	
	// Creates a new blockhandler for the given block type. For internal use only, use ::GetBlockHandler() instead.
	static cBlockHandler * CreateBlockHandler(BLOCKTYPE a_BlockType);

	friend class cBlockInfo;
};




