
// Protocol132.h

// Interfaces to the cProtocol132 class representing the release 1.3.2 protocol (#39)





#pragma once

#include "Protocol125.h"





class cProtocol132 :
	public cProtocol125
{
	typedef cProtocol125 super;
public:

	cProtocol132(cClientHandle * a_Client);

	/// Called when client sends some data:
	virtual void DataReceived(const char * a_Data, int a_Size) override;

	// Modified packets:
	virtual int ParseHandshake(void) override;
} ;




