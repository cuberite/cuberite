
#pragma once

#include "ItemHandler.h"
#include "Blocks/BlockPiston.h"





class cItemPistonHandler final : public cItemHandler
{
	using Super = cItemHandler;

  public:
	using Super::Super;

  private:
	virtual bool CommitPlacement(
		cPlayer & a_Player,
		const cItem & a_HeldItem,
		const Vector3i a_PlacePosition,
		const eBlockFace a_ClickedBlockFace,
		const Vector3i a_CursorPosition
	) const override
	{
		return a_Player.PlaceBlock(
			a_PlacePosition,
			static_cast<BLOCKTYPE>(a_HeldItem.m_ItemType),
			cBlockPistonHandler::DisplacementYawToMetaData(
				a_PlacePosition,
				a_Player.GetEyePosition(),
				a_Player.GetYaw()
			)
		);
	}
};
