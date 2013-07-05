
// Protocol16x.h

/*
Declares the 1.6.x protocol classes:
	- cProtocol161
		- release 1.6.1 protocol (#73)
(others may be added later in the future for the 1.6 release series)
*/





#pragma once

#include "Protocol15x.h"





class cProtocol161 :
	public cProtocol150
{
	typedef cProtocol150 super;
	
public:
	cProtocol161(cClientHandle * a_Client);
	
	// cProtocol150 overrides:
	virtual void SendAttachEntity(const cEntity & a_Entity, const cEntity * a_Vehicle) override;
	virtual void SendChat        (const AString & a_Message) override;
	virtual void SendGameMode    (eGameMode a_GameMode) override;
	virtual void SendHealth      (void) override;
	virtual void SendWindowOpen  (char a_WindowID, char a_WindowType, const AString & a_WindowTitle, char a_NumSlots) override;
	
	virtual int ParseEntityAction   (void) override;
	virtual int ParsePlayerAbilities(void) override;
	
	// New packets:
	virtual int ParseSteerVehicle(void);

	// Enable new packets' handling
	virtual int ParsePacket(unsigned char a_PacketType) override;
} ;




