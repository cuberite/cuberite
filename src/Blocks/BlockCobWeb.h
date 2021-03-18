
// BlockCobWeb.h

// Declares the cBlockCobWebHandler object representing the BlockHandler for cobwebs

#pragma once





class cBlockCobWebHandler final :
	public cBlockHandler
{
public:

	using cBlockHandler::cBlockHandler;

private:

	virtual cItems ConvertToPickups(BlockState a_Block, const cEntity * a_Digger, const cItem * a_Tool) const override
	{
		// Silk touch gives cobweb, anything else gives just string:
		if (ToolHasSilkTouch(a_Tool))
		{
			return cItem(Item::Cobweb);
		}
		else
		{
			return cItem(Item::String);
		}
	}





	virtual ColourID GetMapBaseColourID() const override
	{
		UNUSED(a_Meta);
		return 3;
	}
} ;




