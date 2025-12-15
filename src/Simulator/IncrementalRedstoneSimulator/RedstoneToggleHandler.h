
#pragma once

#include "../../Blocks/BlockButton.h"
#include "../../Blocks/BlockLever.h"





namespace RedstoneToggleHandler
{
#define GETOFFSETATTACHEDTO(BlockType) \
{\
	switch (BlockType::Face(a_Block))\
	{\
		case BlockType::Face::Floor:   return { 0, 1, 0 };\
		case BlockType::Face::Ceiling: return { 0, -1, 0 };\
		case BlockType::Face::Wall:\
		{\
			switch (BlockType::Facing(a_Block))\
			{\
				case BLOCK_FACE_NORTH: return { -1, 0, 0 };\
				case BLOCK_FACE_EAST:  return { 1, 0, 0 };\
				case BLOCK_FACE_SOUTH: return { 0, 0, -1 };\
				case BLOCK_FACE_WEST:  return { 0, 0, 1 };\
				default: return { 0, 0, 0 };\
			}\
		}\
	}\
	break;\
}

	static Vector3i GetOffsetAttachedTo(BlockState a_Block)
	{
		using namespace Block;
		switch (a_Block.Type())
		{
			case BlockType::Lever:                    GETOFFSETATTACHEDTO(Lever)
			case BlockType::AcaciaButton:             GETOFFSETATTACHEDTO(AcaciaButton)
			case BlockType::BirchButton:              GETOFFSETATTACHEDTO(BirchButton)
			case BlockType::CrimsonButton:            GETOFFSETATTACHEDTO(CrimsonButton)
			case BlockType::DarkOakButton:            GETOFFSETATTACHEDTO(DarkOakButton)
			case BlockType::JungleButton:             GETOFFSETATTACHEDTO(JungleButton)
			case BlockType::OakButton:                GETOFFSETATTACHEDTO(OakButton)
			case BlockType::PolishedBlackstoneButton: GETOFFSETATTACHEDTO(PolishedBlackstoneButton)
			case BlockType::SpruceButton:             GETOFFSETATTACHEDTO(SpruceButton)
			case BlockType::StoneButton:              GETOFFSETATTACHEDTO(StoneButton)
			case BlockType::WarpedButton:             GETOFFSETATTACHEDTO(WarpedButton)
			default: break;
		}
		UNREACHABLE(!"Unexpected block passed to button/lever handler");
	}

	static unsigned char GetPowerLevel(BlockState a_Block)
	{
		switch (a_Block.Type())
		{
			case BlockType::Lever: return cBlockLeverHandler::IsLeverOn(a_Block) ? 15 : 0;
			case BlockType::AcaciaButton:
			case BlockType::BirchButton:
			case BlockType::CrimsonButton:
			case BlockType::DarkOakButton:
			case BlockType::JungleButton:
			case BlockType::OakButton:
			case BlockType::PolishedBlackstoneButton:
			case BlockType::SpruceButton:
			case BlockType::StoneButton:
			case BlockType::WarpedButton: return cBlockButtonHandler::IsButtonOn(a_Block) ? 15 : 0;
			default:
			{
				ASSERT(!"Unexpected block passed to button/lever handler");
				return 0;
			}
		}
	}

	static PowerLevel GetPowerDeliveredToPosition(const cChunk & a_Chunk, Vector3i a_Position, BlockState a_Block, Vector3i a_QueryPosition, BlockState a_QueryBlock, bool IsLinked)
	{
		UNUSED(a_QueryBlock);

		const auto QueryOffset = a_QueryPosition - a_Position;

		if (IsLinked && (QueryOffset != GetOffsetAttachedTo(a_Block)))
		{
			return 0;
		}

		return GetPowerLevel(a_Block);
	}

	static void Update(cChunk & a_Chunk, cChunk & CurrentlyTicking, Vector3i a_Position, BlockState a_Block, const PowerLevel Power)
	{
		LOGREDSTONE("Evaluating templatio<> the lever/button (%d %d %d)", a_Position.x, a_Position.y, a_Position.z);
	}

	static void ForValidSourcePositions(const cChunk & a_Chunk, Vector3i a_Position, BlockState a_Block, ForEachSourceCallback & Callback)
	{
		UNUSED(a_Chunk);
		UNUSED(a_Position);
		UNUSED(a_Block);
		UNUSED(Callback);
	}
};
