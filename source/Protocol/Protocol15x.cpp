
// Protocol15x.cpp

/*
Implements the 1.5.x protocol classes:
	- cProtocol150
		- release 1.5 protocol (#60)
(others may be added later in the future for the 1.5 release series)
*/

#include "Globals.h"
#include "Protocol15x.h"





enum
{
	PACKET_WINDOW_OPEN = 0x64,
} ;






///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cProtocol150:

cProtocol150::cProtocol150(cClientHandle * a_Client) :
	super(a_Client)
{
}





void cProtocol150::SendWindowOpen(char a_WindowID, char a_WindowType, const AString & a_WindowTitle, char a_NumSlots)
{
	if (a_WindowType < 0)
	{
		// Do not send for inventory windows
		return;
	}
	cCSLock Lock(m_CSPacket);
	WriteByte  (PACKET_WINDOW_OPEN);
	WriteByte  (a_WindowID);
	WriteByte  (a_WindowType);
	WriteString(a_WindowTitle);
	WriteByte  (a_NumSlots);
	WriteByte  (1);  // Use title
	Flush();
}




