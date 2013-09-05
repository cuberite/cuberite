
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
	}
	ASSERT(!"Unknown protocol version");
	return Printf("Unknown protocol (%d)", a_ProtocolVersion);
}





void cProtocolRecognizer::DataReceived(const char * a_Data, int a_Size)
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





void cProtocolRecognizer::SendChunkData(int a_ChunkX, int a_ChunkZ, cChunkDataSerializer & a_Serializer)
{
	ASSERT(m_Protocol != NULL);
	m_Protocol->SendChunkData(a_ChunkX, a_ChunkZ, a_Serializer);
}





void cProtocolRecognizer::SendCollectPickup(const cPickup & a_Pickup, const cPlayer & a_Player)
{
	ASSERT(m_Protocol != NULL);
	m_Protocol->SendCollectPickup(a_Pickup, a_Player);
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
		WriteByte  ((char)0xff);  // PACKET_DISCONNECT
		WriteString(a_Reason);
	}
}




void cProtocolRecognizer::SendEditSign(int a_BlockX, int a_BlockY, int a_BlockZ)
{
	ASSERT(m_Protocol != NULL);
	m_Protocol->SendEditSign(a_BlockX, a_BlockY, a_BlockZ);
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
	m_Protocol->SendExplosion(a_BlockX,a_BlockY,a_BlockZ,a_Radius, a_BlocksAffected, a_PlayerMotion);
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





void cProtocolRecognizer::SendWindowProperty(const cWindow & a_Window, short a_Property, short a_Value)
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





void cProtocolRecognizer::SendPickupSpawn(const cPickup & a_Pickup)
{
	ASSERT(m_Protocol != NULL);
	m_Protocol->SendPickupSpawn(a_Pickup);
}





void cProtocolRecognizer::SendPlayerAnimation(const cPlayer & a_Player, char a_Animation)
{
	ASSERT(m_Protocol != NULL);
	m_Protocol->SendPlayerAnimation(a_Player, a_Animation);
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





void cProtocolRecognizer::SendRespawn(void)
{
	ASSERT(m_Protocol != NULL);
	m_Protocol->SendRespawn();
}





void cProtocolRecognizer::SendSoundEffect(const AString & a_SoundName, int a_SrcX, int a_SrcY, int a_SrcZ, float a_Volume, float a_Pitch)
{
	ASSERT(m_Protocol != NULL);
	m_Protocol->SendSoundEffect(a_SoundName, a_SrcX, a_SrcY, a_SrcZ, a_Volume, a_Pitch);
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





void cProtocolRecognizer::SendUpdateSign(int a_BlockX, int a_BlockY, int a_BlockZ, const AString & a_Line1, const AString & a_Line2, const AString & a_Line3, const AString & a_Line4)
{
	ASSERT(m_Protocol != NULL);
	m_Protocol->SendUpdateSign(a_BlockX, a_BlockY, a_BlockZ, a_Line1, a_Line2, a_Line3, a_Line4);
}





void cProtocolRecognizer::SendUseBed(const cEntity & a_Entity, int a_BlockX, int a_BlockY, int a_BlockZ )
{
	ASSERT(m_Protocol != NULL);
	m_Protocol->SendUseBed(a_Entity, a_BlockX, a_BlockY, a_BlockZ);
}





void cProtocolRecognizer::SendWeather(eWeather a_Weather)
{
	ASSERT(m_Protocol != NULL);
	m_Protocol->SendWeather(a_Weather);
}





void cProtocolRecognizer::SendWholeInventory(const cInventory & a_Inventory)
{
	ASSERT(m_Protocol != NULL);
	m_Protocol->SendWholeInventory(a_Inventory);
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





void cProtocolRecognizer::SendWindowOpen(char a_WindowID, char a_WindowType, const AString & a_WindowTitle, char a_NumSlots)
{
	ASSERT(m_Protocol != NULL);
	m_Protocol->SendWindowOpen(a_WindowID, a_WindowType, a_WindowTitle, a_NumSlots);
}





AString cProtocolRecognizer::GetAuthServerID(void)
{
	ASSERT(m_Protocol != NULL);
	return m_Protocol->GetAuthServerID();
}





void cProtocolRecognizer::SendData(const char * a_Data, int a_Size)
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
		case 0x02: break;  // Handshake, continue recognizing
		case 0xfe: HandleServerPing(); return false;
		default: return false;
	}
	
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
		{
			m_Protocol = new cProtocol162(m_Client);
			return true;
		}
	}
	m_Protocol = new cProtocol125(m_Client);
	return true;
}





void cProtocolRecognizer::HandleServerPing(void)
{
	AString Reply;
	switch (cRoot::Get()->m_PrimaryServerVersion)
	{
		case PROTO_VERSION_1_2_5:
		case PROTO_VERSION_1_3_2:
		{
			// http://wiki.vg/wiki/index.php?title=Protocol&oldid=3099#Server_List_Ping_.280xFE.29
			Printf(Reply, "%s%s%i%s%i", 
				cRoot::Get()->GetServer()->GetDescription().c_str(),
				cChatColor::Delimiter.c_str(),
				cRoot::Get()->GetServer()->GetNumPlayers(),
				cChatColor::Delimiter.c_str(),
				cRoot::Get()->GetServer()->GetMaxPlayers()
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
		{
			// The server list ping now has 1 more byte of "magic". Mojang just loves to complicate stuff.
			// http://wiki.vg/wiki/index.php?title=Protocol&oldid=3101#Server_List_Ping_.280xFE.29
			// _X 2012_10_31: I know that this needn't eat the byte, since it still may be in transit.
			//    Who cares? We're disconnecting anyway.
			if (m_Buffer.CanReadBytes(1))
			{
				byte val;
				m_Buffer.ReadByte(val);
				ASSERT(val == 0x01);
			}
			
			// http://wiki.vg/wiki/index.php?title=Server_List_Ping&oldid=3100
			AString NumPlayers;
			Printf(NumPlayers, "%d", cRoot::Get()->GetServer()->GetNumPlayers());
			AString MaxPlayers;
			Printf(MaxPlayers, "%d", cRoot::Get()->GetServer()->GetMaxPlayers());
			
			AString ProtocolVersionNum;
			Printf(ProtocolVersionNum, "%d", cRoot::Get()->m_PrimaryServerVersion);
			AString ProtocolVersionTxt(GetVersionTextFromInt(cRoot::Get()->m_PrimaryServerVersion));

			// Cannot use Printf() because of in-string NUL bytes.
			Reply = cChatColor::Delimiter;
			Reply.append("1");
			Reply.push_back(0);
			Reply.append(ProtocolVersionNum);
			Reply.push_back(0);
			Reply.append(ProtocolVersionTxt);
			Reply.push_back(0);
			Reply.append(cRoot::Get()->GetServer()->GetDescription());
			Reply.push_back(0);
			Reply.append(NumPlayers);
			Reply.push_back(0);
			Reply.append(MaxPlayers);
			break;
		}
	}  // switch (m_PrimaryServerVersion)
	m_Client->Kick(Reply);
}




