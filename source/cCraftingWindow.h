#pragma once

#include "cWindow.h"

class cWindowOwner;
class cCraftingWindow : public cWindow
{
public:
	cCraftingWindow( cWindowOwner* a_Owner, bool a_bInventoryVisible );

	virtual void Clicked( cPacket_WindowClick* a_ClickPacket, cPlayer & a_Player );
	virtual void Close( cPlayer & a_Player );
};