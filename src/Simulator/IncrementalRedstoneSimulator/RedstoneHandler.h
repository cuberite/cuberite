
#pragma once

#include "World.h"
#include "Vector3.h"





class cRedstoneHandler
{
public:

	cRedstoneHandler(cWorld & a_World) :
		m_World(a_World)
	{
	}

public:

	// Disable the copy constructor and assignment operator
	cRedstoneHandler(const cRedstoneHandler &) = delete;
	cRedstoneHandler & operator=(const cRedstoneHandler &) = delete;

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

	virtual cVector3iArray Update(const Vector3i & a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta, PoweringData a_PoweringData) = 0;
	virtual unsigned char GetPowerDeliveredToPosition(const Vector3i & a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta, const Vector3i & a_QueryPosition, BLOCKTYPE a_QueryBlockType) = 0;
	virtual unsigned char GetPowerLevel(const Vector3i & a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta) = 0;
	virtual cVector3iArray GetValidSourcePositions(const Vector3i & a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta) = 0;

	// Force a virtual destructor
	virtual ~cRedstoneHandler() {}

protected:

	cWorld & m_World;

	template <class Container>
	static const Container StaticAppend(const Container && a_Lhs, const Container && a_Rhs)
	{
		Container ToReturn = a_Lhs;
		std::copy(a_Rhs.begin(), a_Rhs.end(), std::back_inserter(ToReturn));
		return ToReturn;
	}

	inline static const Vector3i OffsetYP()
	{
		return Vector3i(0, 1, 0);
	}

	inline static const Vector3i OffsetYM()
	{
		return Vector3i(0, -1, 0);
	}

	static const cVector3iArray GetAdjustedRelatives(const Vector3i & a_Position, const cVector3iArray & a_Relatives)
	{
		cVector3iArray Adjusted = a_Relatives;
		std::for_each(Adjusted.begin(), Adjusted.end(), [a_Position](cVector3iArray::value_type & a_Entry) { a_Entry += a_Position; });
		return Adjusted;
	}

	inline static const cVector3iArray GetRelativeAdjacents()
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

	inline static const cVector3iArray GetRelativeLaterals()
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
