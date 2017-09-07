
#pragma once

#include "World.h"





class cRedstoneHandler
{
public:

	cRedstoneHandler() = default;
	DISALLOW_COPY_AND_ASSIGN(cRedstoneHandler);

	struct PoweringData
	{
	public:
		PoweringData(BLOCKTYPE a_PoweringBlock, unsigned char a_PowerLevel) :
			PoweringBlock(a_PoweringBlock),
			PowerLevel(a_PowerLevel)
		{
		}

		PoweringData(void) :
			PoweringBlock(E_BLOCK_AIR),
			PowerLevel(0)
		{
		}

		BLOCKTYPE PoweringBlock;
		unsigned char PowerLevel;

		inline friend bool operator < (const PoweringData & a_Lhs, const PoweringData & a_Rhs)
		{
			return (
				(a_Lhs.PowerLevel < a_Rhs.PowerLevel) ||
				(
					(a_Lhs.PowerLevel == a_Rhs.PowerLevel) &&
					((a_Lhs.PoweringBlock == E_BLOCK_REDSTONE_WIRE) && (a_Rhs.PoweringBlock != E_BLOCK_REDSTONE_WIRE))
				)
			);
		}

		inline friend bool operator == (const PoweringData & a_Lhs, const PoweringData & a_Rhs)
		{
			return (a_Lhs.PowerLevel == a_Rhs.PowerLevel);
		}

		inline friend bool operator != (const PoweringData & a_Lhs, const PoweringData & a_Rhs)
		{
			return !operator ==(a_Lhs, a_Rhs);
		}
	};

	virtual cVector3iArray Update(cWorld & a_World, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta, PoweringData a_PoweringData) const = 0;
	virtual unsigned char GetPowerDeliveredToPosition(cWorld & a_World, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta, Vector3i a_QueryPosition, BLOCKTYPE a_QueryBlockType) const = 0;
	virtual unsigned char GetPowerLevel(cWorld & a_World, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta) const = 0;
	virtual cVector3iArray GetValidSourcePositions(cWorld & a_World, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta) const = 0;

	// Force a virtual destructor
	virtual ~cRedstoneHandler() {}

protected:

	template <class Container>
	static Container StaticAppend(const Container & a_Lhs, const Container & a_Rhs)
	{
		Container ToReturn = a_Lhs;
		std::copy(a_Rhs.begin(), a_Rhs.end(), std::back_inserter(ToReturn));
		return ToReturn;
	}

	inline static Vector3i OffsetYP()
	{
		return Vector3i(0, 1, 0);
	}

	inline static Vector3i OffsetYM()
	{
		return Vector3i(0, -1, 0);
	}

	static cVector3iArray GetAdjustedRelatives(Vector3i a_Position, cVector3iArray a_Relatives)
	{
		for (auto & Entry : a_Relatives)
		{
			Entry += a_Position;
		}
		return a_Relatives;
	}

	inline static cVector3iArray GetRelativeAdjacents()
	{
		return
		{
			{
				{ 1, 0, 0 },
				{ -1, 0, 0 },
				{ 0, 1, 0 },
				{ 0, -1, 0 },
				{ 0, 0, 1 },
				{ 0, 0, -1 },
			}
		};
	}

	inline static cVector3iArray GetRelativeLaterals()
	{
		return
		{
			{
				{ 1, 0, 0 },
				{ -1, 0, 0 },
				{ 0, 0, 1 },
				{ 0, 0, -1 },
			}
		};
	}
};
