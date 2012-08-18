
#pragma once

#include "cInventory.h"





class cCreativeInventory
	: public cInventory
{
public:
	cCreativeInventory(cPlayer * a_Owner);
	~cCreativeInventory();

	virtual void Clicked(short a_SlotNum, bool a_IsRightClick, bool a_IsShiftPressed, const cItem & a_HeldItem) override;
} ;




