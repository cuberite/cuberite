
// ForgeHandshake.cpp

// Implements Forge protocol handshaking

#include "ForgeHandshake.h"

#include "Globals.h"
#include "json/json.h"
#include "../ClientHandle.h"

cForgeHandshake::cForgeHandshake()
{
	LOG("Initializing new cForgeHandshake %p", static_cast<void *>(this));
	isForgeClient = false;
	stage = UNKNOWN;
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
	isForgeClient = flag;
}


void cForgeHandshake::onLoginSuccess(cClientHandle * Client)
{
	if (isForgeClient) {
		AStringVector channels = { "FML|HS", "FML", "FML|MP", "FML", "FORGE" };
		AString channelsString;
		
		for (AStringVector::iterator itr = channels.begin(); itr != channels.end(); ++itr)
		{
			channelsString.append(*itr);
			channelsString.push_back('\0');
		}
		
		Client->SendPluginMessage("REGISTER", channelsString);
		//m_Client->RegisterPluginChannels(channels); // private and only adds to internal data structures, not sending messages
		
		stage = START;
		SendServerHello(Client);
	}
}

void cForgeHandshake::SendServerHello(cClientHandle * Client)
{
	AString message;
	message.push_back(Discriminator_ServerHello); // Discriminator	Byte	Always 0 for ServerHello
	message.push_back('\2'); // FML protocol Version	Byte	Determined from NetworkRegistery. Currently 2.
	// Dimension TODO
	message.push_back('\0');
	message.push_back('\0');
	message.push_back('\0');
	message.push_back('\0');
	
	stage = HELLO;
	Client->SendPluginMessage("FML|HS", message);
}

void cForgeHandshake::DataReceived(const char * a_Data, size_t a_Size)
{
	/// XXX TODO: fix this, there are two cForgeHandshake instances, one in cProtocolRecognizer another in cProtocol_1_9 (both inherit cProtocol),
	// and the wrong one has the Forge client set, so it is not recognized here!
	if (!isForgeClient) {
		LOG("Received unexpected Forge data from non-Forge client (%zu bytes)", a_Size);
		return;
	}
	
	LOG("Received Forge data: %zu bytes: %s", a_Size, a_Data);
	
	if (a_Size <= 1) {
		LOG("Received unexpectedly short Forge data (%zu bytes)", a_Size);
		return;
	}
	
	int discriminator = a_Data[0];
	
	switch (stage)
	{
		case HELLO:
		{
			switch (discriminator)
			{
				case Discriminator_ClientHello:
				{
					LOG("Received ClientHello");
					if (a_Size == 2)
					{
						int fmlProtocolVersion = a_Data[1];
						LOG("Unsupported FML client protocol version received in ClientHello: %d", fmlProtocolVersion);
						stage = ERROR;
					}
					else
					{
						LOG("Unexpectedly short ClientHello received");
						stage = ERROR;
					}
					
					break;
				}
					
				case Discriminator_ModList:
					LOG("Received ModList");
					// TODO: parse client ModList
					// TODO: send server ModList
					break;
					
				default:
					LOG("Unexpected Forge packet %d received in %d stage", discriminator, stage);
					stage = ERROR;
					return;
			}
		}
			
		default:
		{
		}
	}
}
