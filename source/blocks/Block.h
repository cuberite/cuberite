#pragma once
#include "../Defines.h"

class cWorld;
class cPlayer;



class cBlockHandler
{
public:
	cBlockHandler(BLOCKTYPE a_BlockID);

	// Called when the block gets ticked either by a random tick or by a queued tick
	virtual void OnUpdate(cWorld *a_World, int a_X, int a_Y, int a_Z);

	// Will be called by cBlockHandler::PlaceBlock after the player has placed a new block
	virtual void OnPlacedByPlayer(cWorld *a_World, cPlayer * a_Player, int a_X, int a_Y, int a_Z, int a_Dir);
	// Will be called before the player has destroyed a block
	virtual void OnDestroyedByPlayer(cWorld *a_World, cPlayer * a_Player, int a_X, int a_Y, int a_Z);
	// Will be called when a new block was placed. Will be called before OnPlacedByPlayer
	virtual void OnPlaced(cWorld *a_World, int a_X, int a_Y, int a_Z, int a_Dir);
	// Will be called before a block gets destroyed / replaced with air
	virtual void OnDestroyed(cWorld *a_World, int a_X, int a_Y, int a_Z);
	// Will be called when a direct neighbor of this block has been changed (The position is the own position, not the neighbor position)
	virtual void OnNeighborChanged(cWorld *a_World, int a_X, int a_Y, int a_Z);
	// Notifies all neighbors of the give block about a change
	static void NeighborChanged(cWorld *a_World, int a_X, int a_Y, int a_Z);
	// Will be called while the player diggs the block.
	virtual void OnDigging(cWorld *a_World, cPlayer *a_Player, int a_X, int a_Y, int a_Z);
	// Will be called if the user right clicks the block and the block is useable
	virtual void OnUse(cWorld *a_World, cPlayer *a_Player, int a_X, int a_Y, int a_Z);
	// This function handles the real block placement for the give block by a player and also calls the OnPlacedByPlayer function
	virtual void PlaceBlock(cWorld *a_World, cPlayer *a_Player, NIBBLETYPE a_BlockMeta, int a_X, int a_Y, int a_Z, char a_Dir);

	// Indicates how much items are dropped DEFAULT: 1
	virtual char GetDropCount();
	// Indicates the id dropped by this block DEFAULT: BlockID
	virtual int GetDropID();
	// Indicates the Drop Meta data based on the block meta DEFAULT: BlockMeta
	virtual NIBBLETYPE GetDropMeta(NIBBLETYPE a_BlockMeta);
	// This function handles the dropping of a block based on the Drop id, drop count and drop meta. This will not destroy the block
	virtual void DropBlock(cWorld *a_World, int a_X, int a_Y, int a_Z);
	/// Returns step sound name of block
	virtual AString GetStepSound();
	
	// Indicates whether this block needs random ticks DEFAULT: False
	virtual bool NeedsRandomTicks();
	
	/// Checks if the block can stay at the specified coords in the world
	virtual bool CanBeAt(cWorld *a_World, int a_BlockX, int a_BlockY, int a_BlockZ);
	
	/// Checks if the block can be placed at this point. Default: CanBeAt(...) NOTE: This call doesn't actually place the block
	virtual bool CanBePlacedAt(cWorld * a_World, int a_BlockX, int a_BlockY, int a_BlockZ, char a_Dir);
	
	/// Called when the player tries to place a block on top of this block (Only if he aims directly on this block); return false to disallow
	virtual bool AllowBlockOnTop(void);
	
	/// Called to check whether this block supports a rclk action. If it returns true, OnUse() is called
	virtual bool IsUseable(void);
	
	// Indicates whether the client will click through this block. For example digging a fire will hit the block below the fire so fire is clicked through
	virtual bool IsClickedThrough(void);
	
	// Checks if the player can build "inside" this block. For example blocks placed "on" snow will be placed at the same position. So: Snow ignores Build collision
	virtual bool IgnoreBuildCollision(void);
	
	/// Indicates this block can be placed on the side of other blocks. Default: true
	virtual bool CanBePlacedOnSide();
	
	/// Does this block drop if it gets destroyed by an unsuitable situation? Default: true
	virtual bool DropOnUnsuitable();
	

	// Static function to get the blockhandler for an specific block id
	static cBlockHandler * GetBlockHandler(BLOCKTYPE a_BlockID);

	// Deletes all initialised block handlers
	static void Deinit();
	
protected:
	BLOCKTYPE m_BlockID;
	// Creates a new blockhandler for the given block id. For internal use only, use GetBlockHandler instead.
	static cBlockHandler *CreateBlockHandler(BLOCKTYPE a_BlockID);
	static cBlockHandler *m_BlockHandler[256];
	static bool m_HandlerInitialized;	//used to detect if the blockhandlers are initialized
};

// Shortcut to get the blockhandler for a specific block
inline cBlockHandler *BlockHandler(BLOCKTYPE a_BlockID) { return cBlockHandler::GetBlockHandler(a_BlockID); }