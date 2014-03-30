/**
 * Copyright 2014 MCServer Contributors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

// Protocol16x.h

/*
Declares the 1.6.x protocol classes:
	- cProtocol161
		- release 1.6.1 protocol (#73)
	- cProtocol162
		- release 1.6.2 protocol (#74)
		- release 1.6.3 protocol (#77) - no relevant changes
		- release 1.6.4 protocol (#78) - no relevant changes
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
	virtual void SendWindowOpen    (const cWindow & a_Window) override;
	
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




