
// BlockCobWeb.h

// Declares the cBlockCobWebHandler object representing the BlockHandler for cobwebs

#pragma once





class cBlockCobWebHandler :
	public cClearMetaOnDrop<cBlockHandler>
{
public:
	cBlockCobWebHandler(BLOCKTYPE a_BlockType)
		: cClearMetaOnDrop<cBlockHandler>(a_BlockType)
	{
	}
} ;




