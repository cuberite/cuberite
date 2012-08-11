
#pragma once
#include "../Defines.h"





// fwd:
class cWorld;
class cPlayer;





class cItemHandler
{
public:
	cItemHandler(int a_ItemID);
	virtual bool OnItemUse(cWorld *a_World, cPlayer *a_Player, cItem *a_Item, int a_X, int a_Y, int a_Z, char a_Dir);	//eg for fishing or hoes
	virtual bool OnDiggingBlock(cWorld *a_World, cPlayer *a_Player, cItem *a_Item, int a_X, int a_Y, int a_Z, char a_Dir);
	virtual void OnBlockDestroyed(cWorld *a_World, cPlayer *a_Player, cItem *a_Item, int a_X, int a_Y, int a_Z);
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

	virtual FoodInfo GetFoodInfo();
	
	virtual bool EatItem(cPlayer *a_Player, cItem *a_Item);

	virtual void PlaceBlock(cWorld *a_World, cPlayer *a_Player, cItem *a_Item, int a_X, int a_Y, int a_Z, char a_Dir);
	
	virtual bool IsTool();
	virtual bool IsFood();
	//Blocks simply get placed
	virtual bool IsPlaceable();

	virtual BLOCKTYPE GetBlockType();
	virtual NIBBLETYPE GetBlockMeta(short a_ItemDamage);

	virtual bool CanHarvestBlock(BLOCKTYPE a_BlockID);

	static cItemHandler *GetItemHandler(int a_ItemID);

	static void Deinit();

	
protected:
	int m_ItemID;
	static cItemHandler *CreateItemHandler(int m_ItemID);

	static cItemHandler *m_ItemHandler[2266];
	static bool m_HandlerInitialized;	//used to detect if the itemhandlers are initialized
};

//Short function
inline cItemHandler *ItemHandler(int a_ItemID) { return cItemHandler::GetItemHandler(a_ItemID); }