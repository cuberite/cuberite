
// ForgeHandshake.cpp

// Implements Forge protocol handshaking

#include "Globals.h"
#include "ForgeHandshake.h"
#include "json/json.h"
#include "../Server.h"
#include "../Bindings/PluginManager.h"
#include "../ClientHandle.h"
#include "../Root.h"

cForgeHandshake::cForgeHandshake(cClientHandle *client) : m_IsForgeClient(false), m_Errored(false), m_Client(client)
{
}





void cForgeHandshake::AugmentServerListPing(Json::Value & a_ResponseValue)
{
	UInt32 ProtocolVersion = m_Client->GetProtocolVersion();
	cForgeMods & Mods = cRoot::Get()->GetServer()->GetRegisteredForgeMods(ProtocolVersion);

	if (!Mods.HasMods())
	{
		return;
	}

	LOG("Received server ping from version: %d", ProtocolVersion);

	// modinfo:
	Json::Value Modinfo;
	Modinfo["type"] = "FML";

	Json::Value ModList(Json::arrayValue);

	for (auto & item: Mods.GetMods())
	{
		Json::Value Mod;
		Mod["modid"] = item.first;
		Mod["version"] = item.second;
		ModList.append(Mod);
	}

	Modinfo["modList"] = ModList;

	// Augment the response:
	a_ResponseValue["modinfo"] = Modinfo;
}





void cForgeHandshake::BeginForgeHandshake(const AString & a_Name, const AString & a_UUID, const Json::Value & a_Properties)
{
	ASSERT(m_IsForgeClient);

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
	// Discriminator | Byte | Always 0 for ServerHello
	message.push_back(Discriminator_ServerHello);
	// FML protocol Version | Byte | Determined from NetworkRegistery. Currently 2.
	message.push_back('\2');
	// Dimension TODO
	message.push_back('\0');
	message.push_back('\0');
	message.push_back('\0');
	message.push_back('\0');

	m_Client->SendPluginMessage("FML|HS", message);
}

AStringMap cForgeHandshake::ParseModList(const char * a_Data, size_t a_Size)
{
	cByteBuffer buf(a_Size);
	buf.Write(a_Data, a_Size);

	Int8 discriminator;
	buf.ReadBEInt8(discriminator);

	ASSERT(discriminator == 2);

	UInt32 numMods;
	buf.ReadVarInt32(numMods);

	AStringMap mods;

	for (size_t i = 0; i < numMods; ++i)
	{
		AString name, version;
		buf.ReadVarUTF8String(name);
		buf.ReadVarUTF8String(version);

		mods.insert(std::pair<AString, AString>(name, version));

		LOGD("ParseModList name=%s, version=%s", name.c_str(), version.c_str());
	}

	return mods;
}

void cForgeHandshake::DataReceived(cClientHandle * a_Client, const char * a_Data, size_t a_Size)
{
	if (!m_IsForgeClient)
	{
		LOG("Received unexpected Forge data from non-Forge client (%zu bytes)", a_Size);
		return;
	}

	LOGD("Received Forge data: %zu bytes: %s", a_Size, a_Data);

	if (a_Size <= 1)
	{
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
				LOGD("Received ClientHello with FML protocol version %d", fmlProtocolVersion);
				if (fmlProtocolVersion != 2)
				{
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
			LOGD("Received ModList");

			AStringMap clientMods = ParseModList(a_Data, a_Size);
			AString clientModsString;
			for (auto & item: clientMods)
			{
				clientModsString.append(item.first);
				clientModsString.append("@");
				clientModsString.append(item.second);
				clientModsString.append(", ");
			}

			LOG("Client connected with %zu mods: %s", clientMods.size(), clientModsString.c_str());

			if (m_Client->m_ForgeMods != nullptr)
			{
				delete m_Client->m_ForgeMods;
			}

			m_Client->m_ForgeMods = new cForgeMods(clientMods);

			// Let the plugins know about this event, they may refuse the player:
			if (cRoot::Get()->GetPluginManager()->CallHookLoginForge(*a_Client))
			{
				LOG("Modded client refused by plugin");
				SetError();
				return;
			}

			// Send server ModList

			// TODO: max size?
			cByteBuffer buf(1024);

			// Send server-side Forge mods registered by plugins
			auto &serverMods = m_Client->GetForgeMods();

			size_t modCount = serverMods.GetNumMods();

			buf.WriteBEInt8(Discriminator_ModList);
			buf.WriteVarInt32(static_cast<UInt32>(modCount));
			for (size_t i = 0; i < modCount; ++i)
			{
				buf.WriteVarUTF8String(serverMods.GetModNameAt(i));
				buf.WriteVarUTF8String(serverMods.GetModVersionAt(i));
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
			LOGD("Received client HandshakeAck with phase=%d", phase);

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





void cForgeHandshake::SetError()
{
	m_Errored = true;
}
