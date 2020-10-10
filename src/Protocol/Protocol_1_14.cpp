
// Protocol_1_14.cpp

/*
Implements the 1.14 protocol classes:
- release 1.14 protocol (#477)
*/

#include "Globals.h"
#include "Protocol_1_14.h"
#include "Packetizer.h"
#include "../Entities/Player.h"
#include "../Root.h"
#include "../Server.h"
#include "../World.h"

#include "Palettes/Upgrade.h"
#include "Palettes/Palette_1_14.h"





////////////////////////////////////////////////////////////////////////////////
// cProtocol_1_14:

void cProtocol_1_14::SendBlockAction(int a_BlockX, int a_BlockY, int a_BlockZ, char a_Byte1, char a_Byte2, BLOCKTYPE a_BlockType)
{
}





void cProtocol_1_14::SendBlockBreakAnim(UInt32 a_EntityID, int a_BlockX, int a_BlockY, int a_BlockZ, char a_Stage)
{
}





void cProtocol_1_14::SendEditSign(int a_BlockX, int a_BlockY, int a_BlockZ)
{
}





void cProtocol_1_14::SendLogin(const cPlayer & a_Player, const cWorld & a_World)
{
	// Send the Join Game packet:
	{
		cServer * Server = cRoot::Get()->GetServer();
		cPacketizer Pkt(*this, pktJoinGame);
		Pkt.WriteBEUInt32(a_Player.GetUniqueID());
		Pkt.WriteBEUInt8(static_cast<UInt8>(a_Player.GetEffectiveGameMode()) | (Server->IsHardcore() ? 0x08 : 0));
		Pkt.WriteBEInt32(static_cast<Int32>(a_World.GetDimension()));
		Pkt.WriteBEUInt8(static_cast<UInt8>(Clamp<size_t>(Server->GetMaxPlayers(), 0, 255)));
		Pkt.WriteString("default");
		Pkt.WriteVarInt32(ToUnsigned(a_World.GetMaxViewDistance()));
		Pkt.WriteBool(false);
	}

	// Send the spawn position:
	{
		cPacketizer Pkt(*this, pktSpawnPosition);
		Pkt.WriteXZYPosition64(FloorC(a_World.GetSpawnX()), FloorC(a_World.GetSpawnY()), FloorC(a_World.GetSpawnZ()));
	}

	// Send the server difficulty:
	{
		// cPacketizer Pkt(*this, pktDifficulty);
		// Pkt.WriteBEInt8(1);
	}

	// Send player abilities:
	SendPlayerAbilities();
}





void cProtocol_1_14::SendPaintingSpawn(const cPainting & a_Painting)
{
}





void cProtocol_1_14::SendSoundParticleEffect(const EffectID a_EffectID, int a_SrcX, int a_SrcY, int a_SrcZ, int a_Data)
{
}





void cProtocol_1_14::SendUpdateBlockEntity(cBlockEntity & a_BlockEntity)
{
}





void cProtocol_1_14::SendUpdateSign(int a_BlockX, int a_BlockY, int a_BlockZ, const AString & a_Line1, const AString & a_Line2, const AString & a_Line3, const AString & a_Line4)
{
}





void cProtocol_1_14::SendUseBed(const cEntity & a_Entity, int a_BlockX, int a_BlockY, int a_BlockZ)
{
}





UInt32 cProtocol_1_14::GetPacketID(ePacketType a_PacketType)
{
	switch (a_PacketType)
	{
		case cProtocol::pktAttachEntity:         return 0x4A;
		case cProtocol::pktCameraSetTo:          return 0x3E;
		case cProtocol::pktCollectEntity:        return 0x55;
		case cProtocol::pktDestroyEntity:        return 0x37;
		case cProtocol::pktDisconnectDuringGame: return 0x1A;
		case cProtocol::pktEntityEffect:         return 0x59;
		case cProtocol::pktEntityEquipment:      return 0x46;
		case cProtocol::pktEntityHeadLook:       return 0x3B;
		case cProtocol::pktEntityMeta:           return 0x43;
		case cProtocol::pktEntityProperties:     return 0x58;
		case cProtocol::pktEntityStatus:         return 0x1B;
		case cProtocol::pktEntityVelocity:       return 0x45;
		case cProtocol::pktExperience:           return 0x47;
		case cProtocol::pktExplosion:            return 0x1C;
		case cProtocol::pktGameMode:             return 0x1E;
		case cProtocol::pktHeldItemChange:       return 0x3F;
		case cProtocol::pktInventorySlot:        return 0x16;
		case cProtocol::pktKeepAlive:            return 0x20;
		case cProtocol::pktParticleEffect:       return 0x23;
		case cProtocol::pktPlayerAbilities:      return 0x31;
		case cProtocol::pktPlayerList:           return 0x33;
		case cProtocol::pktPlayerMoveLook:       return 0x35;
		case cProtocol::pktPluginMessage:        return 0x18;
		case cProtocol::pktRemoveEntityEffect:   return 0x38;
		case cProtocol::pktResourcePack:         return 0x39;
		case cProtocol::pktRespawn:              return 0x3A;
		case cProtocol::pktScoreboardObjective:  return 0x49;
		case cProtocol::pktSoundEffect:          return 0x19;
		case cProtocol::pktSoundParticleEffect:  return 0x22;
		case cProtocol::pktSpawnPosition:        return 0x4D;
		case cProtocol::pktTeleportEntity:       return 0x56;
		case cProtocol::pktTimeUpdate:           return 0x4E;
		case cProtocol::pktTitle:                return 0x4F;
		case cProtocol::pktUnloadChunk:          return 0x1D;
		case cProtocol::pktUnlockRecipe:         return 0x36;
		case cProtocol::pktUpdateHealth:         return 0x48;
		case cProtocol::pktUpdateScore:          return 0x4C;
		case cProtocol::pktUpdateSign:           return 0x2F;
		case cProtocol::pktWindowItems:          return 0x14;
		case cProtocol::pktWindowOpen:           return 0x2E;
		case cProtocol::pktWindowProperty:       return 0x15;
		default: return Super::GetPacketID(a_PacketType);
	}
}





cProtocol::Version cProtocol_1_14::GetProtocolVersion()
{
	return Version::v1_14;
}





std::pair<short, short> cProtocol_1_14::GetItemFromProtocolID(UInt32 a_ProtocolID)
{
	return PaletteUpgrade::ToItem(Palette_1_14::ToItem(a_ProtocolID));
}





UInt32 cProtocol_1_14::GetProtocolBlockType(BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta)
{
	return Palette_1_14::FromBlock(PaletteUpgrade::FromBlock(a_BlockType, a_Meta));
}





UInt32 cProtocol_1_14::GetProtocolItemType(short a_ItemID, short a_ItemDamage)
{
	return Palette_1_14::FromItem(PaletteUpgrade::FromItem(a_ItemID, a_ItemDamage));
}





UInt32 cProtocol_1_14::GetProtocolStatisticType(Statistic a_Statistic)
{
	return Palette_1_14::From(a_Statistic);
}





bool cProtocol_1_14::HandlePacket(cByteBuffer & a_ByteBuffer, UInt32 a_PacketType)
{
	if (m_State != State::Game)
	{
		return Super::HandlePacket(a_ByteBuffer, a_PacketType);
	}

	// Game
	switch (a_PacketType)
	{
		default: AString dum; a_ByteBuffer.ReadAll(dum); a_ByteBuffer.CommitRead(); a_ByteBuffer.Write(" ", 1);
			return true;
	}
}





void cProtocol_1_14::HandlePacketBlockDig(cByteBuffer & a_ByteBuffer)
{
}





void cProtocol_1_14::HandlePacketBlockPlace(cByteBuffer & a_ByteBuffer)
{
}





void cProtocol_1_14::HandlePacketUpdateSign(cByteBuffer & a_ByteBuffer)
{
}
