
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
#include "Palettes/Upgrade.h"

#include "../ClientHandle.h"
#include "../Root.h"
#include "../Server.h"
#include "../World.h"
#include "../EffectID.h"
#include "../StringCompression.h"
#include "../CompositeChat.h"
#include "../UUID.h"
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

#include "../BlockEntities/BannerEntity.h"
#include "../BlockEntities/BeaconEntity.h"
#include "../BlockEntities/CommandBlockEntity.h"
#include "../BlockEntities/EnchantingTableEntity.h"
#include "../BlockEntities/MobHeadEntity.h"
#include "../BlockEntities/MobSpawnerEntity.h"
#include "../BlockEntities/FlowerPotEntity.h"
#include "../Bindings/PluginManager.h"





const int MAX_ENC_LEN = 512;  // Maximum size of the encrypted message; should be 128, but who knows...





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

				cUUID UUID;
				UUID.FromString(Params[2]);

				Json::Value root;
				if (!JsonUtils::ParseString(Params[3], root))
				{
					LOGERROR("Unable to parse player properties: '%s'", Params[3]);
					m_Client->ProxyInit(Params[1], UUID);
				}
				else
				{
					m_Client->ProxyInit(Params[1], UUID, root);
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
		auto FileName = fmt::format(FMT_STRING("CommLogs/{:x}_{}__{}.log"),
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





void cProtocol_1_8_0::DataReceived(cByteBuffer & a_Buffer, ContiguousByteBuffer & a_Data)
{
	if (m_IsEncrypted)
	{
		m_Decryptor.ProcessData(a_Data.data(), a_Data.size());
	}

	AddReceivedData(a_Buffer, a_Data);
}





void cProtocol_1_8_0::DataPrepared(ContiguousByteBuffer & a_Data)
{
	if (m_IsEncrypted)
	{
		m_Encryptor.ProcessData(a_Data.data(), a_Data.size());
	}
}





void cProtocol_1_8_0::SendAcknowledgeBlockChange(int a_SequenceId)
{
	// used in 1.19+
}





void cProtocol_1_8_0::SendAttachEntity(const cEntity & a_Entity, const cEntity & a_Vehicle)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktAttachEntity);
	Pkt.WriteBEUInt32(a_Entity.GetUniqueID());
	Pkt.WriteBEUInt32(a_Vehicle.GetUniqueID());
	Pkt.WriteBool(false);
}





void cProtocol_1_8_0::SendBlockAction(Vector3i a_BlockPos, char a_Byte1, char a_Byte2, BlockState a_Block)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktBlockAction);
	Pkt.WriteXYZPosition64(a_BlockPos);
	Pkt.WriteBEInt8(a_Byte1);
	Pkt.WriteBEInt8(a_Byte2);
	auto NumericBlock = PaletteUpgrade::ToBlock(a_Block);
	Pkt.WriteVarInt32(NumericBlock.first);
}





void cProtocol_1_8_0::SendBlockBreakAnim(UInt32 a_EntityID, Vector3i a_BlockPos, char a_Stage)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktBlockBreakAnim);
	Pkt.WriteVarInt32(a_EntityID);
	Pkt.WriteXYZPosition64(a_BlockPos);
	Pkt.WriteBEInt8(a_Stage);
}





void cProtocol_1_8_0::SendBlockChange(Vector3i a_BlockPos, BlockState a_Block)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktBlockChange);
	Pkt.WriteXYZPosition64(a_BlockPos);
	auto NumericBlock = PaletteUpgrade::ToBlock(a_Block);
	Pkt.WriteVarInt32((static_cast<UInt32>(NumericBlock.first) << 4) | (static_cast<UInt32>(NumericBlock.second) & 15));
}





void cProtocol_1_8_0::SendRenderDistanceCenter(cChunkCoords a_chunk)
{
	// not used in this version
	return;
}





void cProtocol_1_8_0::SendPlayerListInitChat(const cPlayer & a_Player)
{
	// not used in this version
	return;
}





void cProtocol_1_8_0::SendBlockChanges(int a_ChunkX, int a_ChunkZ, const sSetBlockVector & a_Changes)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktBlockChanges);
	Pkt.WriteBEInt32(a_ChunkX);
	Pkt.WriteBEInt32(a_ChunkZ);
	Pkt.WriteVarInt32(static_cast<UInt32>(a_Changes.size()));

	for (const auto & Change : a_Changes)
	{
		Int16 Coords = static_cast<Int16>(Change.m_RelY | (Change.m_RelZ << 8) | (Change.m_RelX << 12));
		Pkt.WriteBEInt16(Coords);
		auto NumericBlock = PaletteUpgrade::ToBlock(Change.m_Block);
		Pkt.WriteVarInt32(static_cast<UInt32>(NumericBlock.first & 0xFFF) << 4 | (NumericBlock.second & 0xF));
	}  // for itr - a_Changes[]
}





void cProtocol_1_8_0::SendBossBarAdd(UInt32 a_UniqueID, const cCompositeChat & a_Title, float a_FractionFilled, BossBarColor a_Color, BossBarDivisionType a_DivisionType, bool a_DarkenSky, bool a_PlayEndMusic, bool a_CreateFog)
{
	// No such packet here
}





void cProtocol_1_8_0::SendBossBarRemove(UInt32 a_UniqueID)
{
	// No such packet here
}





void cProtocol_1_8_0::SendBossBarUpdateFlags(UInt32 a_UniqueID, bool a_DarkenSky, bool a_PlayEndMusic, bool a_CreateFog)
{
	// No such packet here
}





void cProtocol_1_8_0::SendBossBarUpdateHealth(UInt32 a_UniqueID, float a_FractionFilled)
{
	// No such packet here
}





void cProtocol_1_8_0::SendBossBarUpdateStyle(UInt32 a_UniqueID, BossBarColor a_Color, BossBarDivisionType a_DivisionType)
{
	// No such packet here
}





void cProtocol_1_8_0::SendBossBarUpdateTitle(UInt32 a_UniqueID, const cCompositeChat & a_Title)
{
	// No such packet here
}





void cProtocol_1_8_0::SendTags(void)
{
	// No such packet here
}





void cProtocol_1_8_0::SendCameraSetTo(const cEntity & a_Entity)
{
	cPacketizer Pkt(*this, pktCameraSetTo);
	Pkt.WriteVarInt32(a_Entity.GetUniqueID());
}





void cProtocol_1_8_0::SendChat(const AString & a_Message, eChatType a_Type)
{
	ASSERT(m_State == 3);  // In game mode?

	SendChatRaw(JsonUtils::SerializeSingleValueJsonObject("text", a_Message), a_Type);
}





void cProtocol_1_8_0::SendChat(const cCompositeChat & a_Message, eChatType a_Type, bool a_ShouldUseChatPrefixes)
{
	ASSERT(m_State == 3);  // In game mode?

	SendChatRaw(a_Message.CreateJsonString(a_ShouldUseChatPrefixes), a_Type);
}





void cProtocol_1_8_0::SendChatRaw(const AString & a_MessageRaw, eChatType a_Type)
{
	ASSERT(m_State == 3);  // In game mode?

	// Prevent chat messages that might trigger CVE-2021-44228
	if (a_MessageRaw.find("${") != std::string::npos)
	{
		return;
	}

	// Send the json string to the client:
	cPacketizer Pkt(*this, pktChatRaw);
	Pkt.WriteString(a_MessageRaw);
	Pkt.WriteBEInt8([a_Type]() -> signed char
	{
		switch (a_Type)
		{
			case eChatType::ctChatBox: return 0;
			case eChatType::ctSystem: return 1;
			case eChatType::ctAboveActionBar: return 2;
		}
		UNREACHABLE("Unsupported chat type");
	}());
}





void cProtocol_1_8_0::SendCommandTree()
{
	// no such packet here
	return;
}





void cProtocol_1_8_0::SendChunkData(const ContiguousByteBufferView a_ChunkData)
{
	ASSERT(m_State == 3);  // In game mode?

	cCSLock Lock(m_CSPacket);
	m_Client->SendData(a_ChunkData);
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

	{
		cPacketizer Pkt(*this, pktDestroyEntity);
		Pkt.WriteVarInt32(1);
		Pkt.WriteVarInt32(a_Entity.GetUniqueID());
	}

	if (!a_Entity.IsMob())
	{
		return;
	}

	const auto & Mob = static_cast<const cMonster &>(a_Entity);
	if ((Mob.GetMobType() == mtEnderDragon) || (Mob.GetMobType() == mtWither))
	{
		SendBossBarRemove(Mob.GetUniqueID());
	}
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
			Pkt.WriteString(JsonUtils::SerializeSingleValueJsonObject("text", a_Reason));
			break;
		}
		case State::Game:
		{
			cPacketizer Pkt(*this, pktDisconnectDuringGame);
			Pkt.WriteString(JsonUtils::SerializeSingleValueJsonObject("text", a_Reason));
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





void cProtocol_1_8_0::SendDynamicRegistries()
{
	return;
}





void cProtocol_1_8_0::SendEditSign(Vector3i a_BlockPos)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktEditSign);
	Pkt.WriteXYZPosition64(a_BlockPos);
}





void cProtocol_1_8_0::SendEntityAnimation(const cEntity & a_Entity, const EntityAnimation a_Animation)
{
	ASSERT(m_State == 3);  // In game mode?

	if (a_Animation == EntityAnimation::PlayerEntersBed)
	{
		ASSERT(a_Entity.IsPlayer());
		const auto BedPosition = static_cast<const cPlayer &>(a_Entity).GetLastBedPos();

		cPacketizer Pkt(*this, pktUseBed);
		Pkt.WriteVarInt32(a_Entity.GetUniqueID());
		Pkt.WriteXYZPosition64(BedPosition.x, BedPosition.y, BedPosition.z);
		return;
	}

	if (const auto AnimationID = GetProtocolEntityAnimation(a_Animation); AnimationID != static_cast<unsigned char>(-1))
	{
		cPacketizer Pkt(*this, pktEntityAnimation);
		Pkt.WriteVarInt32(a_Entity.GetUniqueID());
		Pkt.WriteBEUInt8(AnimationID);
		return;
	}

	if (const auto StatusID = GetProtocolEntityStatus(a_Animation); StatusID != -1)
	{
		cPacketizer Pkt(*this, pktEntityStatus);
		Pkt.WriteBEUInt32(a_Entity.GetUniqueID());
		Pkt.WriteBEInt8(StatusID);
	}
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

	const auto Delta = (a_Entity.GetPosition() * 32).Floor() - (a_Entity.GetLastSentPosition() * 32).Floor();

	// Ensure that the delta has enough precision and is within range of a BEInt8:
	if (
		Delta.HasNonZeroLength() &&
		cByteBuffer::CanBEInt8Represent(Delta.x) &&
		cByteBuffer::CanBEInt8Represent(Delta.y) &&
		cByteBuffer::CanBEInt8Represent(Delta.z)
	)
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

	// Too big or small a movement, do a teleport.

	cPacketizer Pkt(*this, pktTeleportEntity);
	Pkt.WriteVarInt32(a_Entity.GetUniqueID());
	Pkt.WriteFPInt(a_Entity.GetPosX());
	Pkt.WriteFPInt(a_Entity.GetPosY());
	Pkt.WriteFPInt(a_Entity.GetPosZ());
	Pkt.WriteByteAngle(a_Entity.GetYaw());
	Pkt.WriteByteAngle(a_Entity.GetPitch());
	Pkt.WriteBool(a_Entity.IsOnGround());
}





void cProtocol_1_8_0::SendEntityProperties(const cEntity & a_Entity)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktEntityProperties);
	Pkt.WriteVarInt32(a_Entity.GetUniqueID());
	WriteEntityProperties(Pkt, a_Entity);
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





void cProtocol_1_8_0::SendFinishConfiguration()
{
	return;
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
	Pkt.WriteBEInt8(static_cast<Int8>(a_ItemIndex));
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
		Pkt.WriteXYZPosition64(a_World.GetSpawnX(), a_World.GetSpawnY(), a_World.GetSpawnZ());
	}

	// Send the server difficulty:
	{
		cPacketizer Pkt(*this, pktDifficulty);
		Pkt.WriteBEInt8(1);
	}
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

	Byte Flags = 0;
	const cPlayer * Player = m_Client->GetPlayer();

	if (Player->IsGameModeCreative() || Player->IsGameModeSpectator())
	{
		Flags |= 0x01;  // Invulnerability.
	}
	if (Player->IsFlying())
	{
		Flags |= 0x02;
	}
	if (Player->CanFly())
	{
		Flags |= 0x04;
	}
	if (Player->IsGameModeCreative())
	{
		Flags |= 0x08;  // Godmode: creative instant break.
	}

	cPacketizer Pkt(*this, pktPlayerAbilities);
	Pkt.WriteBEUInt8(Flags);
	Pkt.WriteBEFloat(static_cast<float>(0.05 * Player->GetFlyingMaxSpeed()));
	Pkt.WriteBEFloat(static_cast<float>(0.1 * Player->GetNormalMaxSpeed()));
}





void cProtocol_1_8_0::SendParticleEffect(const AString & a_ParticleName, Vector3f a_Src, Vector3f a_Offset, float a_ParticleData, int a_ParticleAmount)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktParticleEffect);
	Pkt.WriteBEInt32(GetProtocolParticleID(a_ParticleName));
	Pkt.WriteBool(false);
	Pkt.WriteBEFloat(a_Src.x);
	Pkt.WriteBEFloat(a_Src.y);
	Pkt.WriteBEFloat(a_Src.z);
	Pkt.WriteBEFloat(a_Offset.x);
	Pkt.WriteBEFloat(a_Offset.y);
	Pkt.WriteBEFloat(a_Offset.z);
	Pkt.WriteBEFloat(a_ParticleData);
	Pkt.WriteBEInt32(a_ParticleAmount);
}





void cProtocol_1_8_0::SendParticleEffect(const AString & a_ParticleName, Vector3f a_Src, Vector3f a_Offset, float a_ParticleData, int a_ParticleAmount, std::array<int, 2> a_Data)
{
	ASSERT(m_State == 3);  // In game mode?

	const auto ParticleID = GetProtocolParticleID(a_ParticleName);

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





void cProtocol_1_8_0::SendPlayerListHeaderFooter(const cCompositeChat & a_Header, const cCompositeChat & a_Footer)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktPlayerListHeaderFooter);
	Pkt.WriteString(a_Header.CreateJsonString(false));
	Pkt.WriteString(a_Footer.CreateJsonString(false));
}





void cProtocol_1_8_0::SendPlayerListRemovePlayer(const cPlayer & a_Player)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktPlayerList);
	Pkt.WriteVarInt32(4);
	Pkt.WriteVarInt32(1);
	Pkt.WriteUUID(a_Player.GetUUID());
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
		Pkt.WriteString(JsonUtils::SerializeSingleValueJsonObject("text", a_CustomName));
	}
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





void cProtocol_1_8_0::SendPlayerListUpdatePing()
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktPlayerList);
	Pkt.WriteVarInt32(2);

	const auto World = m_Client->GetPlayer()->GetWorld();
	Pkt.WriteVarInt32(static_cast<UInt32>(World->GetPlayerCount()));
	World->ForEachPlayer([&Pkt](cPlayer & a_Player)
	{
		Pkt.WriteUUID(a_Player.GetUUID());
		Pkt.WriteVarInt32(static_cast<UInt32>(a_Player.GetClientHandle()->GetPing()));
		return false;
	});
}





void cProtocol_1_8_0::SendPlayerMoveLook (const Vector3d a_Pos, const float a_Yaw, const float a_Pitch, const bool a_IsRelative)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktPlayerMoveLook);
	Pkt.WriteBEDouble(a_Pos.x);
	Pkt.WriteBEDouble(a_Pos.y);
	Pkt.WriteBEDouble(a_Pos.z);
	Pkt.WriteBEFloat(a_Yaw);
	Pkt.WriteBEFloat(a_Pitch);

	if (a_IsRelative)
	{
		// Set all bits to 1 - makes everything relative
		Pkt.WriteBEUInt8(static_cast<UInt8>(-1));
	}
	else
	{
		// Set all bits to 0 - make everything absolute
		Pkt.WriteBEUInt8(0);
	}
}





void cProtocol_1_8_0::SendPlayerMoveLook(void)
{
	cPlayer * Player = m_Client->GetPlayer();
	SendPlayerMoveLook(Player->GetPosition(), static_cast<float>(Player->GetYaw()), static_cast<float>(Player->GetPitch()), false);
}





void cProtocol_1_8_0::SendPlayerPermissionLevel()
{
	// 1.8 has no concept of a permission level.
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
	Pkt.WriteBEInt16(a_Player.GetEquippedItem().IsEmpty() ? 0 : PaletteUpgrade::ToItem(a_Player.GetEquippedItem().m_ItemType).first);
	WriteEntityMetadata(Pkt, a_Player);
	Pkt.WriteBEUInt8(0x7f);  // Metadata: end
}





void cProtocol_1_8_0::SendPluginMessage(const AString & a_Channel, const ContiguousByteBufferView a_Message)
{
	ASSERT((m_State == 3) || (m_State == 4));  // In game mode?

	cPacketizer Pkt(*this, pktPluginMessage);
	Pkt.WriteString(a_Channel);
	Pkt.WriteBuf(a_Message);
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





void cProtocol_1_8_0::SendSoundEffect(const AString & a_SoundName, Vector3d a_Origin, float a_Volume, float a_Pitch)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktSoundEffect);
	Pkt.WriteString(a_SoundName);
	Pkt.WriteBEInt32(static_cast<Int32>(a_Origin.x * 8.0));
	Pkt.WriteBEInt32(static_cast<Int32>(a_Origin.y * 8.0));
	Pkt.WriteBEInt32(static_cast<Int32>(a_Origin.z * 8.0));
	Pkt.WriteBEFloat(a_Volume);
	Pkt.WriteBEUInt8(static_cast<Byte>(a_Pitch * 63));
}





void cProtocol_1_8_0::SendSoundParticleEffect(const EffectID a_EffectID, Vector3i a_Origin, int a_Data)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktSoundParticleEffect);
	Pkt.WriteBEInt32(static_cast<int>(a_EffectID));
	Pkt.WriteXYZPosition64(a_Origin);
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
		auto NumericBlock = PaletteUpgrade::ToBlock(Block.GetBlock());
		EntityData = NumericBlock.first | (static_cast<Int32>(NumericBlock.second) << 12);
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

	const auto MobType = GetProtocolMobType(a_Mob.GetMobType());

	// If the type is not valid in this protocol bail out:
	if (MobType == 0)
	{
		return;
	}

	cPacketizer Pkt(*this, pktSpawnMob);
	Pkt.WriteVarInt32(a_Mob.GetUniqueID());
	Pkt.WriteBEUInt8(static_cast<Byte>(MobType));
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





void cProtocol_1_8_0::SendStatistics(const StatisticsManager & a_Manager)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktStatistics);

	// No need to check Size != 0.
	// Assume that the vast majority of the time there's at least one statistic to send:
	Pkt.WriteVarInt32(static_cast<UInt32>(a_Manager.Custom.size()));

	for (const auto & [Statistic, Value] : a_Manager.Custom)
	{
		Pkt.WriteString(GetProtocolStatisticName(Statistic));
		Pkt.WriteVarInt32(static_cast<UInt32>(Value));
	}
}





void cProtocol_1_8_0::SendTabCompletionResults(const AStringVector & a_Results, UInt32 CompletionId)
{
	UNUSED(CompletionId);
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktTabCompletionResults);
	Pkt.WriteVarInt32(static_cast<UInt32>(a_Results.size()));

	for (AStringVector::const_iterator itr = a_Results.begin(), end = a_Results.end(); itr != end; ++itr)
	{
		Pkt.WriteString(*itr);
	}
}





void cProtocol_1_8_0::SendThunderbolt(Vector3i a_Origin)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktSpawnGlobalEntity);
	Pkt.WriteVarInt32(0);  // EntityID = 0, always
	Pkt.WriteBEUInt8(1);  // Type = Thunderbolt
	Pkt.WriteFPInt(a_Origin.x);
	Pkt.WriteFPInt(a_Origin.y);
	Pkt.WriteFPInt(a_Origin.z);
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





void cProtocol_1_8_0::SendTimeUpdate(const cTickTimeLong a_WorldAge, const cTickTimeLong a_WorldDate, const bool a_DoDaylightCycle)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktTimeUpdate);
	Pkt.WriteBEInt64(a_WorldAge.count());

	if (a_DoDaylightCycle)
	{
		Pkt.WriteBEInt64(a_WorldDate.count());
	}
	else
	{
		// Negating the date stops time from advancing on the client
		// (the std::min construction is to handle the case where the date is exactly zero):
		Pkt.WriteBEInt64(std::min(-a_WorldDate.count(), -1LL));
	}
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

	Byte Action;
	switch (a_BlockEntity.GetBlockType())
	{
		case BlockType::EnchantingTable: Action = 0; break;  // The ones with a action of 0 is just a workaround to send the block entities to a client.
		case BlockType::EndPortal:       Action = 0; break;  // Todo: 18.09.2020 - remove this when block entities are transmitted in the ChunkData packet - 12xx12

		case BlockType::Spawner:       Action = 1; break;  // Update mob spawner spinny mob thing
		case BlockType::CommandBlock:
		case BlockType::ChainCommandBlock:
		case BlockType::RepeatingCommandBlock:
			Action = 2; break;  // Update command block text
		case BlockType::Beacon:        Action = 3; break;  // Update beacon entity
		case BlockType::CreeperHead:
		case BlockType::CreeperWallHead:
		case BlockType::DragonHead:
		case BlockType::DragonWallHead:
		case BlockType::PlayerHead:
		case BlockType::PlayerWallHead:
		case BlockType::ZombieHead:
		case BlockType::ZombieWallHead:
		case BlockType::SkeletonSkull:
		case BlockType::SkeletonWallSkull:
		case BlockType::WitherSkeletonSkull:
		case BlockType::WitherSkeletonWallSkull:
			Action = 4; break;  // Update Mobhead entity
		case BlockType::PottedAcaciaSapling:
		case BlockType::PottedAzureBluet:
		case BlockType::PottedBamboo:
		case BlockType::PottedBirchSapling:
		case BlockType::PottedBlueOrchid:
		case BlockType::PottedBrownMushroom:
		case BlockType::PottedCactus:
		case BlockType::PottedCornflower:
		case BlockType::PottedCrimsonRoots:
		case BlockType::PottedCrimsonFungus:
		case BlockType::PottedDandelion:
		case BlockType::PottedDarkOakSapling:
		case BlockType::PottedDeadBush:
		case BlockType::PottedFern:
		case BlockType::PottedJungleSapling:
		case BlockType::PottedLilyOfTheValley:
		case BlockType::PottedOakSapling:
		case BlockType::PottedOrangeTulip:
		case BlockType::PottedOxeyeDaisy:
		case BlockType::PottedPinkTulip:
		case BlockType::PottedPoppy:
		case BlockType::PottedRedMushroom:
		case BlockType::PottedRedTulip:
		case BlockType::PottedSpruceSapling:
		case BlockType::PottedWarpedFungus:
		case BlockType::PottedWarpedRoots:
		case BlockType::PottedWhiteTulip:
		case BlockType::PottedWitherRose:
		case BlockType::PottedAllium:
			Action = 5; break;  // Update flower pot
		case BlockType::BlackBanner:
		case BlockType::BlueBanner:
		case BlockType::BrownBanner:
		case BlockType::CyanBanner:
		case BlockType::GrayBanner:
		case BlockType::GreenBanner:
		case BlockType::LightBlueBanner:
		case BlockType::LightGrayBanner:
		case BlockType::LimeBanner:
		case BlockType::MagentaBanner:
		case BlockType::OrangeBanner:
		case BlockType::PinkBanner:
		case BlockType::PurpleBanner:
		case BlockType::RedBanner:
		case BlockType::WhiteBanner:
		case BlockType::YellowBanner:

		case BlockType::BlackWallBanner:
		case BlockType::BlueWallBanner:
		case BlockType::BrownWallBanner:
		case BlockType::CyanWallBanner:
		case BlockType::GrayWallBanner:
		case BlockType::GreenWallBanner:
		case BlockType::LightBlueWallBanner:
		case BlockType::LightGrayWallBanner:
		case BlockType::LimeWallBanner:
		case BlockType::MagentaWallBanner:
		case BlockType::OrangeWallBanner:
		case BlockType::PinkWallBanner:
		case BlockType::PurpleWallBanner:
		case BlockType::RedWallBanner:
		case BlockType::WhiteWallBanner:
		case BlockType::YellowWallBanner:
			Action = 6; break;  // Update Banner
		case BlockType::BlackBed:
		case BlockType::BlueBed:
		case BlockType::BrownBed:
		case BlockType::CyanBed:
		case BlockType::GrayBed:
		case BlockType::GreenBed:
		case BlockType::LightBlueBed:
		case BlockType::LightGrayBed:
		case BlockType::LimeBed:
		case BlockType::MagentaBed:
		case BlockType::OrangeBed:
		case BlockType::PinkBed:
		case BlockType::PurpleBed:
		case BlockType::RedBed:
		case BlockType::WhiteBed:
		case BlockType::YellowBed:
			Action = 11; break;  // Update bed color

		default: return;  // Block entities change between versions
	}

	cPacketizer Pkt(*this, pktUpdateBlockEntity);
	Pkt.WriteXYZPosition64(a_BlockEntity.GetPosX(), a_BlockEntity.GetPosY(), a_BlockEntity.GetPosZ());
	Pkt.WriteBEUInt8(Action);

	cFastNBTWriter Writer;
	WriteBlockEntity(Writer, a_BlockEntity);
	Writer.Finish();
	Pkt.WriteBuf(Writer.GetResult());
}





void cProtocol_1_8_0::SendUpdateSign(Vector3i a_BlockPos, const AString & a_Line1, const AString & a_Line2, const AString & a_Line3, const AString & a_Line4)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktUpdateSign);
	Pkt.WriteXYZPosition64(a_BlockPos);

	AString Lines[] = { a_Line1, a_Line2, a_Line3, a_Line4 };
	for (size_t i = 0; i < ARRAYCOUNT(Lines); i++)
	{
		Pkt.WriteString(JsonUtils::SerializeSingleValueJsonObject("text", Lines[i]));
	}
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





void cProtocol_1_8_0::SendGameStateChange(eGameStateReason a_Reason, float a_Value)
{
	ASSERT(m_State == 3);  // In game mode?

	{
		cPacketizer Pkt(*this, pktWeather);
		Pkt.WriteBEUInt8(static_cast<UInt8>(a_Reason));
		Pkt.WriteBEFloat(a_Value);
	}
}





void cProtocol_1_8_0::SendWholeInventory(const cWindow & a_Window, const cItem & a_CursorStack)
{
	ASSERT(m_State == 3);  // In game mode?
	UNUSED(a_CursorStack);
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
	Pkt.WriteString(JsonUtils::SerializeSingleValueJsonObject("text", a_Window.GetWindowTitle()));

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





void cProtocol_1_8_0::CompressPacket(CircularBufferCompressor & a_Packet, ContiguousByteBuffer & a_CompressedData)
{
	const auto Uncompressed = a_Packet.GetView();

	if (Uncompressed.size() < CompressionThreshold)
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
		const auto PacketSize = static_cast<UInt32>(cByteBuffer::GetVarIntSize(DataSize) + Uncompressed.size());

		cByteBuffer LengthHeaderBuffer(
			cByteBuffer::GetVarIntSize(PacketSize) +
			cByteBuffer::GetVarIntSize(DataSize)
		);

		LengthHeaderBuffer.WriteVarInt32(PacketSize);
		LengthHeaderBuffer.WriteVarInt32(DataSize);

		ContiguousByteBuffer LengthData;
		LengthHeaderBuffer.ReadAll(LengthData);

		a_CompressedData.reserve(LengthData.size() + Uncompressed.size());
		a_CompressedData = LengthData;
		a_CompressedData += Uncompressed;

		return;
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

	const auto CompressedData = a_Packet.Compress();
	const auto Compressed = CompressedData.GetView();

	const UInt32 DataSize = static_cast<UInt32>(Uncompressed.size());
	const auto PacketSize = static_cast<UInt32>(cByteBuffer::GetVarIntSize(DataSize) + Compressed.size());

	cByteBuffer LengthHeaderBuffer(
		cByteBuffer::GetVarIntSize(PacketSize) +
		cByteBuffer::GetVarIntSize(DataSize)
	);

	LengthHeaderBuffer.WriteVarInt32(PacketSize);
	LengthHeaderBuffer.WriteVarInt32(DataSize);

	ContiguousByteBuffer LengthData;
	LengthHeaderBuffer.ReadAll(LengthData);

	a_CompressedData.reserve(LengthData.size() + Compressed.size());
	a_CompressedData = LengthData;
	a_CompressedData += Compressed;
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





UInt32 cProtocol_1_8_0::GetPacketID(ePacketType a_PacketType) const
{
	switch (a_PacketType)
	{
		case pktAttachEntity:           return 0x1b;
		case pktBlockAction:            return 0x24;
		case pktBlockBreakAnim:         return 0x25;
		case pktBlockChange:            return 0x23;
		case pktBlockChanges:           return 0x22;
		case pktCameraSetTo:            return 0x43;
		case pktChatRaw:                return 0x02;
		case pktCollectEntity:          return 0x0d;
		case pktDestroyEntity:          return 0x13;
		case pktDifficulty:             return 0x41;
		case pktDisconnectDuringGame:   return 0x40;
		case pktDisconnectDuringLogin:  return 0x00;
		case pktDisplayObjective:       return 0x3d;
		case pktEditSign:               return 0x36;
		case pktEncryptionRequest:      return 0x01;
		case pktEntityAnimation:        return 0x0b;
		case pktEntityEffect:           return 0x1d;
		case pktEntityEquipment:        return 0x04;
		case pktEntityHeadLook:         return 0x19;
		case pktEntityLook:             return 0x16;
		case pktEntityMeta:             return 0x1c;
		case pktEntityProperties:       return 0x20;
		case pktEntityRelMove:          return 0x15;
		case pktEntityRelMoveLook:      return 0x17;
		case pktEntityStatus:           return 0x1a;
		case pktEntityVelocity:         return 0x12;
		case pktExperience:             return 0x1f;
		case pktExplosion:              return 0x27;
		case pktGameMode:               return 0x2b;
		case pktHeldItemChange:         return 0x09;
		case pktInventorySlot:          return 0x2f;
		case pktJoinGame:               return 0x01;
		case pktKeepAlive:              return 0x00;
		case pktLeashEntity:            return 0x1b;
		case pktLoginSuccess:           return 0x02;
		case pktMapData:                return 0x34;
		case pktParticleEffect:         return 0x2a;
		case pktPingResponse:           return 0x01;
		case pktPlayerAbilities:        return 0x39;
		case pktPlayerList:             return 0x38;
		case pktPlayerListHeaderFooter: return 0x47;
		case pktPlayerMoveLook:         return 0x08;
		case pktPluginMessage:          return 0x3f;
		case pktRemoveEntityEffect:     return 0x1e;
		case pktResourcePack:           return 0x48;
		case pktRespawn:                return 0x07;
		case pktScoreboardObjective:    return 0x3b;
		case pktSoundEffect:            return 0x29;
		case pktSoundParticleEffect:    return 0x28;
		case pktSpawnExperienceOrb:     return 0x11;
		case pktSpawnGlobalEntity:      return 0x2c;
		case pktSpawnMob:               return 0x0f;
		case pktSpawnObject:            return 0x0e;
		case pktSpawnOtherPlayer:       return 0x0c;
		case pktSpawnPainting:          return 0x10;
		case pktSpawnPosition:          return 0x05;
		case pktStartCompression:       return 0x03;
		case pktStatistics:             return 0x37;
		case pktStatusResponse:         return 0x00;
		case pktTabCompletionResults:   return 0x3a;
		case pktTeleportEntity:         return 0x18;
		case pktTimeUpdate:             return 0x03;
		case pktTitle:                  return 0x45;
		case pktUnloadChunk:            return 0x21;
		case pktUpdateBlockEntity:      return 0x35;
		case pktUpdateHealth:           return 0x06;
		case pktUpdateScore:            return 0x3c;
		case pktUpdateSign:             return 0x33;
		case pktUseBed:                 return 0x0a;
		case pktWeather:                return 0x2b;
		case pktWindowClose:            return 0x2e;
		case pktWindowItems:            return 0x30;
		case pktWindowOpen:             return 0x2d;
		case pktWindowProperty:         return 0x31;
		default:
		{
			LOG("Unhandled outgoing packet type: %s (0x%02x)", cPacketizer::PacketTypeToStr(a_PacketType), a_PacketType);
			ASSERT(!"Unhandled outgoing packet type");
			return 0;
		}
	}
}





unsigned char cProtocol_1_8_0::GetProtocolEntityAnimation(const EntityAnimation a_Animation) const
{
	switch (a_Animation)
	{
		case EntityAnimation::EntityGetsCriticalHit: return 4;
		case EntityAnimation::EntityGetsMagicalCriticalHit: return 5;
		case EntityAnimation::PlayerLeavesBed: return 2;
		case EntityAnimation::PlayerMainHandSwings: return 0;
		case EntityAnimation::PlayerOffHandSwings: return 0;
		default: return static_cast<unsigned char>(-1);
	}
}





signed char cProtocol_1_8_0::GetProtocolEntityStatus(const EntityAnimation a_Animation) const
{
	switch (a_Animation)
	{
		case EntityAnimation::AnimalFallsInLove: return 18;
		case EntityAnimation::FireworkRocketExplodes: return 17;
		case EntityAnimation::GuardianAttacks: return 21;
		case EntityAnimation::HorseTamingFails: return 6;
		case EntityAnimation::HorseTamingSucceeds: return 7;
		case EntityAnimation::IronGolemAttacks: return 4;
		case EntityAnimation::IronGolemOffersGift: return 11;
		case EntityAnimation::MinecartSpawnerDelayResets: return 1;
		case EntityAnimation::MinecartTNTIgnites: return 10;
		case EntityAnimation::MobSpawns: return 20;
		case EntityAnimation::OcelotTrusts: return 6;
		case EntityAnimation::OcelotDistrusts: return 7;
		case EntityAnimation::PawnBerryBushPricks: return 2;
		case EntityAnimation::PawnBurns: return 2;
		case EntityAnimation::PawnDies: return 3;
		case EntityAnimation::PawnDrowns: return 2;
		case EntityAnimation::PawnHurts: return 2;
		case EntityAnimation::PawnThornsPricks: return 2;
		case EntityAnimation::PlayerFinishesEating: return 9;
		case EntityAnimation::RabbitJumps: return 1;
		case EntityAnimation::SheepEatsGrass: return 10;
		case EntityAnimation::VillagerKisses: return 12;
		case EntityAnimation::VillagerShowsAnger: return 13;
		case EntityAnimation::VillagerShowsHappiness: return 14;
		case EntityAnimation::WitchMagicks: return 15;
		case EntityAnimation::WolfShakesWater: return 8;
		case EntityAnimation::WolfTamingFails: return 6;
		case EntityAnimation::WolfTamingSucceeds: return 7;
		case EntityAnimation::ZombieVillagerCureFinishes: return 16;
		default: return -1;
	}
}





UInt32 cProtocol_1_8_0::GetProtocolMobType(const eMonsterType a_MobType) const
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
		case mtEndermite:             return 67;
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

		// Mobs that get replaced with another because they were added later
		case mtCat:                   return GetProtocolMobType(mtOcelot);
		case mtDonkey:                return GetProtocolMobType(mtHorse);
		case mtMule:                  return GetProtocolMobType(mtHorse);
		case mtSkeletonHorse:         return GetProtocolMobType(mtHorse);
		case mtZombieHorse:           return GetProtocolMobType(mtHorse);
		case mtStray:                 return GetProtocolMobType(mtSkeleton);
		case mtHusk:                  return GetProtocolMobType(mtZombie);

		default:                      return 0;
	}
}





cProtocol::Version cProtocol_1_8_0::GetProtocolVersion() const
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
	std::vector<std::pair<cUUID, AString>> playeruuids;
	const std::pair<cUUID, AString> AnonPlayer = {cUUID(), "Anonymous"};
	cRoot::Get()->ForEachPlayer([&playeruuids, &AnonPlayer](const cPlayer & a_Player)
	{
		if (a_Player.GetClientHandle()->GetAllowListing())
		{
			playeruuids.emplace_back(a_Player.GetUUID(), a_Player.GetName());
		}
		else
		{
			playeruuids.push_back(AnonPlayer);
		}
		return true;
	});
	Json::Value sample;
	int i = 0;
	for (const std::pair<cUUID, AString> & PlayerId : playeruuids)
	{
		Json::Value entry;
		entry["name"] = PlayerId.second;
		entry["id"] = PlayerId.first.ToLongString();
		sample[i] = entry;
		i++;
	}
	Players["sample"] = sample;

	// Description:
	Json::Value Description;
	Description["text"] = std::move(ServerDescription);

	// Create the response:
	Json::Value ResponseValue;
	ResponseValue["version"] = Version;
	ResponseValue["players"] = Players;
	ResponseValue["description"] = Description;
	m_Client->ForgeAugmentServerListPing(ResponseValue);
	if (!Favicon.empty())
	{
		ResponseValue["favicon"] = "data:image/png;base64," + Favicon;
	}

	ResponseValue["enforcesSecureChat"] = "false";
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
	ContiguousByteBuffer EncKey;
	if (!a_ByteBuffer.ReadSome(EncKey, EncKeyLength))
	{
		return;
	}
	if (!a_ByteBuffer.ReadVarInt(EncNonceLength))
	{
		return;
	}
	ContiguousByteBuffer EncNonce;
	if (!a_ByteBuffer.ReadSome(EncNonce, EncNonceLength))
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
	int res = rsaDecryptor.Decrypt(EncNonce, reinterpret_cast<Byte *>(DecryptedNonce), sizeof(DecryptedNonce));
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
	res = rsaDecryptor.Decrypt(EncKey, DecryptedKey, sizeof(DecryptedKey));
	if (res != 16)
	{
		LOGD("Bad key length");
		m_Client->Kick("Hacked client");
		return;
	}

	StartEncryption(DecryptedKey);
	m_Client->HandleLogin();
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

	m_Client->SetUsername(std::move(Username));

	// If auth is required, then send the encryption request:
	if (const auto Server = cRoot::Get()->GetServer(); Server->ShouldAuthenticate())
	{
		cPacketizer Pkt(*this, pktEncryptionRequest);
		Pkt.WriteString(Server->GetServerID());
		const auto PubKeyDer = Server->GetPublicKeyDER();
		Pkt.WriteVarInt32(static_cast<UInt32>(PubKeyDer.size()));
		Pkt.WriteBuf(PubKeyDer);
		Pkt.WriteVarInt32(4);
		Pkt.WriteBEInt32(static_cast<int>(reinterpret_cast<intptr_t>(this)));  // Using 'this' as the cryptographic nonce, so that we don't have to generate one each time :)
		return;
	}

	m_Client->HandleLogin();
}





void cProtocol_1_8_0::HandlePacketPlayerSession(cByteBuffer & a_ByteBuffer)
{
	return;
}





void cProtocol_1_8_0::HandlePacketEnterConfiguration(cByteBuffer & a_ByteBuffer)
{
	return;
}





void cProtocol_1_8_0::HandlePacketReady(cByteBuffer & a_ByteBuffer)
{
	return;
}





void cProtocol_1_8_0::HandlePacketAnimation(cByteBuffer & a_ByteBuffer)
{
	m_Client->HandleAnimation(true);  // Packet exists solely for arm-swing notification (main hand).
}





void cProtocol_1_8_0::HandlePacketBlockDig(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadBEUInt8, UInt8, Status);

	Vector3i Position;
	if (!a_ByteBuffer.ReadXYZPosition64(Position))
	{
		return;
	}

	HANDLE_READ(a_ByteBuffer, ReadBEInt8, Int8, Face);

	m_Client->HandleLeftClick(Position, FaceIntToBlockFace(Face), Status);
}





void cProtocol_1_8_0::HandlePacketBlockPlace(cByteBuffer & a_ByteBuffer)
{
	Vector3i BlockPos;
	if (!a_ByteBuffer.ReadXYZPosition64(BlockPos))
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
		m_Client->HandleUseItem(true);
	}
	else
	{
		m_Client->HandleRightClick(BlockPos, blockFace, {CursorX, CursorY, CursorZ}, true);
	}
}





void cProtocol_1_8_0::HandlePacketChatMessage(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadVarUTF8String, AString, Message);

	m_Client->HandleChat(Message);
}





void cProtocol_1_8_0::HandlePacketCommandExecution(cByteBuffer & a_ByteBuffer)
{
	return;
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
			SendStatistics(m_Client->GetPlayer()->GetStatistics());
			break;
		}
		case 2:
		{
			// Open Inventory achievement
			m_Client->GetPlayer()->AwardAchievement(CustomStatistic::AchOpenInventory);
			break;
		}
	}
}





void cProtocol_1_8_0::HandlePacketCommandBlockUpdate(cByteBuffer & a_ByteBuffer)
{
	// Used in 1.13+
	return;
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

	if (PlayerID != m_Client->GetPlayer()->GetUniqueID())
	{
		LOGD("Player \"%s\" attempted to action another entity - hacked client?", m_Client->GetUsername().c_str());
		return;
	}

	switch (Action)
	{
		case 0: return m_Client->HandleCrouch(true);
		case 1: return m_Client->HandleCrouch(false);
		case 2: return m_Client->HandleLeaveBed();
		case 3: return m_Client->HandleSprint(true);
		case 4: return m_Client->HandleSprint(false);
		case 6: return m_Client->HandleOpenHorseInventory();
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
	bool IsFlying = false;
	if ((Flags & 2) != 0)
	{
		IsFlying = true;
	}

	m_Client->HandlePlayerAbilities(IsFlying, FlyingSpeed, WalkingSpeed);
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

	m_Client->HandlePlayerMove({PosX, PosY, PosZ}, IsOnGround);
}





void cProtocol_1_8_0::HandlePacketPlayerPosLook(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadBEDouble, double, PosX);
	HANDLE_READ(a_ByteBuffer, ReadBEDouble, double, PosY);
	HANDLE_READ(a_ByteBuffer, ReadBEDouble, double, PosZ);
	HANDLE_READ(a_ByteBuffer, ReadBEFloat,  float,  Yaw);
	HANDLE_READ(a_ByteBuffer, ReadBEFloat,  float,  Pitch);
	HANDLE_READ(a_ByteBuffer, ReadBool,     bool,   IsOnGround);

	m_Client->HandlePlayerMoveLook({PosX, PosY, PosZ}, Yaw, Pitch, IsOnGround);
}





void cProtocol_1_8_0::HandlePacketPluginMessage(cByteBuffer & a_ByteBuffer)
{
	// https://wiki.vg/index.php?title=Plugin_channels&oldid=14089#MC.7CAdvCmd

	HANDLE_READ(a_ByteBuffer, ReadVarUTF8String, AString, Channel);

	const std::string_view ChannelView = Channel;

	// If the plugin channel is recognized vanilla, handle it directly:
	if (ChannelView.substr(0, 3) == "MC|")
	{
		HandleVanillaPluginMessage(a_ByteBuffer, ChannelView.substr(3));
		return;
	}

	ContiguousByteBuffer Data;

	// Read the plugin message and relay to clienthandle:
	a_ByteBuffer.ReadSome(Data, a_ByteBuffer.GetReadableSpace());
	m_Client->HandlePluginMessage(Channel, Data);
}





void cProtocol_1_8_0::HandlePacketResourcePackStatus(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadVarUTF8String, AString, Hash);
	HANDLE_READ(a_ByteBuffer, ReadBEUInt8, UInt8, Status);

	m_Client->HandleResourcePack(Status);
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

	m_Client->HandleTabCompletion(Text, 0);
}





void cProtocol_1_8_0::HandlePacketUpdateSign(cByteBuffer & a_ByteBuffer)
{
	Vector3i Position;
	if (!a_ByteBuffer.ReadXYZPosition64(Position))
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

	m_Client->HandleUpdateSign(Position, Lines[0], Lines[1], Lines[2], Lines[3]);
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





void cProtocol_1_8_0::HandlePacketBookUpdate(cByteBuffer & a_ByteBuffer)
{
	// Used by clients 1.15+
	return;
}





void cProtocol_1_8_0::HandleVanillaPluginMessage(cByteBuffer & a_ByteBuffer, const std::string_view a_Channel)
{
	if ((a_Channel == "AdvCdm") || (a_Channel == "AdvCmd"))  // Spelling was fixed in 15w34.
	{
		HANDLE_READ(a_ByteBuffer, ReadBEUInt8, UInt8, Type);

		switch (Type)
		{
			case 0x00:
			{
				HANDLE_READ(a_ByteBuffer, ReadBEInt32, Int32, BlockX);
				HANDLE_READ(a_ByteBuffer, ReadBEInt32, Int32, BlockY);
				HANDLE_READ(a_ByteBuffer, ReadBEInt32, Int32, BlockZ);
				HANDLE_READ(a_ByteBuffer, ReadVarUTF8String, AString, Command);
				HANDLE_READ(a_ByteBuffer, ReadBool, bool, TrackOutput);

				// Editing a command-block:
				m_Client->HandleCommandBlockBlockChange({BlockX, BlockY, BlockZ}, Command);
				return;
			}
			case 0x01:
			{
				HANDLE_READ(a_ByteBuffer, ReadBEUInt32, UInt32, EntityID);
				HANDLE_READ(a_ByteBuffer, ReadVarUTF8String, AString, Command);
				HANDLE_READ(a_ByteBuffer, ReadBool, bool, TrackOutput);

				// Editing a command-block-minecart:
				m_Client->HandleCommandBlockEntityChange(EntityID, Command);
				return;
			}
			default:
			{
				LOGD("Player \"%s\" sent an invalid command block edit type - hacked client?", m_Client->GetUsername().c_str());
				return;
			}
		}
	}
	else if (a_Channel == "Beacon")
	{
		HANDLE_READ(a_ByteBuffer, ReadBEUInt32, UInt32, Effect1);
		HANDLE_READ(a_ByteBuffer, ReadBEUInt32, UInt32, Effect2);

		m_Client->HandleBeaconSelection(Effect1, Effect2);
	}
	else if (a_Channel == "BEdit")
	{
		if (cItem UnsignedBook; ReadItem(a_ByteBuffer, UnsignedBook))
		{
			// TODO: m_Client->HandleBookEdit
		}
	}
	else if (a_Channel == "BSign")
	{
		if (cItem WrittenBook; ReadItem(a_ByteBuffer, WrittenBook))
		{
			// TODO: m_Client->HandleBookSign
		}
	}
	else if (a_Channel == "Brand")
	{
		HANDLE_READ(a_ByteBuffer, ReadVarUTF8String, AString, Brand);

		m_Client->SetClientBrand(Brand);
		m_Client->SendPluginMessage("MC|Brand", "\x08""Cuberite");  // Send back our brand, including the length.
	}
	else if (a_Channel == "ItemName")
	{
		HANDLE_READ(a_ByteBuffer, ReadVarUTF8String, AString, ItemName);

		m_Client->HandleAnvilItemName(ItemName);
	}
	else if (a_Channel == "TrSel")
	{
		HANDLE_READ(a_ByteBuffer, ReadBEInt32, Int32, SlotNum);

		m_Client->HandleNPCTrade(SlotNum);
	}
}





void cProtocol_1_8_0::ParseItemMetadata(cItem & a_Item, const ContiguousByteBufferView a_Metadata) const
{
	// Parse into NBT:
	cParsedNBT NBT(a_Metadata);
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
					cFireworkItem::ParseFromNBT(a_Item.m_FireworkItem, NBT, tag, a_Item.m_ItemType);
				}
				break;
			}
			case TAG_Int:
			{
				if (TagName == "RepairCost")
				{
					a_Item.m_RepairCost = NBT.GetInt(tag);
				}
				break;
			}
			default: LOGD("Unimplemented NBT data when parsing!"); break;
		}
	}
}





bool cProtocol_1_8_0::ReadItem(cByteBuffer & a_ByteBuffer, cItem & a_Item, size_t a_KeepRemainingBytes) const
{
	HANDLE_PACKET_READ(a_ByteBuffer, ReadBEInt16, Int16, ItemType);

	if (ItemType == -1)
	{
		// The item is empty, no more data follows
		a_Item.Empty();
		return true;
	}

	HANDLE_PACKET_READ(a_ByteBuffer, ReadBEInt8,  Int8,  ItemCount);
	HANDLE_PACKET_READ(a_ByteBuffer, ReadBEInt16, Int16, ItemDamage);

	a_Item.m_ItemCount  = ItemCount;
	a_Item.m_ItemDamage = ItemDamage;
	if (ItemCount <= 0)
	{
		a_Item.Empty();
	}

	a_Item.m_ItemType = PaletteUpgrade::FromItem(ItemType, ItemDamage);
	if (ItemCategory::IsTool(a_Item.m_ItemType))
	{
		a_Item.m_ItemDamage += ItemDamage;
	}

	ContiguousByteBuffer Metadata;
	if (!a_ByteBuffer.ReadSome(Metadata, a_ByteBuffer.GetReadableSpace() - a_KeepRemainingBytes) || Metadata.empty() || (Metadata[0] == std::byte(0)))
	{
		// No metadata
		return true;
	}

	ParseItemMetadata(a_Item, Metadata);
	return true;
}





void cProtocol_1_8_0::SendEntitySpawn(const cEntity & a_Entity, const UInt8 a_ObjectType, const Int32 a_ObjectData)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktSpawnObject);
	Pkt.WriteVarInt32(a_Entity.GetUniqueID());
	Pkt.WriteBEUInt8(a_ObjectType);
	Pkt.WriteFPInt(a_Entity.GetPosX());
	Pkt.WriteFPInt(a_Entity.GetPosY());
	Pkt.WriteFPInt(a_Entity.GetPosZ());
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





void cProtocol_1_8_0::SendPacket(cPacketizer & a_Pkt)
{
	ASSERT(m_OutPacketBuffer.GetReadableSpace() == m_OutPacketBuffer.GetUsedSpace());

	m_Compressor.ReadFrom(m_OutPacketBuffer);
	m_OutPacketBuffer.CommitRead();

	const auto PacketData = m_Compressor.GetView();

	if ((m_State == 3) || m_CompressionEnabled)
	{
		ContiguousByteBuffer CompressedPacket;

		// Compress the packet payload:
		cProtocol_1_8_0::CompressPacket(m_Compressor, CompressedPacket);

		// Send the packet's payload compressed:
		m_Client->SendData(CompressedPacket);
	}
	else
	{
		// Compression doesn't apply to this state, send raw data:
		m_OutPacketLenBuffer.WriteVarInt32(static_cast<UInt32>(PacketData.size()));
		ContiguousByteBuffer LengthData;
		m_OutPacketLenBuffer.ReadAll(LengthData);
		m_OutPacketLenBuffer.CommitRead();
		m_Client->SendData(LengthData);

		// Send the packet's payload directly:
		m_Client->SendData(PacketData);
	}

	// Log the comm into logfile:
	if (g_ShouldLogCommOut && m_CommLogFile.IsOpen())
	{
		AString Hex;
		ASSERT(PacketData.size() > 0);
		CreateHexDump(Hex, PacketData.data(), PacketData.size(), 16);
		m_CommLogFile.Write(fmt::format(
			FMT_STRING("Outgoing packet: type {} (translated to 0x{:02x}), length {} (0x{:04x}), state {}. Payload (incl. type):\n{}\n"),
			cPacketizer::PacketTypeToStr(a_Pkt.GetPacketType()), GetPacketID(a_Pkt.GetPacketType()),
			PacketData.size(), PacketData.size(), m_State, Hex
		));

		// Useful for debugging a new protocol:
		LOGD("Outgoing packet: type %s (translated to 0x%02x), length %u (0x%04x), state %d. Payload (incl. type):\n%s\n",
			cPacketizer::PacketTypeToStr(a_Pkt.GetPacketType()), GetPacketID(a_Pkt.GetPacketType()),
			0, 0, m_State, Hex
		);
	}

	// Useful for debugging a new protocol:
	// std::this_thread::sleep_for(std::chrono::milliseconds(100));
}





void cProtocol_1_8_0::WriteBlockEntity(cFastNBTWriter & a_Writer, const cBlockEntity & a_BlockEntity) const
{
	switch (a_BlockEntity.GetBlockType())
	{
		case BlockType::BlackBanner:
		case BlockType::BlueBanner:
		case BlockType::BrownBanner:
		case BlockType::CyanBanner:
		case BlockType::GrayBanner:
		case BlockType::GreenBanner:
		case BlockType::LightBlueBanner:
		case BlockType::LightGrayBanner:
		case BlockType::LimeBanner:
		case BlockType::MagentaBanner:
		case BlockType::OrangeBanner:
		case BlockType::PinkBanner:
		case BlockType::PurpleBanner:
		case BlockType::RedBanner:
		case BlockType::WhiteBanner:
		case BlockType::YellowBanner:

		case BlockType::BlackWallBanner:
		case BlockType::BlueWallBanner:
		case BlockType::BrownWallBanner:
		case BlockType::CyanWallBanner:
		case BlockType::GrayWallBanner:
		case BlockType::GreenWallBanner:
		case BlockType::LightBlueWallBanner:
		case BlockType::LightGrayWallBanner:
		case BlockType::LimeWallBanner:
		case BlockType::MagentaWallBanner:
		case BlockType::OrangeWallBanner:
		case BlockType::PinkWallBanner:
		case BlockType::PurpleWallBanner:
		case BlockType::RedWallBanner:
		case BlockType::WhiteWallBanner:
		case BlockType::YellowWallBanner:
		{
			auto & BannerEntity = static_cast<const cBannerEntity &>(a_BlockEntity);
			a_Writer.AddInt("Base", static_cast<Int32>(BannerEntity.GetBaseColor()));
			break;
		}
		case BlockType::Beacon:
		case BlockType::Chest:
		{
			// Nothing!
			break;
		}
		case BlockType::CommandBlock:
		case BlockType::ChainCommandBlock:
		case BlockType::RepeatingCommandBlock:
		{
			auto & CommandBlockEntity = static_cast<const cCommandBlockEntity &>(a_BlockEntity);
			a_Writer.AddByte("TrackOutput", 1);  // Neither I nor the MC wiki has any idea about this
			a_Writer.AddInt("SuccessCount", CommandBlockEntity.GetResult());
			a_Writer.AddString("Command", CommandBlockEntity.GetCommand());
			// You can set custom names for windows in Vanilla
			// For a command block, this would be the 'name' prepended to anything it outputs into global chat
			// MCS doesn't have this, so just leave it @ '@'. (geddit?)
			a_Writer.AddString("CustomName", "@");
			if (!CommandBlockEntity.GetLastOutput().empty())
			{
				a_Writer.AddString("LastOutput", JsonUtils::SerializeSingleValueJsonObject("text", CommandBlockEntity.GetLastOutput()));
			}
			break;
		}
		case BlockType::EnchantingTable:
		case BlockType::EndPortal:
		{
			// Nothing!
			break;
		}
		case BlockType::CreeperHead:
		case BlockType::CreeperWallHead:
		case BlockType::DragonHead:
		case BlockType::DragonWallHead:
		case BlockType::PlayerHead:
		case BlockType::PlayerWallHead:
		case BlockType::SkeletonSkull:
		case BlockType::SkeletonWallSkull:
		case BlockType::WitherSkeletonSkull:
		case BlockType::WitherSkeletonWallSkull:
		case BlockType::ZombieHead:
		case BlockType::ZombieWallHead:
		{
			auto & MobHeadEntity = static_cast<const cMobHeadEntity &>(a_BlockEntity);
			// a_Writer.AddByte("SkullType", MobHeadEntity.GetType() & 0xFF);  // ignore 1.12 < is basically unsupported
			a_Writer.AddByte("Rot", MobHeadEntity.GetRotation() & 0xFF);

			// The new Block Entity format for a Mob Head. See: https://minecraft.wiki/w/Head#Block_entity
			a_Writer.BeginCompound("Owner");
				a_Writer.AddString("Id", MobHeadEntity.GetOwnerUUID().ToShortString());
				a_Writer.AddString("Name", MobHeadEntity.GetOwnerName());
				a_Writer.BeginCompound("Properties");
					a_Writer.BeginList("textures", TAG_Compound);
						a_Writer.BeginCompound("");
							a_Writer.AddString("Signature", MobHeadEntity.GetOwnerTextureSignature());
							a_Writer.AddString("Value", MobHeadEntity.GetOwnerTexture());
						a_Writer.EndCompound();
					a_Writer.EndList();
				a_Writer.EndCompound();
			a_Writer.EndCompound();
			break;
		}
		case BlockType::PottedAcaciaSapling:
		case BlockType::PottedAzureBluet:
		case BlockType::PottedBamboo:
		case BlockType::PottedBirchSapling:
		case BlockType::PottedBlueOrchid:
		case BlockType::PottedBrownMushroom:
		case BlockType::PottedCactus:
		case BlockType::PottedCornflower:
		case BlockType::PottedCrimsonRoots:
		case BlockType::PottedCrimsonFungus:
		case BlockType::PottedDandelion:
		case BlockType::PottedDarkOakSapling:
		case BlockType::PottedDeadBush:
		case BlockType::PottedFern:
		case BlockType::PottedJungleSapling:
		case BlockType::PottedLilyOfTheValley:
		case BlockType::PottedOakSapling:
		case BlockType::PottedOrangeTulip:
		case BlockType::PottedOxeyeDaisy:
		case BlockType::PottedPinkTulip:
		case BlockType::PottedPoppy:
		case BlockType::PottedRedMushroom:
		case BlockType::PottedRedTulip:
		case BlockType::PottedSpruceSapling:
		case BlockType::PottedWarpedFungus:
		case BlockType::PottedWarpedRoots:
		case BlockType::PottedWhiteTulip:
		case BlockType::PottedWitherRose:
		case BlockType::PottedAllium:
		{
			auto & FlowerPotEntity = static_cast<const cFlowerPotEntity &>(a_BlockEntity);
			a_Writer.AddInt("Item", static_cast<Int32>(FlowerPotEntity.GetItem().m_ItemType));
			a_Writer.AddInt("Data", static_cast<Int32>(FlowerPotEntity.GetItem().m_ItemDamage));
			break;
		}
		case BlockType::Spawner:
		{
			auto & MobSpawnerEntity = static_cast<const cMobSpawnerEntity &>(a_BlockEntity);
			a_Writer.AddString("EntityId", cMonster::MobTypeToVanillaName(MobSpawnerEntity.GetEntity()));
			a_Writer.AddShort("Delay", MobSpawnerEntity.GetSpawnDelay());
			break;
		}
		default:
		{
			LOGWARN(fmt::format(FMT_STRING("Unhandled block entity {}"), NamespaceSerializer::From(a_BlockEntity.GetBlockType())));
			return;
		}
	}

	a_Writer.AddInt("x", a_BlockEntity.GetPosX());
	a_Writer.AddInt("y", a_BlockEntity.GetPosY());
	a_Writer.AddInt("z", a_BlockEntity.GetPosZ());
}





void cProtocol_1_8_0::WriteEntityMetadata(cPacketizer & a_Pkt, const cEntity & a_Entity) const
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
	if (a_Entity.IsElytraFlying())
	{
		Flags |= 0x80;
	}
	a_Pkt.WriteBEUInt8(0);  // Byte(0) + index 0
	a_Pkt.WriteBEUInt8(Flags);

	switch (a_Entity.GetEntityType())
	{
		case cEntity::etPlayer:
		{
			auto & Player = static_cast<const cPlayer &>(a_Entity);

			// Player name:
			a_Pkt.WriteBEUInt8(0x82);
			a_Pkt.WriteString(Player.GetName());

			// Player health:
			a_Pkt.WriteBEUInt8(0x66);
			a_Pkt.WriteBEFloat(static_cast<float>(Player.GetHealth()));

			// Skin flags:
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
					auto Content = PaletteUpgrade::ToItem(MinecartContent.m_ItemType).first;
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





void cProtocol_1_8_0::WriteEntityProperties(cPacketizer & a_Pkt, const cEntity & a_Entity) const
{
	if (a_Entity.IsPlayer())
	{
		const auto & Player = static_cast<const cPlayer &>(a_Entity);

		a_Pkt.WriteBEInt32(1);  // Count.
		a_Pkt.WriteString("generic.movementSpeed");
		a_Pkt.WriteBEDouble(0.1 * Player.GetNormalMaxSpeed());  // The default game speed is 0.1, multiply that value by the relative speed.

		// It seems the modifiers aren't conditionally activated; their effects are applied immediately!
		// We have to keep on re-sending this packet when the client notifies us of sprint start and end, and so on. Strange.

		if (Player.IsSprinting())
		{
			a_Pkt.WriteVarInt32(1);  // Modifier count.
			a_Pkt.WriteBEUInt64(0x662a6b8dda3e4c1c);
			a_Pkt.WriteBEUInt64(0x881396ea6097278d);  // UUID of the modifier (sprinting speed boost).
			a_Pkt.WriteBEDouble(Player.GetSprintingMaxSpeed() - Player.GetNormalMaxSpeed());
			a_Pkt.WriteBEUInt8(2);
		}
		else
		{
			a_Pkt.WriteVarInt32(0);
		}
	}
	else
	{
		// const cMonster & Mob = (const cMonster &)a_Entity;

		// TODO: Send properties and modifiers based on the mob type

		a_Pkt.WriteBEInt32(0);
	}
}





void cProtocol_1_8_0::WriteItem(cPacketizer & a_Pkt, const cItem & a_Item) const
{
	short ItemType = PaletteUpgrade::ToItem(a_Item.m_ItemType).first;
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

	if (a_Item.m_Enchantments.IsEmpty() && a_Item.IsBothNameAndLoreEmpty() && (a_Item.m_ItemType != Item::FireworkRocket) && (a_Item.m_ItemType != Item::FireworkStar) && !a_Item.m_ItemColor.IsValid())
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
		const char * TagName = (a_Item.m_ItemType == Item::EnchantedBook) ? "StoredEnchantments" : "ench";
		EnchantmentSerializer::WriteToNBTCompound(a_Item.m_Enchantments, Writer, TagName, false);
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
	if ((a_Item.m_ItemType == Item::FireworkRocket) || (a_Item.m_ItemType == Item::FireworkStar))
	{
		cFireworkItem::WriteToNBTCompound(a_Item.m_FireworkItem, Writer, a_Item.m_ItemType);
	}
	Writer.Finish();

	const auto Result = Writer.GetResult();
	if (Result.empty())
	{
		a_Pkt.WriteBEInt8(0);
		return;
	}
	a_Pkt.WriteBuf(Result);
}





void cProtocol_1_8_0::WriteMobMetadata(cPacketizer & a_Pkt, const cMonster & a_Mob) const
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
			auto NumericBlock = PaletteUpgrade::ToBlock(Enderman.GetCarriedBlock());
			a_Pkt.WriteBEInt16(static_cast<Byte>(NumericBlock.first));
			a_Pkt.WriteBEUInt8(0x11);
			a_Pkt.WriteBEUInt8(static_cast<Byte>(NumericBlock.second));
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

		case mtSkeleton:
		case mtStray:
		{
			a_Pkt.WriteBEUInt8(0x0d);
			a_Pkt.WriteBEUInt8(0);  // Is normal skeleton
			break;
		}

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

		case mtHusk:
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

		case mtBlaze:
		case mtElderGuardian:
		case mtGuardian:
		{
			// TODO: Mobs with extra fields that aren't implemented
			break;
		}

		case mtCat:


		case mtDonkey:
		case mtMule:
		case mtSkeletonHorse:
		case mtZombieHorse:
		{
			// Todo: Mobs not added yet. Grouped ones have the same metadata
			ASSERT(!"cProtocol_1_8::WriteMobMetadata: received unimplemented type");
			break;
		}

		case mtCaveSpider:
		case mtEnderDragon:
		case mtGiant:
		case mtIronGolem:
		case mtMooshroom:
		case mtSilverfish:
		case mtEndermite:
		case mtSnowGolem:
		case mtSpider:
		case mtSquid:
		{
			// Allowed mobs without additional metadata
			break;
		}

		default: UNREACHABLE("cProtocol_1_8::WriteMobMetadata: received mob of invalid type");
	}  // switch (a_Mob.GetType())
}





void cProtocol_1_8_0::AddReceivedData(cByteBuffer & a_Buffer, const ContiguousByteBufferView a_Data)
{
	// Write the incoming data into the comm log file:
	if (g_ShouldLogCommIn && m_CommLogFile.IsOpen())
	{
		if (a_Buffer.GetReadableSpace() > 0)
		{
			ContiguousByteBuffer AllData;
			size_t OldReadableSpace = a_Buffer.GetReadableSpace();
			a_Buffer.ReadAll(AllData);
			a_Buffer.ResetRead();
			a_Buffer.SkipRead(a_Buffer.GetReadableSpace() - OldReadableSpace);
			ASSERT(a_Buffer.GetReadableSpace() == OldReadableSpace);
			AString Hex;
			CreateHexDump(Hex, AllData.data(), AllData.size(), 16);
			m_CommLogFile.Write(fmt::format(
				FMT_STRING("Incoming data, {0} (0x{0:x}) unparsed bytes already present in buffer:\n{1}\n"),
				AllData.size(), Hex
			));
		}
		AString Hex;
		CreateHexDump(Hex, a_Data.data(), a_Data.size(), 16);
		m_CommLogFile.Write(fmt::format(
			FMT_STRING("Incoming data: {0} (0x{0:x}) bytes: \n{1}\n"),
			a_Data.size(), Hex
		));
		m_CommLogFile.Flush();
	}

	if (!a_Buffer.Write(a_Data.data(), a_Data.size()))
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
		if ((m_State == 3) || m_CompressionEnabled)
		{
			UInt32 NumBytesRead = static_cast<UInt32>(a_Buffer.GetReadableSpace());

			UInt32 UncompressedSize;
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
				m_Extractor.ReadFrom(a_Buffer, PacketLen);
				a_Buffer.CommitRead();

				const auto UncompressedData = m_Extractor.Extract(UncompressedSize);
				const auto Uncompressed = UncompressedData.GetView();
				cByteBuffer bb(Uncompressed.size());

				// Compression was used, move the uncompressed data:
				VERIFY(bb.Write(Uncompressed.data(), Uncompressed.size()));

				HandlePacket(bb);
				continue;
			}
		}

		// Move the packet payload to a separate cByteBuffer, bb:
		cByteBuffer bb(PacketLen);

		// No compression was used, move directly:
		VERIFY(a_Buffer.ReadToByteBuffer(bb, static_cast<size_t>(PacketLen)));
		a_Buffer.CommitRead();

		HandlePacket(bb);
	}  // for (ever)

	// Log any leftover bytes into the logfile:
	if (g_ShouldLogCommIn && (a_Buffer.GetReadableSpace() > 0) && m_CommLogFile.IsOpen())
	{
		ContiguousByteBuffer AllData;
		size_t OldReadableSpace = a_Buffer.GetReadableSpace();
		a_Buffer.ReadAll(AllData);
		a_Buffer.ResetRead();
		a_Buffer.SkipRead(a_Buffer.GetReadableSpace() - OldReadableSpace);
		ASSERT(a_Buffer.GetReadableSpace() == OldReadableSpace);
		AString Hex;
		CreateHexDump(Hex, AllData.data(), AllData.size(), 16);
		m_CommLogFile.Write(fmt::format(
			FMT_STRING("There are {0} (0x{0:x}) bytes of non-parse-able data left in the buffer:\n{1}"),
			a_Buffer.GetReadableSpace(), Hex
		));
		m_CommLogFile.Flush();
	}
}





UInt8 cProtocol_1_8_0::GetProtocolEntityType(const cEntity & a_Entity) const
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
			break;
		}
		case Type::etFloater: return 90;
		case Type::etItemFrame: return 71;
		case Type::etLeashKnot: return 77;

		// Non-objects must not be sent
		case Type::etEntity:
		case Type::etPlayer:
		case Type::etMonster:
		case Type::etExpOrb:
		case Type::etPainting: break;
	}
	UNREACHABLE("Unhandled entity kind");
}





int cProtocol_1_8_0::GetProtocolParticleID(const AString & a_ParticleName) const
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





const char * cProtocol_1_8_0::GetProtocolStatisticName(const CustomStatistic a_Statistic)
{
	switch (a_Statistic)
	{
		// V1.8 Achievements
		case CustomStatistic::AchOpenInventory:        return "achievement.openInventory";
		case CustomStatistic::AchMineWood:             return "achievement.mineWood";
		case CustomStatistic::AchBuildWorkBench:       return "achievement.buildWorkBench";
		case CustomStatistic::AchBuildPickaxe:         return "achievement.buildPickaxe";
		case CustomStatistic::AchBuildFurnace:         return "achievement.buildFurnace";
		case CustomStatistic::AchAcquireIron:          return "achievement.acquireIron";
		case CustomStatistic::AchBuildHoe:             return "achievement.buildHoe";
		case CustomStatistic::AchMakeBread:            return "achievement.makeBread";
		case CustomStatistic::AchBakeCake:             return "achievement.bakeCake";
		case CustomStatistic::AchBuildBetterPickaxe:   return "achievement.buildBetterPickaxe";
		case CustomStatistic::AchCookFish:             return "achievement.cookFish";
		case CustomStatistic::AchOnARail:              return "achievement.onARail";
		case CustomStatistic::AchBuildSword:           return "achievement.buildSword";
		case CustomStatistic::AchKillEnemy:            return "achievement.killEnemy";
		case CustomStatistic::AchKillCow:              return "achievement.killCow";
		case CustomStatistic::AchFlyPig:               return "achievement.flyPig";
		case CustomStatistic::AchSnipeSkeleton:        return "achievement.snipeSkeleton";
		case CustomStatistic::AchDiamonds:             return "achievement.diamonds";
		case CustomStatistic::AchPortal:               return "achievement.portal";
		case CustomStatistic::AchGhast:                return "achievement.ghast";
		case CustomStatistic::AchBlazeRod:             return "achievement.blazeRod";
		case CustomStatistic::AchPotion:               return "achievement.potion";
		case CustomStatistic::AchTheEnd:               return "achievement.theEnd";
		case CustomStatistic::AchTheEnd2:              return "achievement.theEnd2";
		case CustomStatistic::AchEnchantments:         return "achievement.enchantments";
		case CustomStatistic::AchOverkill:             return "achievement.overkill";
		case CustomStatistic::AchBookcase:             return "achievement.bookcase";
		case CustomStatistic::AchExploreAllBiomes:     return "achievement.exploreAllBiomes";
		case CustomStatistic::AchSpawnWither:          return "achievement.spawnWither";
		case CustomStatistic::AchKillWither:           return "achievement.killWither";
		case CustomStatistic::AchFullBeacon:           return "achievement.fullBeacon";
		case CustomStatistic::AchBreedCow:             return "achievement.breedCow";
		case CustomStatistic::AchDiamondsToYou:        return "achievement.diamondsToYou";

		// V1.8 stats
		case CustomStatistic::AnimalsBred:               return "stat.animalsBred";
		case CustomStatistic::BoatOneCm:                 return "stat.boatOneCm";
		case CustomStatistic::ClimbOneCm:                return "stat.climbOneCm";
		case CustomStatistic::CrouchOneCm:               return "stat.crouchOneCm";
		case CustomStatistic::DamageDealt:               return "stat.damageDealt";
		case CustomStatistic::DamageTaken:               return "stat.damageTaken";
		case CustomStatistic::Deaths:                    return "stat.deaths";
		case CustomStatistic::Drop:                      return "stat.drop";
		case CustomStatistic::FallOneCm:                 return "stat.fallOneCm";
		case CustomStatistic::FishCaught:                return "stat.fishCaught";
		case CustomStatistic::FlyOneCm:                  return "stat.flyOneCm";
		case CustomStatistic::HorseOneCm:                return "stat.horseOneCm";
		case CustomStatistic::Jump:                      return "stat.jump";
		case CustomStatistic::LeaveGame:                 return "stat.leaveGame";
		case CustomStatistic::MinecartOneCm:             return "stat.minecartOneCm";
		case CustomStatistic::MobKills:                  return "stat.mobKills";
		case CustomStatistic::PigOneCm:                  return "stat.pigOneCm";
		case CustomStatistic::PlayerKills:               return "stat.playerKills";
		case CustomStatistic::PlayOneMinute:             return "stat.playOneMinute";
		case CustomStatistic::SprintOneCm:               return "stat.sprintOneCm";
		case CustomStatistic::SwimOneCm:                 return "stat.swimOneCm";
		case CustomStatistic::TalkedToVillager:          return "stat.talkedToVillager";
		case CustomStatistic::TimeSinceDeath:            return "stat.timeSinceDeath";
		case CustomStatistic::TradedWithVillager:        return "stat.tradedWithVillager";
		case CustomStatistic::WalkOneCm:                 return "stat.walkOneCm";
		case CustomStatistic::WalkUnderWaterOneCm:       return "stat.diveOneCm";

		// V1.8.2 stats
		case CustomStatistic::CleanArmor:                return "stat.armorCleaned";
		case CustomStatistic::CleanBanner:               return "stat.bannerCleaned";
		case CustomStatistic::EatCakeSlice:              return "stat.cakeSlicesEaten";
		case CustomStatistic::EnchantItem:               return "stat.itemEnchanted";
		case CustomStatistic::FillCauldron:              return "stat.cauldronFilled";
		case CustomStatistic::InspectDispenser:          return "stat.dispenserInspected";
		case CustomStatistic::InspectDropper:            return "stat.dropperInspected";
		case CustomStatistic::InspectHopper:             return "stat.hopperInspected";
		case CustomStatistic::InteractWithBeacon:        return "stat.beaconInteraction";
		case CustomStatistic::InteractWithBrewingstand:  return "stat.brewingstandInteraction";
		case CustomStatistic::InteractWithCraftingTable: return "stat.craftingTableInteraction";
		case CustomStatistic::InteractWithFurnace:       return "stat.furnaceInteraction";
		case CustomStatistic::OpenChest:                 return "stat.chestOpened";
		case CustomStatistic::OpenEnderchest:            return "stat.enderchestOpened";
		case CustomStatistic::PlayNoteblock:             return "stat.noteblockPlayed";
		case CustomStatistic::PlayRecord:                return "stat.recordPlayed";
		case CustomStatistic::PotFlower:                 return "stat.flowerPotted";
		case CustomStatistic::TriggerTrappedChest:       return "stat.trappedChestTriggered";
		case CustomStatistic::TuneNoteblock:             return "stat.noteblockTuned";
		case CustomStatistic::UseCauldron:               return "stat.cauldronUsed";

		// V1.9 stats
		case CustomStatistic::AviateOneCm:               return "stat.aviateOneCm";
		case CustomStatistic::SleepInBed:                return "stat.sleepInBed";
		case CustomStatistic::SneakTime:                 return "stat.sneakTime";
		default:                                   return "";
	}
}





void cProtocol_1_8_0::HandlePacket(cByteBuffer & a_Buffer)
{
	UInt32 PacketType;
	if (!a_Buffer.ReadVarInt(PacketType))
	{
		// Not enough data
		return;
	}

	// Log the packet info into the comm log file:
	if (g_ShouldLogCommIn && m_CommLogFile.IsOpen())
	{
		ContiguousByteBuffer PacketData;
		a_Buffer.ReadAll(PacketData);
		a_Buffer.ResetRead();
		a_Buffer.ReadVarInt(PacketType);  // We have already read the packet type once, it will be there again
		ASSERT(PacketData.size() > 0);  // We have written an extra NUL, so there had to be at least one byte read
		PacketData.resize(PacketData.size() - 1);
		AString PacketDataHex;
		CreateHexDump(PacketDataHex, PacketData.data(), PacketData.size(), 16);
		m_CommLogFile.Write(fmt::format(
			FMT_STRING("Next incoming packet is type {0} (0x{0:x}), length {1} (0x{1:x}) at state {2}. Payload:\n{3}\n"),
			PacketType, a_Buffer.GetUsedSpace(), m_State, PacketDataHex
		));
	}

	cProtocol::State oldstate = m_State;

	if (!HandlePacket(a_Buffer, PacketType))
	{
		// Unknown packet, already been reported, but without the length. Log the length here:
		LOGWARNING("Unhandled packet: type 0x%x, state %d, length %u", PacketType, oldstate, a_Buffer.GetUsedSpace());

#ifndef NDEBUG
		// Dump the packet contents into the log:
		a_Buffer.ResetRead();
		ContiguousByteBuffer Packet;
		a_Buffer.ReadAll(Packet);
		Packet.resize(Packet.size() - 1);  // Drop the final NUL pushed there for over-read detection
		AString Out;
		CreateHexDump(Out, Packet.data(), Packet.size(), 24);
		LOGD("Packet contents:\n%s", Out.c_str());
#endif  // !NDEBUG

		// Put a message in the comm log:
		if (g_ShouldLogCommIn && m_CommLogFile.IsOpen())
		{
			m_CommLogFile.Write("^^^^^^ Unhandled packet ^^^^^^\n\n\n");
		}

		return;
	}

	// The packet should have nothing left in the buffer:
	if (a_Buffer.GetReadableSpace() != 0)
	{
		// Read more or less than packet length, report as error
		LOGWARNING("Protocol 1.8: Wrong number of bytes read for packet 0x%x, state %d. Read %zu bytes, packet contained %u bytes",
			PacketType, oldstate, a_Buffer.GetUsedSpace() - a_Buffer.GetReadableSpace(), a_Buffer.GetUsedSpace()
		);

		// Put a message in the comm log:
		if (g_ShouldLogCommIn && m_CommLogFile.IsOpen())
		{
			m_CommLogFile.Write(fmt::format(
				FMT_STRING("^^^^^^ Wrong number of bytes read for this packet (exp 1 left, got {} left) ^^^^^^\n\n\n"),
				a_Buffer.GetReadableSpace()
			));
			m_CommLogFile.Flush();
		}

		ASSERT(!"Read wrong number of bytes!");
		m_Client->PacketError(PacketType);
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





void cProtocol_1_8_0::SendPlayerActionResponse(Vector3i a_blockpos, int a_state_id, cProtocol::PlayerActionResponses a_action, bool a_IsApproved)
{
	// Used by 1.15+
}





void cProtocol_1_8_0::SendSelectKnownPacks()
{
	// used in 1.20.5+
}





void cProtocol_1_8_0::SendInitialChunksComing()
{
	// used in 1.20.3+
}
