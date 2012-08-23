#pragma once

#include "Item.h"
#include "../cWorld.h"

class cItemSignHandler : public cItemHandler
{
public:
	cItemSignHandler(int a_ItemID)
		: cItemHandler(a_ItemID)
	{

	}

	virtual bool IsPlaceable() override
	{
		return true;
	}

	virtual BLOCKTYPE GetBlockType() override
	{
		return E_BLOCK_SIGN_POST;
	}

};