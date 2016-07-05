
#pragma once

#include "RedstoneHandler.h"
#include "Blocks/BlockTripwireHook.h"





class cTripwireHookHandler : public cRedstoneHandler
{
	typedef cRedstoneHandler super;
public:

	cTripwireHookHandler(cWorld & a_World) :
		super(a_World)
	{
	}

	virtual unsigned char GetPowerDeliveredToPosition(const Vector3i & a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta, const Vector3i & a_QueryPosition, BLOCKTYPE a_QueryBlockType) override
	{
		UNUSED(a_QueryBlockType);
		UNUSED(a_QueryPosition);

		return (GetPowerLevel(a_Position, a_BlockType, a_Meta) == 15) ? 15 : 0;
	}

	virtual unsigned char GetPowerLevel(const Vector3i & a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta) override
	{
		UNUSED(a_BlockType);

		bool FoundActivated = false;
		auto Position = a_Position;
		eBlockFace FaceToGoTowards = cBlockTripwireHookHandler::MetadataToDirection(a_Meta);

		for (int i = 0; i < 40; ++i)  // Tripwires can be connected up to 40 blocks
		{
			BLOCKTYPE Type;
			NIBBLETYPE Meta;

			AddFaceDirection(Position.x, Position.y, Position.z, FaceToGoTowards);
			m_World.GetBlockTypeMeta(Position.x, Position.y, Position.z, Type, Meta);

			if (Type == E_BLOCK_TRIPWIRE)
			{
				class cTripwireCallback :
					public cEntityCallback
				{
				public:
					cTripwireCallback(void) :
						m_NumberOfEntities(0),
						m_FoundPlayer(false)
					{
					}

					virtual bool Item(cEntity * a_Entity) override
					{
						return true;
					}

					unsigned int m_NumberOfEntities;
					bool m_FoundPlayer;
				} TripwireCallback;

				if (!m_World.ForEachEntityInBox(cBoundingBox(Vector3d(0.5, 0, 0.5) + Position, 0.5, 0.5), TripwireCallback))
				{
					FoundActivated = true;
				}
			}
			else if (Type == E_BLOCK_TRIPWIRE_HOOK)
			{
				if (ReverseBlockFace(cBlockTripwireHookHandler::MetadataToDirection(Meta)) == FaceToGoTowards)
				{
					// Other hook facing in opposite direction - circuit completed!
					return FoundActivated ? 15 : 1;
				}
				else
				{
					// Tripwire hook not connected at all
					return 0;
				}
			}
			else
			{
				// Tripwire hook not connected at all
				return 0;
			}
		}

		return 0;
	}

	virtual cVector3iArray Update(const Vector3i & a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta, PoweringData a_PoweringData) override
	{
		// LOGD("Evaluating hooky the tripwire hook (%d %d %d)", a_Position.x, a_Position.y, a_Position.z);

		auto Power = GetPowerLevel(a_Position, a_BlockType, a_Meta);
		NIBBLETYPE Meta;
		if (Power == 0)
		{
			Meta = (a_Meta & 0x3);
		}
		else if (Power == 1)
		{
			// Connected but not activated, AND away the highest bit
			Meta = (a_Meta & 0x7) | 0x4;
		}
		else if (Power == 15)
		{
			// Connected and activated, set the 3rd and 4th highest bits
			Meta = (a_Meta | 0xC);
		}
		else
		{
			ASSERT(!"Unexpected tripwire hook power level!");
			return {};
		}

		if (Meta != a_Meta)
		{
			m_World.SetBlockMeta(a_Position, Meta);
			return GetAdjustedRelatives(a_Position, GetRelativeAdjacents());
		}

		return {};
	}

	virtual cVector3iArray GetValidSourcePositions(const Vector3i & a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta) override
	{
		UNUSED(a_BlockType);
		UNUSED(a_Meta);
		UNUSED(a_Position);
		return {};
	}
};
