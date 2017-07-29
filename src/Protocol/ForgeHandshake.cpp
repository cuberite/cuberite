
// ForgeHandshake.cpp

// Implements Forge protocol handshaking

#include "Globals.h"
#include "ForgeHandshake.h"
#include "json/json.h"
#include "../Server.h"
#include "../Bindings/PluginManager.h"
#include "../ClientHandle.h"
#include "../Root.h"


/** Discriminator byte values prefixing the FML|HS packets to determine their type. */
enum Discriminator: signed char
{
	Discriminator_ServerHello = 0,
	Discriminator_ClientHello = 1,
	Discriminator_ModList = 2,
	Discriminator_RegistryData = 3,
	Discriminator_HandshakeReset = -2,
	Discriminator_HandshakeAck = -1,
};

/** Client handshake state phases. */
enum
{
	ClientPhase_WAITINGSERVERDATA = 2,
	ClientPhase_WAITINGSERVERCOMPLETE = 3,
	ClientPhase_PENDINGCOMPLETE = 4,
	ClientPhase_COMPLETE = 5,
};

/** Server handshake state phases. */
enum
{
	ServerPhase_WAITINGCACK = 2,
	ServerPhase_COMPLETE = 3,
};





cForgeHandshake::cForgeHandshake(cClientHandle *a_Client) :
	m_IsForgeClient(false),
	m_Errored(false),
	m_Client(a_Client)
{
}





void cForgeHandshake::AugmentServerListPing(Json::Value & a_ResponseValue)
{
	UInt32 ProtocolVersion = m_Client->GetProtocolVersion();
	AStringMap & Mods = cRoot::Get()->GetServer()->GetRegisteredForgeMods(ProtocolVersion);

	if (Mods.size() == 0)
	{
		return;
	}

	LOG("Received server ping from version: %d", ProtocolVersion);

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





void cForgeHandshake::BeginForgeHandshake(const AString & a_Name, const AString & a_UUID, const Json::Value & a_Properties)
{
	ASSERT(m_IsForgeClient);

	m_Name = &a_Name;
	m_UUID = &a_UUID;
	m_Properties = &a_Properties;

	std::array<AString, 5> Channels{{ "FML|HS", "FML", "FML|MP", "FML", "FORGE" }};
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
	AStringMap Mods;

	if (a_Size < 4)
	{
		LOGD("ParseModList invalid packet, missing length (size=" SIZE_T_FMT ")", a_Size);
		return Mods;
	}

	cByteBuffer Buf(a_Size);
	Buf.Write(a_Data, a_Size);
	UInt32 NumMods;
	if (!Buf.ReadVarInt32(NumMods))
	{
		LOGD("ParseModList failed to read mod count");
		return Mods;
	}

	for (size_t i = 0; i < NumMods; ++i)
	{
		AString Name, Version;
		if (!Buf.ReadVarUTF8String(Name))
		{
			LOGD("ParseModList failed to read mod name at i=" SIZE_T_FMT, i);
			break;
		}
		if (!Buf.ReadVarUTF8String(Version))
		{
			LOGD("ParseModList failed to read mod version at i=" SIZE_T_FMT, i);
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
			LOG("Unsupported FML client protocol version received in ClientHello: %d", FmlProtocolVersion);
			SetError();
		}
	}
	else
	{
		LOG("Unexpectedly short ClientHello received");
		SetError();
	}
}




void cForgeHandshake::HandleModList(cClientHandle * a_Client, const char * a_Data, size_t a_Size)
{
	LOGD("Received ModList");

	AStringMap ClientMods = ParseModList(a_Data + 1, a_Size - 1);
	AString ClientModsString;
	for (auto & item: ClientMods)
	{
		ClientModsString.append(item.first);
		ClientModsString.append("@");
		ClientModsString.append(item.second);
		ClientModsString.append(", ");
	}

	LOG("Client connected with " SIZE_T_FMT " mods: %s", ClientMods.size(), ClientModsString.c_str());

	if (m_Client->m_ForgeMods != nullptr)
	{
		m_Client->m_ForgeMods.reset();
	}

	m_Client->m_ForgeMods = cpp14::make_unique<AStringMap>(ClientMods);

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

	size_t ModCount = ServerMods.size();

	Buf.WriteBEInt8(Discriminator_ModList);
	Buf.WriteVarInt32(static_cast<UInt32>(ModCount));
	for (auto & item: ServerMods)
	{
		const AString & name = item.first;
		const AString & version = item.second;
		Buf.WriteVarUTF8String(name);
		Buf.WriteVarUTF8String(version);
	}
	AString ServerModList;
	Buf.ReadAll(ServerModList);

	m_Client->SendPluginMessage("FML|HS", ServerModList);
}




void cForgeHandshake::HandleHandshakeAck(cClientHandle * a_Client, const char * a_Data, size_t a_Size)
{
	if (a_Size != 2)
	{
		LOG("Unexpected HandshakeAck packet length: " SIZE_T_FMT "", a_Size);
		SetError();
		return;
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
}





void cForgeHandshake::DataReceived(cClientHandle * a_Client, const char * a_Data, size_t a_Size)
{
	if (!m_IsForgeClient)
	{
		LOG("Received unexpected Forge data from non-Forge client (" SIZE_T_FMT " bytes)", a_Size);
		return;
	}
	LOGD("Received Forge data: " SIZE_T_FMT " bytes: %s", a_Size, a_Data);

	if (a_Size <= 1)
	{
		LOG("Received unexpectedly short Forge data (" SIZE_T_FMT " bytes)", a_Size);
		return;
	}

	int Discriminator = a_Data[0];

	switch (Discriminator)
	{
		case Discriminator_ClientHello:
			HandleClientHello(a_Client, a_Data, a_Size);
			break;

		case Discriminator_ModList:
			HandleModList(a_Client, a_Data, a_Size);
			break;

		case Discriminator_HandshakeAck:
			HandleHandshakeAck(a_Client, a_Data, a_Size);
			break;

		default:
		{
			LOG("Unexpected Forge packet %d received", Discriminator);
			SetError();
			return;
		}
	}
}





void cForgeHandshake::SetError()
{
	m_Errored = true;
}
