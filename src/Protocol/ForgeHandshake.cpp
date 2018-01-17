
// ForgeHandshake.cpp

// Implements Forge protocol handshaking

#include "Globals.h"
#include "ForgeHandshake.h"
#include "json/json.h"
#include "../Server.h"
#include "../ByteBuffer.h"
#include "../Bindings/PluginManager.h"
#include "../ClientHandle.h"
#include "../Root.h"


/** Discriminator byte values prefixing the FML|HS packets to determine their type. */
namespace Discriminator
{
	static const Int8 ServerHello = 0;
	static const Int8 ClientHello = 1;
	static const Int8 ModList = 2;
	static const Int8 RegistryData = 3;
	// static const Int8 HandshakeReset = -2;
	static const Int8 HandshakeAck = -1;
}

/** Client handshake state phases. */
namespace ClientPhase
{
	static const Int8 WAITINGSERVERDATA = 2;
	static const Int8 WAITINGSERVERCOMPLETE = 3;
	static const Int8 PENDINGCOMPLETE = 4;
	static const Int8 COMPLETE = 5;
}

/** Server handshake state phases. */
namespace ServerPhase
{
	static const Int8 WAITINGCACK = 2;
	static const Int8 COMPLETE = 3;
}





cForgeHandshake::cForgeHandshake(cClientHandle *a_Client) :
	m_IsForgeClient(false),
	m_Errored(false),
	m_Client(a_Client)
{
}





void cForgeHandshake::SetError(const AString & message)
{
	LOGD("Forge handshake error: %s", message.c_str());
	m_Errored = true;
}





void cForgeHandshake::AugmentServerListPing(Json::Value & a_ResponseValue)
{
	auto ProtocolVersion = m_Client->GetProtocolVersion();
	auto & Mods = cRoot::Get()->GetServer()->GetRegisteredForgeMods(ProtocolVersion);

	if (Mods.empty())
	{
		return;
	}

	LOGD("Received server ping from version: %d", ProtocolVersion);

	Json::Value Modinfo;
	Modinfo["type"] = "FML";

	Json::Value ModList(Json::arrayValue);
	for (auto & item: Mods)
	{
		Json::Value Mod;
		Mod["modid"] = item.first;
		Mod["version"] = item.second;
		ModList.append(Mod);
	}
	Modinfo["modList"] = ModList;

	a_ResponseValue["modinfo"] = Modinfo;
}





void cForgeHandshake::BeginForgeHandshake(const AString & a_Name, const cUUID & a_UUID, const Json::Value & a_Properties)
{
	ASSERT(m_IsForgeClient);

	m_Name = a_Name;
	m_UUID = a_UUID;
	m_Properties = a_Properties;

	static const std::array<AString, 5> Channels{{ "FML|HS", "FML", "FML|MP", "FML", "FORGE" }};
	AString ChannelsString;
	for (auto & Channel: Channels)
	{
		ChannelsString.append(Channel);
		ChannelsString.push_back('\0');
	}

	m_Client->SendPluginMessage("REGISTER", ChannelsString);
	SendServerHello();
}





void cForgeHandshake::SendServerHello()
{
	AString Message;
	cByteBuffer Buf(6);
	// Discriminator | Byte | Always 0 for ServerHello
	Buf.WriteBEInt8(Discriminator::ServerHello);
	// FML protocol Version | Byte | Determined from NetworkRegistery. Currently 2.
	Buf.WriteBEInt8(2);
	// Dimension TODO
	Buf.WriteBEInt32(0);
	Buf.ReadAll(Message);

	m_Client->SendPluginMessage("FML|HS", Message);
}





AStringMap cForgeHandshake::ParseModList(const char * a_Data, size_t a_Size)
{
	AStringMap Mods;

	if (a_Size < 4)
	{
		SetError(Printf("ParseModList invalid packet, missing length (size = %zu)", a_Size));
		return Mods;
	}

	cByteBuffer Buf(a_Size);
	Buf.Write(a_Data, a_Size);
	UInt32 NumMods;
	if (!Buf.ReadVarInt32(NumMods))
	{
		SetError("ParseModList failed to read mod count");
		return Mods;
	}

	for (UInt32 i = 0; i < NumMods; ++i)
	{
		AString Name, Version;
		if (!Buf.ReadVarUTF8String(Name))
		{
			SetError(Printf("ParseModList failed to read mod name at i = %d", i));
			break;
		}
		if (!Buf.ReadVarUTF8String(Version))
		{
			SetError(Printf("ParseModList failed to read mod version at i = %d", i));
			break;
		}
		Mods.insert({Name, Version});
	}

	return Mods;
}




void cForgeHandshake::HandleClientHello(cClientHandle * a_Client, const char * a_Data, size_t a_Size)
{
	if (a_Size == 2)
	{
		int FmlProtocolVersion = a_Data[1];
		LOGD("Received ClientHello with FML protocol version %d", FmlProtocolVersion);
		if (FmlProtocolVersion != 2)
		{
			SetError(Printf("Unsupported FML client protocol version received in ClientHello: %d", FmlProtocolVersion));
		}
	}
	else
	{
		SetError(Printf("Received unexpected length of ClientHello: %zu", a_Size));
	}
}




void cForgeHandshake::HandleModList(cClientHandle * a_Client, const char * a_Data, size_t a_Size)
{
	LOGD("Received ModList");

	auto ClientMods = ParseModList(a_Data + 1, a_Size - 1);
	AString ClientModsString;
	for (auto & item: ClientMods)
	{
		AppendPrintf(ClientModsString, "%s@%s, ", item.first.c_str(), item.second.c_str());
	}

	LOG("Client connected with %zu mods: %s", ClientMods.size(), ClientModsString.c_str());

	m_Client->m_ForgeMods = ClientMods;

	// Let the plugins know about this event, they may refuse the player:
	if (cRoot::Get()->GetPluginManager()->CallHookLoginForge(*a_Client, ClientMods))
	{
		SetError("Modded client refused by plugin");
		return;
	}

	// Send server ModList

	// Send server-side Forge mods registered by plugins
	const auto & ServerMods = m_Client->GetForgeMods();

	const auto ModCount = ServerMods.size();

	cByteBuffer Buf(256 * ModCount);

	Buf.WriteBEInt8(Discriminator::ModList);
	Buf.WriteVarInt32(static_cast<UInt32>(ModCount));
	for (const auto & item: ServerMods)
	{
		Buf.WriteVarUTF8String(item.first);   // name
		Buf.WriteVarUTF8String(item.second);  // version
	}
	AString ServerModList;
	Buf.ReadAll(ServerModList);

	m_Client->SendPluginMessage("FML|HS", ServerModList);
}




void cForgeHandshake::HandleHandshakeAck(cClientHandle * a_Client, const char * a_Data, size_t a_Size)
{
	if (a_Size != 2)
	{
		SetError(Printf("Unexpected HandshakeAck packet length: %zu", a_Size));
		return;
	}

	auto Phase = a_Data[1];
	LOGD("Received client HandshakeAck with phase = %d", Phase);

	switch (Phase)
	{
		case ClientPhase::WAITINGSERVERDATA:
		{
			cByteBuffer Buf(1024);
			Buf.WriteBEInt8(Discriminator::RegistryData);

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

		case ClientPhase::WAITINGSERVERCOMPLETE:
		{
			LOGD("Client finished receiving registry data; acknowledging");

			AString Ack;
			Ack.push_back(Discriminator::HandshakeAck);
			Ack.push_back(ServerPhase::WAITINGCACK);
			m_Client->SendPluginMessage("FML|HS", Ack);
			break;
		}

		case ClientPhase::PENDINGCOMPLETE:
		{
			LOGD("Client is pending completion; sending complete ack");

			AString Ack;
			Ack.push_back(Discriminator::HandshakeAck);
			Ack.push_back(ServerPhase::COMPLETE);
			m_Client->SendPluginMessage("FML|HS", Ack);

			break;
		}

		case ClientPhase::COMPLETE:
		{
			// Now finish logging in
			m_Client->FinishAuthenticate(m_Name, m_UUID, m_Properties);
			break;
		}

		default:
		{
			SetError(Printf("Received unknown phase in Forge handshake acknowledgement: %d", Phase));
			break;
		}
	}
}





void cForgeHandshake::DataReceived(cClientHandle * a_Client, const char * a_Data, size_t a_Size)
{
	if (!m_IsForgeClient)
	{
		SetError(Printf("Received unexpected Forge data from non-Forge client (%zu bytes)", a_Size));
		return;
	}
	if (m_Errored)
	{
		LOGD("Received unexpected Forge data when in errored state, ignored");
		return;
	}

	if (a_Size <= 1)
	{
		SetError(Printf("Received unexpectedly short Forge data (%zu bytes)", a_Size));
		return;
	}

	auto Discriminator = a_Data[0];

	switch (Discriminator)
	{
		case Discriminator::ClientHello:  HandleClientHello(a_Client, a_Data, a_Size); break;
		case Discriminator::ModList:      HandleModList(a_Client, a_Data, a_Size); break;
		case Discriminator::HandshakeAck: HandleHandshakeAck(a_Client, a_Data, a_Size); break;

		default:
		{
			SetError(Printf("Unexpected Forge packet %d received", Discriminator));
			return;
		}
	}
}
