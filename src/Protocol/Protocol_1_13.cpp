
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

#include "../Entities/Boat.h"
#include "../Entities/Minecart.h"
#include "../Entities/Pickup.h"
#include "../Entities/Player.h"
#include "../Entities/ItemFrame.h"
#include "../Entities/ArrowEntity.h"
#include "../Entities/FireworkEntity.h"
#include "../Entities/SplashPotionEntity.h"

#include "../Root.h"
#include "../Server.h"
#include "../ClientHandle.h"
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





cProtocol_1_13::cProtocol_1_13(cClientHandle * a_Client, const AString & a_ServerAddress, UInt16 a_ServerPort, UInt32 a_State) :
	super(a_Client, a_ServerAddress, a_ServerPort, a_State)
{
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
	cPacketizer Pkt(*this, 0x00);  // Response packet
	Pkt.WriteString(Writer.write(ResponseValue));
}





UInt32 cProtocol_1_13::GetPacketId(eOutgoingPackets a_Packet)
{
	switch (a_Packet)
	{
		case sendChatRaw: return 0x0E;
		case sendBlockChanges: return 0x0F;
		case sendInventorySlot: return 0x17;
		case sendUnloadChunk: return 0x1F;
		case sendGameMode: return 0x20;
		case sendDisconnectDuringGame: return 0x1B;
		case sendEntityStatus: return 0x1C;
		case sendKeepAlive: return 0x21;
		case sendJoinGame: return 0x25;
		case sendEntityRelMove: return 0x28;
		case sendEntityRelMoveLook: return 0x29;
		case sendEntityLook: return 0x2A;
		case sendPlayerAbilities: return 0x2E;
		case sendPlayerList: return 0x30;
		case sendPlayerMoveLook: return 0x32;
		case sendDestroyEntity: return 0x35;
		case sendRespawn: return 0x38;
		case sendEntityHeadLook: return 0x39;
		case sendSpawnPosition: return 0x49;
		case sendEntityProperties: return 0x52;
		case sendPlayerMaxSpeed: return 0x52;
		case sendMapData: return 0x26;
		case sendTimeUpdate: return 0x4A;
		case sendWindowItems: return 0x15;
		case sendHealth: return 0x44;
		case sendExperience: return 0x43;
		case sendHeldItemChange: return 0x3D;
		case sendTeleportEntity: return 0x50;
		case sendEntityVelocity: return 0x41;
		case sendEntityMeta: return 0x3F;
		case sendSoundEffect: return 0x1A;
		case sendSoundParticleEffect: return 0x23;
		case sendPluginMessage: return 0x19;
		case sendCollectEntity: return 0x4F;
		case sendEntityEffect: return 0x53;
		default: return super::GetPacketId(a_Packet);
	}
}





bool cProtocol_1_13::HandlePacket(cByteBuffer & a_ByteBuffer, UInt32 a_PacketType)
{
	if (m_State != 3)
	{
		return super::HandlePacket(a_ByteBuffer, a_PacketType);
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
		case 0x0A: HandlePacketPluginMessage(a_ByteBuffer); return true;
		case 0x0D: HandlePacketUseEntity(a_ByteBuffer); return true;
		case 0x0E: HandlePacketKeepAlive(a_ByteBuffer); return true;
		case 0x0F: HandlePacketPlayer(a_ByteBuffer); return true;
		case 0x10: HandlePacketPlayerPos(a_ByteBuffer); return true;
		case 0x11: HandlePacketPlayerPosLook(a_ByteBuffer); return true;
		case 0x12: HandlePacketPlayerLook(a_ByteBuffer); return true;
		case 0x13: HandlePacketVehicleMove(a_ByteBuffer); return true;
		case 0x14: HandlePacketBoatSteer(a_ByteBuffer); return true;
		case 0x15: break;
		case 0x16: break;  // Craft Recipe Request - not yet implemented
		case 0x17: HandlePacketPlayerAbilities(a_ByteBuffer); return true;
		case 0x18: HandlePacketBlockDig(a_ByteBuffer); return true;
		case 0x19: HandlePacketEntityAction(a_ByteBuffer); return true;
		case 0x1A: HandlePacketSteerVehicle(a_ByteBuffer); return true;
		case 0x1B: HandlePacketCraftingBookData(a_ByteBuffer); return true;
		case 0x1D: break;  // Resource pack status - not yet implemented
		case 0x1E: HandlePacketAdvancementTab(a_ByteBuffer); return true;
		case 0x21: HandlePacketSlotSelect(a_ByteBuffer); return true;
		case 0x24: HandlePacketCreativeInventoryAction(a_ByteBuffer); return true;
		case 0x26: HandlePacketUpdateSign(a_ByteBuffer); return true;
		case 0x27: HandlePacketAnimation(a_ByteBuffer); return true;
		case 0x28: HandlePacketSpectate(a_ByteBuffer); return true;
		case 0x29: HandlePacketBlockPlace(a_ByteBuffer); return true;
		case 0x2A: HandlePacketUseItem(a_ByteBuffer); return true;
	}

	return super::HandlePacket(a_ByteBuffer, a_PacketType);
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





void cProtocol_1_13::SendStatistics(const cStatManager & a_Manager)
{
	ASSERT(m_State == 3);  // In game mode?
	cPacketizer Pkt(*this, GetPacketId(sendStatistics));  // Statistics packet
	Pkt.WriteVarInt32(1);  // TODO 2014-05-11 xdot: Optimization: Send "dirty" statistics only

	Pkt.WriteVarInt32(8);
	Pkt.WriteVarInt32(0);
	Pkt.WriteVarInt32(1000);
}





void cProtocol_1_13::SendChunkData(int a_ChunkX, int a_ChunkZ, cChunkDataSerializer & a_Serializer)
{
	ASSERT(m_State == 3);  // In game mode?

	// Serialize first, before creating the Packetizer (the packetizer locks a CS)
	// This contains the flags and bitmasks, too
	const AString & ChunkData = a_Serializer.Serialize(cChunkDataSerializer::RELEASE_1_13, a_ChunkX, a_ChunkZ);

	cCSLock Lock(m_CSPacket);
	SendData(ChunkData.data(), ChunkData.size());
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





void cProtocol_1_13::SendSoundEffect(const AString & a_SoundName, double a_X, double a_Y, double a_Z, float a_Volume, float a_Pitch)
{
	// TODO: Investigate
	return;

	/*
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, GetPacketId(sendSoundEffect));  // Named sound effect packet
	Pkt.WriteString(a_SoundName);
	Pkt.WriteVarInt32(0);  // Master sound category (may want to be changed to a parameter later)
	Pkt.WriteBEInt32(static_cast<Int32>(a_X * 8.0));
	Pkt.WriteBEInt32(static_cast<Int32>(a_Y * 8.0));
	Pkt.WriteBEInt32(static_cast<Int32>(a_Z * 8.0));
	Pkt.WriteBEFloat(a_Volume);
	Pkt.WriteBEUInt8(static_cast<Byte>(a_Pitch * 63));
	*/
}






void cProtocol_1_13::SendBlockChange(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, GetPacketId(sendBlockChange));  // Block Change packet
	Pkt.WritePosition64(a_BlockX, a_BlockY, a_BlockZ);
	Pkt.WriteVarInt32(static_cast<UInt32>(a_BlockType));  // How's this for a global palette??
}





void cProtocol_1_13::SendBlockChanges(int a_ChunkX, int a_ChunkZ, const sSetBlockVector & a_Changes)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, GetPacketId(sendBlockChanges));  // Multi Block Change packet
	Pkt.WriteBEInt32(a_ChunkX);
	Pkt.WriteBEInt32(a_ChunkZ);
	Pkt.WriteVarInt32(static_cast<UInt32>(a_Changes.size()));
	for (sSetBlockVector::const_iterator itr = a_Changes.begin(), end = a_Changes.end(); itr != end; ++itr)
	{
		Int16 Coords = static_cast<Int16>(itr->m_RelY | (itr->m_RelZ << 8) | (itr->m_RelX << 12));
		Pkt.WriteBEInt16(Coords);
		Pkt.WriteVarInt32(static_cast<UInt32>(itr->m_BlockType));
	}  // for itr - a_Changes[]
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





void cProtocol_1_13::WriteEntityMetadata(cPacketizer & a_Pkt, const cEntity & a_Entity)
{
	// TODO: Investigate
}
