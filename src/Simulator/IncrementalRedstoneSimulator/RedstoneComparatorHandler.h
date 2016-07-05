
#pragma once

#include "RedstoneHandler.h"
#include "Blocks/BlockComparator.h"





class cRedstoneComparatorHandler : public cRedstoneHandler
{
	typedef cRedstoneHandler super;
public:

	cRedstoneComparatorHandler(cWorld & a_World) :
		super(a_World)
	{
	}

	unsigned char GetFrontPowerLevel(const Vector3i & a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta, unsigned char a_HighestSidePowerLevel)
	{
		class cContainerCallback : public cBlockEntityCallback
		{
		public:
			cContainerCallback() : m_SignalStrength(0)
			{
			}

			virtual bool Item(cBlockEntity * a_BlockEntity) override
			{
				auto & Contents = static_cast<cBlockEntityWithItems *>(a_BlockEntity)->GetContents();
				float Fullness = 0;  // Is a floating-point type to allow later calculation to produce a non-truncated value
				for (int Slot = 0; Slot != Contents.GetNumSlots(); ++Slot)
				{
					Fullness += Contents.GetSlot(Slot).m_ItemCount / Contents.GetSlot(Slot).GetMaxStackSize();
				}

				m_SignalStrength = static_cast<unsigned char>(1 + (Fullness / Contents.GetNumSlots()) * 14);
				return false;
			}

			unsigned char m_SignalStrength;
		} CCB;

		auto RearCoordinate = cBlockComparatorHandler::GetRearCoordinate(a_Position, a_Meta & 0x3);
		m_World.DoWithBlockEntityAt(RearCoordinate.x, RearCoordinate.y, RearCoordinate.z, CCB);
		auto RearPower = CCB.m_SignalStrength;
		auto PotentialSourceHandler = cIncrementalRedstoneSimulator::CreateComponent(m_World, m_World.GetBlock(RearCoordinate), static_cast<cIncrementalRedstoneSimulator *>(m_World.GetRedstoneSimulator())->GetChunkData());
		if (PotentialSourceHandler != nullptr)
		{
			BLOCKTYPE Type;
			NIBBLETYPE Meta;
			if (m_World.GetBlockTypeMeta(RearCoordinate.x, RearCoordinate.y, RearCoordinate.z, Type, Meta))
			{
				RearPower = std::max(CCB.m_SignalStrength, PotentialSourceHandler->GetPowerDeliveredToPosition(RearCoordinate, Type, Meta, a_Position, a_BlockType));
			}
		}

		if ((a_Meta & 0x4) == 0x4)
		{
			// Subtraction mode
			return static_cast<unsigned char>(std::max(static_cast<char>(RearPower) - a_HighestSidePowerLevel, 0));
		}
		else
		{
			// Comparison mode
			return (std::max(a_HighestSidePowerLevel, RearPower) == a_HighestSidePowerLevel) ? 0 : RearPower;
		}
	}

	virtual unsigned char GetPowerDeliveredToPosition(const Vector3i & a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta, const Vector3i & a_QueryPosition, BLOCKTYPE a_QueryBlockType) override
	{
		UNUSED(a_QueryPosition);
		UNUSED(a_QueryBlockType);

		return (cBlockComparatorHandler::GetFrontCoordinate(a_Position, a_Meta & 0x3) == a_QueryPosition) ? static_cast<cIncrementalRedstoneSimulator *>(m_World.GetRedstoneSimulator())->GetChunkData()->GetCachedPowerData(a_Position).PowerLevel : 0;
	}

	virtual unsigned char GetPowerLevel(const Vector3i & a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta) override
	{
		UNUSED(a_Position);
		UNUSED(a_BlockType);

		auto RearCoordinate = cBlockComparatorHandler::GetRearCoordinate(a_Position, a_Meta & 0x3);
		auto PotentialSourceHandler = cIncrementalRedstoneSimulator::CreateComponent(m_World, m_World.GetBlock(RearCoordinate), static_cast<cIncrementalRedstoneSimulator *>(m_World.GetRedstoneSimulator())->GetChunkData());
		if (PotentialSourceHandler != nullptr)
		{
			BLOCKTYPE Type;
			NIBBLETYPE Meta;
			if (m_World.GetBlockTypeMeta(RearCoordinate.x, RearCoordinate.y, RearCoordinate.z, Type, Meta))
			{
				return PotentialSourceHandler->GetPowerDeliveredToPosition(RearCoordinate, Type, Meta, a_Position, a_BlockType);
			}
		}

		return 0;
	}

	virtual cVector3iArray Update(const Vector3i & a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta, PoweringData a_PoweringData) override
	{
		// LOGD("Evaluating ALU the comparator (%d %d %d)", a_Position.x, a_Position.y, a_Position.z);

		if (GetPowerLevel(a_Position, a_BlockType, a_Meta) > 0)
		{
			m_World.SetBlockMeta(a_Position, a_Meta | 0x8);
		}
		else
		{
			m_World.SetBlockMeta(a_Position, a_Meta & 0x7);
		}

		auto Power = GetFrontPowerLevel(a_Position, a_BlockType, a_Meta, a_PoweringData.PowerLevel);
		auto PreviousFrontPower = static_cast<cIncrementalRedstoneSimulator *>(m_World.GetRedstoneSimulator())->GetChunkData()->ExchangeUpdateOncePowerData(a_Position, PoweringData(a_PoweringData.PoweringBlock, Power));
		if (Power != PreviousFrontPower.PowerLevel)
		{
			return GetAdjustedRelatives(a_Position, GetRelativeLaterals());
		}

		return {};
	}

	virtual cVector3iArray GetValidSourcePositions(const Vector3i & a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta) override
	{
		UNUSED(a_BlockType);
		return cVector3iArray {cBlockComparatorHandler::GetSideCoordinate(a_Position, a_Meta & 0x3, false), cBlockComparatorHandler::GetSideCoordinate(a_Position, a_Meta & 0x3, true)};
	}
};
