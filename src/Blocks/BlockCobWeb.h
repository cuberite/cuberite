
// BlockCobWeb.h

// Declares the cBlockCobWebHandler object representing the BlockHandler for cobwebs

#pragma once





class cBlockCobWebHandler final :
	public cBlockHandler
{
public:

	using cBlockHandler::cBlockHandler;

private:

	virtual cItems ConvertToPickups(NIBBLETYPE a_BlockMeta, const cEntity * a_Digger, const cItem * a_Tool) const override
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





	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) const override
	{
		UNUSED(a_Meta);
		return 3;
	}
} ;




