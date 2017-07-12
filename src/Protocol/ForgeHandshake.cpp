
// ForgeHandshake.cpp

// Implements Forge protocol handshaking

#include "Globals.h"
#include "ForgeHandshake.h"
#include "json/json.h"
#include "../ClientHandle.h"

cForgeHandshake::cForgeHandshake(cClientHandle *client) : m_isForgeClient(false), m_Errored(false), m_Client(client)
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





void cForgeHandshake::BeginForgeHandshake(const AString & a_Name, const AString & a_UUID, const Json::Value & a_Properties)
{
	ASSERT(m_isForgeClient);
	
	m_Name = &a_Name;
	m_UUID = &a_UUID;
	m_Properties = &a_Properties;
	
	AStringVector channels = { "FML|HS", "FML", "FML|MP", "FML", "FORGE" };
	AString channelsString;
	
	for (AStringVector::iterator itr = channels.begin(); itr != channels.end(); ++itr)
	{
		channelsString.append(*itr);
		channelsString.push_back('\0');
	}
	
	m_Client->SendPluginMessage("REGISTER", channelsString);
	SendServerHello();
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
	
	m_Client->SendPluginMessage("FML|HS", message);
}

void cForgeHandshake::DataReceived(const char * a_Data, size_t a_Size)
{
	if (!m_isForgeClient) {
		LOG("Received unexpected Forge data from non-Forge client (%zu bytes)", a_Size);
		return;
	}
	
	// TODO: handle errors
	
	LOG("Received Forge data: %zu bytes: %s", a_Size, a_Data);
	
	if (a_Size <= 1) {
		LOG("Received unexpectedly short Forge data (%zu bytes)", a_Size);
		return;
	}
	
	int discriminator = a_Data[0];
	
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
					SetError();
				}
			}
			else
			{
				LOG("Unexpectedly short ClientHello received");
				SetError();
			}
			
			break;
		}
			
		case Discriminator_ModList:
		{
			LOG("Received ModList");
			// TODO: parse client ModList
			
			// Send server ModList
			
			cByteBuffer buf(1024); // TODO: max size?
			
			// TODO: allow plugins to register mods, for now, using based on what my test client sent
			struct {
				AString name;
				AString version;
			} mods[] = {
				{ "minecraft", "1.12" },
				{ "FML", "8.0.99.999.forge" },
				{ "forge", "14.21.1.2387.mcp-XXX" },
				{ "ironchest", "1.12-7.0.31.818" },
			};
			UInt32 modCount = sizeof(mods) / sizeof(mods[0]);
			
			buf.WriteBEInt8(Discriminator_ModList);
			buf.WriteVarInt32(modCount);
			for (size_t i = 0; i < modCount; ++i) {
				buf.WriteVarUTF8String(mods[i].name);
				buf.WriteVarUTF8String(mods[i].version);
			}
			AString serverModList;
			buf.ReadAll(serverModList);
			
			m_Client->SendPluginMessage("FML|HS", serverModList);
			break;
		}
			
		case Discriminator_HandshakeAck:
		{
			if (a_Size != 2)
			{
				LOG("Unexpected HandshakeAck packet length: %zu", a_Size);
				SetError();
				break;
			}
			
			int phase = a_Data[1];
			LOG("Received client HandshakeAck with phase=%d", phase);
			
			switch (phase)
			{
				case ClientPhase_WAITINGSERVERDATA:
				{
					cByteBuffer buf(1024);
					buf.WriteBEInt8(Discriminator_RegistryData);
					
					// TODO: send real registry data
					bool hasMore = false;
					AString registryName = "potions";
					UInt32 numIDs = 0;
					UInt32 numSubstitutions = 0;
					UInt32 numDummies = 0;
					
					buf.WriteBool(hasMore);
					buf.WriteVarUTF8String(registryName);
					buf.WriteVarInt32(numIDs);
					buf.WriteVarInt32(numSubstitutions);
					buf.WriteVarInt32(numDummies);
					
					AString registryData;
					buf.ReadAll(registryData);
					m_Client->SendPluginMessage("FML|HS", registryData);
					break;
				}
					
				case ClientPhase_WAITINGSERVERCOMPLETE:
				{
					LOG("Client finished receiving registry data; acknowledging");
					
					
					AString ack;
					ack.push_back(Discriminator_HandshakeAck);
					ack.push_back(ServerPhase_WAITINGCACK);
					m_Client->SendPluginMessage("FML|HS", ack);
					break;
				}
					
				case ClientPhase_PENDINGCOMPLETE:
				{
					LOG("Client is pending completion; sending complete ack");
					
					AString ack;
					ack.push_back(Discriminator_HandshakeAck);
					ack.push_back(ServerPhase_COMPLETE);
					m_Client->SendPluginMessage("FML|HS", ack);
					
					// Now finish logging in
					m_Client->FinishAuthenticate(*m_Name, *m_UUID, *m_Properties);
					break;
				}
			}
			break;
		}
			
		default:
			LOG("Unexpected Forge packet %d received", discriminator);
			SetError();
			return;
	}
}





void cForgeHandshake::SetError() {
	m_Errored = true;
}
