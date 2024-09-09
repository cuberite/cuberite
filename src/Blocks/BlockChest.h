
#pragma once

#include "../BlockArea.h"
#include "../Entities/Player.h"
#include "Mixins.h"





class cBlockChestHandler final :
	public cYawRotator<cClearMetaOnDrop<cBlockEntityHandler>, 0x07, 0x03, 0x04, 0x02, 0x05>
{
	using Super = cYawRotator<cClearMetaOnDrop<cBlockEntityHandler>, 0x07, 0x03, 0x04, 0x02, 0x05>;

public:

	using Super::Super;

	/** This enum is used to unify the type of both chests for better readability. */
	enum class eChestType
	{
		Left,
		Single,
		Right
	};

	static inline bool IsChest(BlockState a_Block)
	{
		switch (a_Block.Type())
		{
			case BlockType::Chest:
			case BlockType::TrappedChest:
				return true;
			default:
			{
				return false;
			}
		}
	}

	/** Sets the orientation of the chest. */
	static inline BlockState SetChestOrientation(BlockState a_Block, eChestType a_Orientation)
	{
		using namespace Block;
		switch (a_Block.Type())
		{
			case BlockType::Chest:        return Chest::Chest              (Chest::Facing(a_Block),        a_Orientation == eChestType::Left ? Chest::Type::Left :        a_Orientation == eChestType::Right ? Chest::Type::Right        : Chest::Type::Single);
			case BlockType::TrappedChest: return TrappedChest::TrappedChest(TrappedChest::Facing(a_Block), a_Orientation == eChestType::Left ? TrappedChest::Type::Left : a_Orientation == eChestType::Right ? TrappedChest::Type::Right : TrappedChest::Type::Single);
			default: return a_Block;
		}
	}

	static inline eChestType GetChestComposition(BlockState a_Block)
	{
		using namespace Block;
		switch (a_Block.Type())
		{
			case BlockType::Chest:
			{
				switch (Chest::Type(a_Block))
				{
					case Chest::Type::Single: return eChestType::Single;
					case Chest::Type::Left:   return eChestType::Left;
					case Chest::Type::Right:  return eChestType::Right;
				}
			}
			case BlockType::TrappedChest:
			{
				switch (TrappedChest::Type(a_Block))
				{
					case TrappedChest::Type::Single: return eChestType::Single;
					case TrappedChest::Type::Left:   return eChestType::Left;
					case TrappedChest::Type::Right:  return eChestType::Right;
				}
			}
			default: return eChestType::Single;
		}
	}

	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) const override
	{
		UNUSED(a_Meta);
		return 13;
	}
} ;
