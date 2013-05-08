
// Protocol15x.h

/*
Declares the 1.5.x protocol classes:
	- cProtocol150
		- release 1.5 and 1.5.1 protocol (#60)
		- release 1.5.2 protocol (#61; no relevant changes found)
*/





#pragma once

#include "Protocol14x.h"





class cProtocol150 :
	public cProtocol146
{
	typedef cProtocol146 super;
	
public:
	cProtocol150(cClientHandle * a_Client);
	
	virtual void SendWindowOpen(char a_WindowID, char a_WindowType, const AString & a_WindowTitle, char a_NumSlots) override;
	
	virtual int ParseWindowClick(void);
} ;




