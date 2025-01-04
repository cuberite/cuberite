
#pragma once

#include "BlockHandler.h"





class cBlockPlanksHandler final :
	public cBlockHandler
{
	using Super = cBlockHandler;

public:

	using Super::Super;

	static inline bool IsBlockPlanks(BlockState a_Block)
	{
		switch (a_Block.Type())
		{
			case BlockType::AcaciaPlanks:
			case BlockType::BirchPlanks:
			case BlockType::CrimsonPlanks:
			case BlockType::DarkOakPlanks:
			case BlockType::JunglePlanks:
			case BlockType::OakPlanks:
			case BlockType::SprucePlanks:
			case BlockType::WarpedPlanks:
			case BlockType::BambooPlanks:
			case BlockType::CherryPlanks:
			case BlockType::MangrovePlanks:
			case BlockType::PaleOakPlanks:
				return true;
			default: return false;
		}
	}

private:





	virtual ColourID GetMapBaseColourID() const override
	{
		switch (m_BlockType)
		{
			case BlockType::AcaciaPlanks:  return 15;
			case BlockType::BirchPlanks:   return 2;
			case BlockType::CrimsonPlanks: return 35;
			case BlockType::DarkOakPlanks: return 26;
			case BlockType::JunglePlanks:  return 10;
			case BlockType::OakPlanks:     return 13;
			case BlockType::SprucePlanks:  return 34;
			case BlockType::WarpedPlanks:  return 56;
			case BlockType::BambooPlanks:
			case BlockType::CherryPlanks:
			case BlockType::MangrovePlanks:
			case BlockType::PaleOakPlanks: return 0;
			default:
			{
				ASSERT(!"Unhandled BlockType in planks handler!");
				return 0;
			}
		}
	}
} ;




