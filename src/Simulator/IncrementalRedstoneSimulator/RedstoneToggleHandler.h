
#pragma once

#include "RedstoneHandler.h"
#include "../../Blocks/BlockButton.h"
#include "../../Blocks/BlockLever.h"





class cRedstoneToggleHandler:
	public cRedstoneHandler
{
	using Super = cRedstoneHandler;

public:

	inline static Vector3i GetPositionAttachedTo(Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta)
	{
		switch (a_BlockType)
		{
			case E_BLOCK_LEVER:
			{
				switch (a_Meta & 0x7)
				{
					case 0x0:
					case 0x7: return { a_Position + Vector3i(0, 1, 0) };
					case 0x1: return { a_Position + Vector3i(-1, 0, 0) };
					case 0x2: return { a_Position + Vector3i(1, 0, 0) };
					case 0x3: return { a_Position + Vector3i(0, 0, -1) };
					case 0x4: return { a_Position + Vector3i(0, 0, 1) };
					case 0x5:
					case 0x6: return { a_Position + Vector3i(0, -1, 0) };
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
					case 0x0: return { a_Position + Vector3i(0, 1, 0) };
					case 0x1: return { a_Position + Vector3i(-1, 0, 0) };
					case 0x2: return { a_Position + Vector3i(1, 0, 0) };
					case 0x3: return { a_Position + Vector3i(0, 0, -1) };
					case 0x4: return { a_Position + Vector3i(0, 0, 1) };
					case 0x5: return { a_Position + Vector3i(0, -1, 0) };
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

	virtual unsigned char GetPowerDeliveredToPosition(cWorld & a_World, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta, Vector3i a_QueryPosition, BLOCKTYPE a_QueryBlockType) const override
	{
		UNUSED(a_QueryBlockType);
		if ((GetPositionAttachedTo(a_Position, a_BlockType, a_Meta) == a_QueryPosition) || cIncrementalRedstoneSimulator::IsMechanism(a_QueryBlockType))
		{
			return GetPowerLevel(a_World, a_Position, a_BlockType, a_Meta);
		}
		return 0;
	}

	virtual unsigned char GetPowerLevel(cWorld & a_World, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta) const override
	{
		switch (a_BlockType)
		{
			case E_BLOCK_LEVER: return cBlockLeverHandler::IsLeverOn(a_Meta) ? 15 : 0;
			case E_BLOCK_STONE_BUTTON: return cBlockButtonHandler::IsButtonOn(a_Meta) ? 15 : 0;
			case E_BLOCK_WOODEN_BUTTON: return cBlockButtonHandler::IsButtonOn(a_World, a_Position, a_Meta) ? 15 : 0;
			default:
			{
				ASSERT(!"Unexpected block passed to button/lever handler");
				return 0;
			}
		}
	}

	virtual cVector3iArray Update(cWorld & a_World, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta, PoweringData a_PoweringData) const override
	{
		if ((a_BlockType == E_BLOCK_STONE_BUTTON) || (a_BlockType == E_BLOCK_WOODEN_BUTTON))
		{
			auto ChunkData = static_cast<cIncrementalRedstoneSimulator *>(a_World.GetRedstoneSimulator())->GetChunkData();
			auto DelayInfo = ChunkData->GetMechanismDelayInfo(a_Position);

			// Is button not scheduled for release?
			if (DelayInfo == nullptr)
			{
				// ...but pressed anyway?
				if (a_Meta & 0x08)
				{
					// Yes.
					a_World.SetBlockMeta(a_Position, a_Meta & 0x07, false);
					a_World.WakeUpSimulators(a_Position);
					return GetAdjustedRelatives(a_Position, StaticAppend(GetRelativeLaterals(), cVector3iArray{ OffsetYM() }));
				}
				return {};
			}

			// We are still waiting for the delay
			if (DelayInfo->first > 0)
			{
				return {};
			}

			// Check for arrows - delay resetting is done in "HasArrowInIt"-Function
			if ((a_BlockType == E_BLOCK_WOODEN_BUTTON) && (cBlockButtonHandler::HasArrowInIt(a_World, a_Position, a_Meta)))
			{
				return {};
			}

			// Release button process
			a_World.SetBlockMeta(a_Position, a_World.GetBlockMeta(a_Position) & 0x07, false);
			a_World.WakeUpSimulators(a_Position);
			a_World.BroadcastSoundEffect("block.stone_button.click_off", a_Position, 0.5f, 0.5f);

			ChunkData->m_MechanismDelays.erase(a_Position);
			return GetAdjustedRelatives(a_Position, StaticAppend(GetRelativeLaterals(), cVector3iArray{ OffsetYM() }));
		}
		return {};
	}

	virtual cVector3iArray GetValidSourcePositions(cWorld & a_World, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta) const override
	{
		UNUSED(a_World);
		UNUSED(a_Position);
		UNUSED(a_BlockType);
		UNUSED(a_Meta);
		return {};
	}
};
