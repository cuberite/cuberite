
#pragma once

#include "../Defines.h"
#include "../Item.h"





// fwd:
class cWorld;
class cPlayer;





class cItemHandler
{
public:
	cItemHandler(int a_ItemType);
	
	/// Called when the player tries to use the item. Return false to make the item unusable. DEFAULT: False
	virtual bool OnItemUse(cWorld *a_World, cPlayer *a_Player, cItem *a_Item, int a_BlockX, int a_BlockY, int a_BlockZ, char a_Dir);	//eg for fishing or hoes
	
	/// Called while the player diggs a block using this item
	virtual bool OnDiggingBlock(cWorld * a_World, cPlayer * a_Player, cItem * a_HeldItem, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace);
	
	/// Called when the player destroys a block using this item. This also calls the drop function for the destroyed block
	virtual void OnBlockDestroyed(cWorld * a_World, cPlayer * a_Player, cItem *a_Item, int a_X, int a_Y, int a_Z);
	
	/// Called after the player has eaten this item.
	virtual void OnFoodEaten(cWorld *a_World, cPlayer *a_Player, cItem *a_Item);
	
	/// Returns the maximum stack size for a given item
	virtual char GetMaxStackSize(void);

	struct FoodInfo
	{
		FoodInfo(short a_FoodLevel, float a_Saturation, char a_PoisionChance = 0)
		{
			FoodLevel = a_FoodLevel;
			Saturation = a_Saturation;
			PoisionChance = a_PoisionChance;
		}
		short FoodLevel;
		float Saturation;
		char PoisionChance;	//0 - 100
	};

	/// Returns the FoodInfo for this item. (FoodRecovery, Saturation and PoisionChance)
	virtual FoodInfo GetFoodInfo();
	
	/// Lets the player eat a selected item. Returns true if the player ate the item
	virtual bool EatItem(cPlayer *a_Player, cItem *a_Item);

	/// Indicates if this item is a tool
	virtual bool IsTool(void);
	
	/// Indicates if this item is food
	virtual bool IsFood(void);
	
	/// Blocks simply get placed
	virtual bool IsPlaceable(void);

	/** Called before a block is placed	into a world. 
	The handler should return true to allow placement, false to refuse.
	Also, the handler should set a_BlockType and a_BlockMeta to correct values for the newly placed block.
	*/
	virtual bool GetPlacementBlockTypeMeta(
		cWorld * a_World, cPlayer * a_Player,
		int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, 
		int a_CursorX, int a_CursorY, int a_CursorZ,
		BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta
	);
	
	/// Returns whether this tool/item can harvest a specific block (e.g. wooden pickaxe can harvest stone, but wood can´t) DEFAULT: False
	virtual bool CanHarvestBlock(BLOCKTYPE a_BlockType);

	static cItemHandler * GetItemHandler(int a_ItemType);
	static cItemHandler * GetItemHandler(const cItem & a_Item) { return GetItemHandler(a_Item.m_ItemType); }

	static void Deinit();
	
protected:
	int m_ItemType;
	static cItemHandler *CreateItemHandler(int m_ItemType);

	static cItemHandler *m_ItemHandler[2268];
	static bool m_HandlerInitialized;	//used to detect if the itemhandlers are initialized
};

//Short function
inline cItemHandler *ItemHandler(int a_ItemType) { return cItemHandler::GetItemHandler(a_ItemType); }
