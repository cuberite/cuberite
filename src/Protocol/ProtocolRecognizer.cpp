
// ProtocolRecognizer.cpp

// Implements the cProtocolRecognizer class representing the meta-protocol that recognizes possibly multiple
// protocol versions and redirects everything to them

#include "Globals.h"

#include "ProtocolRecognizer.h"
#include "Protocol_1_8.h"
#include "Protocol_1_9.h"
#include "Protocol_1_10.h"
#include "Protocol_1_11.h"
#include "Protocol_1_12.h"
#include "Protocol_1_13.h"
#include "../ClientHandle.h"
#include "../Root.h"
#include "../Server.h"
#include "../World.h"
#include "../JsonUtils.h"
#include "../Bindings/PluginManager.h"





struct sUnsupportedButPingableProtocolException : public std::runtime_error
{
	explicit sUnsupportedButPingableProtocolException() :
		std::runtime_error("")
	{
	}
};





struct sTriedToJoinWithUnsupportedProtocolException : public std::runtime_error
{
	explicit sTriedToJoinWithUnsupportedProtocolException(const std::string & a_Message) :
		std::runtime_error(a_Message)
	{
	}
};





cMultiVersionProtocol::cMultiVersionProtocol() :
	HandleIncomingData(std::bind(&cMultiVersionProtocol::HandleIncomingDataInRecognitionStage, this, std::placeholders::_1, std::placeholders::_2)),
	m_Buffer(8 KiB)  // We need a larger buffer to support BungeeCord - it sends one huge packet at the start
{
}





AString cMultiVersionProtocol::GetVersionTextFromInt(int a_ProtocolVersion)
{
	switch (a_ProtocolVersion)
	{
		case PROTO_VERSION_1_8_0:   return "1.8";
		case PROTO_VERSION_1_9_0:   return "1.9";
		case PROTO_VERSION_1_9_1:   return "1.9.1";
		case PROTO_VERSION_1_9_2:   return "1.9.2";
		case PROTO_VERSION_1_9_4:   return "1.9.4";
		case PROTO_VERSION_1_10_0:  return "1.10";
		case PROTO_VERSION_1_11_0:  return "1.11";
		case PROTO_VERSION_1_11_1:  return "1.11.1";
		case PROTO_VERSION_1_12:    return "1.12";
		case PROTO_VERSION_1_12_1:  return "1.12.1";
		case PROTO_VERSION_1_12_2:  return "1.12.2";
		case PROTO_VERSION_1_13:    return "1.13";
	}
	ASSERT(!"Unknown protocol version");
	return Printf("Unknown protocol (%d)", a_ProtocolVersion);
}





void cMultiVersionProtocol::HandleIncomingDataInRecognitionStage(cClientHandle & a_Client, std::string_view a_Data)
{
	// We read more than the handshake packet here, oh well.
	if (!m_Buffer.Write(a_Data.data(), a_Data.size()))
	{
		a_Client.Kick("Your client sent too much data; please try again later.");
		return;
	}

	try
	{
		// Note that a_Data is assigned to a subview containing the data to pass to m_Protocol or UnsupportedPing

		TryRecognizeProtocol(a_Client, a_Data);
		if (m_Protocol == nullptr)
		{
			m_Buffer.ResetRead();
			return;
		}

		// The protocol recogniser succesfully identified, switch mode:
		HandleIncomingData = [this](cClientHandle &, const std::string_view a_In)
		{
			// TODO: make it take our a_ReceivedData
			m_Protocol->DataReceived(a_In.data(), a_In.size());
		};
	}
	catch (const sUnsupportedButPingableProtocolException &)
	{
		// Got a server list ping for an unrecognised version,
		// switch into responding to unknown protocols mode:
		HandleIncomingData = [this](cClientHandle & a_Clyent, const std::string_view a_In)
		{
			HandleIncomingDataInOldPingResponseStage(a_Clyent, a_In);
		};
	}
	catch (const std::exception & Oops)
	{
		a_Client.Kick(Oops.what());
		return;
	}

	// Explicitly process any remaining data with the new handler:
	HandleIncomingData(a_Client, a_Data);
}





void cMultiVersionProtocol::HandleIncomingDataInOldPingResponseStage(cClientHandle & a_Client, const std::string_view a_Data)
{
	if (!m_Buffer.Write(a_Data.data(), a_Data.size()))
	{
		a_Client.Kick("Server list ping failed, too much data.");
		return;
	}

	cByteBuffer OutPacketBuffer(6 KiB);

	// Handle server list ping packets
	for (;;)
	{
		UInt32 PacketLen;
		UInt32 PacketID;
		if (
			!m_Buffer.ReadVarInt32(PacketLen) ||
			!m_Buffer.CanReadBytes(PacketLen) ||
			!m_Buffer.ReadVarInt32(PacketID)
		)
		{
			// Not enough data
			m_Buffer.ResetRead();
			break;
		}

		if ((PacketID == 0x00) && (PacketLen == 1))  // Request packet
		{
			HandlePacketStatusRequest(a_Client, OutPacketBuffer);
			SendPacket(a_Client, OutPacketBuffer);
		}
		else if ((PacketID == 0x01) && (PacketLen == 9))  // Ping packet
		{
			HandlePacketStatusPing(a_Client, OutPacketBuffer);
			SendPacket(a_Client, OutPacketBuffer);
		}
		else
		{
			a_Client.Kick("Server list ping failed, unrecognized packet.");
			return;
		}

		m_Buffer.CommitRead();
	}
}





void cMultiVersionProtocol::SendDisconnect(cClientHandle & a_Client, const AString & a_Reason)
{
	if (m_Protocol != nullptr)
	{
		m_Protocol->SendDisconnect(a_Reason);
		return;
	}

	const AString Message = Printf("{\"text\":\"%s\"}", EscapeString(a_Reason).c_str());
	const auto PacketID = GetPacketID(cProtocol::ePacketType::pktDisconnectDuringLogin);
	cByteBuffer Out(
		cByteBuffer::GetVarIntSize(static_cast<UInt32>(Message.size())) + Message.size() +
		cByteBuffer::GetVarIntSize(PacketID)
	);

	VERIFY(Out.WriteVarInt32(PacketID));
	VERIFY(Out.WriteVarUTF8String(Message));
	SendPacket(a_Client, Out);
}





/*void cProtocolRecognizer::SendEditSign(int a_BlockX, int a_BlockY, int a_BlockZ)
{
	ASSERT(m_Protocol != nullptr);
	m_Protocol->SendEditSign(a_BlockX, a_BlockY, a_BlockZ);
}





void cProtocolRecognizer::SendEntityAnimation(const cEntity & a_Entity, char a_Animation)
{
	ASSERT(m_Protocol != nullptr);
	m_Protocol->SendEntityAnimation(a_Entity, a_Animation);
}





void cProtocolRecognizer::SendEntityEffect(const cEntity & a_Entity, int a_EffectID, int a_Amplifier, int a_Duration)
{
	ASSERT(m_Protocol != nullptr);
	m_Protocol->SendEntityEffect(a_Entity, a_EffectID, a_Amplifier, a_Duration);
}





void cProtocolRecognizer::SendEntityEquipment(const cEntity & a_Entity, short a_SlotNum, const cItem & a_Item)
{
	ASSERT(m_Protocol != nullptr);
	m_Protocol->SendEntityEquipment(a_Entity, a_SlotNum, a_Item);
}





void cProtocolRecognizer::SendEntityHeadLook(const cEntity & a_Entity)
{
	ASSERT(m_Protocol != nullptr);
	m_Protocol->SendEntityHeadLook(a_Entity);
}





void cProtocolRecognizer::SendEntityLook(const cEntity & a_Entity)
{
	ASSERT(m_Protocol != nullptr);
	m_Protocol->SendEntityLook(a_Entity);
}





void cProtocolRecognizer::SendEntityMetadata(const cEntity & a_Entity)
{
	ASSERT(m_Protocol != nullptr);
	m_Protocol->SendEntityMetadata(a_Entity);
}





void cProtocolRecognizer::SendEntityPosition(const cEntity & a_Entity)
{
	ASSERT(m_Protocol != nullptr);
	m_Protocol->SendEntityPosition(a_Entity);
}





void cProtocolRecognizer::SendEntityProperties(const cEntity & a_Entity)
{
	ASSERT(m_Protocol != nullptr);
	m_Protocol->SendEntityProperties(a_Entity);
}





void cProtocolRecognizer::SendEntityStatus(const cEntity & a_Entity, char a_Status)
{
	ASSERT(m_Protocol != nullptr);
	m_Protocol->SendEntityStatus(a_Entity, a_Status);
}





void cProtocolRecognizer::SendEntityVelocity(const cEntity & a_Entity)
{
	ASSERT(m_Protocol != nullptr);
	m_Protocol->SendEntityVelocity(a_Entity);
}





void cProtocolRecognizer::SendExplosion(double a_BlockX, double a_BlockY, double a_BlockZ, float a_Radius, const cVector3iArray & a_BlocksAffected, const Vector3d & a_PlayerMotion)
{
	ASSERT(m_Protocol != nullptr);
	m_Protocol->SendExplosion(a_BlockX, a_BlockY, a_BlockZ, a_Radius, a_BlocksAffected, a_PlayerMotion);
}





void cProtocolRecognizer::SendGameMode(eGameMode a_GameMode)
{
	ASSERT(m_Protocol != nullptr);
	m_Protocol->SendGameMode(a_GameMode);
}





void cProtocolRecognizer::SendHealth(void)
{
	ASSERT(m_Protocol != nullptr);
	m_Protocol->SendHealth();
}





void cProtocolRecognizer::SendHeldItemChange(int a_ItemIndex)
{
	ASSERT(m_Protocol != nullptr);
	m_Protocol->SendHeldItemChange(a_ItemIndex);
}





void cProtocolRecognizer::SendHideTitle(void)
{
	ASSERT(m_Protocol != nullptr);
	m_Protocol->SendHideTitle();
}





void cProtocolRecognizer::SendWindowProperty(const cWindow & a_Window, short a_Property, short a_Value)
{
	ASSERT(m_Protocol != nullptr);
	m_Protocol->SendWindowProperty(a_Window, a_Property, a_Value);
}





void cProtocolRecognizer::SendInventorySlot(char a_WindowID, short a_SlotNum, const cItem & a_Item)
{
	ASSERT(m_Protocol != nullptr);
	m_Protocol->SendInventorySlot(a_WindowID, a_SlotNum, a_Item);
}





void cProtocolRecognizer::SendKeepAlive(UInt32 a_PingID)
{
	ASSERT(m_Protocol != nullptr);
	m_Protocol->SendKeepAlive(a_PingID);
}





void cProtocolRecognizer::SendLeashEntity(const cEntity & a_Entity, const cEntity & a_EntityLeashedTo)
{
	ASSERT(m_Protocol != nullptr);
	m_Protocol->SendLeashEntity(a_Entity, a_EntityLeashedTo);
}





void cProtocolRecognizer::SendUnleashEntity(const cEntity & a_Entity)
{
	ASSERT(m_Protocol != nullptr);
	m_Protocol->SendUnleashEntity(a_Entity);
}





void cProtocolRecognizer::SendLogin(const cPlayer & a_Player, const cWorld & a_World)
{
	ASSERT(m_Protocol != nullptr);
	m_Protocol->SendLogin(a_Player, a_World);
}





void cProtocolRecognizer::SendLoginSuccess(void)
{
	ASSERT(m_Protocol != nullptr);
	m_Protocol->SendLoginSuccess();
}





void cProtocolRecognizer::SendMapData(const cMap & a_Map, int a_DataStartX, int a_DataStartY)
{
	ASSERT(m_Protocol != nullptr);
	m_Protocol->SendMapData(a_Map, a_DataStartX, a_DataStartY);
}





void cProtocolRecognizer::SendParticleEffect(const AString & a_ParticleName, float a_SrcX, float a_SrcY, float a_SrcZ, float a_OffsetX, float a_OffsetY, float a_OffsetZ, float a_ParticleData, int a_ParticleAmount)
{
	ASSERT(m_Protocol != nullptr);
	m_Protocol->SendParticleEffect(a_ParticleName, a_SrcX, a_SrcY, a_SrcZ, a_OffsetX, a_OffsetY, a_OffsetZ, a_ParticleData, a_ParticleAmount);
}





void cProtocolRecognizer::SendParticleEffect(const AString & a_ParticleName, Vector3f a_Src, Vector3f a_Offset, float a_ParticleData, int a_ParticleAmount, std::array<int, 2> a_Data)
{
	ASSERT(m_Protocol != nullptr);
	m_Protocol->SendParticleEffect(a_ParticleName, a_Src, a_Offset, a_ParticleData, a_ParticleAmount, a_Data);
}





void cProtocolRecognizer::SendPaintingSpawn(const cPainting & a_Painting)
{
	m_Protocol->SendPaintingSpawn(a_Painting);
}





void cProtocolRecognizer::SendPlayerAbilities(void)
{
	ASSERT(m_Protocol != nullptr);
	m_Protocol->SendPlayerAbilities();
}





void cProtocolRecognizer::SendPlayerListAddPlayer(const cPlayer & a_Player)
{
	ASSERT(m_Protocol != nullptr);
	m_Protocol->SendPlayerListAddPlayer(a_Player);
}





void cProtocolRecognizer::SendPlayerListRemovePlayer(const cPlayer & a_Player)
{
	ASSERT(m_Protocol != nullptr);
	m_Protocol->SendPlayerListRemovePlayer(a_Player);
}





void cProtocolRecognizer::SendPlayerListUpdateGameMode(const cPlayer & a_Player)
{
	ASSERT(m_Protocol != nullptr);
	m_Protocol->SendPlayerListUpdateGameMode(a_Player);
}





void cProtocolRecognizer::SendPlayerListUpdatePing(const cPlayer & a_Player)
{
	ASSERT(m_Protocol != nullptr);
	m_Protocol->SendPlayerListUpdatePing(a_Player);
}





void cProtocolRecognizer::SendPlayerListUpdateDisplayName(const cPlayer & a_Player, const AString & a_CustomName)
{
	ASSERT(m_Protocol != nullptr);
	m_Protocol->SendPlayerListUpdateDisplayName(a_Player, a_CustomName);
}





void cProtocolRecognizer::SendPlayerMaxSpeed(void)
{
	ASSERT(m_Protocol != nullptr);
	m_Protocol->SendPlayerMaxSpeed();
}





void cProtocolRecognizer::SendPlayerMoveLook(void)
{
	ASSERT(m_Protocol != nullptr);
	m_Protocol->SendPlayerMoveLook();
}





void cProtocolRecognizer::SendPlayerPosition(void)
{
	ASSERT(m_Protocol != nullptr);
	m_Protocol->SendPlayerPosition();
}





void cProtocolRecognizer::SendPlayerSpawn(const cPlayer & a_Player)
{
	ASSERT(m_Protocol != nullptr);
	m_Protocol->SendPlayerSpawn(a_Player);
}





void cProtocolRecognizer::SendPluginMessage(const AString & a_Channel, const AString & a_Message)
{
	ASSERT(m_Protocol != nullptr);
	m_Protocol->SendPluginMessage(a_Channel, a_Message);
}





void cProtocolRecognizer::SendRemoveEntityEffect(const cEntity & a_Entity, int a_EffectID)
{
	ASSERT(m_Protocol != nullptr);
	m_Protocol->SendRemoveEntityEffect(a_Entity, a_EffectID);
}





void cProtocolRecognizer::SendResetTitle(void)
{
	ASSERT(m_Protocol != nullptr);
	m_Protocol->SendResetTitle();
}





void cProtocolRecognizer::SendResourcePack(const AString & a_ResourcePackUrl)
{
	ASSERT(m_Protocol != nullptr);
	m_Protocol->SendResourcePack(a_ResourcePackUrl);
}





void cProtocolRecognizer::SendRespawn(eDimension a_Dimension)
{
	ASSERT(m_Protocol != nullptr);
	m_Protocol->SendRespawn(a_Dimension);
}





void cProtocolRecognizer::SendExperience(void)
{
	ASSERT(m_Protocol != nullptr);
	m_Protocol->SendExperience();
}





void cProtocolRecognizer::SendExperienceOrb(const cExpOrb & a_ExpOrb)
{
	ASSERT(m_Protocol != nullptr);
	m_Protocol->SendExperienceOrb(a_ExpOrb);
}





void cProtocolRecognizer::SendScoreboardObjective(const AString & a_Name, const AString & a_DisplayName, Byte a_Mode)
{
	ASSERT(m_Protocol != nullptr);
	m_Protocol->SendScoreboardObjective(a_Name, a_DisplayName, a_Mode);
}





void cProtocolRecognizer::SendScoreUpdate(const AString & a_Objective, const AString & a_Player, cObjective::Score a_Score, Byte a_Mode)
{
	ASSERT(m_Protocol != nullptr);
	m_Protocol->SendScoreUpdate(a_Objective, a_Player, a_Score, a_Mode);
}





void cProtocolRecognizer::SendDisplayObjective(const AString & a_Objective, cScoreboard::eDisplaySlot a_Display)
{
	ASSERT(m_Protocol != nullptr);
	m_Protocol->SendDisplayObjective(a_Objective, a_Display);
}





void cProtocolRecognizer::SendSetSubTitle(const cCompositeChat & a_SubTitle)
{
	ASSERT(m_Protocol != nullptr);
	m_Protocol->SendSetSubTitle(a_SubTitle);
}





void cProtocolRecognizer::SendSetRawSubTitle(const AString & a_SubTitle)
{
	ASSERT(m_Protocol != nullptr);
	m_Protocol->SendSetRawSubTitle(a_SubTitle);
}





void cProtocolRecognizer::SendSetTitle(const cCompositeChat & a_Title)
{
	ASSERT(m_Protocol != nullptr);
	m_Protocol->SendSetTitle(a_Title);
}





void cProtocolRecognizer::SendSetRawTitle(const AString & a_Title)
{
	ASSERT(m_Protocol != nullptr);
	m_Protocol->SendSetRawTitle(a_Title);
}





void cProtocolRecognizer::SendSoundEffect(const AString & a_SoundName, double a_X, double a_Y, double a_Z, float a_Volume, float a_Pitch)
{
	ASSERT(m_Protocol != nullptr);
	m_Protocol->SendSoundEffect(a_SoundName, a_X, a_Y, a_Z, a_Volume, a_Pitch);
}





void cProtocolRecognizer::SendSoundParticleEffect(const EffectID a_EffectID, int a_SrcX, int a_SrcY, int a_SrcZ, int a_Data)
{
	ASSERT(m_Protocol != nullptr);
	m_Protocol->SendSoundParticleEffect(a_EffectID, a_SrcX, a_SrcY, a_SrcZ, a_Data);
}





void cProtocolRecognizer::SendSpawnEntity(const cEntity & a_Entity)
{
	ASSERT(m_Protocol != nullptr);
	m_Protocol->SendSpawnEntity(a_Entity);
}





void cProtocolRecognizer::SendSpawnMob(const cMonster & a_Mob)
{
	ASSERT(m_Protocol != nullptr);
	m_Protocol->SendSpawnMob(a_Mob);
}





void cProtocolRecognizer::SendStatistics(const cStatManager & a_Manager)
{
	ASSERT(m_Protocol != nullptr);
	m_Protocol->SendStatistics(a_Manager);
}





void cProtocolRecognizer::SendTabCompletionResults(const AStringVector & a_Results)
{
	ASSERT(m_Protocol != nullptr);
	m_Protocol->SendTabCompletionResults(a_Results);
}





void cProtocolRecognizer::SendThunderbolt(int a_BlockX, int a_BlockY, int a_BlockZ)
{
	ASSERT(m_Protocol != nullptr);
	m_Protocol->SendThunderbolt(a_BlockX, a_BlockY, a_BlockZ);
}





void cProtocolRecognizer::SendTitleTimes(int a_FadeInTicks, int a_DisplayTicks, int a_FadeOutTicks)
{
	ASSERT(m_Protocol != nullptr);
	m_Protocol->SendTitleTimes(a_FadeInTicks, a_DisplayTicks, a_FadeOutTicks);
}





void cProtocolRecognizer::SendTimeUpdate(Int64 a_WorldAge, Int64 a_TimeOfDay, bool a_DoDaylightCycle)
{
	ASSERT(m_Protocol != nullptr);
	m_Protocol->SendTimeUpdate(a_WorldAge, a_TimeOfDay, a_DoDaylightCycle);
}





void cProtocolRecognizer::SendUnloadChunk(int a_ChunkX, int a_ChunkZ)
{
	ASSERT(m_Protocol != nullptr);
	m_Protocol->SendUnloadChunk(a_ChunkX, a_ChunkZ);
}





void cProtocolRecognizer::SendUpdateBlockEntity(cBlockEntity & a_BlockEntity)
{
	ASSERT(m_Protocol != nullptr);
	m_Protocol->SendUpdateBlockEntity(a_BlockEntity);
}





void cProtocolRecognizer::SendUpdateSign(int a_BlockX, int a_BlockY, int a_BlockZ, const AString & a_Line1, const AString & a_Line2, const AString & a_Line3, const AString & a_Line4)
{
	ASSERT(m_Protocol != nullptr);
	m_Protocol->SendUpdateSign(a_BlockX, a_BlockY, a_BlockZ, a_Line1, a_Line2, a_Line3, a_Line4);
}





void cProtocolRecognizer::SendUseBed(const cEntity & a_Entity, int a_BlockX, int a_BlockY, int a_BlockZ)
{
	ASSERT(m_Protocol != nullptr);
	m_Protocol->SendUseBed(a_Entity, a_BlockX, a_BlockY, a_BlockZ);
}





void cProtocolRecognizer::SendUnlockRecipe(UInt32 a_RecipeID)
{
	ASSERT(m_Protocol != nullptr);
	m_Protocol->SendUnlockRecipe(a_RecipeID);
}





void cProtocolRecognizer::SendInitRecipes(UInt32 a_RecipeID)
{
	ASSERT(m_Protocol != nullptr);
	m_Protocol->SendInitRecipes(a_RecipeID);
}





void cProtocolRecognizer::SendWeather(eWeather a_Weather)
{
	ASSERT(m_Protocol != nullptr);
	m_Protocol->SendWeather(a_Weather);
}





void cProtocolRecognizer::SendWholeInventory(const cWindow & a_Window)
{
	ASSERT(m_Protocol != nullptr);
	m_Protocol->SendWholeInventory(a_Window);
}





void cProtocolRecognizer::SendWindowClose(const cWindow & a_Window)
{
	ASSERT(m_Protocol != nullptr);
	m_Protocol->SendWindowClose(a_Window);
}





void cProtocolRecognizer::SendWindowOpen(const cWindow & a_Window)
{
	ASSERT(m_Protocol != nullptr);
	m_Protocol->SendWindowOpen(a_Window);
}





AString cProtocolRecognizer::GetAuthServerID(void)
{
	ASSERT(m_Protocol != nullptr);
	return m_Protocol->GetAuthServerID();
}





void cProtocolRecognizer::SendData(const char * a_Data, size_t a_Size)
{
	// This is used only when handling the server ping
	m_Client->SendData(a_Data, a_Size);
}*/





void cMultiVersionProtocol::TryRecognizeProtocol(cClientHandle & a_Client, std::string_view & a_Data)
{
	// NOTE: If a new protocol is added or an old one is removed, adjust MCS_CLIENT_VERSIONS and MCS_PROTOCOL_VERSIONS macros in the header file

	// Lengthed protocol, try if it has the entire initial handshake packet:
	UInt32 PacketLen;
	if (!m_Buffer.ReadVarInt(PacketLen))
	{
		// Not enough bytes for the packet length, keep waiting
		return;
	}

	if (!m_Buffer.CanReadBytes(PacketLen))
	{
		// Not enough bytes for the packet, keep waiting
		// More of a sanity check to make sure no one tries anything funny (since ReadXXX can wait for data themselves):
		return;
	}

	m_Protocol = TryRecognizeLengthedProtocol(a_Client, a_Data);
	ASSERT(m_Protocol != nullptr);

	// The protocol has been recognized, initialize it:
	m_Protocol->Initialize(a_Client);
}





std::unique_ptr<cProtocol> cMultiVersionProtocol::TryRecognizeLengthedProtocol(cClientHandle & a_Client, std::string_view & a_Data)
{
	UInt32 PacketType;
	UInt32 ProtocolVersion;
	AString ServerAddress;
	UInt16 ServerPort;
	UInt32 NextState;

	if (!m_Buffer.ReadVarInt(PacketType) || (PacketType != 0x00))
	{
		// Not an initial handshake packet, we don't know how to talk to them
		LOGINFO("Client \"%s\" uses an unsupported protocol (lengthed, initial packet %u)",
			a_Client.GetIPString().c_str(), PacketType
		);

		throw sTriedToJoinWithUnsupportedProtocolException(
			Printf("Your client isn't supported.\nTry connecting with Minecraft " MCS_CLIENT_VERSIONS, ProtocolVersion)
		);
	}

	if (
		!m_Buffer.ReadVarInt(ProtocolVersion) ||
		!m_Buffer.ReadVarUTF8String(ServerAddress) ||
		!m_Buffer.ReadBEUInt16(ServerPort) ||
		!m_Buffer.ReadVarInt(NextState)
	)
	{
		// TryRecognizeProtocol guarantees that we will have as much
		// data to read as the client claims in the protocol length field:
		throw sTriedToJoinWithUnsupportedProtocolException("Incorrect amount of data received - hacked client?");
	}

	// TODO: this should be a protocol property, not ClientHandle:
	a_Client.SetProtocolVersion(ProtocolVersion);

	// The protocol has just been recognized, advance data start
	// to after the handshake and leave the rest to the protocol:
	a_Data = a_Data.substr(m_Buffer.GetUsedSpace() - m_Buffer.GetReadableSpace());

	// We read more than we can handle, purge the rest:
	[[maybe_unused]] const bool Success =
		m_Buffer.SkipRead(m_Buffer.GetReadableSpace());
	ASSERT(Success);

	// All good, eat up the data:
	m_Buffer.CommitRead();

	switch (ProtocolVersion)
	{
		case PROTO_VERSION_1_8_0: return std::make_unique<cProtocol_1_8_0>(&a_Client, ServerAddress, ServerPort, NextState);
		case PROTO_VERSION_1_9_0: return std::make_unique<cProtocol_1_9_0>(&a_Client, ServerAddress, ServerPort, NextState);
		case PROTO_VERSION_1_9_1: return std::make_unique<cProtocol_1_9_1>(&a_Client, ServerAddress, ServerPort, NextState);
		case PROTO_VERSION_1_9_2: return std::make_unique<cProtocol_1_9_2>(&a_Client, ServerAddress, ServerPort, NextState);
		case PROTO_VERSION_1_9_4: return std::make_unique<cProtocol_1_9_4>(&a_Client, ServerAddress, ServerPort, NextState);
		case PROTO_VERSION_1_10_0: return std::make_unique<cProtocol_1_10_0>(&a_Client, ServerAddress, ServerPort, NextState);
		case PROTO_VERSION_1_11_0: return std::make_unique<cProtocol_1_11_0>(&a_Client, ServerAddress, ServerPort, NextState);
		case PROTO_VERSION_1_11_1: return std::make_unique<cProtocol_1_11_1>(&a_Client, ServerAddress, ServerPort, NextState);
		case PROTO_VERSION_1_12: return std::make_unique<cProtocol_1_12>(&a_Client, ServerAddress, ServerPort, NextState);
		case PROTO_VERSION_1_12_1: return std::make_unique<cProtocol_1_12_1>(&a_Client, ServerAddress, ServerPort, NextState);
		case PROTO_VERSION_1_12_2: return std::make_unique<cProtocol_1_12_2>(&a_Client, ServerAddress, ServerPort, NextState);
		case PROTO_VERSION_1_13: return std::make_unique<cProtocol_1_13>(&a_Client, ServerAddress, ServerPort, NextState);
		default:
		{
			LOGD("Client \"%s\" uses an unsupported protocol (lengthed, version %u (0x%x))",
				a_Client.GetIPString().c_str(), ProtocolVersion, ProtocolVersion
			);

			if (NextState != 1)
			{
				throw sTriedToJoinWithUnsupportedProtocolException(
					Printf("Unsupported protocol version %u.\nTry connecting with Minecraft " MCS_CLIENT_VERSIONS, ProtocolVersion)
				);
			}

			throw sUnsupportedButPingableProtocolException();
		}
	}
}





void cMultiVersionProtocol::SendPacket(cClientHandle & a_Client, cByteBuffer & a_OutPacketBuffer)
{
	// Writes out the packet normally.
	UInt32 PacketLen = static_cast<UInt32>(a_OutPacketBuffer.GetUsedSpace());
	cByteBuffer OutPacketLenBuffer(cByteBuffer::GetVarIntSize(PacketLen));

	// Compression doesn't apply to this state, send raw data:
	VERIFY(OutPacketLenBuffer.WriteVarInt32(PacketLen));
	AString LengthData;
	OutPacketLenBuffer.ReadAll(LengthData);
	a_Client.SendData(LengthData.data(), LengthData.size());

	// Send the packet's payload:
	AString PacketData;
	a_OutPacketBuffer.ReadAll(PacketData);
	a_OutPacketBuffer.CommitRead();
	a_Client.SendData(PacketData.data(), PacketData.size());
}





UInt32 cMultiVersionProtocol::GetPacketID(cProtocol::ePacketType a_PacketType)
{
	switch (a_PacketType)
	{
		case cProtocol::ePacketType::pktDisconnectDuringLogin: return 0x00;
		case cProtocol::ePacketType::pktStatusResponse:        return 0x00;
		case cProtocol::ePacketType::pktPingResponse:          return 0x01;
		default:
		{
			ASSERT(!"GetPacketID() called for an unhandled packet");
			return 0;
		}
	}
}





void cMultiVersionProtocol::HandlePacketStatusRequest(cClientHandle & a_Client, cByteBuffer & a_Out)
{
	cServer * Server = cRoot::Get()->GetServer();
	AString ServerDescription = Server->GetDescription();
	auto NumPlayers = static_cast<signed>(Server->GetNumPlayers());
	auto MaxPlayers = static_cast<signed>(Server->GetMaxPlayers());
	AString Favicon = Server->GetFaviconData();
	cRoot::Get()->GetPluginManager()->CallHookServerPing(a_Client, ServerDescription, NumPlayers, MaxPlayers, Favicon);

	// Version:
	Json::Value Version;
	Version["name"] = "Cuberite " MCS_CLIENT_VERSIONS;
	Version["protocol"] = MCS_LATEST_PROTOCOL_VERSION;

	// Players:
	Json::Value Players;
	Players["online"] = NumPlayers;
	Players["max"] = MaxPlayers;
	// TODO: Add "sample"

	// Description:
	Json::Value Description;
	Description["text"] = ServerDescription.c_str();

	// Create the response:
	Json::Value ResponseValue;
	ResponseValue["version"] = Version;
	ResponseValue["players"] = Players;
	ResponseValue["description"] = Description;
	if (!Favicon.empty())
	{
		ResponseValue["favicon"] = Printf("data:image/png;base64,%s", Favicon.c_str());
	}

	AString Response = JsonUtils::WriteFastString(ResponseValue);

	VERIFY(a_Out.WriteVarInt32(GetPacketID(cProtocol::ePacketType::pktStatusResponse)));
	VERIFY(a_Out.WriteVarUTF8String(Response));
}





void cMultiVersionProtocol::HandlePacketStatusPing(cClientHandle & a_Client, cByteBuffer & a_Out)
{
	Int64 Timestamp;
	if (!m_Buffer.ReadBEInt64(Timestamp))
	{
		return;
	}

	VERIFY(a_Out.WriteVarInt32(GetPacketID(cProtocol::ePacketType::pktPingResponse)));
	VERIFY(a_Out.WriteBEInt64(Timestamp));
}
