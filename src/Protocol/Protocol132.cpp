
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
#include "../WorldStorage/EnchantmentSerializer.h"
#include "../StringCompression.h"
#include "PolarSSL++/Sha1Checksum.h"





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





////////////////////////////////////////////////////////////////////////////////
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
		LOGD("There are " SIZE_T_FMT " unsent bytes while deleting cProtocol132", m_DataToSend.size());
	}
}





void cProtocol132::DataReceived(const char * a_Data, size_t a_Size)
{
	if (m_IsEncrypted)
	{
		Byte Decrypted[512];
		while (a_Size > 0)
		{
			size_t NumBytes = (a_Size > sizeof(Decrypted)) ? sizeof(Decrypted) : a_Size;
			m_Decryptor.ProcessData(Decrypted, (Byte *)a_Data, NumBytes);
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
	WriteChar (a_Byte1);
	WriteChar (a_Byte2);
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
	WriteChar   (stage);
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





void cProtocol132::SendCollectEntity(const cEntity & a_Entity, const cPlayer & a_Player)
{
	cCSLock Lock(m_CSPacket);
	WriteByte(PACKET_COLLECT_PICKUP);
	WriteInt (a_Entity.GetUniqueID());
	WriteInt (a_Player.GetUniqueID());
	Flush();
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
	WriteByte  ((Byte)a_Player.GetGameMode());
	WriteByte  ((Byte)(a_World.GetDimension()));
	WriteByte  (2);  // TODO: Difficulty
	WriteByte  (0);  // Unused, used to be world height
	WriteByte  (8);  // Client list width or something
	Flush();
	m_LastSentDimension = a_World.GetDimension();
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
	WriteChar  ((char)((a_Player.GetYaw()   / 360.f) * 256));
	WriteChar  ((char)((a_Player.GetPitch() / 360.f) * 256));
	WriteShort (HeldItem.IsEmpty() ? 0 : HeldItem.m_ItemType);
	// Player metadata: just use a default metadata value, since the client doesn't like starting without any metadata:
	WriteByte  (0);  // Index 0, byte (flags)
	WriteByte  (0);  // Flags, empty
	WriteByte  (0x7f);  // End of metadata
	Flush();
}





void cProtocol132::SendSoundEffect(const AString & a_SoundName, double a_X, double a_Y, double a_Z, float a_Volume, float a_Pitch)
{
	cCSLock Lock(m_CSPacket);
	WriteByte   (PACKET_SOUND_EFFECT);
	WriteString (a_SoundName);
	WriteInt    ((int)(a_X * 8.0));
	WriteInt    ((int)(a_Y * 8.0));
	WriteInt    ((int)(a_Z * 8.0));
	WriteFloat  (a_Volume);
	WriteChar   ((char)(a_Pitch * 63.0f));
	Flush();
}





void cProtocol132::SendSoundParticleEffect(int a_EffectID, int a_SrcX, int a_SrcY, int a_SrcZ, int a_Data)
{
	cCSLock Lock(m_CSPacket);
	WriteByte(PACKET_SOUND_PARTICLE_EFFECT);
	WriteInt (a_EffectID);
	WriteInt (a_SrcX);
	WriteByte((Byte)a_SrcY);
	WriteInt (a_SrcZ);
	WriteInt (a_Data);
	Flush();
}





void cProtocol132::SendSpawnMob(const cMonster & a_Mob)
{
	cCSLock Lock(m_CSPacket);
	WriteByte   (PACKET_SPAWN_MOB);
	WriteInt    (a_Mob.GetUniqueID());
	WriteByte   ((Byte)a_Mob.GetMobType());
	WriteVectorI((Vector3i)(a_Mob.GetPosition() * 32));
	WriteByte   ((Byte)((a_Mob.GetYaw()     / 360.f) * 256));
	WriteByte   ((Byte)((a_Mob.GetPitch()   / 360.f) * 256));
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
	cPlayer * Player = m_Client->GetPlayer();
	const cInventory & Inventory = Player->GetInventory();
	int BaseOffset = a_Window.GetNumSlots() - (cInventory::invNumSlots - cInventory::invInventoryOffset);  // Number of non-inventory slots
	char WindowID = a_Window.GetWindowID();
	for (short i = 0; i < cInventory::invInventoryCount; i++)
	{
		SendInventorySlot(WindowID, BaseOffset + i, Inventory.GetInventorySlot(i));
	}  // for i - Inventory[]
	BaseOffset += cInventory::invInventoryCount;
	for (short i = 0; i < cInventory::invHotbarCount; i++)
	{
		SendInventorySlot(WindowID, BaseOffset + i, Inventory.GetHotbarSlot(i));
	}  // for i - Hotbar[]
	
	// Send even the item being dragged:
	SendInventorySlot(-1, -1, Player->GetDraggingItem());
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

	m_Client->HandleRightClick(PosX, PosY, PosZ, static_cast<eBlockFace>(BlockFace), CursorX, CursorY, CursorZ, HeldItem);
	return PARSE_OK;
}





int cProtocol132::ParseHandshake(void)
{
	HANDLE_PACKET_READ(ReadByte,            Byte,    ProtocolVersion);
	HANDLE_PACKET_READ(ReadBEUTF16String16, AString, Username);
	HANDLE_PACKET_READ(ReadBEUTF16String16, AString, ServerHost);
	HANDLE_PACKET_READ(ReadBEInt,           int,     ServerPort);
	m_Username = Username;

	if (!m_Client->HandleHandshake( m_Username))
	{
		return PARSE_OK;  // Player is not allowed into the server
	}

	// Send a 0xfd Encryption Key Request http://wiki.vg/Protocol#0xFD
	SendEncryptionKeyRequest();

	return PARSE_OK;
}





int cProtocol132::ParseClientStatuses(void)
{
	HANDLE_PACKET_READ(ReadByte, Byte, Status);
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
	// Read the encryption key:
	HANDLE_PACKET_READ(ReadBEShort, short, EncKeyLength);
	if (EncKeyLength > MAX_ENC_LEN)
	{
		LOGD("Too long encryption key");
		m_Client->Kick("Hacked client");
		return PARSE_OK;
	}
	AString EncKey;
	if (!m_ReceivedData.ReadString(EncKey, (size_t)EncKeyLength))
	{
		return PARSE_INCOMPLETE;
	}
	
	// Read the encryption nonce:
	HANDLE_PACKET_READ(ReadBEShort, short, EncNonceLength);
	AString EncNonce;
	if (!m_ReceivedData.ReadString(EncNonce, (size_t)EncNonceLength))
	{
		return PARSE_INCOMPLETE;
	}
	if (EncNonceLength > MAX_ENC_LEN)
	{
		LOGD("Too long encryption nonce");
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
	m_Client->SetLocale(Locale);
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





void cProtocol132::SendData(const char * a_Data, size_t a_Size)
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
	const char * Data = m_DataToSend.data();
	size_t Size = m_DataToSend.size();
	if (m_IsEncrypted)
	{
		Byte Encrypted[8192];  // Larger buffer, we may be sending lots of data (chunks)
		while (Size > 0)
		{
			size_t NumBytes = (Size > sizeof(Encrypted)) ? sizeof(Encrypted) : Size;
			m_Encryptor.ProcessData(Encrypted, (Byte *)Data, NumBytes);
			super::SendData((const char *)Encrypted, NumBytes);
			Size -= NumBytes;
			Data += NumBytes;
		}
	}
	else
	{
		super::SendData(Data, Size);
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
	WriteChar (a_Item.m_ItemCount);
	WriteShort(a_Item.m_ItemDamage);
	
	if (a_Item.m_Enchantments.IsEmpty())
	{
		WriteShort(-1);
		return;
	}

	// Send the enchantments:
	cFastNBTWriter Writer;
	const char * TagName = (a_Item.m_ItemType == E_ITEM_BOOK) ? "StoredEnchantments" : "ench";
	EnchantmentSerializer::WriteToNBTCompound(a_Item.m_Enchantments, Writer, TagName);
	Writer.Finish();
	AString Compressed;
	CompressStringGZIP(Writer.GetResult().data(), Writer.GetResult().size(), Compressed);
	WriteShort((short)Compressed.size());
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
	Metadata.resize((size_t)MetadataLength);
	if (!m_ReceivedData.ReadBuf((void *)Metadata.data(), (size_t)MetadataLength))
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
			EnchantmentSerializer::ParseFromNBT(a_Item.m_Enchantments, NBT, tag);
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
	cServer * Server = cRoot::Get()->GetServer();
	WriteByte(0xfd);
	WriteString(Server->GetServerID());
	const AString & PublicKeyDER = Server->GetPublicKeyDER();
	WriteShort((short)(PublicKeyDER.size()));
	SendData(PublicKeyDER.data(), PublicKeyDER.size());
	WriteShort(4);
	WriteInt((int)(intptr_t)this);  // Using 'this' as the cryptographic nonce, so that we don't have to generate one each time :)
	Flush();
}





void cProtocol132::HandleEncryptionKeyResponse(const AString & a_EncKey, const AString & a_EncNonce)
{
	// Decrypt EncNonce using privkey
	cRsaPrivateKey & rsaDecryptor = cRoot::Get()->GetServer()->GetPrivateKey();

	Int32 DecryptedNonce[MAX_ENC_LEN / sizeof(Int32)];
	int res = rsaDecryptor.Decrypt((const Byte *)a_EncNonce.data(), a_EncNonce.size(), (Byte *)DecryptedNonce, sizeof(DecryptedNonce));
	if (res != 4)
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
	Byte DecryptedKey[MAX_ENC_LEN];
	res = rsaDecryptor.Decrypt((const Byte *)a_EncKey.data(), a_EncKey.size(), DecryptedKey, sizeof(DecryptedKey));
	if (res != 16)
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
	
	#ifdef _DEBUG
	AString DecryptedKeyHex;
	CreateHexDump(DecryptedKeyHex, DecryptedKey, res, 16);
	LOGD("Received encryption key, %d bytes:\n%s", res, DecryptedKeyHex.c_str());
	#endif
	
	StartEncryption(DecryptedKey);
	return;
}





void cProtocol132::StartEncryption(const Byte * a_Key)
{
	m_Encryptor.Init(a_Key, a_Key);
	m_Decryptor.Init(a_Key, a_Key);
	m_IsEncrypted = true;
	
	// Prepare the m_AuthServerID:
	cSha1Checksum Checksum;
	cServer * Server = cRoot::Get()->GetServer();
	AString ServerID = Server->GetServerID();
	Checksum.Update((const Byte *)ServerID.c_str(), ServerID.length());
	Checksum.Update(a_Key, 16);
	Checksum.Update((const Byte *)Server->GetPublicKeyDER().data(), Server->GetPublicKeyDER().size());
	Byte Digest[20];
	Checksum.Finalize(Digest);
	cSha1Checksum::DigestToJava(Digest, m_AuthServerID);
}




