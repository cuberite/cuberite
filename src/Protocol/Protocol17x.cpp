
// Protocol17x.cpp

/*
Implements the 1.7.x protocol classes:
	- cProtocol172
		- release 1.7.2 protocol (#4)
	- cProtocol176
		- release 1.7.6 protocol (#5)
*/

#include "Globals.h"
#include "json/json.h"
#include "Protocol17x.h"
#include "ChunkDataSerializer.h"
#include "PolarSSL++/Sha1Checksum.h"
#include "Packetizer.h"

#include "../ClientHandle.h"
#include "../Root.h"
#include "../Server.h"
#include "../World.h"
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





// fwd: main.cpp:
extern bool g_ShouldLogCommIn, g_ShouldLogCommOut;





////////////////////////////////////////////////////////////////////////////////
// cProtocol172:

cProtocol172::cProtocol172(cClientHandle * a_Client, const AString & a_ServerAddress, UInt16 a_ServerPort, UInt32 a_State) :
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
	if (cRoot::Get()->GetServer()->ShouldAllowBungeeCord() && SplitZeroTerminatedStrings(a_ServerAddress, Params) && (Params.size() == 4))
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
		AString FileName = Printf("CommLogs/%x_%d__%s.log", static_cast<unsigned>(time(nullptr)), sCounter++, a_Client->GetIPString().c_str());
		m_CommLogFile.Open(FileName, cFile::fmWrite);
	}
}





void cProtocol172::DataReceived(const char * a_Data, size_t a_Size)
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





void cProtocol172::SendAttachEntity(const cEntity & a_Entity, const cEntity & a_Vehicle)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, 0x1b);  // Attach Entity packet
	Pkt.WriteBEUInt32(a_Entity.GetUniqueID());
	Pkt.WriteBEUInt32(a_Vehicle.GetUniqueID());
	Pkt.WriteBool(false);
}





void cProtocol172::SendBlockAction(int a_BlockX, int a_BlockY, int a_BlockZ, char a_Byte1, char a_Byte2, BLOCKTYPE a_BlockType)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, 0x24);  // Block Action packet
	Pkt.WriteBEInt32(a_BlockX);
	Pkt.WriteBEInt16(static_cast<Int16>(a_BlockY));
	Pkt.WriteBEInt32(a_BlockZ);
	Pkt.WriteBEInt8(a_Byte1);
	Pkt.WriteBEInt8(a_Byte2);
	Pkt.WriteVarInt32(a_BlockType);
}





void cProtocol172::SendBlockBreakAnim(UInt32 a_EntityID, int a_BlockX, int a_BlockY, int a_BlockZ, char a_Stage)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, 0x25);  // Block Break Animation packet
	Pkt.WriteVarInt32(a_EntityID);
	Pkt.WriteBEInt32(a_BlockX);
	Pkt.WriteBEInt32(a_BlockY);
	Pkt.WriteBEInt32(a_BlockZ);
	Pkt.WriteBEInt8(a_Stage);
}





void cProtocol172::SendBlockChange(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta)
{
	ASSERT(m_State == 3);  // In game mode?
	ASSERT((a_BlockY >= 0) && (a_BlockY < 256));

	cPacketizer Pkt(*this, 0x23);  // Block Change packet
	Pkt.WriteBEInt32(a_BlockX);
	Pkt.WriteBEUInt8(static_cast<UInt8>(a_BlockY));
	Pkt.WriteBEInt32(a_BlockZ);
	Pkt.WriteVarInt32(a_BlockType);
	Pkt.WriteBEUInt8(a_BlockMeta);
}





void cProtocol172::SendBlockChanges(int a_ChunkX, int a_ChunkZ, const sSetBlockVector & a_Changes)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, 0x22);  // Multi Block Change packet
	Pkt.WriteBEInt32(a_ChunkX);
	Pkt.WriteBEInt32(a_ChunkZ);
	Pkt.WriteBEUInt16(static_cast<UInt16>(a_Changes.size()));
	Pkt.WriteBEUInt32(static_cast<UInt32>(a_Changes.size() * 4));
	for (sSetBlockVector::const_iterator itr = a_Changes.begin(), end = a_Changes.end(); itr != end; ++itr)
	{
		int Coords = itr->m_RelY | (itr->m_RelZ << 8) | (itr->m_RelX << 12);
		int Blocks = static_cast<int>(itr->m_BlockMeta | (itr->m_BlockType << 4));
		Pkt.WriteBEInt32((Coords << 16) | Blocks);
	}  // for itr - a_Changes[]
}





void cProtocol172::SendChat(const AString & a_Message, eChatType a_Type)
{
	ASSERT(m_State == 3);  // In game mode?

	if (a_Type != ctChatBox)  // 1.7.2 doesn't support anything else
	{
		return;
	}

	cPacketizer Pkt(*this, 0x02);  // Chat Message packet
	Pkt.WriteString(Printf("{\"text\":\"%s\"}", EscapeString(a_Message).c_str()));
}





void cProtocol172::SendChat(const cCompositeChat & a_Message, eChatType a_Type, bool a_ShouldUseChatPrefixes)
{
	ASSERT(m_State == 3);  // In game mode?

	if (a_Type != ctChatBox)  // 1.7.2 doesn't support anything else
	{
		return;
	}

	// Send the message to the client:
	cPacketizer Pkt(*this, 0x02);
	Pkt.WriteString(a_Message.CreateJsonString(a_ShouldUseChatPrefixes));
}





void cProtocol172::SendChunkData(int a_ChunkX, int a_ChunkZ, cChunkDataSerializer & a_Serializer)
{
	ASSERT(m_State == 3);  // In game mode?

	// Serialize first, before creating the Packetizer (the packetizer locks a CS)
	// This contains the flags and bitmasks, too
	const AString & ChunkData = a_Serializer.Serialize(cChunkDataSerializer::RELEASE_1_3_2, a_ChunkX, a_ChunkZ);

	cPacketizer Pkt(*this, 0x21);  // Chunk Data packet
	Pkt.WriteBEInt32(a_ChunkX);
	Pkt.WriteBEInt32(a_ChunkZ);
	Pkt.WriteBuf(ChunkData.data(), ChunkData.size());
}





void cProtocol172::SendCollectEntity(const cEntity & a_Entity, const cPlayer & a_Player)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, 0x0d);  // Collect Item packet
	Pkt.WriteBEUInt32(a_Entity.GetUniqueID());
	Pkt.WriteBEUInt32(a_Player.GetUniqueID());
}





void cProtocol172::SendDestroyEntity(const cEntity & a_Entity)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, 0x13);  // Destroy Entities packet
	Pkt.WriteBEUInt8(1);
	Pkt.WriteBEUInt32(a_Entity.GetUniqueID());
}





void cProtocol172::SendDetachEntity(const cEntity & a_Entity, const cEntity & a_PreviousVehicle)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, 0x1b);  // Attach Entity packet
	Pkt.WriteBEUInt32(a_Entity.GetUniqueID());
	Pkt.WriteBEUInt32(0);
	Pkt.WriteBool(false);
}





void cProtocol172::SendDisconnect(const AString & a_Reason)
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





void cProtocol172::SendEditSign(int a_BlockX, int a_BlockY, int a_BlockZ)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, 0x36);  // Sign Editor Open packet
	Pkt.WriteBEInt32(a_BlockX);
	Pkt.WriteBEInt32(a_BlockY);
	Pkt.WriteBEInt32(a_BlockZ);
}





void cProtocol172::SendEntityEffect(const cEntity & a_Entity, int a_EffectID, int a_Amplifier, short a_Duration)
{
	ASSERT(m_State == 3);  // In game mode?
	ASSERT((a_EffectID >= 0) && (a_EffectID < 256));
	ASSERT((a_Amplifier >= 0) && (a_Amplifier < 256));

	cPacketizer Pkt(*this, 0x1D);  // Entity Effect packet
	Pkt.WriteBEUInt32(a_Entity.GetUniqueID());
	Pkt.WriteBEUInt8(static_cast<Byte>(a_EffectID));
	Pkt.WriteBEUInt8(static_cast<Byte>(a_Amplifier));
	Pkt.WriteBEInt16(a_Duration);
}





void cProtocol172::SendEntityEquipment(const cEntity & a_Entity, short a_SlotNum, const cItem & a_Item)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, 0x04);  // Entity Equipment packet
	Pkt.WriteBEUInt32(a_Entity.GetUniqueID());
	Pkt.WriteBEInt16(a_SlotNum);
	WriteItem(Pkt, a_Item);
}





void cProtocol172::SendEntityHeadLook(const cEntity & a_Entity)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, 0x19);  // Entity Head Look packet
	Pkt.WriteBEUInt32(a_Entity.GetUniqueID());
	Pkt.WriteByteAngle(a_Entity.GetHeadYaw());
}





void cProtocol172::SendEntityLook(const cEntity & a_Entity)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, 0x16);  // Entity Look packet
	Pkt.WriteBEUInt32(a_Entity.GetUniqueID());
	Pkt.WriteByteAngle(a_Entity.GetYaw());
	Pkt.WriteByteAngle(a_Entity.GetPitch());
}





void cProtocol172::SendEntityMetadata(const cEntity & a_Entity)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, 0x1c);  // Entity Metadata packet
	Pkt.WriteBEUInt32(a_Entity.GetUniqueID());
	WriteEntityMetadata(Pkt, a_Entity);
	Pkt.WriteBEUInt8(0x7f);  // The termination byte
}





void cProtocol172::SendEntityProperties(const cEntity & a_Entity)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, 0x20);  // Entity Properties packet
	Pkt.WriteBEUInt32(a_Entity.GetUniqueID());
	WriteEntityProperties(Pkt, a_Entity);
}





void cProtocol172::SendEntityRelMove(const cEntity & a_Entity, char a_RelX, char a_RelY, char a_RelZ)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, 0x15);  // Entity Relative Move packet
	Pkt.WriteBEUInt32(a_Entity.GetUniqueID());
	Pkt.WriteBEInt8(a_RelX);
	Pkt.WriteBEInt8(a_RelY);
	Pkt.WriteBEInt8(a_RelZ);
}





void cProtocol172::SendEntityRelMoveLook(const cEntity & a_Entity, char a_RelX, char a_RelY, char a_RelZ)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, 0x17);  // Entity Look And Relative Move packet
	Pkt.WriteBEUInt32(a_Entity.GetUniqueID());
	Pkt.WriteBEInt8(a_RelX);
	Pkt.WriteBEInt8(a_RelY);
	Pkt.WriteBEInt8(a_RelZ);
	Pkt.WriteByteAngle(a_Entity.GetYaw());
	Pkt.WriteByteAngle(a_Entity.GetPitch());
}





void cProtocol172::SendEntityStatus(const cEntity & a_Entity, char a_Status)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, 0x1a);  // Entity Status packet
	Pkt.WriteBEUInt32(a_Entity.GetUniqueID());
	Pkt.WriteBEInt8(a_Status);
}





void cProtocol172::SendEntityVelocity(const cEntity & a_Entity)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, 0x12);  // Entity Velocity packet
	Pkt.WriteBEUInt32(a_Entity.GetUniqueID());
	// 400 = 8000 / 20 ... Conversion from our speed in m / s to 8000 m / tick
	Pkt.WriteBEInt16(static_cast<short>(a_Entity.GetSpeedX() * 400));
	Pkt.WriteBEInt16(static_cast<short>(a_Entity.GetSpeedY() * 400));
	Pkt.WriteBEInt16(static_cast<short>(a_Entity.GetSpeedZ() * 400));
}





void cProtocol172::SendExplosion(double a_BlockX, double a_BlockY, double a_BlockZ, float a_Radius, const cVector3iArray & a_BlocksAffected, const Vector3d & a_PlayerMotion)
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





void cProtocol172::SendGameMode(eGameMode a_GameMode)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, 0x2b);  // Change Game State packet
	Pkt.WriteBEUInt8(3);  // Reason: Change game mode
	Pkt.WriteBEFloat(static_cast<float>(a_GameMode));  // The protocol really represents the value with a float!
}





void cProtocol172::SendHealth(void)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, 0x06);  // Update Health packet
	cPlayer * Player = m_Client->GetPlayer();
	Pkt.WriteBEFloat(static_cast<float>(Player->GetHealth()));
	Pkt.WriteBEInt16(static_cast<short>(Player->GetFoodLevel()));
	Pkt.WriteBEFloat(static_cast<float>(Player->GetFoodSaturationLevel()));
}





void cProtocol172::SendHideTitle(void)
{
	// Not implemented in this protocol version
}





void cProtocol172::SendInventorySlot(char a_WindowID, short a_SlotNum, const cItem & a_Item)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, 0x2f);  // Set Slot packet
	Pkt.WriteBEInt8(a_WindowID);
	Pkt.WriteBEInt16(a_SlotNum);
	WriteItem(Pkt, a_Item);
}





void cProtocol172::SendKeepAlive(UInt32 a_PingID)
{
	// Drop the packet if the protocol is not in the Game state yet (caused a client crash):
	if (m_State != 3)
	{
		LOGWARNING("Trying to send a KeepAlive packet to a player who's not yet fully logged in (%d). The protocol class prevented the packet.", m_State);
		return;
	}

	cPacketizer Pkt(*this, 0x00);  // Keep Alive packet
	Pkt.WriteBEUInt32(a_PingID);
}





void cProtocol172::SendLogin(const cPlayer & a_Player, const cWorld & a_World)
{
	// Send the Join Game packet:
	{
		cServer * Server = cRoot::Get()->GetServer();
		cPacketizer Pkt(*this, 0x01);  // Join Game packet
		Pkt.WriteBEUInt32(a_Player.GetUniqueID());
		Pkt.WriteBEUInt8(static_cast<Byte>(a_Player.GetEffectiveGameMode()) | (Server->IsHardcore() ? 0x08 : 0));  // Hardcore flag bit 4
		Pkt.WriteBEInt8(static_cast<char>(a_World.GetDimension()));
		Pkt.WriteBEUInt8(2);  // TODO: Difficulty (set to Normal)
		Pkt.WriteBEUInt8(static_cast<Byte>(std::min(Server->GetMaxPlayers(), 60)));
		Pkt.WriteString("default");  // Level type - wtf?
	}
	m_LastSentDimension = a_World.GetDimension();

	// Send the spawn position:
	{
		cPacketizer Pkt(*this, 0x05);  // Spawn Position packet
		Pkt.WriteBEInt32(static_cast<int>(a_World.GetSpawnX()));
		Pkt.WriteBEInt32(static_cast<int>(a_World.GetSpawnY()));
		Pkt.WriteBEInt32(static_cast<int>(a_World.GetSpawnZ()));
	}

	// Send player abilities:
	SendPlayerAbilities();
}




void cProtocol172::SendLoginSuccess(void)
{
	ASSERT(m_State == 2);  // State: login?

	{
		cPacketizer Pkt(*this, 0x02);  // Login success packet
		Pkt.WriteString(cMojangAPI::MakeUUIDDashed(m_Client->GetUUID()));
		Pkt.WriteString(m_Client->GetUsername());
	}

	m_State = 3;  // State = Game
}





void cProtocol172::SendPaintingSpawn(const cPainting & a_Painting)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, 0x10);  // Spawn Painting packet
	Pkt.WriteVarInt32(a_Painting.GetUniqueID());
	Pkt.WriteString(a_Painting.GetName().c_str());
	Pkt.WriteBEInt32(static_cast<int>(a_Painting.GetPosX()));
	Pkt.WriteBEInt32(static_cast<int>(a_Painting.GetPosY()));
	Pkt.WriteBEInt32(static_cast<int>(a_Painting.GetPosZ()));
	Pkt.WriteBEInt32(a_Painting.GetProtocolFacing());
}





void cProtocol172::SendMapData(const cMap & a_Map, int a_DataStartX, int a_DataStartY)
{
	ASSERT(m_State == 3);  // In game mode?

	{
		ASSERT(a_Map.GetScale() < 256);

		cPacketizer Pkt(*this, 0x34);
		Pkt.WriteVarInt32(static_cast<UInt32>(a_Map.GetID()));
		Pkt.WriteBEUInt16(2);

		Pkt.WriteBEUInt8(2);
		Pkt.WriteBEUInt8(static_cast<Byte>(a_Map.GetScale()));
	}

	{
		ASSERT(a_Map.GetData().size() + 3 <= USHRT_MAX);
		ASSERT((a_DataStartX >= 0) && (a_DataStartX < 256));
		ASSERT((a_DataStartY >= 0) && (a_DataStartY < 256));

		cPacketizer Pkt(*this, 0x34);
		Pkt.WriteVarInt32(static_cast<UInt32>(a_Map.GetID()));
		Pkt.WriteBEUInt16(static_cast<UInt16>(3 + a_Map.GetData().size()));

		Pkt.WriteBEUInt8(0);
		Pkt.WriteBEUInt8(static_cast<Byte>(a_DataStartX));
		Pkt.WriteBEUInt8(static_cast<Byte>(a_DataStartX));

		Pkt.WriteBuf(reinterpret_cast<const char *>(a_Map.GetData().data()), a_Map.GetData().size());
	}

	{
		ASSERT(1 + 3 * a_Map.GetDecorators().size() < USHRT_MAX);

		cPacketizer Pkt(*this, 0x34);
		Pkt.WriteVarInt32(static_cast<UInt32>(a_Map.GetID()));
		Pkt.WriteBEUInt16(static_cast<UInt16>(1 + (3 * a_Map.GetDecorators().size())));

		Pkt.WriteBEUInt8(1);

		for (const auto & Decorator : a_Map.GetDecorators())
		{
			ASSERT(Decorator.GetPixelX() < 256);
			ASSERT(Decorator.GetPixelZ() < 256);
			Pkt.WriteBEUInt8(static_cast<Byte>((Decorator.GetRot() << 4) | static_cast<int>(Decorator.GetType())));
			Pkt.WriteBEUInt8(static_cast<Byte>(Decorator.GetPixelX()));
			Pkt.WriteBEUInt8(static_cast<Byte>(Decorator.GetPixelZ()));
		}
	}
}





void cProtocol172::SendPickupSpawn(const cPickup & a_Pickup)
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
		Pkt.WriteBEUInt32(a_Pickup.GetUniqueID());
		Pkt.WriteBEUInt8((0x05 << 5) | 10);  // Slot type + index 10
		WriteItem(Pkt, a_Pickup.GetItem());
		Pkt.WriteBEUInt8(0x7f);  // End of metadata
	}
}





void cProtocol172::SendPlayerAbilities(void)
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





void cProtocol172::SendEntityAnimation(const cEntity & a_Entity, char a_Animation)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, 0x0b);  // Animation packet
	Pkt.WriteVarInt32(a_Entity.GetUniqueID());
	Pkt.WriteBEInt8(a_Animation);
}





void cProtocol172::SendParticleEffect(const AString & a_ParticleName, float a_SrcX, float a_SrcY, float a_SrcZ, float a_OffsetX, float a_OffsetY, float a_OffsetZ, float a_ParticleData, int a_ParticleAmount)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, 0x2A);
	Pkt.WriteString(a_ParticleName);
	Pkt.WriteBEFloat(a_SrcX);
	Pkt.WriteBEFloat(a_SrcY);
	Pkt.WriteBEFloat(a_SrcZ);
	Pkt.WriteBEFloat(a_OffsetX);
	Pkt.WriteBEFloat(a_OffsetY);
	Pkt.WriteBEFloat(a_OffsetZ);
	Pkt.WriteBEFloat(a_ParticleData);
	Pkt.WriteBEInt32(a_ParticleAmount);
}





void cProtocol172::SendParticleEffect(const AString & a_ParticleName, Vector3f a_Src, Vector3f a_Offset, float a_ParticleData, int a_ParticleAmount, std::array<int, 2> a_Data)
{
	// 1.72 doesn't support extra data
	this->SendParticleEffect(a_ParticleName, a_Src.x, a_Src.y, a_Src.z, a_Offset.x, a_Offset.y, a_Offset.z, a_ParticleData, a_ParticleAmount);
}





void cProtocol172::SendPlayerListAddPlayer(const cPlayer & a_Player)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, 0x38);  // Playerlist Item packet
	Pkt.WriteString(a_Player.GetPlayerListName());
	Pkt.WriteBool(true);
	Pkt.WriteBEInt16(a_Player.GetClientHandle()->GetPing());
}





void cProtocol172::SendPlayerListRemovePlayer(const cPlayer & a_Player)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, 0x38);
	Pkt.WriteString(a_Player.GetPlayerListName());
	Pkt.WriteBool(false);
	Pkt.WriteBEInt16(0);
}





void cProtocol172::SendPlayerListUpdateGameMode(const cPlayer & a_Player)
{
	// Not implemented in this protocol version
	UNUSED(a_Player);
}





void cProtocol172::SendPlayerListUpdatePing(const cPlayer & a_Player)
{
	// It is a simple add player packet in this protocol.
	SendPlayerListAddPlayer(a_Player);
}





void cProtocol172::SendPlayerListUpdateDisplayName(const cPlayer & a_Player, const AString & a_CustomName)
{
	// Not implemented in this protocol version
	UNUSED(a_Player);
	UNUSED(a_CustomName);
}





void cProtocol172::SendPlayerMaxSpeed(void)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, 0x20);  // Entity Properties
	cPlayer * Player = m_Client->GetPlayer();
	Pkt.WriteBEUInt32(Player->GetUniqueID());
	Pkt.WriteBEInt32(1);  // Count
	Pkt.WriteString("generic.movementSpeed");
	// The default game speed is 0.1, multiply that value by the relative speed:
	Pkt.WriteBEDouble(0.1 * Player->GetNormalMaxSpeed());
	if (Player->IsSprinting())
	{
		Pkt.WriteBEInt16(1);  // Modifier count
		Pkt.WriteBEUInt64(0x662a6b8dda3e4c1c);
		Pkt.WriteBEUInt64(0x881396ea6097278d);  // UUID of the modifier
		Pkt.WriteBEDouble(Player->GetSprintingMaxSpeed() - Player->GetNormalMaxSpeed());
		Pkt.WriteBEUInt8(2);
	}
	else
	{
		Pkt.WriteBEInt16(0);  // Modifier count
	}
}





void cProtocol172::SendPlayerMoveLook(void)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, 0x08);  // Player Position And Look packet
	cPlayer * Player = m_Client->GetPlayer();
	Pkt.WriteBEDouble(Player->GetPosX());

	// Protocol docs say this is PosY, but #323 says this is eye-pos
	// Moreover, the "+ 0.001" is there because otherwise the player falls through the block they were standing on.
	Pkt.WriteBEDouble(Player->GetStance() + 0.001);

	Pkt.WriteBEDouble(Player->GetPosZ());
	Pkt.WriteBEFloat(static_cast<float>(Player->GetYaw()));
	Pkt.WriteBEFloat(static_cast<float>(Player->GetPitch()));
	Pkt.WriteBool(Player->IsOnGround());
}





void cProtocol172::SendPlayerPosition(void)
{
	// There is no dedicated packet for this, send the whole thing:
	SendPlayerMoveLook();
}





void cProtocol172::SendPlayerSpawn(const cPlayer & a_Player)
{
	ASSERT(m_State == 3);  // In game mode?

	// Called to spawn another player for the client
	cPacketizer Pkt(*this, 0x0c);  // Spawn Player packet
	Pkt.WriteVarInt32(a_Player.GetUniqueID());
	Pkt.WriteString(cMojangAPI::MakeUUIDDashed(a_Player.GetClientHandle()->GetUUID()));
	if (a_Player.HasCustomName())
	{
		Pkt.WriteString(a_Player.GetCustomName());
	}
	else
	{
		Pkt.WriteString(a_Player.GetName());
	}
	Pkt.WriteFPInt(a_Player.GetPosX());
	Pkt.WriteFPInt(a_Player.GetPosY());
	Pkt.WriteFPInt(a_Player.GetPosZ());
	Pkt.WriteByteAngle(a_Player.GetYaw());
	Pkt.WriteByteAngle(a_Player.GetPitch());
	short ItemType = a_Player.GetEquippedItem().IsEmpty() ? 0 : a_Player.GetEquippedItem().m_ItemType;
	Pkt.WriteBEInt16(ItemType);
	Pkt.WriteBEUInt8((3 << 5) | 6);  // Metadata: float + index 6
	Pkt.WriteBEFloat(static_cast<float>(a_Player.GetHealth()));
	Pkt.WriteBEUInt8(0x7f);  // Metadata: end
}





void cProtocol172::SendPluginMessage(const AString & a_Channel, const AString & a_Message)
{
	ASSERT(m_State == 3);  // In game mode?
	ASSERT(a_Message.size() <= std::numeric_limits<UInt16>::max());

	cPacketizer Pkt(*this, 0x3f);
	Pkt.WriteString(a_Channel);
	Pkt.WriteBEUInt16(static_cast<UInt16>(a_Message.size()));
	Pkt.WriteBuf(a_Message.data(), a_Message.size());
}





void cProtocol172::SendRemoveEntityEffect(const cEntity & a_Entity, int a_EffectID)
{
	ASSERT(m_State == 3);  // In game mode?
	ASSERT((a_EffectID >= 0) && (a_EffectID < 256));

	cPacketizer Pkt(*this, 0x1e);
	Pkt.WriteBEUInt32(a_Entity.GetUniqueID());
	Pkt.WriteBEUInt8(static_cast<Byte>(a_EffectID));
}





void cProtocol172::SendResetTitle(void)
{
	// Not implemented in this protocol version
}





void cProtocol172::SendRespawn(eDimension a_Dimension, bool a_ShouldIgnoreDimensionChecks)
{
	if ((m_LastSentDimension == a_Dimension) && !a_ShouldIgnoreDimensionChecks)
	{
		// Must not send a respawn for the world with the same dimension, the client goes cuckoo if we do (unless we are respawning from death)
		return;
	}

	cPacketizer Pkt(*this, 0x07);  // Respawn packet
	cPlayer * Player = m_Client->GetPlayer();
	Pkt.WriteBEInt32(static_cast<int>(a_Dimension));
	Pkt.WriteBEUInt8(2);  // TODO: Difficulty (set to Normal)
	Pkt.WriteBEUInt8(static_cast<Byte>(Player->GetEffectiveGameMode()));
	Pkt.WriteString("default");
	m_LastSentDimension = a_Dimension;
}





void cProtocol172::SendExperience (void)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, 0x1f);  // Experience Packet
	cPlayer * Player = m_Client->GetPlayer();
	Pkt.WriteBEFloat(Player->GetXpPercentage());
	Pkt.WriteBEInt16(static_cast<Int16>(std::min<int>(Player->GetXpLevel(), std::numeric_limits<Int16>::max())));
	Pkt.WriteBEInt16(static_cast<Int16>(std::min<int>(Player->GetCurrentXp(), std::numeric_limits<Int16>::max())));
}





void cProtocol172::SendExperienceOrb(const cExpOrb & a_ExpOrb)
{
	ASSERT(m_State == 3);  // In game mode?
	ASSERT((a_ExpOrb.GetReward() >= 0) && (a_ExpOrb.GetReward() < SHRT_MAX));

	cPacketizer Pkt(*this, 0x11);
	Pkt.WriteVarInt32(a_ExpOrb.GetUniqueID());
	Pkt.WriteFPInt(a_ExpOrb.GetPosX());
	Pkt.WriteFPInt(a_ExpOrb.GetPosY());
	Pkt.WriteFPInt(a_ExpOrb.GetPosZ());
	Pkt.WriteBEInt16(static_cast<short>(a_ExpOrb.GetReward()));
}





void cProtocol172::SendScoreboardObjective(const AString & a_Name, const AString & a_DisplayName, Byte a_Mode)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, 0x3b);
	Pkt.WriteString(a_Name);
	Pkt.WriteString(a_DisplayName);
	Pkt.WriteBEUInt8(a_Mode);
}





void cProtocol172::SendScoreUpdate(const AString & a_Objective, const AString & a_Player, cObjective::Score a_Score, Byte a_Mode)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, 0x3c);
	Pkt.WriteString(a_Player);
	Pkt.WriteBEUInt8(a_Mode);

	if (a_Mode != 1)
	{
		Pkt.WriteString(a_Objective);
		Pkt.WriteBEInt32(static_cast<int>(a_Score));
	}
}





void cProtocol172::SendDisplayObjective(const AString & a_Objective, cScoreboard::eDisplaySlot a_Display)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, 0x3d);
	Pkt.WriteBEUInt8(static_cast<Byte>(a_Display));
	Pkt.WriteString(a_Objective);
}





void cProtocol172::SendSetSubTitle(const cCompositeChat & a_SubTitle)
{
	// Not implemented in this protocol version
}





void cProtocol172::SendSetRawSubTitle(const AString & a_SubTitle)
{
	// Not implemented in this protocol version
}





void cProtocol172::SendSetTitle(const cCompositeChat & a_Title)
{
	// Not implemented in this protocol version
}





void cProtocol172::SendSetRawTitle(const AString & a_Title)
{
	// Not implemented in this protocol version
}





void cProtocol172::SendSoundEffect(const AString & a_SoundName, double a_X, double a_Y, double a_Z, float a_Volume, float a_Pitch)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, 0x29);  // Sound Effect packet
	Pkt.WriteString(a_SoundName);
	Pkt.WriteBEInt32(static_cast<int>(a_X * 8.0));
	Pkt.WriteBEInt32(static_cast<int>(a_Y * 8.0));
	Pkt.WriteBEInt32(static_cast<int>(a_Z * 8.0));
	Pkt.WriteBEFloat(a_Volume);
	Pkt.WriteBEUInt8(static_cast<Byte>(a_Pitch * 63));
}





void cProtocol172::SendSoundParticleEffect(const EffectID a_EffectID, int a_SrcX, int a_SrcY, int a_SrcZ, int a_Data)
{
	ASSERT(m_State == 3);  // In game mode?
	ASSERT((a_SrcY >= 0) && (a_SrcY < 256));

	cPacketizer Pkt(*this, 0x28);  // Effect packet
	Pkt.WriteBEInt32(static_cast<int>(a_EffectID));
	Pkt.WriteBEInt32(a_SrcX);
	Pkt.WriteBEUInt8(static_cast<Byte>(a_SrcY));
	Pkt.WriteBEInt32(a_SrcZ);
	Pkt.WriteBEInt32(a_Data);
	Pkt.WriteBool(false);
}





void cProtocol172::SendSpawnFallingBlock(const cFallingBlock & a_FallingBlock)
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
	Pkt.WriteBEInt32((static_cast<int>(a_FallingBlock.GetBlockType()) | ((static_cast<int>(a_FallingBlock.GetBlockMeta()) << 16))));
	Pkt.WriteBEInt16(static_cast<short>(a_FallingBlock.GetSpeedX() * 400));
	Pkt.WriteBEInt16(static_cast<short>(a_FallingBlock.GetSpeedY() * 400));
	Pkt.WriteBEInt16(static_cast<short>(a_FallingBlock.GetSpeedZ() * 400));
}





void cProtocol172::SendSpawnMob(const cMonster & a_Mob)
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
	Pkt.WriteBEInt16(static_cast<short>(a_Mob.GetSpeedX() * 400));
	Pkt.WriteBEInt16(static_cast<short>(a_Mob.GetSpeedY() * 400));
	Pkt.WriteBEInt16(static_cast<short>(a_Mob.GetSpeedZ() * 400));
	WriteEntityMetadata(Pkt, a_Mob);
	Pkt.WriteBEUInt8(0x7f);  // Metadata terminator
}





void cProtocol172::SendSpawnObject(const cEntity & a_Entity, char a_ObjectType, int a_ObjectData, Byte a_Yaw, Byte a_Pitch)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, 0xe);  // Spawn Object packet
	Pkt.WriteVarInt32(a_Entity.GetUniqueID());
	Pkt.WriteBEInt8(a_ObjectType);
	Pkt.WriteFPInt(a_Entity.GetPosX());
	Pkt.WriteFPInt(a_Entity.GetPosY());
	Pkt.WriteFPInt(a_Entity.GetPosZ());
	Pkt.WriteByteAngle(a_Entity.GetPitch());
	Pkt.WriteByteAngle(a_Entity.GetYaw());
	Pkt.WriteBEInt32(a_ObjectData);
	if (a_ObjectData != 0)
	{
		Pkt.WriteBEInt16(static_cast<short>(a_Entity.GetSpeedX() * 400));
		Pkt.WriteBEInt16(static_cast<short>(a_Entity.GetSpeedY() * 400));
		Pkt.WriteBEInt16(static_cast<short>(a_Entity.GetSpeedZ() * 400));
	}
}





void cProtocol172::SendSpawnVehicle(const cEntity & a_Vehicle, char a_VehicleType, char a_VehicleSubType)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, 0xe);  // Spawn Object packet
	Pkt.WriteVarInt32(a_Vehicle.GetUniqueID());
	Pkt.WriteBEInt8(a_VehicleType);
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





void cProtocol172::SendStatistics(const cStatManager & a_Manager)
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





void cProtocol172::SendTabCompletionResults(const AStringVector & a_Results)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, 0x3a);  // Tab-Complete packet
	Pkt.WriteVarInt32(static_cast<UInt32>(a_Results.size()));

	for (AStringVector::const_iterator itr = a_Results.begin(), end = a_Results.end(); itr != end; ++itr)
	{
		Pkt.WriteString(*itr);
	}
}





void cProtocol172::SendTeleportEntity(const cEntity & a_Entity)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, 0x18);
	Pkt.WriteBEUInt32(a_Entity.GetUniqueID());
	Pkt.WriteFPInt(a_Entity.GetPosX());
	Pkt.WriteFPInt(a_Entity.GetPosY());
	Pkt.WriteFPInt(a_Entity.GetPosZ());
	Pkt.WriteByteAngle(a_Entity.GetYaw());
	Pkt.WriteByteAngle(a_Entity.GetPitch());
}





void cProtocol172::SendThunderbolt(int a_BlockX, int a_BlockY, int a_BlockZ)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, 0x2c);  // Spawn Global Entity packet
	Pkt.WriteVarInt32(0);  // EntityID = 0, always
	Pkt.WriteBEUInt8(1);  // Type = Thunderbolt
	Pkt.WriteFPInt(a_BlockX);
	Pkt.WriteFPInt(a_BlockY);
	Pkt.WriteFPInt(a_BlockZ);
}





void cProtocol172::SendTitleTimes(int a_FadeInTicks, int a_DisplayTicks, int a_FadeOutTicks)
{
	// Not implemented in this protocol version
}





void cProtocol172::SendTimeUpdate(Int64 a_WorldAge, Int64 a_TimeOfDay, bool a_DoDaylightCycle)
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





void cProtocol172::SendUnloadChunk(int a_ChunkX, int a_ChunkZ)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, 0x21);  // Chunk Data packet
	Pkt.WriteBEInt32(a_ChunkX);
	Pkt.WriteBEInt32(a_ChunkZ);
	Pkt.WriteBool(true);
	Pkt.WriteBEInt16(0);  // Primary bitmap
	Pkt.WriteBEInt16(0);  // Add bitmap
	Pkt.WriteBEInt32(0);  // Compressed data size
}




void cProtocol172::SendUpdateBlockEntity(cBlockEntity & a_BlockEntity)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, 0x35);  // Update tile entity packet
	Pkt.WriteBEInt32(a_BlockEntity.GetPosX());
	Pkt.WriteBEInt16(static_cast<short>(a_BlockEntity.GetPosY()));
	Pkt.WriteBEInt32(a_BlockEntity.GetPosZ());

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





void cProtocol172::SendUpdateSign(int a_BlockX, int a_BlockY, int a_BlockZ, const AString & a_Line1, const AString & a_Line2, const AString & a_Line3, const AString & a_Line4)
{
	ASSERT(m_State == 3);  // In game mode?
	ASSERT((a_BlockY >= 0) && (a_BlockY < cChunkDef::Height));

	cPacketizer Pkt(*this, 0x33);
	Pkt.WriteBEInt32(a_BlockX);
	Pkt.WriteBEInt16(static_cast<Int16>(a_BlockY));
	Pkt.WriteBEInt32(a_BlockZ);
	// Need to send only up to 15 chars, otherwise the client crashes (#598)
	Pkt.WriteString(a_Line1.substr(0, 15));
	Pkt.WriteString(a_Line2.substr(0, 15));
	Pkt.WriteString(a_Line3.substr(0, 15));
	Pkt.WriteString(a_Line4.substr(0, 15));
}





void cProtocol172::SendUseBed(const cEntity & a_Entity, int a_BlockX, int a_BlockY, int a_BlockZ)
{
	ASSERT(m_State == 3);  // In game mode?
	ASSERT((a_BlockY >= 0) && (a_BlockY < cChunkDef::Height));

	cPacketizer Pkt(*this, 0x0a);
	Pkt.WriteBEUInt32(a_Entity.GetUniqueID());
	Pkt.WriteBEInt32(a_BlockX);
	Pkt.WriteBEUInt8(static_cast<Byte>(a_BlockY));
	Pkt.WriteBEInt32(a_BlockZ);
}





void cProtocol172::SendWeather(eWeather a_Weather)
{
	ASSERT(m_State == 3);  // In game mode?

	{
		cPacketizer Pkt(*this, 0x2b);  // Change Game State packet
		Pkt.WriteBEUInt8((a_Weather == wSunny) ? 1 : 2);  // End rain / begin rain
		Pkt.WriteBEFloat(0);  // Unused for weather
	}

	// TODO: Fade effect, somehow
}





void cProtocol172::SendWholeInventory(const cWindow & a_Window)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, 0x30);  // Window Items packet
	Pkt.WriteBEInt8(a_Window.GetWindowID());
	Pkt.WriteBEInt16(static_cast<short>(a_Window.GetNumSlots()));
	cItems Slots;
	a_Window.GetSlots(*(m_Client->GetPlayer()), Slots);
	for (cItems::const_iterator itr = Slots.begin(), end = Slots.end(); itr != end; ++itr)
	{
		WriteItem(Pkt, *itr);
	}  // for itr - Slots[]
}





void cProtocol172::SendWindowClose(const cWindow & a_Window)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, 0x2e);
	Pkt.WriteBEInt8(a_Window.GetWindowID());
}





void cProtocol172::SendWindowOpen(const cWindow & a_Window)
{
	ASSERT(m_State == 3);  // In game mode?

	if (a_Window.GetWindowType() < 0)
	{
		// Do not send this packet for player inventory windows
		return;
	}

	cPacketizer Pkt(*this, 0x2d);
	Pkt.WriteBEInt8(a_Window.GetWindowID());
	Pkt.WriteBEInt8(static_cast<char>(a_Window.GetWindowType()));
	Pkt.WriteString(a_Window.GetWindowTitle());
	Pkt.WriteBEInt8(static_cast<char>(a_Window.GetNumNonInventorySlots()));
	Pkt.WriteBool(true);
	if (a_Window.GetWindowType() == cWindow::wtAnimalChest)
	{
		Pkt.WriteBEInt32(0);  // TODO: The animal's EntityID
	}
}





void cProtocol172::SendWindowProperty(const cWindow & a_Window, short a_Property, short a_Value)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, 0x31);  // Window Property packet
	Pkt.WriteBEInt8(a_Window.GetWindowID());
	Pkt.WriteBEInt16(a_Property);
	Pkt.WriteBEInt16(a_Value);
}





void cProtocol172::AddReceivedData(const char * a_Data, size_t a_Size)
{
	// Write the incoming data into the comm log file:
	if (g_ShouldLogCommIn)
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
		cByteBuffer bb(PacketLen + 1);
		VERIFY(m_ReceivedData.ReadToByteBuffer(bb, static_cast<size_t>(PacketLen)));
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
		if (g_ShouldLogCommIn)
		{
			AString PacketData;
			bb.ReadAll(PacketData);
			bb.ResetRead();
			bb.ReadVarInt(PacketType);  // We have already read the packet type once, it will be there again.
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
			if (g_ShouldLogCommIn)
			{
				m_CommLogFile.Printf("^^^^^^ Unhandled packet ^^^^^^\n\n\n");
			}

			return;
		}

		if (bb.GetReadableSpace() != 1)
		{
			// Read more or less than packet length, report as error
			LOGWARNING("Protocol 1.7: Wrong number of bytes read for packet 0x%x, state %d. Read " SIZE_T_FMT " bytes, packet contained %u bytes",
				PacketType, m_State, bb.GetUsedSpace() - bb.GetReadableSpace(), PacketLen
			);

			// Put a message in the comm log:
			if (g_ShouldLogCommIn)
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
	if (g_ShouldLogCommIn && (m_ReceivedData.GetReadableSpace() > 0))
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




bool cProtocol172::HandlePacket(cByteBuffer & a_ByteBuffer, UInt32 a_PacketType)
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





void cProtocol172::HandlePacketStatusPing(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadBEInt64, Int64, Timestamp);

	cPacketizer Pkt(*this, 0x01);  // Ping packet
	Pkt.WriteBEInt64(Timestamp);
}





void cProtocol172::HandlePacketStatusRequest(cByteBuffer & a_ByteBuffer)
{
	cServer * Server = cRoot::Get()->GetServer();
	AString ServerDescription = Server->GetDescription();
	int NumPlayers = Server->GetNumPlayers();
	int MaxPlayers = Server->GetMaxPlayers();
	AString Favicon = Server->GetFaviconData();
	cRoot::Get()->GetPluginManager()->CallHookServerPing(*m_Client, ServerDescription, NumPlayers, MaxPlayers, Favicon);

	// Version:
	Json::Value Version;
	Version["name"] = "Cuberite 1.7.2";
	Version["protocol"] = 4;

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





void cProtocol172::HandlePacketLoginEncryptionResponse(cByteBuffer & a_ByteBuffer)
{
	UInt16 EncKeyLength, EncNonceLength;
	if (!a_ByteBuffer.ReadBEUInt16(EncKeyLength))
	{
		return;
	}
	if (EncKeyLength > MAX_ENC_LEN)
	{
		LOGD("Invalid Encryption Key length: %u (0x%04x). Kicking client.", EncKeyLength, EncKeyLength);
		m_Client->Kick("Invalid EncKeyLength");
		return;
	}
	AString EncKey;
	if (!a_ByteBuffer.ReadString(EncKey, EncKeyLength))
	{
		return;
	}
	if (!a_ByteBuffer.ReadBEUInt16(EncNonceLength))
	{
		return;
	}
	if (EncNonceLength > MAX_ENC_LEN)
	{
		LOGD("Invalid Encryption Nonce length: %u (0x%04x). Kicking client.", EncNonceLength, EncNonceLength);
		m_Client->Kick("Invalid EncNonceLength");
		return;
	}
	AString EncNonce;
	if (!a_ByteBuffer.ReadString(EncNonce, EncNonceLength))
	{
		return;
	}

	// Decrypt EncNonce using privkey
	cRsaPrivateKey & rsaDecryptor = cRoot::Get()->GetServer()->GetPrivateKey();
	UInt32 DecryptedNonce[MAX_ENC_LEN / sizeof(UInt32)];
	int res = rsaDecryptor.Decrypt(
		reinterpret_cast<const Byte *>(EncNonce.data()), EncNonce.size(),
		reinterpret_cast<Byte *>(DecryptedNonce), sizeof(DecryptedNonce)
	);
	if (res != 4)
	{
		LOGD("Bad nonce length: got %d, exp %d", res, 4);
		m_Client->Kick("Hacked client");
		return;
	}
	if (ntohl(DecryptedNonce[0]) != static_cast<UInt32>(reinterpret_cast<uintptr_t>(this)))
	{
		LOGD("Bad nonce value");
		m_Client->Kick("Hacked client");
		return;
	}

	// Decrypt the symmetric encryption key using privkey:
	Byte DecryptedKey[MAX_ENC_LEN];
	res = rsaDecryptor.Decrypt(
		reinterpret_cast<const Byte *>(EncKey.data()), EncKey.size(),
		DecryptedKey, sizeof(DecryptedKey)
	);
	if (res != 16)
	{
		LOGD("Bad key length");
		m_Client->Kick("Hacked client");
		return;
	}

	StartEncryption(DecryptedKey);
	m_Client->HandleLogin(4, m_Client->GetUsername());
}





void cProtocol172::HandlePacketLoginStart(cByteBuffer & a_ByteBuffer)
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
		cPacketizer Pkt(*this, 0x01);
		Pkt.WriteString(Server->GetServerID());
		const AString & PubKeyDer = Server->GetPublicKeyDER();
		Pkt.WriteBEUInt16(static_cast<UInt16>(PubKeyDer.size()));
		Pkt.WriteBuf(PubKeyDer.data(), PubKeyDer.size());
		Pkt.WriteBEInt16(4);
		Pkt.WriteBEUInt32(static_cast<UInt32>(reinterpret_cast<uintptr_t>(this)));  // Using 'this' as the cryptographic nonce, so that we don't have to generate one each time :)
		m_Client->SetUsername(Username);
		return;
	}

	m_Client->HandleLogin(4, Username);
}





void cProtocol172::HandlePacketAnimation(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadBEUInt32, UInt32, EntityID);
	HANDLE_READ(a_ByteBuffer, ReadBEUInt8,  UInt8,  Animation);
	m_Client->HandleAnimation(Animation);
}





void cProtocol172::HandlePacketBlockDig(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadBEUInt8, UInt8, Status);
	HANDLE_READ(a_ByteBuffer, ReadBEInt32, Int32, BlockX);
	HANDLE_READ(a_ByteBuffer, ReadBEUInt8, UInt8, BlockY);
	HANDLE_READ(a_ByteBuffer, ReadBEInt32, Int32, BlockZ);
	HANDLE_READ(a_ByteBuffer, ReadBEInt8,  Int8,  Face);
	m_Client->HandleLeftClick(BlockX, BlockY, BlockZ, FaceIntToBlockFace(Face), Status);
}





void cProtocol172::HandlePacketBlockPlace(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadBEInt32, Int32, BlockX);
	HANDLE_READ(a_ByteBuffer, ReadBEUInt8, UInt8, BlockY);
	HANDLE_READ(a_ByteBuffer, ReadBEInt32, Int32, BlockZ);
	HANDLE_READ(a_ByteBuffer, ReadBEInt8,  Int8,  Face);
	cItem Item;
	ReadItem(a_ByteBuffer, Item);

	HANDLE_READ(a_ByteBuffer, ReadBEUInt8, UInt8, CursorX);
	HANDLE_READ(a_ByteBuffer, ReadBEUInt8, UInt8, CursorY);
	HANDLE_READ(a_ByteBuffer, ReadBEUInt8, UInt8, CursorZ);
	m_Client->HandleRightClick(BlockX, BlockY, BlockZ, FaceIntToBlockFace(Face), CursorX, CursorY, CursorZ, m_Client->GetPlayer()->GetEquippedItem());
}





void cProtocol172::HandlePacketChatMessage(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadVarUTF8String, AString, Message);
	m_Client->HandleChat(Message);
}





void cProtocol172::HandlePacketClientSettings(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadVarUTF8String, AString, Locale);
	HANDLE_READ(a_ByteBuffer, ReadBEUInt8,       UInt8,   ViewDistance);
	HANDLE_READ(a_ByteBuffer, ReadBEUInt8,       UInt8,   ChatFlags);
	HANDLE_READ(a_ByteBuffer, ReadBEUInt8,       UInt8,   ChatColors);
	HANDLE_READ(a_ByteBuffer, ReadBEUInt8,       UInt8,   Difficulty);
	HANDLE_READ(a_ByteBuffer, ReadBool,          bool,    ShowCape);

	m_Client->SetLocale(Locale);
	m_Client->SetViewDistance(ViewDistance);
	// TODO: Do anything with the other values.
}





void cProtocol172::HandlePacketClientStatus(cByteBuffer & a_ByteBuffer)
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
			m_Client->GetPlayer()->AwardAchievement(achOpenInv);
			break;
		}
	}
}





void cProtocol172::HandlePacketCreativeInventoryAction(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadBEInt16, Int16, SlotNum);
	cItem Item;
	if (!ReadItem(a_ByteBuffer, Item))
	{
		return;
	}
	m_Client->HandleCreativeInventory(SlotNum, Item, (SlotNum < 0) ? caLeftClick : caLeftClickOutside);
}





void cProtocol172::HandlePacketEntityAction(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadBEUInt32, UInt32, PlayerID);
	HANDLE_READ(a_ByteBuffer, ReadBEUInt8,  UInt8,  Action);
	HANDLE_READ(a_ByteBuffer, ReadBEInt32,  Int32,  JumpBoost);

	switch (Action)
	{
		case 1: m_Client->HandleEntityCrouch(PlayerID, true);     break;  // Crouch
		case 2: m_Client->HandleEntityCrouch(PlayerID, false);    break;  // Uncrouch
		case 3: m_Client->HandleEntityLeaveBed(PlayerID);         break;  // Leave Bed
		case 4: m_Client->HandleEntitySprinting(PlayerID, true);  break;  // Start sprinting
		case 5: m_Client->HandleEntitySprinting(PlayerID, false); break;  // Stop sprinting
	}
}





void cProtocol172::HandlePacketKeepAlive(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadBEUInt32, UInt32, KeepAliveID);
	m_Client->HandleKeepAlive(KeepAliveID);
}





void cProtocol172::HandlePacketPlayer(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadBool, bool, IsOnGround);
	// TODO: m_Client->HandlePlayerOnGround(IsOnGround);
}





void cProtocol172::HandlePacketPlayerAbilities(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadBEUInt8, UInt8, Flags);
	HANDLE_READ(a_ByteBuffer, ReadBEFloat, float, FlyingSpeed);
	HANDLE_READ(a_ByteBuffer, ReadBEFloat, float, WalkingSpeed);

	// Convert flags bitfield into individual bool flags:
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





void cProtocol172::HandlePacketPlayerLook(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadBEFloat, float, Yaw);
	HANDLE_READ(a_ByteBuffer, ReadBEFloat, float, Pitch);
	HANDLE_READ(a_ByteBuffer, ReadBool,    bool,  IsOnGround);
	m_Client->HandlePlayerLook(Yaw, Pitch, IsOnGround);
}





void cProtocol172::HandlePacketPlayerPos(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadBEDouble, double, PosX);
	HANDLE_READ(a_ByteBuffer, ReadBEDouble, double, PosY);
	HANDLE_READ(a_ByteBuffer, ReadBEDouble, double, Stance);
	HANDLE_READ(a_ByteBuffer, ReadBEDouble, double, PosZ);
	HANDLE_READ(a_ByteBuffer, ReadBool,     bool,   IsOnGround);
	m_Client->HandlePlayerPos(PosX, PosY, PosZ, Stance, IsOnGround);
}





void cProtocol172::HandlePacketPlayerPosLook(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadBEDouble, double, PosX);
	HANDLE_READ(a_ByteBuffer, ReadBEDouble, double, PosY);
	HANDLE_READ(a_ByteBuffer, ReadBEDouble, double, Stance);
	HANDLE_READ(a_ByteBuffer, ReadBEDouble, double, PosZ);
	HANDLE_READ(a_ByteBuffer, ReadBEFloat,  float,  Yaw);
	HANDLE_READ(a_ByteBuffer, ReadBEFloat,  float,  Pitch);
	HANDLE_READ(a_ByteBuffer, ReadBool,     bool,   IsOnGround);
	m_Client->HandlePlayerMoveLook(PosX, PosY, PosZ, Stance, Yaw, Pitch, IsOnGround);
}





void cProtocol172::HandlePacketPluginMessage(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadVarUTF8String, AString, Channel);
	HANDLE_READ(a_ByteBuffer, ReadBEUInt16,      UInt16,  Length);
	if (Length != a_ByteBuffer.GetReadableSpace() - 1)
	{
		LOGD("Invalid plugin message packet, payload length doesn't match packet length (exp %u, got %u)",
			static_cast<unsigned>(a_ByteBuffer.GetReadableSpace() - 1), Length
		);
		return;
	}

	// If the plugin channel is recognized vanilla, handle it directly:
	if (Channel.substr(0, 3) == "MC|")
	{
		HandleVanillaPluginMessage(a_ByteBuffer, Channel, Length);
		return;
	}

	// Read the plugin message and relay to clienthandle:
	AString Data;
	if (!a_ByteBuffer.ReadString(Data, Length))
	{
		return;
	}
	m_Client->HandlePluginMessage(Channel, Data);
}





void cProtocol172::HandlePacketSlotSelect(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadBEInt16, Int16, SlotNum);
	m_Client->HandleSlotSelected(SlotNum);
}





void cProtocol172::HandlePacketSteerVehicle(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadBEFloat, float, Forward);
	HANDLE_READ(a_ByteBuffer, ReadBEFloat, float, Sideways);
	HANDLE_READ(a_ByteBuffer, ReadBool,    bool,  ShouldJump);
	HANDLE_READ(a_ByteBuffer, ReadBool,    bool,  ShouldUnmount);
	if (ShouldUnmount)
	{
		m_Client->HandleUnmount();
	}
	else
	{
		m_Client->HandleSteerVehicle(Forward, Sideways);
	}
}





void cProtocol172::HandlePacketTabComplete(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadVarUTF8String, AString, Text);
	m_Client->HandleTabCompletion(Text);
}





void cProtocol172::HandlePacketUpdateSign(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadBEInt32,       Int32,   BlockX);
	HANDLE_READ(a_ByteBuffer, ReadBEInt16,       Int16,   BlockY);
	HANDLE_READ(a_ByteBuffer, ReadBEInt32,       Int32,   BlockZ);
	HANDLE_READ(a_ByteBuffer, ReadVarUTF8String, AString, Line1);
	HANDLE_READ(a_ByteBuffer, ReadVarUTF8String, AString, Line2);
	HANDLE_READ(a_ByteBuffer, ReadVarUTF8String, AString, Line3);
	HANDLE_READ(a_ByteBuffer, ReadVarUTF8String, AString, Line4);
	m_Client->HandleUpdateSign(BlockX, BlockY, BlockZ, Line1, Line2, Line3, Line4);
}





void cProtocol172::HandlePacketUseEntity(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadBEUInt32, UInt32,  EntityID);
	HANDLE_READ(a_ByteBuffer, ReadBEUInt8,  UInt8, MouseButton);
	m_Client->HandleUseEntity(EntityID, (MouseButton == 1));
}





void cProtocol172::HandlePacketEnchantItem(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadBEUInt8, UInt8, WindowID);
	HANDLE_READ(a_ByteBuffer, ReadBEUInt8, UInt8, Enchantment);

	m_Client->HandleEnchantItem(WindowID, Enchantment);
}





void cProtocol172::HandlePacketWindowClick(cByteBuffer & a_ByteBuffer)
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

	m_Client->HandleWindowClick(WindowID, SlotNum, Action, Item);
}





void cProtocol172::HandlePacketWindowClose(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadBEUInt8, UInt8, WindowID);
	m_Client->HandleWindowClose(WindowID);
}





void cProtocol172::HandleVanillaPluginMessage(cByteBuffer & a_ByteBuffer, const AString & a_Channel, UInt16 a_PayloadLength)
{
	if (a_Channel == "MC|AdvCdm")
	{
		size_t BeginningSpace = a_ByteBuffer.GetReadableSpace();
		HANDLE_READ(a_ByteBuffer, ReadBEUInt8, UInt8, Mode);
		switch (Mode)
		{
			case 0x00:
			{
				// Block-based commandblock update:
				HANDLE_READ(a_ByteBuffer, ReadBEInt32, Int32, BlockX);
				HANDLE_READ(a_ByteBuffer, ReadBEInt32, Int32, BlockY);
				HANDLE_READ(a_ByteBuffer, ReadBEInt32, Int32, BlockZ);
				HANDLE_READ(a_ByteBuffer, ReadVarUTF8String, AString, Command);
				m_Client->HandleCommandBlockBlockChange(BlockX, BlockY, BlockZ, Command);
				break;
			}

			// TODO: Entity-based commandblock update

			default:
			{
				m_Client->SendChat(Printf("Failure setting command block command; unhandled mode %d", Mode), mtFailure);
				LOG("Unhandled MC|AdvCdm packet mode.");
				return;
			}
		}  // switch (Mode)

		// Read the remainder of the packet (Vanilla sometimes sends bogus data at the end of the packet; #1692):
		size_t BytesRead = BeginningSpace - a_ByteBuffer.GetReadableSpace();
		if (BytesRead < a_PayloadLength)
		{
			LOGD("Protocol 1.7: Skipping garbage data at the end of a vanilla MC|AdvCdm packet, %u bytes",
				static_cast<unsigned>(a_PayloadLength - BytesRead)
			);
			a_ByteBuffer.SkipRead(a_PayloadLength - BytesRead);
		}
		return;
	}
	else if (a_Channel == "MC|Brand")
	{
		// Read the client's brand:
		AString Brand;
		if (a_ByteBuffer.ReadString(Brand, a_PayloadLength))
		{
			m_Client->SetClientBrand(Brand);
		}

		// Send back our brand:
		SendPluginMessage("MC|Brand", "Cuberite");
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
		AString ItemName;
		if (a_ByteBuffer.ReadString(ItemName, a_PayloadLength))
		{
			m_Client->HandleAnvilItemName(ItemName);
		}
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
	VERIFY(a_ByteBuffer.ReadString(Message, a_PayloadLength));
	m_Client->HandlePluginMessage(a_Channel, Message);
}





void cProtocol172::SendData(const char * a_Data, size_t a_Size)
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





void cProtocol172::SendPacket(cPacketizer & a_Packet)
{
	AString DataToSend;

	// Send the packet length
	UInt32 PacketLen = static_cast<UInt32>(m_OutPacketBuffer.GetUsedSpace());

	m_OutPacketLenBuffer.WriteVarInt32(PacketLen);
	m_OutPacketLenBuffer.ReadAll(DataToSend);
	SendData(DataToSend.data(), DataToSend.size());
	m_OutPacketLenBuffer.CommitRead();

	// Send the packet data:
	m_OutPacketBuffer.ReadAll(DataToSend);
	SendData(DataToSend.data(), DataToSend.size());
	m_OutPacketBuffer.CommitRead();

	// Log the comm into logfile:
	if (g_ShouldLogCommOut)
	{
		AString Hex;
		ASSERT(DataToSend.size() > 0);
		CreateHexDump(Hex, DataToSend.data(), DataToSend.size(), 16);
		m_CommLogFile.Printf("Outgoing packet: type %d (0x%x), length %u (0x%x), state %d. Payload (incl. type):\n%s\n",
			a_Packet.GetPacketType(), a_Packet.GetPacketType(), PacketLen, PacketLen, m_State, Hex.c_str()
		);
	}
}





bool cProtocol172::ReadItem(cByteBuffer & a_ByteBuffer, cItem & a_Item)
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

	// Read the metadata
	HANDLE_PACKET_READ(a_ByteBuffer, ReadBEUInt16, UInt16, MetadataLength);
	AString Metadata;
	if (!a_ByteBuffer.ReadString(Metadata, MetadataLength))
	{
		return false;
	}

	ParseItemMetadata(a_Item, Metadata);
	return true;
}





void cProtocol172::ParseItemMetadata(cItem & a_Item, const AString & a_Metadata)
{
	// Uncompress the GZIPped data:
	AString Uncompressed;
	if (UncompressStringGZIP(a_Metadata.data(), a_Metadata.size(), Uncompressed) != Z_OK)
	{
		AString HexDump;
		CreateHexDump(HexDump, a_Metadata.data(), a_Metadata.size(), 16);
		LOGWARNING("Cannot unGZIP item metadata (" SIZE_T_FMT " bytes):\n%s", a_Metadata.size(), HexDump.c_str());
		return;
	}

	// Parse into NBT:
	cParsedNBT NBT(Uncompressed.data(), Uncompressed.size());
	if (!NBT.IsValid())
	{
		AString HexDump;
		CreateHexDump(HexDump, Uncompressed.data(), Uncompressed.size(), 16);
		LOGWARNING("Cannot parse NBT item metadata: (" SIZE_T_FMT " bytes)\n%s", Uncompressed.size(), HexDump.c_str());
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





void cProtocol172::StartEncryption(const Byte * a_Key)
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





eBlockFace cProtocol172::FaceIntToBlockFace(Int8 a_BlockFace)
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





void cProtocol172::WriteItem(cPacketizer & a_Pkt, const cItem & a_Item)
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
		a_Pkt.WriteBEInt16(-1);
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
			Writer.AddInt("color", static_cast<int>(a_Item.m_ItemColor.m_Color));
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

	AString Compressed;
	CompressStringGZIP(Writer.GetResult().data(), Writer.GetResult().size(), Compressed);
	a_Pkt.WriteBEUInt16(static_cast<UInt16>(Compressed.size()));
	a_Pkt.WriteBuf(Compressed.data(), Compressed.size());
}





void cProtocol172::WriteBlockEntity(cPacketizer & a_Pkt, const cBlockEntity & a_BlockEntity)
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
			Writer.AddInt("x",            CommandBlockEntity.GetPosX());
			Writer.AddInt("y",            CommandBlockEntity.GetPosY());
			Writer.AddInt("z",            CommandBlockEntity.GetPosZ());
			Writer.AddString("Command",   CommandBlockEntity.GetCommand().c_str());
			// You can set custom names for windows in Vanilla
			// For a command block, this would be the 'name' prepended to anything it outputs into global chat
			// MCS doesn't have this, so just leave it at '@'.
			Writer.AddString("CustomName", "@");
			Writer.AddString("id",         "Control");  // "Tile Entity ID" - MC wiki; vanilla server always seems to send this though
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
			Writer.AddString("ExtraType", MobHeadEntity.GetOwnerName());
			Writer.AddString("id", "Skull");  // "Tile Entity ID" - MC wiki; vanilla server always seems to send this though
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

	AString Compressed;
	CompressStringGZIP(Writer.GetResult().data(), Writer.GetResult().size(), Compressed);
	a_Pkt.WriteBEUInt16(static_cast<UInt16>(Compressed.size()));
	a_Pkt.WriteBuf(Compressed.data(), Compressed.size());
}





void cProtocol172::WriteEntityMetadata(cPacketizer & a_Pkt, const cEntity & a_Entity)
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
					a_Pkt.WriteBEUInt8(reinterpret_cast<const cArrowEntity &>(a_Entity).IsCritical() ? 1 : 0);
					break;
				}
				case cProjectileEntity::pkFirework:
				{
					a_Pkt.WriteBEUInt8(0xa8);
					WriteItem(a_Pkt, reinterpret_cast<const cFireworkEntity &>(a_Entity).GetItem());
					break;
				}
				default: break;
			}
			break;
		}  // case etProjectile

		case cEntity::etMonster:
		{
			WriteMobMetadata(a_Pkt, reinterpret_cast<const cMonster &>(a_Entity));
			break;
		}  // case etMonster

		case cEntity::etItemFrame:
		{
			auto & Frame = reinterpret_cast<const cItemFrame &>(a_Entity);
			a_Pkt.WriteBEUInt8(0xa2);
			WriteItem(a_Pkt, Frame.GetItem());
			a_Pkt.WriteBEUInt8(0x03);
			a_Pkt.WriteBEUInt8(Frame.GetItemRotation() / 2);
			break;
		}  // case etItemFrame

		default:
		{
			break;
		}
	}  // switch (EntityType)
}





void cProtocol172::WriteMobMetadata(cPacketizer & a_Pkt, const cMonster & a_Mob)
{
	// Living Enitiy Metadata
	if (a_Mob.HasCustomName())
	{
		a_Pkt.WriteBEUInt8(0x8a);
		a_Pkt.WriteString(a_Mob.GetCustomName());

		a_Pkt.WriteBEUInt8(0x0b);
		a_Pkt.WriteBool(a_Mob.IsCustomNameAlwaysVisible());
	}

	a_Pkt.WriteBEUInt8(0x66);
	a_Pkt.WriteBEFloat(static_cast<float>(a_Mob.GetHealth()));

	switch (a_Mob.GetMobType())
	{
		case mtBat:
		{
			a_Pkt.WriteBEUInt8(0x10);
			a_Pkt.WriteBEUInt8(reinterpret_cast<const cBat &>(a_Mob).IsHanging() ? 1 : 0);
			break;
		}  // case mtBat

		case mtCreeper:
		{
			a_Pkt.WriteBEUInt8(0x10);
			a_Pkt.WriteBEUInt8(reinterpret_cast<const cCreeper &>(a_Mob).IsBlowing() ? 1 : 255);
			a_Pkt.WriteBEUInt8(0x11);
			a_Pkt.WriteBEUInt8(reinterpret_cast<const cCreeper &>(a_Mob).IsCharged() ? 1 : 0);
			break;
		}  // case mtCreeper

		case mtEnderman:
		{
			auto & Enderman = reinterpret_cast<const cEnderman &>(a_Mob);
			a_Pkt.WriteBEUInt8(0x10);
			a_Pkt.WriteBEUInt8(static_cast<UInt8>(Enderman.GetCarriedBlock()));
			a_Pkt.WriteBEUInt8(0x11);
			a_Pkt.WriteBEUInt8(static_cast<UInt8>(Enderman.GetCarriedMeta()));
			a_Pkt.WriteBEUInt8(0x12);
			a_Pkt.WriteBEUInt8(Enderman.IsScreaming() ? 1 : 0);
			break;
		}  // case mtEnderman

		case mtGhast:
		{
			a_Pkt.WriteBEUInt8(0x10);
			a_Pkt.WriteBEUInt8(reinterpret_cast<const cGhast &>(a_Mob).IsCharging());
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
			if (Horse.IsBaby())
			{
				Flags |= 0x10;
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
			a_Pkt.WriteBEUInt8(static_cast<Byte>(Horse.GetHorseType()));
			a_Pkt.WriteBEUInt8(0x54);  // Int at index 20
			int Appearance = 0;
			Appearance = Horse.GetHorseColor();
			Appearance |= Horse.GetHorseStyle() << 8;
			a_Pkt.WriteBEInt32(Appearance);
			a_Pkt.WriteBEUInt8(0x56);  // Int at index 22
			a_Pkt.WriteBEInt32(Horse.GetHorseArmour());
			break;
		}  // case mtHorse

		case mtMagmaCube:
		{
			a_Pkt.WriteBEUInt8(0x10);
			a_Pkt.WriteBEUInt8(static_cast<Byte>(reinterpret_cast<const cMagmaCube &>(a_Mob).GetSize()));
			break;
		}  // case mtMagmaCube

		case mtPig:
		{
			a_Pkt.WriteBEUInt8(0x10);
			a_Pkt.WriteBEUInt8(reinterpret_cast<const cPig &>(a_Mob).IsSaddled() ? 1 : 0);
			break;
		}  // case mtPig

		case mtSheep:
		{
			a_Pkt.WriteBEUInt8(0x10);
			auto & Sheep = reinterpret_cast<const cSheep &>(a_Mob);
			Byte SheepMetadata = static_cast<Byte>(Sheep.GetFurColor() & 0x0f);
			if (Sheep.IsSheared())
			{
				SheepMetadata |= 0x10;
			}
			a_Pkt.WriteBEUInt8(SheepMetadata);
			break;
		}  // case mtSheep

		case mtSkeleton:
		{
			a_Pkt.WriteBEUInt8(0x0d);
			a_Pkt.WriteBEUInt8(reinterpret_cast<const cSkeleton &>(a_Mob).IsWither() ? 1 : 0);
			break;
		}  // case mtSkeleton

		case mtSlime:
		{
			a_Pkt.WriteBEUInt8(0x10);
			a_Pkt.WriteBEUInt8(static_cast<Byte>(reinterpret_cast<const cSlime &>(a_Mob).GetSize()));
			break;
		}  // case mtSlime

		case mtVillager:
		{
			a_Pkt.WriteBEUInt8(0x50);
			a_Pkt.WriteBEInt32(reinterpret_cast<const cVillager &>(a_Mob).GetVilType());
			break;
		}  // case mtVillager

		case mtWitch:
		{
			a_Pkt.WriteBEUInt8(0x15);
			a_Pkt.WriteBEUInt8(reinterpret_cast<const cWitch &>(a_Mob).IsAngry() ? 1 : 0);
			break;
		}  // case mtWitch

		case mtWither:
		{
			a_Pkt.WriteBEUInt8(0x54);  // Int at index 20
			a_Pkt.WriteBEInt32(static_cast<int>(reinterpret_cast<const cWither &>(a_Mob).GetWitherInvulnerableTicks()));
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
			a_Pkt.WriteBEUInt8(static_cast<Byte>(Wolf.GetCollarColor()));
			break;
		}  // case mtWolf

		case mtZombie:
		{
			auto & Zombie = reinterpret_cast<const cZombie &>(a_Mob);
			a_Pkt.WriteBEUInt8(0x0c);
			a_Pkt.WriteBEUInt8(Zombie.IsBaby() ? 1 : 0);
			a_Pkt.WriteBEUInt8(0x0d);
			a_Pkt.WriteBEUInt8(Zombie.IsVillagerZombie() ? 1 : 0);
			a_Pkt.WriteBEUInt8(0x0e);
			a_Pkt.WriteBEUInt8(Zombie.IsConverting() ? 1 : 0);
			break;
		}  // case mtZombie

		default:
		{
			// No data to send for this mob
			break;
		}
	}  // switch (a_Mob.GetType())
}





void cProtocol172::WriteEntityProperties(cPacketizer & a_Pkt, const cEntity & a_Entity)
{
	if (!a_Entity.IsMob())
	{
		// No properties for anything else than mobs
		a_Pkt.WriteBEInt32(0);
		return;
	}

	// auto & Mob = reinterpret_cast<const cMonster &>(a_Entity);

	// TODO: Send properties and modifiers based on the mob type

	a_Pkt.WriteBEInt32(0);  // NumProperties
}





////////////////////////////////////////////////////////////////////////////////
// cProtocol176:

cProtocol176::cProtocol176(cClientHandle * a_Client, const AString &a_ServerAddress, UInt16 a_ServerPort, UInt32 a_State) :
	super(a_Client, a_ServerAddress, a_ServerPort, a_State)
{
}





void cProtocol176::SendPlayerSpawn(const cPlayer & a_Player)
{
	// Called to spawn another player for the client
	cPacketizer Pkt(*this, 0x0c);  // Spawn Player packet
	Pkt.WriteVarInt32(a_Player.GetUniqueID());
	Pkt.WriteString(cMojangAPI::MakeUUIDDashed(a_Player.GetClientHandle()->GetUUID()));
	if (a_Player.HasCustomName())
	{
		Pkt.WriteString(a_Player.GetCustomName());
	}
	else
	{
		Pkt.WriteString(a_Player.GetName());
	}

	const Json::Value & Properties = a_Player.GetClientHandle()->GetProperties();
	Pkt.WriteVarInt32(Properties.size());

	for (auto & Node : Properties)
	{
		Pkt.WriteString(Node.get("name", "").asString());
		Pkt.WriteString(Node.get("value", "").asString());
		Pkt.WriteString(Node.get("signature", "").asString());
	}

	Pkt.WriteFPInt(a_Player.GetPosX());
	Pkt.WriteFPInt(a_Player.GetPosY());
	Pkt.WriteFPInt(a_Player.GetPosZ());
	Pkt.WriteByteAngle(a_Player.GetYaw());
	Pkt.WriteByteAngle(a_Player.GetPitch());
	short ItemType = a_Player.GetEquippedItem().IsEmpty() ? 0 : a_Player.GetEquippedItem().m_ItemType;
	Pkt.WriteBEInt16(ItemType);
	Pkt.WriteBEUInt8((3 << 5) | 6);  // Metadata: float + index 6
	Pkt.WriteBEFloat(static_cast<float>(a_Player.GetHealth()));
	Pkt.WriteBEUInt8(0x7f);  // Metadata: end
}





void cProtocol176::HandlePacketStatusRequest(cByteBuffer & a_ByteBuffer)
{
	cServer * Server = cRoot::Get()->GetServer();
	AString Motd = Server->GetDescription();
	int NumPlayers = Server->GetNumPlayers();
	int MaxPlayers = Server->GetMaxPlayers();
	AString Favicon = Server->GetFaviconData();
	cRoot::Get()->GetPluginManager()->CallHookServerPing(*m_Client, Motd, NumPlayers, MaxPlayers, Favicon);

	// Version:
	Json::Value Version;
	Version["name"] = "Cuberite 1.7.6";
	Version["protocol"] = 5;

	// Players:
	Json::Value Players;
	Players["online"] = NumPlayers;
	Players["max"] = MaxPlayers;
	// TODO: Add "sample"

	// Description:
	Json::Value Description;
	Description["text"] = Motd.c_str();

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




