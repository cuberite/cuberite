#pragma once

#include <stdint.h>
#include "Registries/BlockTypes.h"
#include "WorldStorage/NamespaceSerializer.h"

/** The UINT_LEAST16_MAX value is used to mark the value invalid if needed. */

struct BlockState
{
	using DataType = uint_least16_t;

	constexpr BlockState() : ID(0) {}

	constexpr BlockState(uint_least16_t StateID) :
		ID(StateID)
	{
	}

	BlockState(BlockType a_Type);

	/** Gets the block type of this block state. */
	BlockType Type() const;

	bool operator == (BlockState Block) const
	{
		return ID == Block.ID;
	}

	bool operator != (BlockState Block) const
	{
		return ID != Block.ID;
	}

	bool operator < (BlockState Block) const
	{
		return ID < Block.ID;
	}

	/** The state ID of the block state. */
	uint_least16_t ID;

	std::pair<unsigned char, unsigned char> GetLegacyValues();
};


template<> class fmt::formatter<BlockState> : public fmt::formatter<std::string_view>
{
public:
	template <typename FormatContext>
	auto format(const BlockState & a_Block, FormatContext & a_Ctx)
	{
		return fmt::format_to(a_Ctx.out(), "{}", NamespaceSerializer::From(a_Block.Type()));
	}
};

template<> class fmt::formatter<BlockType> : public fmt::formatter<std::string_view>
{
public:
	template <typename FormatContext>
	auto format(const BlockType & a_Block, FormatContext & a_Ctx)
	{
		return fmt::format_to(a_Ctx.out(), "{}", NamespaceSerializer::From(a_Block));
	}
};
