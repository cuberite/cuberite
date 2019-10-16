
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





	virtual cItems ConvertToPickups(NIBBLETYPE a_BlockMeta, cBlockEntity * a_BlockEntity, const cEntity * a_Digger, const cItem * a_Tool) override
	{
		// Silk touch gives cobweb, anything else gives just string:
		if (ToolHasSilkTouch(a_Tool))
		{
			return cItem(m_BlockType, 1, 0);
		}
		else
		{
			return cItem(E_ITEM_STRING, 1, 0);
		}
	}





	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) override
	{
		UNUSED(a_Meta);
		return 3;
	}
} ;




