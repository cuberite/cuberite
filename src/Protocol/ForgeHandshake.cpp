
// ForgeHandshake.cpp

// Implements Forge protocol handshaking

#include "ForgeHandshake.h"

#include "Globals.h"
#include "json/json.h"
#include "../ClientHandle.h"

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
		
		SendServerHello(Client);
	}
}

void cForgeHandshake::SendServerHello(cClientHandle * Client)
{
	AString message;
	message.push_back('\0'); // Discriminator	Byte	Always 0 for ServerHello
	message.push_back('\2'); // FML protocol Version	Byte	Determined from NetworkRegistery. Currently 2.
	// Dimension TODO
	message.push_back('\0');
	message.push_back('\0');
	message.push_back('\0');
	message.push_back('\0');
	
	
	Client->SendPluginMessage("FML|HS", message);
}
