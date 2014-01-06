
// Protocol132.cpp

// Implements the cProtocol132 class representing the release 1.3.2 protocol (#39)

#include "Globals.h"
#include "ChunkDataSerializer.h"
#include "Protocol132.h"
#include "../Root.h"
#include "../Server.h"
#include "../World.h"
#include "../ClientHandle.h"
#include "../Item.h"
#include "../Entities/Player.h"
#include "../Mobs/Monster.h"
#include "../UI/Window.h"
#include "../Entities/Pickup.h"
#include "../WorldStorage/FastNBT.h"
#include "../StringCompression.h"

#ifdef _MSC_VER
	#pragma warning(push)
	#pragma warning(disable:4127)
	#pragma warning(disable:4244)
	#pragma warning(disable:4231)
	#pragma warning(disable:4189)
	#pragma warning(disable:4702)
#endif

#include "cryptopp/randpool.h"

#ifdef _MSC_VER
	#pragma warning(pop)
#endif


#define HANDLE_PACKET_READ(Proc, Type, Var) \
	Type Var; \
	{ \
		if (!m_ReceivedData.Proc(Var)) \
		{ \
			m_ReceivedData.CheckValid(); \
			return PARSE_INCOMPLETE; \
		} \
		m_ReceivedData.CheckValid(); \
	}




typedef unsigned char Byte;





using namespace CryptoPP;





const int MAX_ENC_LEN = 512;  // Maximum size of the encrypted message; should be 128, but who knows...





enum
{
	PACKET_KEEP_ALIVE            = 0x00,
	PACKET_LOGIN                 = 0x01,
	PACKET_ENTITY_EQUIPMENT      = 0x05,
	PACKET_COMPASS               = 0x06,
	PACKET_PLAYER_SPAWN          = 0x14,
	PACKET_COLLECT_PICKUP        = 0x16,
	PACKET_SPAWN_MOB             = 0x18,
	PACKET_DESTROY_ENTITIES      = 0x1d,
	PACKET_CHUNK_DATA            = 0x33,
	PACKET_BLOCK_CHANGE          = 0x35,
	PACKET_BLOCK_ACTION          = 0x36,
	PACKET_BLOCK_BREAK_ANIM      = 0x37,
	PACKET_SOUND_EFFECT          = 0x3e,
	PACKET_SOUND_PARTICLE_EFFECT = 0x3d,
	PACKET_TAB_COMPLETION        = 0xcb,
	PACKET_LOCALE_VIEW_DISTANCE  = 0xcc,
	PACKET_CLIENT_STATUSES       = 0xcd,
	PACKET_ENCRYPTION_KEY_RESP   = 0xfc,
} ;





// Converts a raw 160-bit SHA1 digest into a Java Hex representation
// According to http://wiki.vg/wiki/index.php?title=Protocol_Encryption&oldid=2802
static void DigestToJava(byte a_Digest[20], AString & a_Out)
{
	bool IsNegative = (a_Digest[0] >= 0x80);
	if (IsNegative)
	{
		// Two's complement:
		bool carry = true;  // Add one to the whole number
		for (int i = 19; i >= 0; i--)
		{
			a_Digest[i] = ~a_Digest[i];
			if (carry)
			{
				carry = (a_Digest[i] == 0xff);
				a_Digest[i]++;
			}
		}
	}
	a_Out.clear();
	a_Out.reserve(40);
	for (int i = 0; i < 20; i++)
	{
		AppendPrintf(a_Out, "%02x", a_Digest[i]);
	}
	while ((a_Out.length() > 0) && (a_Out[0] == '0'))
	{
		a_Out.erase(0, 1);
	}
	if (IsNegative)
	{
		a_Out.insert(0, "-");
	}
}





/*
// Self-test the hash formatting for known values:
// sha1(Notch) : 4ed1f46bbe04bc756bcb17c0c7ce3e4632f06a48
// sha1(jeb_)  : -7c9d5b0044c130109a5d7b5fb5c317c02b4e28c1
// sha1(simon) : 88e16a1019277b15d58faf0541e11910eb756f6

class Test
{
public:
	Test(void)
	{
		AString DigestNotch, DigestJeb, DigestSimon;
		byte Digest[20];
		CryptoPP::SHA1 Checksum;
		Checksum.Update((const byte *)"Notch", 5);
		Checksum.Final(Digest);
		DigestToJava(Digest, DigestNotch);
		Checksum.Restart();
		Checksum.Update((const byte *)"jeb_", 4);
		Checksum.Final(Digest);
		DigestToJava(Digest, DigestJeb);
		Checksum.Restart();
		Checksum.Update((const byte *)"simon", 5);
		Checksum.Final(Digest);
		DigestToJava(Digest, DigestSimon);
		printf("Notch: \"%s\"", DigestNotch.c_str());
		printf("jeb_: \"%s\"",  DigestJeb.c_str());
		printf("simon: \"%s\"", DigestSimon.c_str());
	}
} test;
*/





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cProtocol132:

cProtocol132::cProtocol132(cClientHandle * a_Client) :
	super(a_Client),
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
	if (m_IsEncrypted)
	{
		byte Decrypted[512];
		while (a_Size > 0)
		{
			int NumBytes = (a_Size > (int)sizeof(Decrypted)) ? (int)sizeof(Decrypted) : a_Size;
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





void cProtocol132::SendBlockBreakAnim(int a_entityID, int a_BlockX, int a_BlockY, int a_BlockZ, char stage)
{
	cCSLock Lock(m_CSPacket);
	WriteByte   (PACKET_BLOCK_BREAK_ANIM);
	WriteInt    (a_entityID);
	WriteInt    (a_BlockX);
	WriteInt    (a_BlockY);
	WriteInt    (a_BlockZ);
	WriteByte   (stage);
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

	// Send the chunk data:
	AString Serialized = a_Serializer.Serialize(cChunkDataSerializer::RELEASE_1_3_2);
	WriteByte(PACKET_CHUNK_DATA);
	WriteInt (a_ChunkX);
	WriteInt (a_ChunkZ);
	SendData(Serialized.data(), Serialized.size());
	Flush();
}





void cProtocol132::SendCollectPickup(const cPickup & a_Pickup, const cPlayer & a_Player)
{
	cCSLock Lock(m_CSPacket);
	WriteByte(PACKET_COLLECT_PICKUP);
	WriteInt (a_Pickup.GetUniqueID());
	WriteInt (a_Player.GetUniqueID());
	Flush();
	
	// Also send the "pop" sound effect with a somewhat random pitch (fast-random using EntityID ;)
	SendSoundEffect(
		"random.pop",
		(int)(a_Pickup.GetPosX() * 8), (int)(a_Pickup.GetPosY() * 8), (int)(a_Pickup.GetPosZ() * 8),
		0.5, (float)(0.75 + ((float)((a_Pickup.GetUniqueID() * 23) % 32)) / 64)
	);
}





void cProtocol132::SendDestroyEntity(const cEntity & a_Entity)
{
	if (a_Entity.GetUniqueID() == m_Client->GetPlayer()->GetUniqueID())
	{
		// Do not send "destroy self" to the client, the client would crash (FS #254)
		return;
	}
	
	cCSLock Lock(m_CSPacket);
	WriteByte(PACKET_DESTROY_ENTITIES);
	WriteByte(1);  // entity count
	WriteInt (a_Entity.GetUniqueID());
	Flush();
}





void cProtocol132::SendEntityEquipment(const cEntity & a_Entity, short a_SlotNum, const cItem & a_Item)
{
	cCSLock Lock(m_CSPacket);
	WriteByte (PACKET_ENTITY_EQUIPMENT);
	WriteInt  (a_Entity.GetUniqueID());
	WriteShort(a_SlotNum);
	WriteItem (a_Item);
	Flush();
}





void cProtocol132::SendLogin(const cPlayer & a_Player, const cWorld & a_World)
{
	cCSLock Lock(m_CSPacket);
	WriteByte  (PACKET_LOGIN);
	WriteInt   (a_Player.GetUniqueID());  // EntityID of the player
	WriteString("default");  // Level type
	WriteByte  ((int)a_Player.GetGameMode());
	WriteByte  ((Byte)(a_World.GetDimension()));
	WriteByte  (2);  // TODO: Difficulty
	WriteByte  (0);  // Unused, used to be world height
	WriteByte  (8);  // Client list width or something
	Flush();
	
	SendCompass(a_World);
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





void cProtocol132::SendSoundEffect(const AString & a_SoundName, int a_SrcX, int a_SrcY, int a_SrcZ, float a_Volume, float a_Pitch)
{
	cCSLock Lock(m_CSPacket);
	WriteByte   (PACKET_SOUND_EFFECT);
	WriteString (a_SoundName);
	WriteInt    (a_SrcX);
	WriteInt    (a_SrcY);
	WriteInt    (a_SrcZ);
	WriteFloat  (a_Volume);
	WriteByte   ((char)(a_Pitch * 63.0f));
	Flush();
}





void cProtocol132::SendSoundParticleEffect(int a_EffectID, int a_SrcX, int a_SrcY, int a_SrcZ, int a_Data)
{
	cCSLock Lock(m_CSPacket);
	WriteByte(PACKET_SOUND_PARTICLE_EFFECT);
	WriteInt (a_EffectID);
	WriteInt (a_SrcX);
	WriteByte(a_SrcY);
	WriteInt (a_SrcZ);
	WriteInt (a_Data);
	Flush();
}





void cProtocol132::SendSpawnMob(const cMonster & a_Mob)
{
	cCSLock Lock(m_CSPacket);
	WriteByte   (PACKET_SPAWN_MOB);
	WriteInt    (a_Mob.GetUniqueID());
	WriteByte   (a_Mob.GetMobType());
	WriteVectorI((Vector3i)(a_Mob.GetPosition() * 32));
	WriteByte   ((Byte)((a_Mob.GetRotation() / 360.f) * 256));
	WriteByte   ((Byte)((a_Mob.GetPitch() / 360.f) * 256));
	WriteByte   ((Byte)((a_Mob.GetHeadYaw() / 360.f) * 256));
	WriteShort  ((short)(a_Mob.GetSpeedX() * 400));
	WriteShort  ((short)(a_Mob.GetSpeedY() * 400));
	WriteShort  ((short)(a_Mob.GetSpeedZ() * 400));

	WriteCommonMetadata(a_Mob);
	WriteMobMetadata(a_Mob);
	WriteByte(0x7f);

	Flush();
}





void cProtocol132::SendTabCompletionResults(const AStringVector & a_Results)
{
	if (a_Results.empty())
	{
		// No results to send
		return;
	}
	
	AString Serialized(a_Results[0]);
	for (AStringVector::const_iterator itr = a_Results.begin() + 1, end = a_Results.end(); itr != end; ++itr)
	{
		Serialized.push_back(0);
		Serialized.append(*itr);
	}  // for itr - a_Results[]
	
	cCSLock Lock(m_CSPacket);
	WriteByte(PACKET_TAB_COMPLETION);
	WriteString(Serialized);
	Flush();
}





void cProtocol132::SendUnloadChunk(int a_ChunkX, int a_ChunkZ)
{
	// Unloading the chunk is done by sending a "map chunk" packet
	// with IncludeInitialize set to true and primary bitmap set to 0:
	cCSLock Lock(m_CSPacket);
	WriteByte(PACKET_CHUNK_DATA);
	WriteInt (a_ChunkX);
	WriteInt (a_ChunkZ);
	WriteBool(true);      // IncludeInitialize
	WriteShort(0);        // Primary bitmap
	WriteShort(0);        // Add bitmap
	WriteInt(0);
	Flush();
}





void cProtocol132::SendWholeInventory(const cWindow & a_Window)
{
	// 1.3.2 requires player inventory slots to be sent as SetSlot packets, 
	// otherwise it sometimes fails to update the window
	
	// Send the entire window:
	super::SendWholeInventory(a_Window);
	
	// Send the player inventory and hotbar:
	const cInventory & Inventory = m_Client->GetPlayer()->GetInventory();
	int BaseOffset = a_Window.GetNumSlots() - (cInventory::invNumSlots - cInventory::invInventoryOffset);  // Number of non-inventory slots
	char WindowID = a_Window.GetWindowID();
	for (int i = 0; i < cInventory::invInventoryCount; i++)
	{
		SendInventorySlot(WindowID, BaseOffset + i, Inventory.GetInventorySlot(i));
	}  // for i - Inventory[]
	BaseOffset += cInventory::invInventoryCount;
	for (int i = 0; i < cInventory::invHotbarCount; i++)
	{
		SendInventorySlot(WindowID, BaseOffset + i, Inventory.GetHotbarSlot(i));
	}  // for i - Hotbar[]
	
	// Send even the item being dragged:
	SendInventorySlot(-1, -1, m_Client->GetPlayer()->GetDraggingItem());
}





AString cProtocol132::GetAuthServerID(void)
{
	// http://wiki.vg/wiki/index.php?title=Session&oldid=2615
	// Server uses SHA1 to mix ServerID, Client secret and server public key together
	// The mixing is done in StartEncryption, the result is in m_AuthServerID
	
	return m_AuthServerID;
}





int cProtocol132::ParsePacket(unsigned char a_PacketType)
{
	switch (a_PacketType)
	{
		default:                          return super::ParsePacket(a_PacketType);  // off-load previously known packets into cProtocol125
		case PACKET_CLIENT_STATUSES:      return ParseClientStatuses();
		case PACKET_ENCRYPTION_KEY_RESP:  return ParseEncryptionKeyResponse();
		case PACKET_LOCALE_VIEW_DISTANCE: return ParseLocaleViewDistance();
		case PACKET_TAB_COMPLETION:       return ParseTabCompletion();
	}
}





int cProtocol132::ParseBlockPlace(void)
{
	HANDLE_PACKET_READ(ReadBEInt, int,  PosX);
	HANDLE_PACKET_READ(ReadByte,  Byte, PosY);
	HANDLE_PACKET_READ(ReadBEInt, int,  PosZ);
	HANDLE_PACKET_READ(ReadChar,  char, BlockFace);

	cItem HeldItem;
	int res = ParseItem(HeldItem);
	if (res < 0)
	{
		return res;
	}
	
	HANDLE_PACKET_READ(ReadChar, char, CursorX);
	HANDLE_PACKET_READ(ReadChar, char, CursorY);
	HANDLE_PACKET_READ(ReadChar, char, CursorZ);

	m_Client->HandleRightClick(PosX, PosY, PosZ, BlockFace, CursorX, CursorY, CursorZ, HeldItem);
	return PARSE_OK;
}





int cProtocol132::ParseHandshake(void)
{
	HANDLE_PACKET_READ(ReadByte,            Byte,    ProtocolVersion);
	HANDLE_PACKET_READ(ReadBEUTF16String16, AString, Username);
	HANDLE_PACKET_READ(ReadBEUTF16String16, AString, ServerHost);
	HANDLE_PACKET_READ(ReadBEInt,           int,     ServerPort);
	m_Username = Username;

	if (!m_Client->HandleHandshake( m_Username ))
	{
		return PARSE_OK; // Player is not allowed into the server
	}

	// Send a 0xFD Encryption Key Request http://wiki.vg/Protocol#0xFD
	CryptoPP::StringSink sink(m_ServerPublicKey);  // GCC won't allow inline instantiation in the following line, damned temporary refs
	cRoot::Get()->GetServer()->GetPublicKey().Save(sink);
	SendEncryptionKeyRequest();

	return PARSE_OK;
}





int cProtocol132::ParseClientStatuses(void)
{
	HANDLE_PACKET_READ(ReadByte, byte, Status);
	if ((Status & 1) == 0)
	{
		m_Client->HandleLogin(39, m_Username);
	}
	else
	{
		m_Client->HandleRespawn();
	}
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





int cProtocol132::ParseTabCompletion(void)
{
	HANDLE_PACKET_READ(ReadBEUTF16String16, AString, Text);
	m_Client->HandleTabCompletion(Text);
	return PARSE_OK;
}





void cProtocol132::SendData(const char * a_Data, int a_Size)
{
	m_DataToSend.append(a_Data, a_Size);
}





void cProtocol132::Flush(void)
{
	ASSERT(m_CSPacket.IsLockedByCurrentThread());  // Did all packets lock the CS properly?
	
	if (m_DataToSend.empty())
	{
		LOGD("Flushing empty");
		return;
	}
	const char * a_Data = m_DataToSend.data();
	int a_Size = m_DataToSend.size();
	if (m_IsEncrypted)
	{
		byte Encrypted[8192];  // Larger buffer, we may be sending lots of data (chunks)
		while (a_Size > 0)
		{
			int NumBytes = (a_Size > (int)sizeof(Encrypted)) ? (int)sizeof(Encrypted) : a_Size;
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
	
	if (a_Item.IsEmpty())
	{
		WriteShort(-1);
		return;
	}
	
	WriteShort(ItemType);
	WriteByte (a_Item.m_ItemCount);
	WriteShort(a_Item.m_ItemDamage);
	
	if (a_Item.m_Enchantments.IsEmpty())
	{
		WriteShort(-1);
		return;
	}

	// Send the enchantments:
	cFastNBTWriter Writer;
	const char * TagName = (a_Item.m_ItemType == E_ITEM_BOOK) ? "StoredEnchantments" : "ench";
	a_Item.m_Enchantments.WriteToNBTCompound(Writer, TagName);
	Writer.Finish();
	AString Compressed;
	CompressStringGZIP(Writer.GetResult().data(), Writer.GetResult().size(), Compressed);
	WriteShort(Compressed.size());
	SendData(Compressed.data(), Compressed.size());
}





int cProtocol132::ParseItem(cItem & a_Item)
{
	HANDLE_PACKET_READ(ReadBEShort, short, ItemType);

	if (ItemType <= -1)
	{
		a_Item.Empty();
		return PARSE_OK;
	}
	a_Item.m_ItemType = ItemType;

	HANDLE_PACKET_READ(ReadChar,    char,  ItemCount);
	HANDLE_PACKET_READ(ReadBEShort, short, ItemDamage);
	a_Item.m_ItemCount  = ItemCount;
	a_Item.m_ItemDamage = ItemDamage;
	if (ItemCount <= 0)
	{
		a_Item.Empty();
	}

	HANDLE_PACKET_READ(ReadBEShort, short, MetadataLength);
	if (MetadataLength <= 0)
	{
		return PARSE_OK;
	}
		
	// Read the metadata
	AString Metadata;
	Metadata.resize(MetadataLength);
	if (!m_ReceivedData.ReadBuf((void *)Metadata.data(), MetadataLength))
	{
		return PARSE_INCOMPLETE;
	}
	
	return ParseItemMetadata(a_Item, Metadata);
}





int cProtocol132::ParseItemMetadata(cItem & a_Item, const AString & a_Metadata)
{
	// Uncompress the GZIPped data:
	AString Uncompressed;
	if (UncompressStringGZIP(a_Metadata.data(), a_Metadata.size(), Uncompressed) != Z_OK)
	{
		AString HexDump;
		CreateHexDump(HexDump, a_Metadata.data(), a_Metadata.size(), 16);
		LOG("Cannot unGZIP item metadata:\n%s", HexDump.c_str());
		return PARSE_ERROR;
	}
	
	// Parse into NBT:
	cParsedNBT NBT(Uncompressed.data(), Uncompressed.size());
	if (!NBT.IsValid())
	{
		AString HexDump;
		CreateHexDump(HexDump, Uncompressed.data(), Uncompressed.size(), 16);
		LOG("Cannot parse NBT item metadata:\n%s", HexDump.c_str());
		return PARSE_ERROR;
	}
	
	// Load enchantments from the NBT:
	for (int tag = NBT.GetFirstChild(NBT.GetRoot()); tag >= 0; tag = NBT.GetNextSibling(tag))
	{
		if (
			(NBT.GetType(tag) == TAG_List) &&
			(
				(NBT.GetName(tag) == "ench") ||
				(NBT.GetName(tag) == "StoredEnchantments")
			)
		)
		{
			a_Item.m_Enchantments.ParseFromNBT(NBT, tag);
		}
	}
	
	return PARSE_OK;
}





void cProtocol132::SendCompass(const cWorld & a_World)
{
	cCSLock Lock(m_CSPacket);
	WriteByte(PACKET_COMPASS);
	WriteInt((int)(a_World.GetSpawnX()));
	WriteInt((int)(a_World.GetSpawnY()));
	WriteInt((int)(a_World.GetSpawnZ()));
	Flush();
}





void cProtocol132::SendEncryptionKeyRequest(void)
{
	cCSLock Lock(m_CSPacket);
	WriteByte(0xfd);
	WriteString(cRoot::Get()->GetServer()->GetServerID());
	WriteShort((short)m_ServerPublicKey.size());
	SendData(m_ServerPublicKey.data(), m_ServerPublicKey.size());
	WriteShort(4);
	WriteInt((int)(intptr_t)this);  // Using 'this' as the cryptographic nonce, so that we don't have to generate one each time :)
	Flush();
}





void cProtocol132::HandleEncryptionKeyResponse(const AString & a_EncKey, const AString & a_EncNonce)
{
	// Decrypt EncNonce using privkey
	RSAES<PKCS1v15>::Decryptor rsaDecryptor(cRoot::Get()->GetServer()->GetPrivateKey());
	time_t CurTime = time(NULL);
	CryptoPP::RandomPool rng;
	rng.Put((const byte *)&CurTime, sizeof(CurTime));
	Int32 DecryptedNonce[MAX_ENC_LEN / sizeof(Int32)];
	DecodingResult res = rsaDecryptor.Decrypt(rng, (const byte *)a_EncNonce.data(), a_EncNonce.size(), (byte *)DecryptedNonce);
	if (!res.isValidCoding || (res.messageLength != 4))
	{
		LOGD("Bad nonce length");
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
	byte DecryptedKey[MAX_ENC_LEN];
	res = rsaDecryptor.Decrypt(rng, (const byte *)a_EncKey.data(), a_EncKey.size(), DecryptedKey);
	if (!res.isValidCoding || (res.messageLength != 16))
	{
		LOGD("Bad key length");
		m_Client->Kick("Hacked client");
		return;
	}
	
	{
		// Send encryption key response:
		cCSLock Lock(m_CSPacket);
		WriteByte(0xfc);
		WriteShort(0);
		WriteShort(0);
		Flush();
	}
	
	StartEncryption(DecryptedKey);
	return;
}





void cProtocol132::StartEncryption(const byte * a_Key)
{
	m_Encryptor.SetKey(a_Key, 16, MakeParameters(Name::IV(), ConstByteArrayParameter(a_Key, 16))(Name::FeedbackSize(), 1));
	m_Decryptor.SetKey(a_Key, 16, MakeParameters(Name::IV(), ConstByteArrayParameter(a_Key, 16))(Name::FeedbackSize(), 1));
	m_IsEncrypted = true;
	
	// Prepare the m_AuthServerID:
	CryptoPP::SHA1 Checksum;
	AString ServerID = cRoot::Get()->GetServer()->GetServerID();
	Checksum.Update((const byte *)ServerID.c_str(), ServerID.length());
	Checksum.Update(a_Key, 16);
	Checksum.Update((const byte *)m_ServerPublicKey.c_str(), m_ServerPublicKey.length());
	byte Digest[20];
	Checksum.Final(Digest);
	DigestToJava(Digest, m_AuthServerID);
}




