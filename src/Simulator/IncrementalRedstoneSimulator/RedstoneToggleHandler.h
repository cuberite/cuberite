
#pragma once

#include "../../Blocks/BlockButton.h"
#include "../../Blocks/BlockLever.h"





namespace RedstoneToggleHandler
{
	inline Vector3i GetOffsetAttachedTo(Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta)
	{
		switch (a_BlockType)
		{
			case E_BLOCK_LEVER:
			{
				switch (a_Meta & 0x7)
				{
					case 0x0:
					case 0x7: return { 0, 1, 0 };
					case 0x1: return { -1, 0, 0 };
					case 0x2: return { 1, 0, 0 };
					case 0x3: return { 0, 0, -1 };
					case 0x4: return { 0, 0, 1 };
					case 0x5:
					case 0x6: return { 0, -1, 0 };
					default:
					{
						ASSERT(!"Unhandled lever metadata!");
						return { 0, 0, 0 };
					}
				}
			}
			case E_BLOCK_STONE_BUTTON:
			case E_BLOCK_WOODEN_BUTTON:
			{
				switch (a_Meta & 0x7)
				{
					case 0x0: return { 0, 1, 0 };
					case 0x1: return { -1, 0, 0 };
					case 0x2: return { 1, 0, 0 };
					case 0x3: return { 0, 0, -1 };
					case 0x4: return { 0, 0, 1 };
					case 0x5: return { 0, -1, 0 };
					default:
					{
						ASSERT(!"Unhandled button metadata!");
						return { 0, 0, 0 };
					}
				}
			}
			default:
			{
				ASSERT(!"Unexpected block passed to button/lever handler");
				return { 0, 0, 0 };
			}
		}
	}

	inline unsigned char GetPowerLevel(BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta)
	{
		switch (a_BlockType)
		{
			case E_BLOCK_LEVER: return cBlockLeverHandler::IsLeverOn(a_Meta) ? 15 : 0;
			case E_BLOCK_STONE_BUTTON:
			case E_BLOCK_WOODEN_BUTTON: return cBlockButtonHandler::IsButtonOn(a_Meta) ? 15 : 0;
			default:
			{
				ASSERT(!"Unexpected block passed to button/lever handler");
				return 0;
			}
		}
	}

	inline PowerLevel GetPowerDeliveredToPosition(const cChunk & a_Chunk, Vector3i a_Position, BLOCKTYPE a_BlockType, Vector3i a_QueryPosition, BLOCKTYPE a_QueryBlockType, bool IsLinked)
	{
		UNUSED(a_QueryBlockType);

		const auto Meta = a_Chunk.GetMeta(a_Position);
		const auto QueryOffset = a_QueryPosition - a_Position;

		if (IsLinked && (QueryOffset != GetOffsetAttachedTo(a_Position, a_BlockType, Meta)))
		{
			return 0;
		}

		return GetPowerLevel(a_BlockType, Meta);
	}

	inline void Update(cChunk & a_Chunk, cChunk & CurrentlyTicking, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta, const PowerLevel Power)
	{
		// LOGD("Evaluating templatio<> the lever/button (%d %d %d)", a_Position.x, a_Position.y, a_Position.z);
	}

	inline void ForValidSourcePositions(const cChunk & a_Chunk, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta, ForEachSourceCallback & Callback)
	{
		UNUSED(a_Chunk);
		UNUSED(a_Position);
		UNUSED(a_BlockType);
		UNUSED(a_Meta);
		UNUSED(Callback);
	}
};
