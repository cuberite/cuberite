
#pragma once

#include "BlockHandler.h"

class BlockLogHandler final:
	public BlockHandler
{
	using Super = BlockHandler;

	using Super::Super;

	static constexpr IsBlockLog(BlockState a_Block)
	{
		switch (a_Block.Type())
		{
			case BlockType::AcaciaLog:
			case BlockType::BirchLog:
			case BlockType::DarkOakLog:
			case BlockType::JungleLog:
			case BlockType::OakLog:
			case BlockType::SpruceLog:
		}
	}
};
