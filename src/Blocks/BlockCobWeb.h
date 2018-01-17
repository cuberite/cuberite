
// BlockCobWeb.h

// Declares the cBlockCobWebHandler object representing the BlockHandler for cobwebs

#pragma once





class cBlockCobWebHandler :
	public cBlockHandler
{
public:
	cBlockCobWebHandler(BLOCKTYPE a_BlockType)
		: cBlockHandler(a_BlockType)
	{
	}

	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_Meta) override
	{
		a_Pickups.push_back(cItem(E_ITEM_STRING, 1, 0));
	}

	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) override
	{
		UNUSED(a_Meta);
		return 3;
	}
} ;




