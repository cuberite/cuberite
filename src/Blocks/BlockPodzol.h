
#pragma once

class cBlockPodzolHandler final :
	public cBlockHandler
{
	using Super = cBlockHandler;

	using Super::Super;

	virtual cItems ConvertToPickups(const NIBBLETYPE a_BlockMeta, const cItem * const a_Tool) const override
	{
		if (a_Tool != nullptr)
		{
			if (ToolHasSilkTouch(a_Tool))
			{
				return cItem(E_BLOCK_DIRT, 1, E_META_DIRT_PODZOL);
			}
			return cItem(E_BLOCK_DIRT);
		}
		return cItem(E_BLOCK_DIRT);
	}
};
