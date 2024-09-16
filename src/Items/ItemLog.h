
#pragma once

class cItemLogHandler final :
	public cItemHandler
{
	using Super = cItemHandler;

public:
	using Super::Super;

	virtual bool IsPlaceable(void) const override
	{
		return true;
	}


	virtual bool CommitPlacement(cPlayer & a_Player, const cItem & a_HeldItem, const Vector3i a_PlacePosition, const eBlockFace a_ClickedBlockFace, const Vector3i a_CursorPosition) const override
	{
		BlockState BlockToPlace;
		switch (m_ItemType)
		{
			case Item::AcaciaLog:
			{
				auto Axis = Block::AcaciaLog::Axis::X;
				switch (a_ClickedBlockFace)
				{
					case BLOCK_FACE_XM:
					case BLOCK_FACE_XP:
					{
						Axis = Block::AcaciaLog::Axis::X;
						break;
					}
					case BLOCK_FACE_ZM:
					case BLOCK_FACE_ZP:
					{
						Axis = Block::AcaciaLog::Axis::Z;
						break;
					}
					case BLOCK_FACE_YM:
					case BLOCK_FACE_YP:
					{
						Axis = Block::AcaciaLog::Axis::Y;
						break;
					}
					default: UNREACHABLE("Unhandled facing");
				}
				BlockToPlace = Block::AcaciaLog::AcaciaLog(Axis);
				break;
			}
			case Item::BirchLog:
			{
				auto Axis = Block::BirchLog::Axis::X;
				switch (a_ClickedBlockFace)
				{
					case BLOCK_FACE_XM:
					case BLOCK_FACE_XP:
					{
						Axis = Block::BirchLog::Axis::X;
						break;
					}
					case BLOCK_FACE_ZM:
					case BLOCK_FACE_ZP:
					{
						Axis = Block::BirchLog::Axis::Z;
						break;
					}
					case BLOCK_FACE_YM:
					case BLOCK_FACE_YP:
					{
						Axis = Block::BirchLog::Axis::Y;
						break;
					}
					default: UNREACHABLE("Unhandled facing");
				}
				BlockToPlace = Block::BirchLog::BirchLog(Axis);
				break;
			}
			case Item::DarkOakLog:
			{
				auto Axis = Block::DarkOakLog::Axis::X;
				switch (a_ClickedBlockFace)
				{
					case BLOCK_FACE_XM:
					case BLOCK_FACE_XP:
					{
						Axis = Block::DarkOakLog::Axis::X;
						break;
					}
					case BLOCK_FACE_ZM:
					case BLOCK_FACE_ZP:
					{
						Axis = Block::DarkOakLog::Axis::Z;
						break;
					}
					case BLOCK_FACE_YM:
					case BLOCK_FACE_YP:
					{
						Axis = Block::DarkOakLog::Axis::Y;
						break;
					}
					default: UNREACHABLE("Unhandled facing");
				}
				BlockToPlace = Block::DarkOakLog::DarkOakLog(Axis);
				break;
			}
			case Item::JungleLog:
			{
				auto Axis = Block::JungleLog::Axis::X;
				switch (a_ClickedBlockFace)
				{
					case BLOCK_FACE_XM:
					case BLOCK_FACE_XP:
					{
						Axis = Block::JungleLog::Axis::X;
						break;
					}
					case BLOCK_FACE_ZM:
					case BLOCK_FACE_ZP:
					{
						Axis = Block::JungleLog::Axis::Z;
						break;
					}
					case BLOCK_FACE_YM:
					case BLOCK_FACE_YP:
					{
						Axis = Block::JungleLog::Axis::Y;
						break;
					}
					default: UNREACHABLE("Unhandled facing");
				}
				BlockToPlace = Block::JungleLog::JungleLog(Axis);
				break;
			}
			case Item::OakLog:
			{
				auto Axis = Block::OakLog::Axis::X;
				switch (a_ClickedBlockFace)
				{
					case BLOCK_FACE_XM:
					case BLOCK_FACE_XP:
					{
						Axis = Block::OakLog::Axis::X;
						break;
					}
					case BLOCK_FACE_ZM:
					case BLOCK_FACE_ZP:
					{
						Axis = Block::OakLog::Axis::Z;
						break;
					}
					case BLOCK_FACE_YM:
					case BLOCK_FACE_YP:
					{
						Axis = Block::OakLog::Axis::Y;
						break;
					}
					default: UNREACHABLE("Unhandled facing");
				}
				BlockToPlace = Block::OakLog::OakLog(Axis);
				break;
			}
			case Item::SpruceLog:
			{
				auto Axis = Block::SpruceLog::Axis::X;
				switch (a_ClickedBlockFace)
				{
					case BLOCK_FACE_XM:
					case BLOCK_FACE_XP:
					{
						Axis = Block::SpruceLog::Axis::X;
						break;
					}
					case BLOCK_FACE_ZM:
					case BLOCK_FACE_ZP:
					{
						Axis = Block::SpruceLog::Axis::Z;
						break;
					}
					case BLOCK_FACE_YM:
					case BLOCK_FACE_YP:
					{
						Axis = Block::SpruceLog::Axis::Y;
						break;
					}
					default: UNREACHABLE("Unhandled facing");
				}
				BlockToPlace = Block::SpruceLog::SpruceLog(Axis);
				break;
			}
			default:
			{
				FLOGWARNING("{}: Item type not handled {}.", __FUNCTION__, m_ItemType);
				UNREACHABLE("Unhandled log type");
			}
		}
		return a_Player.PlaceBlock(a_PlacePosition, BlockToPlace);
	}
};
