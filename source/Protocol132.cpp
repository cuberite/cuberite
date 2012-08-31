
// Protocol132.cpp

// Implements the cProtocol132 class representing the release 1.3.2 protocol (#39)

#include "Globals.h"
#include "Protocol132.h"
#include "cRoot.h"
#include "cServer.h"
#include "cClientHandle.h"
#include "CryptoPP/osrng.h"
#include "cItem.h"
#include "ChunkDataSerializer.h"
#include "cPlayer.h"
#include "cMonster.h"





#define HANDLE_PACKET_READ(Proc, Type, Var) \
	Type Var; \
	{ \
		if (!m_ReceivedData.Proc(Var)) \
		{ \
			return PARSE_INCOMPLETE; \
		} \
	}




typedef unsigned char Byte;





using namespace CryptoPP;





const int MAX_ENC_LEN = 512;  // Maximum size of the encrypted message; should be 128, but who knows...





enum
{
	PACKET_KEEP_ALIVE       = 0x00,
	PACKET_LOGIN            = 0x01,
	PACKET_PLAYER_SPAWN     = 0x14,
	PACKET_SPAWN_MOB        = 0x18,
	PACKET_DESTROY_ENTITIES = 0x1d,
	PACKET_CHUNK_DATA       = 0x33,
	PACKET_BLOCK_CHANGE     = 0x34,
	PACKET_BLOCK_ACTION     = 0x36,
} ;





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cProtocol132:

cProtocol132::cProtocol132(cClientHandle * a_Client) :
	super(a_Client),
	// DEBUG:
	m_CurrentIn(0),
	m_CurrentOut(0),
	m_EncIn(0),
	m_EncOut(0),
	
	m_IsEncrypted(false)
{
}





cProtocol132::~cProtocol132()
{
	if (!m_DataToSend.empty())
	{
		LOGD("There are %d unsent bytes while deleting cProtocol132", m_DataToSend.size());
	}
}





void cProtocol132::DataReceived(const char * a_Data, int a_Size)
{
	m_CurrentIn += a_Size;
	if (m_IsEncrypted)
	{
		m_EncIn += a_Size;
		byte Decrypted[512];
		while (a_Size > 0)
		{
			int NumBytes = (a_Size > sizeof(Decrypted)) ? sizeof(Decrypted) : a_Size;
			m_Decryptor.ProcessData(Decrypted, (byte *)a_Data, NumBytes);
			super::DataReceived((const char *)Decrypted, NumBytes);
			a_Size -= NumBytes;
			a_Data += NumBytes;
		}
	}
	else
	{
		super::DataReceived(a_Data, a_Size);
	}
}





void cProtocol132::SendBlockAction(int a_BlockX, int a_BlockY, int a_BlockZ, char a_Byte1, char a_Byte2, BLOCKTYPE a_BlockType)
{
	cCSLock Lock(m_CSPacket);
	WriteByte (PACKET_BLOCK_ACTION);
	WriteInt  (a_BlockX);
	WriteShort((short)a_BlockY);
	WriteInt  (a_BlockZ);
	WriteByte (a_Byte1);
	WriteByte (a_Byte2);
	WriteShort(a_BlockType);
	Flush();
}





void cProtocol132::SendBlockChange(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta)
{
	cCSLock Lock(m_CSPacket);
	WriteByte (PACKET_BLOCK_CHANGE);
	WriteInt  (a_BlockX);
	WriteByte ((unsigned char)a_BlockY);
	WriteInt  (a_BlockZ);
	WriteShort(a_BlockType);
	WriteByte (a_BlockMeta);
	Flush();
}





void cProtocol132::SendChunkData(int a_ChunkX, int a_ChunkZ, cChunkDataSerializer & a_Serializer)
{
	cCSLock Lock(m_CSPacket);
	
	// Pre-chunk not used in 1.3.2. Finally.

	//*
	// Send the chunk data:
	// Chunk data seems to break the connection for some reason; without it, the connection lives indefinitely
	AString Serialized = a_Serializer.Serialize(cChunkDataSerializer::RELEASE_1_3_2);
	WriteByte(PACKET_CHUNK_DATA);
	WriteInt (a_ChunkX);
	WriteInt (a_ChunkZ);
	SendData(Serialized.data(), Serialized.size());
	Flush();
	//*/
}





void cProtocol132::SendDestroyEntity(const cEntity & a_Entity)
{
	cCSLock Lock(m_CSPacket);
	WriteByte(PACKET_DESTROY_ENTITIES);
	WriteByte(1);  // entity count
	WriteInt (a_Entity.GetUniqueID());
	Flush();
}





void cProtocol132::SendKeepAlive(int a_PingID)
{
	cCSLock Lock(m_CSPacket);
	WriteByte(PACKET_KEEP_ALIVE);
	WriteInt (a_PingID);
	Flush();
}





void cProtocol132::SendLogin(const cPlayer & a_Player)
{
	cCSLock Lock(m_CSPacket);
	WriteByte  (PACKET_LOGIN);
	WriteInt   (a_Player.GetUniqueID());  // EntityID of the player
	WriteString("default");  // Level type
	WriteByte  ((int)a_Player.GetGameMode());
	WriteByte  (0);  // TODO: Dimension (Nether / Overworld / End)
	WriteByte  (2);  // TODO: Difficulty
	WriteByte  (0);  // Unused, used to be world height
	WriteByte  (8);  // Client list width or something
	Flush();
}





void cProtocol132::SendPlayerSpawn(const cPlayer & a_Player)
{
	const cItem & HeldItem = a_Player.GetEquippedItem();
	cCSLock Lock(m_CSPacket);
	WriteByte	 (PACKET_PLAYER_SPAWN);
	WriteInt   (a_Player.GetUniqueID());
	WriteString(a_Player.GetName());
	WriteInt   ((int)(a_Player.GetPosX() * 32));
	WriteInt   ((int)(a_Player.GetPosY() * 32));
	WriteInt   ((int)(a_Player.GetPosZ() * 32));
	WriteByte  ((char)((a_Player.GetRot().x / 360.f) * 256));
	WriteByte  ((char)((a_Player.GetRot().y / 360.f) * 256));
	WriteShort (HeldItem.IsEmpty() ? 0 : HeldItem.m_ItemType);
	// Player metadata: just use a default metadata value, since the client doesn't like starting without any metadata:
	WriteByte  (0);  // Index 0, byte (flags)
	WriteByte  (0);  // Flags, empty
	WriteByte  (0x7f);  // End of metadata
	Flush();
}





void cProtocol132::SendSpawnMob(const cMonster & a_Mob)
{
	cCSLock Lock(m_CSPacket);
	WriteByte   (PACKET_SPAWN_MOB);
	WriteInt    (a_Mob.GetUniqueID());
	WriteByte   (a_Mob.GetMobType());
	WriteVectorI((Vector3i)(a_Mob.GetPosition() * 32));
	WriteByte   (0);  // yaw
	WriteByte   (0);  // pitch
	WriteByte   (0);  // head yaw
	WriteShort  (0);  // Velocity Z
	WriteShort  (0);  // Velocity X
	WriteShort  (0);  // Velocity Y
	AString MetaData = GetEntityMetaData(a_Mob);
	SendData (MetaData.data(), MetaData.size());
	Flush();
}





void cProtocol132::SendUnloadChunk(int a_ChunkX, int a_ChunkZ)
{
	// Not used in 1.3.2
	// Does it unload chunks on its own?
}





int cProtocol132::ParsePacket(unsigned char a_PacketType)
{
	// DEBUG:
	LOGD("Received packet 0x%02x, current bytecount %d, enc %d", a_PacketType, m_CurrentIn, m_EncIn);
	
	switch (a_PacketType)
	{
		default:   return super::ParsePacket(a_PacketType);  // off-load previously known packets into cProtocol125
		case 0xcc: return ParseLocaleViewDistance();
		case 0xcd: return ParseClientStatuses();
		case 0xfc: return ParseEncryptionKeyResponse();
	}
}





int cProtocol132::ParseBlockPlace(void)
{
	HANDLE_PACKET_READ(ReadBEInt, int,  PosX);
	HANDLE_PACKET_READ(ReadByte,  Byte, PosY);
	HANDLE_PACKET_READ(ReadBEInt, int,  PosZ);
	HANDLE_PACKET_READ(ReadChar,  char, Direction);

	cItem HeldItem;
	int res = ParseItem(HeldItem);
	if (res < 0)
	{
		return res;
	}
	
	HANDLE_PACKET_READ(ReadChar, char, CursorX);
	HANDLE_PACKET_READ(ReadChar, char, CursorY);
	HANDLE_PACKET_READ(ReadChar, char, CursorZ);

	m_Client->HandleBlockPlace(PosX, PosY, PosZ, Direction, HeldItem);
	return PARSE_OK;
}





int cProtocol132::ParseHandshake(void)
{
	HANDLE_PACKET_READ(ReadByte,            Byte,    ProtocolVersion);
	HANDLE_PACKET_READ(ReadBEUTF16String16, AString, Username);
	HANDLE_PACKET_READ(ReadBEUTF16String16, AString, ServerHost);
	HANDLE_PACKET_READ(ReadBEInt,           int,     ServerPort);
	m_Username = Username;

	AString key;
	CryptoPP::StringSink sink(key);  // GCC won't allow inline instantiation in the following line, damned temporary refs
	cRoot::Get()->GetServer()->GetPublicKey().Save(sink);
	
	// Send a 0xFD Encryption Key Request http://wiki.vg/Protocol#0xFD
	WriteByte((char)0xfd);
	WriteString("MCServer");
	WriteShort((short)key.size());
	SendData(key.data(), key.size());
	WriteShort(4);
	WriteInt((int)this);  // Using 'this' as the cryptographic nonce, so that we don't have to generate one each time :)
	Flush();
	return PARSE_OK;
}





int cProtocol132::ParseClientStatuses(void)
{
	HANDLE_PACKET_READ(ReadByte, byte, Status);
	m_Client->HandleLogin(39, m_Username);
	return PARSE_OK;
}





int cProtocol132::ParseEncryptionKeyResponse(void)
{
	HANDLE_PACKET_READ(ReadBEShort, short, EncKeyLength);
	AString EncKey;
	if (!m_ReceivedData.ReadString(EncKey, EncKeyLength))
	{
		return PARSE_INCOMPLETE;
	}
	HANDLE_PACKET_READ(ReadBEShort, short, EncNonceLength);
	AString EncNonce;
	if (!m_ReceivedData.ReadString(EncNonce, EncNonceLength))
	{
		return PARSE_INCOMPLETE;
	}
	if ((EncKeyLength > MAX_ENC_LEN) || (EncNonceLength > MAX_ENC_LEN))
	{
		LOGD("Too long encryption");
		m_Client->Kick("Hacked client");
		return PARSE_OK;
	}
	
	HandleEncryptionKeyResponse(EncKey, EncNonce);
	return PARSE_OK;
}





int cProtocol132::ParseLocaleViewDistance(void)
{
	HANDLE_PACKET_READ(ReadBEUTF16String16, AString, Locale);
	HANDLE_PACKET_READ(ReadChar,            char,    ViewDistance);
	HANDLE_PACKET_READ(ReadChar,            char,    ChatFlags);
	HANDLE_PACKET_READ(ReadChar,            char,    ClientDifficulty);
	// TODO: m_Client->HandleLocale(Locale);
	// TODO: m_Client->HandleViewDistance(ViewDistance);
	// TODO: m_Client->HandleChatFlags(ChatFlags);
	// Ignoring client difficulty
	return PARSE_OK;
}





int cProtocol132::ParseLogin(void)
{
	// Login packet not used in 1.3.2
	return PARSE_ERROR;
}





int cProtocol132::ParsePlayerAbilities(void)
{
	HANDLE_PACKET_READ(ReadBool, bool, Flags);
	HANDLE_PACKET_READ(ReadChar, char, FlyingSpeed);
	HANDLE_PACKET_READ(ReadChar, char, WalkingSpeed);
	// TODO: m_Client->HandlePlayerAbilities(...);
	return PARSE_OK;
}





void cProtocol132::SendData(const char * a_Data, int a_Size)
{
	// DEBUG:
	m_DataToSend.append(a_Data, a_Size);
	m_CurrentOut += a_Size;
}





void cProtocol132::Flush(void)
{
	// DEBUG
	if (m_DataToSend.empty())
	{
		LOGD("Flushing empty");
		return;
	}
	LOGD("Flushing packet 0x%02x, %d bytes; %d bytes out, %d enc out", (unsigned char)m_DataToSend[0], m_DataToSend.size(), m_CurrentOut, m_EncOut);
	const char * a_Data = m_DataToSend.data();
	int a_Size = m_DataToSend.size();
	if (m_IsEncrypted)
	{
		m_EncOut += a_Size;
		byte Encrypted[4096];  // Larger buffer, we may be sending lots of data (chunks)
		while (a_Size > 0)
		{
			int NumBytes = (a_Size > sizeof(Encrypted)) ? sizeof(Encrypted) : a_Size;
			m_Encryptor.ProcessData(Encrypted, (byte *)a_Data, NumBytes);
			super::SendData((const char *)Encrypted, NumBytes);
			a_Size -= NumBytes;
			a_Data += NumBytes;
		}
	}
	else
	{
		super::SendData(a_Data, a_Size);
	}
	m_DataToSend.clear();
	// cSleep::MilliSleep(300);
}





void cProtocol132::WriteItem(const cItem & a_Item)
{
	short ItemType = a_Item.m_ItemType;
	ASSERT(ItemType >= -1);  // Check validity of packets in debug runtime
	if (ItemType <= 0)
	{
		// Fix, to make sure no invalid values are sent.
		ItemType = -1;
	}
	
	WriteShort(ItemType);
	if (a_Item.IsEmpty())
	{
		return;
	}
	
	WriteByte (a_Item.m_ItemCount);
	WriteShort(a_Item.m_ItemDamage);
	
	// TODO: Implement enchantments
	WriteShort(-1);
}





void cProtocol132::HandleEncryptionKeyResponse(const AString & a_EncKey, const AString & a_EncNonce)
{
	// Decrypt EncNonce using privkey
	RSAES<PKCS1v15>::Decryptor rsaDecryptor(cRoot::Get()->GetServer()->GetPrivateKey());
	AutoSeededRandomPool rng;
	byte DecryptedNonce[MAX_ENC_LEN];
	DecodingResult res = rsaDecryptor.Decrypt(rng, (const byte *)a_EncNonce.data(), a_EncNonce.size(), DecryptedNonce);
	if (!res.isValidCoding || (res.messageLength != 4))
	{
		LOGD("Bad nonce length");
		m_Client->Kick("Hacked client");
		return;
	}
	if (ntohl(*((int *)DecryptedNonce)) != (unsigned)this)
	{
		LOGD("Bad nonce value");
		m_Client->Kick("Hacked client");
		return;
	}
	
	// Decrypt the symmetric encryption key using privkey:
	byte DecryptedKey[MAX_ENC_LEN];
	res = rsaDecryptor.Decrypt(rng, (const byte *)a_EncKey.data(), a_EncKey.size(), DecryptedKey);
	if (!res.isValidCoding || (res.messageLength != 16))
	{
		LOGD("Bad key length");
		m_Client->Kick("Hacked client");
		return;
	}
	
	// Send encryption key response:
	WriteByte((char)0xfc);
	WriteShort(0);
	WriteShort(0);
	Flush();
	
	StartEncryption(DecryptedKey);
	return;
}





void cProtocol132::StartEncryption(const byte * a_Key)
{
	m_Encryptor.SetKey(a_Key, 16, MakeParameters(Name::IV(), ConstByteArrayParameter(a_Key, 16))(Name::FeedbackSize(), 1));
	m_Decryptor.SetKey(a_Key, 16, MakeParameters(Name::IV(), ConstByteArrayParameter(a_Key, 16))(Name::FeedbackSize(), 1));
	m_IsEncrypted = true;
}




