
// Packetizer.cpp

// Implements the cPacketizer class representing a wrapper for sending a single packet over a protocol.

#include "Globals.h"
#include "Packetizer.h"
#include "../UUID.h"





////////////////////////////////////////////////////////////////////////////////
// cPacketizer:

cPacketizer::~cPacketizer()
{
	m_Protocol.SendPacket(*this);
}





void cPacketizer::WriteByteAngle(double a_Angle)
{
	WriteBEInt8(static_cast<Int8>(255 * a_Angle / 360));
}





void cPacketizer::WriteFPInt(double a_Value)
{
	WriteBEInt32(static_cast<Int32>(a_Value * 32));
}





void cPacketizer::WriteUUID(const cUUID & a_UUID)
{
	for (auto val : a_UUID.ToRaw())
	{
		VERIFY(m_Out.WriteBEUInt8(val));
	}
}





AString cPacketizer::PacketTypeToStr(cProtocol::ePacketType a_PacketType)
{
	switch (a_PacketType)
	{
		case cProtocol::pktAttachEntity:          return "pktAttachEntity";
		case cProtocol::pktBlockAction:           return "pktBlockAction";
		case cProtocol::pktBlockBreakAnim:        return "pktBlockBreakAnim";
		case cProtocol::pktBlockChange:           return "pktBlockChange";
		case cProtocol::pktBlockChanges:          return "pktBlockChanges";
		case cProtocol::pktCameraSetTo:           return "pktCameraSetTo";
		case cProtocol::pktChatRaw:               return "pktChatRaw";
		case cProtocol::pktCollectEntity:         return "pktCollectEntity";
		case cProtocol::pktDestroyEntity:         return "pktDestroyEntity";
		case cProtocol::pktDifficulty:            return "pktDifficulty";
		case cProtocol::pktDisconnectDuringLogin: return "pktDisconnectDuringLogin";
		case cProtocol::pktDisconnectDuringGame:  return "pktDisconnectDuringGame";
		case cProtocol::pktDisplayObjective:      return "pktDisplayObjective";
		case cProtocol::pktEditSign:              return "pktEditSign";
		case cProtocol::pktEncryptionRequest:     return "pktEncryptionRequest";
		case cProtocol::pktEntityAnimation:       return "pktEntityAnimation";
		case cProtocol::pktEntityEffect:          return "pktEntityEffect";
		case cProtocol::pktEntityEquipment:       return "pktEntityEquipment";
		case cProtocol::pktEntityHeadLook:        return "pktEntityHeadLook";
		case cProtocol::pktEntityLook:            return "pktEntityLook";
		case cProtocol::pktEntityMeta:            return "pktEntityMeta";
		case cProtocol::pktEntityProperties:      return "pktEntityProperties";
		case cProtocol::pktEntityRelMove:         return "pktEntityRelMove";
		case cProtocol::pktEntityRelMoveLook:     return "pktEntityRelMoveLook";
		case cProtocol::pktEntityStatus:          return "pktEntityStatus";
		case cProtocol::pktEntityVelocity:        return "pktEntityVelocity";
		case cProtocol::pktExperience:            return "pktExperience";
		case cProtocol::pktExplosion:             return "pktExplosion";
		case cProtocol::pktGameMode:              return "pktGameMode";
		case cProtocol::pktHeldItemChange:        return "pktHeldItemChange";
		case cProtocol::pktInventorySlot:         return "pktInventorySlot";
		case cProtocol::pktJoinGame:              return "pktJoinGame";
		case cProtocol::pktKeepAlive:             return "pktKeepAlive";
		case cProtocol::pktLeashEntity:           return "pktLeashEntity";
		case cProtocol::pktLoginSuccess:          return "pktLoginSuccess";
		case cProtocol::pktMapData:               return "pktMapData";
		case cProtocol::pktParticleEffect:        return "pktParticleEffect";
		case cProtocol::pktPingResponse:          return "pktPingResponse";
		case cProtocol::pktPlayerAbilities:       return "pktPlayerAbilities";
		case cProtocol::pktPlayerList:            return "pktPlayerList";
		case cProtocol::pktPlayerMaxSpeed:        return "pktPlayerMaxSpeed";
		case cProtocol::pktPlayerMoveLook:        return "pktPlayerMoveLook";
		case cProtocol::pktPluginMessage:         return "pktPluginMessage";
		case cProtocol::pktRemoveEntityEffect:    return "pktRemoveEntityEffect";
		case cProtocol::pktResourcePack:          return "pktResourcePack";
		case cProtocol::pktRespawn:               return "pktRespawn";
		case cProtocol::pktScoreboardObjective:   return "pktScoreboardObjective";
		case cProtocol::pktSpawnObject:           return "pktSpawnObject";
		case cProtocol::pktSoundEffect:           return "pktSoundEffect";
		case cProtocol::pktSoundParticleEffect:   return "pktSoundParticleEffect";
		case cProtocol::pktSpawnExperienceOrb:    return "pktSpawnExperienceOrb";
		case cProtocol::pktSpawnGlobalEntity:     return "pktSpawnGlobalEntity";
		case cProtocol::pktSpawnMob:              return "pktSpawnMob";
		case cProtocol::pktSpawnOtherPlayer:      return "pktSpawnOtherPlayer";
		case cProtocol::pktSpawnPainting:         return "pktSpawnPainting";
		case cProtocol::pktSpawnPosition:         return "pktSpawnPosition";
		case cProtocol::pktStartCompression:      return "pktStartCompression";
		case cProtocol::pktStatistics:            return "pktStatistics";
		case cProtocol::pktStatusResponse:        return "pktStatusResponse";
		case cProtocol::pktTabCompletionResults:  return "pktTabCompletionResults";
		case cProtocol::pktTeleportEntity:        return "pktTeleportEntity";
		case cProtocol::pktTimeUpdate:            return "pktTimeUpdate";
		case cProtocol::pktTitle:                 return "pktTitle";
		case cProtocol::pktUnloadChunk:           return "pktUnloadChunk";
		case cProtocol::pktUnlockRecipe:          return "pktUnlockRecipe";
		case cProtocol::pktUpdateBlockEntity:     return "pktUpdateBlockEntity";
		case cProtocol::pktUpdateHealth:          return "pktUpdateHealth";
		case cProtocol::pktUpdateScore:           return "pktUpdateScore";
		case cProtocol::pktUpdateSign:            return "pktUpdateSign";
		case cProtocol::pktUseBed:                return "pktUseBed";
		case cProtocol::pktWeather:               return "pktWeather";
		case cProtocol::pktWindowItems:           return "pktWindowItems";
		case cProtocol::pktWindowClose:           return "pktWindowClose";
		case cProtocol::pktWindowOpen:            return "pktWindowOpen";
		case cProtocol::pktWindowProperty:        return "pktWindowProperty";
	}
	return Printf("Unknown packet type: 0x%02x", a_PacketType);
}
