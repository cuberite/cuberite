
// Protocol_1_12.h

/*
Declares the 1.12 protocol classes:
	- cProtocol_1_12
		- release 1.12 protocol (#335)
	- cProtocol_1_12_1
		- release 1.12.1 protocol (#338)
(others may be added later in the future for the 1.12 release series)
*/





#pragma once

#include "Protocol_1_11.h"





class cProtocol_1_12 :
	public cProtocol_1_11_1
{
	typedef cProtocol_1_11_1 super;

public:
	cProtocol_1_12(cClientHandle * a_Client, const AString &a_ServerAddress, UInt16 a_ServerPort, UInt32 a_State);

	virtual void SendAttachEntity(const cEntity & a_Entity, const cEntity & a_Vehicle) override;
	virtual void SendCameraSetTo(const cEntity & a_Entity) override;
	virtual void SendCollectEntity(const cEntity & a_Entity, const cPlayer & a_Player, int a_Count) override;
	virtual void SendDestroyEntity(const cEntity & a_Entity) override;
	virtual void SendDetachEntity(const cEntity & a_Entity, const cEntity & a_PreviousVehicle) override;
	virtual void SendDisplayObjective(const AString & a_Objective, cScoreboard::eDisplaySlot a_Display) override;
	virtual void SendEntityEffect(const cEntity & a_Entity, int a_EffectID, int a_Amplifier, short a_Duration) override;
	virtual void SendEntityEquipment(const cEntity & a_Entity, short a_SlotNum, const cItem & a_Item) override;
	virtual void SendEntityHeadLook(const cEntity & a_Entity) override;
	virtual void SendEntityLook(const cEntity & a_Entity) override;
	virtual void SendEntityMetadata(const cEntity & a_Entity) override;
	virtual void SendEntityProperties(const cEntity & a_Entity) override;
	virtual void SendEntityRelMove(const cEntity & a_Entity, char a_RelX, char a_RelY, char a_RelZ) override;
	virtual void SendEntityRelMoveLook(const cEntity & a_Entity, char a_RelX, char a_RelY, char a_RelZ) override;
	virtual void SendEntityVelocity(const cEntity & a_Entity) override;
	virtual void SendExperience(void) override;
	virtual void SendHealth(void) override;
	virtual void SendHideTitle(void) override;
	virtual void SendLogin(const cPlayer & a_Player, const cWorld & a_World) override;
	virtual void SendPlayerMaxSpeed(void) override;
	virtual void SendRemoveEntityEffect(const cEntity & a_Entity, int a_EffectID) override;
	virtual void SendResetTitle(void) override;
	virtual void SendRespawn(eDimension a_Dimension) override;
	virtual void SendScoreUpdate(const AString & a_Objective, const AString & a_Player, cObjective::Score a_Score, Byte a_Mode) override;
	virtual void SendScoreboardObjective(const AString & a_Name, const AString & a_DisplayName, Byte a_Mode) override;
	virtual void SendSetRawSubTitle(const AString & a_SubTitle) override;
	virtual void SendSetRawTitle(const AString & a_Title) override;
	virtual void SendSpawnMob(const cMonster & a_Mob) override;
	virtual void SendTeleportEntity(const cEntity & a_Entity) override;
	virtual void SendTeam(const cTeam & a_Team, Byte a_Mode) override;
	virtual void SendTeamChangeMembership(const AString & a_TeamName, bool a_IsAdding, const std::set<AString> & a_Delta) override;
	virtual void SendTimeUpdate(Int64 a_WorldAge, Int64 a_TimeOfDay, bool a_DoDaylightCycle) override;
	virtual void SendTitleTimes(int a_FadeInTicks, int a_DisplayTicks, int a_FadeOutTicks) override;
	virtual void SendUpdateBlockEntity(cBlockEntity & a_BlockEntity) override;
protected:
	virtual bool HandlePacket(cByteBuffer & a_ByteBuffer, UInt32 a_PacketType) override;
	virtual void HandlePacketAdvancementTab(cByteBuffer & a_ByteBuffer);
	virtual void HandlePacketCraftingBookData(cByteBuffer & a_ByteBuffer);
	virtual void HandlePacketBlockPlace(cByteBuffer & a_ByteBuffer) override;
	virtual void HandlePacketStatusRequest(cByteBuffer & a_ByteBuffer) override;
	virtual void WriteEntityMetadata(cPacketizer & a_Pkt, const cEntity & a_Entity) override;
	virtual void WriteMobMetadata(cPacketizer & a_Pkt, const cMonster & a_Mob) override;
};





class cProtocol_1_12_1 :
	public cProtocol_1_12
{
	typedef cProtocol_1_12 super;

public:
	cProtocol_1_12_1(cClientHandle * a_Client, const AString &a_ServerAddress, UInt16 a_ServerPort, UInt32 a_State);

	virtual void SendRespawn(eDimension a_Dimension) override;
	virtual void SendPlayerListAddPlayer(const cPlayer & a_Player) override;
	virtual void SendPlayerListRemovePlayer(const cPlayer & a_Player) override;
	virtual void SendPlayerListUpdateGameMode(const cPlayer & a_Player) override;
	virtual void SendPlayerListUpdatePing(const cPlayer & a_Player) override;
	virtual void SendPlayerListUpdateDisplayName(const cPlayer & a_Player, const AString & a_CustomName) override;
	virtual void SendPlayerAbilities(void) override;
	virtual void SendPlayerMoveLook(void) override;
	virtual void SendUseBed(const cEntity & a_Entity, int a_BlockX, int a_BlockY, int a_BlockZ) override;
	virtual void SendDestroyEntity(const cEntity & a_Entity) override;
	virtual void SendRemoveEntityEffect(const cEntity & a_Entity, int a_EffectID) override;
	virtual void SendEntityHeadLook(const cEntity & a_Entity) override;
	virtual void SendCameraSetTo(const cEntity & a_Entity) override;
	virtual void SendDisplayObjective(const AString & a_Objective, cScoreboard::eDisplaySlot a_Display) override;
	virtual void SendEntityMetadata(const cEntity & a_Entity) override;
	virtual void SendEntityVelocity(const cEntity & a_Entity) override;
	virtual void SendEntityEquipment(const cEntity & a_Entity, short a_SlotNum, const cItem & a_Item) override;
	virtual void SendExperience(void) override;
	virtual void SendHealth(void) override;
	virtual void SendScoreboardObjective(const AString & a_Name, const AString & a_DisplayName, Byte a_Mode) override;
	virtual void SendAttachEntity(const cEntity & a_Entity, const cEntity & a_Vehicle) override;
	virtual void SendDetachEntity(const cEntity & a_Entity, const cEntity & a_PreviousVehicle) override;
	virtual void SendScoreUpdate(const AString & a_Objective, const AString & a_Player, cObjective::Score a_Score, Byte a_Mode) override;
	virtual void SendLogin(const cPlayer & a_Player, const cWorld & a_World) override;
	virtual void SendTimeUpdate(Int64 a_WorldAge, Int64 a_TimeOfDay, bool a_DoDaylightCycle) override;
	virtual void SendHideTitle(void) override;
	virtual void SendResetTitle(void) override;
	virtual void SendSetRawSubTitle(const AString & a_SubTitle) override;
	virtual void SendSetRawTitle(const AString & a_Title) override;
	virtual void SendTitleTimes(int a_FadeInTicks, int a_DisplayTicks, int a_FadeOutTicks) override;
	virtual void SendCollectEntity(const cEntity & a_Entity, const cPlayer & a_Player, int a_Count) override;
	virtual void SendTeleportEntity(const cEntity & a_Entity) override;
	virtual void SendEntityProperties(const cEntity & a_Entity) override;
	virtual void SendEntityEffect(const cEntity & a_Entity, int a_EffectID, int a_Amplifier, short a_Duration) override;
	virtual void SendPlayerMaxSpeed(void) override;

protected:
	virtual bool HandlePacket(cByteBuffer & a_ByteBuffer, UInt32 a_PacketType) override;
	virtual void HandlePacketStatusRequest(cByteBuffer & a_ByteBuffer) override;
};




