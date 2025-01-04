
#pragma once

#include "BlockHandler.h"

class cBlockLogHandler final:
	public cBlockHandler
{
	using Super = cBlockHandler;

public:
	using Super::Super;

	static inline bool IsBlockLog(BlockState a_Block)
	{
		switch (a_Block.Type())
		{
			case BlockType::AcaciaLog:
			case BlockType::BirchLog:
			case BlockType::DarkOakLog:
			case BlockType::JungleLog:
			case BlockType::OakLog:
			case BlockType::SpruceLog:
			case BlockType::CherryLog:
			case BlockType::MangroveLog:
			case BlockType::PaleOakLog:
				return true;
			default: return false;
		}
	}
};
