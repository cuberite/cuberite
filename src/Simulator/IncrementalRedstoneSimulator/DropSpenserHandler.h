
#pragma once

#include "RedstoneHandler.h"
#include "BlockEntities/DropSpenserEntity.h"





class cDropSpenserHandler : public cRedstoneHandler
{
	typedef cRedstoneHandler super;
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

	virtual unsigned char GetPowerDeliveredToPosition(cWorld & a_World, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta, Vector3i a_QueryPosition, BLOCKTYPE a_QueryBlockType) const override
	{
		UNUSED(a_World);
		UNUSED(a_Position);
		UNUSED(a_BlockType);
		UNUSED(a_Meta);
		UNUSED(a_QueryPosition);
		UNUSED(a_QueryBlockType);
		return 0;
	}

	virtual unsigned char GetPowerLevel(cWorld & a_World, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta) const override
	{
		UNUSED(a_World);
		UNUSED(a_Position);
		UNUSED(a_BlockType);
		UNUSED(a_Meta);
		return 0;
	}

	virtual cVector3iArray Update(cWorld & a_World, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta, PoweringData a_PoweringData) const override
	{
		// LOGD("Evaluating spencer the dropspenser (%d %d %d)", a_Position.x, a_Position.y, a_Position.z);
		bool IsPoweredNow = (a_PoweringData.PowerLevel > 0);
		bool WasPoweredPreviously = IsActivated(a_Meta);
		if (IsPoweredNow && !WasPoweredPreviously)
		{
			a_World.DoWithDropSpenserAt(a_Position.x, a_Position.y, a_Position.z, [](cDropSpenserEntity & a_DropSpenser)
				{
					a_DropSpenser.Activate();
					return false;
				}
			);
		}

		// Update the internal dropspenser state if necessary
		if (IsPoweredNow != WasPoweredPreviously)
		{
			a_World.SetBlockMeta(a_Position, SetActivationState(a_Meta, IsPoweredNow));
		}

		return {};
	}

	virtual cVector3iArray GetValidSourcePositions(cWorld & a_World, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta) const override
	{
		UNUSED(a_World);
		UNUSED(a_BlockType);
		UNUSED(a_Meta);

		return GetAdjustedRelatives(a_Position, GetRelativeAdjacents());
	}
};
