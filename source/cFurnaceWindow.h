#pragma once

#include "cWindow.h"

class cFurnaceEntity;
class cWindowOwner;
class cFurnaceWindow : public cWindow
{
public:
	cFurnaceWindow( cFurnaceEntity* a_Owner );

	virtual void Clicked( cPacket_WindowClick* a_ClickPacket, cPlayer & a_Player );
	virtual void Close( cPlayer & a_Player );
private:
	cFurnaceEntity* m_Furnace;
};