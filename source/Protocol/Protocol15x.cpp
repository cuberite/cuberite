
// Protocol15x.cpp

/*
Implements the 1.5.x protocol classes:
	- cProtocol150
		- release 1.5 protocol (#60)
		- release 1.5.2 protocol (#61, no relevant changes found)
*/

#include "Globals.h"
#include "Protocol15x.h"
#include "../ClientHandle.h"
#include "../Item.h"





#define HANDLE_PACKET_READ(Proc, Type, Var) \
	Type Var; \
	{ \
		if (!m_ReceivedData.Proc(Var)) \
		{ \
			m_ReceivedData.CheckValid(); \
			return PARSE_INCOMPLETE; \
		} \
		m_ReceivedData.CheckValid(); \
	}





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





int cProtocol150::ParseWindowClick(void)
{
	HANDLE_PACKET_READ(ReadChar,    char,  WindowID);
	HANDLE_PACKET_READ(ReadBEShort, short, SlotNum);
	HANDLE_PACKET_READ(ReadByte,    Byte,  Button);
	HANDLE_PACKET_READ(ReadBEShort, short, TransactionID);
	HANDLE_PACKET_READ(ReadByte,    Byte,  Mode);
	cItem HeldItem;
	int res = ParseItem(HeldItem);
	if (res < 0)
	{
		return res;
	}
	
	// Convert Button, Mode, SlotNum and HeldItem into eClickAction:
	eClickAction Action;
	switch ((Mode << 8) | Button)
	{
		case 0x0000: Action = (SlotNum != -999) ? caLeftClick  : caLeftClickOutside;  break;
		case 0x0001: Action = (SlotNum != -999) ? caRightClick : caRightClickOutside; break;
		case 0x0100: Action = caShiftLeftClick;  break;
		case 0x0101: Action = caShiftRightClick; break;
		case 0x0200: Action = caNumber1;         break;
		case 0x0201: Action = caNumber2;         break;
		case 0x0202: Action = caNumber3;         break;
		case 0x0203: Action = caNumber4;         break;
		case 0x0204: Action = caNumber5;         break;
		case 0x0205: Action = caNumber6;         break;
		case 0x0206: Action = caNumber7;         break;
		case 0x0207: Action = caNumber8;         break;
		case 0x0208: Action = caNumber9;         break;
		case 0x0300: Action = caMiddleClick;     break;
		case 0x0400: Action = (SlotNum == -999) ? caLeftClickOutsideHoldNothing  : caDropKey;           break;
		case 0x0401: Action = (SlotNum == -999) ? caRightClickOutsideHoldNothing : caCtrlDropKey;       break;
		case 0x0500: Action = (SlotNum == -999) ? caLeftPaintBegin               : caUnknown;           break;
		case 0x0501: Action = (SlotNum != -999) ? caLeftPaintProgress            : caUnknown;           break;
		case 0x0502: Action = (SlotNum == -999) ? caLeftPaintEnd                 : caUnknown;           break;
		case 0x0504: Action = (SlotNum == -999) ? caRightPaintBegin              : caUnknown;           break;
		case 0x0505: Action = (SlotNum != -999) ? caRightPaintProgress           : caUnknown;           break;
		case 0x0506: Action = (SlotNum == -999) ? caRightPaintEnd                : caUnknown;           break;
		case 0x0600: Action = caDblClick; break;
	}
	
	if (Action == caUnknown)
	{
		LOGWARNING("Received an unknown click action combination: Mode = %d, Button = %d, Slot = %d, HeldItem = %s. Ignoring packet.",
			Mode, Button, SlotNum, ItemToFullString(HeldItem).c_str()
		);
		ASSERT(!"Unknown click action");
		return PARSE_OK;
	}
	
	m_Client->HandleWindowClick(WindowID, SlotNum, Action, HeldItem);
	return PARSE_OK;
}





