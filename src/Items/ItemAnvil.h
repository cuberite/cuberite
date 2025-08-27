
#pragma once

#include "ItemHandler.h"
#include "Blocks/BlockAnvil.h"
#include "Registries/BlockItemConverter.h"
#include "Protocol/Palettes/Upgrade.h"




class cItemAnvilHandler final  :
	public cItemHandler
{
	using Super = cItemHandler;

public:

	using Super::Super;

	virtual bool CommitPlacement(cPlayer & a_Player, const cItem & a_HeldItem, const Vector3i a_PlacePosition, const eBlockFace a_ClickedBlockFace, const Vector3i a_CursorPosition) const override
	{
		BlockState to_place;
		switch (m_ItemType)
		{
			case Item::Anvil: to_place = Block::Anvil::Anvil(RotateBlockFaceCW(RotationToBlockFace(a_Player.GetYaw(), true))); break;
			case Item::ChippedAnvil: to_place = Block::ChippedAnvil::ChippedAnvil(RotateBlockFaceCW(RotationToBlockFace(a_Player.GetYaw(), true))); break;
			case Item::DamagedAnvil: to_place = Block::DamagedAnvil::DamagedAnvil(RotateBlockFaceCW(RotationToBlockFace(a_Player.GetYaw(), true))); break;
			default: UNREACHABLE("Invalid anvil type");
		}
		return a_Player.PlaceBlock(a_PlacePosition, to_place);
	}

	bool IsPlaceable() const override
	{
		return true;
	}
};
