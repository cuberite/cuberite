
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
	static const auto WAITINGCACK = std::byte(2);
	static const auto COMPLETE = std::byte(3);
}





cForgeHandshake::cForgeHandshake() :
	IsForgeClient(false),
	m_Errored(false)
{
}





void cForgeHandshake::SetError(const AString & message)
{
	LOGD("Forge handshake error: %s", message.c_str());
	m_Errored = true;
}





void cForgeHandshake::AugmentServerListPing(cClientHandle & a_Client, Json::Value & a_ResponseValue)
{
	auto ProtocolVersion = a_Client.GetProtocolVersion();
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





void cForgeHandshake::BeginForgeHandshake(cClientHandle & a_Client)
{
	ASSERT(IsForgeClient);

	static const std::array<std::string_view, 5> Channels{{ "FML|HS", "FML", "FML|MP", "FML", "FORGE" }};
	ContiguousByteBuffer ChannelsString;
	for (auto & Channel: Channels)
	{
		ChannelsString.append({ reinterpret_cast<const std::byte *>(Channel.data()), Channel.size() });
		ChannelsString.push_back(std::byte(0));
	}

	a_Client.SendPluginMessage("REGISTER", ChannelsString);
	SendServerHello(a_Client);
}





void cForgeHandshake::SendServerHello(cClientHandle & a_Client)
{
	cByteBuffer Buf(6);
	// Discriminator | Byte | Always 0 for ServerHello
	Buf.WriteBEInt8(Discriminator::ServerHello);
	// FML protocol Version | Byte | Determined from NetworkRegistery. Currently 2.
	Buf.WriteBEInt8(2);
	// Dimension TODO
	Buf.WriteBEInt32(0);

	ContiguousByteBuffer Message;
	Buf.ReadAll(Message);

	a_Client.SendPluginMessage("FML|HS", Message);
}





AStringMap cForgeHandshake::ParseModList(const ContiguousByteBufferView a_Data)
{
	AStringMap Mods;

	if (a_Data.size() < 4)
	{
		SetError(fmt::format(FMT_STRING("ParseModList invalid packet, missing length (size = {})"), a_Data.size()));
		return Mods;
	}

	cByteBuffer Buf(a_Data.size());
	Buf.Write(a_Data.data(), a_Data.size());
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
			SetError(fmt::format(FMT_STRING("ParseModList failed to read mod name at i = {}"), i));
			break;
		}
		if (!Buf.ReadVarUTF8String(Version))
		{
			SetError(fmt::format(FMT_STRING("ParseModList failed to read mod version at i = {}"), i));
			break;
		}
		Mods.insert({Name, Version});
	}

	return Mods;
}





void cForgeHandshake::HandleClientHello(cClientHandle & a_Client, const ContiguousByteBufferView a_Data)
{
	if (a_Data.size() == 2)
	{
		const auto FmlProtocolVersion = static_cast<Int8>(a_Data[1]);
		LOGD("Received ClientHello with FML protocol version %d", FmlProtocolVersion);
		if (FmlProtocolVersion != 2)
		{
			SetError(fmt::format(FMT_STRING("Unsupported FML client protocol version received in ClientHello: {}"), FmlProtocolVersion));
		}
	}
	else
	{
		SetError(fmt::format(FMT_STRING("Received unexpected length of ClientHello: {}"), a_Data.size()));
	}
}





void cForgeHandshake::HandleModList(cClientHandle & a_Client, const ContiguousByteBufferView a_Data)
{
	LOGD("Received ModList");

	auto ClientMods = ParseModList(a_Data.substr(1));
	AString ClientModsString;
	for (auto & item: ClientMods)
	{
		ClientModsString.append(fmt::format(FMT_STRING("{}@{}, "), item.first, item.second));
	}

	LOG("Client connected with %zu mods: %s", ClientMods.size(), ClientModsString);

	a_Client.m_ForgeMods = ClientMods;

	// Let the plugins know about this event, they may refuse the player:
	if (cRoot::Get()->GetPluginManager()->CallHookLoginForge(a_Client, ClientMods))
	{
		SetError("Modded client refused by plugin");
		return;
	}

	// Send server ModList

	// Send server-side Forge mods registered by plugins
	const auto & ServerMods = a_Client.GetForgeMods();

	const auto ModCount = ServerMods.size();

	cByteBuffer Buf(256 * ModCount);

	Buf.WriteBEInt8(Discriminator::ModList);
	Buf.WriteVarInt32(static_cast<UInt32>(ModCount));
	for (const auto & item: ServerMods)
	{
		Buf.WriteVarUTF8String(item.first);   // name
		Buf.WriteVarUTF8String(item.second);  // version
	}

	ContiguousByteBuffer ServerModList;
	Buf.ReadAll(ServerModList);

	a_Client.SendPluginMessage("FML|HS", ServerModList);
}





void cForgeHandshake::HandleHandshakeAck(cClientHandle & a_Client, const ContiguousByteBufferView a_Data)
{
	if (a_Data.size() != 2)
	{
		SetError(fmt::format(FMT_STRING("Unexpected HandshakeAck packet length: {}"), a_Data.size()));
		return;
	}

	const auto Phase = static_cast<Int8>(a_Data[1]);
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

			ContiguousByteBuffer RegistryData;
			Buf.ReadAll(RegistryData);
			a_Client.SendPluginMessage("FML|HS", RegistryData);
			break;
		}

		case ClientPhase::WAITINGSERVERCOMPLETE:
		{
			LOGD("Client finished receiving registry data; acknowledging");

			ContiguousByteBuffer Ack;
			Ack.push_back(std::byte(Discriminator::HandshakeAck));
			Ack.push_back(ServerPhase::WAITINGCACK);
			a_Client.SendPluginMessage("FML|HS", Ack);
			break;
		}

		case ClientPhase::PENDINGCOMPLETE:
		{
			LOGD("Client is pending completion; sending complete ack");

			ContiguousByteBuffer Ack;
			Ack.push_back(std::byte(Discriminator::HandshakeAck));
			Ack.push_back(ServerPhase::COMPLETE);
			a_Client.SendPluginMessage("FML|HS", Ack);

			break;
		}

		case ClientPhase::COMPLETE:
		{
			// Now finish logging in:
			a_Client.FinishAuthenticate();
			break;
		}

		default:
		{
			SetError(fmt::format("Received unknown phase in Forge handshake acknowledgement: {}", Phase));
			break;
		}
	}
}





void cForgeHandshake::DataReceived(cClientHandle & a_Client, const ContiguousByteBufferView a_Data)
{
	if (!IsForgeClient)
	{
		SetError(fmt::format(FMT_STRING("Received unexpected Forge data from non-Forge client ({} bytes)"), a_Data.size()));
		return;
	}
	if (m_Errored)
	{
		LOGD("Received unexpected Forge data when in errored state, ignored");
		return;
	}

	if (a_Data.size() <= 1)
	{
		SetError(fmt::format(FMT_STRING("Received unexpectedly short Forge data ({} bytes)"), a_Data.size()));
		return;
	}

	const auto Discriminator = static_cast<Int8>(a_Data[0]);
	switch (Discriminator)
	{
		case Discriminator::ClientHello:  HandleClientHello(a_Client, a_Data); break;
		case Discriminator::ModList:      HandleModList(a_Client, a_Data); break;
		case Discriminator::HandshakeAck: HandleHandshakeAck(a_Client, a_Data); break;

		default:
		{
			SetError(fmt::format(FMT_STRING("Unexpected Forge packet {0} (0x{0:x}) received"), Discriminator));
			return;
		}
	}
}
