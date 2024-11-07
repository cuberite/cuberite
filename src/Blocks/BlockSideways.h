
#pragma once

#include "BlockHandler.h"





/** Handler for blocks that have 3 orientations (hay bale, log), specified by the upper 2 bits in meta.
Additionally supports the metadata specifying block sub-type in its lower 2 bits. */
class cBlockSidewaysHandler final :
	public cBlockHandler
{
	using Super = cBlockHandler;

public:

	using Super::Super;

private:

	virtual cItems ConvertToPickups(const NIBBLETYPE a_BlockMeta, const cItem * const a_Tool) const override
	{
		// Reset the orientation part of meta, keep the sub-type part of meta:
		return cItem(m_BlockType, 1, a_BlockMeta & 0x03);
	}
} ;
