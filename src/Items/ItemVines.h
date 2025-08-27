
#pragma once

#include "Blocks/BlockVines.h"
#include "Items/ItemHandler.h"





class cItemVinesHandler final  :
	public cSimplePlaceableItemHandler
{
	using Super = cSimplePlaceableItemHandler;

public:

	using Super::Super;

private:

	virtual bool CommitPlacement(cPlayer & a_Player, const cItem & a_HeldItem, const Vector3i a_PlacePosition, const eBlockFace a_ClickedBlockFace, const Vector3i a_CursorPosition) const override
	{
		using namespace Block;
		// TODO: Disallow placement where the vine doesn't attach to something properly
		auto BlockToReplace = a_Player.GetWorld()->GetBlock(a_PlacePosition);
		if (BlockToReplace.Type() == BlockType::Vine)
		{
			if (cBlockVinesHandler::IsAttachedTo(BlockToReplace, ReverseBlockFace(a_ClickedBlockFace)))
			{
				// There is already a vine at that rotation
				return false;
			}
			switch (ReverseBlockFace(a_ClickedBlockFace))
			{
				//                                      East                         North                        South                        Up                        West
				case BLOCK_FACE_XM: BlockToReplace = Vine::Vine(Vine::East(BlockToReplace), Vine::North(BlockToReplace), Vine::South(BlockToReplace), Vine::Up(BlockToReplace), true);                       break;
				case BLOCK_FACE_XP: BlockToReplace = Vine::Vine(true,                       Vine::North(BlockToReplace), Vine::South(BlockToReplace), Vine::Up(BlockToReplace), Vine::West(BlockToReplace)); break;
				case BLOCK_FACE_YP: BlockToReplace = Vine::Vine(Vine::East(BlockToReplace), Vine::North(BlockToReplace), Vine::South(BlockToReplace), true,                     Vine::West(BlockToReplace)); break;
				case BLOCK_FACE_ZM: BlockToReplace = Vine::Vine(Vine::East(BlockToReplace), true,                        Vine::South(BlockToReplace), Vine::Up(BlockToReplace), Vine::West(BlockToReplace)); break;
				case BLOCK_FACE_ZP: BlockToReplace = Vine::Vine(Vine::East(BlockToReplace), Vine::North(BlockToReplace), true,                        Vine::Up(BlockToReplace), Vine::West(BlockToReplace)); break;
				default: return false;
			}
		}
		else
		{
			switch (ReverseBlockFace(a_ClickedBlockFace))
			{
				//                                              East        North       South        Up        West
				case BLOCK_FACE_XM: BlockToReplace = Vine::Vine(false, false, false, false, true); break;
				case BLOCK_FACE_XP: BlockToReplace = Vine::Vine(true,  false, false, false, false); break;
				case BLOCK_FACE_YP: BlockToReplace = Vine::Vine(false, false, false, true,  false); break;
				case BLOCK_FACE_ZM: BlockToReplace = Vine::Vine(false, true,  false, false, false); break;
				case BLOCK_FACE_ZP: BlockToReplace = Vine::Vine(false, false, true,  false, false); break;
				default: return false;
			}
		}
		return a_Player.PlaceBlock(a_PlacePosition, BlockToReplace);
	}
};
