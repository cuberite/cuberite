
#pragma once

#include "RedstoneHandler.h"
#include "BlockEntities/DropSpenserEntity.h"





class cDropSpenserHandler : public cRedstoneHandler
{
	typedef cRedstoneHandler super;
public:

	cDropSpenserHandler(cWorld & a_World) :
		super(a_World)
	{
	}

	virtual unsigned char GetPowerDeliveredToPosition(const Vector3i & a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta, const Vector3i & a_QueryPosition, BLOCKTYPE a_QueryBlockType) override
	{
		UNUSED(a_Position);
		UNUSED(a_BlockType);
		UNUSED(a_Meta);
		UNUSED(a_QueryPosition);
		UNUSED(a_QueryBlockType);
		return 0;
	}

	virtual unsigned char GetPowerLevel(const Vector3i & a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta) override
	{
		UNUSED(a_Position);
		UNUSED(a_BlockType);
		UNUSED(a_Meta);
		return 0;
	}

	virtual cVector3iArray Update(const Vector3i & a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta, PoweringData a_PoweringData) override
	{
		LOGD("Evaluating spencer the dropspenser (%d %d %d)", a_Position.x, a_Position.y, a_Position.z);

		if (a_PoweringData.PowerLevel > 0)
		{
			class cSetPowerToDropSpenser :
				public cDropSpenserCallback
			{
			public:
				virtual bool Item(cDropSpenserEntity * a_DropSpenser) override
				{
					a_DropSpenser->Activate();
					return false;
				}
			} DrSpSP;

			m_World.DoWithDropSpenserAt(a_Position.x, a_Position.y, a_Position.z, DrSpSP);
		}

		return {};
	}

	virtual cVector3iArray GetValidSourcePositions(const Vector3i & a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta) override
	{
		UNUSED(a_BlockType);
		UNUSED(a_Meta);

		return GetAdjustedRelatives(a_Position, GetRelativeAdjacents());
	}
};
