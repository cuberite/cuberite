
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
#include "../BlockEntities/FlowerPotEntity.h"
#include "Bindings/PluginManager.h"





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
	m_OutPacketBuffer(64 KiB),
	m_OutPacketLenBuffer(20),  // 20 bytes is more than enough for one VarInt
	m_IsEncrypted(false),
	m_LastSentDimension(dimNotSet)
{
	// Create the comm log file, if so requested:
	if (g_ShouldLogCommIn || g_ShouldLogCommOut)
	{
		static int sCounter = 0;
		cFile::CreateFolder("CommLogs");
		AString FileName = Printf("CommLogs/%x_%d__%s.log", (unsigned)time(NULL), sCounter++, a_Client->GetIPString().c_str());
		m_CommLogFile.Open(FileName, cFile::fmWrite);
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
			m_Decryptor.ProcessData(Decrypted, (Byte *)a_Data, NumBytes);
			AddReceivedData((const char *)Decrypted, NumBytes);
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
	Pkt.WriteInt(a_Entity.GetUniqueID());
	Pkt.WriteInt((a_Vehicle != NULL) ? a_Vehicle->GetUniqueID() : 0);
	Pkt.WriteBool(false);
}





void cProtocol180::SendBlockAction(int a_BlockX, int a_BlockY, int a_BlockZ, char a_Byte1, char a_Byte2, BLOCKTYPE a_BlockType)
{
	ASSERT(m_State == 3);  // In game mode?
	
	cPacketizer Pkt(*this, 0x24);  // Block Action packet
	Pkt.WritePosition(a_BlockX, a_BlockY, a_BlockZ);
	Pkt.WriteByte(a_Byte1);
	Pkt.WriteByte(a_Byte2);
	Pkt.WriteVarInt(a_BlockType);
}





void cProtocol180::SendBlockBreakAnim(int a_EntityID, int a_BlockX, int a_BlockY, int a_BlockZ, char a_Stage)
{
	ASSERT(m_State == 3);  // In game mode?
	
	cPacketizer Pkt(*this, 0x25);  // Block Break Animation packet
	Pkt.WriteVarInt(a_EntityID);
	Pkt.WritePosition(a_BlockX, a_BlockY, a_BlockZ);
	Pkt.WriteChar(a_Stage);
}





void cProtocol180::SendBlockChange(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, 0x23);  // Block Change packet
	Pkt.WritePosition(a_BlockX, a_BlockY, a_BlockZ);
	Pkt.WriteVarInt(((UInt32)a_BlockType << 4) | ((UInt32)a_BlockMeta & 15));
}





void cProtocol180::SendBlockChanges(int a_ChunkX, int a_ChunkZ, const sSetBlockVector & a_Changes)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, 0x22);  // Multi Block Change packet
	Pkt.WriteInt(a_ChunkX);
	Pkt.WriteInt(a_ChunkZ);
	Pkt.WriteVarInt((UInt32)a_Changes.size());
	for (sSetBlockVector::const_iterator itr = a_Changes.begin(), end = a_Changes.end(); itr != end; ++itr)
	{
		short Coords = (short) (itr->y | (itr->z << 8) | (itr->x << 12));
		Pkt.WriteShort(Coords);
		Pkt.WriteVarInt((itr->BlockType & 0xFFF) << 4 | (itr->BlockMeta & 0xF));
	}  // for itr - a_Changes[]
}





void cProtocol180::SendChat(const AString & a_Message)
{
	ASSERT(m_State == 3);  // In game mode?
	
	cPacketizer Pkt(*this, 0x02);  // Chat Message packet
	Pkt.WriteString(Printf("{\"text\":\"%s\"}", EscapeString(a_Message).c_str()));
	Pkt.WriteChar(0);
}





void cProtocol180::SendChat(const cCompositeChat & a_Message)
{
	ASSERT(m_State == 3);  // In game mode?

	cWorld * World = m_Client->GetPlayer()->GetWorld();
	bool ShouldUseChatPrefixes = (World == NULL) ? false : World->ShouldUseChatPrefixes();

	// Send the message to the client:
	cPacketizer Pkt(*this, 0x02);
	Pkt.WriteString(a_Message.CreateJsonString(ShouldUseChatPrefixes));
	Pkt.WriteChar(0);
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
	Pkt.WriteVarInt(a_Entity.GetUniqueID());
	Pkt.WriteVarInt(a_Player.GetUniqueID());
}





void cProtocol180::SendDestroyEntity(const cEntity & a_Entity)
{
	ASSERT(m_State == 3);  // In game mode?
	
	cPacketizer Pkt(*this, 0x13);  // Destroy Entities packet
	Pkt.WriteVarInt(1);
	Pkt.WriteVarInt(a_Entity.GetUniqueID());
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
	Pkt.WritePosition(a_BlockX, a_BlockY, a_BlockZ);
}





void cProtocol180::SendEntityEffect(const cEntity & a_Entity, int a_EffectID, int a_Amplifier, short a_Duration)
{
	ASSERT(m_State == 3);  // In game mode?
	
	cPacketizer Pkt(*this, 0x1D);  // Entity Effect packet
	Pkt.WriteVarInt(a_Entity.GetUniqueID());
	Pkt.WriteByte(a_EffectID);
	Pkt.WriteByte(a_Amplifier);
	Pkt.WriteVarInt((UInt32)a_Duration);
	Pkt.WriteBool(false);  // Hide particles
}





void cProtocol180::SendEntityEquipment(const cEntity & a_Entity, short a_SlotNum, const cItem & a_Item)
{
	ASSERT(m_State == 3);  // In game mode?
	
	cPacketizer Pkt(*this, 0x04);  // Entity Equipment packet
	Pkt.WriteVarInt((UInt32)a_Entity.GetUniqueID());
	Pkt.WriteShort(a_SlotNum);
	Pkt.WriteItem(a_Item);
}





void cProtocol180::SendEntityHeadLook(const cEntity & a_Entity)
{
	ASSERT(m_State == 3);  // In game mode?
	
	cPacketizer Pkt(*this, 0x19);  // Entity Head Look packet
	Pkt.WriteVarInt((UInt32)a_Entity.GetUniqueID());
	Pkt.WriteByteAngle(a_Entity.GetHeadYaw());
}





void cProtocol180::SendEntityLook(const cEntity & a_Entity)
{
	ASSERT(m_State == 3);  // In game mode?
	
	cPacketizer Pkt(*this, 0x16);  // Entity Look packet
	Pkt.WriteVarInt(a_Entity.GetUniqueID());
	Pkt.WriteByteAngle(a_Entity.GetYaw());
	Pkt.WriteByteAngle(a_Entity.GetPitch());
	Pkt.WriteBool(true);  // TODO: IsOnGround() on entities
}





void cProtocol180::SendEntityMetadata(const cEntity & a_Entity)
{
	ASSERT(m_State == 3);  // In game mode?
	
	cPacketizer Pkt(*this, 0x1c);  // Entity Metadata packet
	Pkt.WriteVarInt(a_Entity.GetUniqueID());
	Pkt.WriteEntityMetadata(a_Entity);
	Pkt.WriteByte(0x7f);  // The termination byte
}





void cProtocol180::SendEntityProperties(const cEntity & a_Entity)
{
	ASSERT(m_State == 3);  // In game mode?
	
	cPacketizer Pkt(*this, 0x20);  // Entity Properties packet
	Pkt.WriteVarInt(a_Entity.GetUniqueID());
	Pkt.WriteEntityProperties(a_Entity);
}





void cProtocol180::SendEntityRelMove(const cEntity & a_Entity, char a_RelX, char a_RelY, char a_RelZ)
{
	ASSERT(m_State == 3);  // In game mode?
	
	cPacketizer Pkt(*this, 0x15);  // Entity Relative Move packet
	Pkt.WriteVarInt(a_Entity.GetUniqueID());
	Pkt.WriteByte(a_RelX);
	Pkt.WriteByte(a_RelY);
	Pkt.WriteByte(a_RelZ);
	Pkt.WriteBool(true);  // TODO: IsOnGround() on entities
}





void cProtocol180::SendEntityRelMoveLook(const cEntity & a_Entity, char a_RelX, char a_RelY, char a_RelZ)
{
	ASSERT(m_State == 3);  // In game mode?
	
	cPacketizer Pkt(*this, 0x17);  // Entity Look And Relative Move packet
	Pkt.WriteVarInt(a_Entity.GetUniqueID());
	Pkt.WriteByte(a_RelX);
	Pkt.WriteByte(a_RelY);
	Pkt.WriteByte(a_RelZ);
	Pkt.WriteByteAngle(a_Entity.GetYaw());
	Pkt.WriteByteAngle(a_Entity.GetPitch());
	Pkt.WriteBool(true);  // TODO: IsOnGround() on entities
}





void cProtocol180::SendEntityStatus(const cEntity & a_Entity, char a_Status)
{
	ASSERT(m_State == 3);  // In game mode?
	
	cPacketizer Pkt(*this, 0x1a);  // Entity Status packet
	Pkt.WriteInt(a_Entity.GetUniqueID());
	Pkt.WriteChar(a_Status);
}





void cProtocol180::SendEntityVelocity(const cEntity & a_Entity)
{
	ASSERT(m_State == 3);  // In game mode?
	
	cPacketizer Pkt(*this, 0x12);  // Entity Velocity packet
	Pkt.WriteVarInt(a_Entity.GetUniqueID());
	// 400 = 8000 / 20 ... Conversion from our speed in m/s to 8000 m/tick
	Pkt.WriteShort((short)(a_Entity.GetSpeedX() * 400));
	Pkt.WriteShort((short)(a_Entity.GetSpeedY() * 400));
	Pkt.WriteShort((short)(a_Entity.GetSpeedZ() * 400));
}





void cProtocol180::SendExplosion(double a_BlockX, double a_BlockY, double a_BlockZ, float a_Radius, const cVector3iArray & a_BlocksAffected, const Vector3d & a_PlayerMotion)
{
	ASSERT(m_State == 3);  // In game mode?
	
	cPacketizer Pkt(*this, 0x27);  // Explosion packet
	Pkt.WriteFloat((float)a_BlockX);
	Pkt.WriteFloat((float)a_BlockY);
	Pkt.WriteFloat((float)a_BlockZ);
	Pkt.WriteFloat((float)a_Radius);
	Pkt.WriteInt((int)a_BlocksAffected.size());
	for (cVector3iArray::const_iterator itr = a_BlocksAffected.begin(), end = a_BlocksAffected.end(); itr != end; ++itr)
	{
		Pkt.WriteChar((char)itr->x);
		Pkt.WriteChar((char)itr->y);
		Pkt.WriteChar((char)itr->z);
	}  // for itr - a_BlockAffected[]
	Pkt.WriteFloat((float)a_PlayerMotion.x);
	Pkt.WriteFloat((float)a_PlayerMotion.y);
	Pkt.WriteFloat((float)a_PlayerMotion.z);
}





void cProtocol180::SendGameMode(eGameMode a_GameMode)
{
	ASSERT(m_State == 3);  // In game mode?
	
	cPacketizer Pkt(*this, 0x2b);  // Change Game State packet
	Pkt.WriteByte(3);  // Reason: Change game mode
	Pkt.WriteFloat((float)a_GameMode);
}





void cProtocol180::SendHealth(void)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, 0x06);  // Update Health packet
	cPlayer * Player = m_Client->GetPlayer();
	Pkt.WriteFloat((float)Player->GetHealth());
	Pkt.WriteVarInt((UInt32)Player->GetFoodLevel());
	Pkt.WriteFloat((float)Player->GetFoodSaturationLevel());
}





void cProtocol180::SendInventorySlot(char a_WindowID, short a_SlotNum, const cItem & a_Item)
{
	ASSERT(m_State == 3);  // In game mode?
	
	cPacketizer Pkt(*this, 0x2f);  // Set Slot packet
	Pkt.WriteChar(a_WindowID);
	Pkt.WriteShort(a_SlotNum);
	Pkt.WriteItem(a_Item);
}





void cProtocol180::SendKeepAlive(int a_PingID)
{
	// Drop the packet if the protocol is not in the Game state yet (caused a client crash):
	if (m_State != 3)
	{
		LOGWARNING("Trying to send a KeepAlive packet to a player who's not yet fully logged in (%d). The protocol class prevented the packet.", m_State);
		return;
	}
	
	cPacketizer Pkt(*this, 0x00);  // Keep Alive packet
	Pkt.WriteVarInt(a_PingID);
}





void cProtocol180::SendLogin(const cPlayer & a_Player, const cWorld & a_World)
{
	// Send the Join Game packet:
	{
		cServer * Server = cRoot::Get()->GetServer();
		cPacketizer Pkt(*this, 0x01);  // Join Game packet
		Pkt.WriteInt(a_Player.GetUniqueID());
		Pkt.WriteByte((Byte)a_Player.GetEffectiveGameMode() | (Server->IsHardcore() ? 0x08 : 0));  // Hardcore flag bit 4
		Pkt.WriteChar((char)a_World.GetDimension());
		Pkt.WriteByte(2);  // TODO: Difficulty (set to Normal)
		Pkt.WriteByte(Server->GetMaxPlayers());
		Pkt.WriteString("default");  // Level type - wtf?
		Pkt.WriteBool(false);  // Reduced Debug Info - wtf?
	}
	m_LastSentDimension = a_World.GetDimension();
	
	// Send the spawn position:
	{
		cPacketizer Pkt(*this, 0x05);  // Spawn Position packet
		Pkt.WritePosition((int)a_World.GetSpawnX(), (int)a_World.GetSpawnY(), (int)a_World.GetSpawnZ());
	}

	// Send the server difficulty:
	{
		cPacketizer Pkt(*this, 0x41);
		Pkt.WriteChar(1);
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
		Pkt.WriteVarInt(256);
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

	switch (a_Painting.GetDirection())
	{
		case 0: PosZ += 1; break;
		case 1: PosX -= 1; break;
		case 2: PosZ -= 1; break;
		case 3: PosX += 1; break;
	}

	cPacketizer Pkt(*this, 0x10);  // Spawn Painting packet
	Pkt.WriteVarInt(a_Painting.GetUniqueID());
	Pkt.WriteString(a_Painting.GetName().c_str());
	Pkt.WritePosition((int)PosX, (int)PosY, (int)PosZ);
	Pkt.WriteChar(a_Painting.GetDirection());
}





void cProtocol180::SendMapColumn(int a_ID, int a_X, int a_Y, const Byte * a_Colors, unsigned int a_Length, unsigned int m_Scale)
{
	ASSERT(m_State == 3);  // In game mode?
	
	cPacketizer Pkt(*this, 0x34);
	Pkt.WriteVarInt(a_ID);
	Pkt.WriteByte(m_Scale);

	Pkt.WriteVarInt(0);
	Pkt.WriteByte(1);
	Pkt.WriteByte(a_Length);
	Pkt.WriteByte(a_X);
	Pkt.WriteByte(a_Y);

	Pkt.WriteVarInt(a_Length);
	for (unsigned int i = 0; i < a_Length; ++i)
	{
		Pkt.WriteByte(a_Colors[i]);
	}
}





void cProtocol180::SendMapDecorators(int a_ID, const cMapDecoratorList & a_Decorators, unsigned int m_Scale)
{
	ASSERT(m_State == 3);  // In game mode?
	
	cPacketizer Pkt(*this, 0x34);
	Pkt.WriteVarInt(a_ID);
	Pkt.WriteByte(m_Scale);
	Pkt.WriteVarInt(a_Decorators.size());

	for (cMapDecoratorList::const_iterator it = a_Decorators.begin(); it != a_Decorators.end(); ++it)
	{
		Pkt.WriteByte((it->GetType() << 4) | (it->GetRot() & 0xf));
		Pkt.WriteByte(it->GetPixelX());
		Pkt.WriteByte(it->GetPixelZ());
	}

	Pkt.WriteByte(0);
}





void cProtocol180::SendMapInfo(int a_ID, unsigned int a_Scale)
{
	UNUSED(a_ID);
	UNUSED(a_Scale);

	// This packet was removed in 1.8
}





void cProtocol180::SendPickupSpawn(const cPickup & a_Pickup)
{
	ASSERT(m_State == 3);  // In game mode?
	
	{
		cPacketizer Pkt(*this, 0x0e);  // Spawn Object packet
		Pkt.WriteVarInt(a_Pickup.GetUniqueID());
		Pkt.WriteByte(2);  // Type = Pickup
		Pkt.WriteFPInt(a_Pickup.GetPosX());
		Pkt.WriteFPInt(a_Pickup.GetPosY());
		Pkt.WriteFPInt(a_Pickup.GetPosZ());
		Pkt.WriteByteAngle(a_Pickup.GetYaw());
		Pkt.WriteByteAngle(a_Pickup.GetPitch());
		Pkt.WriteInt(0);  // No object data
	}

	{
		cPacketizer Pkt(*this, 0x1c);  // Entity Metadata packet
		Pkt.WriteVarInt(a_Pickup.GetUniqueID());
		Pkt.WriteByte((0x05 << 5) | 10);  // Slot type + index 10
		Pkt.WriteItem(a_Pickup.GetItem());
		Pkt.WriteByte(0x7f);  // End of metadata
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
	Pkt.WriteByte(Flags);
	Pkt.WriteFloat((float)(0.05 * Player->GetFlyingMaxSpeed()));
	Pkt.WriteFloat((float)(0.1 * Player->GetNormalMaxSpeed()));
}





void cProtocol180::SendEntityAnimation(const cEntity & a_Entity, char a_Animation)
{
	ASSERT(m_State == 3);  // In game mode?
	
	cPacketizer Pkt(*this, 0x0b);  // Animation packet
	Pkt.WriteVarInt(a_Entity.GetUniqueID());
	Pkt.WriteChar(a_Animation);
}





void cProtocol180::SendParticleEffect(const AString & a_ParticleName, float a_SrcX, float a_SrcY, float a_SrcZ, float a_OffsetX, float a_OffsetY, float a_OffsetZ, float a_ParticleData, int a_ParticleAmount)
{
	ASSERT(m_State == 3);  // In game mode?
	int ParticleID = GetParticleID(a_ParticleName);

	cPacketizer Pkt(*this, 0x2A);
	Pkt.WriteInt(ParticleID);
	Pkt.WriteBool(false);
	Pkt.WriteFloat(a_SrcX);
	Pkt.WriteFloat(a_SrcY);
	Pkt.WriteFloat(a_SrcZ);
	Pkt.WriteFloat(a_OffsetX);
	Pkt.WriteFloat(a_OffsetY);
	Pkt.WriteFloat(a_OffsetZ);
	Pkt.WriteFloat(a_ParticleData);
	Pkt.WriteInt(a_ParticleAmount);
}





void cProtocol180::SendPlayerListAddPlayer(const cPlayer & a_Player)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, 0x38);  // Playerlist Item packet
	Pkt.WriteVarInt(0);
	Pkt.WriteVarInt(1);
	Pkt.WriteUUID(a_Player.GetUUID());
	Pkt.WriteString(a_Player.GetName());

	const Json::Value & Properties = a_Player.GetClientHandle()->GetProperties();
	Pkt.WriteVarInt(Properties.size());
	for (Json::Value::iterator itr = Properties.begin(), end = Properties.end(); itr != end; ++itr)
	{
		Pkt.WriteString(((Json::Value)*itr).get("name", "").asString());
		Pkt.WriteString(((Json::Value)*itr).get("value", "").asString());
		AString Signature = ((Json::Value)*itr).get("signature", "").asString();
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

	Pkt.WriteVarInt((UInt32)a_Player.GetGameMode());
	Pkt.WriteVarInt((UInt32)a_Player.GetClientHandle()->GetPing());

	AString CustomName = a_Player.GetPlayerListName();
	if (CustomName != a_Player.GetName())
	{
		Pkt.WriteBool(true);
		Pkt.WriteString(Printf("{\"text\":\"%s\"}", CustomName.c_str()));
	}
	else
	{
		Pkt.WriteBool(false);
	}
}





void cProtocol180::SendPlayerListRemovePlayer(const cPlayer & a_Player)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, 0x38);  // Playerlist Item packet
	Pkt.WriteVarInt(4);
	Pkt.WriteVarInt(1);
	Pkt.WriteUUID(a_Player.GetUUID());
}





void cProtocol180::SendPlayerListUpdateGameMode(const cPlayer & a_Player)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, 0x38);  // Playerlist Item packet
	Pkt.WriteVarInt(1);
	Pkt.WriteVarInt(1);
	Pkt.WriteUUID(a_Player.GetUUID());
	Pkt.WriteVarInt((UInt32)a_Player.GetGameMode());
}





void cProtocol180::SendPlayerListUpdatePing(const cPlayer & a_Player)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, 0x38);  // Playerlist Item packet
	Pkt.WriteVarInt(2);
	Pkt.WriteVarInt(1);
	Pkt.WriteUUID(a_Player.GetUUID());
	Pkt.WriteVarInt((UInt32)a_Player.GetClientHandle()->GetPing());
}





void cProtocol180::SendPlayerListUpdateDisplayName(const cPlayer & a_Player, const AString & a_OldListName)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, 0x38);  // Playerlist Item packet
	Pkt.WriteVarInt(3);
	Pkt.WriteVarInt(1);
	Pkt.WriteUUID(a_Player.GetUUID());

	AString CustomName = a_Player.GetPlayerListName();
	if (CustomName == a_Player.GetName())
	{
		Pkt.WriteBool(false);
	}
	else
	{
		Pkt.WriteBool(true);
		Pkt.WriteString(Printf("{\"text\":\"%s\"}", CustomName.c_str()));
	}
}





void cProtocol180::SendPlayerMaxSpeed(void)
{
	ASSERT(m_State == 3);  // In game mode?
	
	cPacketizer Pkt(*this, 0x20);  // Entity Properties
	cPlayer * Player = m_Client->GetPlayer();
	Pkt.WriteVarInt(Player->GetUniqueID());
	Pkt.WriteInt(1);  // Count
	Pkt.WriteString("generic.movementSpeed");
	// The default game speed is 0.1, multiply that value by the relative speed:
	Pkt.WriteDouble(0.1 * Player->GetNormalMaxSpeed());
	if (Player->IsSprinting())
	{
		Pkt.WriteVarInt(1);  // Modifier count
		Pkt.WriteInt64(0x662a6b8dda3e4c1c);
		Pkt.WriteInt64(0x881396ea6097278d);  // UUID of the modifier
		Pkt.WriteDouble(Player->GetSprintingMaxSpeed() - Player->GetNormalMaxSpeed());
		Pkt.WriteByte(2);
	}
	else
	{
		Pkt.WriteVarInt(0);  // Modifier count
	}
}





void cProtocol180::SendPlayerMoveLook(void)
{
	ASSERT(m_State == 3);  // In game mode?
	
	cPacketizer Pkt(*this, 0x08);  // Player Position And Look packet
	cPlayer * Player = m_Client->GetPlayer();
	Pkt.WriteDouble(Player->GetPosX());
	
	// The "+ 0.001" is there because otherwise the player falls through the block they were standing on.
	Pkt.WriteDouble(Player->GetStance() + 0.001);
	
	Pkt.WriteDouble(Player->GetPosZ());
	Pkt.WriteFloat((float)Player->GetYaw());
	Pkt.WriteFloat((float)Player->GetPitch());
	Pkt.WriteByte(0);
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
	Pkt.WriteVarInt(a_Player.GetUniqueID());
	Pkt.WriteUUID(cMojangAPI::MakeUUIDShort(a_Player.GetUUID()));
	Pkt.WriteFPInt(a_Player.GetPosX());
	Pkt.WriteFPInt(a_Player.GetPosY());
	Pkt.WriteFPInt(a_Player.GetPosZ());
	Pkt.WriteByteAngle(a_Player.GetYaw());
	Pkt.WriteByteAngle(a_Player.GetPitch());
	short ItemType = a_Player.GetEquippedItem().IsEmpty() ? 0 : a_Player.GetEquippedItem().m_ItemType;
	Pkt.WriteShort(ItemType);
	Pkt.WriteByte((3 << 5) | 6);  // Metadata: float + index 6
	Pkt.WriteFloat((float)a_Player.GetHealth());
	Pkt.WriteByte((4 << 5 | (2 & 0x1F)) & 0xFF);
	Pkt.WriteString(a_Player.GetName());
	Pkt.WriteByte(0x7f);  // Metadata: end
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
	Pkt.WriteVarInt(a_Entity.GetUniqueID());
	Pkt.WriteByte(a_EffectID);
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
	Pkt.WriteInt((int)a_Dimension);
	Pkt.WriteByte(2);  // TODO: Difficulty (set to Normal)
	Pkt.WriteByte((Byte)Player->GetEffectiveGameMode());
	Pkt.WriteString("default");
	m_LastSentDimension = a_Dimension;
}





void cProtocol180::SendExperience(void)
{
	ASSERT(m_State == 3);  // In game mode?
	
	cPacketizer Pkt(*this, 0x1f);  // Experience Packet
	cPlayer * Player = m_Client->GetPlayer();
	Pkt.WriteFloat(Player->GetXpPercentage());
	Pkt.WriteVarInt((UInt32)Player->GetXpLevel());
	Pkt.WriteVarInt((UInt32)Player->GetCurrentXp());
}





void cProtocol180::SendExperienceOrb(const cExpOrb & a_ExpOrb)
{
	ASSERT(m_State == 3);  // In game mode?
	
	cPacketizer Pkt(*this, 0x11);
	Pkt.WriteVarInt(a_ExpOrb.GetUniqueID());
	Pkt.WriteFPInt(a_ExpOrb.GetPosX());
	Pkt.WriteFPInt(a_ExpOrb.GetPosY());
	Pkt.WriteFPInt(a_ExpOrb.GetPosZ());
	Pkt.WriteShort(a_ExpOrb.GetReward());
}





void cProtocol180::SendScoreboardObjective(const AString & a_Name, const AString & a_DisplayName, Byte a_Mode)
{
	ASSERT(m_State == 3);  // In game mode?
	
	cPacketizer Pkt(*this, 0x3b);
	Pkt.WriteString(a_Name);
	Pkt.WriteByte(a_Mode);
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
	Pkt.WriteByte(a_Mode);
	Pkt.WriteString(a_Objective);

	if (a_Mode != 1)
	{
		Pkt.WriteVarInt((UInt32) a_Score);
	}
}





void cProtocol180::SendDisplayObjective(const AString & a_Objective, cScoreboard::eDisplaySlot a_Display)
{
	ASSERT(m_State == 3);  // In game mode?
	
	cPacketizer Pkt(*this, 0x3d);
	Pkt.WriteByte((int) a_Display);
	Pkt.WriteString(a_Objective);
}





void cProtocol180::SendSoundEffect(const AString & a_SoundName, double a_X, double a_Y, double a_Z, float a_Volume, float a_Pitch)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, 0x29);  // Sound Effect packet
	Pkt.WriteString(a_SoundName);
	Pkt.WriteInt((int)(a_X * 8.0));
	Pkt.WriteInt((int)(a_Y * 8.0));
	Pkt.WriteInt((int)(a_Z * 8.0));
	Pkt.WriteFloat(a_Volume);
	Pkt.WriteByte((Byte)(a_Pitch * 63));
}





void cProtocol180::SendSoundParticleEffect(int a_EffectID, int a_SrcX, int a_SrcY, int a_SrcZ, int a_Data)
{
	ASSERT(m_State == 3);  // In game mode?
	
	cPacketizer Pkt(*this, 0x28);  // Effect packet
	Pkt.WriteInt(a_EffectID);
	Pkt.WritePosition(a_SrcX, a_SrcY, a_SrcZ);
	Pkt.WriteInt(a_Data);
	Pkt.WriteBool(false);
}





void cProtocol180::SendSpawnFallingBlock(const cFallingBlock & a_FallingBlock)
{
	ASSERT(m_State == 3);  // In game mode?
	
	cPacketizer Pkt(*this, 0x0e);  // Spawn Object packet
	Pkt.WriteVarInt(a_FallingBlock.GetUniqueID());
	Pkt.WriteByte(70);  // Falling block
	Pkt.WriteFPInt(a_FallingBlock.GetPosX());
	Pkt.WriteFPInt(a_FallingBlock.GetPosY());
	Pkt.WriteFPInt(a_FallingBlock.GetPosZ());
	Pkt.WriteByteAngle(a_FallingBlock.GetYaw());
	Pkt.WriteByteAngle(a_FallingBlock.GetPitch());
	Pkt.WriteInt(((int)a_FallingBlock.GetBlockType()) | (((int)a_FallingBlock.GetBlockMeta()) << 12));
	Pkt.WriteShort((short)(a_FallingBlock.GetSpeedX() * 400));
	Pkt.WriteShort((short)(a_FallingBlock.GetSpeedY() * 400));
	Pkt.WriteShort((short)(a_FallingBlock.GetSpeedZ() * 400));
}





void cProtocol180::SendSpawnMob(const cMonster & a_Mob)
{
	ASSERT(m_State == 3);  // In game mode?
	
	cPacketizer Pkt(*this, 0x0f);  // Spawn Mob packet
	Pkt.WriteVarInt(a_Mob.GetUniqueID());
	Pkt.WriteByte((Byte)a_Mob.GetMobType());
	Pkt.WriteFPInt(a_Mob.GetPosX());
	Pkt.WriteFPInt(a_Mob.GetPosY());
	Pkt.WriteFPInt(a_Mob.GetPosZ());
	Pkt.WriteByteAngle(a_Mob.GetPitch());
	Pkt.WriteByteAngle(a_Mob.GetHeadYaw());
	Pkt.WriteByteAngle(a_Mob.GetYaw());
	Pkt.WriteShort((short)(a_Mob.GetSpeedX() * 400));
	Pkt.WriteShort((short)(a_Mob.GetSpeedY() * 400));
	Pkt.WriteShort((short)(a_Mob.GetSpeedZ() * 400));
	Pkt.WriteEntityMetadata(a_Mob);
	Pkt.WriteByte(0x7f);  // Metadata terminator
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
	Pkt.WriteVarInt(a_Entity.GetUniqueID());
	Pkt.WriteByte(a_ObjectType);
	Pkt.WriteFPInt(PosX);
	Pkt.WriteFPInt(a_Entity.GetPosY());
	Pkt.WriteFPInt(PosZ);
	Pkt.WriteByteAngle(a_Entity.GetPitch());
	Pkt.WriteByteAngle(Yaw);
	Pkt.WriteInt(a_ObjectData);
	if (a_ObjectData != 0)
	{
		Pkt.WriteShort((short)(a_Entity.GetSpeedX() * 400));
		Pkt.WriteShort((short)(a_Entity.GetSpeedY() * 400));
		Pkt.WriteShort((short)(a_Entity.GetSpeedZ() * 400));
	}
}





void cProtocol180::SendSpawnVehicle(const cEntity & a_Vehicle, char a_VehicleType, char a_VehicleSubType)
{
	ASSERT(m_State == 3);  // In game mode?
	
	cPacketizer Pkt(*this, 0xe);  // Spawn Object packet
	Pkt.WriteVarInt(a_Vehicle.GetUniqueID());
	Pkt.WriteByte(a_VehicleType);
	Pkt.WriteFPInt(a_Vehicle.GetPosX());
	Pkt.WriteFPInt(a_Vehicle.GetPosY());
	Pkt.WriteFPInt(a_Vehicle.GetPosZ());
	Pkt.WriteByteAngle(a_Vehicle.GetPitch());
	Pkt.WriteByteAngle(a_Vehicle.GetYaw());
	Pkt.WriteInt(a_VehicleSubType);
	if (a_VehicleSubType != 0)
	{
		Pkt.WriteShort((short)(a_Vehicle.GetSpeedX() * 400));
		Pkt.WriteShort((short)(a_Vehicle.GetSpeedY() * 400));
		Pkt.WriteShort((short)(a_Vehicle.GetSpeedZ() * 400));
	}
}





void cProtocol180::SendStatistics(const cStatManager & a_Manager)
{
	ASSERT(m_State == 3);  // In game mode?
	
	cPacketizer Pkt(*this, 0x37);
	Pkt.WriteVarInt(statCount);  // TODO 2014-05-11 xdot: Optimization: Send "dirty" statistics only

	for (size_t i = 0; i < (size_t)statCount; ++i)
	{
		StatValue Value = a_Manager.GetValue((eStatistic) i);
		const AString & StatName = cStatInfo::GetName((eStatistic) i);

		Pkt.WriteString(StatName);
		Pkt.WriteVarInt(Value);
	}
}





void cProtocol180::SendTabCompletionResults(const AStringVector & a_Results)
{
	ASSERT(m_State == 3);  // In game mode?
	
	cPacketizer Pkt(*this, 0x3a);  // Tab-Complete packet
	Pkt.WriteVarInt((int)a_Results.size());

	for (AStringVector::const_iterator itr = a_Results.begin(), end = a_Results.end(); itr != end; ++itr)
	{
		Pkt.WriteString(*itr);
	}
}





void cProtocol180::SendTeleportEntity(const cEntity & a_Entity)
{
	ASSERT(m_State == 3);  // In game mode?
	
	cPacketizer Pkt(*this, 0x18);
	Pkt.WriteVarInt(a_Entity.GetUniqueID());
	Pkt.WriteFPInt(a_Entity.GetPosX());
	Pkt.WriteFPInt(a_Entity.GetPosY());
	Pkt.WriteFPInt(a_Entity.GetPosZ());
	Pkt.WriteByteAngle(a_Entity.GetYaw());
	Pkt.WriteByteAngle(a_Entity.GetPitch());
	Pkt.WriteBool(true);  // TODO: IsOnGrond() on entities
}





void cProtocol180::SendThunderbolt(int a_BlockX, int a_BlockY, int a_BlockZ)
{
	ASSERT(m_State == 3);  // In game mode?
	
	cPacketizer Pkt(*this, 0x2c);  // Spawn Global Entity packet
	Pkt.WriteVarInt(0);  // EntityID = 0, always
	Pkt.WriteByte(1);  // Type = Thunderbolt
	Pkt.WriteFPInt(a_BlockX);
	Pkt.WriteFPInt(a_BlockY);
	Pkt.WriteFPInt(a_BlockZ);
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
	Pkt.WriteInt64(a_WorldAge);
	Pkt.WriteInt64(a_TimeOfDay);
}





void cProtocol180::SendUnloadChunk(int a_ChunkX, int a_ChunkZ)
{
	ASSERT(m_State == 3);  // In game mode?
	
	cPacketizer Pkt(*this, 0x21);  // Chunk Data packet
	Pkt.WriteInt(a_ChunkX);
	Pkt.WriteInt(a_ChunkZ);
	Pkt.WriteBool(true);
	Pkt.WriteShort(0);  // Primary bitmap
	Pkt.WriteVarInt(0);  // Data size
}




void cProtocol180::SendUpdateBlockEntity(cBlockEntity & a_BlockEntity)
{
	ASSERT(m_State == 3);  // In game mode?
	
	cPacketizer Pkt(*this, 0x35);  // Update tile entity packet
	Pkt.WritePosition(a_BlockEntity.GetPosX(), a_BlockEntity.GetPosY(), a_BlockEntity.GetPosZ());

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
	Pkt.WriteByte(Action);

	Pkt.WriteBlockEntity(a_BlockEntity);
}





void cProtocol180::SendUpdateSign(int a_BlockX, int a_BlockY, int a_BlockZ, const AString & a_Line1, const AString & a_Line2, const AString & a_Line3, const AString & a_Line4)
{
	ASSERT(m_State == 3);  // In game mode?
	
	cPacketizer Pkt(*this, 0x33);
	Pkt.WritePosition(a_BlockX, a_BlockY, a_BlockZ);
	Pkt.WriteString(Printf("{\"text\": \"%s\"}", a_Line1.c_str()));
	Pkt.WriteString(Printf("{\"text\": \"%s\"}", a_Line2.c_str()));
	Pkt.WriteString(Printf("{\"text\": \"%s\"}", a_Line3.c_str()));
	Pkt.WriteString(Printf("{\"text\": \"%s\"}", a_Line4.c_str()));
}





void cProtocol180::SendUseBed(const cEntity & a_Entity, int a_BlockX, int a_BlockY, int a_BlockZ)
{
	ASSERT(m_State == 3);  // In game mode?
	
	cPacketizer Pkt(*this, 0x0a);
	Pkt.WriteVarInt(a_Entity.GetUniqueID());
	Pkt.WritePosition(a_BlockX, a_BlockY, a_BlockZ);
}





void cProtocol180::SendWeather(eWeather a_Weather)
{
	ASSERT(m_State == 3);  // In game mode?
	
	{
		cPacketizer Pkt(*this, 0x2b);  // Change Game State packet
		Pkt.WriteByte((a_Weather == wSunny) ? 1 : 2);  // End rain / begin rain
		Pkt.WriteFloat(0);  // Unused for weather
	}

	// TODO: Fade effect, somehow
}





void cProtocol180::SendWholeInventory(const cWindow & a_Window)
{
	ASSERT(m_State == 3);  // In game mode?
	
	cPacketizer Pkt(*this, 0x30);  // Window Items packet
	Pkt.WriteChar(a_Window.GetWindowID());
	Pkt.WriteShort(a_Window.GetNumSlots());
	cItems Slots;
	a_Window.GetSlots(*(m_Client->GetPlayer()), Slots);
	for (cItems::const_iterator itr = Slots.begin(), end = Slots.end(); itr != end; ++itr)
	{
		Pkt.WriteItem(*itr);
	}  // for itr - Slots[]
}





void cProtocol180::SendWindowClose(const cWindow & a_Window)
{
	ASSERT(m_State == 3);  // In game mode?
	
	cPacketizer Pkt(*this, 0x2e);
	Pkt.WriteChar(a_Window.GetWindowID());
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
	Pkt.WriteChar(a_Window.GetWindowID());
	Pkt.WriteString(a_Window.GetWindowTypeName());
	Pkt.WriteString(Printf("{\"text\":\"%s\"}", a_Window.GetWindowTitle().c_str()));

	switch (a_Window.GetWindowType())
	{
		case cWindow::wtWorkbench:
		case cWindow::wtEnchantment:
		case cWindow::wtAnvil:
		{
			Pkt.WriteChar(0);
			break;
		}
		default:
		{
			Pkt.WriteChar(a_Window.GetNumNonInventorySlots());
			break;
		}
	}

	if (a_Window.GetWindowType() == cWindow::wtAnimalChest)
	{
		Pkt.WriteInt(0);  // TODO: The animal's EntityID
	}
}





void cProtocol180::SendWindowProperty(const cWindow & a_Window, int a_Property, int a_Value)
{
	ASSERT(m_State == 3);  // In game mode?
	
	cPacketizer Pkt(*this, 0x31);  // Window Property packet
	Pkt.WriteChar(a_Window.GetWindowID());
	Pkt.WriteShort(a_Property);
	Pkt.WriteShort(a_Value);
}





bool cProtocol180::CompressPacket(const AString & a_Packet, AString & a_CompressedData)
{
	// Compress the data:
	char CompressedData[MAX_COMPRESSED_PACKET_LEN];

	uLongf CompressedSize = compressBound(a_Packet.size());
	if (CompressedSize >= MAX_COMPRESSED_PACKET_LEN)
	{
		ASSERT(!"Too high packet size.");
		return false;
	}

	int Status = compress2((Bytef*)CompressedData, &CompressedSize, (const Bytef*)a_Packet.data(), a_Packet.size(), Z_DEFAULT_COMPRESSION);
	if (Status != Z_OK)
	{
		return false;
	}

	AString LengthData;
	cByteBuffer Buffer(20);
	Buffer.WriteVarInt((UInt32)a_Packet.size());
	Buffer.ReadAll(LengthData);
	Buffer.CommitRead();

	Buffer.WriteVarInt(CompressedSize + LengthData.size());
	Buffer.WriteVarInt(a_Packet.size());
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
		m_CommLogFile.Printf("Incoming data: %d (0x%x) bytes: \n%s\n",
			(unsigned)a_Size, (unsigned)a_Size, Hex.c_str()
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
		VERIFY(m_ReceivedData.ReadToByteBuffer(bb, (int)PacketLen));
		m_ReceivedData.CommitRead();

		// Compressed packets
		if (m_State == 3)
		{
			UInt32 CompressedSize;
			if (!bb.ReadVarInt(CompressedSize))
			{
				// Not enough data
				break;
			}
		}

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
			bb.ReadVarInt(PacketType);
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
				CreateHexDump(Out, Packet.data(), (int)Packet.size(), 24);
				LOGD("Packet contents:\n%s", Out.c_str());
			#endif  // _DEBUG
			
			// Put a message in the comm log:
			if (g_ShouldLogCommIn)
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
	Pkt.WriteInt64(Timestamp);
}





void cProtocol180::HandlePacketStatusRequest(cByteBuffer & a_ByteBuffer)
{
	cServer * Server = cRoot::Get()->GetServer();
	AString ServerDescription = Server->GetDescription();
	int NumPlayers = Server->GetNumPlayers();
	int MaxPlayers = Server->GetMaxPlayers();
	AString Favicon = Server->GetFaviconData();
	cRoot::Get()->GetPluginManager()->CallHookServerPing(*m_Client, ServerDescription, NumPlayers, MaxPlayers, Favicon);

	// Version:
	Json::Value Version;
	Version["name"] = "1.8";
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
	a_ByteBuffer.ReadVarInt(EncKeyLength);
	AString EncKey;
	if (!a_ByteBuffer.ReadString(EncKey, EncKeyLength))
	{
		return;
	}
	a_ByteBuffer.ReadVarInt(EncNonceLength);
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
	Int32 DecryptedNonce[MAX_ENC_LEN / sizeof(Int32)];
	int res = rsaDecryptor.Decrypt((const Byte *)EncNonce.data(), EncNonce.size(), (Byte *)DecryptedNonce, sizeof(DecryptedNonce));
	if (res != 4)
	{
		LOGD("Bad nonce length: got %d, exp %d", res, 4);
		m_Client->Kick("Hacked client");
		return;
	}
	if (ntohl(DecryptedNonce[0]) != (unsigned)(uintptr_t)this)
	{
		LOGD("Bad nonce value");
		m_Client->Kick("Hacked client");
		return;
	}
	
	// Decrypt the symmetric encryption key using privkey:
	Byte DecryptedKey[MAX_ENC_LEN];
	res = rsaDecryptor.Decrypt((const Byte *)EncKey.data(), EncKey.size(), DecryptedKey, sizeof(DecryptedKey));
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
	
	cServer * Server = cRoot::Get()->GetServer();
	// If auth is required, then send the encryption request:
	if (Server->ShouldAuthenticate())
	{
		cPacketizer Pkt(*this, 0x01);
		Pkt.WriteString(Server->GetServerID());
		const AString & PubKeyDer = Server->GetPublicKeyDER();
		Pkt.WriteVarInt((short)PubKeyDer.size());
		Pkt.WriteBuf(PubKeyDer.data(), PubKeyDer.size());
		Pkt.WriteVarInt(4);
		Pkt.WriteInt((int)(intptr_t)this);  // Using 'this' as the cryptographic nonce, so that we don't have to generate one each time :)
		m_Client->SetUsername(Username);
		return;
	}
	
	m_Client->HandleLogin(4, Username);
}





void cProtocol180::HandlePacketAnimation(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadBEInt, int,  EntityID);
	HANDLE_READ(a_ByteBuffer, ReadByte,  Byte, Animation);
	m_Client->HandleAnimation(Animation);
}





void cProtocol180::HandlePacketBlockDig(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadByte,  Byte, Status);

	int BlockX, BlockY, BlockZ;
	if (!a_ByteBuffer.ReadPosition(BlockX, BlockY, BlockZ))
	{
		return;
	}

	HANDLE_READ(a_ByteBuffer, ReadByte,  Byte, Face);
	m_Client->HandleLeftClick(BlockX, BlockY, BlockZ, static_cast<eBlockFace>(Face), Status);
}





void cProtocol180::HandlePacketBlockPlace(cByteBuffer & a_ByteBuffer)
{
	int BlockX, BlockY, BlockZ;
	if (!a_ByteBuffer.ReadPosition(BlockX, BlockY, BlockZ))
	{
		return;
	}

	HANDLE_READ(a_ByteBuffer, ReadByte,  Byte, Face);
	if (Face == 255)
	{
		Face = 0;
	}

	cItem Item;
	ReadItem(a_ByteBuffer, Item, 3);

	HANDLE_READ(a_ByteBuffer, ReadByte,  Byte, CursorX);
	HANDLE_READ(a_ByteBuffer, ReadByte,  Byte, CursorY);
	HANDLE_READ(a_ByteBuffer, ReadByte,  Byte, CursorZ);
	m_Client->HandleRightClick(BlockX, BlockY, BlockZ, static_cast<eBlockFace>(Face), CursorX, CursorY, CursorZ, m_Client->GetPlayer()->GetEquippedItem());
}





void cProtocol180::HandlePacketChatMessage(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadVarUTF8String, AString, Message);
	m_Client->HandleChat(Message);
}





void cProtocol180::HandlePacketClientSettings(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadVarUTF8String, AString, Locale);
	HANDLE_READ(a_ByteBuffer, ReadByte,          Byte,    ViewDistance);
	HANDLE_READ(a_ByteBuffer, ReadByte,          Byte,    ChatFlags);
	HANDLE_READ(a_ByteBuffer, ReadBool,          bool,    ChatColors);
	HANDLE_READ(a_ByteBuffer, ReadChar,          char,    SkinFlags);
	
	m_Client->SetLocale(Locale);
	// TODO: Handle other values
}





void cProtocol180::HandlePacketClientStatus(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadChar, char, ActionID);
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





void cProtocol180::HandlePacketCreativeInventoryAction(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadBEShort, short, SlotNum);
	cItem Item;
	if (!ReadItem(a_ByteBuffer, Item))
	{
		return;
	}
	m_Client->HandleCreativeInventory(SlotNum, Item);
}





void cProtocol180::HandlePacketEntityAction(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadVarInt, UInt32,  PlayerID);
	HANDLE_READ(a_ByteBuffer, ReadChar,   char,    Action);
	HANDLE_READ(a_ByteBuffer, ReadVarInt, UInt32,  JumpBoost);

	switch (Action)
	{
		case 0: m_Client->HandleEntityCrouch(PlayerID, true);     break;  // Crouch
		case 1: m_Client->HandleEntityCrouch(PlayerID, false);    break;  // Uncrouch
		case 2: m_Client->HandleEntityLeaveBed(PlayerID);         break;  // Leave Bed
		case 3: m_Client->HandleEntitySprinting(PlayerID, true);  break;  // Start sprinting
		case 4: m_Client->HandleEntitySprinting(PlayerID, false); break;  // Stop sprinting
	}
}





void cProtocol180::HandlePacketKeepAlive(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadVarInt, UInt32, KeepAliveID);
	m_Client->HandleKeepAlive((int)KeepAliveID);
}





void cProtocol180::HandlePacketPlayer(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadBool, bool, IsOnGround);
	// TODO: m_Client->HandlePlayerOnGround(IsOnGround);
}





void cProtocol180::HandlePacketPlayerAbilities(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadByte,    Byte,  Flags);
	HANDLE_READ(a_ByteBuffer, ReadBEFloat, float, FlyingSpeed);
	HANDLE_READ(a_ByteBuffer, ReadBEFloat, float, WalkingSpeed);

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





void cProtocol180::HandlePacketPlayerLook(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadBEFloat, float, Yaw);
	HANDLE_READ(a_ByteBuffer, ReadBEFloat, float, Pitch);
	HANDLE_READ(a_ByteBuffer, ReadBool,    bool,  IsOnGround);
	m_Client->HandlePlayerLook(Yaw, Pitch, IsOnGround);
}





void cProtocol180::HandlePacketPlayerPos(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadBEDouble, double, PosX);
	HANDLE_READ(a_ByteBuffer, ReadBEDouble, double, PosY);
	HANDLE_READ(a_ByteBuffer, ReadBEDouble, double, PosZ);
	HANDLE_READ(a_ByteBuffer, ReadBool,     bool,   IsOnGround);
	m_Client->HandlePlayerPos(PosX, PosY, PosZ, PosY + 1.62, IsOnGround);
}





void cProtocol180::HandlePacketPlayerPosLook(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadBEDouble, double, PosX);
	HANDLE_READ(a_ByteBuffer, ReadBEDouble, double, PosY);
	HANDLE_READ(a_ByteBuffer, ReadBEDouble, double, PosZ);
	HANDLE_READ(a_ByteBuffer, ReadBEFloat,  float,  Yaw);
	HANDLE_READ(a_ByteBuffer, ReadBEFloat,  float,  Pitch);
	HANDLE_READ(a_ByteBuffer, ReadBool,     bool,   IsOnGround);
	m_Client->HandlePlayerMoveLook(PosX, PosY, PosZ, PosY + 1.62, Yaw, Pitch, IsOnGround);
}





void cProtocol180::HandlePacketPluginMessage(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadVarUTF8String, AString, Channel);
	AString Data;
	a_ByteBuffer.ReadAll(Data);
	m_Client->HandlePluginMessage(Channel, Data);
}





void cProtocol180::HandlePacketSlotSelect(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadBEShort, short, SlotNum);
	m_Client->HandleSlotSelected(SlotNum);
}





void cProtocol180::HandlePacketSteerVehicle(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadBEFloat, float, Forward);
	HANDLE_READ(a_ByteBuffer, ReadBEFloat, float, Sideways);
	HANDLE_READ(a_ByteBuffer, ReadChar,    char,  Flags);

	if ((Flags & 0x2) != 0)
	{
		m_Client->HandleUnmount();
	}
	else if ((Flags & 0x1) != 0)
	{
		m_Client->HandleSteerVehicle(Forward, Sideways);
	}
}





void cProtocol180::HandlePacketTabComplete(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadVarUTF8String, AString, Text);
	HANDLE_READ(a_ByteBuffer, ReadBool, bool, HasPosition);

	if (HasPosition)
	{
		HANDLE_READ(a_ByteBuffer, ReadBEInt64, Int64, Position);
	}

	m_Client->HandleTabCompletion(Text);
}





void cProtocol180::HandlePacketUpdateSign(cByteBuffer & a_ByteBuffer)
{
	int BlockX, BlockY, BlockZ;
	if (!a_ByteBuffer.ReadPosition(BlockX, BlockY, BlockZ))
	{
		return;
	}

	AString Lines[4];
	for (int i = 0; i < 4; i++)
	{
		HANDLE_READ(a_ByteBuffer, ReadVarUTF8String, AString, Line);
		Lines[i] = Line.substr(1, Line.length() - 2);  // Remove ""
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
			m_Client->HandleUseEntity((int)EntityID, false);
			break;
		}
		case 1:
		{
			m_Client->HandleUseEntity((int)EntityID, true);
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
	HANDLE_READ(a_ByteBuffer, ReadByte, Byte, WindowID);
	HANDLE_READ(a_ByteBuffer, ReadByte, Byte, Enchantment);

	m_Client->HandleEnchantItem(WindowID, Enchantment);
}





void cProtocol180::HandlePacketWindowClick(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadChar,    char,  WindowID);
	HANDLE_READ(a_ByteBuffer, ReadBEShort, short, SlotNum);
	HANDLE_READ(a_ByteBuffer, ReadByte,    Byte,  Button);
	HANDLE_READ(a_ByteBuffer, ReadBEShort, short, TransactionID);
	HANDLE_READ(a_ByteBuffer, ReadByte,    Byte,  Mode);
	cItem Item;
	ReadItem(a_ByteBuffer, Item);

	// Convert Button, Mode, SlotNum and HeldItem into eClickAction:
	eClickAction Action;
	switch ((Mode << 8) | Button)
	{
		case 0x0000: Action = (SlotNum != -999) ? caLeftClick  : caLeftClickOutside;  break;
		case 0x0001: Action = (SlotNum != -999) ? caRightClick : caRightClickOutside; break;
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
		case 0x0300: Action = caMiddleClick;     break;
		case 0x0400: Action = (SlotNum == -999) ? caLeftClickOutsideHoldNothing  : caDropKey;     break;
		case 0x0401: Action = (SlotNum == -999) ? caRightClickOutsideHoldNothing : caCtrlDropKey; break;
		case 0x0500: Action = (SlotNum == -999) ? caLeftPaintBegin               : caUnknown;     break;
		case 0x0501: Action = (SlotNum != -999) ? caLeftPaintProgress            : caUnknown;     break;
		case 0x0502: Action = (SlotNum == -999) ? caLeftPaintEnd                 : caUnknown;     break;
		case 0x0504: Action = (SlotNum == -999) ? caRightPaintBegin              : caUnknown;     break;
		case 0x0505: Action = (SlotNum != -999) ? caRightPaintProgress           : caUnknown;     break;
		case 0x0506: Action = (SlotNum == -999) ? caRightPaintEnd                : caUnknown;     break;
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





void cProtocol180::HandlePacketWindowClose(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadChar, char, WindowID);
	m_Client->HandleWindowClose(WindowID);
}





void cProtocol180::WritePacket(cByteBuffer & a_Packet)
{
	cCSLock Lock(m_CSPacket);
	AString Pkt;
	a_Packet.ReadAll(Pkt);
	WriteVarInt((UInt32)Pkt.size());
	SendData(Pkt.data(), Pkt.size());
	Flush();
}





void cProtocol180::SendData(const char * a_Data, size_t a_Size)
{
	if (m_IsEncrypted)
	{
		Byte Encrypted[8192];  // Larger buffer, we may be sending lots of data (chunks)
		while (a_Size > 0)
		{
			size_t NumBytes = (a_Size > sizeof(Encrypted)) ? sizeof(Encrypted) : a_Size;
			m_Encryptor.ProcessData(Encrypted, (Byte *)a_Data, NumBytes);
			m_Client->SendData((const char *)Encrypted, NumBytes);
			a_Size -= NumBytes;
			a_Data += NumBytes;
		}
	}
	else
	{
		m_Client->SendData(a_Data, a_Size);
	}
}





bool cProtocol180::ReadItem(cByteBuffer & a_ByteBuffer, cItem & a_Item, size_t a_RemainingBytes)
{
	HANDLE_PACKET_READ(a_ByteBuffer, ReadBEShort, short, ItemType);
	if (ItemType == -1)
	{
		// The item is empty, no more data follows
		a_Item.Empty();
		return true;
	}
	a_Item.m_ItemType = ItemType;
	
	HANDLE_PACKET_READ(a_ByteBuffer, ReadChar,    char,  ItemCount);
	HANDLE_PACKET_READ(a_ByteBuffer, ReadBEShort, short, ItemDamage);
	a_Item.m_ItemCount  = ItemCount;
	a_Item.m_ItemDamage = ItemDamage;
	if (ItemCount <= 0)
	{
		a_Item.Empty();
	}

	AString Metadata;
	if (!a_ByteBuffer.ReadString(Metadata, a_ByteBuffer.GetReadableSpace() - a_RemainingBytes) || (Metadata.size() == 0) || (Metadata[0] == 0))
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
		CreateHexDump(HexDump, a_Metadata.data(), a_Metadata.size(), 16);
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
								AppendPrintf(Lore, "%s`", NBT.GetString(loretag).c_str());  // Append the lore with a grave accent/backtick, used internally by MCS to display a new line in the client; don't forget to c_str ;)
							}

							a_Item.m_Lore = Lore;
						}
					}
				}
				else if ((TagName == "Fireworks") || (TagName == "Explosion"))
				{
					cFireworkItem::ParseFromNBT(a_Item.m_FireworkItem, NBT, tag, (ENUM_ITEM_ID)a_Item.m_ItemType);
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
	cServer * Server = cRoot::Get()->GetServer();
	const AString & ServerID = Server->GetServerID();
	Checksum.Update((const Byte *)ServerID.c_str(), ServerID.length());
	Checksum.Update(a_Key, 16);
	Checksum.Update((const Byte *)Server->GetPublicKeyDER().data(), Server->GetPublicKeyDER().size());
	Byte Digest[20];
	Checksum.Finalize(Digest);
	cSha1Checksum::DigestToJava(Digest, m_AuthServerID);
}





////////////////////////////////////////////////////////////////////////////////
// cProtocol180::cPacketizer:

cProtocol180::cPacketizer::~cPacketizer()
{
	UInt32 PacketLen = (UInt32)m_Out.GetUsedSpace();
	AString PacketData, CompressedPacket;
	m_Out.ReadAll(PacketData);
	m_Out.CommitRead();

	if ((m_Protocol.m_State == 3) && (PacketLen >= 256))
	{
		if (!cProtocol180::CompressPacket(PacketData, CompressedPacket))
		{
			return;
		}
	}
	else if (m_Protocol.m_State == 3)
	{
		m_Protocol.m_OutPacketLenBuffer.WriteVarInt(PacketLen + 1);
		m_Protocol.m_OutPacketLenBuffer.WriteVarInt(0);

		AString LengthData;
		m_Protocol.m_OutPacketLenBuffer.ReadAll(LengthData);
		m_Protocol.SendData(LengthData.data(), LengthData.size());
	}
	else
	{
		m_Protocol.m_OutPacketLenBuffer.WriteVarInt(PacketLen);

		AString LengthData;
		m_Protocol.m_OutPacketLenBuffer.ReadAll(LengthData);
		m_Protocol.SendData(LengthData.data(), LengthData.size());
	}

	if (CompressedPacket.empty())
	{
		m_Protocol.m_OutPacketLenBuffer.CommitRead();
		m_Protocol.SendData(PacketData.data(), PacketData.size());
	}
	else
	{
		m_Protocol.SendData(CompressedPacket.data(), CompressedPacket.size());
	}

	// Log the comm into logfile:
	if (g_ShouldLogCommOut)
	{
		AString Hex;
		ASSERT(PacketData.size() > 0);
		CreateHexDump(Hex, PacketData.data() + 1, PacketData.size() - 1, 16);
		m_Protocol.m_CommLogFile.Printf("Outgoing packet: type %d (0x%x), length %u (0x%x), state %d. Payload:\n%s\n",
			PacketData[0], PacketData[0], PacketLen, PacketLen, m_Protocol.m_State, Hex.c_str()
		);
	}
}





void cProtocol180::cPacketizer::WriteUUID(const AString & a_UUID)
{
	if (a_UUID.length() != 32)
	{
		LOGWARNING("Attempt to send a bad uuid (length isn't 32): %s", a_UUID.c_str());
		ASSERT(!"Wrong uuid length!");
		return;
	}
	AString UUID_1 = a_UUID.substr(0, 16);
	AString UUID_2 = a_UUID.substr(16);

	Int64 Value_1, Value_2;
	sscanf(UUID_1.c_str(), "%llx", &Value_1);
	sscanf(UUID_2.c_str(), "%llx", &Value_2);

	WriteInt64(Value_1);
	WriteInt64(Value_2);
}





void cProtocol180::cPacketizer::WriteItem(const cItem & a_Item)
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
		WriteShort(-1);
		return;
	}
	
	WriteShort(ItemType);
	WriteByte (a_Item.m_ItemCount);
	WriteShort(a_Item.m_ItemDamage);
	
	if (a_Item.m_Enchantments.IsEmpty() && a_Item.IsBothNameAndLoreEmpty() && (a_Item.m_ItemType != E_ITEM_FIREWORK_ROCKET) && (a_Item.m_ItemType != E_ITEM_FIREWORK_STAR))
	{
		WriteChar(0);
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
	if (!a_Item.IsBothNameAndLoreEmpty())
	{
		Writer.BeginCompound("display");
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
		cFireworkItem::WriteToNBTCompound(a_Item.m_FireworkItem, Writer, (ENUM_ITEM_ID)a_Item.m_ItemType);
	}
	Writer.Finish();

	AString Result = Writer.GetResult();
	if (Result.size() == 0)
	{
		WriteChar(0);
		return;
	}
	WriteBuf(Result.data(), Result.size());
}





void cProtocol180::cPacketizer::WriteBlockEntity(const cBlockEntity & a_BlockEntity)
{
	cFastNBTWriter Writer;

	switch (a_BlockEntity.GetBlockType())
	{
		case E_BLOCK_BEACON:
		{
			cBeaconEntity & BeaconEntity = (cBeaconEntity &)a_BlockEntity;

			Writer.AddInt("x", BeaconEntity.GetPosX());
			Writer.AddInt("y", BeaconEntity.GetPosY());
			Writer.AddInt("z", BeaconEntity.GetPosZ());
			Writer.AddInt("Primary", BeaconEntity.GetPrimaryEffect());
			Writer.AddInt("Secondary", BeaconEntity.GetSecondaryEffect());
			Writer.AddInt("Levels", BeaconEntity.GetBeaconLevel());
			Writer.AddString("id", "Beacon");  // "Tile Entity ID" - MC wiki; vanilla server always seems to send this though
			break;
		}
		case E_BLOCK_COMMAND_BLOCK:
		{
			cCommandBlockEntity & CommandBlockEntity = (cCommandBlockEntity &)a_BlockEntity;

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
				AString Output;
				Printf(Output, "{\"text\":\"%s\"}", CommandBlockEntity.GetLastOutput().c_str());

				Writer.AddString("LastOutput", Output.c_str());
			}
			break;
		}
		case E_BLOCK_HEAD:
		{
			cMobHeadEntity & MobHeadEntity = (cMobHeadEntity &)a_BlockEntity;

			Writer.AddInt("x", MobHeadEntity.GetPosX());
			Writer.AddInt("y", MobHeadEntity.GetPosY());
			Writer.AddInt("z", MobHeadEntity.GetPosZ());
			Writer.AddByte("SkullType", MobHeadEntity.GetType() & 0xFF);
			Writer.AddByte("Rot", MobHeadEntity.GetRotation() & 0xFF);
			Writer.AddString("ExtraType", MobHeadEntity.GetOwner().c_str());
			Writer.AddString("id", "Skull");  // "Tile Entity ID" - MC wiki; vanilla server always seems to send this though
			break;
		}
		case E_BLOCK_FLOWER_POT:
		{
			cFlowerPotEntity & FlowerPotEntity = (cFlowerPotEntity &)a_BlockEntity;

			Writer.AddInt("x", FlowerPotEntity.GetPosX());
			Writer.AddInt("y", FlowerPotEntity.GetPosY());
			Writer.AddInt("z", FlowerPotEntity.GetPosZ());
			Writer.AddInt("Item", (Int32) FlowerPotEntity.GetItem().m_ItemType);
			Writer.AddInt("Data", (Int32) FlowerPotEntity.GetItem().m_ItemDamage);
			Writer.AddString("id", "FlowerPot");  // "Tile Entity ID" - MC wiki; vanilla server always seems to send this though
			break;
		}
		default: break;
	}

	Writer.Finish();
	WriteBuf(Writer.GetResult().data(), Writer.GetResult().size());
}





void cProtocol180::cPacketizer::WriteByteAngle(double a_Angle)
{
	WriteByte((char)(255 * a_Angle / 360));
}





void cProtocol180::cPacketizer::WriteFPInt(double a_Value)
{
	int Value = (int)(a_Value * 32);
	WriteInt(Value);
}





void cProtocol180::cPacketizer::WriteEntityMetadata(const cEntity & a_Entity)
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
	WriteByte(0);  // Byte(0) + index 0
	WriteByte(Flags);
	
	switch (a_Entity.GetEntityType())
	{
		case cEntity::etPlayer: break;  // TODO?
		case cEntity::etPickup:
		{
			WriteByte((5 << 5) | 10);  // Slot(5) + index 10
			WriteItem(((const cPickup &)a_Entity).GetItem());
			break;
		}
		case cEntity::etMinecart:
		{
			WriteByte(0x51);

			// The following expression makes Minecarts shake more with less health or higher damage taken
			// It gets half the maximum health, and takes it away from the current health minus the half health:
			/*
			Health: 5 | 3 - (5 - 3) = 1 (shake power)
			Health: 3 | 3 - (3 - 3) = 3
			Health: 1 | 3 - (1 - 3) = 5
			*/
			WriteInt((((a_Entity.GetMaxHealth() / 2) - (a_Entity.GetHealth() - (a_Entity.GetMaxHealth() / 2))) * ((const cMinecart &)a_Entity).LastDamage()) * 4);
			WriteByte(0x52);
			WriteInt(1);  // Shaking direction, doesn't seem to affect anything
			WriteByte(0x73);
			WriteFloat((float)(((const cMinecart &)a_Entity).LastDamage() + 10));  // Damage taken / shake effect multiplyer
			
			if (((cMinecart &)a_Entity).GetPayload() == cMinecart::mpNone)
			{
				cRideableMinecart & RideableMinecart = ((cRideableMinecart &)a_Entity);
				const cItem & MinecartContent = RideableMinecart.GetContent();
				if (!MinecartContent.IsEmpty())
				{
					WriteByte(0x54);
					int Content = MinecartContent.m_ItemType;
					Content |= MinecartContent.m_ItemDamage << 8;
					WriteInt(Content);
					WriteByte(0x55);
					WriteInt(RideableMinecart.GetBlockHeight());
					WriteByte(0x56);
					WriteByte(1);
				}
			}
			else if (((cMinecart &)a_Entity).GetPayload() == cMinecart::mpFurnace)
			{
				WriteByte(0x10);
				WriteByte(((const cMinecartWithFurnace &)a_Entity).IsFueled() ? 1 : 0);
			}
			break;
		}
		case cEntity::etProjectile:
		{
			cProjectileEntity & Projectile = (cProjectileEntity &)a_Entity;
			switch (Projectile.GetProjectileKind())
			{
				case cProjectileEntity::pkArrow:
				{
					WriteByte(0x10);
					WriteByte(((const cArrowEntity &)a_Entity).IsCritical() ? 1 : 0);
					break;
				}
				case cProjectileEntity::pkFirework:
				{
					WriteByte(0xA8);
					WriteItem(((const cFireworkEntity &)a_Entity).GetItem());
					break;
				}
				default: break;
			}
			break;
		}
		case cEntity::etMonster:
		{
			WriteMobMetadata((const cMonster &)a_Entity);
			break;
		}
		case cEntity::etItemFrame:
		{
			cItemFrame & Frame = (cItemFrame &)a_Entity;
			WriteByte(0xA8);
			WriteItem(Frame.GetItem());
			WriteByte(0x09);
			WriteByte(Frame.GetRotation());
			break;
		}
		default: break;
	}
}





void cProtocol180::cPacketizer::WriteMobMetadata(const cMonster & a_Mob)
{
	switch (a_Mob.GetMobType())
	{
		case mtCreeper:
		{
			WriteByte(0x10);
			WriteByte(((const cCreeper &)a_Mob).IsBlowing() ? 1 : -1);
			WriteByte(0x11);
			WriteByte(((const cCreeper &)a_Mob).IsCharged() ? 1 : 0);
			break;
		}
		
		case mtBat:
		{
			WriteByte(0x10);
			WriteByte(((const cBat &)a_Mob).IsHanging() ? 1 : 0);
			break;
		}
		
		case mtPig:
		{
			WriteByte(0x10);
			WriteByte(((const cPig &)a_Mob).IsSaddled() ? 1 : 0);
			break;
		}
		
		case mtVillager:
		{
			WriteByte(0x50);
			WriteInt(((const cVillager &)a_Mob).GetVilType());
			break;
		}
		
		case mtZombie:
		{
			WriteByte(0x0c);
			WriteByte(((const cZombie &)a_Mob).IsBaby() ? 1 : 0);
			WriteByte(0x0d);
			WriteByte(((const cZombie &)a_Mob).IsVillagerZombie() ? 1 : 0);
			WriteByte(0x0e);
			WriteByte(((const cZombie &)a_Mob).IsConverting() ? 1 : 0);
			break;
		}
		
		case mtGhast:
		{
			WriteByte(0x10);
			WriteByte(((const cGhast &)a_Mob).IsCharging());
			break;
		}
		
		case mtWolf:
		{
			const cWolf & Wolf = (const cWolf &)a_Mob;
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
			WriteByte(0x10);
			WriteByte(WolfStatus);

			WriteByte(0x72);
			WriteFloat((float)(a_Mob.GetHealth()));
			WriteByte(0x13);
			WriteByte(Wolf.IsBegging() ? 1 : 0);
			WriteByte(0x14);
			WriteByte(Wolf.GetCollarColor());
			break;
		}
		
		case mtSheep:
		{
			WriteByte(0x10);
			Byte SheepMetadata = 0;
			SheepMetadata = ((const cSheep &)a_Mob).GetFurColor();
			if (((const cSheep &)a_Mob).IsSheared())
			{
				SheepMetadata |= 0x10;
			}
			WriteByte(SheepMetadata);
			break;
		}
		
		case mtEnderman:
		{
			WriteByte(0x30);
			WriteShort((Byte)(((const cEnderman &)a_Mob).GetCarriedBlock()));
			WriteByte(0x11);
			WriteByte((Byte)(((const cEnderman &)a_Mob).GetCarriedMeta()));
			WriteByte(0x12);
			WriteByte(((const cEnderman &)a_Mob).IsScreaming() ? 1 : 0);
			break;
		}
		
		case mtSkeleton:
		{
			WriteByte(0x0d);
			WriteByte(((const cSkeleton &)a_Mob).IsWither() ? 1 : 0);
			break;
		}
		
		case mtWitch:
		{
			WriteByte(0x15);
			WriteByte(((const cWitch &)a_Mob).IsAngry() ? 1 : 0);
			break;
		}

		case mtWither:
		{
			WriteByte(0x54);  // Int at index 20
			WriteInt(((const cWither &)a_Mob).GetWitherInvulnerableTicks());
			WriteByte(0x66);  // Float at index 6
			WriteFloat((float)(a_Mob.GetHealth()));
			break;
		}
		
		case mtSlime:
		{
			WriteByte(0x10);
			WriteByte(((const cSlime &)a_Mob).GetSize());
			break;
		}
		
		case mtMagmaCube:
		{
			WriteByte(0x10);
			WriteByte(((const cMagmaCube &)a_Mob).GetSize());
			break;
		}
		
		case mtHorse:
		{
			const cHorse & Horse = (const cHorse &)a_Mob;
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
			WriteByte(0x50);  // Int at index 16
			WriteInt(Flags);
			WriteByte(0x13);  // Byte at index 19
			WriteByte(Horse.GetHorseType());
			WriteByte(0x54);  // Int at index 20
			int Appearance = 0;
			Appearance = Horse.GetHorseColor();
			Appearance |= Horse.GetHorseStyle() << 8;
			WriteInt(Appearance);
			WriteByte(0x56);  // Int at index 22
			WriteInt(Horse.GetHorseArmour());
			break;
		}
	}  // switch (a_Mob.GetType())
}





void cProtocol180::cPacketizer::WriteEntityProperties(const cEntity & a_Entity)
{
	if (!a_Entity.IsMob())
	{
		// No properties for anything else than mobs
		WriteInt(0);
		return;
	}

	// const cMonster & Mob = (const cMonster &)a_Entity;

	// TODO: Send properties and modifiers based on the mob type
	
	WriteInt(0);  // NumProperties
}



