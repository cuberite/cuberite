#pragma once
#include "BlockHandler.h"
#include "BlockEntity.h"

class cBlockNoteHandler :  public cBlockEntityHandler
{
public:
	cBlockNoteHandler(BLOCKTYPE a_BlockID)
		: cBlockEntityHandler(a_BlockID)
	{
	}

};
