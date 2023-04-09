
#pragma once





class cBlockAirHandler final :
	public cBlockHandler
{
public:

	using cBlockHandler::cBlockHandler;

	static constexpr bool IsBlockAir(BlockState a_Block)
	{
		switch (a_Block.Type())
		{
			case BlockType::Air:
			case BlockType::CaveAir:
			case BlockType::VoidAir:
				return true;
			default: return false;
		}
	}

private:

	virtual cItems ConvertToPickups(const NIBBLETYPE a_BlockMeta, const cItem * const a_Tool) const override
	{
		// Don't drop anything:
		return {};
	}
};
