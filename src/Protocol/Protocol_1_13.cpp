
// Protocol_1_13.cpp

/*
Implements the 1.13 protocol classes:
- release 1.13 protocol (#393)
*/

#include "Globals.h"
#include "Protocol_1_13.h"
#include "ProtocolRecognizer.h"
#include "ChunkDataSerializer.h"
#include "Packetizer.h"
#include "ProtocolPalettes.h"

#include "../Entities/Boat.h"
#include "../Entities/Minecart.h"
#include "../Entities/Pickup.h"
#include "../Entities/Player.h"
#include "../Entities/ItemFrame.h"
#include "../Entities/ArrowEntity.h"
#include "../Entities/FireworkEntity.h"
#include "../Entities/SplashPotionEntity.h"

#include "../BlockTypePalette.h"
#include "../ClientHandle.h"
#include "../Root.h"
#include "../Server.h"

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





cProtocol_1_13::cProtocol_1_13(cClientHandle * a_Client, const AString & a_ServerAddress, UInt16 a_ServerPort, UInt32 a_State) :
	Super(a_Client, a_ServerAddress, a_ServerPort, a_State)
{
}





void cProtocol_1_13::Initialize(cClientHandle & a_Client)
{
	// Get the palettes; fail if not available:
	auto paletteVersion = this->GetPaletteVersion();
	m_BlockTypePalette = cRoot::Get()->GetProtocolPalettes().blockTypePalette(paletteVersion);
	if (m_BlockTypePalette == nullptr)
	{
		throw std::runtime_error(Printf("This server doesn't support protocol %s.", paletteVersion));
	}

	// Process the palette into the temporary BLOCKTYPE -> NetBlockID map:
	auto upg = cRoot::Get()->GetUpgradeBlockTypePalette();
	m_BlockTypeMap = m_BlockTypePalette->createTransformMapWithFallback(upg, 0);
}





UInt32 cProtocol_1_13::GetPacketID(ePacketType a_PacketType)
{
	switch (a_PacketType)
	{
		case pktAttachEntity:         return 0x40;
		case pktBlockChanges:         return 0x0f;
		case pktCameraSetTo:          return 0x3c;
		case pktChatRaw:              return 0x0e;
		case pktCollectEntity:        return 0x4f;
		case pktDestroyEntity:        return 0x35;
		case pktDisconnectDuringGame: return 0x1b;
		case pktEditSign:             return 0x2c;
		case pktEntityEffect:         return 0x53;
		case pktEntityEquipment:      return 0x42;
		case pktEntityHeadLook:       return 0x39;
		case pktEntityLook:           return 0x2a;
		case pktEntityMeta:           return 0x3f;
		case pktEntityProperties:     return 0x52;
		case pktEntityRelMove:        return 0x28;
		case pktEntityRelMoveLook:    return 0x29;
		case pktEntityStatus:         return 0x1c;
		case pktEntityVelocity:       return 0x41;
		case pktExperience:           return 0x43;
		case pktExplosion:            return 0x1e;
		case pktGameMode:             return 0x20;
		case pktHeldItemChange:       return 0x3d;
		case pktInventorySlot:        return 0x17;
		case pktJoinGame:             return 0x25;
		case pktKeepAlive:            return 0x21;
		case pktMapData:              return 0x26;
		case pktParticleEffect:       return 0x24;
		case pktPlayerAbilities:      return 0x2e;
		case pktPlayerList:           return 0x30;
		case pktPlayerMaxSpeed:       return 0x52;
		case pktPlayerMoveLook:       return 0x32;
		case pktPluginMessage:        return 0x19;
		case pktRemoveEntityEffect:   return 0x36;
		case pktRespawn:              return 0x38;
		case pktScoreboardObjective:  return 0x45;
		case pktSoundEffect:          return 0x1a;
		case pktSoundParticleEffect:  return 0x23;
		case pktSpawnPosition:        return 0x49;
		case pktTabCompletionResults: return 0x10;
		case pktTeleportEntity:       return 0x50;
		case pktTimeUpdate:           return 0x4a;
		case pktTitle:                return 0x4b;
		case pktUnloadChunk:          return 0x1f;
		case pktUpdateHealth:         return 0x44;
		case pktUpdateScore:          return 0x48;
		case pktUpdateSign:           return GetPacketID(pktUpdateBlockEntity);
		case pktUseBed:               return 0x33;
		case pktWindowClose:          return 0x13;
		case pktWindowItems:          return 0x15;
		case pktWindowOpen:           return 0x14;
		case pktWindowProperty:       return 0x16;
		default: return Super::GetPacketID(a_PacketType);
	}
}





AString cProtocol_1_13::GetPaletteVersion() const
{
	return "1.13";
}





bool cProtocol_1_13::HandlePacket(cByteBuffer & a_ByteBuffer, UInt32 a_PacketType)
{
	if (m_State != 3)
	{
		return Super::HandlePacket(a_ByteBuffer, a_PacketType);
	}

	// Game
	switch (a_PacketType)
	{
		case 0x00: HandleConfirmTeleport(a_ByteBuffer); return true;
		case 0x05: HandlePacketTabComplete(a_ByteBuffer); return true;
		case 0x02: HandlePacketChatMessage(a_ByteBuffer); return true;
		case 0x03: HandlePacketClientStatus(a_ByteBuffer); return true;
		case 0x04: HandlePacketClientSettings(a_ByteBuffer); return true;
		case 0x06: break;  // Confirm transaction - not used in Cuberite
		case 0x07: HandlePacketEnchantItem(a_ByteBuffer); return true;
		case 0x08: HandlePacketWindowClick(a_ByteBuffer); return true;
		case 0x09: HandlePacketWindowClose(a_ByteBuffer); return true;
		case 0x0a: HandlePacketPluginMessage(a_ByteBuffer); return true;
		case 0x0d: HandlePacketUseEntity(a_ByteBuffer); return true;
		case 0x0e: HandlePacketKeepAlive(a_ByteBuffer); return true;
		case 0x0f: HandlePacketPlayer(a_ByteBuffer); return true;
		case 0x10: HandlePacketPlayerPos(a_ByteBuffer); return true;
		case 0x11: HandlePacketPlayerPosLook(a_ByteBuffer); return true;
		case 0x12: HandlePacketPlayerLook(a_ByteBuffer); return true;
		case 0x13: HandlePacketVehicleMove(a_ByteBuffer); return true;
		case 0x14: HandlePacketBoatSteer(a_ByteBuffer); return true;
		case 0x15: break;  // Pick item - not yet implemented
		case 0x16: break;  // Craft Recipe Request - not yet implemented
		case 0x17: HandlePacketPlayerAbilities(a_ByteBuffer); return true;
		case 0x18: HandlePacketBlockDig(a_ByteBuffer); return true;
		case 0x19: HandlePacketEntityAction(a_ByteBuffer); return true;
		case 0x1a: HandlePacketSteerVehicle(a_ByteBuffer); return true;
		case 0x1b: HandlePacketCraftingBookData(a_ByteBuffer); return true;
		case 0x1d: break;  // Resource pack status - not yet implemented
		case 0x1e: HandlePacketAdvancementTab(a_ByteBuffer); return true;
		case 0x21: HandlePacketSlotSelect(a_ByteBuffer); return true;
		case 0x24: HandlePacketCreativeInventoryAction(a_ByteBuffer); return true;
		case 0x26: HandlePacketUpdateSign(a_ByteBuffer); return true;
		case 0x27: HandlePacketAnimation(a_ByteBuffer); return true;
		case 0x28: HandlePacketSpectate(a_ByteBuffer); return true;
		case 0x29: HandlePacketBlockPlace(a_ByteBuffer); return true;
		case 0x2a: HandlePacketUseItem(a_ByteBuffer); return true;
	}

	return Super::HandlePacket(a_ByteBuffer, a_PacketType);
}





void cProtocol_1_13::HandlePacketStatusRequest(cByteBuffer & a_ByteBuffer)
{
	cServer * Server = cRoot::Get()->GetServer();
	AString ServerDescription = Server->GetDescription();
	auto NumPlayers = static_cast<signed>(Server->GetNumPlayers());
	auto MaxPlayers = static_cast<signed>(Server->GetMaxPlayers());
	AString Favicon = Server->GetFaviconData();
	cRoot::Get()->GetPluginManager()->CallHookServerPing(*m_Client, ServerDescription, NumPlayers, MaxPlayers, Favicon);

	// Version:
	Json::Value Version;
	Version["name"] = "Cuberite 1.13";
	Version["protocol"] = cProtocolRecognizer::PROTO_VERSION_1_13;

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
	Json::FastWriter Writer;
	cPacketizer Pkt(*this, pktStatusResponse);
	Pkt.WriteString(Writer.write(ResponseValue));
}





void cProtocol_1_13::HandlePacketPluginMessage(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadVarUTF8String, AString, Channel);

	// If the plugin channel is recognized vanilla, handle it directly:
	if (Channel.substr(0, 15) == "minecraft:brand")
	{
		HANDLE_READ(a_ByteBuffer, ReadVarUTF8String, AString, Brand);
		m_Client->SetClientBrand(Brand);

		// Send back our brand, including the length:
		SendPluginMessage("minecraft:brand", "\x08""Cuberite");
		return;
	}

	// Read the plugin message and relay to clienthandle:
	AString Data;
	VERIFY(a_ByteBuffer.ReadString(Data, a_ByteBuffer.GetReadableSpace() - 1));  // Always succeeds
	m_Client->HandlePluginMessage(Channel, Data);
}





void cProtocol_1_13::SendBlockChange(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktBlockChange);
	Pkt.WritePosition64(a_BlockX, a_BlockY, a_BlockZ);
	Pkt.WriteVarInt32(static_cast<UInt32>(a_BlockType));  // TODO: Palette
}





void cProtocol_1_13::SendBlockChanges(int a_ChunkX, int a_ChunkZ, const sSetBlockVector & a_Changes)
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
		Pkt.WriteVarInt32(static_cast<UInt32>(itr->m_BlockType));  // TODO: Palette
	}  // for itr - a_Changes[]
}





void cProtocol_1_13::SendChunkData(int a_ChunkX, int a_ChunkZ, cChunkDataSerializer & a_Serializer)
{
	ASSERT(m_State == 3);  // In game mode?

	const AString & ChunkData = a_Serializer.Serialize(cChunkDataSerializer::RELEASE_1_13, a_ChunkX, a_ChunkZ, m_BlockTypeMap);
	cCSLock Lock(m_CSPacket);
	SendData(ChunkData.data(), ChunkData.size());
}





void cProtocol_1_13::SendMapData(const cMap & a_Map, int a_DataStartX, int a_DataStartY)
{
	// TODO
}





void cProtocol_1_13::SendPaintingSpawn(const cPainting & a_Painting)
{
	// TODO
}





void cProtocol_1_13::SendParticleEffect(const AString & a_ParticleName, float a_SrcX, float a_SrcY, float a_SrcZ, float a_OffsetX, float a_OffsetY, float a_OffsetZ, float a_ParticleData, int a_ParticleAmount)
{
	// TODO
}





void cProtocol_1_13::SendParticleEffect(const AString & a_ParticleName, Vector3f a_Src, Vector3f a_Offset, float a_ParticleData, int a_ParticleAmount, std::array<int, 2> a_Data)
{
	// TODO
}





void cProtocol_1_13::SendPluginMessage(const AString & a_Channel, const AString & a_Message)
{
	// TODO
}





void cProtocol_1_13::SendScoreboardObjective(const AString & a_Name, const AString & a_DisplayName, Byte a_Mode)
{
	// TODO
}





void cProtocol_1_13::SendSoundEffect(const AString & a_SoundName, double a_X, double a_Y, double a_Z, float a_Volume, float a_Pitch)
{
	// TODO
}





void cProtocol_1_13::SendStatistics(const cStatManager & a_Manager)
{
	// TODO
}





void cProtocol_1_13::SendTabCompletionResults(const AStringVector & a_Results)
{
	// TODO
}





void cProtocol_1_13::SendUpdateBlockEntity(cBlockEntity & a_BlockEntity)
{
	// TODO
}





bool cProtocol_1_13::ReadItem(cByteBuffer & a_ByteBuffer, cItem & a_Item, size_t a_KeepRemainingBytes)
{
	HANDLE_PACKET_READ(a_ByteBuffer, ReadBEInt16, Int16, ItemType);
	if (ItemType == -1)
	{
		// The item is empty, no more data follows
		a_Item.Empty();
		return true;
	}
	a_Item.m_ItemType = ItemType;

	HANDLE_PACKET_READ(a_ByteBuffer, ReadBEInt8, Int8, ItemCount);
	a_Item.m_ItemCount = ItemCount;
	a_Item.m_ItemDamage = 0;  // o no, no more damage in 1.13
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





void cProtocol_1_13::WriteItem(cPacketizer & a_Pkt, const cItem & a_Item)
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

	// Normal item
	// TODO: use new item ids
	a_Pkt.WriteBEInt16(ItemType);
	a_Pkt.WriteBEInt8(a_Item.m_ItemCount);

	// TODO: NBT
	a_Pkt.WriteBEInt8(0);
}





void cProtocol_1_13::WriteEntityMetadata(cPacketizer & a_Pkt, const cEntity & a_Entity)
{
	// TODO: Investigate
}
