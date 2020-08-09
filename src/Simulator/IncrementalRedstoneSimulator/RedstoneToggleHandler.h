
#pragma once

#include "RedstoneHandler.h"
#include "../../Blocks/BlockButton.h"
#include "../../Blocks/BlockLever.h"





class cRedstoneToggleHandler final : public cRedstoneHandler
{
	inline static Vector3i GetOffsetAttachedTo(Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta)
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

	virtual unsigned char GetPowerDeliveredToPosition(const cChunk & a_Chunk, Vector3i a_Position, BLOCKTYPE a_BlockType, Vector3i a_QueryPosition, BLOCKTYPE a_QueryBlockType, bool IsLinked) const override
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

	static unsigned char GetPowerLevel(BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta)
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

	virtual void Update(cChunk & a_Chunk, cChunk & CurrentlyTicking, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta, PoweringData a_PoweringData) const override
	{
		// LOGD("Evaluating templatio<> the lever/button (%d %d %d)", a_Position.x, a_Position.y, a_Position.z);
	}

	virtual void ForValidSourcePositions(const cChunk & a_Chunk, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta, SourceCallback Callback) const override
	{
		UNUSED(a_Chunk);
		UNUSED(a_Position);
		UNUSED(a_BlockType);
		UNUSED(a_Meta);
		UNUSED(Callback);
	}
};
