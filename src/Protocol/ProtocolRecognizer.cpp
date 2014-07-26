
// ProtocolRecognizer.cpp

// Implements the cProtocolRecognizer class representing the meta-protocol that recognizes possibly multiple
// protocol versions and redirects everything to them

#include "Globals.h"

#include "ProtocolRecognizer.h"
#include "Protocol125.h"
#include "Protocol132.h"
#include "Protocol14x.h"
#include "Protocol15x.h"
#include "Protocol16x.h"
#include "Protocol17x.h"
#include "../ClientHandle.h"
#include "../Root.h"
#include "../Server.h"
#include "../World.h"
#include "../ChatColor.h"





cProtocolRecognizer::cProtocolRecognizer(cClientHandle * a_Client) :
	super(a_Client),
	m_Protocol(NULL),
	m_Buffer(512)
{
}





cProtocolRecognizer::~cProtocolRecognizer()
{
	delete m_Protocol;
	m_Protocol = NULL;
}





AString cProtocolRecognizer::GetVersionTextFromInt(int a_ProtocolVersion)
{
	switch (a_ProtocolVersion)
	{
		case PROTO_VERSION_1_2_5: return "1.2.5";
		case PROTO_VERSION_1_3_2: return "1.3.2";
		case PROTO_VERSION_1_4_2: return "1.4.2";
		case PROTO_VERSION_1_4_4: return "1.4.4";
		case PROTO_VERSION_1_4_6: return "1.4.6";
		case PROTO_VERSION_1_5_0: return "1.5";
		case PROTO_VERSION_1_5_2: return "1.5.2";
		case PROTO_VERSION_1_6_1: return "1.6.1";
		case PROTO_VERSION_1_6_2: return "1.6.2";
		case PROTO_VERSION_1_6_3: return "1.6.3";
		case PROTO_VERSION_1_6_4: return "1.6.4";
		case PROTO_VERSION_1_7_2: return "1.7.2";
		case PROTO_VERSION_1_7_6: return "1.7.6";
	}
	ASSERT(!"Unknown protocol version");
	return Printf("Unknown protocol (%d)", a_ProtocolVersion);
}





void cProtocolRecognizer::DataReceived(const char * a_Data, size_t a_Size)
{
	if (m_Protocol == NULL)
	{
		if (!m_Buffer.Write(a_Data, a_Size))
		{
			m_Client->Kick("Unsupported protocol version");
			return;
		}
		
		if (!TryRecognizeProtocol())
		{
			return;
		}

		// The protocol has just been recognized, dump the whole m_Buffer contents into it for parsing:
		AString Dump;
		m_Buffer.ResetRead();
		m_Buffer.ReadAll(Dump);
		m_Protocol->DataReceived(Dump.data(), Dump.size());
	}
	else
	{
		m_Protocol->DataReceived(a_Data, a_Size);
	}
}





void cProtocolRecognizer::SendAttachEntity(const cEntity & a_Entity, const cEntity * a_Vehicle)
{
	ASSERT(m_Protocol != NULL);
	m_Protocol->SendAttachEntity(a_Entity, a_Vehicle);
}





void cProtocolRecognizer::SendBlockAction(int a_BlockX, int a_BlockY, int a_BlockZ, char a_Byte1, char a_Byte2, BLOCKTYPE a_BlockType)
{
	ASSERT(m_Protocol != NULL);
	m_Protocol->SendBlockAction(a_BlockX, a_BlockY, a_BlockZ, a_Byte1, a_Byte2, a_BlockType);
}





void cProtocolRecognizer::SendBlockBreakAnim(int a_entityID, int a_BlockX, int a_BlockY, int a_BlockZ, char stage)
{
	ASSERT(m_Protocol != NULL);
	m_Protocol->SendBlockBreakAnim(a_entityID, a_BlockX, a_BlockY, a_BlockZ, stage);
}





void cProtocolRecognizer::SendBlockChange(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta)
{
	ASSERT(m_Protocol != NULL);
	m_Protocol->SendBlockChange(a_BlockX, a_BlockY, a_BlockZ, a_BlockType, a_BlockMeta);
}





void cProtocolRecognizer::SendBlockChanges(int a_ChunkX, int a_ChunkZ, const sSetBlockVector & a_Changes)
{
	ASSERT(m_Protocol != NULL);
	m_Protocol->SendBlockChanges(a_ChunkX, a_ChunkZ, a_Changes);
}





void cProtocolRecognizer::SendChat(const AString & a_Message)
{
	ASSERT(m_Protocol != NULL);
	m_Protocol->SendChat(a_Message);
}





void cProtocolRecognizer::SendChat(const cCompositeChat & a_Message)
{
	ASSERT(m_Protocol != NULL);
	m_Protocol->SendChat(a_Message);
}





void cProtocolRecognizer::SendChunkData(int a_ChunkX, int a_ChunkZ, cChunkDataSerializer & a_Serializer)
{
	ASSERT(m_Protocol != NULL);
	m_Protocol->SendChunkData(a_ChunkX, a_ChunkZ, a_Serializer);
}





void cProtocolRecognizer::SendCollectEntity(const cEntity & a_Entity, const cPlayer & a_Player)
{
	ASSERT(m_Protocol != NULL);
	m_Protocol->SendCollectEntity(a_Entity, a_Player);
}





void cProtocolRecognizer::SendDestroyEntity(const cEntity & a_Entity)
{
	ASSERT(m_Protocol != NULL);
	m_Protocol->SendDestroyEntity(a_Entity);
}





void cProtocolRecognizer::SendDisconnect(const AString & a_Reason)
{
	if (m_Protocol != NULL)
	{
		m_Protocol->SendDisconnect(a_Reason);
	}
	else
	{
		// This is used when the client sends a server-ping, respond with the default packet:
		WriteByte  (0xff);  // PACKET_DISCONNECT
		WriteString(a_Reason);
	}
}




void cProtocolRecognizer::SendEditSign(int a_BlockX, int a_BlockY, int a_BlockZ)
{
	ASSERT(m_Protocol != NULL);
	m_Protocol->SendEditSign(a_BlockX, a_BlockY, a_BlockZ);
}





void cProtocolRecognizer::SendEntityEffect(const cEntity & a_Entity, int a_EffectID, int a_Amplifier, short a_Duration)
{
	ASSERT(m_Protocol != NULL);
	m_Protocol->SendEntityEffect(a_Entity, a_EffectID, a_Amplifier, a_Duration);
}





void cProtocolRecognizer::SendEntityEquipment(const cEntity & a_Entity, short a_SlotNum, const cItem & a_Item)
{
	ASSERT(m_Protocol != NULL);
	m_Protocol->SendEntityEquipment(a_Entity, a_SlotNum, a_Item);
}





void cProtocolRecognizer::SendEntityHeadLook(const cEntity & a_Entity)
{
	ASSERT(m_Protocol != NULL);
	m_Protocol->SendEntityHeadLook(a_Entity);
}





void cProtocolRecognizer::SendEntityLook(const cEntity & a_Entity)
{
	ASSERT(m_Protocol != NULL);
	m_Protocol->SendEntityLook(a_Entity);
}





void cProtocolRecognizer::SendEntityMetadata(const cEntity & a_Entity)
{
	ASSERT(m_Protocol != NULL);
	m_Protocol->SendEntityMetadata(a_Entity);
}





void cProtocolRecognizer::SendEntityProperties(const cEntity & a_Entity)
{
	ASSERT(m_Protocol != NULL);
	m_Protocol->SendEntityProperties(a_Entity);
}





void cProtocolRecognizer::SendEntityRelMove(const cEntity & a_Entity, char a_RelX, char a_RelY, char a_RelZ)
{
	ASSERT(m_Protocol != NULL);
	m_Protocol->SendEntityRelMove(a_Entity, a_RelX, a_RelY, a_RelZ);
}





void cProtocolRecognizer::SendEntityRelMoveLook(const cEntity & a_Entity, char a_RelX, char a_RelY, char a_RelZ)
{
	ASSERT(m_Protocol != NULL);
	m_Protocol->SendEntityRelMoveLook(a_Entity, a_RelX, a_RelY, a_RelZ);
}





void cProtocolRecognizer::SendEntityStatus(const cEntity & a_Entity, char a_Status)
{
	ASSERT(m_Protocol != NULL);
	m_Protocol->SendEntityStatus(a_Entity, a_Status);
}





void cProtocolRecognizer::SendEntityVelocity(const cEntity & a_Entity)
{
	ASSERT(m_Protocol != NULL);
	m_Protocol->SendEntityVelocity(a_Entity);
}





void cProtocolRecognizer::SendExplosion(double a_BlockX, double a_BlockY, double a_BlockZ, float a_Radius, const cVector3iArray & a_BlocksAffected, const Vector3d & a_PlayerMotion)
{
	ASSERT(m_Protocol != NULL);
	m_Protocol->SendExplosion(a_BlockX, a_BlockY, a_BlockZ, a_Radius, a_BlocksAffected, a_PlayerMotion);
}





void cProtocolRecognizer::SendGameMode(eGameMode a_GameMode)
{
	ASSERT(m_Protocol != NULL);
	m_Protocol->SendGameMode(a_GameMode);
}





void cProtocolRecognizer::SendHealth(void)
{
	ASSERT(m_Protocol != NULL);
	m_Protocol->SendHealth();
}





void cProtocolRecognizer::SendWindowProperty(const cWindow & a_Window, int a_Property, int a_Value)
{
	ASSERT(m_Protocol != NULL);
	m_Protocol->SendWindowProperty(a_Window, a_Property, a_Value);
}





void cProtocolRecognizer::SendInventorySlot(char a_WindowID, short a_SlotNum, const cItem & a_Item)
{
	ASSERT(m_Protocol != NULL);
	m_Protocol->SendInventorySlot(a_WindowID, a_SlotNum, a_Item);
}





void cProtocolRecognizer::SendKeepAlive(int a_PingID)
{
	ASSERT(m_Protocol != NULL);
	m_Protocol->SendKeepAlive(a_PingID);
}





void cProtocolRecognizer::SendLogin(const cPlayer & a_Player, const cWorld & a_World)
{
	ASSERT(m_Protocol != NULL);
	m_Protocol->SendLogin(a_Player, a_World);
}





void cProtocolRecognizer::SendLoginSuccess(void)
{
	ASSERT(m_Protocol != NULL);
	m_Protocol->SendLoginSuccess();
}





void cProtocolRecognizer::SendMapColumn(int a_ID, int a_X, int a_Y, const Byte * a_Colors, unsigned int a_Length)
{
	ASSERT(m_Protocol != NULL);
	m_Protocol->SendMapColumn(a_ID, a_X, a_Y, a_Colors, a_Length);
}





void cProtocolRecognizer::SendMapDecorators(int a_ID, const cMapDecoratorList & a_Decorators)
{
	ASSERT(m_Protocol != NULL);
	m_Protocol->SendMapDecorators(a_ID, a_Decorators);
}





void cProtocolRecognizer::SendMapInfo(int a_ID, unsigned int a_Scale)
{
	ASSERT(m_Protocol != NULL);
	m_Protocol->SendMapInfo(a_ID, a_Scale);
}





void cProtocolRecognizer::SendParticleEffect(const AString & a_ParticleName, float a_SrcX, float a_SrcY, float a_SrcZ, float a_OffsetX, float a_OffsetY, float a_OffsetZ, float a_ParticleData, int a_ParticleAmmount)
{
	ASSERT(m_Protocol != NULL);
	m_Protocol->SendParticleEffect(a_ParticleName, a_SrcX, a_SrcY, a_SrcZ, a_OffsetX, a_OffsetY, a_OffsetZ, a_ParticleData, a_ParticleAmmount);
}




void cProtocolRecognizer::SendPaintingSpawn(const cPainting & a_Painting)
{
	m_Protocol->SendPaintingSpawn(a_Painting);
}





void cProtocolRecognizer::SendPickupSpawn(const cPickup & a_Pickup)
{
	ASSERT(m_Protocol != NULL);
	m_Protocol->SendPickupSpawn(a_Pickup);
}





void cProtocolRecognizer::SendPlayerAbilities(void)
{
	ASSERT(m_Protocol != NULL);
	m_Protocol->SendPlayerAbilities();
}





void cProtocolRecognizer::SendEntityAnimation(const cEntity & a_Entity, char a_Animation)
{
	ASSERT(m_Protocol != NULL);
	m_Protocol->SendEntityAnimation(a_Entity, a_Animation);
}





void cProtocolRecognizer::SendPlayerListItem(const cPlayer & a_Player, bool a_IsOnline)
{
	ASSERT(m_Protocol != NULL);
	m_Protocol->SendPlayerListItem(a_Player, a_IsOnline);
}





void cProtocolRecognizer::SendPlayerMaxSpeed(void)
{
	ASSERT(m_Protocol != NULL);
	m_Protocol->SendPlayerMaxSpeed();
}





void cProtocolRecognizer::SendPlayerMoveLook(void)
{
	ASSERT(m_Protocol != NULL);
	m_Protocol->SendPlayerMoveLook();
}





void cProtocolRecognizer::SendPlayerPosition(void)
{
	ASSERT(m_Protocol != NULL);
	m_Protocol->SendPlayerPosition();
}





void cProtocolRecognizer::SendPlayerSpawn(const cPlayer & a_Player)
{
	ASSERT(m_Protocol != NULL);
	m_Protocol->SendPlayerSpawn(a_Player);
}





void cProtocolRecognizer::SendPluginMessage(const AString & a_Channel, const AString & a_Message)
{
	ASSERT(m_Protocol != NULL);
	m_Protocol->SendPluginMessage(a_Channel, a_Message);
}





void cProtocolRecognizer::SendRemoveEntityEffect(const cEntity & a_Entity, int a_EffectID)
{
	ASSERT(m_Protocol != NULL);
	m_Protocol->SendRemoveEntityEffect(a_Entity, a_EffectID);
}





void cProtocolRecognizer::SendRespawn(const cWorld & a_World, bool a_ShouldIgnoreDimensionChecks)
{
	ASSERT(m_Protocol != NULL);
	m_Protocol->SendRespawn(a_World, a_ShouldIgnoreDimensionChecks);
}





void cProtocolRecognizer::SendExperience(void)
{
	ASSERT(m_Protocol != NULL);
	m_Protocol->SendExperience();
}





void cProtocolRecognizer::SendExperienceOrb(const cExpOrb & a_ExpOrb)
{
	ASSERT(m_Protocol != NULL);
	m_Protocol->SendExperienceOrb(a_ExpOrb);
}





void cProtocolRecognizer::SendScoreboardObjective(const AString & a_Name, const AString & a_DisplayName, Byte a_Mode)
{
	ASSERT(m_Protocol != NULL);
	m_Protocol->SendScoreboardObjective(a_Name, a_DisplayName, a_Mode);
}





void cProtocolRecognizer::SendScoreUpdate(const AString & a_Objective, const AString & a_Player, cObjective::Score a_Score, Byte a_Mode)
{
	ASSERT(m_Protocol != NULL);
	m_Protocol->SendScoreUpdate(a_Objective, a_Player, a_Score, a_Mode);
}





void cProtocolRecognizer::SendDisplayObjective(const AString & a_Objective, cScoreboard::eDisplaySlot a_Display)
{
	ASSERT(m_Protocol != NULL);
	m_Protocol->SendDisplayObjective(a_Objective, a_Display);
}





void cProtocolRecognizer::SendSoundEffect(const AString & a_SoundName, double a_X, double a_Y, double a_Z, float a_Volume, float a_Pitch)
{
	ASSERT(m_Protocol != NULL);
	m_Protocol->SendSoundEffect(a_SoundName, a_X, a_Y, a_Z, a_Volume, a_Pitch);
}





void cProtocolRecognizer::SendSoundParticleEffect(int a_EffectID, int a_SrcX, int a_SrcY, int a_SrcZ, int a_Data)
{
	ASSERT(m_Protocol != NULL);
	m_Protocol->SendSoundParticleEffect(a_EffectID, a_SrcX, a_SrcY, a_SrcZ, a_Data);
}





void cProtocolRecognizer::SendSpawnFallingBlock(const cFallingBlock & a_FallingBlock)
{
	ASSERT(m_Protocol != NULL);
	m_Protocol->SendSpawnFallingBlock(a_FallingBlock);
}





void cProtocolRecognizer::SendSpawnMob(const cMonster & a_Mob)
{
	ASSERT(m_Protocol != NULL);
	m_Protocol->SendSpawnMob(a_Mob);
}





void cProtocolRecognizer::SendSpawnObject(const cEntity & a_Entity, char a_ObjectType, int a_ObjectData, Byte a_Yaw, Byte a_Pitch)
{
	ASSERT(m_Protocol != NULL);
	m_Protocol->SendSpawnObject(a_Entity, a_ObjectType, a_ObjectData, a_Yaw, a_Pitch);
}





void cProtocolRecognizer::SendSpawnVehicle(const cEntity & a_Vehicle, char a_VehicleType, char a_VehicleSubType)
{
	ASSERT(m_Protocol != NULL);
	m_Protocol->SendSpawnVehicle(a_Vehicle, a_VehicleType, a_VehicleSubType);
}





void cProtocolRecognizer::SendStatistics(const cStatManager & a_Manager)
{
	ASSERT(m_Protocol != NULL);
	m_Protocol->SendStatistics(a_Manager);
}





void cProtocolRecognizer::SendTabCompletionResults(const AStringVector & a_Results)
{
	ASSERT(m_Protocol != NULL);
	m_Protocol->SendTabCompletionResults(a_Results);
}





void cProtocolRecognizer::SendTeleportEntity(const cEntity & a_Entity)
{
	ASSERT(m_Protocol != NULL);
	m_Protocol->SendTeleportEntity(a_Entity);
}





void cProtocolRecognizer::SendThunderbolt(int a_BlockX, int a_BlockY, int a_BlockZ)
{
	ASSERT(m_Protocol != NULL);
	m_Protocol->SendThunderbolt(a_BlockX, a_BlockY, a_BlockZ);
}





void cProtocolRecognizer::SendTimeUpdate(Int64 a_WorldAge, Int64 a_TimeOfDay)
{
	ASSERT(m_Protocol != NULL);
	m_Protocol->SendTimeUpdate(a_WorldAge, a_TimeOfDay);
}





void cProtocolRecognizer::SendUnloadChunk(int a_ChunkX, int a_ChunkZ)
{
	ASSERT(m_Protocol != NULL);
	m_Protocol->SendUnloadChunk(a_ChunkX, a_ChunkZ);
}





void cProtocolRecognizer::SendUpdateBlockEntity(cBlockEntity & a_BlockEntity)
{
	ASSERT(m_Protocol != NULL);
	m_Protocol->SendUpdateBlockEntity(a_BlockEntity);
}





void cProtocolRecognizer::SendUpdateSign(int a_BlockX, int a_BlockY, int a_BlockZ, const AString & a_Line1, const AString & a_Line2, const AString & a_Line3, const AString & a_Line4)
{
	ASSERT(m_Protocol != NULL);
	m_Protocol->SendUpdateSign(a_BlockX, a_BlockY, a_BlockZ, a_Line1, a_Line2, a_Line3, a_Line4);
}





void cProtocolRecognizer::SendUseBed(const cEntity & a_Entity, int a_BlockX, int a_BlockY, int a_BlockZ)
{
	ASSERT(m_Protocol != NULL);
	m_Protocol->SendUseBed(a_Entity, a_BlockX, a_BlockY, a_BlockZ);
}





void cProtocolRecognizer::SendWeather(eWeather a_Weather)
{
	ASSERT(m_Protocol != NULL);
	m_Protocol->SendWeather(a_Weather);
}





void cProtocolRecognizer::SendWholeInventory(const cWindow & a_Window)
{
	ASSERT(m_Protocol != NULL);
	m_Protocol->SendWholeInventory(a_Window);
}





void cProtocolRecognizer::SendWindowClose(const cWindow & a_Window)
{
	ASSERT(m_Protocol != NULL);
	m_Protocol->SendWindowClose(a_Window);
}





void cProtocolRecognizer::SendWindowOpen(const cWindow & a_Window)
{
	ASSERT(m_Protocol != NULL);
	m_Protocol->SendWindowOpen(a_Window);
}





AString cProtocolRecognizer::GetAuthServerID(void)
{
	ASSERT(m_Protocol != NULL);
	return m_Protocol->GetAuthServerID();
}





void cProtocolRecognizer::SendData(const char * a_Data, size_t a_Size)
{
	// This is used only when handling the server ping
	m_Client->SendData(a_Data, a_Size);
}





bool cProtocolRecognizer::TryRecognizeProtocol(void)
{
	// NOTE: If a new protocol is added or an old one is removed, adjust MCS_CLIENT_VERSIONS and
	// MCS_PROTOCOL_VERSIONS macros in the header file, as well as PROTO_VERSION_LATEST macro
	
	// The first packet should be a Handshake, 0x02:
	unsigned char PacketType;
	if (!m_Buffer.ReadByte(PacketType))
	{
		return false;
	}
	switch (PacketType)
	{
		case 0x02: return TryRecognizeLengthlessProtocol();  // Handshake, continue recognizing
		case 0xfe:
		{
			// This may be either a packet length or the length-less Ping packet
			Byte NextByte;
			if (!m_Buffer.ReadByte(NextByte))
			{
				// Not enough data for either protocol
				// This could actually happen with the 1.2 / 1.3 client, but their support is fading out anyway
				return false;
			}
			if (NextByte != 0x01)
			{
				// This is definitely NOT a length-less Ping packet, handle as lengthed protocol:
				break;
			}
			if (!m_Buffer.ReadByte(NextByte))
			{
				// There is no more data. Although this *could* mean TCP fragmentation, it is highly unlikely
				// and rather this is a 1.4 client sending a regular Ping packet (without the following Plugin message)
				SendLengthlessServerPing();
				return false;
			}
			if (NextByte == 0xfa)
			{
				// Definitely a length-less Ping followed by a Plugin message
				SendLengthlessServerPing();
				return false;
			}
			// Definitely a lengthed Initial handshake, handle below:
			break;
		}
	}  // switch (PacketType)

	// This must be a lengthed protocol, try if it has the entire initial handshake packet:
	m_Buffer.ResetRead();
	UInt32 PacketLen;
	UInt32 ReadSoFar = (UInt32)m_Buffer.GetReadableSpace();
	if (!m_Buffer.ReadVarInt(PacketLen))
	{
		// Not enough bytes for the packet length, keep waiting
		return false;
	}
	ReadSoFar -= (UInt32)m_Buffer.GetReadableSpace();
	if (!m_Buffer.CanReadBytes(PacketLen))
	{
		// Not enough bytes for the packet, keep waiting
		return false;
	}
	return TryRecognizeLengthedProtocol(PacketLen - ReadSoFar);
}





bool cProtocolRecognizer::TryRecognizeLengthlessProtocol(void)
{
	// The comm started with 0x02, which is a Handshake packet in the length-less protocol family
	// 1.3.2 starts with 0x02 0x39 <name-length-short>
	// 1.2.5 starts with 0x02 <name-length-short> and name is expected to less than 0x3900 long :)
	char ch;
	if (!m_Buffer.ReadChar(ch))
	{
		return false;
	}
	switch (ch)
	{
		case PROTO_VERSION_1_3_2:
		{
			m_Protocol = new cProtocol132(m_Client);
			return true;
		}
		case PROTO_VERSION_1_4_2:
		case PROTO_VERSION_1_4_4:
		{
			m_Protocol = new cProtocol142(m_Client);
			return true;
		}
		case PROTO_VERSION_1_4_6:
		{
			m_Protocol = new cProtocol146(m_Client);
			return true;
		}
		case PROTO_VERSION_1_5_0:
		case PROTO_VERSION_1_5_2:
		{
			m_Protocol = new cProtocol150(m_Client);
			return true;
		}
		case PROTO_VERSION_1_6_1:
		{
			m_Protocol = new cProtocol161(m_Client);
			return true;
		}
		case PROTO_VERSION_1_6_2:
		case PROTO_VERSION_1_6_3:
		case PROTO_VERSION_1_6_4:
		{
			m_Protocol = new cProtocol162(m_Client);
			return true;
		}
	}
	m_Protocol = new cProtocol125(m_Client);
	return true;
}





bool cProtocolRecognizer::TryRecognizeLengthedProtocol(UInt32 a_PacketLengthRemaining)
{
	UInt32 PacketType;
	if (!m_Buffer.ReadVarInt(PacketType))
	{
		return false;
	}
	if (PacketType != 0x00)
	{
		// Not an initial handshake packet, we don't know how to talk to them
		LOGINFO("Client \"%s\" uses an unsupported protocol (lengthed, initial packet %u)",
			m_Client->GetIPString().c_str(), PacketType
		);
		m_Client->Kick("Unsupported protocol version");
		return false;
	}
	UInt32 ProtocolVersion;
	if (!m_Buffer.ReadVarInt(ProtocolVersion))
	{
		return false;
	}
	switch (ProtocolVersion)
	{
		case PROTO_VERSION_1_7_2:
		{
			AString ServerAddress;
			short ServerPort;
			UInt32 NextState;
			m_Buffer.ReadVarUTF8String(ServerAddress);
			m_Buffer.ReadBEShort(ServerPort);
			m_Buffer.ReadVarInt(NextState);
			m_Buffer.CommitRead();
			m_Protocol = new cProtocol172(m_Client, ServerAddress, (UInt16)ServerPort, NextState);
			return true;
		}
		case PROTO_VERSION_1_7_6:
		{
			AString ServerAddress;
			short ServerPort;
			UInt32 NextState;
			m_Buffer.ReadVarUTF8String(ServerAddress);
			m_Buffer.ReadBEShort(ServerPort);
			m_Buffer.ReadVarInt(NextState);
			m_Buffer.CommitRead();
			m_Protocol = new cProtocol176(m_Client, ServerAddress, (UInt16)ServerPort, NextState);
			return true;
		}
	}
	LOGINFO("Client \"%s\" uses an unsupported protocol (lengthed, version %u)",
		m_Client->GetIPString().c_str(), ProtocolVersion
	);
	m_Client->Kick("Unsupported protocol version");
	return false;
}





void cProtocolRecognizer::SendLengthlessServerPing(void)
{
	AString Reply;
	cServer * Server = cRoot::Get()->GetServer();
	switch (cRoot::Get()->GetPrimaryServerVersion())
	{
		case PROTO_VERSION_1_2_5:
		case PROTO_VERSION_1_3_2:
		{
			// http://wiki.vg/wiki/index.php?title=Protocol&oldid=3099#Server_List_Ping_.280xFE.29
			Printf(Reply, "%s%s%i%s%i",
				Server->GetDescription().c_str(),
				cChatColor::Delimiter,
				Server->GetNumPlayers(),
				cChatColor::Delimiter,
				Server->GetMaxPlayers()
			);
			break;
		}
		
		case PROTO_VERSION_1_4_2:
		case PROTO_VERSION_1_4_4:
		case PROTO_VERSION_1_4_6:
		case PROTO_VERSION_1_5_0:
		case PROTO_VERSION_1_5_2:
		case PROTO_VERSION_1_6_1:
		case PROTO_VERSION_1_6_2:
		case PROTO_VERSION_1_6_3:
		case PROTO_VERSION_1_6_4:
		{
			// The server list ping now has 1 more byte of "magic". Mojang just loves to complicate stuff.
			// http://wiki.vg/wiki/index.php?title=Protocol&oldid=3101#Server_List_Ping_.280xFE.29
			// _X 2012_10_31: I know that this needn't eat the byte, since it still may be in transit.
			//    Who cares? We're disconnecting anyway.
			m_Buffer.ResetRead();
			if (m_Buffer.CanReadBytes(2))
			{
				Byte val;
				m_Buffer.ReadByte(val);  // Packet type - Serverlist ping
				m_Buffer.ReadByte(val);  // 0x01 magic value
				ASSERT(val == 0x01);
			}
			
			// http://wiki.vg/wiki/index.php?title=Server_List_Ping&oldid=3100
			AString NumPlayers;
			Printf(NumPlayers, "%d", Server->GetNumPlayers());
			AString MaxPlayers;
			Printf(MaxPlayers, "%d", Server->GetMaxPlayers());
			
			AString ProtocolVersionNum;
			Printf(ProtocolVersionNum, "%d", cRoot::Get()->GetPrimaryServerVersion());
			AString ProtocolVersionTxt(GetVersionTextFromInt(cRoot::Get()->GetPrimaryServerVersion()));

			// Cannot use Printf() because of in-string NUL bytes.
			Reply = cChatColor::Delimiter;
			Reply.append("1");
			Reply.push_back(0);
			Reply.append(ProtocolVersionNum);
			Reply.push_back(0);
			Reply.append(ProtocolVersionTxt);
			Reply.push_back(0);
			Reply.append(Server->GetDescription());
			Reply.push_back(0);
			Reply.append(NumPlayers);
			Reply.push_back(0);
			Reply.append(MaxPlayers);
			break;
		}
	}  // switch (m_PrimaryServerVersion)
	m_Client->Kick(Reply);
}




