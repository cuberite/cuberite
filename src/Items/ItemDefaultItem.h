
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
		FLOGERROR("{}: Unimplemented item handler called for item {}", __FUNCTION__, m_ItemType);
		// ASSERT(!"Unimplemented item handler called!");
		return false;
	}

	virtual void OnItemShoot(cPlayer *, const Vector3i a_BlockPos, eBlockFace a_BlockFace) const override
	{
		FLOGERROR("{}: Unimplemented item handler called for item {}", __FUNCTION__, m_ItemType);
		// ASSERT(!"Unimplemented item handler called!");
	}

	virtual void OnUpdate(cWorld * a_World, cPlayer * a_Player, const cItem & a_Item) const override
	{
		FLOGERROR("{}: Unimplemented item handler called for item {}", __FUNCTION__, m_ItemType);
		// ASSERT(!"Unimplemented item handler called!");
	}

	virtual bool OnDiggingBlock(
			cWorld * a_World, cPlayer * a_Player, const cItem & a_HeldItem,
			const Vector3i a_ClickedBlockPos,
			eBlockFace a_ClickedBlockFace
	) const override
	{
		FLOGERROR("{}: Unimplemented item handler called for item {}", __FUNCTION__, m_ItemType);
		// ASSERT(!"Unimplemented item handler called!");
		return false;
	}

	virtual void OnEntityAttack(cPlayer * a_Attacker, cEntity * a_AttackedEntity) const override
	{
		FLOGERROR("{}: Unimplemented item handler called for item {}", __FUNCTION__, m_ItemType);
		// ASSERT(!"Unimplemented item handler called!");
	}

	virtual void OnFoodEaten(cWorld *a_World, cPlayer *a_Player, cItem *a_Item) const override
	{
		FLOGERROR("{}: Unimplemented item handler called for item {}", __FUNCTION__, m_ItemType);
		// ASSERT(!"Unimplemented item handler called!");
	}

	virtual short GetDurabilityLossByAction(eDurabilityLostAction a_Action) const override
	{
		FLOGERROR("{}: Unimplemented item handler called for item {}", __FUNCTION__, m_ItemType);
		// ASSERT(!"Unimplemented item handler called!");
		return 0;
	}

	virtual FoodInfo GetFoodInfo(const cItem * a_Item) const override
	{
		FLOGERROR("{}: Unimplemented item handler called for item {}", __FUNCTION__, m_ItemType);
		// ASSERT(!"Unimplemented item handler called!");
		return FoodInfo(0, 0);
	}

	virtual bool EatItem(cPlayer * a_Player, cItem * a_Item) const override
	{
		FLOGERROR("{}: Unimplemented item handler called for item {}", __FUNCTION__, m_ItemType);
		// ASSERT(!"Unimplemented item handler called!");
		return false;
	}

	virtual bool IsFood(void) const override
	{
		FLOGERROR("{}: Unimplemented item handler called for item {}", __FUNCTION__, m_ItemType);
		// ASSERT(!"Unimplemented item handler called!");
		return false;
	}

	virtual bool IsDrinkable(short a_ItemDamage) const override
	{
		FLOGERROR("{}: Unimplemented item handler called for item {}", __FUNCTION__, m_ItemType);
		// ASSERT(!"Unimplemented item handler called!");
		return false;
	}

	virtual bool IsPlaceable(void) const override
	{
		FLOGERROR("{}: Unimplemented item handler called for item {}", __FUNCTION__, m_ItemType);
		// ASSERT(!"Unimplemented item handler called!");
		return false;
	}

	virtual bool CanRepairWithRawMaterial(const cItem & a_Item) const override
	{
		FLOGERROR("{}: Unimplemented item handler called for item {}", __FUNCTION__, m_ItemType);
		// ASSERT(!"Unimplemented item handler called!");
		return false;
	}

	virtual bool CanHarvestBlock(BlockState a_Block) const override
	{
		FLOGERROR("{}: Unimplemented item handler called for item {}", __FUNCTION__, m_ItemType);
		// ASSERT(!"Unimplemented item handler called!");
		return false;
	}

	virtual float GetBlockBreakingStrength(BlockState a_Block) const override
	{
		FLOGERROR("{}: Unimplemented item handler called for item {}", __FUNCTION__, m_ItemType);
		// ASSERT(!"Unimplemented item handler called!");
		return 1.0f;
	}

protected:
	bool CommitPlacement(cPlayer & aPlayer, const cItem & aHeldItem, const Vector3i aPlacePosition, const eBlockFace aClickedBlockFace, const Vector3i aCursorPosition) const override
	{
		FLOGERROR("{}: Unimplemented item handler called for item {}", __FUNCTION__, m_ItemType);
		// ASSERT(!"Unimplemented item handler called!");
		return false;
	}
};
