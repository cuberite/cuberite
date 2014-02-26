
#pragma once

#include "BlockRedstone.h"
#include "BlockTorch.h"





class cBlockRedstoneTorchHandler :
	public cBlockTorchHandler
{
public:
	cBlockRedstoneTorchHandler(BLOCKTYPE a_BlockType)
		: cBlockTorchHandler(a_BlockType)
	{
	}


	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		// Always drop the ON torch, meta 0
		a_Pickups.push_back(cItem(E_BLOCK_REDSTONE_TORCH_ON, 1, 0));
	}


	virtual const char * GetStepSound(void) override
	{
		return "step.wood";
	}


	virtual NIBBLETYPE MetaRotateCCW(NIBBLETYPE a_Meta) override
	{
		// Bit 4 stays, the rest is swapped around according to a table:
		NIBBLETYPE TopBits = (a_Meta & 0x08);
		switch (a_Meta & 0x07)
		{
			case 0x01: return TopBits | 0x04;  // East  -> North
			case 0x02: return TopBits | 0x03;  // West  -> South
			case 0x03: return TopBits | 0x01;  // South -> East
			case 0x04: return TopBits | 0x02;  // North -> West
			default:   return a_Meta;          // Floor -> Floor
		}
	}


	virtual NIBBLETYPE MetaRotateCW(NIBBLETYPE a_Meta) override
	{
		// Bit 4 stays, the rest is swapped around according to a table:
		NIBBLETYPE TopBits = (a_Meta & 0x08);
		switch (a_Meta & 0x07)
		{
			case 0x01: return TopBits | 0x03;  // East  -> South
			case 0x02: return TopBits | 0x04;  // West  -> North
			case 0x03: return TopBits | 0x02;  // South -> West
			case 0x04: return TopBits | 0x01;  // North -> East
			default:   return a_Meta;          // Floor -> Floor
		}
	}


	virtual NIBBLETYPE MetaMirrorXY(NIBBLETYPE a_Meta) override
	{
		// Bit 4 stays, the rest is swapped around according to a table:
		NIBBLETYPE TopBits = (a_Meta & 0x08);
		switch (a_Meta & 0x07)
		{
			case 0x03: return TopBits | 0x04;  // South -> North
			case 0x04: return TopBits | 0x03;  // North -> South
			default:   return a_Meta;          // Keep the rest
		}
	}


	virtual NIBBLETYPE MetaMirrorYZ(NIBBLETYPE a_Meta) override
	{
		// Bit 4 stays, the rest is swapped around according to a table:
		NIBBLETYPE TopBits = (a_Meta & 0x08);
		switch (a_Meta & 0x07)
		{
			case 0x01: return TopBits | 0x02;  // East -> West
			case 0x02: return TopBits | 0x01;  // West -> East
			default:   return a_Meta;          // Keep the rest
		}
	}
} ;




