
#pragma once

#include "BlockHandler.h"





class cBlockNetherrack final : public cBlockHandler
{
public:

	using cBlockHandler::cBlockHandler;

private:

	virtual bool CanSustainPlant(BlockState a_Plant) const override
	{
		return a_Plant.Type() == BlockType::NetherWart;
	}
};
