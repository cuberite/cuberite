#include "Globals.h"

#include "Protocol_1_9.h"
#include "Protocol_1_12.h"





UInt32 cProtocol_1_9_0::GetPacketId(eOutgoingPackets a_Packet)
{
	switch (a_Packet)
	{
		case sendAttachEntity:          return 0x40;
		case sendBlockAction:           return 0x0a;
		case sendBlockBreakAnim:        return 0x08;
		case sendBlockChange:           return 0x0b;
		case sendBlockChanges:          return 0x10;
		case sendCameraSetTo:           return 0x36;
		case sendChatRaw:               return 0x0f;
		case sendCollectEntity:         return 0x49;
		case sendDestroyEntity:         return 0x30;
		case sendDifficulty:            return 0x0d;
		case sendDisconnectDuringGame:  return 0x1a;
		case sendDisconnectDuringLogin: return 0x0;
		case sendDisplayObjective:      return 0x38;
		case sendEditSign:              return 0x2a;
		case sendEntityAnimation:       return 0x06;
		case sendEntityEffect:          return 0x4c;
		case sendEntityEquipment:       return 0x3c;
		case sendEntityHeadLook:        return 0x34;
		case sendEntityLook:            return 0x27;
		case sendEntityMeta:            return 0x39;
		case sendEntityProperties:      return 0x4b;
		case sendEntityRelMove:         return 0x25;
		case sendEntityRelMoveLook:     return 0x26;
		case sendEntityStatus:          return 0x1b;
		case sendEntityVelocity:        return 0x3b;
		case sendExperience:            return 0x3d;
		case sendExperienceOrb:         return 0x01;
		case sendExplosion:             return 0x1c;
		case sendGameMode:              return 0x1e;
		case sendHealth:                return 0x3e;
		case sendInventorySlot:         return 0x16;
		case sendJoinGame:              return 0x23;
		case sendKeepAlive:             return 0x1f;
		case sendLeashEntity:           return 0x3a;
		case sendMapData:               return 0x24;
		case sendPaintingSpawn:         return 0x04;
		case sendParticleEffect:        return 0x22;
		case sendPlayerAbilities:       return 0x2b;
		case sendPlayerList:            return 0x2d;
		case sendPlayerMaxSpeed:        return 0x4b;
		case sendPlayerMoveLook:        return 0x2e;
		case sendPlayerSpawn:           return 0x05;
		case sendPluginMessage:         return 0x18;
		case sendRemoveEntityEffect:    return 0x31;
		case sendRespawn:               return 0x33;
		case sendScoreboardObjective:   return 0x3f;
		case sendScoreUpdate:           return 0x42;
		case sendSpawnGlobalEntity:     return 0x02;
		case sendSpawnObject:           return 0x00;
		case sendSpawnPosition:         return 0x43;
		case sendSoundEffect:           return 0x19;
		case sendSoundParticleEffect:   return 0x21;
		case sendSpawnMob:              return 0x03;
		case sendStatistics:            return 0x07;
		case sendTabCompletion:         return 0x0e;
		case sendTeleportEntity:        return 0x4a;
		case sendTimeUpdate:            return 0x44;
		case sendTitle:                 return 0x45;
		case sendUnloadChunk:           return 0x1d;
		case sendUpdateBlockEntity:     return 0x09;
		case sendUpdateSign:            return 0x46;
		case sendUseBed:                return 0x2f;
		case sendWeather:               return 0x1e;
		case sendWindowClose:           return 0x12;
		case sendWindowItems:           return 0x14;
		case sendWindowOpen:            return 0x13;
		case sendWindowProperty:        return 0x15;
	}
	UNREACHABLE("Unsupported outgoing packet type");
}





UInt32 cProtocol_1_9_4::GetPacketId(eOutgoingPackets a_Packet)
{
	switch (a_Packet)
	{
		case sendCollectEntity:    return 0x48;
		case sendEntityEffect:     return 0x4b;
		case sendEntityProperties: return 0x4a;
		case sendPlayerMaxSpeed:   return 0x4a;
		case sendTeleportEntity:   return 0x49;

		default: return super::GetPacketId(a_Packet);
	}
}





UInt32 cProtocol_1_12::GetPacketId(eOutgoingPackets a_Packet)
{
	switch (a_Packet)
	{
		case sendAttachEntity:        return 0x42;
		case sendCameraSetTo:         return 0x38;
		case sendCollectEntity:       return 0x4a;
		case sendDestroyEntity:       return 0x31;
		case sendDisplayObjective:    return 0x3a;
		case sendEntityEffect:        return 0x4e;
		case sendEntityEquipment:     return 0x3e;
		case sendEntityHeadLook:      return 0x35;
		case sendEntityLook:          return 0x28;
		case sendEntityMeta:          return 0x3b;
		case sendEntityProperties:    return 0x4d;
		case sendEntityRelMove:       return 0x26;
		case sendEntityRelMoveLook:   return 0x27;
		case sendEntityVelocity:      return 0x3d;
		case sendExperience:          return 0x3f;
		case sendHealth:              return 0x40;
		case sendLeashEntity:         return 0x3c;
		case sendPlayerMaxSpeed:      return 0x4d;
		case sendRemoveEntityEffect:  return 0x32;
		case sendRespawn:             return 0x34;
		case sendScoreboardObjective: return 0x41;
		case sendScoreUpdate:         return 0x44;
		case sendSpawnPosition:       return 0x45;
		case sendTeleportEntity:      return 0x4b;
		case sendTimeUpdate:          return 0x46;
		case sendTitle:               return 0x47;
		case sendUpdateBlockEntity:   return 0x09;

		default: return super::GetPacketId(a_Packet);
	}
}





UInt32 cProtocol_1_12_1::GetPacketId(eOutgoingPackets a_Packet)
{
	switch (a_Packet)
	{
		case sendAttachEntity:        return 0x43;
		case sendCameraSetTo:         return 0x39;
		case sendCollectEntity:       return 0x4b;
		case sendDestroyEntity:       return 0x32;
		case sendDisplayObjective:    return 0x3b;
		case sendEntityEffect:        return 0x4f;
		case sendEntityEquipment:     return 0x3f;
		case sendEntityHeadLook:      return 0x36;
		case sendEntityMeta:          return 0x3c;
		case sendEntityProperties:    return 0x4e;
		case sendEntityVelocity:      return 0x3e;
		case sendExperience:          return 0x40;
		case sendHealth:              return 0x41;
		case sendLeashEntity:         return 0x3d;
		case sendPlayerList:          return 0x2e;
		case sendPlayerAbilities:     return 0x2c;
		case sendPlayerMaxSpeed:      return 0x4e;
		case sendPlayerMoveLook:      return 0x2f;
		case sendRemoveEntityEffect:  return 0x33;
		case sendRespawn:             return 0x35;
		case sendScoreboardObjective: return 0x42;
		case sendScoreUpdate:         return 0x45;
		case sendSpawnPosition:       return 0x46;
		case sendUseBed:              return 0x30;
		case sendTeleportEntity:      return 0x4c;
		case sendTimeUpdate:          return 0x47;
		case sendTitle:               return 0x48;

		default: return super::GetPacketId(a_Packet);
	}
}
