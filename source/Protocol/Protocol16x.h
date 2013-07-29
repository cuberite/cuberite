
// Protocol16x.h

/*
Declares the 1.6.x protocol classes:
	- cProtocol161
		- release 1.6.1 protocol (#73)
	- cProtocol162
		- release 1.6.2 protocol (#74)
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
	
protected:

	// cProtocol150 overrides:
	virtual void SendAttachEntity  (const cEntity & a_Entity, const cEntity * a_Vehicle) override;
	virtual void SendChat          (const AString & a_Message) override;
	virtual void SendEditSign      (int a_BlockX, int a_BlockY, int a_BlockZ) override;  ///< Request the client to open up the sign editor for the sign (1.6+)
	virtual void SendGameMode      (eGameMode a_GameMode) override;
	virtual void SendHealth        (void) override;
	virtual void SendPlayerMaxSpeed(void) override;
	virtual void SendRespawn       (void) override;
	virtual void SendWindowOpen    (char a_WindowID, char a_WindowType, const AString & a_WindowTitle, char a_NumSlots) override;
	
	virtual int ParseEntityAction   (void) override;
	virtual int ParsePlayerAbilities(void) override;
	
	// New packets:
	virtual int ParseSteerVehicle(void);

	// Enable new packets' handling
	virtual int ParsePacket(unsigned char a_PacketType) override;
} ;





class cProtocol162 :
	public cProtocol161
{
	typedef cProtocol161 super;
	
public:
	cProtocol162(cClientHandle * a_Client);

protected:
	// cProtocol161 overrides:
	virtual void SendPlayerMaxSpeed(void) override;
} ;




