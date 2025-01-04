
#pragma once

#include "SimplePlaceableItemHandler.h"
#include "Items/ItemHandler.h"
#include "Registries/BlockItemConverter.h"





class cItemButtonHandler final  :
	public cSimplePlaceableItemHandler
{
	using Super = cSimplePlaceableItemHandler;

public:

	using Super::Super;

private:

#define GETBLOCKFROMPLACEMENT(ButtonType) \
{\
	enum ButtonType::Face Facing;\
	switch (a_ClickedBlockFace)\
	{\
		case BLOCK_FACE_BOTTOM: Facing = ButtonType::Face::Ceiling; break;\
		case BLOCK_FACE_TOP:    Facing = ButtonType::Face::Floor; break;\
		case BLOCK_FACE_NORTH:\
		case BLOCK_FACE_SOUTH:\
		case BLOCK_FACE_WEST:\
		case BLOCK_FACE_EAST:   Facing = ButtonType::Face::Wall; break;   \
		default: return false;\
	}\
	return ButtonType::ButtonType(Facing, a_ClickedBlockFace, false);\
	break;\
}

	BlockState GetBlockFromPlacement(BlockType a_Type, eBlockFace a_ClickedBlockFace) const
	{
		using namespace Block;

		switch (a_Type)
		{
			case BlockType::AcaciaButton:             GETBLOCKFROMPLACEMENT(AcaciaButton)
			case BlockType::BirchButton:              GETBLOCKFROMPLACEMENT(BirchButton)
			case BlockType::CrimsonButton:            GETBLOCKFROMPLACEMENT(CrimsonButton)
			case BlockType::DarkOakButton:            GETBLOCKFROMPLACEMENT(DarkOakButton)
			case BlockType::JungleButton:             GETBLOCKFROMPLACEMENT(JungleButton)
			case BlockType::OakButton:                GETBLOCKFROMPLACEMENT(OakButton)
			case BlockType::PolishedBlackstoneButton: GETBLOCKFROMPLACEMENT(PolishedBlackstoneButton)
			case BlockType::SpruceButton:             GETBLOCKFROMPLACEMENT(SpruceButton)
			case BlockType::StoneButton:              GETBLOCKFROMPLACEMENT(StoneButton)
			case BlockType::WarpedButton:             GETBLOCKFROMPLACEMENT(WarpedButton)
			case BlockType::BambooButton:             GETBLOCKFROMPLACEMENT(BambooButton)
			case BlockType::CherryButton:             GETBLOCKFROMPLACEMENT(CherryButton)
			case BlockType::MangroveButton:           GETBLOCKFROMPLACEMENT(MangroveButton)
			case BlockType::PaleOakButton:            GETBLOCKFROMPLACEMENT(PaleOakButton)
			default: return false;
		}
	}

	virtual bool CommitPlacement(cPlayer & a_Player, const cItem & a_HeldItem, const Vector3i a_PlacePosition, const eBlockFace a_ClickedBlockFace, const Vector3i a_CursorPosition) const override
	{
		return a_Player.PlaceBlock(a_PlacePosition, GetBlockFromPlacement(BlockItemConverter::FromItem(a_HeldItem.m_ItemType), a_ClickedBlockFace));
	}
};
