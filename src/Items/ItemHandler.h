
#pragma once

#include "../Defines.h"
#include "../Item.h"





// fwd:
class cWorld;
class cPlayer;
class cBlockPluginInterface;





class cItemHandler
{
public:

	/** Actions that may cause durability of an item may be lost, where the
	magnitude of the loss depends on the specific item used to perform the
	action */
	enum eDurabilityLostAction
	{
		dlaAttackEntity,
		dlaBreakBlock,
		dlaBreakBlockInstant,
	};

	cItemHandler(int a_ItemType);

	/** Force virtual destructor */
	virtual ~cItemHandler() {}


	/** Called when the player tries to place the item (right mouse button, IsPlaceable() == true).
	The block coords are for the block that has been clicked.
	The default handler uses GetBlocksToPlace() and places the returned blocks.
	Override if the item needs advanced processing, such as spawning a mob based on the blocks being placed.
	If the block placement is refused inside this call, it will automatically revert the client-side changes.
	Returns true if the placement succeeded, false if the placement was aborted for any reason. */
	virtual bool OnPlayerPlace(
		cWorld & a_World, cPlayer & a_Player, const cItem & a_EquippedItem,
		int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace,
		int a_CursorX, int a_CursorY, int a_CursorZ
	);


	/** Called from OnPlayerPlace() to determine the blocks that the current placement operation should set.
	The block coords are where the new (main) block should be placed.
	The default handler uses GetPlacementBlockTypeMeta() and provides that as the single block at the specified coords.
	Returns true if the placement succeeded, false if the placement was aborted for any reason.
	If aborted, the server then sends all original blocks in the coords provided in a_BlocksToSet to the client. */
	virtual bool GetBlocksToPlace(
		cWorld & a_World, cPlayer & a_Player, const cItem & a_EquippedItem,
		int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace,
		int a_CursorX, int a_CursorY, int a_CursorZ,
		sSetBlockVector & a_BlocksToSet
	);


	/** Called when the player right-clicks with this item and IsPlaceable() == true, and OnPlayerPlace() is not overridden.
	This function should provide the block type and meta for the placed block, or refuse the placement.
	Returns true to allow placement, false to refuse. */
	virtual bool GetPlacementBlockTypeMeta(
		cWorld * a_World, cPlayer * a_Player,
		int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace,
		int a_CursorX, int a_CursorY, int a_CursorZ,
		BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta
	);


	/** Called when the player tries to use the item (right mouse button).
	Return false to abort the usage. DEFAULT: False */
	virtual bool OnItemUse(
		cWorld * a_World, cPlayer * a_Player, cBlockPluginInterface & a_PluginInterface, const cItem & a_Item,
		int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace
	);


	/** Called when the client sends the SHOOT status in the lclk packet */
	virtual void OnItemShoot(cPlayer *, int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace)
	{
		UNUSED(a_BlockX);
		UNUSED(a_BlockY);
		UNUSED(a_BlockZ);
		UNUSED(a_BlockFace);
	}

	/** Called every tick while the item is on the player's inventory (used by maps, for example) - For now, called only for equipped items */
	virtual void OnUpdate(cWorld * a_World, cPlayer * a_Player, const cItem & a_Item)
	{
		UNUSED(a_World);
		UNUSED(a_Player);
		UNUSED(a_Item);
	}

	/** Called while the player diggs a block using this item */
	virtual bool OnDiggingBlock(cWorld * a_World, cPlayer * a_Player, const cItem & a_HeldItem, int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace);

	/** Called when the player destroys a block using this item. This also calls the drop function for the destroyed block */
	virtual void OnBlockDestroyed(cWorld * a_World, cPlayer * a_Player, const cItem & a_Item, int a_BlockX, int a_BlockY, int a_BlockZ);

	/** Called when a player attacks a other entity. */
	virtual void OnEntityAttack(cPlayer * a_Attacker, cEntity * a_AttackedEntity);

	/** Called after the player has eaten this item. */
	virtual void OnFoodEaten(cWorld *a_World, cPlayer *a_Player, cItem *a_Item);

	/** Get the durability lost which the item will get, when a specified action
	was performed. This is only relevant for uses where the damage taken may
	depend on the item used. */
	virtual short GetDurabilityLossByAction(eDurabilityLostAction a_Action);

	/** Returns the maximum stack size for a given item */
	virtual char GetMaxStackSize(void);

	struct FoodInfo
	{
		int    FoodLevel;
		double Saturation;

		FoodInfo(int a_FoodLevel, double a_Saturation) :
			FoodLevel(a_FoodLevel),
			Saturation(a_Saturation)
		{
		}
	} ;

	/** Returns the FoodInfo for this item. (FoodRecovery and Saturation) */
	virtual FoodInfo GetFoodInfo(const cItem * a_Item);

	/** Lets the player eat a selected item. Returns true if the player ate the item */
	virtual bool EatItem(cPlayer * a_Player, cItem * a_Item);

	/** Indicates if this item is a tool */
	virtual bool IsTool(void);

	/** Indicates if this item is food */
	virtual bool IsFood(void);

	/** Indicates if this item is drinkable */
	virtual bool IsDrinkable(short a_ItemDamage);

	/** Blocks simply get placed */
	virtual bool IsPlaceable(void);

	/** Can the anvil repair this item, when a_Item is the second input? */
	virtual bool CanRepairWithRawMaterial(short a_ItemType);

	/** Returns whether this tool / item can harvest a specific block (e.g. iron pickaxe can harvest diamond ore, but wooden one can't).
	Defaults to false unless overridden. */
	virtual bool CanHarvestBlock(BLOCKTYPE a_BlockType);

	/** Returns the strength to break a specific block.
	Defaults to 1 unless overriden. */
	virtual float GetBlockBreakingStrength(BLOCKTYPE a_Block);

	static cItemHandler * GetItemHandler(int a_ItemType);
	static cItemHandler * GetItemHandler(const cItem & a_Item) { return GetItemHandler(a_Item.m_ItemType); }

	static void Deinit();

protected:
	int m_ItemType;
	static cItemHandler * CreateItemHandler(int m_ItemType);

	static cItemHandler * m_ItemHandler[E_ITEM_LAST + 1];
	static bool m_HandlerInitialized;  // used to detect if the itemhandlers are initialized
};

// Short function
inline cItemHandler *ItemHandler(int a_ItemType) { return cItemHandler::GetItemHandler(a_ItemType); }
