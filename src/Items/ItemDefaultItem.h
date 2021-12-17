
#pragma once

#include "ItemHandler.h"





class cDefaultItemHandler final :
	public cItemHandler
{
public:

	using cItemHandler::cItemHandler;
};

class cUnimplementedItemHandler final :
	public cItemHandler
{
public:
	using cItemHandler::cItemHandler;

	virtual bool OnItemUse(
		cWorld * a_World,
		cPlayer * a_Player,
		cBlockPluginInterface & a_PluginInterface,
		const cItem & a_HeldItem,
		const Vector3i a_ClickedBlockPos,
		eBlockFace a_ClickedBlockFace
	) const override
	{
		ASSERT(!"Unimplemented item handler called!");
		return false;
	}

	virtual void OnItemShoot(cPlayer *, const Vector3i a_BlockPos, eBlockFace a_BlockFace) const override
	{
		ASSERT(!"Unimplemented item handler called!");
	}

	virtual void OnUpdate(cWorld * a_World, cPlayer * a_Player, const cItem & a_Item) const override
	{
		ASSERT(!"Unimplemented item handler called!");
	}

	virtual bool OnDiggingBlock(
			cWorld * a_World, cPlayer * a_Player, const cItem & a_HeldItem,
			const Vector3i a_ClickedBlockPos,
			eBlockFace a_ClickedBlockFace
	) const override
	{
		ASSERT(!"Unimplemented item handler called!");
		return false;
	}

	virtual void OnEntityAttack(cPlayer * a_Attacker, cEntity * a_AttackedEntity) const override
	{
		ASSERT(!"Unimplemented item handler called!");
	}

	virtual void OnFoodEaten(cWorld *a_World, cPlayer *a_Player, cItem *a_Item) const override
	{
		ASSERT(!"Unimplemented item handler called!");
	}

	virtual short GetDurabilityLossByAction(eDurabilityLostAction a_Action) const override
	{
		ASSERT(!"Unimplemented item handler called!");
		return 0;
	}

	virtual FoodInfo GetFoodInfo(const cItem * a_Item) const override
	{
		ASSERT(!"Unimplemented item handler called!");
		return FoodInfo(0, 0);
	}

	/** Lets the player eat a selected item. Returns true if the player ate the item */
	virtual bool EatItem(cPlayer * a_Player, cItem * a_Item) const override
	{
		ASSERT(!"Unimplemented item handler called!");
		return false;
	}

	/** Indicates if this item is food */
	virtual bool IsFood(void) const override
	{
		ASSERT(!"Unimplemented item handler called!");
		return false;
	}

	/** Indicates if this item is drinkable */
	virtual bool IsDrinkable(short a_ItemDamage) const override
	{
		ASSERT(!"Unimplemented item handler called!");
		return false;
	}

	/** Blocks simply get placed */
	virtual bool IsPlaceable(void) const override
	{
		ASSERT(!"Unimplemented item handler called!");
		return false;
	}

	/** Can the anvil repair this item, when a_Item is the second input? */
	virtual bool CanRepairWithRawMaterial(const cItem & a_Item) const override
	{
		ASSERT(!"Unimplemented item handler called!");
		return false;
	}

	/** Returns whether this tool / item can harvest a specific block (e.g. iron pickaxe can harvest diamond ore, but wooden one can't).
	Defaults to false unless overridden. */
	virtual bool CanHarvestBlock(BlockState a_Block) const override
	{
		ASSERT(!"Unimplemented item handler called!");
		return false;
	}

	virtual float GetBlockBreakingStrength(BlockState a_Block) const override
	{
		ASSERT(!"Unimplemented item handler called!");
		return 1.0f;
	}

protected:
	bool CommitPlacement(cPlayer & aPlayer, const cItem & aHeldItem, const Vector3i aPlacePosition, const eBlockFace aClickedBlockFace, const Vector3i aCursorPosition) const override
	{
		ASSERT(!"Unimplemented item handler called!");
		return false;
	}
};
