
// ForgeHandshake.cpp

// Implements Forge protocol handshaking

#include "Globals.h"
#include "ForgeHandshake.h"
#include "json/json.h"
#include "../Server.h"
#include "../Bindings/PluginManager.h"
#include "../ClientHandle.h"
#include "../Root.h"

cForgeHandshake::cForgeHandshake(cClientHandle *client) :
	m_IsForgeClient(false),
	m_Errored(false),
	m_Client(client)
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

	AStringVector Channels = { "FML|HS", "FML", "FML|MP", "FML", "FORGE" };
	AString ChannelsString;

	for (AStringVector::iterator itr = Channels.begin(); itr != Channels.end(); ++itr)
	{
		ChannelsString.append(*itr);
		ChannelsString.push_back('\0');
	}

	m_Client->SendPluginMessage("REGISTER", ChannelsString);
	SendServerHello();
}





void cForgeHandshake::SendServerHello()
{
	AString Message;
	// Discriminator | Byte | Always 0 for ServerHello
	Message.push_back(Discriminator_ServerHello);
	// FML protocol Version | Byte | Determined from NetworkRegistery. Currently 2.
	Message.push_back('\2');
	// Dimension TODO
	Message.push_back('\0');
	Message.push_back('\0');
	Message.push_back('\0');
	Message.push_back('\0');

	m_Client->SendPluginMessage("FML|HS", Message);
}





AStringMap cForgeHandshake::ParseModList(const char * a_Data, size_t a_Size)
{
	cByteBuffer Buf(a_Size);
	Buf.Write(a_Data, a_Size);

	Int8 Discriminator;
	Buf.ReadBEInt8(Discriminator);

	ASSERT(Discriminator == 2);

	UInt32 NumMods;
	Buf.ReadVarInt32(NumMods);

	AStringMap Mods;

	for (size_t i = 0; i < NumMods; ++i)
	{
		AString Name, Version;
		Buf.ReadVarUTF8String(Name);
		Buf.ReadVarUTF8String(Version);

		Mods.insert(std::pair<AString, AString>(Name, Version));

		LOGD("ParseModList name=%s, version=%s", Name.c_str(), Version.c_str());
	}

	return Mods;
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

	int Discriminator = a_Data[0];

	switch (Discriminator)
	{
		case Discriminator_ClientHello:
		{
			if (a_Size == 2)
			{
				int FmlProtocolVersion = a_Data[1];
				LOGD("Received ClientHello with FML protocol version %d", FmlProtocolVersion);
				if (FmlProtocolVersion != 2)
				{
					LOG("Unsupported FML client protocol version received in ClientHello: %d", FmlProtocolVersion);
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

			AStringMap ClientMods = ParseModList(a_Data, a_Size);
			AString ClientModsString;
			for (auto & item: ClientMods)
			{
				ClientModsString.append(item.first);
				ClientModsString.append("@");
				ClientModsString.append(item.second);
				ClientModsString.append(", ");
			}

			LOG("Client connected with %zu mods: %s", ClientMods.size(), ClientModsString.c_str());

			if (m_Client->m_ForgeMods != nullptr)
			{
				delete m_Client->m_ForgeMods;
			}

			m_Client->m_ForgeMods = new cForgeMods(ClientMods);

			// Let the plugins know about this event, they may refuse the player:
			if (cRoot::Get()->GetPluginManager()->CallHookLoginForge(*a_Client))
			{
				LOG("Modded client refused by plugin");
				SetError();
				return;
			}

			// Send server ModList

			// TODO: max size?
			cByteBuffer Buf(1024);

			// Send server-side Forge mods registered by plugins
			auto &ServerMods = m_Client->GetForgeMods();

			size_t ModCount = ServerMods.GetNumMods();

			Buf.WriteBEInt8(Discriminator_ModList);
			Buf.WriteVarInt32(static_cast<UInt32>(ModCount));
			for (size_t i = 0; i < ModCount; ++i)
			{
				const AString & name = ServerMods.GetModNameAt(i);
				const AString & version = ServerMods.GetModVersion(name);
				Buf.WriteVarUTF8String(name);
				Buf.WriteVarUTF8String(version);
			}
			AString ServerModList;
			Buf.ReadAll(ServerModList);

			m_Client->SendPluginMessage("FML|HS", ServerModList);
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

			int Phase = a_Data[1];
			LOGD("Received client HandshakeAck with phase=%d", Phase);

			switch (Phase)
			{
				case ClientPhase_WAITINGSERVERDATA:
				{
					cByteBuffer Buf(1024);
					Buf.WriteBEInt8(Discriminator_RegistryData);

					// TODO: send real registry data
					bool HasMore = false;
					AString RegistryName = "potions";
					UInt32 NumIDs = 0;
					UInt32 NumSubstitutions = 0;
					UInt32 NumDummies = 0;

					Buf.WriteBool(HasMore);
					Buf.WriteVarUTF8String(RegistryName);
					Buf.WriteVarInt32(NumIDs);
					Buf.WriteVarInt32(NumSubstitutions);
					Buf.WriteVarInt32(NumDummies);

					AString RegistryData;
					Buf.ReadAll(RegistryData);
					m_Client->SendPluginMessage("FML|HS", RegistryData);
					break;
				}

				case ClientPhase_WAITINGSERVERCOMPLETE:
				{
					LOG("Client finished receiving registry data; acknowledging");

					AString Ack;
					Ack.push_back(Discriminator_HandshakeAck);
					Ack.push_back(ServerPhase_WAITINGCACK);
					m_Client->SendPluginMessage("FML|HS", Ack);
					break;
				}

				case ClientPhase_PENDINGCOMPLETE:
				{
					LOG("Client is pending completion; sending complete ack");

					AString Ack;
					Ack.push_back(Discriminator_HandshakeAck);
					Ack.push_back(ServerPhase_COMPLETE);
					m_Client->SendPluginMessage("FML|HS", Ack);

					// Now finish logging in
					m_Client->FinishAuthenticate(*m_Name, *m_UUID, *m_Properties);
					break;
				}
			}
			break;
		}

		default:
			LOG("Unexpected Forge packet %d received", Discriminator);
			SetError();
			return;
	}
}





void cForgeHandshake::SetError()
{
	m_Errored = true;
}
