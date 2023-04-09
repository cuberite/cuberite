
#pragma once

class cBlockHayBaleHandler final :
	public cBlockHandler
{
	using Super = cBlockHandler;

	// Todo: Mark as override once BlockState are implemented
	BlockState RotateCCW(BlockState a_Block) const
	{
		using namespace Block;
		auto Axis = Block::HayBale::Axis(a_Block);
		switch (Axis)
		{
			case HayBale::Axis::X: return HayBale::HayBale(HayBale::Axis::Z);
			case HayBale::Axis::Z: return HayBale::HayBale(HayBale::Axis::X);
			default: return a_Block;
		}
	}


	BlockState RotateCW(BlockState a_Block) const
	{
		using namespace Block;
		auto Axis = Block::HayBale::Axis(a_Block);
		switch (Axis)
		{
			case HayBale::Axis::X: return HayBale::HayBale(HayBale::Axis::Z);
			case HayBale::Axis::Z: return HayBale::HayBale(HayBale::Axis::X);
			default: return a_Block;
		}
	}
};
