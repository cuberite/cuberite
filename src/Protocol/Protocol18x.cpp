
// Protocol18x.cpp

/*
Implements the 1.8.x protocol classes:
	- cProtocol180
		- release 1.8.0 protocol (#47)
(others may be added later in the future for the 1.8 release series)
*/

#include "Globals.h"
#include "json/json.h"
#include "Protocol18x.h"
#include "ChunkDataSerializer.h"
#include "PolarSSL++/Sha1Checksum.h"
#include "Packetizer.h"

#include "../ClientHandle.h"
#include "../Root.h"
#include "../Server.h"
#include "../World.h"
#include "../EffectID.h"
#include "../StringCompression.h"
#include "../CompositeChat.h"
#include "../Statistics.h"

#include "../WorldStorage/FastNBT.h"
#include "../WorldStorage/EnchantmentSerializer.h"

#include "../Entities/ExpOrb.h"
#include "../Entities/Minecart.h"
#include "../Entities/FallingBlock.h"
#include "../Entities/Painting.h"
#include "../Entities/Pickup.h"
#include "../Entities/Player.h"
#include "../Entities/ItemFrame.h"
#include "../Entities/ArrowEntity.h"
#include "../Entities/FireworkEntity.h"

#include "../Mobs/IncludeAllMonsters.h"
#include "../UI/Window.h"

#include "../BlockEntities/BeaconEntity.h"
#include "../BlockEntities/CommandBlockEntity.h"
#include "../BlockEntities/MobHeadEntity.h"
#include "../BlockEntities/MobSpawnerEntity.h"
#include "../BlockEntities/FlowerPotEntity.h"
#include "Bindings/PluginManager.h"





/** The slot number that the client uses to indicate "outside the window". */
static const Int16 SLOT_NUM_OUTSIDE = -999;





#define HANDLE_READ(ByteBuf, Proc, Type, Var) \
	Type Var; \
	if (!ByteBuf.Proc(Var))\
	{\
		return;\
	}





#define HANDLE_PACKET_READ(ByteBuf, Proc, Type, Var) \
	Type Var; \
	{ \
		if (!ByteBuf.Proc(Var)) \
		{ \
			ByteBuf.CheckValid(); \
			return false; \
		} \
		ByteBuf.CheckValid(); \
	}





const int MAX_ENC_LEN = 512;  // Maximum size of the encrypted message; should be 128, but who knows...
const uLongf MAX_COMPRESSED_PACKET_LEN = 200 KiB;  // Maximum size of compressed packets.





// fwd: main.cpp:
extern bool g_ShouldLogCommIn, g_ShouldLogCommOut;





////////////////////////////////////////////////////////////////////////////////
// cProtocol180:

cProtocol180::cProtocol180(cClientHandle * a_Client, const AString & a_ServerAddress, UInt16 a_ServerPort, UInt32 a_State) :
	super(a_Client),
	m_ServerAddress(a_ServerAddress),
	m_ServerPort(a_ServerPort),
	m_State(a_State),
	m_ReceivedData(32 KiB),
	m_IsEncrypted(false),
	m_LastSentDimension(dimNotSet)
{

	// BungeeCord handling:
	// If BC is setup with ip_forward == true, it sends additional data in the login packet's ServerAddress field:
	// hostname\00ip-address\00uuid\00profile-properties-as-json
	AStringVector Params;
	if (cRoot::Get()->GetServer().ShouldAllowBungeeCord() && SplitZeroTerminatedStrings(a_ServerAddress, Params) && (Params.size() == 4))
	{
		LOGD("Player at %s connected via BungeeCord", Params[1].c_str());
		m_ServerAddress = Params[0];
		m_Client->SetIPString(Params[1]);
		m_Client->SetUUID(cMojangAPI::MakeUUIDShort(Params[2]));
		m_Client->SetProperties(Params[3]);
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





void cProtocol180::DataReceived(const char * a_Data, size_t a_Size)
{
	if (m_IsEncrypted)
	{
		Byte Decrypted[512];
		while (a_Size > 0)
		{
			size_t NumBytes = (a_Size > sizeof(Decrypted)) ? sizeof(Decrypted) : a_Size;
			m_Decryptor.ProcessData(Decrypted, reinterpret_cast<const Byte *>(a_Data), NumBytes);
			AddReceivedData(reinterpret_cast<const char *>(Decrypted), NumBytes);
			a_Size -= NumBytes;
			a_Data += NumBytes;
		}
	}
	else
	{
		AddReceivedData(a_Data, a_Size);
	}
}





void cProtocol180::SendAttachEntity(const cEntity & a_Entity, const cEntity * a_Vehicle)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, 0x1b);  // Attach Entity packet
	Pkt.WriteBEUInt32(a_Entity.GetUniqueID());
	Pkt.WriteBEUInt32((a_Vehicle != nullptr) ? a_Vehicle->GetUniqueID() : 0);
	Pkt.WriteBool(false);
}





void cProtocol180::SendBlockAction(int a_BlockX, int a_BlockY, int a_BlockZ, char a_Byte1, char a_Byte2, BLOCKTYPE a_BlockType)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, 0x24);  // Block Action packet
	Pkt.WritePosition64(a_BlockX, a_BlockY, a_BlockZ);
	Pkt.WriteBEInt8(a_Byte1);
	Pkt.WriteBEInt8(a_Byte2);
	Pkt.WriteVarInt32(a_BlockType);
}





void cProtocol180::SendBlockBreakAnim(UInt32 a_EntityID, int a_BlockX, int a_BlockY, int a_BlockZ, char a_Stage)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, 0x25);  // Block Break Animation packet
	Pkt.WriteVarInt32(a_EntityID);
	Pkt.WritePosition64(a_BlockX, a_BlockY, a_BlockZ);
	Pkt.WriteBEInt8(a_Stage);
}





void cProtocol180::SendBlockChange(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, 0x23);  // Block Change packet
	Pkt.WritePosition64(a_BlockX, a_BlockY, a_BlockZ);
	Pkt.WriteVarInt32((static_cast<UInt32>(a_BlockType) << 4) | (static_cast<UInt32>(a_BlockMeta) & 15));
}





void cProtocol180::SendBlockChanges(int a_ChunkX, int a_ChunkZ, const sSetBlockVector & a_Changes)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, 0x22);  // Multi Block Change packet
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





void cProtocol180::SendChat(const AString & a_Message, eChatType a_Type)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, 0x02);  // Chat Message packet
	Pkt.WriteString(Printf("{\"text\":\"%s\"}", EscapeString(a_Message).c_str()));
	Pkt.WriteBEInt8(a_Type);
}





void cProtocol180::SendChat(const cCompositeChat & a_Message, eChatType a_Type, bool a_ShouldUseChatPrefixes)
{
	ASSERT(m_State == 3);  // In game mode?


	// Send the message to the client:
	cPacketizer Pkt(*this, 0x02);
	Pkt.WriteString(a_Message.CreateJsonString(a_ShouldUseChatPrefixes));
	Pkt.WriteBEInt8(a_Type);
}





void cProtocol180::SendChunkData(int a_ChunkX, int a_ChunkZ, cChunkDataSerializer & a_Serializer)
{
	ASSERT(m_State == 3);  // In game mode?

	// Serialize first, before creating the Packetizer (the packetizer locks a CS)
	// This contains the flags and bitmasks, too
	const AString & ChunkData = a_Serializer.Serialize(cChunkDataSerializer::RELEASE_1_8_0, a_ChunkX, a_ChunkZ);

	cCSLock Lock(m_CSPacket);
	SendData(ChunkData.data(), ChunkData.size());
}





void cProtocol180::SendCollectEntity(const cEntity & a_Entity, const cPlayer & a_Player)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, 0x0d);  // Collect Item packet
	Pkt.WriteVarInt32(a_Entity.GetUniqueID());
	Pkt.WriteVarInt32(a_Player.GetUniqueID());
}





void cProtocol180::SendDestroyEntity(const cEntity & a_Entity)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, 0x13);  // Destroy Entities packet
	Pkt.WriteVarInt32(1);
	Pkt.WriteVarInt32(a_Entity.GetUniqueID());
}





void cProtocol180::SendDisconnect(const AString & a_Reason)
{
	switch (m_State)
	{
		case 2:
		{
			// During login:
			cPacketizer Pkt(*this, 0);
			Pkt.WriteString(Printf("{\"text\":\"%s\"}", EscapeString(a_Reason).c_str()));
			break;
		}
		case 3:
		{
			// In-game:
			cPacketizer Pkt(*this, 0x40);
			Pkt.WriteString(Printf("{\"text\":\"%s\"}", EscapeString(a_Reason).c_str()));
			break;
		}
	}
}





void cProtocol180::SendEditSign(int a_BlockX, int a_BlockY, int a_BlockZ)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, 0x36);  // Sign Editor Open packet
	Pkt.WritePosition64(a_BlockX, a_BlockY, a_BlockZ);
}





void cProtocol180::SendEntityEffect(const cEntity & a_Entity, int a_EffectID, int a_Amplifier, short a_Duration)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, 0x1D);  // Entity Effect packet
	Pkt.WriteVarInt32(a_Entity.GetUniqueID());
	Pkt.WriteBEUInt8(static_cast<UInt8>(a_EffectID));
	Pkt.WriteBEUInt8(static_cast<UInt8>(a_Amplifier));
	Pkt.WriteVarInt32(static_cast<UInt32>(a_Duration));
	Pkt.WriteBool(false);  // Hide particles
}





void cProtocol180::SendEntityEquipment(const cEntity & a_Entity, short a_SlotNum, const cItem & a_Item)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, 0x04);  // Entity Equipment packet
	Pkt.WriteVarInt32(a_Entity.GetUniqueID());
	Pkt.WriteBEInt16(a_SlotNum);
	WriteItem(Pkt, a_Item);
}





void cProtocol180::SendEntityHeadLook(const cEntity & a_Entity)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, 0x19);  // Entity Head Look packet
	Pkt.WriteVarInt32(a_Entity.GetUniqueID());
	Pkt.WriteByteAngle(a_Entity.GetHeadYaw());
}





void cProtocol180::SendEntityLook(const cEntity & a_Entity)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, 0x16);  // Entity Look packet
	Pkt.WriteVarInt32(a_Entity.GetUniqueID());
	Pkt.WriteByteAngle(a_Entity.GetYaw());
	Pkt.WriteByteAngle(a_Entity.GetPitch());
	Pkt.WriteBool(a_Entity.IsOnGround());
}





void cProtocol180::SendEntityMetadata(const cEntity & a_Entity)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, 0x1c);  // Entity Metadata packet
	Pkt.WriteVarInt32(a_Entity.GetUniqueID());
	WriteEntityMetadata(Pkt, a_Entity);
	Pkt.WriteBEUInt8(0x7f);  // The termination byte
}





void cProtocol180::SendEntityProperties(const cEntity & a_Entity)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, 0x20);  // Entity Properties packet
	Pkt.WriteVarInt32(a_Entity.GetUniqueID());
	WriteEntityProperties(Pkt, a_Entity);
}





void cProtocol180::SendEntityRelMove(const cEntity & a_Entity, char a_RelX, char a_RelY, char a_RelZ)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, 0x15);  // Entity Relative Move packet
	Pkt.WriteVarInt32(a_Entity.GetUniqueID());
	Pkt.WriteBEInt8(a_RelX);
	Pkt.WriteBEInt8(a_RelY);
	Pkt.WriteBEInt8(a_RelZ);
	Pkt.WriteBool(a_Entity.IsOnGround());
}





void cProtocol180::SendEntityRelMoveLook(const cEntity & a_Entity, char a_RelX, char a_RelY, char a_RelZ)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, 0x17);  // Entity Look And Relative Move packet
	Pkt.WriteVarInt32(a_Entity.GetUniqueID());
	Pkt.WriteBEInt8(a_RelX);
	Pkt.WriteBEInt8(a_RelY);
	Pkt.WriteBEInt8(a_RelZ);
	Pkt.WriteByteAngle(a_Entity.GetYaw());
	Pkt.WriteByteAngle(a_Entity.GetPitch());
	Pkt.WriteBool(a_Entity.IsOnGround());
}





void cProtocol180::SendEntityStatus(const cEntity & a_Entity, char a_Status)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, 0x1a);  // Entity Status packet
	Pkt.WriteBEUInt32(a_Entity.GetUniqueID());
	Pkt.WriteBEInt8(a_Status);
}





void cProtocol180::SendEntityVelocity(const cEntity & a_Entity)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, 0x12);  // Entity Velocity packet
	Pkt.WriteVarInt32(a_Entity.GetUniqueID());
	// 400 = 8000 / 20 ... Conversion from our speed in m / s to 8000 m / tick
	Pkt.WriteBEInt16(static_cast<Int16>(a_Entity.GetSpeedX() * 400));
	Pkt.WriteBEInt16(static_cast<Int16>(a_Entity.GetSpeedY() * 400));
	Pkt.WriteBEInt16(static_cast<Int16>(a_Entity.GetSpeedZ() * 400));
}





void cProtocol180::SendExplosion(double a_BlockX, double a_BlockY, double a_BlockZ, float a_Radius, const cVector3iArray & a_BlocksAffected, const Vector3d & a_PlayerMotion)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, 0x27);  // Explosion packet
	Pkt.WriteBEFloat(static_cast<float>(a_BlockX));
	Pkt.WriteBEFloat(static_cast<float>(a_BlockY));
	Pkt.WriteBEFloat(static_cast<float>(a_BlockZ));
	Pkt.WriteBEFloat(static_cast<float>(a_Radius));
	Pkt.WriteBEUInt32(static_cast<UInt32>(a_BlocksAffected.size()));
	for (cVector3iArray::const_iterator itr = a_BlocksAffected.begin(), end = a_BlocksAffected.end(); itr != end; ++itr)
	{
		Pkt.WriteBEInt8(static_cast<Int8>(itr->x));
		Pkt.WriteBEInt8(static_cast<Int8>(itr->y));
		Pkt.WriteBEInt8(static_cast<Int8>(itr->z));
	}  // for itr - a_BlockAffected[]
	Pkt.WriteBEFloat(static_cast<float>(a_PlayerMotion.x));
	Pkt.WriteBEFloat(static_cast<float>(a_PlayerMotion.y));
	Pkt.WriteBEFloat(static_cast<float>(a_PlayerMotion.z));
}





void cProtocol180::SendGameMode(eGameMode a_GameMode)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, 0x2b);  // Change Game State packet
	Pkt.WriteBEUInt8(3);  // Reason: Change game mode
	Pkt.WriteBEFloat(static_cast<float>(a_GameMode));  // The protocol really represents the value with a float!
}





void cProtocol180::SendHealth(void)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, 0x06);  // Update Health packet
	cPlayer * Player = m_Client->GetPlayer();
	Pkt.WriteBEFloat(static_cast<float>(Player->GetHealth()));
	Pkt.WriteVarInt32(static_cast<UInt32>(Player->GetFoodLevel()));
	Pkt.WriteBEFloat(static_cast<float>(Player->GetFoodSaturationLevel()));
}





void cProtocol180::SendHideTitle(void)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, 0x45);  // Title packet
	Pkt.WriteVarInt32(3);  // Hide title
}





void cProtocol180::SendInventorySlot(char a_WindowID, short a_SlotNum, const cItem & a_Item)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, 0x2f);  // Set Slot packet
	Pkt.WriteBEInt8(a_WindowID);
	Pkt.WriteBEInt16(a_SlotNum);
	WriteItem(Pkt, a_Item);
}





void cProtocol180::SendKeepAlive(UInt32 a_PingID)
{
	// Drop the packet if the protocol is not in the Game state yet (caused a client crash):
	if (m_State != 3)
	{
		LOGWARNING("Trying to send a KeepAlive packet to a player who's not yet fully logged in (%d). The protocol class prevented the packet.", m_State);
		return;
	}

	cPacketizer Pkt(*this, 0x00);  // Keep Alive packet
	Pkt.WriteVarInt32(a_PingID);
}





void cProtocol180::SendLogin(const cPlayer & a_Player, const cWorld & a_World)
{
	// Send the Join Game packet:
	{
		auto & Server = cRoot::Get()->GetServer();
		cPacketizer Pkt(*this, 0x01);  // Join Game packet
		Pkt.WriteBEUInt32(a_Player.GetUniqueID());
		Pkt.WriteBEUInt8(static_cast<UInt8>(a_Player.GetEffectiveGameMode()) | (Server.IsHardcore() ? 0x08 : 0));  // Hardcore flag bit 4
		Pkt.WriteBEInt8(static_cast<Int8>(a_World.GetDimension()));
		Pkt.WriteBEUInt8(2);  // TODO: Difficulty (set to Normal)
		Pkt.WriteBEUInt8(static_cast<UInt8>(Clamp<int>(Server.GetMaxPlayers(), 0, 255)));
		Pkt.WriteString("default");  // Level type - wtf?
		Pkt.WriteBool(false);  // Reduced Debug Info - wtf?
	}
	m_LastSentDimension = a_World.GetDimension();

	// Send the spawn position:
	{
		cPacketizer Pkt(*this, 0x05);  // Spawn Position packet
		Pkt.WritePosition64(FloorC(a_World.GetSpawnX()), FloorC(a_World.GetSpawnY()), FloorC(a_World.GetSpawnZ()));
	}

	// Send the server difficulty:
	{
		cPacketizer Pkt(*this, 0x41);
		Pkt.WriteBEInt8(1);
	}

	// Send player abilities:
	SendPlayerAbilities();
}




void cProtocol180::SendLoginSuccess(void)
{
	ASSERT(m_State == 2);  // State: login?

	// Enable compression:
	{
		cPacketizer Pkt(*this, 0x03);  // Set compression packet
		Pkt.WriteVarInt32(256);
	}

	m_State = 3;  // State = Game

	{
		cPacketizer Pkt(*this, 0x02);  // Login success packet
		Pkt.WriteString(cMojangAPI::MakeUUIDDashed(m_Client->GetUUID()));
		Pkt.WriteString(m_Client->GetUsername());
	}
}





void cProtocol180::SendPaintingSpawn(const cPainting & a_Painting)
{
	ASSERT(m_State == 3);  // In game mode?
	double PosX = a_Painting.GetPosX();
	double PosY = a_Painting.GetPosY();
	double PosZ = a_Painting.GetPosZ();

	cPacketizer Pkt(*this, 0x10);  // Spawn Painting packet
	Pkt.WriteVarInt32(a_Painting.GetUniqueID());
	Pkt.WriteString(a_Painting.GetName().c_str());
	Pkt.WritePosition64(static_cast<Int32>(PosX), static_cast<Int32>(PosY), static_cast<Int32>(PosZ));
	Pkt.WriteBEInt8(static_cast<Int8>(a_Painting.GetProtocolFacing()));
}





void cProtocol180::SendMapData(const cMap & a_Map, int a_DataStartX, int a_DataStartY)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, 0x34);
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





void cProtocol180::SendPickupSpawn(const cPickup & a_Pickup)
{
	ASSERT(m_State == 3);  // In game mode?

	{
		cPacketizer Pkt(*this, 0x0e);  // Spawn Object packet
		Pkt.WriteVarInt32(a_Pickup.GetUniqueID());
		Pkt.WriteBEUInt8(2);  // Type = Pickup
		Pkt.WriteFPInt(a_Pickup.GetPosX());
		Pkt.WriteFPInt(a_Pickup.GetPosY());
		Pkt.WriteFPInt(a_Pickup.GetPosZ());
		Pkt.WriteByteAngle(a_Pickup.GetYaw());
		Pkt.WriteByteAngle(a_Pickup.GetPitch());
		Pkt.WriteBEInt32(0);  // No object data
	}

	{
		cPacketizer Pkt(*this, 0x1c);  // Entity Metadata packet
		Pkt.WriteVarInt32(a_Pickup.GetUniqueID());
		Pkt.WriteBEUInt8((0x05 << 5) | 10);  // Slot type + index 10
		WriteItem(Pkt, a_Pickup.GetItem());
		Pkt.WriteBEUInt8(0x7f);  // End of metadata
	}
}





void cProtocol180::SendPlayerAbilities(void)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, 0x39);  // Player Abilities packet
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





void cProtocol180::SendEntityAnimation(const cEntity & a_Entity, char a_Animation)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, 0x0b);  // Animation packet
	Pkt.WriteVarInt32(a_Entity.GetUniqueID());
	Pkt.WriteBEInt8(a_Animation);
}





void cProtocol180::SendParticleEffect(const AString & a_ParticleName, float a_SrcX, float a_SrcY, float a_SrcZ, float a_OffsetX, float a_OffsetY, float a_OffsetZ, float a_ParticleData, int a_ParticleAmount)
{
	ASSERT(m_State == 3);  // In game mode?
	int ParticleID = GetParticleID(a_ParticleName);

	cPacketizer Pkt(*this, 0x2A);
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





void cProtocol180::SendParticleEffect(const AString & a_ParticleName, Vector3f a_Src, Vector3f a_Offset, float a_ParticleData, int a_ParticleAmount, std::array<int, 2> a_Data)
{
	ASSERT(m_State == 3);  // In game mode?
	int ParticleID = GetParticleID(a_ParticleName);

	cPacketizer Pkt(*this, 0x2A);
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





void cProtocol180::SendPlayerListAddPlayer(const cPlayer & a_Player)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, 0x38);  // Playerlist Item packet
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

	Pkt.WriteVarInt32(static_cast<UInt32>(a_Player.GetGameMode()));
	Pkt.WriteVarInt32(static_cast<UInt32>(a_Player.GetClientHandle()->GetPing()));
	Pkt.WriteBool(false);
}





void cProtocol180::SendPlayerListRemovePlayer(const cPlayer & a_Player)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, 0x38);  // Playerlist Item packet
	Pkt.WriteVarInt32(4);
	Pkt.WriteVarInt32(1);
	Pkt.WriteUUID(a_Player.GetUUID());
}





void cProtocol180::SendPlayerListUpdateGameMode(const cPlayer & a_Player)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, 0x38);  // Playerlist Item packet
	Pkt.WriteVarInt32(1);
	Pkt.WriteVarInt32(1);
	Pkt.WriteUUID(a_Player.GetUUID());
	Pkt.WriteVarInt32(static_cast<UInt32>(a_Player.GetGameMode()));
}





void cProtocol180::SendPlayerListUpdatePing(const cPlayer & a_Player)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, 0x38);  // Playerlist Item packet
	Pkt.WriteVarInt32(2);
	Pkt.WriteVarInt32(1);
	Pkt.WriteUUID(a_Player.GetUUID());
	Pkt.WriteVarInt32(static_cast<UInt32>(a_Player.GetClientHandle()->GetPing()));
}





void cProtocol180::SendPlayerListUpdateDisplayName(const cPlayer & a_Player, const AString & a_CustomName)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, 0x38);  // Playerlist Item packet
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





void cProtocol180::SendPlayerMaxSpeed(void)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, 0x20);  // Entity Properties
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





void cProtocol180::SendPlayerMoveLook(void)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, 0x08);  // Player Position And Look packet
	cPlayer * Player = m_Client->GetPlayer();
	Pkt.WriteBEDouble(Player->GetPosX());
	Pkt.WriteBEDouble(Player->GetPosY());
	Pkt.WriteBEDouble(Player->GetPosZ());
	Pkt.WriteBEFloat(static_cast<float>(Player->GetYaw()));
	Pkt.WriteBEFloat(static_cast<float>(Player->GetPitch()));
	Pkt.WriteBEUInt8(0);
}





void cProtocol180::SendPlayerPosition(void)
{
	// There is no dedicated packet for this, send the whole thing:
	SendPlayerMoveLook();
}





void cProtocol180::SendPlayerSpawn(const cPlayer & a_Player)
{
	// Called to spawn another player for the client
	cPacketizer Pkt(*this, 0x0c);  // Spawn Player packet
	Pkt.WriteVarInt32(a_Player.GetUniqueID());
	Pkt.WriteUUID(cMojangAPI::MakeUUIDShort(a_Player.GetUUID()));
	Pkt.WriteFPInt(a_Player.GetPosX());
	Pkt.WriteFPInt(a_Player.GetPosY() + 0.001);  // The "+ 0.001" is there because otherwise the player falls through the block they were standing on.
	Pkt.WriteFPInt(a_Player.GetPosZ());
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





void cProtocol180::SendPluginMessage(const AString & a_Channel, const AString & a_Message)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, 0x3f);
	Pkt.WriteString(a_Channel);
	Pkt.WriteBuf(a_Message.data(), a_Message.size());
}





void cProtocol180::SendRemoveEntityEffect(const cEntity & a_Entity, int a_EffectID)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, 0x1e);
	Pkt.WriteVarInt32(a_Entity.GetUniqueID());
	Pkt.WriteBEUInt8(static_cast<UInt8>(a_EffectID));
}





void cProtocol180::SendResetTitle(void)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, 0x45);  // Title packet
	Pkt.WriteVarInt32(4);  // Reset title
}





void cProtocol180::SendRespawn(eDimension a_Dimension, bool a_ShouldIgnoreDimensionChecks)
{
	if ((m_LastSentDimension == a_Dimension) && !a_ShouldIgnoreDimensionChecks)
	{
		// Must not send a respawn for the world with the same dimension, the client goes cuckoo if we do (unless we are respawning from death)
		return;
	}

	cPacketizer Pkt(*this, 0x07);  // Respawn packet
	cPlayer * Player = m_Client->GetPlayer();
	Pkt.WriteBEInt32(static_cast<Int32>(a_Dimension));
	Pkt.WriteBEUInt8(2);  // TODO: Difficulty (set to Normal)
	Pkt.WriteBEUInt8(static_cast<Byte>(Player->GetEffectiveGameMode()));
	Pkt.WriteString("default");
	m_LastSentDimension = a_Dimension;
}





void cProtocol180::SendExperience(void)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, 0x1f);  // Experience Packet
	cPlayer * Player = m_Client->GetPlayer();
	Pkt.WriteBEFloat(Player->GetXpPercentage());
	Pkt.WriteVarInt32(static_cast<UInt32>(Player->GetXpLevel()));
	Pkt.WriteVarInt32(static_cast<UInt32>(Player->GetCurrentXp()));
}





void cProtocol180::SendExperienceOrb(const cExpOrb & a_ExpOrb)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, 0x11);
	Pkt.WriteVarInt32(a_ExpOrb.GetUniqueID());
	Pkt.WriteFPInt(a_ExpOrb.GetPosX());
	Pkt.WriteFPInt(a_ExpOrb.GetPosY());
	Pkt.WriteFPInt(a_ExpOrb.GetPosZ());
	Pkt.WriteBEInt16(static_cast<Int16>(a_ExpOrb.GetReward()));
}





void cProtocol180::SendScoreboardObjective(const AString & a_Name, const AString & a_DisplayName, Byte a_Mode)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, 0x3b);
	Pkt.WriteString(a_Name);
	Pkt.WriteBEUInt8(a_Mode);
	if ((a_Mode == 0) || (a_Mode == 2))
	{
		Pkt.WriteString(a_DisplayName);
		Pkt.WriteString("integer");
	}
}





void cProtocol180::SendScoreUpdate(const AString & a_Objective, const AString & a_Player, cObjective::Score a_Score, Byte a_Mode)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, 0x3c);
	Pkt.WriteString(a_Player);
	Pkt.WriteBEUInt8(a_Mode);
	Pkt.WriteString(a_Objective);

	if (a_Mode != 1)
	{
		Pkt.WriteVarInt32(static_cast<UInt32>(a_Score));
	}
}





void cProtocol180::SendDisplayObjective(const AString & a_Objective, cScoreboard::eDisplaySlot a_Display)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, 0x3d);
	Pkt.WriteBEUInt8(static_cast<UInt8>(a_Display));
	Pkt.WriteString(a_Objective);
}





void cProtocol180::SendSetSubTitle(const cCompositeChat & a_SubTitle)
{
	SendSetRawSubTitle(a_SubTitle.CreateJsonString(false));
}





void cProtocol180::SendSetRawSubTitle(const AString & a_SubTitle)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, 0x45);  // Title packet
	Pkt.WriteVarInt32(1);  // Set subtitle

	Pkt.WriteString(a_SubTitle);
}





void cProtocol180::SendSetTitle(const cCompositeChat & a_Title)
{
	SendSetRawTitle(a_Title.CreateJsonString(false));
}





void cProtocol180::SendSetRawTitle(const AString & a_Title)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, 0x45);  // Title packet
	Pkt.WriteVarInt32(0);  // Set title

	Pkt.WriteString(a_Title);
}





void cProtocol180::SendSoundEffect(const AString & a_SoundName, double a_X, double a_Y, double a_Z, float a_Volume, float a_Pitch)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, 0x29);  // Sound Effect packet
	Pkt.WriteString(a_SoundName);
	Pkt.WriteBEInt32(static_cast<Int32>(a_X * 8.0));
	Pkt.WriteBEInt32(static_cast<Int32>(a_Y * 8.0));
	Pkt.WriteBEInt32(static_cast<Int32>(a_Z * 8.0));
	Pkt.WriteBEFloat(a_Volume);
	Pkt.WriteBEUInt8(static_cast<Byte>(a_Pitch * 63));
}





void cProtocol180::SendSoundParticleEffect(const EffectID a_EffectID, int a_SrcX, int a_SrcY, int a_SrcZ, int a_Data)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, 0x28);  // Effect packet
	Pkt.WriteBEInt32(static_cast<int>(a_EffectID));
	Pkt.WritePosition64(a_SrcX, a_SrcY, a_SrcZ);
	Pkt.WriteBEInt32(a_Data);
	Pkt.WriteBool(false);
}





void cProtocol180::SendSpawnFallingBlock(const cFallingBlock & a_FallingBlock)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, 0x0e);  // Spawn Object packet
	Pkt.WriteVarInt32(a_FallingBlock.GetUniqueID());
	Pkt.WriteBEUInt8(70);  // Falling block
	Pkt.WriteFPInt(a_FallingBlock.GetPosX());
	Pkt.WriteFPInt(a_FallingBlock.GetPosY());
	Pkt.WriteFPInt(a_FallingBlock.GetPosZ());
	Pkt.WriteByteAngle(a_FallingBlock.GetYaw());
	Pkt.WriteByteAngle(a_FallingBlock.GetPitch());
	Pkt.WriteBEInt32(static_cast<Int32>(a_FallingBlock.GetBlockType()) | (static_cast<Int32>(a_FallingBlock.GetBlockMeta()) << 12));
	Pkt.WriteBEInt16(static_cast<Int16>(a_FallingBlock.GetSpeedX() * 400));
	Pkt.WriteBEInt16(static_cast<Int16>(a_FallingBlock.GetSpeedY() * 400));
	Pkt.WriteBEInt16(static_cast<Int16>(a_FallingBlock.GetSpeedZ() * 400));
}





void cProtocol180::SendSpawnMob(const cMonster & a_Mob)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, 0x0f);  // Spawn Mob packet
	Pkt.WriteVarInt32(a_Mob.GetUniqueID());
	Pkt.WriteBEUInt8(static_cast<Byte>(a_Mob.GetMobType()));
	Pkt.WriteFPInt(a_Mob.GetPosX());
	Pkt.WriteFPInt(a_Mob.GetPosY());
	Pkt.WriteFPInt(a_Mob.GetPosZ());
	Pkt.WriteByteAngle(a_Mob.GetPitch());
	Pkt.WriteByteAngle(a_Mob.GetHeadYaw());
	Pkt.WriteByteAngle(a_Mob.GetYaw());
	Pkt.WriteBEInt16(static_cast<Int16>(a_Mob.GetSpeedX() * 400));
	Pkt.WriteBEInt16(static_cast<Int16>(a_Mob.GetSpeedY() * 400));
	Pkt.WriteBEInt16(static_cast<Int16>(a_Mob.GetSpeedZ() * 400));
	WriteEntityMetadata(Pkt, a_Mob);
	Pkt.WriteBEUInt8(0x7f);  // Metadata terminator
}





void cProtocol180::SendSpawnObject(const cEntity & a_Entity, char a_ObjectType, int a_ObjectData, Byte a_Yaw, Byte a_Pitch)
{
	ASSERT(m_State == 3);  // In game mode?
	double PosX = a_Entity.GetPosX();
	double PosZ = a_Entity.GetPosZ();
	double Yaw = a_Entity.GetYaw();
	if (a_ObjectType == 71)
	{
		FixItemFramePositions(a_ObjectData, PosX, PosZ, Yaw);
	}

	cPacketizer Pkt(*this, 0xe);  // Spawn Object packet
	Pkt.WriteVarInt32(a_Entity.GetUniqueID());
	Pkt.WriteBEUInt8(static_cast<UInt8>(a_ObjectType));
	Pkt.WriteFPInt(PosX);
	Pkt.WriteFPInt(a_Entity.GetPosY());
	Pkt.WriteFPInt(PosZ);
	Pkt.WriteByteAngle(a_Entity.GetPitch());
	Pkt.WriteByteAngle(Yaw);
	Pkt.WriteBEInt32(a_ObjectData);
	if (a_ObjectData != 0)
	{
		Pkt.WriteBEInt16(static_cast<Int16>(a_Entity.GetSpeedX() * 400));
		Pkt.WriteBEInt16(static_cast<Int16>(a_Entity.GetSpeedY() * 400));
		Pkt.WriteBEInt16(static_cast<Int16>(a_Entity.GetSpeedZ() * 400));
	}
}





void cProtocol180::SendSpawnVehicle(const cEntity & a_Vehicle, char a_VehicleType, char a_VehicleSubType)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, 0xe);  // Spawn Object packet
	Pkt.WriteVarInt32(a_Vehicle.GetUniqueID());
	Pkt.WriteBEUInt8(static_cast<UInt8>(a_VehicleType));
	Pkt.WriteFPInt(a_Vehicle.GetPosX());
	Pkt.WriteFPInt(a_Vehicle.GetPosY());
	Pkt.WriteFPInt(a_Vehicle.GetPosZ());
	Pkt.WriteByteAngle(a_Vehicle.GetPitch());
	Pkt.WriteByteAngle(a_Vehicle.GetYaw());
	Pkt.WriteBEInt32(a_VehicleSubType);
	if (a_VehicleSubType != 0)
	{
		Pkt.WriteBEInt16(static_cast<Int16>(a_Vehicle.GetSpeedX() * 400));
		Pkt.WriteBEInt16(static_cast<Int16>(a_Vehicle.GetSpeedY() * 400));
		Pkt.WriteBEInt16(static_cast<Int16>(a_Vehicle.GetSpeedZ() * 400));
	}
}





void cProtocol180::SendStatistics(const cStatManager & a_Manager)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, 0x37);
	Pkt.WriteVarInt32(statCount);  // TODO 2014-05-11 xdot: Optimization: Send "dirty" statistics only

	size_t Count = static_cast<size_t>(statCount);
	for (size_t i = 0; i < Count; ++i)
	{
		StatValue Value = a_Manager.GetValue(static_cast<eStatistic>(i));
		const AString & StatName = cStatInfo::GetName(static_cast<eStatistic>(i));

		Pkt.WriteString(StatName);
		Pkt.WriteVarInt32(static_cast<UInt32>(Value));
	}
}





void cProtocol180::SendTabCompletionResults(const AStringVector & a_Results)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, 0x3a);  // Tab-Complete packet
	Pkt.WriteVarInt32(static_cast<UInt32>(a_Results.size()));

	for (AStringVector::const_iterator itr = a_Results.begin(), end = a_Results.end(); itr != end; ++itr)
	{
		Pkt.WriteString(*itr);
	}
}





void cProtocol180::SendTeleportEntity(const cEntity & a_Entity)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, 0x18);
	Pkt.WriteVarInt32(a_Entity.GetUniqueID());
	Pkt.WriteFPInt(a_Entity.GetPosX());
	Pkt.WriteFPInt(a_Entity.GetPosY());
	Pkt.WriteFPInt(a_Entity.GetPosZ());
	Pkt.WriteByteAngle(a_Entity.GetYaw());
	Pkt.WriteByteAngle(a_Entity.GetPitch());
	Pkt.WriteBool(a_Entity.IsOnGround());
}





void cProtocol180::SendThunderbolt(int a_BlockX, int a_BlockY, int a_BlockZ)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, 0x2c);  // Spawn Global Entity packet
	Pkt.WriteVarInt32(0);  // EntityID = 0, always
	Pkt.WriteBEUInt8(1);  // Type = Thunderbolt
	Pkt.WriteFPInt(a_BlockX);
	Pkt.WriteFPInt(a_BlockY);
	Pkt.WriteFPInt(a_BlockZ);
}





void cProtocol180::SendTitleTimes(int a_FadeInTicks, int a_DisplayTicks, int a_FadeOutTicks)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, 0x45);  // Title packet
	Pkt.WriteVarInt32(2);  // Set title display times

	Pkt.WriteBEInt32(a_FadeInTicks);
	Pkt.WriteBEInt32(a_DisplayTicks);
	Pkt.WriteBEInt32(a_FadeOutTicks);
}





void cProtocol180::SendTimeUpdate(Int64 a_WorldAge, Int64 a_TimeOfDay, bool a_DoDaylightCycle)
{
	ASSERT(m_State == 3);  // In game mode?
	if (!a_DoDaylightCycle)
	{
		// When writing a "-" before the number the client ignores it but it will stop the client-side time expiration.
		a_TimeOfDay = std::min(-a_TimeOfDay, -1LL);
	}

	cPacketizer Pkt(*this, 0x03);
	Pkt.WriteBEInt64(a_WorldAge);
	Pkt.WriteBEInt64(a_TimeOfDay);
}





void cProtocol180::SendUnloadChunk(int a_ChunkX, int a_ChunkZ)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, 0x21);  // Chunk Data packet
	Pkt.WriteBEInt32(a_ChunkX);
	Pkt.WriteBEInt32(a_ChunkZ);
	Pkt.WriteBool(true);
	Pkt.WriteBEInt16(0);  // Primary bitmap
	Pkt.WriteVarInt32(0);  // Data size
}




void cProtocol180::SendUpdateBlockEntity(cBlockEntity & a_BlockEntity)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, 0x35);  // Update tile entity packet
	Pkt.WritePosition64(a_BlockEntity.GetPosX(), a_BlockEntity.GetPosY(), a_BlockEntity.GetPosZ());

	Byte Action = 0;
	switch (a_BlockEntity.GetBlockType())
	{
		case E_BLOCK_MOB_SPAWNER:   Action = 1; break;  // Update mob spawner spinny mob thing
		case E_BLOCK_COMMAND_BLOCK: Action = 2; break;  // Update command block text
		case E_BLOCK_BEACON:        Action = 3; break;  // Update beacon entity
		case E_BLOCK_HEAD:          Action = 4; break;  // Update Mobhead entity
		case E_BLOCK_FLOWER_POT:    Action = 5; break;  // Update flower pot
		default: ASSERT(!"Unhandled or unimplemented BlockEntity update request!"); break;
	}
	Pkt.WriteBEUInt8(Action);

	WriteBlockEntity(Pkt, a_BlockEntity);
}





void cProtocol180::SendUpdateSign(int a_BlockX, int a_BlockY, int a_BlockZ, const AString & a_Line1, const AString & a_Line2, const AString & a_Line3, const AString & a_Line4)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, 0x33);
	Pkt.WritePosition64(a_BlockX, a_BlockY, a_BlockZ);

	Json::StyledWriter JsonWriter;
	AString Lines[] = { a_Line1, a_Line2, a_Line3, a_Line4 };
	for (size_t i = 0; i < ARRAYCOUNT(Lines); i++)
	{
		Json::Value RootValue;
		RootValue["text"] = Lines[i];
		Pkt.WriteString(JsonWriter.write(RootValue).c_str());
	}
}





void cProtocol180::SendUseBed(const cEntity & a_Entity, int a_BlockX, int a_BlockY, int a_BlockZ)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, 0x0a);
	Pkt.WriteVarInt32(a_Entity.GetUniqueID());
	Pkt.WritePosition64(a_BlockX, a_BlockY, a_BlockZ);
}





void cProtocol180::SendWeather(eWeather a_Weather)
{
	ASSERT(m_State == 3);  // In game mode?

	{
		cPacketizer Pkt(*this, 0x2b);  // Change Game State packet
		Pkt.WriteBEUInt8((a_Weather == wSunny) ? 1 : 2);  // End rain / begin rain
		Pkt.WriteBEFloat(0);  // Unused for weather
	}

	// TODO: Fade effect, somehow
}





void cProtocol180::SendWholeInventory(const cWindow & a_Window)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, 0x30);  // Window Items packet
	Pkt.WriteBEInt8(a_Window.GetWindowID());
	Pkt.WriteBEInt16(static_cast<Int16>(a_Window.GetNumSlots()));
	cItems Slots;
	a_Window.GetSlots(*(m_Client->GetPlayer()), Slots);
	for (cItems::const_iterator itr = Slots.begin(), end = Slots.end(); itr != end; ++itr)
	{
		WriteItem(Pkt, *itr);
	}  // for itr - Slots[]
}





void cProtocol180::SendWindowClose(const cWindow & a_Window)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, 0x2e);
	Pkt.WriteBEInt8(a_Window.GetWindowID());
}





void cProtocol180::SendWindowOpen(const cWindow & a_Window)
{
	ASSERT(m_State == 3);  // In game mode?

	if (a_Window.GetWindowType() < 0)
	{
		// Do not send this packet for player inventory windows
		return;
	}

	cPacketizer Pkt(*this, 0x2d);
	Pkt.WriteBEInt8(a_Window.GetWindowID());
	Pkt.WriteString(a_Window.GetWindowTypeName());
	Pkt.WriteString(Printf("{\"text\":\"%s\"}", a_Window.GetWindowTitle().c_str()));

	switch (a_Window.GetWindowType())
	{
		case cWindow::wtWorkbench:
		case cWindow::wtEnchantment:
		case cWindow::wtAnvil:
		{
			Pkt.WriteBEInt8(0);
			break;
		}
		default:
		{
			Pkt.WriteBEInt8(static_cast<Int8>(a_Window.GetNumNonInventorySlots()));
			break;
		}
	}

	if (a_Window.GetWindowType() == cWindow::wtAnimalChest)
	{
		Pkt.WriteBEInt32(0);  // TODO: The animal's EntityID
	}
}





void cProtocol180::SendWindowProperty(const cWindow & a_Window, short a_Property, short a_Value)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, 0x31);  // Window Property packet
	Pkt.WriteBEInt8(a_Window.GetWindowID());
	Pkt.WriteBEInt16(a_Property);
	Pkt.WriteBEInt16(a_Value);
}





bool cProtocol180::CompressPacket(const AString & a_Packet, AString & a_CompressedData)
{
	// Compress the data:
	char CompressedData[MAX_COMPRESSED_PACKET_LEN];

	uLongf CompressedSize = compressBound(static_cast<uLongf>(a_Packet.size()));
	if (CompressedSize >= MAX_COMPRESSED_PACKET_LEN)
	{
		ASSERT(!"Too high packet size.");
		return false;
	}

	int Status = compress2(
		reinterpret_cast<Bytef *>(CompressedData), &CompressedSize,
		reinterpret_cast<const Bytef *>(a_Packet.data()), static_cast<uLongf>(a_Packet.size()), Z_DEFAULT_COMPRESSION
	);
	if (Status != Z_OK)
	{
		return false;
	}

	AString LengthData;
	cByteBuffer Buffer(20);
	Buffer.WriteVarInt32(static_cast<UInt32>(a_Packet.size()));
	Buffer.ReadAll(LengthData);
	Buffer.CommitRead();

	Buffer.WriteVarInt32(static_cast<UInt32>(CompressedSize + LengthData.size()));
	Buffer.WriteVarInt32(static_cast<UInt32>(a_Packet.size()));
	Buffer.ReadAll(LengthData);
	Buffer.CommitRead();

	a_CompressedData.clear();
	a_CompressedData.reserve(LengthData.size() + CompressedSize);
	a_CompressedData.append(LengthData.data(), LengthData.size());
	a_CompressedData.append(CompressedData, CompressedSize);
	return true;
}





int cProtocol180::GetParticleID(const AString & a_ParticleName)
{
	static bool IsInitialized = false;
	static std::map<AString, int> ParticleMap;
	if (!IsInitialized)
	{
		// Initialize the ParticleMap:
		ParticleMap["explode"]          = 0;
		ParticleMap["largeexplode"]     = 1;
		ParticleMap["hugeexplosion"]    = 2;
		ParticleMap["fireworksspark"]   = 3;
		ParticleMap["bubble"]           = 4;
		ParticleMap["splash"]           = 5;
		ParticleMap["wake"]             = 6;
		ParticleMap["suspended"]        = 7;
		ParticleMap["depthsuspend"]     = 8;
		ParticleMap["crit"]             = 9;
		ParticleMap["magiccrit"]        = 10;
		ParticleMap["smoke"]            = 11;
		ParticleMap["largesmoke"]       = 12;
		ParticleMap["spell"]            = 13;
		ParticleMap["instantspell"]     = 14;
		ParticleMap["mobspell"]         = 15;
		ParticleMap["mobspellambient"]  = 16;
		ParticleMap["witchmagic"]       = 17;
		ParticleMap["dripwater"]        = 18;
		ParticleMap["driplava"]         = 19;
		ParticleMap["angryvillager"]    = 20;
		ParticleMap["happyVillager"]    = 21;
		ParticleMap["townaura"]         = 22;
		ParticleMap["note"]             = 23;
		ParticleMap["portal"]           = 24;
		ParticleMap["enchantmenttable"] = 25;
		ParticleMap["flame"]            = 26;
		ParticleMap["lava"]             = 27;
		ParticleMap["footstep"]         = 28;
		ParticleMap["cloud"]            = 29;
		ParticleMap["reddust"]          = 30;
		ParticleMap["snowballpoof"]     = 31;
		ParticleMap["snowshovel"]       = 32;
		ParticleMap["slime"]            = 33;
		ParticleMap["heart"]            = 34;
		ParticleMap["barrier"]          = 35;
		ParticleMap["iconcrack"]        = 36;
		ParticleMap["blockcrack"]       = 37;
		ParticleMap["blockdust"]        = 38;
		ParticleMap["droplet"]          = 39;
		ParticleMap["take"]             = 40;
		ParticleMap["mobappearance"]    = 41;
	}

	AString ParticleName = StrToLower(a_ParticleName);
	if (ParticleMap.find(ParticleName) == ParticleMap.end())
	{
		LOGWARNING("Unknown particle: %s", a_ParticleName.c_str());
		ASSERT(!"Unknown particle");
		return 0;
	}

	return ParticleMap[ParticleName];
}





void cProtocol180::FixItemFramePositions(int a_ObjectData, double & a_PosX, double & a_PosZ, double & a_Yaw)
{
	switch (a_ObjectData)
	{
		case 0:
		{
			a_PosZ += 1;
			a_Yaw = 0;
			break;
		}
		case 1:
		{
			a_PosX -= 1;
			a_Yaw = 90;
			break;
		}
		case 2:
		{
			a_PosZ -= 1;
			a_Yaw = 180;
			break;
		}
		case 3:
		{
			a_PosX += 1;
			a_Yaw = 270;
			break;
		}
	}
}





void cProtocol180::AddReceivedData(const char * a_Data, size_t a_Size)
{
	// Write the incoming data into the comm log file:
	if (g_ShouldLogCommIn && m_CommLogFile.IsOpen())
	{
		if (m_ReceivedData.GetReadableSpace() > 0)
		{
			AString AllData;
			size_t OldReadableSpace = m_ReceivedData.GetReadableSpace();
			m_ReceivedData.ReadAll(AllData);
			m_ReceivedData.ResetRead();
			m_ReceivedData.SkipRead(m_ReceivedData.GetReadableSpace() - OldReadableSpace);
			ASSERT(m_ReceivedData.GetReadableSpace() == OldReadableSpace);
			AString Hex;
			CreateHexDump(Hex, AllData.data(), AllData.size(), 16);
			m_CommLogFile.Printf("Incoming data, " SIZE_T_FMT " (0x" SIZE_T_FMT_HEX ") unparsed bytes already present in buffer:\n%s\n",
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

	if (!m_ReceivedData.Write(a_Data, a_Size))
	{
		// Too much data in the incoming queue, report to caller:
		m_Client->PacketBufferFull();
		return;
	}

	// Handle all complete packets:
	for (;;)
	{
		UInt32 PacketLen;
		if (!m_ReceivedData.ReadVarInt(PacketLen))
		{
			// Not enough data
			m_ReceivedData.ResetRead();
			break;
		}
		if (!m_ReceivedData.CanReadBytes(PacketLen))
		{
			// The full packet hasn't been received yet
			m_ReceivedData.ResetRead();
			break;
		}

		// Check packet for compression:
		UInt32 UncompressedSize = 0;
		AString UncompressedData;
		if (m_State == 3)
		{
			UInt32 NumBytesRead = static_cast<UInt32>(m_ReceivedData.GetReadableSpace());

			if (!m_ReceivedData.ReadVarInt(UncompressedSize))
			{
				m_Client->Kick("Compression packet incomplete");
				return;
			}

			NumBytesRead -= static_cast<UInt32>(m_ReceivedData.GetReadableSpace());  // How many bytes has the UncompressedSize taken up?
			ASSERT(PacketLen > NumBytesRead);
			PacketLen -= NumBytesRead;

			if (UncompressedSize > 0)
			{
				// Decompress the data:
				AString CompressedData;
				VERIFY(m_ReceivedData.ReadString(CompressedData, PacketLen));
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
			VERIFY(m_ReceivedData.ReadToByteBuffer(bb, static_cast<size_t>(PacketLen)));
		}
		else
		{
			// Compression was used, move the uncompressed data:
			VERIFY(bb.Write(UncompressedData.data(), UncompressedData.size()));
		}
		m_ReceivedData.CommitRead();

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
			LOGWARNING("Protocol 1.8: Wrong number of bytes read for packet 0x%x, state %d. Read " SIZE_T_FMT " bytes, packet contained %u bytes",
				PacketType, m_State, bb.GetUsedSpace() - bb.GetReadableSpace(), PacketLen
			);

			// Put a message in the comm log:
			if (g_ShouldLogCommIn && m_CommLogFile.IsOpen())
			{
				m_CommLogFile.Printf("^^^^^^ Wrong number of bytes read for this packet (exp %d left, got " SIZE_T_FMT " left) ^^^^^^\n\n\n",
					1, bb.GetReadableSpace()
				);
				m_CommLogFile.Flush();
			}

			ASSERT(!"Read wrong number of bytes!");
			m_Client->PacketError(PacketType);
		}
	}  // for (ever)

	// Log any leftover bytes into the logfile:
	if (g_ShouldLogCommIn && (m_ReceivedData.GetReadableSpace() > 0) && m_CommLogFile.IsOpen())
	{
		AString AllData;
		size_t OldReadableSpace = m_ReceivedData.GetReadableSpace();
		m_ReceivedData.ReadAll(AllData);
		m_ReceivedData.ResetRead();
		m_ReceivedData.SkipRead(m_ReceivedData.GetReadableSpace() - OldReadableSpace);
		ASSERT(m_ReceivedData.GetReadableSpace() == OldReadableSpace);
		AString Hex;
		CreateHexDump(Hex, AllData.data(), AllData.size(), 16);
		m_CommLogFile.Printf("There are " SIZE_T_FMT " (0x" SIZE_T_FMT_HEX ") bytes of non-parse-able data left in the buffer:\n%s",
			m_ReceivedData.GetReadableSpace(), m_ReceivedData.GetReadableSpace(), Hex.c_str()
		);
		m_CommLogFile.Flush();
	}
}




bool cProtocol180::HandlePacket(cByteBuffer & a_ByteBuffer, UInt32 a_PacketType)
{
	switch (m_State)
	{
		case 1:
		{
			// Status
			switch (a_PacketType)
			{
				case 0x00: HandlePacketStatusRequest(a_ByteBuffer); return true;
				case 0x01: HandlePacketStatusPing   (a_ByteBuffer); return true;
			}
			break;
		}

		case 2:
		{
			// Login
			switch (a_PacketType)
			{
				case 0x00: HandlePacketLoginStart             (a_ByteBuffer); return true;
				case 0x01: HandlePacketLoginEncryptionResponse(a_ByteBuffer); return true;
			}
			break;
		}

		case 3:
		{
			// Game
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
			}
			break;
		}
		default:
		{
			// Received a packet in an unknown state, report:
			LOGWARNING("Received a packet in an unknown protocol state %d. Ignoring further packets.", m_State);

			// Cannot kick the client - we don't know this state and thus the packet number for the kick packet

			// Switch to a state when all further packets are silently ignored:
			m_State = 255;
			return false;
		}
		case 255:
		{
			// This is the state used for "not processing packets anymore" when we receive a bad packet from a client.
			// Do not output anything (the caller will do that for us), just return failure
			return false;
		}
	}  // switch (m_State)

	// Unknown packet type, report to the ClientHandle:
	m_Client->PacketUnknown(a_PacketType);
	return false;
}





void cProtocol180::HandlePacketStatusPing(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadBEInt64, Int64, Timestamp);

	cPacketizer Pkt(*this, 0x01);  // Ping packet
	Pkt.WriteBEInt64(Timestamp);
}





void cProtocol180::HandlePacketStatusRequest(cByteBuffer & a_ByteBuffer)
{
	auto & Server = cRoot::Get()->GetServer();
	AString ServerDescription = Server.GetDescription();
	int NumPlayers = Server.GetNumPlayers();
	int MaxPlayers = Server.GetMaxPlayers();
	AString Favicon = Server.GetFaviconData();
	cRoot::Get()->GetPluginManager().CallHookServerPing(*m_Client, ServerDescription, NumPlayers, MaxPlayers, Favicon);

	// Version:
	Json::Value Version;
	Version["name"] = "Cuberite 1.8";
	Version["protocol"] = 47;

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

	Json::StyledWriter Writer;
	AString Response = Writer.write(ResponseValue);

	cPacketizer Pkt(*this, 0x00);  // Response packet
	Pkt.WriteString(Response);
}





void cProtocol180::HandlePacketLoginEncryptionResponse(cByteBuffer & a_ByteBuffer)
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
	cRsaPrivateKey & rsaDecryptor = cRoot::Get()->GetServer().GetPrivateKey();
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
	m_Client->HandleLogin(4, m_Client->GetUsername());
}





void cProtocol180::HandlePacketLoginStart(cByteBuffer & a_ByteBuffer)
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

	auto & Server = cRoot::Get()->GetServer();
	// If auth is required, then send the encryption request:
	if (Server.ShouldAuthenticate())
	{
		cPacketizer Pkt(*this, 0x01);
		Pkt.WriteString(Server.GetServerID());
		const AString & PubKeyDer = Server.GetPublicKeyDER();
		Pkt.WriteVarInt32(static_cast<UInt32>(PubKeyDer.size()));
		Pkt.WriteBuf(PubKeyDer.data(), PubKeyDer.size());
		Pkt.WriteVarInt32(4);
		Pkt.WriteBEInt32(static_cast<int>(reinterpret_cast<intptr_t>(this)));  // Using 'this' as the cryptographic nonce, so that we don't have to generate one each time :)
		m_Client->SetUsername(Username);
		return;
	}

	m_Client->HandleLogin(4, Username);
}





void cProtocol180::HandlePacketAnimation(cByteBuffer & a_ByteBuffer)
{
	m_Client->QueueDataCommit(std::bind(&cClientHandle::HandleAnimation, m_Client, 0));  // Packet exists solely for arm-swing notification
}





void cProtocol180::HandlePacketBlockDig(cByteBuffer & a_ByteBuffer)
{
	if (!m_Client->EnforceBlockInteractionsRate())
	{
		return;
	}

	HANDLE_READ(a_ByteBuffer, ReadBEUInt8, UInt8, Status);

	int BlockX, BlockY, BlockZ;
	if (!a_ByteBuffer.ReadPosition64(BlockX, BlockY, BlockZ))
	{
		return;
	}

	HANDLE_READ(a_ByteBuffer, ReadBEInt8, Int8, Face);
	m_Client->QueueDataCommit(std::bind(&cClientHandle::HandleLeftClick, m_Client, BlockX, BlockY, BlockZ, FaceIntToBlockFace(Face), Status));
}





void cProtocol180::HandlePacketBlockPlace(cByteBuffer & a_ByteBuffer)
{
	if (!m_Client->EnforceBlockInteractionsRate())
	{
		return;
	}

	int BlockX, BlockY, BlockZ;
	if (!a_ByteBuffer.ReadPosition64(BlockX, BlockY, BlockZ))
	{
		return;
	}

	HANDLE_READ(a_ByteBuffer, ReadBEInt8, Int8, Face);

	cItem Item;
	ReadItem(a_ByteBuffer, Item, 3);

	HANDLE_READ(a_ByteBuffer, ReadBEUInt8, UInt8, CursorX);
	HANDLE_READ(a_ByteBuffer, ReadBEUInt8, UInt8, CursorY);
	HANDLE_READ(a_ByteBuffer, ReadBEUInt8, UInt8, CursorZ);
	m_Client->QueueDataCommit(std::bind(&cClientHandle::HandleRightClick, m_Client, BlockX, BlockY, BlockZ, FaceIntToBlockFace(Face), CursorX, CursorY, CursorZ, m_Client->GetPlayer()->GetEquippedItem()));
}





void cProtocol180::HandlePacketChatMessage(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadVarUTF8String, AString, Message);
	m_Client->QueueDataCommit(std::bind(&cClientHandle::HandleChat, m_Client, Message));
}





void cProtocol180::HandlePacketClientSettings(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadVarUTF8String, AString, Locale);
	HANDLE_READ(a_ByteBuffer, ReadBEUInt8,       UInt8,   ViewDistance);
	HANDLE_READ(a_ByteBuffer, ReadBEUInt8,       UInt8,   ChatFlags);
	HANDLE_READ(a_ByteBuffer, ReadBool,          bool,    ChatColors);
	HANDLE_READ(a_ByteBuffer, ReadBEUInt8,       UInt8,   SkinFlags);

	m_Client->SetLocale(Locale);
	m_Client->SetViewDistance(ViewDistance);
	// TODO: Handle other values
}





void cProtocol180::HandlePacketClientStatus(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadBEUInt8, UInt8, ActionID);

	switch (ActionID)
	{
		case 0:
		{  // Respawn
			m_Client->QueueDataCommit(std::bind(&cClientHandle::HandleRespawn, m_Client));
			break;
		}
		case 1:
		{  // Request stats
			// TODO: move cPlayer::GetStatManager call to SendStatistics to improve security
			m_Client->QueueDataCommit(std::bind(&cClientHandle::SendStatistics, m_Client, m_Client->GetPlayer()->GetStatManager()));
			break;
		}
		case 2:
		{  // Open Inventory achievement
			m_Client->QueueDataCommit(std::bind(&cPlayer::AwardAchievement, m_Client->GetPlayer(), achOpenInv));
			break;
		}
	}
}





void cProtocol180::HandlePacketCreativeInventoryAction(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadBEInt16, Int16, SlotNum);
	cItem Item;
	if (!ReadItem(a_ByteBuffer, Item))
	{
		return;
	}
	m_Client->QueueDataCommit(std::bind(&cClientHandle::HandleCreativeInventory, m_Client, SlotNum, Item, (SlotNum == -1) ? caLeftClickOutside : caLeftClick));
}





void cProtocol180::HandlePacketEntityAction(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadVarInt,  UInt32, PlayerID);
	HANDLE_READ(a_ByteBuffer, ReadBEUInt8, UInt8,  Action);
	HANDLE_READ(a_ByteBuffer, ReadVarInt,  UInt32, JumpBoost);

	switch (Action)
	{
		case 0: m_Client->QueueDataCommit(std::bind(&cClientHandle::HandleEntityCrouch, m_Client, PlayerID, true));     break;  // Crouch
		case 1: m_Client->QueueDataCommit(std::bind(&cClientHandle::HandleEntityCrouch, m_Client, PlayerID, false));    break;  // Uncrouch
		case 2: m_Client->QueueDataCommit(std::bind(&cClientHandle::HandleEntityLeaveBed, m_Client, PlayerID));         break;  // Leave Bed
		case 3: m_Client->QueueDataCommit(std::bind(&cClientHandle::HandleEntitySprinting, m_Client, PlayerID, true));  break;  // Start sprinting
		case 4: m_Client->QueueDataCommit(std::bind(&cClientHandle::HandleEntitySprinting, m_Client, PlayerID, false)); break;  // Stop sprinting
	}
}





void cProtocol180::HandlePacketKeepAlive(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadVarInt, UInt32, KeepAliveID);
	m_Client->HandleKeepAlive(KeepAliveID);
}





void cProtocol180::HandlePacketPlayer(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadBool, bool, IsOnGround);
	// TODO: m_Client->HandlePlayerOnGround(IsOnGround);
}





void cProtocol180::HandlePacketPlayerAbilities(cByteBuffer & a_ByteBuffer)
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

	m_Client->QueueDataCommit(std::bind(&cClientHandle::HandlePlayerAbilities, m_Client, CanFly, IsFlying, FlyingSpeed, WalkingSpeed));
}





void cProtocol180::HandlePacketPlayerLook(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadBEFloat, float, Yaw);
	HANDLE_READ(a_ByteBuffer, ReadBEFloat, float, Pitch);
	HANDLE_READ(a_ByteBuffer, ReadBool,    bool,  IsOnGround);
	m_Client->QueueDataCommit(std::bind(&cClientHandle::HandlePlayerLook, m_Client, Yaw, Pitch, IsOnGround));
}





void cProtocol180::HandlePacketPlayerPos(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadBEDouble, double, PosX);
	HANDLE_READ(a_ByteBuffer, ReadBEDouble, double, PosY);
	HANDLE_READ(a_ByteBuffer, ReadBEDouble, double, PosZ);
	HANDLE_READ(a_ByteBuffer, ReadBool,     bool,   IsOnGround);
	m_Client->QueueDataCommit(std::bind(&cClientHandle::HandlePlayerPos, m_Client, PosX, PosY, PosZ, PosY + (m_Client->GetPlayer()->IsCrouched() ? 1.54 : 1.62), IsOnGround));
}





void cProtocol180::HandlePacketPlayerPosLook(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadBEDouble, double, PosX);
	HANDLE_READ(a_ByteBuffer, ReadBEDouble, double, PosY);
	HANDLE_READ(a_ByteBuffer, ReadBEDouble, double, PosZ);
	HANDLE_READ(a_ByteBuffer, ReadBEFloat,  float,  Yaw);
	HANDLE_READ(a_ByteBuffer, ReadBEFloat,  float,  Pitch);
	HANDLE_READ(a_ByteBuffer, ReadBool,     bool,   IsOnGround);
	m_Client->QueueDataCommit(std::bind(&cClientHandle::HandlePlayerMoveLook, m_Client, PosX, PosY, PosZ, PosY + 1.62, Yaw, Pitch, IsOnGround));
}





void cProtocol180::HandlePacketPluginMessage(cByteBuffer & a_ByteBuffer)
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





void cProtocol180::HandlePacketSlotSelect(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadBEInt16, Int16, SlotNum);
	m_Client->QueueDataCommit(std::bind(&cClientHandle::HandleSlotSelected, m_Client, SlotNum));
}





void cProtocol180::HandlePacketSteerVehicle(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadBEFloat, float, Sideways);
	HANDLE_READ(a_ByteBuffer, ReadBEFloat, float, Forward);
	HANDLE_READ(a_ByteBuffer, ReadBEUInt8, UInt8, Flags);

	if ((Flags & 0x2) != 0)
	{
		m_Client->QueueDataCommit(std::bind(&cClientHandle::HandleUnmount, m_Client));
	}
	else if ((Flags & 0x1) != 0)
	{
		// jump
	}
	else
	{
		m_Client->QueueDataCommit(std::bind(&cClientHandle::HandleSteerVehicle, m_Client, Forward, Sideways));
	}
}





void cProtocol180::HandlePacketTabComplete(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadVarUTF8String, AString, Text);
	HANDLE_READ(a_ByteBuffer, ReadBool,          bool,    HasPosition);

	if (HasPosition)
	{
		HANDLE_READ(a_ByteBuffer, ReadBEInt64, Int64, Position);
	}

	m_Client->QueueDataCommit(std::bind(&cClientHandle::HandleTabCompletion, m_Client, Text));
}





void cProtocol180::HandlePacketUpdateSign(cByteBuffer & a_ByteBuffer)
{
	int BlockX, BlockY, BlockZ;
	if (!a_ByteBuffer.ReadPosition64(BlockX, BlockY, BlockZ))
	{
		return;
	}

	AString Lines[4];
	Json::Value root;
	Json::Reader reader;
	for (int i = 0; i < 4; i++)
	{
		HANDLE_READ(a_ByteBuffer, ReadVarUTF8String, AString, Line);
		if (reader.parse(Line, root, false))
		{
			Lines[i] = root.asString();
		}
	}

	m_Client->HandleUpdateSign(BlockX, BlockY, BlockZ, Lines[0], Lines[1], Lines[2], Lines[3]);
}





void cProtocol180::HandlePacketUseEntity(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadVarInt, UInt32, EntityID);
	HANDLE_READ(a_ByteBuffer, ReadVarInt, UInt32, Type);

	switch (Type)
	{
		case 0:
		{
			m_Client->QueueDataCommit(std::bind(&cClientHandle::HandleUseEntity, m_Client, EntityID, false));
			break;
		}
		case 1:
		{
			m_Client->QueueDataCommit(std::bind(&cClientHandle::HandleUseEntity, m_Client, EntityID, true));
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





void cProtocol180::HandlePacketEnchantItem(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadBEUInt8, UInt8, WindowID);
	HANDLE_READ(a_ByteBuffer, ReadBEUInt8, UInt8, Enchantment);

	m_Client->QueueDataCommit(std::bind(&cClientHandle::HandleEnchantItem, m_Client, WindowID, Enchantment));
}





void cProtocol180::HandlePacketWindowClick(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadBEUInt8,  UInt8,  WindowID);
	HANDLE_READ(a_ByteBuffer, ReadBEInt16,  Int16,  SlotNum);
	HANDLE_READ(a_ByteBuffer, ReadBEUInt8,  UInt8,  Button);
	HANDLE_READ(a_ByteBuffer, ReadBEUInt16, UInt16, TransactionID);
	HANDLE_READ(a_ByteBuffer, ReadBEUInt8,  UInt8,  Mode);
	cItem Item;
	ReadItem(a_ByteBuffer, Item);

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
		case 0x0600: Action = caDblClick; break;
		default:
		{
			LOGWARNING("Unhandled window click mode / button combination: %d (0x%x)", (Mode << 8) | Button, (Mode << 8) | Button);
			Action = caUnknown;
			break;
		}
	}

	m_Client->QueueDataCommit(std::bind(&cClientHandle::HandleWindowClick, m_Client, WindowID, SlotNum, Action, Item));
}





void cProtocol180::HandlePacketWindowClose(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadBEUInt8, UInt8, WindowID);
	m_Client->QueueDataCommit(std::bind(&cClientHandle::HandleWindowClose, m_Client, WindowID));
}





void cProtocol180::HandleVanillaPluginMessage(cByteBuffer & a_ByteBuffer, const AString & a_Channel)
{
	if (a_Channel == "MC|AdvCdm")
	{
		HANDLE_READ(a_ByteBuffer, ReadBEUInt8, UInt8, Mode)
		switch (Mode)
		{
			case 0x00:
			{
				HANDLE_READ(a_ByteBuffer, ReadBEInt32, Int32, BlockX);
				HANDLE_READ(a_ByteBuffer, ReadBEInt32, Int32, BlockY);
				HANDLE_READ(a_ByteBuffer, ReadBEInt32, Int32, BlockZ);
				HANDLE_READ(a_ByteBuffer, ReadVarUTF8String, AString, Command);
				m_Client->QueueDataCommit(std::bind(&cClientHandle::HandleCommandBlockBlockChange, m_Client, BlockX, BlockY, BlockZ, Command));
				break;
			}

			default:
			{
				m_Client->QueueDataCommit(
					std::bind(
						static_cast<void(cClientHandle::*)(const AString &, eMessageType, const AString &)>(&cClientHandle::SendChat),
						m_Client,
						Printf("Failure setting command block command; unhandled mode %u (0x%02x)", Mode, Mode),
						mtFailure,
						""
					)
				);

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
		SendPluginMessage("MC|Brand", "\x08Cuberite");
		return;
	}
	else if (a_Channel == "MC|Beacon")
	{
		HANDLE_READ(a_ByteBuffer, ReadBEInt32, Int32, Effect1);
		HANDLE_READ(a_ByteBuffer, ReadBEInt32, Int32, Effect2);
		m_Client->QueueDataCommit(std::bind(&cClientHandle::HandleBeaconSelection, m_Client, Effect1, Effect2));
		return;
	}
	else if (a_Channel == "MC|ItemName")
	{
		HANDLE_READ(a_ByteBuffer, ReadVarUTF8String, AString, ItemName);
		m_Client->QueueDataCommit(std::bind(&cClientHandle::HandleAnvilItemName, m_Client, ItemName));
		return;
	}
	else if (a_Channel == "MC|TrSel")
	{
		HANDLE_READ(a_ByteBuffer, ReadBEInt32, Int32, SlotNum);
		m_Client->QueueDataCommit(std::bind(&cClientHandle::HandleNPCTrade, m_Client, SlotNum));
		return;
	}
	LOG("Unhandled vanilla plugin channel: \"%s\".", a_Channel.c_str());

	// Read the payload and send it through to the clienthandle:
	AString Message;
	VERIFY(a_ByteBuffer.ReadString(Message, a_ByteBuffer.GetReadableSpace() - 1));
	m_Client->HandlePluginMessage(a_Channel, Message);
}





void cProtocol180::SendData(const char * a_Data, size_t a_Size)
{
	if (m_IsEncrypted)
	{
		Byte Encrypted[8192];  // Larger buffer, we may be sending lots of data (chunks)
		while (a_Size > 0)
		{
			size_t NumBytes = (a_Size > sizeof(Encrypted)) ? sizeof(Encrypted) : a_Size;
			m_Encryptor.ProcessData(Encrypted, reinterpret_cast<Byte *>(const_cast<char*>(a_Data)), NumBytes);
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





bool cProtocol180::ReadItem(cByteBuffer & a_ByteBuffer, cItem & a_Item, size_t a_KeepRemainingBytes)
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





void cProtocol180::ParseItemMetadata(cItem & a_Item, const AString & a_Metadata)
{
	// Parse into NBT:
	cParsedNBT NBT(a_Metadata.data(), a_Metadata.size());
	if (!NBT.IsValid())
	{
		AString HexDump;
		CreateHexDump(HexDump, a_Metadata.data(), std::max<size_t>(a_Metadata.size(), 1024), 16);
		LOGWARNING("Cannot parse NBT item metadata: (" SIZE_T_FMT " bytes)\n%s", a_Metadata.size(), HexDump.c_str());
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
							AString Lore;

							for (int loretag = NBT.GetFirstChild(displaytag); loretag >= 0; loretag = NBT.GetNextSibling(loretag))  // Loop through array of strings
							{
								AppendPrintf(Lore, "%s`", NBT.GetString(loretag).c_str());  // Append the lore with a grave accent / backtick, used internally by MCS to display a new line in the client; don't forget to c_str ;)
							}

							a_Item.m_Lore = Lore;
						}
						else if ((NBT.GetType(displaytag) == TAG_Int) && (NBT.GetName(displaytag) == "color"))
						{
							a_Item.m_ItemColor.m_Color = static_cast<unsigned int>(NBT.GetInt(displaytag));
						}
					}
				}
				else if ((TagName == "Fireworks") || (TagName == "Explosion"))
				{
					cFireworkItem::ParseFromNBT(a_Item.m_FireworkItem, NBT, tag, static_cast<ENUM_ITEM_ID>(a_Item.m_ItemType));
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





void cProtocol180::StartEncryption(const Byte * a_Key)
{
	m_Encryptor.Init(a_Key, a_Key);
	m_Decryptor.Init(a_Key, a_Key);
	m_IsEncrypted = true;

	// Prepare the m_AuthServerID:
	cSha1Checksum Checksum;
	auto & Server = cRoot::Get()->GetServer();
	const AString & ServerID = Server.GetServerID();
	Checksum.Update(reinterpret_cast<const Byte *>(ServerID.c_str()), ServerID.length());
	Checksum.Update(a_Key, 16);
	Checksum.Update(reinterpret_cast<const Byte *>(Server.GetPublicKeyDER().data()), Server.GetPublicKeyDER().size());
	Byte Digest[20];
	Checksum.Finalize(Digest);
	cSha1Checksum::DigestToJava(Digest, m_AuthServerID);
}





eBlockFace cProtocol180::FaceIntToBlockFace(Int8 a_BlockFace)
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
// cProtocol180::cPacketizer:

void cProtocol180::SendPacket(cPacketizer & a_Pkt)
{
	UInt32 PacketLen = static_cast<UInt32>(m_OutPacketBuffer.GetUsedSpace());
	AString PacketData, CompressedPacket;
	m_OutPacketBuffer.ReadAll(PacketData);
	m_OutPacketBuffer.CommitRead();

	if ((m_State == 3) && (PacketLen >= 256))
	{
		// Compress the packet payload:
		if (!cProtocol180::CompressPacket(PacketData, CompressedPacket))
		{
			return;
		}
	}
	else if (m_State == 3)
	{
		// The packet is not compressed, indicate this in the packet header:
		m_OutPacketLenBuffer.WriteVarInt32(PacketLen + 1);
		m_OutPacketLenBuffer.WriteVarInt32(0);
		AString LengthData;
		m_OutPacketLenBuffer.ReadAll(LengthData);
		SendData(LengthData.data(), LengthData.size());
	}
	else
	{
		// Compression doesn't apply to this state, send raw data:
		m_OutPacketLenBuffer.WriteVarInt32(PacketLen);
		AString LengthData;
		m_OutPacketLenBuffer.ReadAll(LengthData);
		SendData(LengthData.data(), LengthData.size());
	}

	// Send the packet's payload, either direct or compressed:
	if (CompressedPacket.empty())
	{
		m_OutPacketLenBuffer.CommitRead();
		SendData(PacketData.data(), PacketData.size());
	}
	else
	{
		SendData(CompressedPacket.data(), CompressedPacket.size());
	}

	// Log the comm into logfile:
	if (g_ShouldLogCommOut && m_CommLogFile.IsOpen())
	{
		AString Hex;
		ASSERT(PacketData.size() > 0);
		CreateHexDump(Hex, PacketData.data(), PacketData.size(), 16);
		m_CommLogFile.Printf("Outgoing packet: type %d (0x%x), length %u (0x%x), state %d. Payload (incl. type):\n%s\n",
			a_Pkt.GetPacketType(), a_Pkt.GetPacketType(), PacketLen, PacketLen, m_State, Hex.c_str()
		);
	}
}





void cProtocol180::WriteItem(cPacketizer & a_Pkt, const cItem & a_Item)
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
			Writer.AddString("Name", a_Item.m_CustomName.c_str());
		}
		if (!a_Item.IsLoreEmpty())
		{
			Writer.BeginList("Lore", TAG_String);

			AStringVector Decls = StringSplit(a_Item.m_Lore, "`");
			for (AStringVector::const_iterator itr = Decls.begin(), end = Decls.end(); itr != end; ++itr)
			{
				if (itr->empty())
				{
					// The decl is empty (two `s), ignore
					continue;
				}
				Writer.AddString("", itr->c_str());
			}

			Writer.EndList();
		}
		Writer.EndCompound();
	}
	if ((a_Item.m_ItemType == E_ITEM_FIREWORK_ROCKET) || (a_Item.m_ItemType == E_ITEM_FIREWORK_STAR))
	{
		cFireworkItem::WriteToNBTCompound(a_Item.m_FireworkItem, Writer, static_cast<ENUM_ITEM_ID>(a_Item.m_ItemType));
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





void cProtocol180::WriteBlockEntity(cPacketizer & a_Pkt, const cBlockEntity & a_BlockEntity)
{
	cFastNBTWriter Writer;

	switch (a_BlockEntity.GetBlockType())
	{
		case E_BLOCK_BEACON:
		{
			auto & BeaconEntity = reinterpret_cast<const cBeaconEntity &>(a_BlockEntity);
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
			auto & CommandBlockEntity = reinterpret_cast<const cCommandBlockEntity &>(a_BlockEntity);
			Writer.AddByte("TrackOutput", 1);  // Neither I nor the MC wiki has any idea about this
			Writer.AddInt("SuccessCount", CommandBlockEntity.GetResult());
			Writer.AddInt("x", CommandBlockEntity.GetPosX());
			Writer.AddInt("y", CommandBlockEntity.GetPosY());
			Writer.AddInt("z", CommandBlockEntity.GetPosZ());
			Writer.AddString("Command", CommandBlockEntity.GetCommand().c_str());
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

		case E_BLOCK_HEAD:
		{
			auto & MobHeadEntity = reinterpret_cast<const cMobHeadEntity &>(a_BlockEntity);
			Writer.AddInt("x", MobHeadEntity.GetPosX());
			Writer.AddInt("y", MobHeadEntity.GetPosY());
			Writer.AddInt("z", MobHeadEntity.GetPosZ());
			Writer.AddByte("SkullType", MobHeadEntity.GetType() & 0xFF);
			Writer.AddByte("Rot", MobHeadEntity.GetRotation() & 0xFF);
			Writer.AddString("id", "Skull");  // "Tile Entity ID" - MC wiki; vanilla server always seems to send this though

			// The new Block Entity format for a Mob Head. See: http://minecraft.gamepedia.com/Head#Block_entity
			Writer.BeginCompound("Owner");
				Writer.AddString("Id", MobHeadEntity.GetOwnerUUID());
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
			auto & FlowerPotEntity = reinterpret_cast<const cFlowerPotEntity &>(a_BlockEntity);
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
			auto & MobSpawnerEntity = reinterpret_cast<const cMobSpawnerEntity &>(a_BlockEntity);
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





void cProtocol180::WriteEntityMetadata(cPacketizer & a_Pkt, const cEntity & a_Entity)
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
		case cEntity::etPlayer: break;  // TODO?
		case cEntity::etPickup:
		{
			a_Pkt.WriteBEUInt8((5 << 5) | 10);  // Slot(5) + index 10
			WriteItem(a_Pkt, reinterpret_cast<const cPickup &>(a_Entity).GetItem());
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
			auto & Minecart = reinterpret_cast<const cMinecart &>(a_Entity);
			a_Pkt.WriteBEInt32((((a_Entity.GetMaxHealth() / 2) - (a_Entity.GetHealth() - (a_Entity.GetMaxHealth() / 2))) * Minecart.LastDamage()) * 4);
			a_Pkt.WriteBEUInt8(0x52);
			a_Pkt.WriteBEInt32(1);  // Shaking direction, doesn't seem to affect anything
			a_Pkt.WriteBEUInt8(0x73);
			a_Pkt.WriteBEFloat(static_cast<float>(Minecart.LastDamage() + 10));  // Damage taken / shake effect multiplyer

			if (Minecart.GetPayload() == cMinecart::mpNone)
			{
				auto & RideableMinecart = reinterpret_cast<const cRideableMinecart &>(Minecart);
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
				a_Pkt.WriteBEUInt8(reinterpret_cast<const cMinecartWithFurnace &>(Minecart).IsFueled() ? 1 : 0);
			}
			break;
		}  // case etMinecart

		case cEntity::etProjectile:
		{
			auto & Projectile = reinterpret_cast<const cProjectileEntity &>(a_Entity);
			switch (Projectile.GetProjectileKind())
			{
				case cProjectileEntity::pkArrow:
				{
					a_Pkt.WriteBEUInt8(0x10);
					a_Pkt.WriteBEUInt8(reinterpret_cast<const cArrowEntity &>(Projectile).IsCritical() ? 1 : 0);
					break;
				}
				case cProjectileEntity::pkFirework:
				{
					a_Pkt.WriteBEUInt8(0xa8);
					WriteItem(a_Pkt, reinterpret_cast<const cFireworkEntity &>(Projectile).GetItem());
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
			WriteMobMetadata(a_Pkt, reinterpret_cast<const cMonster &>(a_Entity));
			break;
		}

		case cEntity::etItemFrame:
		{
			auto & Frame = reinterpret_cast<const cItemFrame &>(a_Entity);
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





void cProtocol180::WriteMobMetadata(cPacketizer & a_Pkt, const cMonster & a_Mob)
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
			auto & Bat = reinterpret_cast<const cBat &>(a_Mob);
			a_Pkt.WriteBEUInt8(0x10);
			a_Pkt.WriteBEUInt8(Bat.IsHanging() ? 1 : 0);
			break;
		}  // case mtBat

		case mtCreeper:
		{
			auto & Creeper = reinterpret_cast<const cCreeper &>(a_Mob);
			a_Pkt.WriteBEUInt8(0x10);
			a_Pkt.WriteBEUInt8(Creeper.IsBlowing() ? 1 : 255);
			a_Pkt.WriteBEUInt8(0x11);
			a_Pkt.WriteBEUInt8(Creeper.IsCharged() ? 1 : 0);
			break;
		}  // case mtCreeper

		case mtEnderman:
		{
			auto & Enderman = reinterpret_cast<const cEnderman &>(a_Mob);
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
			auto & Ghast = reinterpret_cast<const cGhast &>(a_Mob);
			a_Pkt.WriteBEUInt8(0x10);
			a_Pkt.WriteBEUInt8(Ghast.IsCharging());
			break;
		}  // case mtGhast

		case mtHorse:
		{
			auto & Horse = reinterpret_cast<const cHorse &>(a_Mob);
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
			auto & MagmaCube = reinterpret_cast<const cMagmaCube &>(a_Mob);
			a_Pkt.WriteBEUInt8(0x10);
			a_Pkt.WriteBEUInt8(static_cast<UInt8>(MagmaCube.GetSize()));
			break;
		}  // case mtMagmaCube

		case mtOcelot:
		{
			auto & Ocelot = reinterpret_cast<const cOcelot &>(a_Mob);
			a_Pkt.WriteBEUInt8(0x0c);
			a_Pkt.WriteBEInt8(Ocelot.IsBaby() ? -1 : (Ocelot.IsInLoveCooldown() ? 1 : 0));
			break;
		}  // case mtOcelot

		case mtCow:
		{
			auto & Cow = reinterpret_cast<const cCow &>(a_Mob);
			a_Pkt.WriteBEUInt8(0x0c);
			a_Pkt.WriteBEInt8(Cow.IsBaby() ? -1 : (Cow.IsInLoveCooldown() ? 1 : 0));
			break;
		}  // case mtCow

		case mtChicken:
		{
			auto & Chicken = reinterpret_cast<const cChicken &>(a_Mob);
			a_Pkt.WriteBEUInt8(0x0c);
			a_Pkt.WriteBEInt8(Chicken.IsBaby() ? -1 : (Chicken.IsInLoveCooldown() ? 1 : 0));
			break;
		}  // case mtChicken

		case mtPig:
		{
			auto & Pig = reinterpret_cast<const cPig &>(a_Mob);
			a_Pkt.WriteBEUInt8(0x0c);
			a_Pkt.WriteBEInt8(Pig.IsBaby() ? -1 : (Pig.IsInLoveCooldown() ? 1 : 0));
			a_Pkt.WriteBEUInt8(0x10);
			a_Pkt.WriteBEUInt8(Pig.IsSaddled() ? 1 : 0);
			break;
		}  // case mtPig

		case mtSheep:
		{
			auto & Sheep = reinterpret_cast<const cSheep &>(a_Mob);
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
			auto & Rabbit = reinterpret_cast<const cRabbit &>(a_Mob);
			a_Pkt.WriteBEUInt8(0x12);
			a_Pkt.WriteBEUInt8(Rabbit.GetRabbitTypeAsNumber());
			a_Pkt.WriteBEUInt8(0x0c);
			a_Pkt.WriteBEInt8(Rabbit.IsBaby() ? -1 : (Rabbit.IsInLoveCooldown() ? 1 : 0));
			break;
		}  // case mtRabbit

		case mtSkeleton:
		{
			auto & Skeleton = reinterpret_cast<const cSkeleton &>(a_Mob);
			a_Pkt.WriteBEUInt8(0x0d);
			a_Pkt.WriteBEUInt8(Skeleton.IsWither() ? 1 : 0);
			break;
		}  // case mtSkeleton

		case mtSlime:
		{
			auto & Slime = reinterpret_cast<const cSlime &>(a_Mob);
			a_Pkt.WriteBEUInt8(0x10);
			a_Pkt.WriteBEUInt8(static_cast<UInt8>(Slime.GetSize()));
			break;
		}  // case mtSlime

		case mtVillager:
		{
			auto & Villager = reinterpret_cast<const cVillager &>(a_Mob);
			a_Pkt.WriteBEUInt8(0x50);
			a_Pkt.WriteBEInt32(Villager.GetVilType());
			a_Pkt.WriteBEUInt8(0x0c);
			a_Pkt.WriteBEInt8(Villager.IsBaby() ? -1 : 0);
			break;
		}  // case mtVillager

		case mtWitch:
		{
			auto & Witch = reinterpret_cast<const cWitch &>(a_Mob);
			a_Pkt.WriteBEUInt8(0x15);
			a_Pkt.WriteBEUInt8(Witch.IsAngry() ? 1 : 0);
			break;
		}  // case mtWitch

		case mtWither:
		{
			auto & Wither = reinterpret_cast<const cWither &>(a_Mob);
			a_Pkt.WriteBEUInt8(0x54);  // Int at index 20
			a_Pkt.WriteBEInt32(static_cast<Int32>(Wither.GetWitherInvulnerableTicks()));
			a_Pkt.WriteBEUInt8(0x66);  // Float at index 6
			a_Pkt.WriteBEFloat(static_cast<float>(a_Mob.GetHealth()));
			break;
		}  // case mtWither

		case mtWolf:
		{
			auto & Wolf = reinterpret_cast<const cWolf &>(a_Mob);
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
			auto & Zombie = reinterpret_cast<const cZombie &>(a_Mob);
			a_Pkt.WriteBEUInt8(0x0c);
			a_Pkt.WriteBEInt8(Zombie.IsBaby() ? 1 : -1);
			a_Pkt.WriteBEUInt8(0x0d);
			a_Pkt.WriteBEUInt8(Zombie.IsVillagerZombie() ? 1 : 0);
			a_Pkt.WriteBEUInt8(0x0e);
			a_Pkt.WriteBEUInt8(Zombie.IsConverting() ? 1 : 0);
			break;
		}  // case mtZombie

		case mtZombiePigman:
		{
			auto & ZombiePigman = reinterpret_cast<const cZombiePigman &>(a_Mob);
			a_Pkt.WriteBEUInt8(0x0c);
			a_Pkt.WriteBEInt8(ZombiePigman.IsBaby() ? 1 : -1);
			break;
		}  // case mtZombiePigman
	}  // switch (a_Mob.GetType())
}





void cProtocol180::WriteEntityProperties(cPacketizer & a_Pkt, const cEntity & a_Entity)
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



