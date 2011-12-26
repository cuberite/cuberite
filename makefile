###################################################
#
# Makefile for MCServer
# Creator [Xcode -> Makefile Ver: Feb 14 2007 09:18:41]
# Created: [Thu Feb 24 19:53:17 2011]
#
###################################################

#
# Macros
#

CC = /usr/bin/g++ -msse4
CC_OPTIONS = -O2 -s
CCE_OPTIONS = -O2 -s -x c
LNK_OPTIONS = -lstdc++ -pthread


#
# INCLUDE directories for MCServer
#

INCLUDE = -I.\
		-Isource\
		-Isource/md5\
		-IWebServer\
		-Isource/packets\
		-Itolua++-1.0.93/src/lib\
		-Ilua-5.1.4/src\
		-Izlib-1.2.5\
		-IiniFile\
		-Itolua++-1.0.93/include\
		-Ijsoncpp-src-0.5.0/include\
		-Ijsoncpp-src-0.5.0/src/lib_json


#
# Build MCServer
#

MCServer : \
		build/json_reader.o\
		build/json_value.o\
		build/json_writer.o\
		build/cMakeDir.o\
		build/cGenSettings.o\
		build/cCuboid.o\
		build/cNoise.o\
		build/cTimer.o\
		build/cSleep.o\
		build/cMonsterConfig.o\
		build/cSpider.o\
		build/cThread.o\
		build/cBlockingTCPLink.o\
		build/cAuthenticator.o\
		build/cLuaCommandBinder.o\
		build/cChicken.o\
		build/cCow.o\
		build/cPig.o\
		build/cSheep.o\
		build/cSquid.o\
		build/cWolf.o\
		build/cZombie.o\
		build/cEnderman.o\
		build/cCreeper.o\
		build/cGhast.o\
		build/cCavespider.o\
		build/cZombiepigman.o\
		build/cSkeleton.o\
		build/cSlime.o\
		build/cSilverfish.o\
		build/md5.o\
		build/cReferenceManager.o\
		build/Bindings.o\
		build/cBlockToPickup.o\
		build/cChatColor.o\
		build/cChestEntity.o\
		build/cChunk.o\
		build/cChunkMap.o\
		build/cChunkGenerator.o\
		build/cClientHandle.o\
		build/cCraftingWindow.o\
		build/cCriticalSection.o\
		build/cEntity.o\
		build/cEvent.o\
		build/cFurnaceEntity.o\
		build/cFurnaceRecipe.o\
		build/cFurnaceWindow.o\
		build/cGroup.o\
		build/cGroupManager.o\
		build/cInventory.o\
		build/cLog.o\
		build/cMonster.o\
		build/cAggressiveMonster.o\
		build/cPassiveMonster.o\
		build/cPassiveAggressiveMonster.o\
		build/cRedstone.o\
		build/cPiston.o\
		build/cPawn.o\
		build/cPickup.o\
		build/cPlayer.o\
		build/cPlugin_Lua.o\
		build/cPlugin.o\
		build/cPluginManager.o\
		build/cRecipeChecker.o\
		build/cRoot.o\
		build/cSemaphore.o\
		build/cServer.o\
		build/cSignEntity.o\
		build/cStringMap.o\
		build/cWebAdmin.o\
		build/cWebPlugin.o\
		build/cWindow.o\
		build/cWorld.o\
		build/cWorldGenerator.o\
		build/main.o\
		build/ManualBindings.o\
		build/Matrix4f.o\
		build/Vector3d.o\
		build/Vector3f.o\
		build/Vector3i.o\
		build/cHeartBeat.o\
		build/cTCPLink.o\
		build/cMCLogger.o\
		build/cTracer.o\
		build/cPacket_13.o\
		build/cPacket_AddToInventory.o\
		build/cPacket_ArmAnim.o\
		build/cPacket_BlockChange.o\
		build/cPacket_BlockAction.o\
		build/cPacket_Explosion.o\
		build/cPacket_SoundEffect.o\
		build/cPacket_BlockDig.o\
		build/cPacket_BlockPlace.o\
		build/cPacket_Chat.o\
		build/cPacket_CollectItem.o\
		build/cPacket_DestroyEntity.o\
		build/cPacket_Disconnect.o\
		build/cPacket_EntityEquipment.o\
		build/cPacket_CreateInventoryAction.o\
		build/cPacket_NewInvalidState.o\
		build/cPacket_Thunderbolt.o\
		build/cPacket_EntityLook.o\
		build/cPacket_EntityStatus.o\
		build/cPacket_Flying.o\
		build/cPacket_Handshake.o\
		build/cPacket_InventoryProgressBar.o\
		build/cPacket_InventorySlot.o\
		build/cPacket_ItemData.o\
		build/cPacket_ItemSwitch.o\
		build/cPacket_KeepAlive.o\
		build/cPacket_Login.o\
		build/cPacket_MapChunk.o\
		build/cPacket_Metadata.o\
		build/cPacket_MultiBlock.o\
		build/cPacket_NamedEntitySpawn.o\
		build/cPacket_PickupSpawn.o\
		build/cPacket_PlayerListItem.o\
		build/cPacket_PlayerLook.o\
		build/cPacket_PlayerMoveLook.o\
		build/cPacket_PlayerPosition.o\
		build/cPacket_PreChunk.o\
		build/cPacket_RelativeEntityMove.o\
		build/cPacket_RelativeEntityMoveLook.o\
		build/cPacket_Respawn.o\
		build/cPacket_SpawnMob.o\
		build/cPacket_TeleportEntity.o\
		build/cPacket_TimeUpdate.o\
		build/cPacket_UpdateHealth.o\
		build/cPacket_UpdateSign.o\
		build/cPacket_UseEntity.o\
		build/cPacket_WholeInventory.o\
		build/cPacket_WindowClick.o\
		build/cPacket_WindowClose.o\
		build/cPacket_WindowOpen.o\
		build/cPacket.o\
		build/base64.o\
		build/Socket.o\
		build/StdHelpers.o\
		build/UrlHelper.o\
		build/WebServer.o\
		build/tolua_event.o\
		build/tolua_is.o\
		build/tolua_map.o\
		build/tolua_push.o\
		build/tolua_to.o\
		build/lapi.o\
		build/lauxlib.o\
		build/lbaselib.o\
		build/lcode.o\
		build/ldblib.o\
		build/ldebug.o\
		build/ldo.o\
		build/ldump.o\
		build/lfunc.o\
		build/lgc.o\
		build/linit.o\
		build/liolib.o\
		build/llex.o\
		build/lmathlib.o\
		build/lmem.o\
		build/loadlib.o\
		build/lobject.o\
		build/lopcodes.o\
		build/loslib.o\
		build/lparser.o\
		build/lstate.o\
		build/lstring.o\
		build/lstrlib.o\
		build/ltable.o\
		build/ltablib.o\
		build/ltm.o\
		build/luac.o\
		build/lundump.o\
		build/lvm.o\
		build/lzio.o\
		build/print.o\
		build/adler32.o\
		build/compress.o\
		build/crc32.o\
		build/deflate.o\
		build/gzclose.o\
		build/gzlib.o\
		build/gzread.o\
		build/gzwrite.o\
		build/infback.o\
		build/inffast.o\
		build/inflate.o\
		build/inftrees.o\
		build/trees.o\
		build/uncompr.o\
		build/zutil.o\
		build/iniFile.o\
		build/cSocket.o\
		build/cFluidSimulator.o\
		build/cWaterSimulator.o\
		build/cLavaSimulator.o\
		build/cFileFormatUpdater.o\
		build/cItem.o
	$(CC) $(LNK_OPTIONS) \
		build/json_reader.o\
		build/json_value.o\
		build/json_writer.o\
		build/cMakeDir.o\
		build/cGenSettings.o\
		build/cCuboid.o\
		build/cNoise.o\
		build/cTimer.o\
		build/cSleep.o\
		build/cMonsterConfig.o\
		build/cSpider.o\
		build/cThread.o\
		build/cBlockingTCPLink.o\
		build/cAuthenticator.o\
		build/cLuaCommandBinder.o\
		build/cChicken.o\
		build/cCow.o\
		build/cPig.o\
		build/cSheep.o\
		build/cSquid.o\
		build/cWolf.o\
		build/cZombie.o\
		build/cEnderman.o\
		build/cCreeper.o\
		build/cGhast.o\
		build/cCavespider.o\
		build/cZombiepigman.o\
		build/cSkeleton.o\
		build/cSlime.o\
		build/cSilverfish.o\
		build/md5.o\
		build/cReferenceManager.o\
		build/Bindings.o\
		build/cBlockToPickup.o\
		build/cChatColor.o\
		build/cChestEntity.o\
		build/cChunk.o\
		build/cChunkMap.o\
		build/cChunkGenerator.o\
		build/cClientHandle.o\
		build/cCraftingWindow.o\
		build/cCriticalSection.o\
		build/cEntity.o\
		build/cEvent.o\
		build/cFurnaceEntity.o\
		build/cFurnaceRecipe.o\
		build/cFurnaceWindow.o\
		build/cGroup.o\
		build/cGroupManager.o\
		build/cInventory.o\
		build/cLog.o\
		build/cMonster.o\
		build/cAggressiveMonster.o\
		build/cPassiveMonster.o\
		build/cPassiveAggressiveMonster.o\
		build/cRedstone.o\
		build/cPiston.o\
		build/cPawn.o\
		build/cPickup.o\
		build/cPlayer.o\
		build/cPlugin_Lua.o\
		build/cPlugin.o\
		build/cPluginManager.o\
		build/cRecipeChecker.o\
		build/cRoot.o\
		build/cSemaphore.o\
		build/cServer.o\
		build/cSignEntity.o\
		build/cStringMap.o\
		build/cWebAdmin.o\
		build/cWebPlugin.o\
		build/cWindow.o\
		build/cWorld.o\
		build/cWorldGenerator.o\
		build/main.o\
		build/ManualBindings.o\
		build/Matrix4f.o\
		build/Vector3d.o\
		build/Vector3f.o\
		build/Vector3i.o\
		build/cHeartBeat.o\
		build/cTCPLink.o\
		build/cMCLogger.o\
		build/cTracer.o\
		build/cPacket_13.o\
		build/cPacket_AddToInventory.o\
		build/cPacket_ArmAnim.o\
		build/cPacket_BlockChange.o\
		build/cPacket_BlockAction.o\
		build/cPacket_Explosion.o\
		build/cPacket_SoundEffect.o\
		build/cPacket_BlockDig.o\
		build/cPacket_BlockPlace.o\
		build/cPacket_Chat.o\
		build/cPacket_CollectItem.o\
		build/cPacket_DestroyEntity.o\
		build/cPacket_Disconnect.o\
		build/cPacket_EntityEquipment.o\
		build/cPacket_CreateInventoryAction.o\
		build/cPacket_NewInvalidState.o\
		build/cPacket_Thunderbolt.o\
		build/cPacket_EntityLook.o\
		build/cPacket_EntityStatus.o\
		build/cPacket_Flying.o\
		build/cPacket_Handshake.o\
		build/cPacket_InventoryProgressBar.o\
		build/cPacket_InventorySlot.o\
		build/cPacket_ItemData.o\
		build/cPacket_ItemSwitch.o\
		build/cPacket_KeepAlive.o\
		build/cPacket_Login.o\
		build/cPacket_MapChunk.o\
		build/cPacket_Metadata.o\
		build/cPacket_MultiBlock.o\
		build/cPacket_NamedEntitySpawn.o\
		build/cPacket_PickupSpawn.o\
		build/cPacket_PlayerListItem.o\
		build/cPacket_PlayerLook.o\
		build/cPacket_PlayerMoveLook.o\
		build/cPacket_PlayerPosition.o\
		build/cPacket_PreChunk.o\
		build/cPacket_RelativeEntityMove.o\
		build/cPacket_RelativeEntityMoveLook.o\
		build/cPacket_Respawn.o\
		build/cPacket_SpawnMob.o\
		build/cPacket_TeleportEntity.o\
		build/cPacket_TimeUpdate.o\
		build/cPacket_UpdateHealth.o\
		build/cPacket_UpdateSign.o\
		build/cPacket_UseEntity.o\
		build/cPacket_WholeInventory.o\
		build/cPacket_WindowClick.o\
		build/cPacket_WindowClose.o\
		build/cPacket_WindowOpen.o\
		build/cPacket.o\
		build/base64.o\
		build/Socket.o\
		build/StdHelpers.o\
		build/UrlHelper.o\
		build/WebServer.o\
		build/tolua_event.o\
		build/tolua_is.o\
		build/tolua_map.o\
		build/tolua_push.o\
		build/tolua_to.o\
		build/lapi.o\
		build/lauxlib.o\
		build/lbaselib.o\
		build/lcode.o\
		build/ldblib.o\
		build/ldebug.o\
		build/ldo.o\
		build/ldump.o\
		build/lfunc.o\
		build/lgc.o\
		build/linit.o\
		build/liolib.o\
		build/llex.o\
		build/lmathlib.o\
		build/lmem.o\
		build/loadlib.o\
		build/lobject.o\
		build/lopcodes.o\
		build/loslib.o\
		build/lparser.o\
		build/lstate.o\
		build/lstring.o\
		build/lstrlib.o\
		build/ltable.o\
		build/ltablib.o\
		build/ltm.o\
		build/luac.o\
		build/lundump.o\
		build/lvm.o\
		build/lzio.o\
		build/print.o\
		build/adler32.o\
		build/compress.o\
		build/crc32.o\
		build/deflate.o\
		build/gzclose.o\
		build/gzlib.o\
		build/gzread.o\
		build/gzwrite.o\
		build/infback.o\
		build/inffast.o\
		build/inflate.o\
		build/inftrees.o\
		build/trees.o\
		build/uncompr.o\
		build/zutil.o\
		build/iniFile.o\
		build/cSocket.o\
		build/cFluidSimulator.o\
		build/cWaterSimulator.o\
		build/cLavaSimulator.o\
		build/cFileFormatUpdater.o\
		build/cItem.o\
		-o MCServer

clean : 
		rm \
		build/json_reader.o\
		build/json_value.o\
		build/json_writer.o\
		build/cMakeDir.o\
		build/cGenSettings.o\
		build/cCuboid.o\
		build/cNoise.o\
		build/cTimer.o\
		build/cSleep.o\
		build/cMonsterConfig.o\
		build/cSpider.o\
		build/cThread.o\
		build/cBlockingTCPLink.o\
		build/cAuthenticator.o\
		build/cLuaCommandBinder.o\
		build/cChicken.o\
		build/cCow.o\
		build/cPig.o\
		build/cSheep.o\
		build/cSquid.o\
		build/cWolf.o\
		build/cZombie.o\
		build/cEnderman.o\
		build/cCreeper.o\
		build/cGhast.o\
		build/cCavespider.o\
		build/cZombiepigman.o\
		build/cSkeleton.o\
		build/cSlime.o\
		build/cSilverfish.o\
		build/md5.o\
		build/cReferenceManager.o\
		build/Bindings.o\
		build/cBlockToPickup.o\
		build/cChatColor.o\
		build/cChestEntity.o\
		build/cChunk.o\
		build/cChunkMap.o\
		build/cChunkGenerator.o\
		build/cClientHandle.o\
		build/cCraftingWindow.o\
		build/cCriticalSection.o\
		build/cEntity.o\
		build/cEvent.o\
		build/cFurnaceEntity.o\
		build/cFurnaceRecipe.o\
		build/cFurnaceWindow.o\
		build/cGroup.o\
		build/cGroupManager.o\
		build/cInventory.o\
		build/cLog.o\
		build/cMonster.o\
		build/cAggressiveMonster.o\
		build/cPassiveMonster.o\
		build/cPassiveAggressiveMonster.o\
		build/cRedstone.o\
		build/cPiston.o\
		build/cPawn.o\
		build/cPickup.o\
		build/cPlayer.o\
		build/cPlugin_Lua.o\
		build/cPlugin.o\
		build/cPluginManager.o\
		build/cRecipeChecker.o\
		build/cRoot.o\
		build/cSemaphore.o\
		build/cServer.o\
		build/cSignEntity.o\
		build/cStringMap.o\
		build/cWebAdmin.o\
		build/cWebPlugin.o\
		build/cWindow.o\
		build/cWorld.o\
		build/cWorldGenerator.o\
		build/main.o\
		build/ManualBindings.o\
		build/Matrix4f.o\
		build/Vector3d.o\
		build/Vector3f.o\
		build/Vector3i.o\
		build/cHeartBeat.o\
		build/cTCPLink.o\
		build/cMCLogger.o\
		build/cTracer.o\
		build/cPacket_13.o\
		build/cPacket_AddToInventory.o\
		build/cPacket_ArmAnim.o\
		build/cPacket_BlockChange.o\
		build/cPacket_BlockAction.o\
		build/cPacket_Explosion.o\
		build/cPacket_SoundEffect.o\
		build/cPacket_BlockDig.o\
		build/cPacket_BlockPlace.o\
		build/cPacket_Chat.o\
		build/cPacket_CollectItem.o\
		build/cPacket_DestroyEntity.o\
		build/cPacket_Disconnect.o\
		build/cPacket_EntityEquipment.o\
		build/cPacket_CreateInventoryAction.o\
		build/cPacket_NewInvalidState.o\
		build/cPacket_Thunderbolt.o\
		build/cPacket_EntityLook.o\
		build/cPacket_EntityStatus.o\
		build/cPacket_Flying.o\
		build/cPacket_Handshake.o\
		build/cPacket_InventoryProgressBar.o\
		build/cPacket_InventorySlot.o\
		build/cPacket_ItemData.o\
		build/cPacket_ItemSwitch.o\
		build/cPacket_KeepAlive.o\
		build/cPacket_Login.o\
		build/cPacket_MapChunk.o\
		build/cPacket_Metadata.o\
		build/cPacket_MultiBlock.o\
		build/cPacket_NamedEntitySpawn.o\
		build/cPacket_PickupSpawn.o\
		build/cPacket_PlayerListItem.o\
		build/cPacket_PlayerLook.o\
		build/cPacket_PlayerMoveLook.o\
		build/cPacket_PlayerPosition.o\
		build/cPacket_PreChunk.o\
		build/cPacket_RelativeEntityMove.o\
		build/cPacket_RelativeEntityMoveLook.o\
		build/cPacket_Respawn.o\
		build/cPacket_SpawnMob.o\
		build/cPacket_TeleportEntity.o\
		build/cPacket_TimeUpdate.o\
		build/cPacket_UpdateHealth.o\
		build/cPacket_UpdateSign.o\
		build/cPacket_UseEntity.o\
		build/cPacket_WholeInventory.o\
		build/cPacket_WindowClick.o\
		build/cPacket_WindowClose.o\
		build/cPacket_WindowOpen.o\
		build/cPacket.o\
		build/base64.o\
		build/Socket.o\
		build/StdHelpers.o\
		build/UrlHelper.o\
		build/WebServer.o\
		build/tolua_event.o\
		build/tolua_is.o\
		build/tolua_map.o\
		build/tolua_push.o\
		build/tolua_to.o\
		build/lapi.o\
		build/lauxlib.o\
		build/lbaselib.o\
		build/lcode.o\
		build/ldblib.o\
		build/ldebug.o\
		build/ldo.o\
		build/ldump.o\
		build/lfunc.o\
		build/lgc.o\
		build/linit.o\
		build/liolib.o\
		build/llex.o\
		build/lmathlib.o\
		build/lmem.o\
		build/loadlib.o\
		build/lobject.o\
		build/lopcodes.o\
		build/loslib.o\
		build/lparser.o\
		build/lstate.o\
		build/lstring.o\
		build/lstrlib.o\
		build/ltable.o\
		build/ltablib.o\
		build/ltm.o\
		build/luac.o\
		build/lundump.o\
		build/lvm.o\
		build/lzio.o\
		build/print.o\
		build/adler32.o\
		build/compress.o\
		build/crc32.o\
		build/deflate.o\
		build/gzclose.o\
		build/gzlib.o\
		build/gzread.o\
		build/gzwrite.o\
		build/infback.o\
		build/inffast.o\
		build/inflate.o\
		build/inftrees.o\
		build/trees.o\
		build/uncompr.o\
		build/zutil.o\
		build/iniFile.o\
		build/cSocket.o\
		build/cFluidSimulator.o\
		build/cWaterSimulator.o\
		build/cLavaSimulator.o\
		build/cFileFormatUpdater.o\
		build/cItem.o\
		MCServer

install : MCServer
		cp MCServer MCServer

#
# Build the parts of MCServer
#


# Item # 1 -- cLuaCommandBinder --
build/cLuaCommandBinder.o : source/cLuaCommandBinder.cpp
	$(CC) $(CC_OPTIONS) source/cLuaCommandBinder.cpp -c $(INCLUDE) -o build/cLuaCommandBinder.o


# Item # 2 -- cChicken --
build/cChicken.o : source/cChicken.cpp
	$(CC) $(CC_OPTIONS) source/cChicken.cpp -c $(INCLUDE) -o build/cChicken.o


# Item # 3 -- md5 --
build/md5.o : source/md5/md5.cpp
	$(CC) $(CC_OPTIONS) source/md5/md5.cpp -c $(INCLUDE) -o build/md5.o


# Item # 4 -- cReferenceManager --
build/cReferenceManager.o : source/cReferenceManager.cpp
	$(CC) $(CC_OPTIONS) source/cReferenceManager.cpp -c $(INCLUDE) -o build/cReferenceManager.o


# Item # 5 -- Bindings --
build/Bindings.o : source/Bindings.cpp
	$(CC) $(CC_OPTIONS) source/Bindings.cpp -c $(INCLUDE) -o build/Bindings.o


# Item # 6 -- cBlockToPickup --
build/cBlockToPickup.o : source/cBlockToPickup.cpp
	$(CC) $(CC_OPTIONS) source/cBlockToPickup.cpp -c $(INCLUDE) -o build/cBlockToPickup.o


# Item # 7 -- cChatColor --
build/cChatColor.o : source/cChatColor.cpp
	$(CC) $(CC_OPTIONS) source/cChatColor.cpp -c $(INCLUDE) -o build/cChatColor.o


# Item # 8 -- cChestEntity --
build/cChestEntity.o : source/cChestEntity.cpp
	$(CC) $(CC_OPTIONS) source/cChestEntity.cpp -c $(INCLUDE) -o build/cChestEntity.o


# Item # 9 -- cChunk --
build/cChunk.o : source/cChunk.cpp
	$(CC) $(CC_OPTIONS) source/cChunk.cpp -c $(INCLUDE) -o build/cChunk.o


# Item # 10 -- cChunkMap --
build/cChunkMap.o : source/cChunkMap.cpp
	$(CC) $(CC_OPTIONS) source/cChunkMap.cpp -c $(INCLUDE) -o build/cChunkMap.o

# Item # 10 -- cChunkGenerator --
build/cChunkGenerator.o : source/cChunkGenerator.cpp
	$(CC) $(CC_OPTIONS) source/cChunkGenerator.cpp -c $(INCLUDE) -o build/cChunkGenerator.o


# Item # 11 -- cClientHandle --
build/cClientHandle.o : source/cClientHandle.cpp
	$(CC) $(CC_OPTIONS) source/cClientHandle.cpp -c $(INCLUDE) -o build/cClientHandle.o


# Item # 12 -- cCraftingWindow --
build/cCraftingWindow.o : source/cCraftingWindow.cpp
	$(CC) $(CC_OPTIONS) source/cCraftingWindow.cpp -c $(INCLUDE) -o build/cCraftingWindow.o


# Item # 13 -- cCriticalSection --
build/cCriticalSection.o : source/cCriticalSection.cpp
	$(CC) $(CC_OPTIONS) source/cCriticalSection.cpp -c $(INCLUDE) -o build/cCriticalSection.o


# Item # 14 -- cEntity --
build/cEntity.o : source/cEntity.cpp
	$(CC) $(CC_OPTIONS) source/cEntity.cpp -c $(INCLUDE) -o build/cEntity.o


# Item # 15 -- cEvent --
build/cEvent.o : source/cEvent.cpp
	$(CC) $(CC_OPTIONS) source/cEvent.cpp -c $(INCLUDE) -o build/cEvent.o


# Item # 16 -- cFurnaceEntity --
build/cFurnaceEntity.o : source/cFurnaceEntity.cpp
	$(CC) $(CC_OPTIONS) source/cFurnaceEntity.cpp -c $(INCLUDE) -o build/cFurnaceEntity.o


# Item # 17 -- cFurnaceRecipe --
build/cFurnaceRecipe.o : source/cFurnaceRecipe.cpp
	$(CC) $(CC_OPTIONS) source/cFurnaceRecipe.cpp -c $(INCLUDE) -o build/cFurnaceRecipe.o


# Item # 18 -- cFurnaceWindow --
build/cFurnaceWindow.o : source/cFurnaceWindow.cpp
	$(CC) $(CC_OPTIONS) source/cFurnaceWindow.cpp -c $(INCLUDE) -o build/cFurnaceWindow.o


# Item # 19 -- cGroup --
build/cGroup.o : source/cGroup.cpp
	$(CC) $(CC_OPTIONS) source/cGroup.cpp -c $(INCLUDE) -o build/cGroup.o


# Item # 20 -- cGroupManager --
build/cGroupManager.o : source/cGroupManager.cpp
	$(CC) $(CC_OPTIONS) source/cGroupManager.cpp -c $(INCLUDE) -o build/cGroupManager.o


# Item # 21 -- cInventory --
build/cInventory.o : source/cInventory.cpp
	$(CC) $(CC_OPTIONS) source/cInventory.cpp -c $(INCLUDE) -o build/cInventory.o


# Item # 22 -- cLog --
build/cLog.o : source/cLog.cpp
	$(CC) $(CC_OPTIONS) source/cLog.cpp -c $(INCLUDE) -o build/cLog.o


# Item # 23 -- cMonster --
build/cMonster.o : source/cMonster.cpp
	$(CC) $(CC_OPTIONS) source/cMonster.cpp -c $(INCLUDE) -o build/cMonster.o

# Item # 23 -- cAggressiveMonster --
build/cAggressiveMonster.o : source/cAggressiveMonster.cpp
	$(CC) $(CC_OPTIONS) source/cAggressiveMonster.cpp -c $(INCLUDE) -o build/cAggressiveMonster.o

# Item # 23 -- cPassiveMonster --
build/cPassiveMonster.o : source/cPassiveMonster.cpp
	$(CC) $(CC_OPTIONS) source/cPassiveMonster.cpp -c $(INCLUDE) -o build/cPassiveMonster.o

# Item # 23 -- cPassiveAgressiveMonster --
build/cPassiveAggressiveMonster.o : source/cPassiveAggressiveMonster.cpp
	$(CC) $(CC_OPTIONS) source/cPassiveAggressiveMonster.cpp -c $(INCLUDE) -o build/cPassiveAggressiveMonster.o


# Item # 23 -- cRedstone --
build/cRedstone.o : source/cRedstone.cpp
	$(CC) $(CC_OPTIONS) source/cRedstone.cpp -c $(INCLUDE) -o build/cRedstone.o


# Item # 23 -- cPiston --
build/cPiston.o : source/cPiston.cpp
	$(CC) $(CC_OPTIONS) source/cPiston.cpp -c $(INCLUDE) -o build/cPiston.o


# Item # 25 -- cPawn --
build/cPawn.o : source/cPawn.cpp
	$(CC) $(CC_OPTIONS) source/cPawn.cpp -c $(INCLUDE) -o build/cPawn.o


# Item # 26 -- cPickup --
build/cPickup.o : source/cPickup.cpp
	$(CC) $(CC_OPTIONS) source/cPickup.cpp -c $(INCLUDE) -o build/cPickup.o


# Item # 27 -- cPlayer --
build/cPlayer.o : source/cPlayer.cpp
	$(CC) $(CC_OPTIONS) source/cPlayer.cpp -c $(INCLUDE) -o build/cPlayer.o


# Item # 28 -- cPlugin_Lua --
build/cPlugin_Lua.o : source/cPlugin_Lua.cpp
	$(CC) $(CC_OPTIONS) source/cPlugin_Lua.cpp -c $(INCLUDE) -o build/cPlugin_Lua.o


# Item # 29 -- cPlugin --
build/cPlugin.o : source/cPlugin.cpp
	$(CC) $(CC_OPTIONS) source/cPlugin.cpp -c $(INCLUDE) -o build/cPlugin.o


# Item # 30 -- cPluginManager --
build/cPluginManager.o : source/cPluginManager.cpp
	$(CC) $(CC_OPTIONS) source/cPluginManager.cpp -c $(INCLUDE) -o build/cPluginManager.o


# Item # 31 -- cRecipeChecker --
build/cRecipeChecker.o : source/cRecipeChecker.cpp
	$(CC) $(CC_OPTIONS) source/cRecipeChecker.cpp -c $(INCLUDE) -o build/cRecipeChecker.o


# Item # 32 -- cRoot --
build/cRoot.o : source/cRoot.cpp
	$(CC) $(CC_OPTIONS) source/cRoot.cpp -c $(INCLUDE) -o build/cRoot.o


# Item # 33 -- cSemaphore --
build/cSemaphore.o : source/cSemaphore.cpp
	$(CC) $(CC_OPTIONS) source/cSemaphore.cpp -c $(INCLUDE) -o build/cSemaphore.o


# Item # 34 -- cServer --
build/cServer.o : source/cServer.cpp
	$(CC) $(CC_OPTIONS) source/cServer.cpp -c $(INCLUDE) -o build/cServer.o


# Item # 35 -- cSignEntity --
build/cSignEntity.o : source/cSignEntity.cpp
	$(CC) $(CC_OPTIONS) source/cSignEntity.cpp -c $(INCLUDE) -o build/cSignEntity.o


# Item # 36 -- cStringMap --
build/cStringMap.o : source/cStringMap.cpp
	$(CC) $(CC_OPTIONS) source/cStringMap.cpp -c $(INCLUDE) -o build/cStringMap.o


# Item # 37 -- cWebAdmin --
build/cWebAdmin.o : source/cWebAdmin.cpp
	$(CC) $(CC_OPTIONS) source/cWebAdmin.cpp -c $(INCLUDE) -o build/cWebAdmin.o


# Item # 38 -- cWebPlugin --
build/cWebPlugin.o : source/cWebPlugin.cpp
	$(CC) $(CC_OPTIONS) source/cWebPlugin.cpp -c $(INCLUDE) -o build/cWebPlugin.o


# Item # 39 -- cWindow --
build/cWindow.o : source/cWindow.cpp
	$(CC) $(CC_OPTIONS) source/cWindow.cpp -c $(INCLUDE) -o build/cWindow.o


# Item # 40 -- cWorld --
build/cWorld.o : source/cWorld.cpp
	$(CC) $(CC_OPTIONS) source/cWorld.cpp -c $(INCLUDE) -o build/cWorld.o

# Item # 40 -- cWorldGenerator --
build/cWorldGenerator.o : source/cWorldGenerator.cpp
	$(CC) $(CC_OPTIONS) source/cWorldGenerator.cpp -c $(INCLUDE) -o build/cWorldGenerator.o


# Item # 41 -- main --
build/main.o : source/main.cpp
	$(CC) $(CC_OPTIONS) source/main.cpp -c $(INCLUDE) -o build/main.o


# Item # 42 -- ManualBindings --
build/ManualBindings.o : source/ManualBindings.cpp
	$(CC) $(CC_OPTIONS) source/ManualBindings.cpp -c $(INCLUDE) -o build/ManualBindings.o


# Item # 43 -- Matrix4f --
build/Matrix4f.o : source/Matrix4f.cpp
	$(CC) $(CC_OPTIONS) source/Matrix4f.cpp -c $(INCLUDE) -o build/Matrix4f.o

# Item # 45 -- Vector3d --
build/Vector3d.o : source/Vector3d.cpp
	$(CC) $(CC_OPTIONS) source/Vector3d.cpp -c $(INCLUDE) -o build/Vector3d.o


# Item # 46 -- Vector3f --
build/Vector3f.o : source/Vector3f.cpp
	$(CC) $(CC_OPTIONS) source/Vector3f.cpp -c $(INCLUDE) -o build/Vector3f.o


# Item # 47 -- Vector3i --
build/Vector3i.o : source/Vector3i.cpp
	$(CC) $(CC_OPTIONS) source/Vector3i.cpp -c $(INCLUDE) -o build/Vector3i.o


# Item # 48 -- cHeartBeat --
build/cHeartBeat.o : source/cHeartBeat.cpp
	$(CC) $(CC_OPTIONS) source/cHeartBeat.cpp -c $(INCLUDE) -o build/cHeartBeat.o


# Item # 49 -- cTCPLink --
build/cTCPLink.o : source/cTCPLink.cpp
	$(CC) $(CC_OPTIONS) source/cTCPLink.cpp -c $(INCLUDE) -o build/cTCPLink.o


# Item # 50 -- cMCLogger --
build/cMCLogger.o : source/cMCLogger.cpp
	$(CC) $(CC_OPTIONS) source/cMCLogger.cpp -c $(INCLUDE) -o build/cMCLogger.o


# Item # 51 -- cTracer --
build/cTracer.o : source/cTracer.cpp
	$(CC) $(CC_OPTIONS) source/cTracer.cpp -c $(INCLUDE) -o build/cTracer.o

build/cPacket_13.o : source/packets/cPacket_13.cpp
	$(CC) $(CC_OPTIONS) source/packets/cPacket_13.cpp -c $(INCLUDE) -o build/cPacket_13.o
	
build/cPacket_AddToInventory.o : source/packets/cPacket_AddToInventory.cpp
	$(CC) $(CC_OPTIONS) source/packets/cPacket_AddToInventory.cpp -c $(INCLUDE) -o build/cPacket_AddToInventory.o
	
build/cPacket_ArmAnim.o : source/packets/cPacket_ArmAnim.cpp
	$(CC) $(CC_OPTIONS) source/packets/cPacket_ArmAnim.cpp -c $(INCLUDE) -o build/cPacket_ArmAnim.o

build/cPacket_BlockChange.o : source/packets/cPacket_BlockChange.cpp
	$(CC) $(CC_OPTIONS) source/packets/cPacket_BlockChange.cpp -c $(INCLUDE) -o build/cPacket_BlockChange.o
	
build/cPacket_BlockAction.o : source/packets/cPacket_BlockAction.cpp
	$(CC) $(CC_OPTIONS) source/packets/cPacket_BlockAction.cpp -c $(INCLUDE) -o build/cPacket_BlockAction.o
	
build/cPacket_Explosion.o : source/packets/cPacket_Explosion.cpp
	$(CC) $(CC_OPTIONS) source/packets/cPacket_Explosion.cpp -c $(INCLUDE) -o build/cPacket_Explosion.o
	
build/cPacket_SoundEffect.o : source/packets/cPacket_SoundEffect.cpp
	$(CC) $(CC_OPTIONS) source/packets/cPacket_SoundEffect.cpp -c $(INCLUDE) -o build/cPacket_SoundEffect.o
	
build/cPacket_BlockDig.o : source/packets/cPacket_BlockDig.cpp
	$(CC) $(CC_OPTIONS) source/packets/cPacket_BlockDig.cpp -c $(INCLUDE) -o build/cPacket_BlockDig.o
	
build/cPacket_BlockPlace.o : source/packets/cPacket_BlockPlace.cpp
	$(CC) $(CC_OPTIONS) source/packets/cPacket_BlockPlace.cpp -c $(INCLUDE) -o build/cPacket_BlockPlace.o
	
build/cPacket_Chat.o : source/packets/cPacket_Chat.cpp
	$(CC) $(CC_OPTIONS) source/packets/cPacket_Chat.cpp -c $(INCLUDE) -o build/cPacket_Chat.o

build/cPacket_CollectItem.o : source/packets/cPacket_CollectItem.cpp
	$(CC) $(CC_OPTIONS) source/packets/cPacket_CollectItem.cpp -c $(INCLUDE) -o build/cPacket_CollectItem.o

build/cPacket_DestroyEntity.o : source/packets/cPacket_DestroyEntity.cpp
	$(CC) $(CC_OPTIONS) source/packets/cPacket_DestroyEntity.cpp -c $(INCLUDE) -o build/cPacket_DestroyEntity.o

build/cPacket_Disconnect.o : source/packets/cPacket_Disconnect.cpp
	$(CC) $(CC_OPTIONS) source/packets/cPacket_Disconnect.cpp -c $(INCLUDE) -o build/cPacket_Disconnect.o

build/cPacket_EntityEquipment.o : source/packets/cPacket_EntityEquipment.cpp
	$(CC) $(CC_OPTIONS) source/packets/cPacket_EntityEquipment.cpp -c $(INCLUDE) -o build/cPacket_EntityEquipment.o

build/cPacket_CreateInventoryAction.o : source/packets/cPacket_CreateInventoryAction.cpp
	$(CC) $(CC_OPTIONS) source/packets/cPacket_CreateInventoryAction.cpp -c $(INCLUDE) -o build/cPacket_CreateInventoryAction.o

build/cPacket_NewInvalidState.o : source/packets/cPacket_NewInvalidState.cpp
	$(CC) $(CC_OPTIONS) source/packets/cPacket_NewInvalidState.cpp -c $(INCLUDE) -o build/cPacket_NewInvalidState.o

build/cPacket_Thunderbolt.o : source/packets/cPacket_Thunderbolt.cpp
	$(CC) $(CC_OPTIONS) source/packets/cPacket_Thunderbolt.cpp -c $(INCLUDE) -o build/cPacket_Thunderbolt.o

build/cPacket_EntityLook.o : source/packets/cPacket_EntityLook.cpp
	$(CC) $(CC_OPTIONS) source/packets/cPacket_EntityLook.cpp -c $(INCLUDE) -o build/cPacket_EntityLook.o

build/cPacket_EntityStatus.o : source/packets/cPacket_EntityStatus.cpp
	$(CC) $(CC_OPTIONS) source/packets/cPacket_EntityStatus.cpp -c $(INCLUDE) -o build/cPacket_EntityStatus.o

build/cPacket_Flying.o : source/packets/cPacket_Flying.cpp
	$(CC) $(CC_OPTIONS) source/packets/cPacket_Flying.cpp -c $(INCLUDE) -o build/cPacket_Flying.o

build/cPacket_Handshake.o : source/packets/cPacket_Handshake.cpp
	$(CC) $(CC_OPTIONS) source/packets/cPacket_Handshake.cpp -c $(INCLUDE) -o build/cPacket_Handshake.o

build/cPacket_InventoryProgressBar.o : source/packets/cPacket_InventoryProgressBar.cpp
	$(CC) $(CC_OPTIONS) source/packets/cPacket_InventoryProgressBar.cpp -c $(INCLUDE) -o build/cPacket_InventoryProgressBar.o

build/cPacket_InventorySlot.o : source/packets/cPacket_InventorySlot.cpp
	$(CC) $(CC_OPTIONS) source/packets/cPacket_InventorySlot.cpp -c $(INCLUDE) -o build/cPacket_InventorySlot.o

build/cPacket_ItemData.o : source/packets/cPacket_ItemData.cpp
	$(CC) $(CC_OPTIONS) source/packets/cPacket_ItemData.cpp -c $(INCLUDE) -o build/cPacket_ItemData.o

build/cPacket_ItemSwitch.o : source/packets/cPacket_ItemSwitch.cpp
	$(CC) $(CC_OPTIONS) source/packets/cPacket_ItemSwitch.cpp -c $(INCLUDE) -o build/cPacket_ItemSwitch.o

build/cPacket_KeepAlive.o : source/packets/cPacket_KeepAlive.cpp
	$(CC) $(CC_OPTIONS) source/packets/cPacket_KeepAlive.cpp -c $(INCLUDE) -o build/cPacket_KeepAlive.o

build/cPacket_Login.o : source/packets/cPacket_Login.cpp
	$(CC) $(CC_OPTIONS) source/packets/cPacket_Login.cpp -c $(INCLUDE) -o build/cPacket_Login.o

build/cPacket_MapChunk.o : source/packets/cPacket_MapChunk.cpp
	$(CC) $(CC_OPTIONS) source/packets/cPacket_MapChunk.cpp -c $(INCLUDE) -o build/cPacket_MapChunk.o

build/cPacket_Metadata.o : source/packets/cPacket_Metadata.cpp
	$(CC) $(CC_OPTIONS) source/packets/cPacket_Metadata.cpp -c $(INCLUDE) -o build/cPacket_Metadata.o

build/cPacket_MultiBlock.o : source/packets/cPacket_MultiBlock.cpp
	$(CC) $(CC_OPTIONS) source/packets/cPacket_MultiBlock.cpp -c $(INCLUDE) -o build/cPacket_MultiBlock.o

build/cPacket_NamedEntitySpawn.o : source/packets/cPacket_NamedEntitySpawn.cpp
	$(CC) $(CC_OPTIONS) source/packets/cPacket_NamedEntitySpawn.cpp -c $(INCLUDE) -o build/cPacket_NamedEntitySpawn.o

build/cPacket_PickupSpawn.o : source/packets/cPacket_PickupSpawn.cpp
	$(CC) $(CC_OPTIONS) source/packets/cPacket_PickupSpawn.cpp -c $(INCLUDE) -o build/cPacket_PickupSpawn.o

build/cPacket_PlayerListItem.o : source/packets/cPacket_PlayerListItem.cpp
	$(CC) $(CC_OPTIONS) source/packets/cPacket_PlayerListItem.cpp -c $(INCLUDE) -o build/cPacket_PlayerListItem.o

build/cPacket_PlayerLook.o : source/packets/cPacket_PlayerLook.cpp
	$(CC) $(CC_OPTIONS) source/packets/cPacket_PlayerLook.cpp -c $(INCLUDE) -o build/cPacket_PlayerLook.o

build/cPacket_PlayerMoveLook.o : source/packets/cPacket_PlayerMoveLook.cpp
	$(CC) $(CC_OPTIONS) source/packets/cPacket_PlayerMoveLook.cpp -c $(INCLUDE) -o build/cPacket_PlayerMoveLook.o

build/cPacket_PlayerPosition.o : source/packets/cPacket_PlayerPosition.cpp
	$(CC) $(CC_OPTIONS) source/packets/cPacket_PlayerPosition.cpp -c $(INCLUDE) -o build/cPacket_PlayerPosition.o

build/cPacket_PreChunk.o : source/packets/cPacket_PreChunk.cpp
	$(CC) $(CC_OPTIONS) source/packets/cPacket_PreChunk.cpp -c $(INCLUDE) -o build/cPacket_PreChunk.o

build/cPacket_RelativeEntityMove.o : source/packets/cPacket_RelativeEntityMove.cpp
	$(CC) $(CC_OPTIONS) source/packets/cPacket_RelativeEntityMove.cpp -c $(INCLUDE) -o build/cPacket_RelativeEntityMove.o

build/cPacket_RelativeEntityMoveLook.o : source/packets/cPacket_RelativeEntityMoveLook.cpp
	$(CC) $(CC_OPTIONS) source/packets/cPacket_RelativeEntityMoveLook.cpp -c $(INCLUDE) -o build/cPacket_RelativeEntityMoveLook.o

build/cPacket_Respawn.o : source/packets/cPacket_Respawn.cpp
	$(CC) $(CC_OPTIONS) source/packets/cPacket_Respawn.cpp -c $(INCLUDE) -o build/cPacket_Respawn.o

build/cPacket_SpawnMob.o : source/packets/cPacket_SpawnMob.cpp
	$(CC) $(CC_OPTIONS) source/packets/cPacket_SpawnMob.cpp -c $(INCLUDE) -o build/cPacket_SpawnMob.o

build/cPacket_TeleportEntity.o : source/packets/cPacket_TeleportEntity.cpp
	$(CC) $(CC_OPTIONS) source/packets/cPacket_TeleportEntity.cpp -c $(INCLUDE) -o build/cPacket_TeleportEntity.o

build/cPacket_TimeUpdate.o : source/packets/cPacket_TimeUpdate.cpp
	$(CC) $(CC_OPTIONS) source/packets/cPacket_TimeUpdate.cpp -c $(INCLUDE) -o build/cPacket_TimeUpdate.o

build/cPacket_UpdateHealth.o : source/packets/cPacket_UpdateHealth.cpp
	$(CC) $(CC_OPTIONS) source/packets/cPacket_UpdateHealth.cpp -c $(INCLUDE) -o build/cPacket_UpdateHealth.o

build/cPacket_UpdateSign.o : source/packets/cPacket_UpdateSign.cpp
	$(CC) $(CC_OPTIONS) source/packets/cPacket_UpdateSign.cpp -c $(INCLUDE) -o build/cPacket_UpdateSign.o

build/cPacket_UseEntity.o : source/packets/cPacket_UseEntity.cpp
	$(CC) $(CC_OPTIONS) source/packets/cPacket_UseEntity.cpp -c $(INCLUDE) -o build/cPacket_UseEntity.o

build/cPacket_WholeInventory.o : source/packets/cPacket_WholeInventory.cpp
	$(CC) $(CC_OPTIONS) source/packets/cPacket_WholeInventory.cpp -c $(INCLUDE) -o build/cPacket_WholeInventory.o

build/cPacket_WindowClick.o : source/packets/cPacket_WindowClick.cpp
	$(CC) $(CC_OPTIONS) source/packets/cPacket_WindowClick.cpp -c $(INCLUDE) -o build/cPacket_WindowClick.o

build/cPacket_WindowClose.o : source/packets/cPacket_WindowClose.cpp
	$(CC) $(CC_OPTIONS) source/packets/cPacket_WindowClose.cpp -c $(INCLUDE) -o build/cPacket_WindowClose.o

build/cPacket_WindowOpen.o : source/packets/cPacket_WindowOpen.cpp
	$(CC) $(CC_OPTIONS) source/packets/cPacket_WindowOpen.cpp -c $(INCLUDE) -o build/cPacket_WindowOpen.o

# Item # 62 -- cPacket --
build/cPacket.o : source/packets/cPacket.cpp
	$(CC) $(CC_OPTIONS) source/packets/cPacket.cpp -c $(INCLUDE) -o build/cPacket.o


# Item # 63 -- base64 --
build/base64.o : WebServer/base64.cpp
	$(CC) $(CC_OPTIONS) WebServer/base64.cpp -c $(INCLUDE) -o build/base64.o


# Item # 64 -- Socket --
build/Socket.o : WebServer/Socket.cpp
	$(CC) $(CC_OPTIONS) WebServer/Socket.cpp -c $(INCLUDE) -o build/Socket.o


# Item # 65 -- StdHelpers --
build/StdHelpers.o : WebServer/StdHelpers.cpp
	$(CC) $(CC_OPTIONS) WebServer/StdHelpers.cpp -c $(INCLUDE) -o build/StdHelpers.o


# Item # 66 -- UrlHelper --
build/UrlHelper.o : WebServer/UrlHelper.cpp
	$(CC) $(CC_OPTIONS) WebServer/UrlHelper.cpp -c $(INCLUDE) -o build/UrlHelper.o


# Item # 67 -- WebServer --
build/WebServer.o : WebServer/WebServer.cpp
	$(CC) $(CC_OPTIONS) WebServer/WebServer.cpp -c $(INCLUDE) -o build/WebServer.o


# Item # 68 -- tolua_event --
build/tolua_event.o : tolua++-1.0.93/src/lib/tolua_event.c
	$(CC) $(CCE_OPTIONS) tolua++-1.0.93/src/lib/tolua_event.c -c $(INCLUDE) -o build/tolua_event.o


# Item # 69 -- tolua_is --
build/tolua_is.o : tolua++-1.0.93/src/lib/tolua_is.c
	$(CC) $(CCE_OPTIONS) tolua++-1.0.93/src/lib/tolua_is.c -c $(INCLUDE) -o build/tolua_is.o


# Item # 70 -- tolua_map --
build/tolua_map.o : tolua++-1.0.93/src/lib/tolua_map.c
	$(CC) $(CCE_OPTIONS) tolua++-1.0.93/src/lib/tolua_map.c -c $(INCLUDE) -o build/tolua_map.o


# Item # 71 -- tolua_push --
build/tolua_push.o : tolua++-1.0.93/src/lib/tolua_push.c
	$(CC) $(CCE_OPTIONS) tolua++-1.0.93/src/lib/tolua_push.c -c $(INCLUDE) -o build/tolua_push.o


# Item # 72 -- tolua_to --
build/tolua_to.o : tolua++-1.0.93/src/lib/tolua_to.c
	$(CC) $(CCE_OPTIONS) tolua++-1.0.93/src/lib/tolua_to.c -c $(INCLUDE) -o build/tolua_to.o


# Item # 73 -- lapi --
build/lapi.o : lua-5.1.4/src/lapi.c
	$(CC) $(CCE_OPTIONS) lua-5.1.4/src/lapi.c -c $(INCLUDE) -o build/lapi.o


# Item # 74 -- lauxlib --
build/lauxlib.o : lua-5.1.4/src/lauxlib.c
	$(CC) $(CCE_OPTIONS) lua-5.1.4/src/lauxlib.c -c $(INCLUDE) -o build/lauxlib.o


# Item # 75 -- lbaselib --
build/lbaselib.o : lua-5.1.4/src/lbaselib.c
	$(CC) $(CCE_OPTIONS) lua-5.1.4/src/lbaselib.c -c $(INCLUDE) -o build/lbaselib.o


# Item # 76 -- lcode --
build/lcode.o : lua-5.1.4/src/lcode.c
	$(CC) $(CCE_OPTIONS) lua-5.1.4/src/lcode.c -c $(INCLUDE) -o build/lcode.o


# Item # 77 -- ldblib --
build/ldblib.o : lua-5.1.4/src/ldblib.c
	$(CC) $(CCE_OPTIONS) lua-5.1.4/src/ldblib.c -c $(INCLUDE) -o build/ldblib.o


# Item # 78 -- ldebug --
build/ldebug.o : lua-5.1.4/src/ldebug.c
	$(CC) $(CCE_OPTIONS) lua-5.1.4/src/ldebug.c -c $(INCLUDE) -o build/ldebug.o


# Item # 79 -- ldo --
build/ldo.o : lua-5.1.4/src/ldo.c
	$(CC) $(CCE_OPTIONS) lua-5.1.4/src/ldo.c -c $(INCLUDE) -o build/ldo.o


# Item # 80 -- ldump --
build/ldump.o : lua-5.1.4/src/ldump.c
	$(CC) $(CCE_OPTIONS) lua-5.1.4/src/ldump.c -c $(INCLUDE) -o build/ldump.o


# Item # 81 -- lfunc --
build/lfunc.o : lua-5.1.4/src/lfunc.c
	$(CC) $(CCE_OPTIONS) lua-5.1.4/src/lfunc.c -c $(INCLUDE) -o build/lfunc.o


# Item # 82 -- lgc --
build/lgc.o : lua-5.1.4/src/lgc.c
	$(CC) $(CCE_OPTIONS) lua-5.1.4/src/lgc.c -c $(INCLUDE) -o build/lgc.o


# Item # 83 -- linit --
build/linit.o : lua-5.1.4/src/linit.c
	$(CC) $(CCE_OPTIONS) lua-5.1.4/src/linit.c -c $(INCLUDE) -o build/linit.o


# Item # 84 -- liolib --
build/liolib.o : lua-5.1.4/src/liolib.c
	$(CC) $(CCE_OPTIONS) lua-5.1.4/src/liolib.c -c $(INCLUDE) -o build/liolib.o


# Item # 85 -- llex --
build/llex.o : lua-5.1.4/src/llex.c
	$(CC) $(CCE_OPTIONS) lua-5.1.4/src/llex.c -c $(INCLUDE) -o build/llex.o


# Item # 86 -- lmathlib --
build/lmathlib.o : lua-5.1.4/src/lmathlib.c
	$(CC) $(CCE_OPTIONS) lua-5.1.4/src/lmathlib.c -c $(INCLUDE) -o build/lmathlib.o


# Item # 87 -- lmem --
build/lmem.o : lua-5.1.4/src/lmem.c
	$(CC) $(CCE_OPTIONS) lua-5.1.4/src/lmem.c -c $(INCLUDE) -o build/lmem.o


# Item # 88 -- loadlib --
build/loadlib.o : lua-5.1.4/src/loadlib.c
	$(CC) $(CCE_OPTIONS) lua-5.1.4/src/loadlib.c -c $(INCLUDE) -o build/loadlib.o


# Item # 89 -- lobject --
build/lobject.o : lua-5.1.4/src/lobject.c
	$(CC) $(CCE_OPTIONS) lua-5.1.4/src/lobject.c -c $(INCLUDE) -o build/lobject.o


# Item # 90 -- lopcodes --
build/lopcodes.o : lua-5.1.4/src/lopcodes.c
	$(CC) $(CCE_OPTIONS) lua-5.1.4/src/lopcodes.c -c $(INCLUDE) -o build/lopcodes.o


# Item # 91 -- loslib --
build/loslib.o : lua-5.1.4/src/loslib.c
	$(CC) $(CCE_OPTIONS) lua-5.1.4/src/loslib.c -c $(INCLUDE) -o build/loslib.o


# Item # 92 -- lparser --
build/lparser.o : lua-5.1.4/src/lparser.c
	$(CC) $(CCE_OPTIONS) lua-5.1.4/src/lparser.c -c $(INCLUDE) -o build/lparser.o


# Item # 93 -- lstate --
build/lstate.o : lua-5.1.4/src/lstate.c
	$(CC) $(CCE_OPTIONS) lua-5.1.4/src/lstate.c -c $(INCLUDE) -o build/lstate.o


# Item # 94 -- lstring --
build/lstring.o : lua-5.1.4/src/lstring.c
	$(CC) $(CCE_OPTIONS) lua-5.1.4/src/lstring.c -c $(INCLUDE) -o build/lstring.o


# Item # 95 -- lstrlib --
build/lstrlib.o : lua-5.1.4/src/lstrlib.c
	$(CC) $(CCE_OPTIONS) lua-5.1.4/src/lstrlib.c -c $(INCLUDE) -o build/lstrlib.o


# Item # 96 -- ltable --
build/ltable.o : lua-5.1.4/src/ltable.c
	$(CC) $(CCE_OPTIONS) lua-5.1.4/src/ltable.c -c $(INCLUDE) -o build/ltable.o


# Item # 97 -- ltablib --
build/ltablib.o : lua-5.1.4/src/ltablib.c
	$(CC) $(CCE_OPTIONS) lua-5.1.4/src/ltablib.c -c $(INCLUDE) -o build/ltablib.o


# Item # 98 -- ltm --
build/ltm.o : lua-5.1.4/src/ltm.c
	$(CC) $(CCE_OPTIONS) lua-5.1.4/src/ltm.c -c $(INCLUDE) -o build/ltm.o


# Item # 99 -- luac --
build/luac.o : lua-5.1.4/src/luac.c
	$(CC) $(CCE_OPTIONS) lua-5.1.4/src/luac.c -c $(INCLUDE) -o build/luac.o


# Item # 100 -- lundump --
build/lundump.o : lua-5.1.4/src/lundump.c
	$(CC) $(CCE_OPTIONS) lua-5.1.4/src/lundump.c -c $(INCLUDE) -o build/lundump.o


# Item # 101 -- lvm --
build/lvm.o : lua-5.1.4/src/lvm.c
	$(CC) $(CCE_OPTIONS) lua-5.1.4/src/lvm.c -c $(INCLUDE) -o build/lvm.o


# Item # 102 -- lzio --
build/lzio.o : lua-5.1.4/src/lzio.c
	$(CC) $(CCE_OPTIONS) lua-5.1.4/src/lzio.c -c $(INCLUDE) -o build/lzio.o


# Item # 103 -- print --
build/print.o : lua-5.1.4/src/print.c
	$(CC) $(CCE_OPTIONS) lua-5.1.4/src/print.c -c $(INCLUDE) -o build/print.o


# Item # 104 -- adler32 --
build/adler32.o : zlib-1.2.5/adler32.c
	$(CC) $(CCE_OPTIONS) zlib-1.2.5/adler32.c -c $(INCLUDE) -o build/adler32.o


# Item # 105 -- compress --
build/compress.o : zlib-1.2.5/compress.c
	$(CC) $(CCE_OPTIONS) zlib-1.2.5/compress.c -c $(INCLUDE) -o build/compress.o


# Item # 106 -- crc32 --
build/crc32.o : zlib-1.2.5/crc32.c
	$(CC) $(CCE_OPTIONS) zlib-1.2.5/crc32.c -c $(INCLUDE) -o build/crc32.o


# Item # 107 -- deflate --
build/deflate.o : zlib-1.2.5/deflate.c
	$(CC) $(CCE_OPTIONS) zlib-1.2.5/deflate.c -c $(INCLUDE) -o build/deflate.o


# Item # 108 -- gzclose --
build/gzclose.o : zlib-1.2.5/gzclose.c
	$(CC) $(CCE_OPTIONS) zlib-1.2.5/gzclose.c -c $(INCLUDE) -o build/gzclose.o


# Item # 109 -- gzlib --
build/gzlib.o : zlib-1.2.5/gzlib.c
	$(CC) $(CCE_OPTIONS) zlib-1.2.5/gzlib.c -c $(INCLUDE) -o build/gzlib.o


# Item # 110 -- gzread --
build/gzread.o : zlib-1.2.5/gzread.c
	$(CC) $(CCE_OPTIONS) zlib-1.2.5/gzread.c -c $(INCLUDE) -o build/gzread.o


# Item # 111 -- gzwrite --
build/gzwrite.o : zlib-1.2.5/gzwrite.c
	$(CC) $(CCE_OPTIONS) zlib-1.2.5/gzwrite.c -c $(INCLUDE) -o build/gzwrite.o


# Item # 112 -- infback --
build/infback.o : zlib-1.2.5/infback.c
	$(CC) $(CCE_OPTIONS) zlib-1.2.5/infback.c -c $(INCLUDE) -o build/infback.o


# Item # 113 -- inffast --
build/inffast.o : zlib-1.2.5/inffast.c
	$(CC) $(CCE_OPTIONS) zlib-1.2.5/inffast.c -c $(INCLUDE) -o build/inffast.o


# Item # 114 -- inflate --
build/inflate.o : zlib-1.2.5/inflate.c
	$(CC) $(CCE_OPTIONS) zlib-1.2.5/inflate.c -c $(INCLUDE) -o build/inflate.o


# Item # 115 -- inftrees --
build/inftrees.o : zlib-1.2.5/inftrees.c
	$(CC) $(CCE_OPTIONS) zlib-1.2.5/inftrees.c -c $(INCLUDE) -o build/inftrees.o


# Item # 116 -- trees --
build/trees.o : zlib-1.2.5/trees.c
	$(CC) $(CCE_OPTIONS) zlib-1.2.5/trees.c -c $(INCLUDE) -o build/trees.o


# Item # 117 -- uncompr --
build/uncompr.o : zlib-1.2.5/uncompr.c
	$(CC) $(CCE_OPTIONS) zlib-1.2.5/uncompr.c -c $(INCLUDE) -o build/uncompr.o


# Item # 118 -- zutil --
build/zutil.o : zlib-1.2.5/zutil.c
	$(CC) $(CCE_OPTIONS) zlib-1.2.5/zutil.c -c $(INCLUDE) -o build/zutil.o


# Item # 119 -- iniFile --
build/iniFile.o : iniFile/iniFile.cpp
	$(CC) $(CC_OPTIONS) iniFile/iniFile.cpp -c $(INCLUDE) -o build/iniFile.o

# Item # 120 --Authenticator --
build/cAuthenticator.o: source/cAuthenticator.cpp
	$(CC) $(CC_OPTIONS) source/cAuthenticator.cpp -c $(INCLUDE) -o build/cAuthenticator.o

build/cBlockingTCPLink.o: source/cBlockingTCPLink.cpp
	$(CC) $(CC_OPTIONS) source/cBlockingTCPLink.cpp -c $(INCLUDE) -o build/cBlockingTCPLink.o

build/cThread.o: source/cThread.cpp
	$(CC) $(CC_OPTIONS) source/cThread.cpp -c $(INCLUDE) -o build/cThread.o

build/cSpider.o: source/cSpider.cpp
	$(CC) $(CC_OPTIONS) source/cSpider.cpp -c $(INCLUDE) -o build/cSpider.o

build/cCow.o: source/cCow.cpp
	$(CC) $(CC_OPTIONS) source/cCow.cpp -c $(INCLUDE) -o build/cCow.o

build/cPig.o: source/cPig.cpp
	$(CC) $(CC_OPTIONS) source/cPig.cpp -c $(INCLUDE) -o build/cPig.o

build/cSheep.o: source/cSheep.cpp
	$(CC) $(CC_OPTIONS) source/cSheep.cpp -c $(INCLUDE) -o build/cSheep.o

build/cSquid.o: source/cSquid.cpp
	$(CC) $(CC_OPTIONS) source/cSquid.cpp -c $(INCLUDE) -o build/cSquid.o

build/cWolf.o: source/cWolf.cpp
	$(CC) $(CC_OPTIONS) source/cWolf.cpp -c $(INCLUDE) -o build/cWolf.o

build/cZombie.o: source/cZombie.cpp
	$(CC) $(CC_OPTIONS) source/cZombie.cpp -c $(INCLUDE) -o build/cZombie.o

build/cEnderman.o: source/cEnderman.cpp
	$(CC) $(CC_OPTIONS) source/cEnderman.cpp -c $(INCLUDE) -o build/cEnderman.o

build/cCreeper.o: source/cCreeper.cpp
	$(CC) $(CC_OPTIONS) source/cCreeper.cpp -c $(INCLUDE) -o build/cCreeper.o

build/cGhast.o: source/cGhast.cpp
	$(CC) $(CC_OPTIONS) source/cGhast.cpp -c $(INCLUDE) -o build/cGhast.o

build/cCavespider.o: source/cCavespider.cpp
	$(CC) $(CC_OPTIONS) source/cCavespider.cpp -c $(INCLUDE) -o build/cCavespider.o

build/cZombiepigman.o: source/cZombiepigman.cpp
	$(CC) $(CC_OPTIONS) source/cZombiepigman.cpp -c $(INCLUDE) -o build/cZombiepigman.o

build/cSkeleton.o: source/cSkeleton.cpp
	$(CC) $(CC_OPTIONS) source/cSkeleton.cpp -c $(INCLUDE) -o build/cSkeleton.o

build/cSlime.o: source/cSlime.cpp
	$(CC) $(CC_OPTIONS) source/cSlime.cpp -c $(INCLUDE) -o build/cSlime.o

build/cSilverfish.o: source/cSilverfish.cpp
	$(CC) $(CC_OPTIONS) source/cSilverfish.cpp -c $(INCLUDE) -o build/cSilverfish.o

build/cMonsterConfig.o: source/cMonsterConfig.cpp
	$(CC) $(CC_OPTIONS) source/cMonsterConfig.cpp -c $(INCLUDE) -o build/cMonsterConfig.o

build/cSleep.o: source/cSleep.cpp
	$(CC) $(CC_OPTIONS) source/cSleep.cpp -c $(INCLUDE) -o build/cSleep.o

build/cTimer.o: source/cTimer.cpp
	$(CC) $(CC_OPTIONS) source/cTimer.cpp -c $(INCLUDE) -o build/cTimer.o

build/cNoise.o : source/cNoise.cpp
	$(CC) $(CC_OPTIONS) source/cNoise.cpp -c $(INCLUDE) -o build/cNoise.o

build/cCuboid.o : source/cCuboid.cpp
	$(CC) $(CC_OPTIONS) source/cCuboid.cpp -c $(INCLUDE) -o build/cCuboid.o

build/cGenSettings.o : source/cGenSettings.cpp
	$(CC) $(CC_OPTIONS) source/cGenSettings.cpp -c $(INCLUDE) -o build/cGenSettings.o

build/json_reader.o : jsoncpp-src-0.5.0/src/lib_json/json_reader.cpp
	$(CC) $(CC_OPTIONS) jsoncpp-src-0.5.0/src/lib_json/json_reader.cpp -c $(INCLUDE) -o build/json_reader.o

build/json_value.o : jsoncpp-src-0.5.0/src/lib_json/json_value.cpp
	$(CC) $(CC_OPTIONS) jsoncpp-src-0.5.0/src/lib_json/json_value.cpp -c $(INCLUDE) -o build/json_value.o

build/json_writer.o : jsoncpp-src-0.5.0/src/lib_json/json_writer.cpp
	$(CC) $(CC_OPTIONS) jsoncpp-src-0.5.0/src/lib_json/json_writer.cpp -c $(INCLUDE) -o build/json_writer.o

build/cMakeDir.o : source/cMakeDir.cpp
	$(CC) $(CC_OPTIONS) source/cMakeDir.cpp -c $(INCLUDE) -o build/cMakeDir.o

build/cSocket.o : source/cSocket.cpp
	$(CC) $(CC_OPTIONS) source/cSocket.cpp -c $(INCLUDE) -o build/cSocket.o	

build/cFluidSimulator.o : source/cFluidSimulator.cpp
	$(CC) $(CC_OPTIONS) source/cFluidSimulator.cpp -c $(INCLUDE) -o build/cFluidSimulator.o

build/cWaterSimulator.o : source/cWaterSimulator.cpp
	$(CC) $(CC_OPTIONS) source/cWaterSimulator.cpp -c $(INCLUDE) -o build/cWaterSimulator.o	

build/cLavaSimulator.o : source/cLavaSimulator.cpp
	$(CC) $(CC_OPTIONS) source/cLavaSimulator.cpp -c $(INCLUDE) -o build/cLavaSimulator.o	

build/cFileFormatUpdater.o : source/cFileFormatUpdater.cpp
	$(CC) $(CC_OPTIONS) source/cFileFormatUpdater.cpp -c $(INCLUDE) -o build/cFileFormatUpdater.o	

build/cItem.o : source/cItem.cpp
	$(CC) $(CC_OPTIONS) source/cItem.cpp -c $(INCLUDE) -o build/cItem.o	


##### END RUN ####
