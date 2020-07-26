
#pragma once

#include "RedstoneHandler.h"
#include "../../BlockEntities/DropSpenserEntity.h"





class cDropSpenserHandler final : public cRedstoneHandler
{
public:

	inline static bool IsActivated(NIBBLETYPE a_Meta)
	{
		return (a_Meta & E_META_DROPSPENSER_ACTIVATED) != 0;
	}

	inline static NIBBLETYPE SetActivationState(NIBBLETYPE a_Meta, bool IsOn)
	{
		if (IsOn)
		{
			return a_Meta | E_META_DROPSPENSER_ACTIVATED;  // set the bit
		}
		else
		{
			return a_Meta & ~E_META_DROPSPENSER_ACTIVATED;  // clear the bit
		}
	}

	virtual unsigned char GetPowerDeliveredToPosition(cChunk & a_Chunk, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta, Vector3i a_QueryPosition, BLOCKTYPE a_QueryBlockType) const override
	{
		UNUSED(a_Chunk);
		UNUSED(a_Position);
		UNUSED(a_BlockType);
		UNUSED(a_Meta);
		UNUSED(a_QueryPosition);
		UNUSED(a_QueryBlockType);
		return 0;
	}

	virtual void Update(cChunk & a_Chunk, cChunk &, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta, PoweringData a_PoweringData) const override
	{
		// LOGD("Evaluating spencer the dropspenser (%d %d %d)", a_Position.x, a_Position.y, a_Position.z);

		const bool IsPoweredNow = (a_PoweringData.PowerLevel > 0);
		const bool WasPoweredPreviously = IsActivated(a_Meta);

		if (IsPoweredNow && !WasPoweredPreviously)
		{
			a_Chunk.DoWithDropSpenserAt(a_Position, [](cDropSpenserEntity & a_DropSpenser)
			{
				a_DropSpenser.Activate();
				return false;
			});
		}

		// Update the internal dropspenser state if necessary
		if (IsPoweredNow != WasPoweredPreviously)
		{
			a_Chunk.SetMeta(a_Position, SetActivationState(a_Meta, IsPoweredNow));
		}
	}

	virtual void ForValidSourcePositions(cChunk & a_Chunk, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta, SourceCallback Callback) const override
	{
		UNUSED(a_Chunk);
		UNUSED(a_BlockType);
		UNUSED(a_Meta);
		InvokeForAdjustedRelatives(Callback, a_Position, RelativeAdjacents);
	}
};
