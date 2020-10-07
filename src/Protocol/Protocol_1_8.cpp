
// Protocol_1_8.cpp

/*
Implements the 1.8 protocol classes:
	- cProtocol_1_8_0
		- release 1.8 protocol (#47)
*/

#include "Globals.h"
#include "Protocol_1_8.h"
#include "main.h"
#include "../mbedTLS++/Sha1Checksum.h"
#include "Packetizer.h"

#include "../ClientHandle.h"
#include "../Root.h"
#include "../Server.h"
#include "../World.h"
#include "../EffectID.h"
#include "../StringCompression.h"
#include "../CompositeChat.h"
#include "../UUID.h"
#include "../World.h"
#include "../JsonUtils.h"

#include "../WorldStorage/FastNBT.h"
#include "../WorldStorage/EnchantmentSerializer.h"

#include "../Entities/ExpOrb.h"
#include "../Entities/Minecart.h"
#include "../Entities/FallingBlock.h"
#include "../Entities/Floater.h"
#include "../Entities/Painting.h"
#include "../Entities/Pickup.h"
#include "../Entities/Player.h"
#include "../Entities/ItemFrame.h"
#include "../Entities/ArrowEntity.h"
#include "../Entities/FireworkEntity.h"

#include "../Mobs/IncludeAllMonsters.h"
#include "../UI/Window.h"
#include "../UI/HorseWindow.h"

#include "../BlockEntities/BeaconEntity.h"
#include "../BlockEntities/CommandBlockEntity.h"
#include "../BlockEntities/EnchantingTableEntity.h"
#include "../BlockEntities/MobHeadEntity.h"
#include "../BlockEntities/MobSpawnerEntity.h"
#include "../BlockEntities/FlowerPotEntity.h"
#include "../Bindings/PluginManager.h"





#define HANDLE_READ(ByteBuf, Proc, Type, Var) \
	Type Var; \
	do { \
		if (!ByteBuf.Proc(Var))\
		{\
			return;\
		} \
	} while (false)





#define HANDLE_PACKET_READ(ByteBuf, Proc, Type, Var) \
	Type Var; \
	do { \
		{ \
			if (!ByteBuf.Proc(Var)) \
			{ \
				ByteBuf.CheckValid(); \
				return false; \
			} \
			ByteBuf.CheckValid(); \
		} \
	} while (false)





const int MAX_ENC_LEN = 512;  // Maximum size of the encrypted message; should be 128, but who knows...
const uLongf MAX_COMPRESSED_PACKET_LEN = 200 KiB;  // Maximum size of compressed packets.
static const UInt32 CompressionThreshold = 256;  // After how large a packet should we compress it.





////////////////////////////////////////////////////////////////////////////////
// cProtocol_1_8_0:

cProtocol_1_8_0::cProtocol_1_8_0(cClientHandle * a_Client, const AString & a_ServerAddress, State a_State) :
	Super(a_Client),
	m_State(a_State),
	m_ServerAddress(a_ServerAddress),
	m_IsEncrypted(false)
{
	AStringVector Params;
	SplitZeroTerminatedStrings(a_ServerAddress, Params);

	if (Params.size() >= 2)
	{
		m_ServerAddress = Params[0];

		if (Params[1] == "FML")
		{
			LOGD("Forge client connected!");
			m_Client->SetIsForgeClient();
		}
		else if (Params.size() == 4)
		{
			if (cRoot::Get()->GetServer()->ShouldAllowBungeeCord())
			{
				// BungeeCord handling:
				// If BC is setup with ip_forward == true, it sends additional data in the login packet's ServerAddress field:
				// hostname\00ip-address\00uuid\00profile-properties-as-json

				LOGD("Player at %s connected via BungeeCord", Params[1].c_str());

				m_Client->SetIPString(Params[1]);

				cUUID UUID;
				UUID.FromString(Params[2]);
				m_Client->SetUUID(UUID);

				Json::Value root;
				if (!JsonUtils::ParseString(Params[3], root))
				{
					LOGERROR("Unable to parse player properties: '%s'", Params[3]);
				}
				else
				{
					m_Client->SetProperties(root);
				}
			}
			else
			{
				LOG("BungeeCord is disabled, but client sent additional data, set AllowBungeeCord=1 if you want to allow it");
			}
		}
		else
		{
			LOG("Unknown additional data sent in server address (BungeeCord/FML?): %zu parameters", Params.size());
			// TODO: support FML + BungeeCord? (what parameters does it send in that case?) https://github.com/SpigotMC/BungeeCord/issues/899
		}
	}

	// Create the comm log file, if so requested:
	if (g_ShouldLogCommIn || g_ShouldLogCommOut)
	{
		static int sCounter = 0;
		cFile::CreateFolder("CommLogs");
		AString IP(a_Client->GetIPString());
		ReplaceString(IP, ":", "_");
		AString FileName = Printf("CommLogs/%x_%d__%s.log",
			static_cast<unsigned>(time(nullptr)),
			sCounter++,
			IP.c_str()
		);
		if (!m_CommLogFile.Open(FileName, cFile::fmWrite))
		{
			LOG("Cannot log communication to file, the log file \"%s\" cannot be opened for writing.", FileName.c_str());
		}
	}
}





void cProtocol_1_8_0::DataReceived(cByteBuffer & a_Buffer, const char * a_Data, size_t a_Size)
{
	if (m_IsEncrypted)
	{
		Byte Decrypted[512];
		while (a_Size > 0)
		{
			size_t NumBytes = (a_Size > sizeof(Decrypted)) ? sizeof(Decrypted) : a_Size;
			m_Decryptor.ProcessData(Decrypted, reinterpret_cast<const Byte *>(a_Data), NumBytes);
			AddReceivedData(a_Buffer, reinterpret_cast<const char *>(Decrypted), NumBytes);
			a_Size -= NumBytes;
			a_Data += NumBytes;
		}
	}
	else
	{
		AddReceivedData(a_Buffer, a_Data, a_Size);
	}
}





void cProtocol_1_8_0::SendAttachEntity(const cEntity & a_Entity, const cEntity & a_Vehicle)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktAttachEntity);
	Pkt.WriteBEUInt32(a_Entity.GetUniqueID());
	Pkt.WriteBEUInt32(a_Vehicle.GetUniqueID());
	Pkt.WriteBool(false);
}





void cProtocol_1_8_0::SendBlockAction(int a_BlockX, int a_BlockY, int a_BlockZ, char a_Byte1, char a_Byte2, BLOCKTYPE a_BlockType)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktBlockAction);
	Pkt.WriteXYZPosition64(a_BlockX, a_BlockY, a_BlockZ);
	Pkt.WriteBEInt8(a_Byte1);
	Pkt.WriteBEInt8(a_Byte2);
	Pkt.WriteVarInt32(a_BlockType);
}





void cProtocol_1_8_0::SendBlockBreakAnim(UInt32 a_EntityID, int a_BlockX, int a_BlockY, int a_BlockZ, char a_Stage)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktBlockBreakAnim);
	Pkt.WriteVarInt32(a_EntityID);
	Pkt.WriteXYZPosition64(a_BlockX, a_BlockY, a_BlockZ);
	Pkt.WriteBEInt8(a_Stage);
}





void cProtocol_1_8_0::SendBlockChange(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktBlockChange);
	Pkt.WriteXYZPosition64(a_BlockX, a_BlockY, a_BlockZ);
	Pkt.WriteVarInt32((static_cast<UInt32>(a_BlockType) << 4) | (static_cast<UInt32>(a_BlockMeta) & 15));
}





void cProtocol_1_8_0::SendBlockChanges(int a_ChunkX, int a_ChunkZ, const sSetBlockVector & a_Changes)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktBlockChanges);
	Pkt.WriteBEInt32(a_ChunkX);
	Pkt.WriteBEInt32(a_ChunkZ);
	Pkt.WriteVarInt32(static_cast<UInt32>(a_Changes.size()));
	for (sSetBlockVector::const_iterator itr = a_Changes.begin(), end = a_Changes.end(); itr != end; ++itr)
	{
		Int16 Coords = static_cast<Int16>(itr->m_RelY | (itr->m_RelZ << 8) | (itr->m_RelX << 12));
		Pkt.WriteBEInt16(Coords);
		Pkt.WriteVarInt32(static_cast<UInt32>(itr->m_BlockType & 0xFFF) << 4 | (itr->m_BlockMeta & 0xF));
	}  // for itr - a_Changes[]
}





void cProtocol_1_8_0::SendCameraSetTo(const cEntity & a_Entity)
{
	cPacketizer Pkt(*this, pktCameraSetTo);
	Pkt.WriteVarInt32(a_Entity.GetUniqueID());
}





void cProtocol_1_8_0::SendChat(const AString & a_Message, eChatType a_Type)
{
	ASSERT(m_State == 3);  // In game mode?

	SendChatRaw(Printf("{\"text\":\"%s\"}", EscapeString(a_Message).c_str()), a_Type);
}





void cProtocol_1_8_0::SendChat(const cCompositeChat & a_Message, eChatType a_Type, bool a_ShouldUseChatPrefixes)
{
	ASSERT(m_State == 3);  // In game mode?

	SendChatRaw(a_Message.CreateJsonString(a_ShouldUseChatPrefixes), a_Type);
}





void cProtocol_1_8_0::SendChatRaw(const AString & a_MessageRaw, eChatType a_Type)
{
	ASSERT(m_State == 3);  // In game mode?

	// Send the json string to the client:
	cPacketizer Pkt(*this, pktChatRaw);
	Pkt.WriteString(a_MessageRaw);
	Pkt.WriteBEInt8(a_Type);
}





void cProtocol_1_8_0::SendChunkData(const std::string_view a_ChunkData)
{
	ASSERT(m_State == 3);  // In game mode?

	cCSLock Lock(m_CSPacket);
	SendData(a_ChunkData.data(), a_ChunkData.size());
}





void cProtocol_1_8_0::SendCollectEntity(const cEntity & a_Collected, const cEntity & a_Collector, unsigned a_Count)
{
	UNUSED(a_Count);
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktCollectEntity);
	Pkt.WriteVarInt32(a_Collected.GetUniqueID());
	Pkt.WriteVarInt32(a_Collector.GetUniqueID());
}





void cProtocol_1_8_0::SendDestroyEntity(const cEntity & a_Entity)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktDestroyEntity);
	Pkt.WriteVarInt32(1);
	Pkt.WriteVarInt32(a_Entity.GetUniqueID());
}





void cProtocol_1_8_0::SendDetachEntity(const cEntity & a_Entity, const cEntity & a_PreviousVehicle)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktAttachEntity);
	Pkt.WriteBEUInt32(a_Entity.GetUniqueID());
	Pkt.WriteBEUInt32(0);
	Pkt.WriteBool(false);
}





void cProtocol_1_8_0::SendDisconnect(const AString & a_Reason)
{
	switch (m_State)
	{
		case State::Login:
		{
			cPacketizer Pkt(*this, pktDisconnectDuringLogin);
			Pkt.WriteString(Printf("{\"text\":\"%s\"}", EscapeString(a_Reason).c_str()));
			break;
		}
		case State::Game:
		{
			cPacketizer Pkt(*this, pktDisconnectDuringGame);
			Pkt.WriteString(Printf("{\"text\":\"%s\"}", EscapeString(a_Reason).c_str()));
			break;
		}
		default:
		{
			FLOGERROR(
				"Tried to send disconnect in invalid game state {0}",
				static_cast<int>(m_State)
			);
		}
	}
}





void cProtocol_1_8_0::SendEditSign(int a_BlockX, int a_BlockY, int a_BlockZ)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktEditSign);
	Pkt.WriteXYZPosition64(a_BlockX, a_BlockY, a_BlockZ);
}





void cProtocol_1_8_0::SendEntityAnimation(const cEntity & a_Entity, char a_Animation)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktEntityAnimation);
	Pkt.WriteVarInt32(a_Entity.GetUniqueID());
	Pkt.WriteBEInt8(a_Animation);
}





void cProtocol_1_8_0::SendEntityEffect(const cEntity & a_Entity, int a_EffectID, int a_Amplifier, int a_Duration)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktEntityEffect);
	Pkt.WriteVarInt32(a_Entity.GetUniqueID());
	Pkt.WriteBEUInt8(static_cast<UInt8>(a_EffectID));
	Pkt.WriteBEUInt8(static_cast<UInt8>(a_Amplifier));
	Pkt.WriteVarInt32(static_cast<UInt32>(a_Duration));
	Pkt.WriteBool(false);  // Hide particles
}





void cProtocol_1_8_0::SendEntityEquipment(const cEntity & a_Entity, short a_SlotNum, const cItem & a_Item)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktEntityEquipment);
	Pkt.WriteVarInt32(a_Entity.GetUniqueID());
	Pkt.WriteBEInt16(a_SlotNum);
	WriteItem(Pkt, a_Item);
}





void cProtocol_1_8_0::SendEntityHeadLook(const cEntity & a_Entity)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktEntityHeadLook);
	Pkt.WriteVarInt32(a_Entity.GetUniqueID());
	Pkt.WriteByteAngle(a_Entity.GetHeadYaw());
}





void cProtocol_1_8_0::SendEntityLook(const cEntity & a_Entity)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktEntityLook);
	Pkt.WriteVarInt32(a_Entity.GetUniqueID());
	Pkt.WriteByteAngle(a_Entity.GetYaw());
	Pkt.WriteByteAngle(a_Entity.GetPitch());
	Pkt.WriteBool(a_Entity.IsOnGround());
}





void cProtocol_1_8_0::SendEntityMetadata(const cEntity & a_Entity)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktEntityMeta);
	Pkt.WriteVarInt32(a_Entity.GetUniqueID());
	WriteEntityMetadata(Pkt, a_Entity);
	Pkt.WriteBEUInt8(0x7f);  // The termination byte
}





void cProtocol_1_8_0::SendEntityPosition(const cEntity & a_Entity)
{
	ASSERT(m_State == 3);  // In game mode?

	const auto Delta = (a_Entity.GetPosition() - a_Entity.GetLastSentPosition()) * 32;

	// Limitations of a byte
	static const auto Max = std::numeric_limits<Int8>::max();

	if ((std::abs(Delta.x) <= Max) && (std::abs(Delta.y) <= Max) && (std::abs(Delta.z) <= Max))
	{
		const auto Move = static_cast<Vector3<Int8>>(Delta);

		// Difference within limitations, use a relative move packet
		if (a_Entity.IsOrientationDirty())
		{
			cPacketizer Pkt(*this, pktEntityRelMoveLook);
			Pkt.WriteVarInt32(a_Entity.GetUniqueID());
			Pkt.WriteBEInt8(Move.x);
			Pkt.WriteBEInt8(Move.y);
			Pkt.WriteBEInt8(Move.z);
			Pkt.WriteByteAngle(a_Entity.GetYaw());
			Pkt.WriteByteAngle(a_Entity.GetPitch());
			Pkt.WriteBool(a_Entity.IsOnGround());
		}
		else
		{
			cPacketizer Pkt(*this, pktEntityRelMove);
			Pkt.WriteVarInt32(a_Entity.GetUniqueID());
			Pkt.WriteBEInt8(Move.x);
			Pkt.WriteBEInt8(Move.y);
			Pkt.WriteBEInt8(Move.z);
			Pkt.WriteBool(a_Entity.IsOnGround());
		}

		return;
	}

	// Too big a movement, do a teleport
	SendEntityTeleport(a_Entity);
}





void cProtocol_1_8_0::SendEntityProperties(const cEntity & a_Entity)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktEntityProperties);
	Pkt.WriteVarInt32(a_Entity.GetUniqueID());
	WriteEntityProperties(Pkt, a_Entity);
}





void cProtocol_1_8_0::SendEntityStatus(const cEntity & a_Entity, char a_Status)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktEntityStatus);
	Pkt.WriteBEUInt32(a_Entity.GetUniqueID());
	Pkt.WriteBEInt8(a_Status);
}





void cProtocol_1_8_0::SendEntityVelocity(const cEntity & a_Entity)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktEntityVelocity);
	Pkt.WriteVarInt32(a_Entity.GetUniqueID());
	// 400 = 8000 / 20 ... Conversion from our speed in m / s to 8000 m / tick
	Pkt.WriteBEInt16(static_cast<Int16>(a_Entity.GetSpeedX() * 400));
	Pkt.WriteBEInt16(static_cast<Int16>(a_Entity.GetSpeedY() * 400));
	Pkt.WriteBEInt16(static_cast<Int16>(a_Entity.GetSpeedZ() * 400));
}





void cProtocol_1_8_0::SendExperience(void)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktExperience);
	cPlayer * Player = m_Client->GetPlayer();
	Pkt.WriteBEFloat(Player->GetXpPercentage());
	Pkt.WriteVarInt32(static_cast<UInt32>(Player->GetXpLevel()));
	Pkt.WriteVarInt32(static_cast<UInt32>(Player->GetCurrentXp()));
}





void cProtocol_1_8_0::SendExperienceOrb(const cExpOrb & a_ExpOrb)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktSpawnExperienceOrb);
	Pkt.WriteVarInt32(a_ExpOrb.GetUniqueID());
	Pkt.WriteFPInt(a_ExpOrb.GetPosX());
	Pkt.WriteFPInt(a_ExpOrb.GetPosY());
	Pkt.WriteFPInt(a_ExpOrb.GetPosZ());
	Pkt.WriteBEInt16(static_cast<Int16>(a_ExpOrb.GetReward()));
}





void cProtocol_1_8_0::SendExplosion(const Vector3f a_Position, const float a_Power)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktExplosion);
	Pkt.WriteBEFloat(a_Position.x);
	Pkt.WriteBEFloat(a_Position.y);
	Pkt.WriteBEFloat(a_Position.z);
	Pkt.WriteBEFloat(a_Power);
	Pkt.WriteBEUInt32(0);
	Pkt.WriteBEFloat(0);
	Pkt.WriteBEFloat(0);
	Pkt.WriteBEFloat(0);
}





void cProtocol_1_8_0::SendGameMode(eGameMode a_GameMode)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktGameMode);
	Pkt.WriteBEUInt8(3);  // Reason: Change game mode
	Pkt.WriteBEFloat(static_cast<float>(a_GameMode));  // The protocol really represents the value with a float!
}





void cProtocol_1_8_0::SendHealth(void)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktUpdateHealth);
	cPlayer * Player = m_Client->GetPlayer();
	Pkt.WriteBEFloat(static_cast<float>(Player->GetHealth()));
	Pkt.WriteVarInt32(static_cast<UInt32>(Player->GetFoodLevel()));
	Pkt.WriteBEFloat(static_cast<float>(Player->GetFoodSaturationLevel()));
}





void cProtocol_1_8_0::SendHeldItemChange(int a_ItemIndex)
{
	ASSERT((a_ItemIndex >= 0) && (a_ItemIndex <= 8));  // Valid check

	cPacketizer Pkt(*this, pktHeldItemChange);
	cPlayer * Player = m_Client->GetPlayer();
	Pkt.WriteBEInt8(static_cast<Int8>(Player->GetInventory().GetEquippedSlotNum()));
}





void cProtocol_1_8_0::SendHideTitle(void)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktTitle);
	Pkt.WriteVarInt32(3);  // Hide title
}





void cProtocol_1_8_0::SendInventorySlot(char a_WindowID, short a_SlotNum, const cItem & a_Item)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktInventorySlot);
	Pkt.WriteBEInt8(a_WindowID);
	Pkt.WriteBEInt16(a_SlotNum);
	WriteItem(Pkt, a_Item);
}





void cProtocol_1_8_0::SendKeepAlive(UInt32 a_PingID)
{
	// Drop the packet if the protocol is not in the Game state yet (caused a client crash):
	if (m_State != 3)
	{
		LOGWARNING("Trying to send a KeepAlive packet to a player who's not yet fully logged in (%d). The protocol class prevented the packet.", m_State);
		return;
	}

	cPacketizer Pkt(*this, pktKeepAlive);
	Pkt.WriteVarInt32(a_PingID);
}





void cProtocol_1_8_0::SendLeashEntity(const cEntity & a_Entity, const cEntity & a_EntityLeashedTo)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktLeashEntity);
	Pkt.WriteBEUInt32(a_Entity.GetUniqueID());
	Pkt.WriteBEUInt32(a_EntityLeashedTo.GetUniqueID());
	Pkt.WriteBool(true);
}





void cProtocol_1_8_0::SendUnleashEntity(const cEntity & a_Entity)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktLeashEntity);
	Pkt.WriteBEUInt32(a_Entity.GetUniqueID());
	Pkt.WriteBEInt32(-1);
	Pkt.WriteBool(true);
}





void cProtocol_1_8_0::SendLogin(const cPlayer & a_Player, const cWorld & a_World)
{
	// Send the Join Game packet:
	{
		cServer * Server = cRoot::Get()->GetServer();
		cPacketizer Pkt(*this, pktJoinGame);
		Pkt.WriteBEUInt32(a_Player.GetUniqueID());
		Pkt.WriteBEUInt8(static_cast<UInt8>(a_Player.GetEffectiveGameMode()) | (Server->IsHardcore() ? 0x08 : 0));  // Hardcore flag bit 4
		Pkt.WriteBEInt8(static_cast<Int8>(a_World.GetDimension()));
		Pkt.WriteBEUInt8(2);  // TODO: Difficulty (set to Normal)
		Pkt.WriteBEUInt8(static_cast<UInt8>(Clamp<size_t>(Server->GetMaxPlayers(), 0, 255)));
		Pkt.WriteString("default");  // Level type - wtf?
		Pkt.WriteBool(false);  // Reduced Debug Info - wtf?
	}

	// Send the spawn position:
	{
		cPacketizer Pkt(*this, pktSpawnPosition);
		Pkt.WriteXYZPosition64(FloorC(a_World.GetSpawnX()), FloorC(a_World.GetSpawnY()), FloorC(a_World.GetSpawnZ()));
	}

	// Send the server difficulty:
	{
		cPacketizer Pkt(*this, pktDifficulty);
		Pkt.WriteBEInt8(1);
	}

	// Send player abilities:
	SendPlayerAbilities();
}





void cProtocol_1_8_0::SendLoginSuccess(void)
{
	ASSERT(m_State == 2);  // State: login?

	// Enable compression:
	{
		cPacketizer Pkt(*this, pktStartCompression);
		Pkt.WriteVarInt32(CompressionThreshold);
	}

	m_State = State::Game;

	{
		cPacketizer Pkt(*this, pktLoginSuccess);
		Pkt.WriteString(m_Client->GetUUID().ToLongString());
		Pkt.WriteString(m_Client->GetUsername());
	}
}





void cProtocol_1_8_0::SendPaintingSpawn(const cPainting & a_Painting)
{
	ASSERT(m_State == 3);  // In game mode?
	double PosX = a_Painting.GetPosX();
	double PosY = a_Painting.GetPosY();
	double PosZ = a_Painting.GetPosZ();

	cPacketizer Pkt(*this, pktSpawnPainting);
	Pkt.WriteVarInt32(a_Painting.GetUniqueID());
	Pkt.WriteString(a_Painting.GetName());
	Pkt.WriteXYZPosition64(static_cast<Int32>(PosX), static_cast<Int32>(PosY), static_cast<Int32>(PosZ));
	Pkt.WriteBEInt8(static_cast<Int8>(a_Painting.GetProtocolFacing()));
}





void cProtocol_1_8_0::SendMapData(const cMap & a_Map, int a_DataStartX, int a_DataStartY)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktMapData);
	Pkt.WriteVarInt32(a_Map.GetID());
	Pkt.WriteBEUInt8(static_cast<UInt8>(a_Map.GetScale()));

	Pkt.WriteVarInt32(static_cast<UInt32>(a_Map.GetDecorators().size()));
	for (const auto & Decorator : a_Map.GetDecorators())
	{
		Pkt.WriteBEUInt8(static_cast<Byte>((static_cast<Int32>(Decorator.GetType()) << 4) | (Decorator.GetRot() & 0xF)));
		Pkt.WriteBEUInt8(static_cast<UInt8>(Decorator.GetPixelX()));
		Pkt.WriteBEUInt8(static_cast<UInt8>(Decorator.GetPixelZ()));
	}

	Pkt.WriteBEUInt8(128);
	Pkt.WriteBEUInt8(128);
	Pkt.WriteBEUInt8(static_cast<UInt8>(a_DataStartX));
	Pkt.WriteBEUInt8(static_cast<UInt8>(a_DataStartY));
	Pkt.WriteVarInt32(static_cast<UInt32>(a_Map.GetData().size()));
	for (auto itr = a_Map.GetData().cbegin(); itr != a_Map.GetData().cend(); ++itr)
	{
		Pkt.WriteBEUInt8(*itr);
	}
}





void cProtocol_1_8_0::SendPlayerAbilities(void)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktPlayerAbilities);
	Byte Flags = 0;
	cPlayer * Player = m_Client->GetPlayer();
	if (Player->IsGameModeCreative())
	{
		Flags |= 0x01;
		Flags |= 0x08;  // Godmode, used for creative
	}
	if (Player->IsFlying())
	{
		Flags |= 0x02;
	}
	if (Player->CanFly())
	{
		Flags |= 0x04;
	}
	Pkt.WriteBEUInt8(Flags);
	Pkt.WriteBEFloat(static_cast<float>(0.05 * Player->GetFlyingMaxSpeed()));
	Pkt.WriteBEFloat(static_cast<float>(0.1 * Player->GetNormalMaxSpeed()));
}





void cProtocol_1_8_0::SendParticleEffect(const AString & a_ParticleName, float a_SrcX, float a_SrcY, float a_SrcZ, float a_OffsetX, float a_OffsetY, float a_OffsetZ, float a_ParticleData, int a_ParticleAmount)
{
	ASSERT(m_State == 3);  // In game mode?
	int ParticleID = GetParticleID(a_ParticleName);

	cPacketizer Pkt(*this, pktParticleEffect);
	Pkt.WriteBEInt32(ParticleID);
	Pkt.WriteBool(false);
	Pkt.WriteBEFloat(a_SrcX);
	Pkt.WriteBEFloat(a_SrcY);
	Pkt.WriteBEFloat(a_SrcZ);
	Pkt.WriteBEFloat(a_OffsetX);
	Pkt.WriteBEFloat(a_OffsetY);
	Pkt.WriteBEFloat(a_OffsetZ);
	Pkt.WriteBEFloat(a_ParticleData);
	Pkt.WriteBEInt32(a_ParticleAmount);
}





void cProtocol_1_8_0::SendParticleEffect(const AString & a_ParticleName, Vector3f a_Src, Vector3f a_Offset, float a_ParticleData, int a_ParticleAmount, std::array<int, 2> a_Data)
{
	ASSERT(m_State == 3);  // In game mode?
	int ParticleID = GetParticleID(a_ParticleName);

	cPacketizer Pkt(*this, pktParticleEffect);
	Pkt.WriteBEInt32(ParticleID);
	Pkt.WriteBool(false);
	Pkt.WriteBEFloat(a_Src.x);
	Pkt.WriteBEFloat(a_Src.y);
	Pkt.WriteBEFloat(a_Src.z);
	Pkt.WriteBEFloat(a_Offset.x);
	Pkt.WriteBEFloat(a_Offset.y);
	Pkt.WriteBEFloat(a_Offset.z);
	Pkt.WriteBEFloat(a_ParticleData);
	Pkt.WriteBEInt32(a_ParticleAmount);
	switch (ParticleID)
	{
		// iconcrack
		case 36:
		{
			Pkt.WriteVarInt32(static_cast<UInt32>(a_Data[0]));
			Pkt.WriteVarInt32(static_cast<UInt32>(a_Data[1]));
			break;
		}
		// blockcrack
		// blockdust
		case 37:
		case 38:
		{
			Pkt.WriteVarInt32(static_cast<UInt32>(a_Data[0]));
			break;
		}
		default:
		{
			break;
		}
	}
}





void cProtocol_1_8_0::SendPlayerListAddPlayer(const cPlayer & a_Player)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktPlayerList);
	Pkt.WriteVarInt32(0);
	Pkt.WriteVarInt32(1);
	Pkt.WriteUUID(a_Player.GetUUID());
	Pkt.WriteString(a_Player.GetPlayerListName());

	const Json::Value & Properties = a_Player.GetClientHandle()->GetProperties();
	Pkt.WriteVarInt32(Properties.size());
	for (auto & Node : Properties)
	{
		Pkt.WriteString(Node.get("name", "").asString());
		Pkt.WriteString(Node.get("value", "").asString());
		AString Signature = Node.get("signature", "").asString();
		if (Signature.empty())
		{
			Pkt.WriteBool(false);
		}
		else
		{
			Pkt.WriteBool(true);
			Pkt.WriteString(Signature);
		}
	}

	Pkt.WriteVarInt32(static_cast<UInt32>(a_Player.GetEffectiveGameMode()));
	Pkt.WriteVarInt32(static_cast<UInt32>(a_Player.GetClientHandle()->GetPing()));
	Pkt.WriteBool(false);
}





void cProtocol_1_8_0::SendPlayerListRemovePlayer(const cPlayer & a_Player)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktPlayerList);
	Pkt.WriteVarInt32(4);
	Pkt.WriteVarInt32(1);
	Pkt.WriteUUID(a_Player.GetUUID());
}





void cProtocol_1_8_0::SendPlayerListUpdateGameMode(const cPlayer & a_Player)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktPlayerList);
	Pkt.WriteVarInt32(1);
	Pkt.WriteVarInt32(1);
	Pkt.WriteUUID(a_Player.GetUUID());
	Pkt.WriteVarInt32(static_cast<UInt32>(a_Player.GetEffectiveGameMode()));
}





void cProtocol_1_8_0::SendPlayerListUpdatePing(const cPlayer & a_Player)
{
	ASSERT(m_State == 3);  // In game mode?

	auto ClientHandle = a_Player.GetClientHandlePtr();
	if (ClientHandle != nullptr)
	{
		cPacketizer Pkt(*this, pktPlayerList);
		Pkt.WriteVarInt32(2);
		Pkt.WriteVarInt32(1);
		Pkt.WriteUUID(a_Player.GetUUID());
		Pkt.WriteVarInt32(static_cast<UInt32>(ClientHandle->GetPing()));
	}
}





void cProtocol_1_8_0::SendPlayerListUpdateDisplayName(const cPlayer & a_Player, const AString & a_CustomName)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktPlayerList);
	Pkt.WriteVarInt32(3);
	Pkt.WriteVarInt32(1);
	Pkt.WriteUUID(a_Player.GetUUID());

	if (a_CustomName.empty())
	{
		Pkt.WriteBool(false);
	}
	else
	{
		Pkt.WriteBool(true);
		Pkt.WriteString(Printf("{\"text\":\"%s\"}", a_CustomName.c_str()));
	}
}





void cProtocol_1_8_0::SendPlayerMaxSpeed(void)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktEntityProperties);
	cPlayer * Player = m_Client->GetPlayer();
	Pkt.WriteVarInt32(Player->GetUniqueID());
	Pkt.WriteBEInt32(1);  // Count
	Pkt.WriteString("generic.movementSpeed");
	// The default game speed is 0.1, multiply that value by the relative speed:
	Pkt.WriteBEDouble(0.1 * Player->GetNormalMaxSpeed());
	if (Player->IsSprinting())
	{
		Pkt.WriteVarInt32(1);  // Modifier count
		Pkt.WriteBEUInt64(0x662a6b8dda3e4c1c);
		Pkt.WriteBEUInt64(0x881396ea6097278d);  // UUID of the modifier
		Pkt.WriteBEDouble(Player->GetSprintingMaxSpeed() - Player->GetNormalMaxSpeed());
		Pkt.WriteBEUInt8(2);
	}
	else
	{
		Pkt.WriteVarInt32(0);  // Modifier count
	}
}





void cProtocol_1_8_0::SendPlayerMoveLook(void)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktPlayerMoveLook);
	cPlayer * Player = m_Client->GetPlayer();
	Pkt.WriteBEDouble(Player->GetPosX());
	Pkt.WriteBEDouble(Player->GetPosY());
	Pkt.WriteBEDouble(Player->GetPosZ());
	Pkt.WriteBEFloat(static_cast<float>(Player->GetYaw()));
	Pkt.WriteBEFloat(static_cast<float>(Player->GetPitch()));
	Pkt.WriteBEUInt8(0);
}





void cProtocol_1_8_0::SendPlayerPosition(void)
{
	// There is no dedicated packet for this, send the whole thing:
	SendPlayerMoveLook();
}





void cProtocol_1_8_0::SendPlayerSpawn(const cPlayer & a_Player)
{
	// Called to spawn another player for the client
	cPacketizer Pkt(*this, pktSpawnOtherPlayer);
	Pkt.WriteVarInt32(a_Player.GetUniqueID());
	Pkt.WriteUUID(a_Player.GetUUID());
	Vector3d LastSentPos = a_Player.GetLastSentPosition();
	Pkt.WriteFPInt(LastSentPos.x);
	Pkt.WriteFPInt(LastSentPos.y + 0.001);  // The "+ 0.001" is there because otherwise the player falls through the block they were standing on.
	Pkt.WriteFPInt(LastSentPos.z);
	Pkt.WriteByteAngle(a_Player.GetYaw());
	Pkt.WriteByteAngle(a_Player.GetPitch());
	short ItemType = a_Player.GetEquippedItem().IsEmpty() ? 0 : a_Player.GetEquippedItem().m_ItemType;
	Pkt.WriteBEInt16(ItemType);
	Pkt.WriteBEUInt8((3 << 5) | 6);  // Metadata: float + index 6
	Pkt.WriteBEFloat(static_cast<float>(a_Player.GetHealth()));
	Pkt.WriteBEUInt8((4 << 5 | (2 & 0x1F)) & 0xFF);
	Pkt.WriteString(a_Player.GetName());
	Pkt.WriteBEUInt8(0x7f);  // Metadata: end
}





void cProtocol_1_8_0::SendPluginMessage(const AString & a_Channel, const AString & a_Message)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktPluginMessage);
	Pkt.WriteString(a_Channel);
	Pkt.WriteBuf(a_Message.data(), a_Message.size());
}





void cProtocol_1_8_0::SendRemoveEntityEffect(const cEntity & a_Entity, int a_EffectID)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktRemoveEntityEffect);
	Pkt.WriteVarInt32(a_Entity.GetUniqueID());
	Pkt.WriteBEUInt8(static_cast<UInt8>(a_EffectID));
}





void cProtocol_1_8_0::SendResetTitle(void)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktTitle);
	Pkt.WriteVarInt32(4);  // Reset title
}





void cProtocol_1_8_0::SendResourcePack(const AString & a_ResourcePackUrl)
{
	cPacketizer Pkt(*this, pktResourcePack);

	cSha1Checksum Checksum;
	Checksum.Update(reinterpret_cast<const Byte *>(a_ResourcePackUrl.c_str()), a_ResourcePackUrl.size());
	Byte Digest[20];
	Checksum.Finalize(Digest);
	AString Sha1Output;
	cSha1Checksum::DigestToHex(Digest, Sha1Output);

	Pkt.WriteString(a_ResourcePackUrl);
	Pkt.WriteString(Sha1Output);
}





void cProtocol_1_8_0::SendRespawn(eDimension a_Dimension)
{
	cPacketizer Pkt(*this, pktRespawn);
	cPlayer * Player = m_Client->GetPlayer();
	Pkt.WriteBEInt32(static_cast<Int32>(a_Dimension));
	Pkt.WriteBEUInt8(2);  // TODO: Difficulty (set to Normal)
	Pkt.WriteBEUInt8(static_cast<Byte>(Player->GetEffectiveGameMode()));
	Pkt.WriteString("default");
}





void cProtocol_1_8_0::SendScoreboardObjective(const AString & a_Name, const AString & a_DisplayName, Byte a_Mode)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktScoreboardObjective);
	Pkt.WriteString(a_Name);
	Pkt.WriteBEUInt8(a_Mode);
	if ((a_Mode == 0) || (a_Mode == 2))
	{
		Pkt.WriteString(a_DisplayName);
		Pkt.WriteString("integer");
	}
}





void cProtocol_1_8_0::SendScoreUpdate(const AString & a_Objective, const AString & a_Player, cObjective::Score a_Score, Byte a_Mode)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktUpdateScore);
	Pkt.WriteString(a_Player);
	Pkt.WriteBEUInt8(a_Mode);
	Pkt.WriteString(a_Objective);

	if (a_Mode != 1)
	{
		Pkt.WriteVarInt32(static_cast<UInt32>(a_Score));
	}
}





void cProtocol_1_8_0::SendDisplayObjective(const AString & a_Objective, cScoreboard::eDisplaySlot a_Display)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktDisplayObjective);
	Pkt.WriteBEUInt8(static_cast<UInt8>(a_Display));
	Pkt.WriteString(a_Objective);
}





void cProtocol_1_8_0::SendSetSubTitle(const cCompositeChat & a_SubTitle)
{
	SendSetRawSubTitle(a_SubTitle.CreateJsonString(false));
}





void cProtocol_1_8_0::SendSetRawSubTitle(const AString & a_SubTitle)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktTitle);
	Pkt.WriteVarInt32(1);  // Set subtitle
	Pkt.WriteString(a_SubTitle);
}





void cProtocol_1_8_0::SendSetTitle(const cCompositeChat & a_Title)
{
	SendSetRawTitle(a_Title.CreateJsonString(false));
}





void cProtocol_1_8_0::SendSetRawTitle(const AString & a_Title)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktTitle);
	Pkt.WriteVarInt32(0);  // Set title
	Pkt.WriteString(a_Title);
}





void cProtocol_1_8_0::SendSoundEffect(const AString & a_SoundName, double a_X, double a_Y, double a_Z, float a_Volume, float a_Pitch)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktSoundEffect);
	Pkt.WriteString(a_SoundName);
	Pkt.WriteBEInt32(static_cast<Int32>(a_X * 8.0));
	Pkt.WriteBEInt32(static_cast<Int32>(a_Y * 8.0));
	Pkt.WriteBEInt32(static_cast<Int32>(a_Z * 8.0));
	Pkt.WriteBEFloat(a_Volume);
	Pkt.WriteBEUInt8(static_cast<Byte>(a_Pitch * 63));
}





void cProtocol_1_8_0::SendSoundParticleEffect(const EffectID a_EffectID, int a_SrcX, int a_SrcY, int a_SrcZ, int a_Data)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktSoundParticleEffect);
	Pkt.WriteBEInt32(static_cast<int>(a_EffectID));
	Pkt.WriteXYZPosition64(a_SrcX, a_SrcY, a_SrcZ);
	Pkt.WriteBEInt32(a_Data);
	Pkt.WriteBool(false);
}





void cProtocol_1_8_0::SendSpawnEntity(const cEntity & a_Entity)
{
	Int32 EntityData = /* Default: velocity present flag */ 1;
	const auto EntityType = GetProtocolEntityType(a_Entity);

	if (a_Entity.IsMinecart())
	{
		const auto & Cart = static_cast<const cMinecart &>(a_Entity);
		EntityData = static_cast<Int32>(Cart.GetPayload());
	}
	else if (a_Entity.IsItemFrame())
	{
		const auto & Frame = static_cast<const cItemFrame &>(a_Entity);
		EntityData = static_cast<Int32>(Frame.GetProtocolFacing());
	}
	else if (a_Entity.IsFallingBlock())
	{
		const auto & Block = static_cast<const cFallingBlock &>(a_Entity);
		EntityData = Block.GetBlockType() | (static_cast<Int32>(Block.GetBlockMeta()) << 12);
	}
	else if (a_Entity.IsFloater())
	{
		const auto & Floater = static_cast<const cFloater &>(a_Entity);
		EntityData = static_cast<Int32>(Floater.GetOwnerID());
	}
	else if (a_Entity.IsProjectile())
	{
		using PType = cProjectileEntity::eKind;
		const auto & Projectile = static_cast<const cProjectileEntity &>(a_Entity);

		if (Projectile.GetProjectileKind() == PType::pkArrow)
		{
			const auto & Arrow = static_cast<const cArrowEntity &>(Projectile);
			EntityData = static_cast<Int32>(Arrow.GetCreatorUniqueID() + 1);
		}
	}

	SendEntitySpawn(a_Entity, EntityType, EntityData);
}





void cProtocol_1_8_0::SendSpawnMob(const cMonster & a_Mob)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktSpawnMob);
	Pkt.WriteVarInt32(a_Mob.GetUniqueID());
	Pkt.WriteBEUInt8(static_cast<Byte>(GetProtocolMobType(a_Mob.GetMobType())));
	Vector3d LastSentPos = a_Mob.GetLastSentPosition();
	Pkt.WriteFPInt(LastSentPos.x);
	Pkt.WriteFPInt(LastSentPos.y);
	Pkt.WriteFPInt(LastSentPos.z);
	Pkt.WriteByteAngle(a_Mob.GetHeadYaw());  // Doesn't seem to be used
	Pkt.WriteByteAngle(a_Mob.GetPitch());
	Pkt.WriteByteAngle(a_Mob.GetYaw());
	Pkt.WriteBEInt16(static_cast<Int16>(a_Mob.GetSpeedX() * 400));
	Pkt.WriteBEInt16(static_cast<Int16>(a_Mob.GetSpeedY() * 400));
	Pkt.WriteBEInt16(static_cast<Int16>(a_Mob.GetSpeedZ() * 400));
	WriteEntityMetadata(Pkt, a_Mob);
	Pkt.WriteBEUInt8(0x7f);  // Metadata terminator
}





void cProtocol_1_8_0::SendStatistics(const cStatManager & a_Manager)
{
	ASSERT(m_State == 3);  // In game mode?

	UInt32 Size = 0;
	a_Manager.ForEachStatisticType([&Size](const auto & Store)
	{
		Size += static_cast<UInt32>(Store.size());
	});

	// No need to check Size != 0
	// Assume that the vast majority of the time there's at least one statistic to send

	cPacketizer Pkt(*this, pktStatistics);
	Pkt.WriteVarInt32(Size);

	a_Manager.ForEachStatisticType([&Pkt](const cStatManager::CustomStore & Store)
	{
		for (const auto & Item : Store)
		{
			Pkt.WriteString(GetProtocolStatisticName(Item.first));
			Pkt.WriteVarInt32(static_cast<UInt32>(Item.second));
		}
	});
}





void cProtocol_1_8_0::SendTabCompletionResults(const AStringVector & a_Results)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktTabCompletionResults);
	Pkt.WriteVarInt32(static_cast<UInt32>(a_Results.size()));

	for (AStringVector::const_iterator itr = a_Results.begin(), end = a_Results.end(); itr != end; ++itr)
	{
		Pkt.WriteString(*itr);
	}
}





void cProtocol_1_8_0::SendThunderbolt(int a_BlockX, int a_BlockY, int a_BlockZ)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktSpawnGlobalEntity);
	Pkt.WriteVarInt32(0);  // EntityID = 0, always
	Pkt.WriteBEUInt8(1);  // Type = Thunderbolt
	Pkt.WriteFPInt(a_BlockX);
	Pkt.WriteFPInt(a_BlockY);
	Pkt.WriteFPInt(a_BlockZ);
}





void cProtocol_1_8_0::SendTitleTimes(int a_FadeInTicks, int a_DisplayTicks, int a_FadeOutTicks)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktTitle);
	Pkt.WriteVarInt32(2);  // Set title display times
	Pkt.WriteBEInt32(a_FadeInTicks);
	Pkt.WriteBEInt32(a_DisplayTicks);
	Pkt.WriteBEInt32(a_FadeOutTicks);
}





void cProtocol_1_8_0::SendTimeUpdate(Int64 a_WorldAge, Int64 a_TimeOfDay, bool a_DoDaylightCycle)
{
	ASSERT(m_State == 3);  // In game mode?
	if (!a_DoDaylightCycle)
	{
		// When writing a "-" before the number the client ignores it but it will stop the client-side time expiration.
		a_TimeOfDay = std::min(-a_TimeOfDay, -1LL);
	}

	cPacketizer Pkt(*this, pktTimeUpdate);
	Pkt.WriteBEInt64(a_WorldAge);
	Pkt.WriteBEInt64(a_TimeOfDay);
}





void cProtocol_1_8_0::SendUnloadChunk(int a_ChunkX, int a_ChunkZ)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktUnloadChunk);
	Pkt.WriteBEInt32(a_ChunkX);
	Pkt.WriteBEInt32(a_ChunkZ);
	Pkt.WriteBool(true);
	Pkt.WriteBEInt16(0);  // Primary bitmap
	Pkt.WriteVarInt32(0);  // Data size
}





void cProtocol_1_8_0::SendUpdateBlockEntity(cBlockEntity & a_BlockEntity)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktUpdateBlockEntity);
	Pkt.WriteXYZPosition64(a_BlockEntity.GetPosX(), a_BlockEntity.GetPosY(), a_BlockEntity.GetPosZ());

	Byte Action = 0;
	switch (a_BlockEntity.GetBlockType())
	{
		case E_BLOCK_MOB_SPAWNER:       Action = 1; break;  // Update mob spawner spinny mob thing
		case E_BLOCK_COMMAND_BLOCK:     Action = 2; break;  // Update command block text
		case E_BLOCK_BEACON:            Action = 3; break;  // Update beacon entity
		case E_BLOCK_HEAD:              Action = 4; break;  // Update Mobhead entity
		case E_BLOCK_FLOWER_POT:        Action = 5; break;  // Update flower pot
		case E_BLOCK_BED:               Action = 11; break;  // Update bed color

		case E_BLOCK_ENCHANTMENT_TABLE: Action = 0; break;  // The ones with a action of 0 is just a workaround to send the block entities to a client.
		case E_BLOCK_END_PORTAL:        Action = 0; break;  // Todo: 18.09.2020 - remove this when block entities are transmitted in the ChunkData packet - 12xx12

		default: ASSERT(!"Unhandled or unimplemented BlockEntity update request!"); break;
	}
	Pkt.WriteBEUInt8(Action);

	WriteBlockEntity(Pkt, a_BlockEntity);
}





void cProtocol_1_8_0::SendUpdateSign(int a_BlockX, int a_BlockY, int a_BlockZ, const AString & a_Line1, const AString & a_Line2, const AString & a_Line3, const AString & a_Line4)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktUpdateSign);
	Pkt.WriteXYZPosition64(a_BlockX, a_BlockY, a_BlockZ);

	AString Lines[] = { a_Line1, a_Line2, a_Line3, a_Line4 };
	for (size_t i = 0; i < ARRAYCOUNT(Lines); i++)
	{
		Json::Value RootValue;
		RootValue["text"] = Lines[i];
		Pkt.WriteString(JsonUtils::WriteFastString(RootValue));
	}
}





void cProtocol_1_8_0::SendUseBed(const cEntity & a_Entity, int a_BlockX, int a_BlockY, int a_BlockZ)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktUseBed);
	Pkt.WriteVarInt32(a_Entity.GetUniqueID());
	Pkt.WriteXYZPosition64(a_BlockX, a_BlockY, a_BlockZ);
}





void cProtocol_1_8_0::SendUnlockRecipe(UInt32 a_RecipeID)
{
	// Client doesn't support this feature
	return;
}





void cProtocol_1_8_0::SendInitRecipes(UInt32 a_RecipeID)
{
	// Client doesn't support this feature
	return;
}





void cProtocol_1_8_0::SendWeather(eWeather a_Weather)
{
	ASSERT(m_State == 3);  // In game mode?

	{
		cPacketizer Pkt(*this, pktWeather);
		Pkt.WriteBEUInt8((a_Weather == wSunny) ? 1 : 2);  // End rain / begin rain
		Pkt.WriteBEFloat(0);  // Unused for weather
	}

	// TODO: Fade effect, somehow
}





void cProtocol_1_8_0::SendWholeInventory(const cWindow & a_Window)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktWindowItems);
	Pkt.WriteBEUInt8(static_cast<UInt8>(a_Window.GetWindowID()));
	Pkt.WriteBEInt16(static_cast<Int16>(a_Window.GetNumSlots()));
	cItems Slots;
	a_Window.GetSlots(*(m_Client->GetPlayer()), Slots);
	for (cItems::const_iterator itr = Slots.begin(), end = Slots.end(); itr != end; ++itr)
	{
		WriteItem(Pkt, *itr);
	}  // for itr - Slots[]
}





void cProtocol_1_8_0::SendWindowClose(const cWindow & a_Window)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktWindowClose);
	Pkt.WriteBEUInt8(static_cast<UInt8>(a_Window.GetWindowID()));
}





void cProtocol_1_8_0::SendWindowOpen(const cWindow & a_Window)
{
	ASSERT(m_State == 3);  // In game mode?

	if (a_Window.GetWindowType() < 0)
	{
		// Do not send this packet for player inventory windows
		return;
	}

	cPacketizer Pkt(*this, pktWindowOpen);
	Pkt.WriteBEUInt8(static_cast<UInt8>(a_Window.GetWindowID()));
	Pkt.WriteString(a_Window.GetWindowTypeName());
	Pkt.WriteString(Printf("{\"text\":\"%s\"}", a_Window.GetWindowTitle().c_str()));

	switch (a_Window.GetWindowType())
	{
		case cWindow::wtWorkbench:
		case cWindow::wtEnchantment:
		case cWindow::wtAnvil:
		{
			Pkt.WriteBEUInt8(0);
			break;
		}
		default:
		{
			Pkt.WriteBEUInt8(static_cast<UInt8>(a_Window.GetNumNonInventorySlots()));
			break;
		}
	}

	if (a_Window.GetWindowType() == cWindow::wtAnimalChest)
	{
		UInt32 HorseID = static_cast<const cHorseWindow &>(a_Window).GetHorseID();
		Pkt.WriteBEInt32(static_cast<Int32>(HorseID));
	}
}





void cProtocol_1_8_0::SendWindowProperty(const cWindow & a_Window, size_t a_Property, short a_Value)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktWindowProperty);
	Pkt.WriteBEUInt8(static_cast<UInt8>(a_Window.GetWindowID()));
	Pkt.WriteBEInt16(static_cast<Int16>(a_Property));
	Pkt.WriteBEInt16(a_Value);
}





bool cProtocol_1_8_0::CompressPacket(const AString & a_Packet, AString & a_CompressedData)
{
	const auto UncompressedSize = static_cast<size_t>(a_Packet.size());

	if (UncompressedSize < CompressionThreshold)
	{
		/* Size doesn't reach threshold, not worth compressing.

		--------------- Packet format ----------------
		|--- Header ---------------------------------|
		| PacketSize: Size of all fields below       |
		| DataSize: Zero, means below not compressed |
		|--- Body -----------------------------------|
		| a_Packet: copy of uncompressed data        |
		----------------------------------------------
		*/
		const UInt32 DataSize = 0;
		const auto PacketSize = static_cast<UInt32>(
			cByteBuffer::GetVarIntSize(DataSize) + UncompressedSize);

		cByteBuffer LengthHeaderBuffer(
			cByteBuffer::GetVarIntSize(PacketSize) +
			cByteBuffer::GetVarIntSize(DataSize)
		);

		LengthHeaderBuffer.WriteVarInt32(PacketSize);
		LengthHeaderBuffer.WriteVarInt32(DataSize);

		AString LengthData;
		LengthHeaderBuffer.ReadAll(LengthData);

		a_CompressedData.reserve(LengthData.size() + UncompressedSize);
		a_CompressedData.assign(LengthData.data(), LengthData.size());
		a_CompressedData.append(a_Packet);

		return true;
	}

	/* Definitely worth compressing.

	--------------- Packet format ----------------
	|--- Header ---------------------------------|
	| PacketSize: Size of all fields below       |
	| DataSize: Size of uncompressed a_Packet    |
	|--- Body -----------------------------------|
	| CompressedData: compressed a_Packet        |
	----------------------------------------------
	*/

	// Compress the data:
	char CompressedData[MAX_COMPRESSED_PACKET_LEN];

	uLongf CompressedSize = compressBound(static_cast<uLongf>(a_Packet.size()));
	if (CompressedSize >= MAX_COMPRESSED_PACKET_LEN)
	{
		ASSERT(!"Too high packet size.");
		return false;
	}

	if (
		compress2(
			reinterpret_cast<Bytef *>(CompressedData), &CompressedSize,
			reinterpret_cast<const Bytef *>(a_Packet.data()), static_cast<uLongf>(a_Packet.size()), Z_DEFAULT_COMPRESSION
		) != Z_OK
	)
	{
		return false;
	}

	const UInt32 DataSize = static_cast<UInt32>(UncompressedSize);
	const auto PacketSize = static_cast<UInt32>(
		cByteBuffer::GetVarIntSize(DataSize) + CompressedSize);

	cByteBuffer LengthHeaderBuffer(
		cByteBuffer::GetVarIntSize(PacketSize) +
		cByteBuffer::GetVarIntSize(DataSize)
	);

	LengthHeaderBuffer.WriteVarInt32(PacketSize);
	LengthHeaderBuffer.WriteVarInt32(DataSize);

	AString LengthData;
	LengthHeaderBuffer.ReadAll(LengthData);

	a_CompressedData.reserve(LengthData.size() + CompressedSize);
	a_CompressedData.assign(LengthData.data(), LengthData.size());
	a_CompressedData.append(CompressedData, CompressedSize);

	return true;
}





int cProtocol_1_8_0::GetParticleID(const AString & a_ParticleName)
{
	static const std::unordered_map<AString, int> ParticleMap
	{
		// Initialize the ParticleMap:
		{ "explode",          0 },
		{ "largeexplode",     1 },
		{ "hugeexplosion",    2 },
		{ "fireworksspark",   3 },
		{ "bubble",           4 },
		{ "splash",           5 },
		{ "wake",             6 },
		{ "suspended",        7 },
		{ "depthsuspend",     8 },
		{ "crit",             9 },
		{ "magiccrit",        10 },
		{ "smoke",            11 },
		{ "largesmoke",       12 },
		{ "spell",            13 },
		{ "instantspell",     14 },
		{ "mobspell",         15 },
		{ "mobspellambient",  16 },
		{ "witchmagic",       17 },
		{ "dripwater",        18 },
		{ "driplava",         19 },
		{ "angryvillager",    20 },
		{ "happyvillager",    21 },
		{ "townaura",         22 },
		{ "note",             23 },
		{ "portal",           24 },
		{ "enchantmenttable", 25 },
		{ "flame",            26 },
		{ "lava",             27 },
		{ "footstep",         28 },
		{ "cloud",            29 },
		{ "reddust",          30 },
		{ "snowballpoof",     31 },
		{ "snowshovel",       32 },
		{ "slime",            33 },
		{ "heart",            34 },
		{ "barrier",          35 },
		{ "iconcrack",        36 },
		{ "blockcrack",       37 },
		{ "blockdust",        38 },
		{ "droplet",          39 },
		{ "take",             40 },
		{ "mobappearance",    41 },
		{ "dragonbreath",     42 },
		{ "endrod",           43 },
		{ "damageindicator",  44 },
		{ "sweepattack",      45 },
		{ "fallingdust",      46 },
		{ "totem",            47 },
		{ "spit",             48 }
	};

	const auto ParticleName = StrToLower(a_ParticleName);
	const auto FindResult = ParticleMap.find(ParticleName);
	if (FindResult == ParticleMap.end())
	{
		LOGWARNING("Unknown particle: %s", a_ParticleName.c_str());
		ASSERT(!"Unknown particle");
		return 0;
	}

	return FindResult->second;
}





UInt32 cProtocol_1_8_0::GetProtocolMobType(eMonsterType a_MobType)
{
	switch (a_MobType)
	{
		// Map invalid type to Giant for easy debugging (if this ever spawns, something has gone very wrong)
		case mtInvalidType:           return 53;
		case mtBat:                   return 65;
		case mtBlaze:                 return 61;
		case mtCaveSpider:            return 59;
		case mtChicken:               return 93;
		case mtCow:                   return 92;
		case mtCreeper:               return 50;
		case mtEnderDragon:           return 63;
		case mtEnderman:              return 58;
		case mtGhast:                 return 56;
		case mtGiant:                 return 53;
		case mtGuardian:              return 68;
		case mtHorse:                 return 100;
		case mtIronGolem:             return 99;
		case mtMagmaCube:             return 62;
		case mtMooshroom:             return 96;
		case mtOcelot:                return 98;
		case mtPig:                   return 90;
		case mtRabbit:                return 101;
		case mtSheep:                 return 91;
		case mtSilverfish:            return 60;
		case mtSkeleton:              return 51;
		case mtSlime:                 return 55;
		case mtSnowGolem:             return 97;
		case mtSpider:                return 52;
		case mtSquid:                 return 94;
		case mtVillager:              return 120;
		case mtWitch:                 return 66;
		case mtWither:                return 64;
		case mtWitherSkeleton:        return 51;
		case mtWolf:                  return 95;
		case mtZombie:                return 54;
		case mtZombiePigman:          return 57;
		case mtZombieVillager:        return 27;
	}
	UNREACHABLE("Unsupported mob type");
}





void cProtocol_1_8_0::AddReceivedData(cByteBuffer & a_Buffer, const char * a_Data, size_t a_Size)
{
	// Write the incoming data into the comm log file:
	if (g_ShouldLogCommIn && m_CommLogFile.IsOpen())
	{
		if (a_Buffer.GetReadableSpace() > 0)
		{
			AString AllData;
			size_t OldReadableSpace = a_Buffer.GetReadableSpace();
			a_Buffer.ReadAll(AllData);
			a_Buffer.ResetRead();
			a_Buffer.SkipRead(a_Buffer.GetReadableSpace() - OldReadableSpace);
			ASSERT(a_Buffer.GetReadableSpace() == OldReadableSpace);
			AString Hex;
			CreateHexDump(Hex, AllData.data(), AllData.size(), 16);
			m_CommLogFile.Printf("Incoming data, %zu (0x%zx) unparsed bytes already present in buffer:\n%s\n",
				AllData.size(), AllData.size(), Hex.c_str()
			);
		}
		AString Hex;
		CreateHexDump(Hex, a_Data, a_Size, 16);
		m_CommLogFile.Printf("Incoming data: %u (0x%x) bytes: \n%s\n",
			static_cast<unsigned>(a_Size), static_cast<unsigned>(a_Size), Hex.c_str()
		);
		m_CommLogFile.Flush();
	}

	if (!a_Buffer.Write(a_Data, a_Size))
	{
		// Too much data in the incoming queue, report to caller:
		m_Client->PacketBufferFull();
		return;
	}

	// Handle all complete packets:
	for (;;)
	{
		UInt32 PacketLen;
		if (!a_Buffer.ReadVarInt(PacketLen))
		{
			// Not enough data
			a_Buffer.ResetRead();
			break;
		}
		if (!a_Buffer.CanReadBytes(PacketLen))
		{
			// The full packet hasn't been received yet
			a_Buffer.ResetRead();
			break;
		}

		// Check packet for compression:
		UInt32 UncompressedSize = 0;
		AString UncompressedData;
		if (m_State == 3)
		{
			UInt32 NumBytesRead = static_cast<UInt32>(a_Buffer.GetReadableSpace());

			if (!a_Buffer.ReadVarInt(UncompressedSize))
			{
				m_Client->Kick("Compression packet incomplete");
				return;
			}

			NumBytesRead -= static_cast<UInt32>(a_Buffer.GetReadableSpace());  // How many bytes has the UncompressedSize taken up?
			ASSERT(PacketLen > NumBytesRead);
			PacketLen -= NumBytesRead;

			if (UncompressedSize > 0)
			{
				// Decompress the data:
				AString CompressedData;
				VERIFY(a_Buffer.ReadString(CompressedData, PacketLen));
				if (InflateString(CompressedData.data(), PacketLen, UncompressedData) != Z_OK)
				{
					m_Client->Kick("Compression failure");
					return;
				}
				PacketLen = static_cast<UInt32>(UncompressedData.size());
				if (PacketLen != UncompressedSize)
				{
					m_Client->Kick("Wrong uncompressed packet size given");
					return;
				}
			}
		}

		// Move the packet payload to a separate cByteBuffer, bb:
		cByteBuffer bb(PacketLen + 1);
		if (UncompressedSize == 0)
		{
			// No compression was used, move directly
			VERIFY(a_Buffer.ReadToByteBuffer(bb, static_cast<size_t>(PacketLen)));
		}
		else
		{
			// Compression was used, move the uncompressed data:
			VERIFY(bb.Write(UncompressedData.data(), UncompressedData.size()));
		}
		a_Buffer.CommitRead();

		UInt32 PacketType;
		if (!bb.ReadVarInt(PacketType))
		{
			// Not enough data
			break;
		}

		// Write one NUL extra, so that we can detect over-reads
		bb.Write("\0", 1);

		// Log the packet info into the comm log file:
		if (g_ShouldLogCommIn && m_CommLogFile.IsOpen())
		{
			AString PacketData;
			bb.ReadAll(PacketData);
			bb.ResetRead();
			bb.ReadVarInt(PacketType);  // We have already read the packet type once, it will be there again
			ASSERT(PacketData.size() > 0);  // We have written an extra NUL, so there had to be at least one byte read
			PacketData.resize(PacketData.size() - 1);
			AString PacketDataHex;
			CreateHexDump(PacketDataHex, PacketData.data(), PacketData.size(), 16);
			m_CommLogFile.Printf("Next incoming packet is type %u (0x%x), length %u (0x%x) at state %d. Payload:\n%s\n",
				PacketType, PacketType, PacketLen, PacketLen, m_State, PacketDataHex.c_str()
			);
		}

		if (!HandlePacket(bb, PacketType))
		{
			// Unknown packet, already been reported, but without the length. Log the length here:
			LOGWARNING("Unhandled packet: type 0x%x, state %d, length %u", PacketType, m_State, PacketLen);

			#ifdef _DEBUG
				// Dump the packet contents into the log:
				bb.ResetRead();
				AString Packet;
				bb.ReadAll(Packet);
				Packet.resize(Packet.size() - 1);  // Drop the final NUL pushed there for over-read detection
				AString Out;
				CreateHexDump(Out, Packet.data(), Packet.size(), 24);
				LOGD("Packet contents:\n%s", Out.c_str());
			#endif  // _DEBUG

			// Put a message in the comm log:
			if (g_ShouldLogCommIn && m_CommLogFile.IsOpen())
			{
				m_CommLogFile.Printf("^^^^^^ Unhandled packet ^^^^^^\n\n\n");
			}

			return;
		}

		// The packet should have 1 byte left in the buffer - the NUL we had added
		if (bb.GetReadableSpace() != 1)
		{
			// Read more or less than packet length, report as error
			LOGWARNING("Protocol 1.8: Wrong number of bytes read for packet 0x%x, state %d. Read %zu bytes, packet contained %u bytes",
				PacketType, m_State, bb.GetUsedSpace() - bb.GetReadableSpace(), PacketLen
			);

			// Put a message in the comm log:
			if (g_ShouldLogCommIn && m_CommLogFile.IsOpen())
			{
				m_CommLogFile.Printf("^^^^^^ Wrong number of bytes read for this packet (exp %d left, got %zu left) ^^^^^^\n\n\n",
					1, bb.GetReadableSpace()
				);
				m_CommLogFile.Flush();
			}

			ASSERT(!"Read wrong number of bytes!");
			m_Client->PacketError(PacketType);
		}
	}  // for (ever)

	// Log any leftover bytes into the logfile:
	if (g_ShouldLogCommIn && (a_Buffer.GetReadableSpace() > 0) && m_CommLogFile.IsOpen())
	{
		AString AllData;
		size_t OldReadableSpace = a_Buffer.GetReadableSpace();
		a_Buffer.ReadAll(AllData);
		a_Buffer.ResetRead();
		a_Buffer.SkipRead(a_Buffer.GetReadableSpace() - OldReadableSpace);
		ASSERT(a_Buffer.GetReadableSpace() == OldReadableSpace);
		AString Hex;
		CreateHexDump(Hex, AllData.data(), AllData.size(), 16);
		m_CommLogFile.Printf("There are %zu (0x%zx) bytes of non-parse-able data left in the buffer:\n%s",
			a_Buffer.GetReadableSpace(), a_Buffer.GetReadableSpace(), Hex.c_str()
		);
		m_CommLogFile.Flush();
	}
}





UInt32 cProtocol_1_8_0::GetPacketID(ePacketType a_PacketType)
{
	switch (a_PacketType)
	{
		case pktAttachEntity:          return 0x1b;
		case pktBlockAction:           return 0x24;
		case pktBlockBreakAnim:        return 0x25;
		case pktBlockChange:           return 0x23;
		case pktBlockChanges:          return 0x22;
		case pktCameraSetTo:           return 0x43;
		case pktChatRaw:               return 0x02;
		case pktCollectEntity:         return 0x0d;
		case pktDestroyEntity:         return 0x13;
		case pktDifficulty:            return 0x41;
		case pktDisconnectDuringGame:  return 0x40;
		case pktDisconnectDuringLogin: return 0x00;
		case pktDisplayObjective:      return 0x3d;
		case pktEditSign:              return 0x36;
		case pktEncryptionRequest:     return 0x01;
		case pktEntityAnimation:       return 0x0b;
		case pktEntityEffect:          return 0x1d;
		case pktEntityEquipment:       return 0x04;
		case pktEntityHeadLook:        return 0x19;
		case pktEntityLook:            return 0x16;
		case pktEntityMeta:            return 0x1c;
		case pktEntityProperties:      return 0x20;
		case pktEntityRelMove:         return 0x15;
		case pktEntityRelMoveLook:     return 0x17;
		case pktEntityStatus:          return 0x1a;
		case pktEntityVelocity:        return 0x12;
		case pktExperience:            return 0x1f;
		case pktExplosion:             return 0x27;
		case pktGameMode:              return 0x2b;
		case pktHeldItemChange:        return 0x09;
		case pktInventorySlot:         return 0x2f;
		case pktJoinGame:              return 0x01;
		case pktKeepAlive:             return 0x00;
		case pktLeashEntity:           return 0x1b;
		case pktLoginSuccess:          return 0x02;
		case pktMapData:               return 0x34;
		case pktParticleEffect:        return 0x2a;
		case pktPingResponse:          return 0x01;
		case pktPlayerAbilities:       return 0x39;
		case pktPlayerList:            return 0x38;
		case pktPlayerMoveLook:        return 0x08;
		case pktPluginMessage:         return 0x3f;
		case pktRemoveEntityEffect:    return 0x1e;
		case pktResourcePack:          return 0x48;
		case pktRespawn:               return 0x07;
		case pktScoreboardObjective:   return 0x3b;
		case pktSoundEffect:           return 0x29;
		case pktSoundParticleEffect:   return 0x28;
		case pktSpawnExperienceOrb:    return 0x11;
		case pktSpawnGlobalEntity:     return 0x2c;
		case pktSpawnMob:              return 0x0f;
		case pktSpawnObject:           return 0x0e;
		case pktSpawnOtherPlayer:      return 0x0c;
		case pktSpawnPainting:         return 0x10;
		case pktSpawnPosition:         return 0x05;
		case pktStartCompression:      return 0x03;
		case pktStatistics:            return 0x37;
		case pktStatusResponse:        return 0x00;
		case pktTabCompletionResults:  return 0x3a;
		case pktTeleportEntity:        return 0x18;
		case pktTimeUpdate:            return 0x03;
		case pktTitle:                 return 0x45;
		case pktUnloadChunk:           return 0x21;
		case pktUpdateBlockEntity:     return 0x35;
		case pktUpdateHealth:          return 0x06;
		case pktUpdateScore:           return 0x3c;
		case pktUpdateSign:            return 0x33;
		case pktUseBed:                return 0x0a;
		case pktWeather:               return 0x2b;
		case pktWindowClose:           return 0x2e;
		case pktWindowItems:           return 0x30;
		case pktWindowOpen:            return 0x2d;
		case pktWindowProperty:        return 0x31;
		default:
		{
			LOG("Unhandled outgoing packet type: %s (0x%02x)", cPacketizer::PacketTypeToStr(a_PacketType), a_PacketType);
			ASSERT(!"Unhandled outgoing packet type");
			return 0;
		}
	}
}





cProtocol::Version cProtocol_1_8_0::GetProtocolVersion()
{
	return Version::v1_8_0;
}





bool cProtocol_1_8_0::HandlePacket(cByteBuffer & a_ByteBuffer, UInt32 a_PacketType)
{
	switch (m_State)
	{
		case State::Status:
		{
			switch (a_PacketType)
			{
				case 0x00: HandlePacketStatusRequest(a_ByteBuffer); return true;
				case 0x01: HandlePacketStatusPing   (a_ByteBuffer); return true;
			}
			break;
		}

		case State::Login:
		{
			switch (a_PacketType)
			{
				case 0x00: HandlePacketLoginStart             (a_ByteBuffer); return true;
				case 0x01: HandlePacketLoginEncryptionResponse(a_ByteBuffer); return true;
			}
			break;
		}

		case State::Game:
		{
			switch (a_PacketType)
			{
				case 0x00: HandlePacketKeepAlive              (a_ByteBuffer); return true;
				case 0x01: HandlePacketChatMessage            (a_ByteBuffer); return true;
				case 0x02: HandlePacketUseEntity              (a_ByteBuffer); return true;
				case 0x03: HandlePacketPlayer                 (a_ByteBuffer); return true;
				case 0x04: HandlePacketPlayerPos              (a_ByteBuffer); return true;
				case 0x05: HandlePacketPlayerLook             (a_ByteBuffer); return true;
				case 0x06: HandlePacketPlayerPosLook          (a_ByteBuffer); return true;
				case 0x07: HandlePacketBlockDig               (a_ByteBuffer); return true;
				case 0x08: HandlePacketBlockPlace             (a_ByteBuffer); return true;
				case 0x09: HandlePacketSlotSelect             (a_ByteBuffer); return true;
				case 0x0a: HandlePacketAnimation              (a_ByteBuffer); return true;
				case 0x0b: HandlePacketEntityAction           (a_ByteBuffer); return true;
				case 0x0c: HandlePacketSteerVehicle           (a_ByteBuffer); return true;
				case 0x0d: HandlePacketWindowClose            (a_ByteBuffer); return true;
				case 0x0e: HandlePacketWindowClick            (a_ByteBuffer); return true;
				case 0x0f:  // Confirm transaction - not used in MCS
				case 0x10: HandlePacketCreativeInventoryAction(a_ByteBuffer); return true;
				case 0x11: HandlePacketEnchantItem            (a_ByteBuffer); return true;
				case 0x12: HandlePacketUpdateSign             (a_ByteBuffer); return true;
				case 0x13: HandlePacketPlayerAbilities        (a_ByteBuffer); return true;
				case 0x14: HandlePacketTabComplete            (a_ByteBuffer); return true;
				case 0x15: HandlePacketClientSettings         (a_ByteBuffer); return true;
				case 0x16: HandlePacketClientStatus           (a_ByteBuffer); return true;
				case 0x17: HandlePacketPluginMessage          (a_ByteBuffer); return true;
				case 0x18: HandlePacketSpectate               (a_ByteBuffer); return true;
				case 0x19: HandlePacketResourcePackStatus     (a_ByteBuffer); return true;
			}
			break;
		}
	}  // switch (m_State)

	// Unknown packet type, report to the ClientHandle:
	m_Client->PacketUnknown(a_PacketType);
	return false;
}





void cProtocol_1_8_0::HandlePacketStatusPing(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadBEInt64, Int64, Timestamp);

	cPacketizer Pkt(*this, pktPingResponse);
	Pkt.WriteBEInt64(Timestamp);
}





void cProtocol_1_8_0::HandlePacketStatusRequest(cByteBuffer & a_ByteBuffer)
{
	cServer * Server = cRoot::Get()->GetServer();
	AString ServerDescription = Server->GetDescription();
	auto NumPlayers = static_cast<signed>(Server->GetNumPlayers());
	auto MaxPlayers = static_cast<signed>(Server->GetMaxPlayers());
	AString Favicon = Server->GetFaviconData();
	cRoot::Get()->GetPluginManager()->CallHookServerPing(*m_Client, ServerDescription, NumPlayers, MaxPlayers, Favicon);

	// Version:
	Json::Value Version;
	const auto ProtocolVersion = GetProtocolVersion();
	Version["name"] = "Cuberite " + cMultiVersionProtocol::GetVersionTextFromInt(ProtocolVersion);
	Version["protocol"] = static_cast<std::underlying_type_t<cProtocol::Version>>(ProtocolVersion);

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
	m_Client->ForgeAugmentServerListPing(ResponseValue);
	if (!Favicon.empty())
	{
		ResponseValue["favicon"] = Printf("data:image/png;base64,%s", Favicon.c_str());
	}

	// Serialize the response into a packet:
	cPacketizer Pkt(*this, pktStatusResponse);
	Pkt.WriteString(JsonUtils::WriteFastString(ResponseValue));
}





void cProtocol_1_8_0::HandlePacketLoginEncryptionResponse(cByteBuffer & a_ByteBuffer)
{
	UInt32 EncKeyLength, EncNonceLength;
	if (!a_ByteBuffer.ReadVarInt(EncKeyLength))
	{
		return;
	}
	AString EncKey;
	if (!a_ByteBuffer.ReadString(EncKey, EncKeyLength))
	{
		return;
	}
	if (!a_ByteBuffer.ReadVarInt(EncNonceLength))
	{
		return;
	}
	AString EncNonce;
	if (!a_ByteBuffer.ReadString(EncNonce, EncNonceLength))
	{
		return;
	}
	if ((EncKeyLength > MAX_ENC_LEN) || (EncNonceLength > MAX_ENC_LEN))
	{
		LOGD("Too long encryption");
		m_Client->Kick("Hacked client");
		return;
	}

	// Decrypt EncNonce using privkey
	cRsaPrivateKey & rsaDecryptor = cRoot::Get()->GetServer()->GetPrivateKey();
	UInt32 DecryptedNonce[MAX_ENC_LEN / sizeof(Int32)];
	int res = rsaDecryptor.Decrypt(reinterpret_cast<const Byte *>(EncNonce.data()), EncNonce.size(), reinterpret_cast<Byte *>(DecryptedNonce), sizeof(DecryptedNonce));
	if (res != 4)
	{
		LOGD("Bad nonce length: got %d, exp %d", res, 4);
		m_Client->Kick("Hacked client");
		return;
	}
	if (ntohl(DecryptedNonce[0]) != static_cast<unsigned>(reinterpret_cast<uintptr_t>(this)))
	{
		LOGD("Bad nonce value");
		m_Client->Kick("Hacked client");
		return;
	}

	// Decrypt the symmetric encryption key using privkey:
	Byte DecryptedKey[MAX_ENC_LEN];
	res = rsaDecryptor.Decrypt(reinterpret_cast<const Byte *>(EncKey.data()), EncKey.size(), DecryptedKey, sizeof(DecryptedKey));
	if (res != 16)
	{
		LOGD("Bad key length");
		m_Client->Kick("Hacked client");
		return;
	}

	StartEncryption(DecryptedKey);
	m_Client->HandleLogin(m_Client->GetUsername());
}





void cProtocol_1_8_0::HandlePacketLoginStart(cByteBuffer & a_ByteBuffer)
{
	AString Username;
	if (!a_ByteBuffer.ReadVarUTF8String(Username))
	{
		m_Client->Kick("Bad username");
		return;
	}

	if (!m_Client->HandleHandshake(Username))
	{
		// The client is not welcome here, they have been sent a Kick packet already
		return;
	}

	cServer * Server = cRoot::Get()->GetServer();
	// If auth is required, then send the encryption request:
	if (Server->ShouldAuthenticate())
	{
		cPacketizer Pkt(*this, pktEncryptionRequest);
		Pkt.WriteString(Server->GetServerID());
		const AString & PubKeyDer = Server->GetPublicKeyDER();
		Pkt.WriteVarInt32(static_cast<UInt32>(PubKeyDer.size()));
		Pkt.WriteBuf(PubKeyDer.data(), PubKeyDer.size());
		Pkt.WriteVarInt32(4);
		Pkt.WriteBEInt32(static_cast<int>(reinterpret_cast<intptr_t>(this)));  // Using 'this' as the cryptographic nonce, so that we don't have to generate one each time :)
		m_Client->SetUsername(Username);
		return;
	}

	m_Client->HandleLogin(Username);
}





void cProtocol_1_8_0::HandlePacketAnimation(cByteBuffer & a_ByteBuffer)
{
	m_Client->HandleAnimation(0);  // Packet exists solely for arm-swing notification
}





void cProtocol_1_8_0::HandlePacketBlockDig(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadBEUInt8, UInt8, Status);

	int BlockX, BlockY, BlockZ;
	if (!a_ByteBuffer.ReadXYZPosition64(BlockX, BlockY, BlockZ))
	{
		return;
	}

	HANDLE_READ(a_ByteBuffer, ReadBEInt8, Int8, Face);
	m_Client->HandleLeftClick(BlockX, BlockY, BlockZ, FaceIntToBlockFace(Face), Status);
}





void cProtocol_1_8_0::HandlePacketBlockPlace(cByteBuffer & a_ByteBuffer)
{
	int BlockX, BlockY, BlockZ;
	if (!a_ByteBuffer.ReadXYZPosition64(BlockX, BlockY, BlockZ))
	{
		return;
	}

	HANDLE_READ(a_ByteBuffer, ReadBEInt8, Int8, Face);

	cItem Item;  // Ignored
	ReadItem(a_ByteBuffer, Item, 3);

	HANDLE_READ(a_ByteBuffer, ReadBEUInt8, UInt8, CursorX);
	HANDLE_READ(a_ByteBuffer, ReadBEUInt8, UInt8, CursorY);
	HANDLE_READ(a_ByteBuffer, ReadBEUInt8, UInt8, CursorZ);
	eBlockFace blockFace = FaceIntToBlockFace(Face);
	if (blockFace == eBlockFace::BLOCK_FACE_NONE)
	{
		m_Client->HandleUseItem(eHand::hMain);
	}
	else
	{
		m_Client->HandleRightClick(BlockX, BlockY, BlockZ, blockFace, CursorX, CursorY, CursorZ, eHand::hMain);
	}
}





void cProtocol_1_8_0::HandlePacketChatMessage(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadVarUTF8String, AString, Message);
	m_Client->HandleChat(Message);
}





void cProtocol_1_8_0::HandlePacketClientSettings(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadVarUTF8String, AString, Locale);
	HANDLE_READ(a_ByteBuffer, ReadBEUInt8,       UInt8,   ViewDistance);
	HANDLE_READ(a_ByteBuffer, ReadBEUInt8,       UInt8,   ChatFlags);
	HANDLE_READ(a_ByteBuffer, ReadBool,          bool,    ChatColors);
	HANDLE_READ(a_ByteBuffer, ReadBEUInt8,       UInt8,   SkinParts);

	m_Client->SetLocale(Locale);
	m_Client->SetViewDistance(ViewDistance);
	m_Client->GetPlayer()->SetSkinParts(SkinParts);
	// TODO: Handle chat flags and chat colors
}





void cProtocol_1_8_0::HandlePacketClientStatus(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadBEUInt8, UInt8, ActionID);
	switch (ActionID)
	{
		case 0:
		{
			// Respawn
			m_Client->HandleRespawn();
			break;
		}
		case 1:
		{
			// Request stats
			const cStatManager & Manager = m_Client->GetPlayer()->GetStatManager();
			SendStatistics(Manager);

			break;
		}
		case 2:
		{
			// Open Inventory achievement
			m_Client->GetPlayer()->AwardAchievement(Statistic::AchOpenInventory);
			break;
		}
	}
}





void cProtocol_1_8_0::HandlePacketCreativeInventoryAction(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadBEInt16, Int16, SlotNum);
	cItem Item;
	if (!ReadItem(a_ByteBuffer, Item))
	{
		return;
	}
	m_Client->HandleCreativeInventory(SlotNum, Item, (SlotNum == -1) ? caLeftClickOutside : caLeftClick);
}





void cProtocol_1_8_0::HandlePacketEntityAction(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadVarInt,  UInt32, PlayerID);
	HANDLE_READ(a_ByteBuffer, ReadBEUInt8, UInt8,  Action);
	HANDLE_READ(a_ByteBuffer, ReadVarInt,  UInt32, JumpBoost);

	switch (Action)
	{
		case 0: m_Client->HandleEntityCrouch(PlayerID, true);     break;  // Crouch
		case 1: m_Client->HandleEntityCrouch(PlayerID, false);    break;  // Uncrouch
		case 2: m_Client->HandleEntityLeaveBed(PlayerID);         break;  // Leave Bed
		case 3: m_Client->HandleEntitySprinting(PlayerID, true);  break;  // Start sprinting
		case 4: m_Client->HandleEntitySprinting(PlayerID, false); break;  // Stop sprinting
		case 6: m_Client->HandleOpenHorseInventory(PlayerID);     break;  // Open horse inventory
	}
}





void cProtocol_1_8_0::HandlePacketKeepAlive(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadVarInt, UInt32, KeepAliveID);
	m_Client->HandleKeepAlive(KeepAliveID);
}





void cProtocol_1_8_0::HandlePacketPlayer(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadBool, bool, IsOnGround);
	// TODO: m_Client->HandlePlayerOnGround(IsOnGround);
}





void cProtocol_1_8_0::HandlePacketPlayerAbilities(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadBEUInt8, UInt8, Flags);
	HANDLE_READ(a_ByteBuffer, ReadBEFloat, float, FlyingSpeed);
	HANDLE_READ(a_ByteBuffer, ReadBEFloat, float, WalkingSpeed);

	// COnvert the bitfield into individual boolean flags:
	bool IsFlying = false, CanFly = false;
	if ((Flags & 2) != 0)
	{
		IsFlying = true;
	}
	if ((Flags & 4) != 0)
	{
		CanFly = true;
	}

	m_Client->HandlePlayerAbilities(CanFly, IsFlying, FlyingSpeed, WalkingSpeed);
}





void cProtocol_1_8_0::HandlePacketPlayerLook(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadBEFloat, float, Yaw);
	HANDLE_READ(a_ByteBuffer, ReadBEFloat, float, Pitch);
	HANDLE_READ(a_ByteBuffer, ReadBool,    bool,  IsOnGround);
	m_Client->HandlePlayerLook(Yaw, Pitch, IsOnGround);
}





void cProtocol_1_8_0::HandlePacketPlayerPos(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadBEDouble, double, PosX);
	HANDLE_READ(a_ByteBuffer, ReadBEDouble, double, PosY);
	HANDLE_READ(a_ByteBuffer, ReadBEDouble, double, PosZ);
	HANDLE_READ(a_ByteBuffer, ReadBool,     bool,   IsOnGround);
	m_Client->HandlePlayerPos(PosX, PosY, PosZ, PosY + (m_Client->GetPlayer()->IsCrouched() ? 1.54 : 1.62), IsOnGround);
}





void cProtocol_1_8_0::HandlePacketPlayerPosLook(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadBEDouble, double, PosX);
	HANDLE_READ(a_ByteBuffer, ReadBEDouble, double, PosY);
	HANDLE_READ(a_ByteBuffer, ReadBEDouble, double, PosZ);
	HANDLE_READ(a_ByteBuffer, ReadBEFloat,  float,  Yaw);
	HANDLE_READ(a_ByteBuffer, ReadBEFloat,  float,  Pitch);
	HANDLE_READ(a_ByteBuffer, ReadBool,     bool,   IsOnGround);
	m_Client->HandlePlayerMoveLook(PosX, PosY, PosZ, PosY + 1.62, Yaw, Pitch, IsOnGround);
}





void cProtocol_1_8_0::HandlePacketPluginMessage(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadVarUTF8String, AString, Channel);

	// If the plugin channel is recognized vanilla, handle it directly:
	if (Channel.substr(0, 3) == "MC|")
	{
		HandleVanillaPluginMessage(a_ByteBuffer, Channel);

		// Skip any unread data (vanilla sometimes sends garbage at the end of a packet; #1692):
		if (a_ByteBuffer.GetReadableSpace() > 1)
		{
			LOGD("Protocol 1.8: Skipping garbage data at the end of a vanilla PluginMessage packet, %u bytes",
				static_cast<unsigned>(a_ByteBuffer.GetReadableSpace() - 1)
			);
			a_ByteBuffer.SkipRead(a_ByteBuffer.GetReadableSpace() - 1);
		}

		return;
	}

	// Read the plugin message and relay to clienthandle:
	AString Data;
	VERIFY(a_ByteBuffer.ReadString(Data, a_ByteBuffer.GetReadableSpace() - 1));  // Always succeeds
	m_Client->HandlePluginMessage(Channel, Data);
}





void cProtocol_1_8_0::HandlePacketResourcePackStatus(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadVarUTF8String, AString, Hash);
	HANDLE_READ(a_ByteBuffer, ReadBEUInt8, UInt8, Status);
}





void cProtocol_1_8_0::HandlePacketSlotSelect(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadBEInt16, Int16, SlotNum);
	m_Client->HandleSlotSelected(SlotNum);
}





void cProtocol_1_8_0::HandlePacketSpectate(cByteBuffer &a_ByteBuffer)
{
	cUUID playerUUID;
	if (!a_ByteBuffer.ReadUUID(playerUUID))
	{
		return;
	}

	m_Client->HandleSpectate(playerUUID);
}





void cProtocol_1_8_0::HandlePacketSteerVehicle(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadBEFloat, float, Sideways);
	HANDLE_READ(a_ByteBuffer, ReadBEFloat, float, Forward);
	HANDLE_READ(a_ByteBuffer, ReadBEUInt8, UInt8, Flags);

	if ((Flags & 0x2) != 0)
	{
		m_Client->HandleUnmount();
	}
	else if ((Flags & 0x1) != 0)
	{
		// jump
	}
	else
	{
		m_Client->HandleSteerVehicle(Forward, Sideways);
	}
}





void cProtocol_1_8_0::HandlePacketTabComplete(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadVarUTF8String, AString, Text);
	HANDLE_READ(a_ByteBuffer, ReadBool,          bool,    HasPosition);

	if (HasPosition)
	{
		HANDLE_READ(a_ByteBuffer, ReadBEInt64, Int64, Position);
	}

	m_Client->HandleTabCompletion(Text);
}





void cProtocol_1_8_0::HandlePacketUpdateSign(cByteBuffer & a_ByteBuffer)
{
	int BlockX, BlockY, BlockZ;
	if (!a_ByteBuffer.ReadXYZPosition64(BlockX, BlockY, BlockZ))
	{
		return;
	}

	AString Lines[4];
	Json::Value root;
	for (int i = 0; i < 4; i++)
	{
		HANDLE_READ(a_ByteBuffer, ReadVarUTF8String, AString, Line);
		if (JsonUtils::ParseString(Line, root) && root.isString())
		{
			Lines[i] = root.asString();
		}
	}

	m_Client->HandleUpdateSign(BlockX, BlockY, BlockZ, Lines[0], Lines[1], Lines[2], Lines[3]);
}





void cProtocol_1_8_0::HandlePacketUseEntity(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadVarInt, UInt32, EntityID);
	HANDLE_READ(a_ByteBuffer, ReadVarInt, UInt32, Type);

	switch (Type)
	{
		case 0:
		{
			m_Client->HandleUseEntity(EntityID, false);
			break;
		}
		case 1:
		{
			m_Client->HandleUseEntity(EntityID, true);
			break;
		}
		case 2:
		{
			HANDLE_READ(a_ByteBuffer, ReadBEFloat, float, TargetX);
			HANDLE_READ(a_ByteBuffer, ReadBEFloat, float, TargetY);
			HANDLE_READ(a_ByteBuffer, ReadBEFloat, float, TargetZ);

			// TODO: Do anything
			break;
		}
		default:
		{
			ASSERT(!"Unhandled use entity type!");
			return;
		}
	}
}





void cProtocol_1_8_0::HandlePacketEnchantItem(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadBEUInt8, UInt8, WindowID);
	HANDLE_READ(a_ByteBuffer, ReadBEUInt8, UInt8, Enchantment);

	m_Client->HandleEnchantItem(WindowID, Enchantment);
}





void cProtocol_1_8_0::HandlePacketWindowClick(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadBEUInt8,  UInt8,  WindowID);
	HANDLE_READ(a_ByteBuffer, ReadBEInt16,  Int16,  SlotNum);
	HANDLE_READ(a_ByteBuffer, ReadBEUInt8,  UInt8,  Button);
	HANDLE_READ(a_ByteBuffer, ReadBEUInt16, UInt16, TransactionID);
	HANDLE_READ(a_ByteBuffer, ReadBEUInt8,  UInt8,  Mode);
	cItem Item;
	ReadItem(a_ByteBuffer, Item);

	/** The slot number that the client uses to indicate "outside the window". */
	static const Int16 SLOT_NUM_OUTSIDE = -999;

	// Convert Button, Mode, SlotNum and HeldItem into eClickAction:
	eClickAction Action;
	switch ((Mode << 8) | Button)
	{
		case 0x0000: Action = (SlotNum != SLOT_NUM_OUTSIDE) ? caLeftClick  : caLeftClickOutside;  break;
		case 0x0001: Action = (SlotNum != SLOT_NUM_OUTSIDE) ? caRightClick : caRightClickOutside; break;
		case 0x0100: Action = caShiftLeftClick;  break;
		case 0x0101: Action = caShiftRightClick; break;
		case 0x0200: Action = caNumber1;         break;
		case 0x0201: Action = caNumber2;         break;
		case 0x0202: Action = caNumber3;         break;
		case 0x0203: Action = caNumber4;         break;
		case 0x0204: Action = caNumber5;         break;
		case 0x0205: Action = caNumber6;         break;
		case 0x0206: Action = caNumber7;         break;
		case 0x0207: Action = caNumber8;         break;
		case 0x0208: Action = caNumber9;         break;
		case 0x0302: Action = caMiddleClick;     break;
		case 0x0400: Action = (SlotNum == SLOT_NUM_OUTSIDE) ? caLeftClickOutsideHoldNothing  : caDropKey;     break;
		case 0x0401: Action = (SlotNum == SLOT_NUM_OUTSIDE) ? caRightClickOutsideHoldNothing : caCtrlDropKey; break;
		case 0x0500: Action = (SlotNum == SLOT_NUM_OUTSIDE) ? caLeftPaintBegin               : caUnknown;     break;
		case 0x0501: Action = (SlotNum != SLOT_NUM_OUTSIDE) ? caLeftPaintProgress            : caUnknown;     break;
		case 0x0502: Action = (SlotNum == SLOT_NUM_OUTSIDE) ? caLeftPaintEnd                 : caUnknown;     break;
		case 0x0504: Action = (SlotNum == SLOT_NUM_OUTSIDE) ? caRightPaintBegin              : caUnknown;     break;
		case 0x0505: Action = (SlotNum != SLOT_NUM_OUTSIDE) ? caRightPaintProgress           : caUnknown;     break;
		case 0x0506: Action = (SlotNum == SLOT_NUM_OUTSIDE) ? caRightPaintEnd                : caUnknown;     break;
		case 0x0508: Action = (SlotNum == SLOT_NUM_OUTSIDE) ? caMiddlePaintBegin             : caUnknown;     break;
		case 0x0509: Action = (SlotNum != SLOT_NUM_OUTSIDE) ? caMiddlePaintProgress          : caUnknown;     break;
		case 0x050a: Action = (SlotNum == SLOT_NUM_OUTSIDE) ? caMiddlePaintEnd               : caUnknown;     break;
		case 0x0600: Action = caDblClick; break;
		default:
		{
			LOGWARNING("Unhandled window click mode / button combination: %d (0x%x)", (Mode << 8) | Button, (Mode << 8) | Button);
			Action = caUnknown;
			break;
		}
	}

	m_Client->HandleWindowClick(WindowID, SlotNum, Action, Item);
}





void cProtocol_1_8_0::HandlePacketWindowClose(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadBEUInt8, UInt8, WindowID);
	m_Client->HandleWindowClose(WindowID);
}





void cProtocol_1_8_0::HandleVanillaPluginMessage(cByteBuffer & a_ByteBuffer, const AString & a_Channel)
{
	if (a_Channel == "MC|AdvCdm")
	{
		HANDLE_READ(a_ByteBuffer, ReadBEUInt8, UInt8, Mode);
		switch (Mode)
		{
			case 0x00:
			{
				HANDLE_READ(a_ByteBuffer, ReadBEInt32, Int32, BlockX);
				HANDLE_READ(a_ByteBuffer, ReadBEInt32, Int32, BlockY);
				HANDLE_READ(a_ByteBuffer, ReadBEInt32, Int32, BlockZ);
				HANDLE_READ(a_ByteBuffer, ReadVarUTF8String, AString, Command);
				m_Client->HandleCommandBlockBlockChange(BlockX, BlockY, BlockZ, Command);
				break;
			}

			default:
			{
				m_Client->SendChat(Printf("Failure setting command block command; unhandled mode %u (0x%02x)", Mode, Mode), mtFailure);
				LOG("Unhandled MC|AdvCdm packet mode.");
				return;
			}
		}  // switch (Mode)
		return;
	}
	else if (a_Channel == "MC|Brand")
	{
		HANDLE_READ(a_ByteBuffer, ReadVarUTF8String, AString, Brand);
		m_Client->SetClientBrand(Brand);
		// Send back our brand, including the length:
		SendPluginMessage("MC|Brand", "\x08""Cuberite");
		return;
	}
	else if (a_Channel == "MC|Beacon")
	{
		HANDLE_READ(a_ByteBuffer, ReadBEInt32, Int32, Effect1);
		HANDLE_READ(a_ByteBuffer, ReadBEInt32, Int32, Effect2);
		m_Client->HandleBeaconSelection(Effect1, Effect2);
		return;
	}
	else if (a_Channel == "MC|ItemName")
	{
		HANDLE_READ(a_ByteBuffer, ReadVarUTF8String, AString, ItemName);
		m_Client->HandleAnvilItemName(ItemName);
		return;
	}
	else if (a_Channel == "MC|TrSel")
	{
		HANDLE_READ(a_ByteBuffer, ReadBEInt32, Int32, SlotNum);
		m_Client->HandleNPCTrade(SlotNum);
		return;
	}
	LOG("Unhandled vanilla plugin channel: \"%s\".", a_Channel.c_str());

	// Read the payload and send it through to the clienthandle:
	AString Message;
	VERIFY(a_ByteBuffer.ReadString(Message, a_ByteBuffer.GetReadableSpace() - 1));
	m_Client->HandlePluginMessage(a_Channel, Message);
}





void cProtocol_1_8_0::SendData(const char * a_Data, size_t a_Size)
{
	if (m_IsEncrypted)
	{
		Byte Encrypted[8192];  // Larger buffer, we may be sending lots of data (chunks)
		while (a_Size > 0)
		{
			size_t NumBytes = (a_Size > sizeof(Encrypted)) ? sizeof(Encrypted) : a_Size;
			m_Encryptor.ProcessData(Encrypted, reinterpret_cast<const Byte *>(a_Data), NumBytes);
			m_Client->SendData(reinterpret_cast<const char *>(Encrypted), NumBytes);
			a_Size -= NumBytes;
			a_Data += NumBytes;
		}
	}
	else
	{
		m_Client->SendData(a_Data, a_Size);
	}
}





bool cProtocol_1_8_0::ReadItem(cByteBuffer & a_ByteBuffer, cItem & a_Item, size_t a_KeepRemainingBytes)
{
	HANDLE_PACKET_READ(a_ByteBuffer, ReadBEInt16, Int16, ItemType);
	if (ItemType == -1)
	{
		// The item is empty, no more data follows
		a_Item.Empty();
		return true;
	}
	a_Item.m_ItemType = ItemType;

	HANDLE_PACKET_READ(a_ByteBuffer, ReadBEInt8,  Int8,  ItemCount);
	HANDLE_PACKET_READ(a_ByteBuffer, ReadBEInt16, Int16, ItemDamage);
	a_Item.m_ItemCount  = ItemCount;
	a_Item.m_ItemDamage = ItemDamage;
	if (ItemCount <= 0)
	{
		a_Item.Empty();
	}

	AString Metadata;
	if (!a_ByteBuffer.ReadString(Metadata, a_ByteBuffer.GetReadableSpace() - a_KeepRemainingBytes - 1) || (Metadata.size() == 0) || (Metadata[0] == 0))
	{
		// No metadata
		return true;
	}

	ParseItemMetadata(a_Item, Metadata);
	return true;
}





void cProtocol_1_8_0::ParseItemMetadata(cItem & a_Item, const AString & a_Metadata)
{
	// Parse into NBT:
	cParsedNBT NBT(a_Metadata.data(), a_Metadata.size());
	if (!NBT.IsValid())
	{
		AString HexDump;
		CreateHexDump(HexDump, a_Metadata.data(), std::max<size_t>(a_Metadata.size(), 1024), 16);
		LOGWARNING("Cannot parse NBT item metadata: %s at (%zu / %zu bytes)\n%s",
			NBT.GetErrorCode().message().c_str(), NBT.GetErrorPos(), a_Metadata.size(), HexDump.c_str()
		);
		return;
	}

	// Load enchantments and custom display names from the NBT data:
	for (int tag = NBT.GetFirstChild(NBT.GetRoot()); tag >= 0; tag = NBT.GetNextSibling(tag))
	{
		AString TagName = NBT.GetName(tag);
		switch (NBT.GetType(tag))
		{
			case TAG_List:
			{
				if ((TagName == "ench") || (TagName == "StoredEnchantments"))  // Enchantments tags
				{
					EnchantmentSerializer::ParseFromNBT(a_Item.m_Enchantments, NBT, tag);
				}
				break;
			}
			case TAG_Compound:
			{
				if (TagName == "display")  // Custom name and lore tag
				{
					for (int displaytag = NBT.GetFirstChild(tag); displaytag >= 0; displaytag = NBT.GetNextSibling(displaytag))
					{
						if ((NBT.GetType(displaytag) == TAG_String) && (NBT.GetName(displaytag) == "Name"))  // Custon name tag
						{
							a_Item.m_CustomName = NBT.GetString(displaytag);
						}
						else if ((NBT.GetType(displaytag) == TAG_List) && (NBT.GetName(displaytag) == "Lore"))  // Lore tag
						{
							for (int loretag = NBT.GetFirstChild(displaytag); loretag >= 0; loretag = NBT.GetNextSibling(loretag))  // Loop through array of strings
							{
								a_Item.m_LoreTable.push_back(NBT.GetString(loretag));
							}
						}
						else if ((NBT.GetType(displaytag) == TAG_Int) && (NBT.GetName(displaytag) == "color"))
						{
							a_Item.m_ItemColor.m_Color = static_cast<unsigned int>(NBT.GetInt(displaytag));
						}
					}
				}
				else if ((TagName == "Fireworks") || (TagName == "Explosion"))
				{
					cFireworkItem::ParseFromNBT(a_Item.m_FireworkItem, NBT, tag, static_cast<ENUM_ITEM_TYPE>(a_Item.m_ItemType));
				}
				break;
			}
			case TAG_Int:
			{
				if (TagName == "RepairCost")
				{
					a_Item.m_RepairCost = NBT.GetInt(tag);
				}
			}
			default: LOGD("Unimplemented NBT data when parsing!"); break;
		}
	}
}





void cProtocol_1_8_0::StartEncryption(const Byte * a_Key)
{
	m_Encryptor.Init(a_Key, a_Key);
	m_Decryptor.Init(a_Key, a_Key);
	m_IsEncrypted = true;

	// Prepare the m_AuthServerID:
	cSha1Checksum Checksum;
	cServer * Server = cRoot::Get()->GetServer();
	const AString & ServerID = Server->GetServerID();
	Checksum.Update(reinterpret_cast<const Byte *>(ServerID.c_str()), ServerID.length());
	Checksum.Update(a_Key, 16);
	Checksum.Update(reinterpret_cast<const Byte *>(Server->GetPublicKeyDER().data()), Server->GetPublicKeyDER().size());
	Byte Digest[20];
	Checksum.Finalize(Digest);
	cSha1Checksum::DigestToJava(Digest, m_AuthServerID);
}





eBlockFace cProtocol_1_8_0::FaceIntToBlockFace(const Int32 a_BlockFace)
{
	// Normalize the blockface values returned from the protocol
	// Anything known gets mapped 1:1, everything else returns BLOCK_FACE_NONE
	switch (a_BlockFace)
	{
		case BLOCK_FACE_XM: return BLOCK_FACE_XM;
		case BLOCK_FACE_XP: return BLOCK_FACE_XP;
		case BLOCK_FACE_YM: return BLOCK_FACE_YM;
		case BLOCK_FACE_YP: return BLOCK_FACE_YP;
		case BLOCK_FACE_ZM: return BLOCK_FACE_ZM;
		case BLOCK_FACE_ZP: return BLOCK_FACE_ZP;
		default: return BLOCK_FACE_NONE;
	}
}





////////////////////////////////////////////////////////////////////////////////
// cProtocol_1_8_0::cPacketizer:

void cProtocol_1_8_0::SendPacket(cPacketizer & a_Pkt)
{
	UInt32 PacketLen = static_cast<UInt32>(m_OutPacketBuffer.GetUsedSpace());
	AString PacketData, CompressedPacket;
	m_OutPacketBuffer.ReadAll(PacketData);
	m_OutPacketBuffer.CommitRead();

	if (m_State == 3)
	{
		// Compress the packet payload:
		if (!cProtocol_1_8_0::CompressPacket(PacketData, CompressedPacket))
		{
			return;
		}

		// Send the packet's payload compressed:
		SendData(CompressedPacket.data(), CompressedPacket.size());
	}
	else
	{
		// Compression doesn't apply to this state, send raw data:
		m_OutPacketLenBuffer.WriteVarInt32(PacketLen);
		AString LengthData;
		m_OutPacketLenBuffer.ReadAll(LengthData);
		SendData(LengthData.data(), LengthData.size());

		// Send the packet's payload directly:
		m_OutPacketLenBuffer.CommitRead();
		SendData(PacketData.data(), PacketData.size());
	}

	// Log the comm into logfile:
	if (g_ShouldLogCommOut && m_CommLogFile.IsOpen())
	{
		AString Hex;
		ASSERT(PacketData.size() > 0);
		CreateHexDump(Hex, PacketData.data(), PacketData.size(), 16);
		m_CommLogFile.Printf("Outgoing packet: type %s (translated to 0x%02x), length %u (0x%04x), state %d. Payload (incl. type):\n%s\n",
			cPacketizer::PacketTypeToStr(a_Pkt.GetPacketType()), GetPacketID(a_Pkt.GetPacketType()),
			PacketLen, PacketLen, m_State, Hex
		);
		/*
		// Useful for debugging a new protocol:
		LOGD("Outgoing packet: type %s (translated to 0x%02x), length %u (0x%04x), state %d. Payload (incl. type):\n%s\n",
			cPacketizer::PacketTypeToStr(a_Pkt.GetPacketType()), GetPacketID(a_Pkt.GetPacketType()),
			PacketLen, PacketLen, m_State, Hex
		);
		//*/
	}
	/*
	// Useful for debugging a new protocol:
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	*/
}





void cProtocol_1_8_0::SendEntitySpawn(const cEntity & a_Entity, const UInt8 a_ObjectType, const Int32 a_ObjectData)
{
	ASSERT(m_State == 3);  // In game mode?

	{
		cPacketizer Pkt(*this, pktSpawnObject);
		Pkt.WriteVarInt32(a_Entity.GetUniqueID());
		Pkt.WriteBEUInt8(a_ObjectType);
		Pkt.WriteFPInt(a_Entity.GetPosX());  // Position appears to be ignored...
		Pkt.WriteFPInt(a_Entity.GetPosY());
		Pkt.WriteFPInt(a_Entity.GetPosY());
		Pkt.WriteByteAngle(a_Entity.GetPitch());
		Pkt.WriteByteAngle(a_Entity.GetYaw());
		Pkt.WriteBEInt32(a_ObjectData);

		if (a_ObjectData != 0)
		{
			Pkt.WriteBEInt16(static_cast<Int16>(a_Entity.GetSpeedX() * 400));
			Pkt.WriteBEInt16(static_cast<Int16>(a_Entity.GetSpeedY() * 400));
			Pkt.WriteBEInt16(static_cast<Int16>(a_Entity.GetSpeedZ() * 400));
		}
	}

	// Otherwise 1.8 clients don't show the entity
	SendEntityTeleport(a_Entity);
}





void cProtocol_1_8_0::WriteItem(cPacketizer & a_Pkt, const cItem & a_Item)
{
	short ItemType = a_Item.m_ItemType;
	ASSERT(ItemType >= -1);  // Check validity of packets in debug runtime
	if (ItemType <= 0)
	{
		// Fix, to make sure no invalid values are sent.
		ItemType = -1;
	}

	if (a_Item.IsEmpty())
	{
		a_Pkt.WriteBEInt16(-1);
		return;
	}

	a_Pkt.WriteBEInt16(ItemType);
	a_Pkt.WriteBEInt8(a_Item.m_ItemCount);
	a_Pkt.WriteBEInt16(a_Item.m_ItemDamage);

	if (a_Item.m_Enchantments.IsEmpty() && a_Item.IsBothNameAndLoreEmpty() && (a_Item.m_ItemType != E_ITEM_FIREWORK_ROCKET) && (a_Item.m_ItemType != E_ITEM_FIREWORK_STAR) && !a_Item.m_ItemColor.IsValid())
	{
		a_Pkt.WriteBEInt8(0);
		return;
	}


	// Send the enchantments and custom names:
	cFastNBTWriter Writer;
	if (a_Item.m_RepairCost != 0)
	{
		Writer.AddInt("RepairCost", a_Item.m_RepairCost);
	}
	if (!a_Item.m_Enchantments.IsEmpty())
	{
		const char * TagName = (a_Item.m_ItemType == E_ITEM_BOOK) ? "StoredEnchantments" : "ench";
		EnchantmentSerializer::WriteToNBTCompound(a_Item.m_Enchantments, Writer, TagName);
	}
	if (!a_Item.IsBothNameAndLoreEmpty() || a_Item.m_ItemColor.IsValid())
	{
		Writer.BeginCompound("display");
		if (a_Item.m_ItemColor.IsValid())
		{
			Writer.AddInt("color", static_cast<Int32>(a_Item.m_ItemColor.m_Color));
		}

		if (!a_Item.IsCustomNameEmpty())
		{
			Writer.AddString("Name", a_Item.m_CustomName);
		}
		if (!a_Item.IsLoreEmpty())
		{
			Writer.BeginList("Lore", TAG_String);

			for (const auto & Line : a_Item.m_LoreTable)
			{
				Writer.AddString("", Line);
			}

			Writer.EndList();
		}
		Writer.EndCompound();
	}
	if ((a_Item.m_ItemType == E_ITEM_FIREWORK_ROCKET) || (a_Item.m_ItemType == E_ITEM_FIREWORK_STAR))
	{
		cFireworkItem::WriteToNBTCompound(a_Item.m_FireworkItem, Writer, static_cast<ENUM_ITEM_TYPE>(a_Item.m_ItemType));
	}
	Writer.Finish();

	AString Result = Writer.GetResult();
	if (Result.size() == 0)
	{
		a_Pkt.WriteBEInt8(0);
		return;
	}
	a_Pkt.WriteBuf(Result.data(), Result.size());
}





void cProtocol_1_8_0::WriteBlockEntity(cPacketizer & a_Pkt, const cBlockEntity & a_BlockEntity)
{
	cFastNBTWriter Writer;

	switch (a_BlockEntity.GetBlockType())
	{
		case E_BLOCK_BEACON:
		{
			auto & BeaconEntity = static_cast<const cBeaconEntity &>(a_BlockEntity);
			Writer.AddInt("x",         BeaconEntity.GetPosX());
			Writer.AddInt("y",         BeaconEntity.GetPosY());
			Writer.AddInt("z",         BeaconEntity.GetPosZ());
			Writer.AddInt("Primary",   BeaconEntity.GetPrimaryEffect());
			Writer.AddInt("Secondary", BeaconEntity.GetSecondaryEffect());
			Writer.AddInt("Levels",    BeaconEntity.GetBeaconLevel());
			Writer.AddString("id", "Beacon");  // "Tile Entity ID" - MC wiki; vanilla server always seems to send this though
			break;
		}

		case E_BLOCK_COMMAND_BLOCK:
		{
			auto & CommandBlockEntity = static_cast<const cCommandBlockEntity &>(a_BlockEntity);
			Writer.AddByte("TrackOutput", 1);  // Neither I nor the MC wiki has any idea about this
			Writer.AddInt("SuccessCount", CommandBlockEntity.GetResult());
			Writer.AddInt("x", CommandBlockEntity.GetPosX());
			Writer.AddInt("y", CommandBlockEntity.GetPosY());
			Writer.AddInt("z", CommandBlockEntity.GetPosZ());
			Writer.AddString("Command", CommandBlockEntity.GetCommand());
			// You can set custom names for windows in Vanilla
			// For a command block, this would be the 'name' prepended to anything it outputs into global chat
			// MCS doesn't have this, so just leave it @ '@'. (geddit?)
			Writer.AddString("CustomName", "@");
			Writer.AddString("id", "Control");  // "Tile Entity ID" - MC wiki; vanilla server always seems to send this though
			if (!CommandBlockEntity.GetLastOutput().empty())
			{
				Writer.AddString("LastOutput", Printf("{\"text\":\"%s\"}", CommandBlockEntity.GetLastOutput().c_str()));
			}
			break;
		}

		case E_BLOCK_ENCHANTMENT_TABLE:
		{
			auto & EnchantingTableEntity = static_cast<const cEnchantingTableEntity &>(a_BlockEntity);
			Writer.AddInt("x", EnchantingTableEntity.GetPosX());
			Writer.AddInt("y", EnchantingTableEntity.GetPosY());
			Writer.AddInt("z", EnchantingTableEntity.GetPosZ());
			if (!EnchantingTableEntity.GetCustomName().empty())
			{
				Writer.AddString("CustomName", EnchantingTableEntity.GetCustomName());
			}
			Writer.AddString("id", "EnchantingTable");
			break;
		}

		case E_BLOCK_END_PORTAL:
		{
			Writer.AddInt("x", a_BlockEntity.GetPosX());
			Writer.AddInt("y", a_BlockEntity.GetPosY());
			Writer.AddInt("z", a_BlockEntity.GetPosZ());
			Writer.AddString("id", "EndPortal");
			break;
		}

		case E_BLOCK_HEAD:
		{
			auto & MobHeadEntity = static_cast<const cMobHeadEntity &>(a_BlockEntity);
			Writer.AddInt("x", MobHeadEntity.GetPosX());
			Writer.AddInt("y", MobHeadEntity.GetPosY());
			Writer.AddInt("z", MobHeadEntity.GetPosZ());
			Writer.AddByte("SkullType", MobHeadEntity.GetType() & 0xFF);
			Writer.AddByte("Rot", MobHeadEntity.GetRotation() & 0xFF);
			Writer.AddString("id", "Skull");  // "Tile Entity ID" - MC wiki; vanilla server always seems to send this though

			// The new Block Entity format for a Mob Head. See: https://minecraft.gamepedia.com/Head#Block_entity
			Writer.BeginCompound("Owner");
				Writer.AddString("Id", MobHeadEntity.GetOwnerUUID().ToShortString());
				Writer.AddString("Name", MobHeadEntity.GetOwnerName());
				Writer.BeginCompound("Properties");
					Writer.BeginList("textures", TAG_Compound);
						Writer.BeginCompound("");
							Writer.AddString("Signature", MobHeadEntity.GetOwnerTextureSignature());
							Writer.AddString("Value", MobHeadEntity.GetOwnerTexture());
						Writer.EndCompound();
					Writer.EndList();
				Writer.EndCompound();
			Writer.EndCompound();
			break;
		}

		case E_BLOCK_FLOWER_POT:
		{
			auto & FlowerPotEntity = static_cast<const cFlowerPotEntity &>(a_BlockEntity);
			Writer.AddInt("x", FlowerPotEntity.GetPosX());
			Writer.AddInt("y", FlowerPotEntity.GetPosY());
			Writer.AddInt("z", FlowerPotEntity.GetPosZ());
			Writer.AddInt("Item", static_cast<Int32>(FlowerPotEntity.GetItem().m_ItemType));
			Writer.AddInt("Data", static_cast<Int32>(FlowerPotEntity.GetItem().m_ItemDamage));
			Writer.AddString("id", "FlowerPot");  // "Tile Entity ID" - MC wiki; vanilla server always seems to send this though
			break;
		}

		case E_BLOCK_MOB_SPAWNER:
		{
			auto & MobSpawnerEntity = static_cast<const cMobSpawnerEntity &>(a_BlockEntity);
			Writer.AddInt("x", MobSpawnerEntity.GetPosX());
			Writer.AddInt("y", MobSpawnerEntity.GetPosY());
			Writer.AddInt("z", MobSpawnerEntity.GetPosZ());
			Writer.AddString("EntityId", cMonster::MobTypeToVanillaName(MobSpawnerEntity.GetEntity()));
			Writer.AddShort("Delay", MobSpawnerEntity.GetSpawnDelay());
			Writer.AddString("id", "MobSpawner");
			break;
		}

		default:
		{
			break;
		}
	}

	Writer.Finish();
	a_Pkt.WriteBuf(Writer.GetResult().data(), Writer.GetResult().size());
}





void cProtocol_1_8_0::WriteEntityMetadata(cPacketizer & a_Pkt, const cEntity & a_Entity)
{
	// Common metadata:
	Byte Flags = 0;
	if (a_Entity.IsOnFire())
	{
		Flags |= 0x01;
	}
	if (a_Entity.IsCrouched())
	{
		Flags |= 0x02;
	}
	if (a_Entity.IsSprinting())
	{
		Flags |= 0x08;
	}
	if (a_Entity.IsRclking())
	{
		Flags |= 0x10;
	}
	if (a_Entity.IsInvisible())
	{
		Flags |= 0x20;
	}
	a_Pkt.WriteBEUInt8(0);  // Byte(0) + index 0
	a_Pkt.WriteBEUInt8(Flags);

	switch (a_Entity.GetEntityType())
	{
		case cEntity::etPlayer:
		{
			auto & Player = static_cast<const cPlayer &>(a_Entity);

			// Player health (not handled since players aren't monsters)
			a_Pkt.WriteBEUInt8(0x66);
			a_Pkt.WriteBEFloat(static_cast<float>(Player.GetHealth()));

			// Skin flags
			a_Pkt.WriteBEUInt8(0x0A);
			a_Pkt.WriteBEUInt8(static_cast<UInt8>(Player.GetSkinParts()));

			break;
		}
		case cEntity::etPickup:
		{
			a_Pkt.WriteBEUInt8((5 << 5) | 10);  // Slot(5) + index 10
			WriteItem(a_Pkt, static_cast<const cPickup &>(a_Entity).GetItem());
			break;
		}
		case cEntity::etMinecart:
		{
			a_Pkt.WriteBEUInt8(0x51);

			// The following expression makes Minecarts shake more with less health or higher damage taken
			// It gets half the maximum health, and takes it away from the current health minus the half health:
			/*
			Health: 5 | 3 - (5 - 3) = 1 (shake power)
			Health: 3 | 3 - (3 - 3) = 3
			Health: 1 | 3 - (1 - 3) = 5
			*/
			auto & Minecart = static_cast<const cMinecart &>(a_Entity);
			a_Pkt.WriteBEInt32(static_cast<Int32>((((a_Entity.GetMaxHealth() / 2) - (a_Entity.GetHealth() - (a_Entity.GetMaxHealth() / 2))) * Minecart.LastDamage()) * 4));
			a_Pkt.WriteBEUInt8(0x52);
			a_Pkt.WriteBEInt32(1);  // Shaking direction, doesn't seem to affect anything
			a_Pkt.WriteBEUInt8(0x73);
			a_Pkt.WriteBEFloat(static_cast<float>(Minecart.LastDamage() + 10));  // Damage taken / shake effect multiplyer

			if (Minecart.GetPayload() == cMinecart::mpNone)
			{
				auto & RideableMinecart = static_cast<const cRideableMinecart &>(Minecart);
				const cItem & MinecartContent = RideableMinecart.GetContent();
				if (!MinecartContent.IsEmpty())
				{
					a_Pkt.WriteBEUInt8(0x54);
					int Content = MinecartContent.m_ItemType;
					Content |= MinecartContent.m_ItemDamage << 8;
					a_Pkt.WriteBEInt32(Content);
					a_Pkt.WriteBEUInt8(0x55);
					a_Pkt.WriteBEInt32(RideableMinecart.GetBlockHeight());
					a_Pkt.WriteBEUInt8(0x56);
					a_Pkt.WriteBEUInt8(1);
				}
			}
			else if (Minecart.GetPayload() == cMinecart::mpFurnace)
			{
				a_Pkt.WriteBEUInt8(0x10);
				a_Pkt.WriteBEUInt8(static_cast<const cMinecartWithFurnace &>(Minecart).IsFueled() ? 1 : 0);
			}
			break;
		}  // case etMinecart

		case cEntity::etProjectile:
		{
			auto & Projectile = static_cast<const cProjectileEntity &>(a_Entity);
			switch (Projectile.GetProjectileKind())
			{
				case cProjectileEntity::pkArrow:
				{
					a_Pkt.WriteBEUInt8(0x10);
					a_Pkt.WriteBEUInt8(static_cast<const cArrowEntity &>(Projectile).IsCritical() ? 1 : 0);
					break;
				}
				case cProjectileEntity::pkFirework:
				{
					a_Pkt.WriteBEUInt8(0xa8);
					WriteItem(a_Pkt, static_cast<const cFireworkEntity &>(Projectile).GetItem());
					break;
				}
				default:
				{
					break;
				}
			}
			break;
		}  // case etProjectile

		case cEntity::etMonster:
		{
			WriteMobMetadata(a_Pkt, static_cast<const cMonster &>(a_Entity));
			break;
		}

		case cEntity::etItemFrame:
		{
			auto & Frame = static_cast<const cItemFrame &>(a_Entity);
			a_Pkt.WriteBEUInt8(0xa8);
			WriteItem(a_Pkt, Frame.GetItem());
			a_Pkt.WriteBEUInt8(0x09);
			a_Pkt.WriteBEUInt8(Frame.GetItemRotation());
			break;
		}  // case etItemFrame

		default:
		{
			break;
		}
	}
}





void cProtocol_1_8_0::WriteMobMetadata(cPacketizer & a_Pkt, const cMonster & a_Mob)
{
	// Living Enitiy Metadata
	if (a_Mob.HasCustomName())
	{
		a_Pkt.WriteBEUInt8(0x82);
		a_Pkt.WriteString(a_Mob.GetCustomName());

		a_Pkt.WriteBEUInt8(0x03);
		a_Pkt.WriteBool(a_Mob.IsCustomNameAlwaysVisible());
	}

	a_Pkt.WriteBEUInt8(0x66);
	a_Pkt.WriteBEFloat(static_cast<float>(a_Mob.GetHealth()));

	switch (a_Mob.GetMobType())
	{
		case mtBat:
		{
			auto & Bat = static_cast<const cBat &>(a_Mob);
			a_Pkt.WriteBEUInt8(0x10);
			a_Pkt.WriteBEUInt8(Bat.IsHanging() ? 1 : 0);
			break;
		}  // case mtBat

		case mtChicken:
		{
			auto & Chicken = static_cast<const cChicken &>(a_Mob);
			a_Pkt.WriteBEUInt8(0x0c);
			a_Pkt.WriteBEInt8(Chicken.IsBaby() ? -1 : (Chicken.IsInLoveCooldown() ? 1 : 0));
			break;
		}  // case mtChicken

		case mtCow:
		{
			auto & Cow = static_cast<const cCow &>(a_Mob);
			a_Pkt.WriteBEUInt8(0x0c);
			a_Pkt.WriteBEInt8(Cow.IsBaby() ? -1 : (Cow.IsInLoveCooldown() ? 1 : 0));
			break;
		}  // case mtCow

		case mtCreeper:
		{
			auto & Creeper = static_cast<const cCreeper &>(a_Mob);
			a_Pkt.WriteBEUInt8(0x10);
			a_Pkt.WriteBEUInt8(Creeper.IsBlowing() ? 1 : 255);
			a_Pkt.WriteBEUInt8(0x11);
			a_Pkt.WriteBEUInt8(Creeper.IsCharged() ? 1 : 0);
			break;
		}  // case mtCreeper

		case mtEnderman:
		{
			auto & Enderman = static_cast<const cEnderman &>(a_Mob);
			a_Pkt.WriteBEUInt8(0x30);
			a_Pkt.WriteBEInt16(static_cast<Byte>(Enderman.GetCarriedBlock()));
			a_Pkt.WriteBEUInt8(0x11);
			a_Pkt.WriteBEUInt8(static_cast<Byte>(Enderman.GetCarriedMeta()));
			a_Pkt.WriteBEUInt8(0x12);
			a_Pkt.WriteBEUInt8(Enderman.IsScreaming() ? 1 : 0);
			break;
		}  // case mtEnderman

		case mtGhast:
		{
			auto & Ghast = static_cast<const cGhast &>(a_Mob);
			a_Pkt.WriteBEUInt8(0x10);
			a_Pkt.WriteBEUInt8(Ghast.IsCharging());
			break;
		}  // case mtGhast

		case mtHorse:
		{
			auto & Horse = static_cast<const cHorse &>(a_Mob);
			int Flags = 0;
			if (Horse.IsTame())
			{
				Flags |= 0x02;
			}
			if (Horse.IsSaddled())
			{
				Flags |= 0x04;
			}
			if (Horse.IsChested())
			{
				Flags |= 0x08;
			}
			if (Horse.IsEating())
			{
				Flags |= 0x20;
			}
			if (Horse.IsRearing())
			{
				Flags |= 0x40;
			}
			if (Horse.IsMthOpen())
			{
				Flags |= 0x80;
			}
			a_Pkt.WriteBEUInt8(0x50);  // Int at index 16
			a_Pkt.WriteBEInt32(Flags);
			a_Pkt.WriteBEUInt8(0x13);  // Byte at index 19
			a_Pkt.WriteBEUInt8(static_cast<UInt8>(Horse.GetHorseType()));
			a_Pkt.WriteBEUInt8(0x54);  // Int at index 20
			int Appearance = 0;
			Appearance = Horse.GetHorseColor();
			Appearance |= Horse.GetHorseStyle() << 8;
			a_Pkt.WriteBEInt32(Appearance);
			a_Pkt.WriteBEUInt8(0x56);  // Int at index 22
			a_Pkt.WriteBEInt32(Horse.GetHorseArmour());
			a_Pkt.WriteBEUInt8(0x0c);
			a_Pkt.WriteBEInt8(Horse.IsBaby() ? -1 : (Horse.IsInLoveCooldown() ? 1 : 0));
			break;
		}  // case mtHorse

		case mtMagmaCube:
		{
			auto & MagmaCube = static_cast<const cMagmaCube &>(a_Mob);
			a_Pkt.WriteBEUInt8(0x10);
			a_Pkt.WriteBEUInt8(static_cast<UInt8>(MagmaCube.GetSize()));
			break;
		}  // case mtMagmaCube

		case mtOcelot:
		{
			auto & Ocelot = static_cast<const cOcelot &>(a_Mob);
			a_Pkt.WriteBEUInt8(0x0c);
			a_Pkt.WriteBEInt8(Ocelot.IsBaby() ? -1 : (Ocelot.IsInLoveCooldown() ? 1 : 0));
			break;
		}  // case mtOcelot

		case mtPig:
		{
			auto & Pig = static_cast<const cPig &>(a_Mob);
			a_Pkt.WriteBEUInt8(0x0c);
			a_Pkt.WriteBEInt8(Pig.IsBaby() ? -1 : (Pig.IsInLoveCooldown() ? 1 : 0));
			a_Pkt.WriteBEUInt8(0x10);
			a_Pkt.WriteBEUInt8(Pig.IsSaddled() ? 1 : 0);
			break;
		}  // case mtPig

		case mtSheep:
		{
			auto & Sheep = static_cast<const cSheep &>(a_Mob);
			a_Pkt.WriteBEUInt8(0x0c);
			a_Pkt.WriteBEInt8(Sheep.IsBaby() ? -1 : (Sheep.IsInLoveCooldown() ? 1 : 0));

			a_Pkt.WriteBEUInt8(0x10);
			Byte SheepMetadata = 0;
			SheepMetadata = static_cast<Byte>(Sheep.GetFurColor());
			if (Sheep.IsSheared())
			{
				SheepMetadata |= 0x10;
			}
			a_Pkt.WriteBEUInt8(SheepMetadata);
			break;
		}  // case mtSheep

		case mtRabbit:
		{
			auto & Rabbit = static_cast<const cRabbit &>(a_Mob);
			a_Pkt.WriteBEUInt8(0x12);
			a_Pkt.WriteBEUInt8(static_cast<UInt8>(Rabbit.GetRabbitType()));
			a_Pkt.WriteBEUInt8(0x0c);
			a_Pkt.WriteBEInt8(Rabbit.IsBaby() ? -1 : (Rabbit.IsInLoveCooldown() ? 1 : 0));
			break;
		}  // case mtRabbit

		case mtSlime:
		{
			auto & Slime = static_cast<const cSlime &>(a_Mob);
			a_Pkt.WriteBEUInt8(0x10);
			a_Pkt.WriteBEUInt8(static_cast<UInt8>(Slime.GetSize()));
			break;
		}  // case mtSlime

		case mtVillager:
		{
			auto & Villager = static_cast<const cVillager &>(a_Mob);
			a_Pkt.WriteBEUInt8(0x50);
			a_Pkt.WriteBEInt32(Villager.GetVilType());
			a_Pkt.WriteBEUInt8(0x0c);
			a_Pkt.WriteBEInt8(Villager.IsBaby() ? -1 : 0);
			break;
		}  // case mtVillager

		case mtWitch:
		{
			auto & Witch = static_cast<const cWitch &>(a_Mob);
			a_Pkt.WriteBEUInt8(0x15);
			a_Pkt.WriteBEUInt8(Witch.IsAngry() ? 1 : 0);
			break;
		}  // case mtWitch

		case mtWither:
		{
			auto & Wither = static_cast<const cWither &>(a_Mob);
			a_Pkt.WriteBEUInt8(0x54);  // Int at index 20
			a_Pkt.WriteBEInt32(static_cast<Int32>(Wither.GetWitherInvulnerableTicks()));
			a_Pkt.WriteBEUInt8(0x66);  // Float at index 6
			a_Pkt.WriteBEFloat(static_cast<float>(a_Mob.GetHealth()));
			break;
		}  // case mtWither

		case mtWitherSkeleton:
		{
			a_Pkt.WriteBEUInt8(0x0d);
			a_Pkt.WriteBEUInt8(1);  // Is wither skeleton
			break;
		}  // case mtWitherSkeleton

		case mtWolf:
		{
			auto & Wolf = static_cast<const cWolf &>(a_Mob);
			Byte WolfStatus = 0;
			if (Wolf.IsSitting())
			{
				WolfStatus |= 0x1;
			}
			if (Wolf.IsAngry())
			{
				WolfStatus |= 0x2;
			}
			if (Wolf.IsTame())
			{
				WolfStatus |= 0x4;
			}
			a_Pkt.WriteBEUInt8(0x10);
			a_Pkt.WriteBEUInt8(WolfStatus);

			a_Pkt.WriteBEUInt8(0x72);
			a_Pkt.WriteBEFloat(static_cast<float>(a_Mob.GetHealth()));
			a_Pkt.WriteBEUInt8(0x13);
			a_Pkt.WriteBEUInt8(Wolf.IsBegging() ? 1 : 0);
			a_Pkt.WriteBEUInt8(0x14);
			a_Pkt.WriteBEUInt8(static_cast<UInt8>(Wolf.GetCollarColor()));
			a_Pkt.WriteBEUInt8(0x0c);
			a_Pkt.WriteBEInt8(Wolf.IsBaby() ? -1 : 0);
			break;
		}  // case mtWolf

		case mtZombie:
		{
			auto & Zombie = static_cast<const cZombie &>(a_Mob);
			a_Pkt.WriteBEUInt8(0x0c);
			a_Pkt.WriteBEInt8(Zombie.IsBaby() ? 1 : -1);
			a_Pkt.WriteBEUInt8(0x0d);
			a_Pkt.WriteBEUInt8(0);
			a_Pkt.WriteBEUInt8(0x0e);
			a_Pkt.WriteBEUInt8(0);
			break;
		}  // case mtZombie

		case mtZombiePigman:
		{
			auto & ZombiePigman = static_cast<const cZombiePigman &>(a_Mob);
			a_Pkt.WriteBEUInt8(0x0c);
			a_Pkt.WriteBEInt8(ZombiePigman.IsBaby() ? 1 : -1);
			break;
		}  // case mtZombiePigman

		case mtZombieVillager:
		{
			auto & ZombieVillager = reinterpret_cast<const cZombieVillager &>(a_Mob);
			a_Pkt.WriteBEUInt8(0x0c);
			a_Pkt.WriteBEInt8(ZombieVillager.IsBaby() ? 1 : -1);
			a_Pkt.WriteBEUInt8(0x0d);
			a_Pkt.WriteBEUInt8(1);
			a_Pkt.WriteBEUInt8(0x0e);
			a_Pkt.WriteBEUInt8((ZombieVillager.ConversionTime() == -1) ? 0 : 1);
			break;
		}  // case mtZombieVillager

		default: break;
	}  // switch (a_Mob.GetType())
}





void cProtocol_1_8_0::WriteEntityProperties(cPacketizer & a_Pkt, const cEntity & a_Entity)
{
	if (!a_Entity.IsMob())
	{
		// No properties for anything else than mobs
		a_Pkt.WriteBEInt32(0);
		return;
	}

	// const cMonster & Mob = (const cMonster &)a_Entity;

	// TODO: Send properties and modifiers based on the mob type

	a_Pkt.WriteBEInt32(0);  // NumProperties
}





void cProtocol_1_8_0::SendEntityTeleport(const cEntity & a_Entity)
{
	cPacketizer Pkt(*this, pktTeleportEntity);
	Pkt.WriteVarInt32(a_Entity.GetUniqueID());
	Pkt.WriteFPInt(a_Entity.GetPosX());
	Pkt.WriteFPInt(a_Entity.GetPosY());
	Pkt.WriteFPInt(a_Entity.GetPosZ());
	Pkt.WriteByteAngle(a_Entity.GetYaw());
	Pkt.WriteByteAngle(a_Entity.GetPitch());
	Pkt.WriteBool(a_Entity.IsOnGround());
}





UInt8 cProtocol_1_8_0::GetProtocolEntityType(const cEntity & a_Entity)
{
	using Type = cEntity::eEntityType;

	switch (a_Entity.GetEntityType())
	{
		case Type::etEnderCrystal: return 51;
		case Type::etPickup: return 2;
		case Type::etFallingBlock: return 70;
		case Type::etMinecart: return 10;
		case Type::etBoat: return 1;
		case Type::etTNT: return 50;
		case Type::etProjectile:
		{
			using PType = cProjectileEntity::eKind;
			const auto & Projectile = static_cast<const cProjectileEntity &>(a_Entity);

			switch (Projectile.GetProjectileKind())
			{
				case PType::pkArrow: return 60;
				case PType::pkSnowball: return 61;
				case PType::pkEgg: return 62;
				case PType::pkGhastFireball: return 63;
				case PType::pkFireCharge: return 64;
				case PType::pkEnderPearl: return 65;
				case PType::pkExpBottle: return 75;
				case PType::pkSplashPotion: return 73;
				case PType::pkFirework: return 76;
				case PType::pkWitherSkull: return 66;
			}
		}
		case Type::etFloater: return 90;
		case Type::etItemFrame: return 71;
		case Type::etLeashKnot: return 77;

		// Non-objects must not be sent
		case Type::etEntity:
		case Type::etPlayer:
		case Type::etMonster:
		case Type::etExpOrb:
		case Type::etPainting: UNREACHABLE("Tried to spawn an unhandled entity");
	}
	UNREACHABLE("Unhandled entity kind");
}





const char * cProtocol_1_8_0::GetProtocolStatisticName(Statistic a_Statistic)
{
	switch (a_Statistic)
	{
		// V1.8 Achievements
		case Statistic::AchOpenInventory:        return "achievement.openInventory";
		case Statistic::AchMineWood:             return "achievement.mineWood";
		case Statistic::AchBuildWorkBench:       return "achievement.buildWorkBench";
		case Statistic::AchBuildPickaxe:         return "achievement.buildPickaxe";
		case Statistic::AchBuildFurnace:         return "achievement.buildFurnace";
		case Statistic::AchAcquireIron:          return "achievement.acquireIron";
		case Statistic::AchBuildHoe:             return "achievement.buildHoe";
		case Statistic::AchMakeBread:            return "achievement.makeBread";
		case Statistic::AchBakeCake:             return "achievement.bakeCake";
		case Statistic::AchBuildBetterPickaxe:   return "achievement.buildBetterPickaxe";
		case Statistic::AchCookFish:             return "achievement.cookFish";
		case Statistic::AchOnARail:              return "achievement.onARail";
		case Statistic::AchBuildSword:           return "achievement.buildSword";
		case Statistic::AchKillEnemy:            return "achievement.killEnemy";
		case Statistic::AchKillCow:              return "achievement.killCow";
		case Statistic::AchFlyPig:               return "achievement.flyPig";
		case Statistic::AchSnipeSkeleton:        return "achievement.snipeSkeleton";
		case Statistic::AchDiamonds:             return "achievement.diamonds";
		case Statistic::AchPortal:               return "achievement.portal";
		case Statistic::AchGhast:                return "achievement.ghast";
		case Statistic::AchBlazeRod:             return "achievement.blazeRod";
		case Statistic::AchPotion:               return "achievement.potion";
		case Statistic::AchTheEnd:               return "achievement.theEnd";
		case Statistic::AchTheEnd2:              return "achievement.theEnd2";
		case Statistic::AchEnchantments:         return "achievement.enchantments";
		case Statistic::AchOverkill:             return "achievement.overkill";
		case Statistic::AchBookcase:             return "achievement.bookcase";
		case Statistic::AchExploreAllBiomes:     return "achievement.exploreAllBiomes";
		case Statistic::AchSpawnWither:          return "achievement.spawnWither";
		case Statistic::AchKillWither:           return "achievement.killWither";
		case Statistic::AchFullBeacon:           return "achievement.fullBeacon";
		case Statistic::AchBreedCow:             return "achievement.breedCow";
		case Statistic::AchDiamondsToYou:        return "achievement.diamondsToYou";

		// V1.8 stats
		case Statistic::AnimalsBred:               return "stat.animalsBred";
		case Statistic::BoatOneCm:                 return "stat.boatOneCm";
		case Statistic::ClimbOneCm:                return "stat.climbOneCm";
		case Statistic::CrouchOneCm:               return "stat.crouchOneCm";
		case Statistic::DamageDealt:               return "stat.damageDealt";
		case Statistic::DamageTaken:               return "stat.damageTaken";
		case Statistic::Deaths:                    return "stat.deaths";
		case Statistic::Drop:                      return "stat.drop";
		case Statistic::FallOneCm:                 return "stat.fallOneCm";
		case Statistic::FishCaught:                return "stat.fishCaught";
		case Statistic::FlyOneCm:                  return "stat.flyOneCm";
		case Statistic::HorseOneCm:                return "stat.horseOneCm";
		case Statistic::Jump:                      return "stat.jump";
		case Statistic::LeaveGame:                 return "stat.leaveGame";
		case Statistic::MinecartOneCm:             return "stat.minecartOneCm";
		case Statistic::MobKills:                  return "stat.mobKills";
		case Statistic::PigOneCm:                  return "stat.pigOneCm";
		case Statistic::PlayerKills:               return "stat.playerKills";
		case Statistic::PlayOneMinute:             return "stat.playOneMinute";
		case Statistic::SprintOneCm:               return "stat.sprintOneCm";
		case Statistic::SwimOneCm:                 return "stat.swimOneCm";
		case Statistic::TalkedToVillager:          return "stat.talkedToVillager";
		case Statistic::TimeSinceDeath:            return "stat.timeSinceDeath";
		case Statistic::TradedWithVillager:        return "stat.tradedWithVillager";
		case Statistic::WalkOneCm:                 return "stat.walkOneCm";
		case Statistic::WalkUnderWaterOneCm:       return "stat.diveOneCm";

		// V1.8.2 stats
		case Statistic::CleanArmor:                return "stat.armorCleaned";
		case Statistic::CleanBanner:               return "stat.bannerCleaned";
		case Statistic::EatCakeSlice:              return "stat.cakeSlicesEaten";
		case Statistic::EnchantItem:               return "stat.itemEnchanted";
		case Statistic::FillCauldron:              return "stat.cauldronFilled";
		case Statistic::InspectDispenser:          return "stat.dispenserInspected";
		case Statistic::InspectDropper:            return "stat.dropperInspected";
		case Statistic::InspectHopper:             return "stat.hopperInspected";
		case Statistic::InteractWithBeacon:        return "stat.beaconInteraction";
		case Statistic::InteractWithBrewingstand:  return "stat.brewingstandInteraction";
		case Statistic::InteractWithCraftingTable: return "stat.craftingTableInteraction";
		case Statistic::InteractWithFurnace:       return "stat.furnaceInteraction";
		case Statistic::OpenChest:                 return "stat.chestOpened";
		case Statistic::OpenEnderchest:            return "stat.enderchestOpened";
		case Statistic::PlayNoteblock:             return "stat.noteblockPlayed";
		case Statistic::PlayRecord:                return "stat.recordPlayed";
		case Statistic::PotFlower:                 return "stat.flowerPotted";
		case Statistic::TriggerTrappedChest:       return "stat.trappedChestTriggered";
		case Statistic::TuneNoteblock:             return "stat.noteblockTuned";
		case Statistic::UseCauldron:               return "stat.cauldronUsed";

		// V1.9 stats
		case Statistic::AviateOneCm:               return "stat.aviateOneCm";
		case Statistic::SleepInBed:                return "stat.sleepInBed";
		case Statistic::SneakTime:                 return "stat.sneakTime";
		default:                                   return "";
	}
}
