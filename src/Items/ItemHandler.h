
#pragma once

#include "../Defines.h"
#include "../Item.h"
#include "../Entities/EntityEffect.h"





// fwd:
class cWorld;
class cPlayer;





class cItemHandler
{
public:

	enum eDurabilityLostAction
	{
		dlaBreakBlock,
		dlaAttackEntity,
	};

	cItemHandler(int a_ItemType);
	
	/** Force virtual destructor */
	virtual ~cItemHandler() {}
	
	/** Called when the player tries to use the item (right mouse button). Return false to make the item unusable. DEFAULT: False */
	virtual bool OnItemUse(cWorld * a_World, cPlayer * a_Player, const cItem & a_Item, int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_Dir);
	
	/** Called when the client sends the SHOOT status in the lclk packet */
	virtual void OnItemShoot(cPlayer *, int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace)
	{
		UNUSED(a_BlockX);
		UNUSED(a_BlockY);
		UNUSED(a_BlockZ);
		UNUSED(a_BlockFace);
	}

	/** Called every tick while the item is on the player's inventory (Used by maps) - For now, called only for equipped items */
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

	/** Get the durability lost which the item will get, when a specified action was performed. */
	virtual short GetDurabilityLossByAction(eDurabilityLostAction a_Action);

	/** Returns the maximum stack size for a given item */
	virtual char GetMaxStackSize(void);

	struct FoodInfo
	{
		int    FoodLevel;
		double Saturation;

		FoodInfo(int a_FoodLevel, double a_Saturation, int a_PoisonChance = 0) :
			FoodLevel(a_FoodLevel),
			Saturation(a_Saturation)
		{
		}
	} ;

	/** Returns the FoodInfo for this item. (FoodRecovery and Saturation) */
	virtual FoodInfo GetFoodInfo();

	/** If this function returns true, it sets the arguments to a effect who will be activated when you eat the item. */
	virtual bool GetEatEffect(cEntityEffect::eType & a_EffectType, int & a_EffectDurationTicks, short & a_EffectIntensity, float & a_Chance);

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

	/** Called before a block is placed into a world.
	The handler should return true to allow placement, false to refuse.
	Also, the handler should set a_BlockType and a_BlockMeta to correct values for the newly placed block.
	*/
	virtual bool GetPlacementBlockTypeMeta(
		cWorld * a_World, cPlayer * a_Player,
		int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace,
		int a_CursorX, int a_CursorY, int a_CursorZ,
		BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta
	);
	
	/** Returns whether this tool/item can harvest a specific block (e.g. wooden pickaxe can harvest stone, but wood can't) DEFAULT: False */
	virtual bool CanHarvestBlock(BLOCKTYPE a_BlockType);

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
