
// ForgeHandshake.cpp

// Implements Forge protocol handshaking

#include "ForgeHandshake.h"

#include "Globals.h"
#include "json/json.h"
#include "../ClientHandle.h"

cForgeHandshake::cForgeHandshake(cClientHandle *client) : m_Client(client), m_isForgeClient(false), m_stage(UNKNOWN)
{
}





void cForgeHandshake::augmentServerListPing(Json::Value & ResponseValue)
{
	// modinfo:
	// TODO: only send if mods enabled
	Json::Value Modinfo;
	Modinfo["type"] = "FML";

	Json::Value ModList(Json::arrayValue);
	// TODO: customizable modList
	Modinfo["modList"] = ModList;

	// Augment the response:
	ResponseValue["modinfo"] = Modinfo;
}

void cForgeHandshake::setIsForgeClient(bool flag)
{
	LOG("Setting isForgeClient = %d of %p", flag, static_cast<void *>(this));
	m_isForgeClient = flag;
}


void cForgeHandshake::onLoginSuccess()
{
	if (m_isForgeClient) {
		AStringVector channels = { "FML|HS", "FML", "FML|MP", "FML", "FORGE" };
		AString channelsString;
		
		for (AStringVector::iterator itr = channels.begin(); itr != channels.end(); ++itr)
		{
			channelsString.append(*itr);
			channelsString.push_back('\0');
		}
		
		m_Client->SendPluginMessage("REGISTER", channelsString);
		//m_Client->RegisterPluginChannels(channels); // private and only adds to internal data structures, not sending messages
		
		m_stage = START;
		SendServerHello();
	}
}

void cForgeHandshake::SendServerHello()
{
	AString message;
	message.push_back(Discriminator_ServerHello); // Discriminator	Byte	Always 0 for ServerHello
	message.push_back('\2'); // FML protocol Version	Byte	Determined from NetworkRegistery. Currently 2.
	// Dimension TODO
	message.push_back('\0');
	message.push_back('\0');
	message.push_back('\0');
	message.push_back('\0');
	
	m_stage = HELLO;
	m_Client->SendPluginMessage("FML|HS", message);
}

void cForgeHandshake::DataReceived(const char * a_Data, size_t a_Size)
{
	if (!m_isForgeClient) {
		LOG("Received unexpected Forge data from non-Forge client (%zu bytes)", a_Size);
		return;
	}
	
	LOG("Received Forge data: %zu bytes: %s", a_Size, a_Data);
	
	if (a_Size <= 1) {
		LOG("Received unexpectedly short Forge data (%zu bytes)", a_Size);
		return;
	}
	
	int discriminator = a_Data[0];
	
	switch (m_stage)
	{
		case HELLO:
		{
			switch (discriminator)
			{
				case Discriminator_ClientHello:
				{
					if (a_Size == 2)
					{
						int fmlProtocolVersion = a_Data[1];
						LOG("Received ClientHello with FML protocol version %d", fmlProtocolVersion);
						if (fmlProtocolVersion != 2) {
							LOG("Unsupported FML client protocol version received in ClientHello: %d", fmlProtocolVersion);
							m_stage = ERROR;
						}
					}
					else
					{
						LOG("Unexpectedly short ClientHello received");
						m_stage = ERROR;
					}
					
					break;
				}
					
				case Discriminator_ModList:
				{
					LOG("Received ModList");
					// TODO: parse client ModList
					
					// TODO: send server ModList
					AString serverModList;
					/*
					serverModList.push_back(Discriminator_ModList);
					serverModList.push_back('\0'); // TODO: number of mods varint
					// TODO: array of strings of mod name and version
					// TODO: this should match ModList sent in status packet
					 */
					serverModList.assign(a_Data, a_Size); // just echoing back client's mods for now
					// TODO: fix
					
					m_Client->SendPluginMessage("FML|HS", serverModList);
					break;
				}
					
				default:
					LOG("Unexpected Forge packet %d received in %d stage", discriminator, m_stage);
					m_stage = ERROR;
					return;
			}
		}
			
		default:
		{
		}
	}
}
