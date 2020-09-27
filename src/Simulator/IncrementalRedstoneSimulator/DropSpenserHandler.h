
#pragma once

#include "../../BlockEntities/DropSpenserEntity.h"





namespace DropSpenserHandler
{
	inline bool IsActivated(NIBBLETYPE a_Meta)
	{
		return (a_Meta & E_META_DROPSPENSER_ACTIVATED) != 0;
	}

	inline NIBBLETYPE SetActivationState(NIBBLETYPE a_Meta, bool IsOn)
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

	inline PowerLevel GetPowerDeliveredToPosition(const cChunk & a_Chunk, Vector3i a_Position, BLOCKTYPE a_BlockType, Vector3i a_QueryPosition, BLOCKTYPE a_QueryBlockType, bool IsLinked)
	{
		UNUSED(a_Chunk);
		UNUSED(a_Position);
		UNUSED(a_BlockType);
		UNUSED(a_QueryPosition);
		UNUSED(a_QueryBlockType);
		UNUSED(IsLinked);
		return 0;
	}

	inline void Update(cChunk & a_Chunk, cChunk &, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta, const PowerLevel Power)
	{
		// LOGD("Evaluating spencer the dropspenser (%d %d %d)", a_Position.x, a_Position.y, a_Position.z);

		const bool IsPoweredNow = (Power > 0);
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

	inline void ForValidSourcePositions(const cChunk & a_Chunk, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta, ForEachSourceCallback & Callback)
	{
		UNUSED(a_Chunk);
		UNUSED(a_BlockType);
		UNUSED(a_Meta);

		// Consider indirect power:
		Callback.CheckIndirectPower();

		// Consider normal adjacents:
		InvokeForAdjustedRelatives(Callback, a_Position, RelativeAdjacents);
	}
};
