return
{
	cWorld =
	{
		Functions =
		{
			AreCommandBlocksEnabled =
			{
				{
					Params =
					{
					},
					Returns =
					{
						{
							Type = "boolean",
						},
					},
				},
			},
			BroadcastChat =
			{
				{
					Params =
					{
						{
							Type = "string",
							Name = "a_Message",
						},
						{
							Type = "cClientHandle",
							Name = "a_Exclude",
						},
						{
							Type = "eMessageType",
							Name = "a_ChatPrefix",
						},
					},
					Returns =
					{
						{
							Type = "string",
						},
					},
					Desc = "Retrieves the world height at the specified coords; returns nullopt if chunk not loaded / generated",
				},
				{
					Params =
					{
						{
							Type = "cCompositeChat",
							Name = "a_Message",
						},
						{
							Type = "cClientHandle",
							Name = "a_Exclude",
						},
					},
					Returns =
					{
					},
				},
			},
			BroadcastChatDeath =
			{
				{
					Params =
					{
						{
							Type = "string",
							Name = "a_Message",
						},
						{
							Type = "cClientHandle",
							Name = "a_Exclude",
						},
					},
					Returns =
					{
						{
							Type = "string",
						},
					},
				},
			},
			BroadcastChatFailure =
			{
				{
					Params =
					{
						{
							Type = "string",
							Name = "a_Message",
						},
						{
							Type = "cClientHandle",
							Name = "a_Exclude",
						},
					},
					Returns =
					{
						{
							Type = "string",
						},
					},
				},
			},
			BroadcastChatFatal =
			{
				{
					Params =
					{
						{
							Type = "string",
							Name = "a_Message",
						},
						{
							Type = "cClientHandle",
							Name = "a_Exclude",
						},
					},
					Returns =
					{
						{
							Type = "string",
						},
					},
				},
			},
			BroadcastChatInfo =
			{
				{
					Params =
					{
						{
							Type = "string",
							Name = "a_Message",
						},
						{
							Type = "cClientHandle",
							Name = "a_Exclude",
						},
					},
					Returns =
					{
						{
							Type = "string",
						},
					},
				},
			},
			BroadcastChatSuccess =
			{
				{
					Params =
					{
						{
							Type = "string",
							Name = "a_Message",
						},
						{
							Type = "cClientHandle",
							Name = "a_Exclude",
						},
					},
					Returns =
					{
						{
							Type = "string",
						},
					},
				},
			},
			BroadcastChatWarning =
			{
				{
					Params =
					{
						{
							Type = "string",
							Name = "a_Message",
						},
						{
							Type = "cClientHandle",
							Name = "a_Exclude",
						},
					},
					Returns =
					{
						{
							Type = "string",
						},
					},
				},
			},
			BroadcastPlayerListHeaderFooter =
			{
				{
					Params =
					{
						{
							Type = "cCompositeChat",
							Name = "a_Header",
						},
						{
							Type = "cCompositeChat",
							Name = "a_Footer",
						},
					},
					Returns =
					{
					},
				},
			},
			CastThunderbolt =
			{
				{
					Params =
					{
						{
							Type = "Vector3d",
							Name = "a_Block",
						},
					},
					Returns =
					{
					},
					Desc = "Casts a thunderbolt at the specified coords",
				},
				{
					Params =
					{
						{
							Type = "number",
							Name = "a_BlockX",
						},
						{
							Type = "number",
							Name = "a_BlockY",
						},
						{
							Type = "number",
							Name = "a_BlockZ",
						},
					},
					Returns =
					{
					},
				},
			},
			ChangeWeather =
			{
				{
					Params =
					{
					},
					Returns =
					{
					},
					Desc = "Forces a weather change in the next game tick",
				},
			},
			DigBlock =
			{
				{
					Params =
					{
						{
							Type = "Vector3d",
							Name = "a_BlockPos",
						},
						{
							Type = "cEntity",
							Name = "a_Digger",
						},
					},
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Desc = "Replaces the specified block with air, and calls the OnBroken block handler.\
	Wakes up the simulators. Doesn't produce pickups, use DropBlockAsPickups() for that instead.\
	Returns true on success, false if the chunk is not loaded.",
				},
				{
					Params =
					{
						{
							Type = "number",
							Name = "a_X",
						},
						{
							Type = "number",
							Name = "a_Y",
						},
						{
							Type = "number",
							Name = "a_Z",
						},
						{
							Type = "cEntity",
							Name = "a_Digger",
						},
					},
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Desc = "OBSOLETE, use the Vector3-based overload instead.\
	Replaces the specified block with air, and calls the apropriate block handlers (OnBreaking(), OnBroken()).\
	Wakes up the simulators.\
	Doesn't produce pickups, use DropBlockAsPickups() for that instead.\
	Returns true on success, false if the chunk is not loaded.",
				},
			},
			DropBlockAsPickups =
			{
				{
					Params =
					{
						{
							Type = "Vector3d",
							Name = "a_BlockPos",
						},
						{
							Type = "cEntity",
							Name = "a_Digger",
						},
						{
							Type = "cItem",
							Name = "a_Tool",
						},
					},
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Desc = "Digs the specified block, and spawns the appropriate pickups for it.\
	a_Digger is an optional entity causing the digging, usually the player.\
	a_Tool is an optional item used to dig up the block, used by the handlers (empty hand vs shears produce different pickups from leaves).\
	An empty hand is assumed if a_Tool is nullptr.\
	Returns true on success, false if the chunk is not loaded.",
				},
			},
			FlushPendingBlockChanges =
			{
				{
					Params =
					{
					},
					Returns =
					{
					},
					Desc = "Construct the world and read settings from its ini file.\
	@param a_DeadlockDetect is used for tracking this world's age, detecting a possible deadlock.\
	@param a_WorldNames is a list of all world names, used to validate linked worlds",
				},
			},
			GenerateChunk =
			{
				{
					Params =
					{
						{
							Type = "number",
							Name = "a_ChunkX",
						},
						{
							Type = "number",
							Name = "a_ChunkZ",
						},
					},
					Returns =
					{
					},
					Desc = "Generates the given chunk.",
				},
			},
			GetBiomeAt =
			{
				{
					Params =
					{
						{
							Type = "number",
							Name = "a_BlockX",
						},
						{
							Type = "number",
							Name = "a_BlockZ",
						},
					},
					Returns =
					{
						{
							Type = "EMCSBiome",
						},
					},
					Desc = "Returns the biome at the specified coords. Reads the biome from the chunk, if loaded, otherwise uses the world generator to provide the biome value",
				},
			},
			GetDataPath =
			{
				{
					Params =
					{
					},
					Returns =
					{
						{
							Type = "string",
						},
					},
					Desc = "Returns the data path to the world data",
				},
			},
			GetDefaultWeathernumbererval =
			{
				{
					Params =
					{
						{
							Type = "eWeather",
							Name = "a_Weather",
						},
					},
					Returns =
					{
						{
							Type = "number",
						},
					},
					Desc = "Returns the default weather numbererval for the specific weather type.\
	Returns -1 for any unknown weather.",
				},
			},
			GetDimension =
			{
				{
					Params =
					{
					},
					Returns =
					{
						{
							Type = "eDimension",
						},
					},
				},
			},
			GetGameMode =
			{
				{
					Params =
					{
					},
					Returns =
					{
						{
							Type = "eGameMode",
						},
					},
					Desc = "Returns the current game mode. Partly OBSOLETE, you should use IsGameModeXXX() functions wherever applicable",
				},
			},
			GetGeneratorQueueLength =
			{
				{
					Params =
					{
					},
					Returns =
					{
						{
							Type = "number",
						},
					},
					Desc = "Returns the number of chunks loaded and dirty, and in the lighting queue",
				},
			},
			GetIniFileName =
			{
				{
					Params =
					{
					},
					Returns =
					{
						{
							Type = "string",
						},
					},
					Desc = "Returns the name of the world.ini file used by this world",
				},
			},
			GetLightingQueueLength =
			{
				{
					Params =
					{
					},
					Returns =
					{
						{
							Type = "number",
						},
					},
				},
			},
			GetLinkedEndWorldName =
			{
				{
					Params =
					{
					},
					Returns =
					{
						{
							Type = "string",
						},
					},
				},
			},
			GetLinkedNetherWorldName =
			{
				{
					Params =
					{
					},
					Returns =
					{
						{
							Type = "string",
						},
					},
				},
			},
			GetLinkedOverworldName =
			{
				{
					Params =
					{
					},
					Returns =
					{
						{
							Type = "string",
						},
					},
				},
			},
			GetMapManager =
			{
				{
					Params =
					{
					},
					Returns =
					{
						{
							Type = "cMapManager",
						},
					},
					Desc = "Returns the associated map manager instance.",
				},
			},
			GetMaxCactusHeight =
			{
				{
					Params =
					{
					},
					Returns =
					{
						{
							Type = "number",
						},
					},
				},
			},
			GetMaxNetherPortalHeight =
			{
				{
					Params =
					{
					},
					Returns =
					{
						{
							Type = "number",
						},
					},
				},
			},
			GetMaxNetherPortalWidth =
			{
				{
					Params =
					{
					},
					Returns =
					{
						{
							Type = "number",
						},
					},
				},
			},
			GetMaxSugarcaneHeight =
			{
				{
					Params =
					{
					},
					Returns =
					{
						{
							Type = "number",
						},
					},
				},
			},
			GetMaxViewDistance =
			{
				{
					Params =
					{
					},
					Returns =
					{
						{
							Type = "number",
						},
					},
				},
			},
			GetMinNetherPortalHeight =
			{
				{
					Params =
					{
					},
					Returns =
					{
						{
							Type = "number",
						},
					},
					Desc = "Returns or sets the minumim or maximum netherportal height",
				},
			},
			GetMinNetherPortalWidth =
			{
				{
					Params =
					{
					},
					Returns =
					{
						{
							Type = "number",
						},
					},
					Desc = "Returns or sets the minumim or maximum netherportal width",
				},
			},
			GetName =
			{
				{
					Params =
					{
					},
					Returns =
					{
						{
							Type = "string",
						},
					},
					Desc = "Returns the name of the world",
				},
			},
			GetNumChunks =
			{
				{
					Params =
					{
					},
					Returns =
					{
						{
							Type = "number",
						},
					},
					Desc = "Returns the number of chunks loaded",
				},
			},
			GetNumUnusedDirtyChunks =
			{
				{
					Params =
					{
					},
					Returns =
					{
						{
							Type = "number",
						},
					},
					Desc = "Returns the number of unused dirty chunks. That's the number of chunks that we can save and then unload.",
				},
			},
			GetScoreBoard =
			{
				{
					Params =
					{
					},
					Returns =
					{
						{
							Type = "cScoreboard",
						},
					},
					Desc = "Returns the associated scoreboard instance.",
				},
			},
			GetSeed =
			{
				{
					Params =
					{
					},
					Returns =
					{
						{
							Type = "number",
						},
					},
					Desc = "Returns the seed of the world.",
				},
			},
			GetSpawnPos =
			{
				{
					Params =
					{
					},
					Returns =
					{
						{
							Type = "Vector3d",
						},
					},
				},
			},
			GetSpawnX =
			{
				{
					Params =
					{
					},
					Returns =
					{
						{
							Type = "number",
						},
					},
				},
			},
			GetSpawnY =
			{
				{
					Params =
					{
					},
					Returns =
					{
						{
							Type = "number",
						},
					},
				},
			},
			GetSpawnZ =
			{
				{
					Params =
					{
					},
					Returns =
					{
						{
							Type = "number",
						},
					},
				},
			},
			GetStorageLoadQueueLength =
			{
				{
					Params =
					{
					},
					Returns =
					{
						{
							Type = "number",
						},
					},
				},
			},
			GetStorageSaveQueueLength =
			{
				{
					Params =
					{
					},
					Returns =
					{
						{
							Type = "number",
						},
					},
				},
			},
			GetTNTShrapnelLevel =
			{
				{
					Params =
					{
					},
					Returns =
					{
						{
							Type = "eShrapnelLevel",
						},
					},
				},
			},
			GetTicksUntilWeatherChange =
			{
				{
					Params =
					{
					},
					Returns =
					{
						{
							Type = "number",
						},
					},
				},
			},
			GetWeather =
			{
				{
					Params =
					{
					},
					Returns =
					{
						{
							Type = "eWeather",
						},
					},
					Desc = "Returns the current weather. Instead of comparing values directly to the weather constants, use IsWeatherXXX() functions, if possible",
				},
			},
			GrowPlantAt =
			{
				{
					Params =
					{
						{
							Type = "Vector3d",
							Name = "a_BlockPos",
						},
						{
							Type = "number",
							Name = "a_NumStages",
						},
					},
					Returns =
					{
						{
							Type = "number",
						},
					},
					Desc = "Grows the plant at the specified position by at most a_NumStages.\
	The block's Grow handler is invoked.\
	Returns the number of stages the plant has grown, 0 if not a plant.",
				},
			},
			GrowRipePlant =
			{
				{
					Params =
					{
						{
							Type = "Vector3d",
							Name = "a_BlockPos",
						},
					},
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Desc = "Grows the plant at the specified block to its ripe stage.\
	Returns true if grown, false if not (invalid chunk, non-growable block, already ripe).",
				},
				{
					Params =
					{
						{
							Type = "number",
							Name = "a_BlockX",
						},
						{
							Type = "number",
							Name = "a_BlockY",
						},
						{
							Type = "number",
							Name = "a_BlockZ",
						},
						{
							Type = "boolean",
							Name = "a_IsByBonemeal",
						},
					},
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Desc = "OBSOLETE, use the Vector3-based overload instead.\
	Grows the plant at the specified block to its ripe stage.\
	a_IsByBonemeal is obsolete, do not use.\
	Returns true if grown, false if not (invalid chunk, non-growable block, already ripe).",
				},
			},
			IsBlockDirectlyWatered =
			{
				{
					Params =
					{
						{
							Type = "number",
							Name = "a_BlockX",
						},
						{
							Type = "number",
							Name = "a_BlockY",
						},
						{
							Type = "number",
							Name = "a_BlockZ",
						},
					},
					Returns =
					{
						{
							Type = "boolean",
						},
					},
				},
			},
			IsDaylightCycleEnabled =
			{
				{
					Params =
					{
					},
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Desc = "Is the daylight cycle enabled?",
				},
			},
			IsDeepSnowEnabled =
			{
				{
					Params =
					{
					},
					Returns =
					{
						{
							Type = "boolean",
						},
					},
				},
			},
			IsFarmlandTramplingEnabled =
			{
				{
					Params =
					{
					},
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Desc = "Returns true if farmland trampling is enabled",
				},
			},
			IsGameModeAdventure =
			{
				{
					Params =
					{
					},
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Desc = "Returns true if the world is in Adventure mode",
				},
			},
			IsGameModeCreative =
			{
				{
					Params =
					{
					},
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Desc = "Returns true if the world is in Creative mode",
				},
			},
			IsGameModeSpectator =
			{
				{
					Params =
					{
					},
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Desc = "Returns true if the world is in Spectator mode",
				},
			},
			IsGameModeSurvival =
			{
				{
					Params =
					{
					},
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Desc = "Returns true if the world is in Survival mode",
				},
			},
			IsPVPEnabled =
			{
				{
					Params =
					{
					},
					Returns =
					{
						{
							Type = "boolean",
						},
					},
				},
			},
			IsSavingEnabled =
			{
				{
					Params =
					{
					},
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Desc = "Get whether saving chunks is enabled",
				},
			},
			IsSlimeChunk =
			{
				{
					Params =
					{
						{
							Type = "number",
							Name = "a_ChunkX",
						},
						{
							Type = "number",
							Name = "a_ChunkZ",
						},
					},
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Desc = "Returns true if slimes should spawn in the chunk.",
				},
			},
			IsTrapdoorOpen =
			{
				{
					Params =
					{
						{
							Type = "number",
							Name = "a_BlockX",
						},
						{
							Type = "number",
							Name = "a_BlockY",
						},
						{
							Type = "number",
							Name = "a_BlockZ",
						},
					},
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Desc = "Is the trapdoor open? Returns false if there is no trapdoor at the specified coords.",
				},
			},
			IsWeatherRain =
			{
				{
					Params =
					{
					},
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Desc = "Returns true if the current weather is rainy.",
				},
			},
			IsWeatherRainAt =
			{
				{
					Params =
					{
						{
							Type = "number",
							Name = "a_BlockX",
						},
						{
							Type = "number",
							Name = "a_BlockZ",
						},
					},
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Desc = "Returns true if it is raining at the specified location. This takes numbero account biomes.",
				},
			},
			IsWeatherStorm =
			{
				{
					Params =
					{
					},
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Desc = "Returns true if the current weather is stormy.",
				},
			},
			IsWeatherStormAt =
			{
				{
					Params =
					{
						{
							Type = "number",
							Name = "a_BlockX",
						},
						{
							Type = "number",
							Name = "a_BlockZ",
						},
					},
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Desc = "Returns true if the weather is stormy at the specified location. This takes numbero account biomes.",
				},
			},
			IsWeatherSunny =
			{
				{
					Params =
					{
					},
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Desc = "Returns true if the current weather is sunny.",
				},
			},
			IsWeatherSunnyAt =
			{
				{
					Params =
					{
						{
							Type = "number",
							Name = "a_BlockX",
						},
						{
							Type = "number",
							Name = "a_BlockZ",
						},
					},
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Desc = "Returns true if it is sunny at the specified location. This takes numbero account biomes.",
				},
			},
			IsWeatherWet =
			{
				{
					Params =
					{
					},
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Desc = "Returns true if the world currently has any precipitation - rain, storm or snow.",
				},
			},
			IsWeatherWetAt =
			{
				{
					Params =
					{
						{
							Type = "number",
							Name = "a_BlockX",
						},
						{
							Type = "number",
							Name = "a_BlockZ",
						},
					},
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Desc = "Returns true if it is raining or storming at the specified location.\
	This takes numbero account biomes.",
				},
			},
			IsWeatherWetAtXYZ =
			{
				{
					Params =
					{
						{
							Type = "Vector3d",
							Name = "a_Position",
						},
					},
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Desc = "Returns true if it is raining or storming at the specified location,\
	and the rain reaches (the bottom of) the specified block position.",
				},
			},
			PickupsFromBlock =
			{
				{
					Params =
					{
						{
							Type = "Vector3d",
							Name = "a_BlockPos",
						},
						{
							Type = "cEntity",
							Name = "a_Digger",
						},
						{
							Type = "cItem",
							Name = "a_Tool",
						},
					},
					Returns =
					{
						{
							Type = "cItems",
						},
					},
					Desc = "Returns all the pickups that would result if the a_Digger dug up the block at a_BlockPos using a_Tool\
	a_Digger is usually a player, but can be nullptr for natural causes.\
	a_Tool is an optional item used to dig up the block, used by the handlers (empty hand vs shears produce different pickups from leaves).\
	An empty hand is assumed if a_Tool is nullptr.\
	Returns an empty cItems object if the chunk is not present.",
				},
			},
			QueueBlockForTick =
			{
				{
					Params =
					{
						{
							Type = "number",
							Name = "a_BlockX",
						},
						{
							Type = "number",
							Name = "a_BlockY",
						},
						{
							Type = "number",
							Name = "a_BlockZ",
						},
						{
							Type = "number",
							Name = "a_TicksToWait",
						},
					},
					Returns =
					{
					},
					Desc = "Queues the block to be ticked after the specified number of game ticks",
				},
			},
			QueueSaveAllChunks =
			{
				{
					Params =
					{
					},
					Returns =
					{
					},
					Desc = "Queues a task to save all chunks onto the tick thread. The prefferred way of saving chunks from external sources",
				},
			},
			QueueUnloadUnusedChunks =
			{
				{
					Params =
					{
					},
					Returns =
					{
					},
					Desc = "Queues a task to unload unused chunks onto the tick thread. The prefferred way of unloading.",
				},
			},
			RegenerateChunk =
			{
				{
					Params =
					{
						{
							Type = "number",
							Name = "a_ChunkX",
						},
						{
							Type = "number",
							Name = "a_ChunkZ",
						},
					},
					Returns =
					{
					},
					Desc = "Regenerate the given chunk.",
				},
			},
			SendBlockTo =
			{
				{
					Params =
					{
						{
							Type = "number",
							Name = "a_X",
						},
						{
							Type = "number",
							Name = "a_Y",
						},
						{
							Type = "number",
							Name = "a_Z",
						},
						{
							Type = "cPlayer",
							Name = "a_Player",
						},
					},
					Returns =
					{
					},
					Desc = "Sends the block at the specified coords to the player.\
	Used mainly when plugins disable block-placing or block-breaking, to restore the previous block.",
				},
			},
			SetAreaBiome =
			{
				{
					Params =
					{
						{
							Type = "number",
							Name = "a_MinX",
						},
						{
							Type = "number",
							Name = "a_MaxX",
						},
						{
							Type = "number",
							Name = "a_MinZ",
						},
						{
							Type = "number",
							Name = "a_MaxZ",
						},
						{
							Type = "EMCSBiome",
							Name = "a_Biome",
						},
					},
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Desc = "Sets the biome at the area. Returns true if successful, false if any subarea failed (chunk not loaded).\
	(Re)sends the chunks to their relevant clients if successful.",
				},
				{
					Params =
					{
						{
							Type = "cCuboid",
							Name = "a_Area",
						},
						{
							Type = "EMCSBiome",
							Name = "a_Biome",
						},
					},
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Desc = "Sets the biome at the area. Returns true if successful, false if any subarea failed (chunk not loaded).\
	(Re)sends the chunks to their relevant clients if successful.\
	The cuboid needn't be sorted.",
				},
			},
			SetBiomeAt =
			{
				{
					Params =
					{
						{
							Type = "number",
							Name = "a_BlockX",
						},
						{
							Type = "number",
							Name = "a_BlockZ",
						},
						{
							Type = "EMCSBiome",
							Name = "a_Biome",
						},
					},
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Desc = "Sets the biome at the specified coords. Returns true if successful, false if not (chunk not loaded).\
	Doesn't resend the chunk to clients, use ForceSendChunkTo() for that.",
				},
			},
			SetChunkAlwaysTicked =
			{
				{
					Params =
					{
						{
							Type = "number",
							Name = "a_ChunkX",
						},
						{
							Type = "number",
							Name = "a_ChunkZ",
						},
						{
							Type = "boolean",
							Name = "a_AlwaysTicked",
						},
					},
					Returns =
					{
					},
					Desc = "Increments (a_AlwaysTicked == true) or decrements (false) the m_AlwaysTicked counter for the specified chunk.\
	If the m_AlwaysTicked counter is greater than zero, the chunk is ticked in the tick-thread regardless of\
	whether it has any clients or not.\
	This function allows nesting and task-concurrency (multiple separate tasks can request ticking and as long\
	as at least one requests is active the chunk will be ticked).",
				},
			},
			SetCommandBlockCommand =
			{
				{
					Params =
					{
						{
							Type = "number",
							Name = "a_BlockX",
						},
						{
							Type = "number",
							Name = "a_BlockY",
						},
						{
							Type = "number",
							Name = "a_BlockZ",
						},
						{
							Type = "string",
							Name = "a_Command",
						},
					},
					Returns =
					{
						{
							Type = "boolean",
						},
						{
							Type = "string",
						},
					},
					Desc = "Sets the command block command. Returns true if command changed.",
				},
			},
			SetCommandBlocksEnabled =
			{
				{
					Params =
					{
						{
							Type = "boolean",
							Name = "a_Flag",
						},
					},
					Returns =
					{
					},
				},
			},
			SetDaylightCycleEnabled =
			{
				{
					Params =
					{
						{
							Type = "boolean",
							Name = "a_IsDaylightCycleEnabled",
						},
					},
					Returns =
					{
					},
					Desc = "Sets the daylight cycle to true / false.",
				},
			},
			SetLinkedEndWorldName =
			{
				{
					Params =
					{
						{
							Type = "string",
							Name = "a_Name",
						},
					},
					Returns =
					{
						{
							Type = "string",
						},
					},
				},
			},
			SetLinkedNetherWorldName =
			{
				{
					Params =
					{
						{
							Type = "string",
							Name = "a_Name",
						},
					},
					Returns =
					{
						{
							Type = "string",
						},
					},
				},
			},
			SetLinkedOverworldName =
			{
				{
					Params =
					{
						{
							Type = "string",
							Name = "a_Name",
						},
					},
					Returns =
					{
						{
							Type = "string",
						},
					},
				},
			},
			SetMaxNetherPortalHeight =
			{
				{
					Params =
					{
						{
							Type = "number",
							Name = "a_NewMaxHeight",
						},
					},
					Returns =
					{
					},
				},
			},
			SetMaxNetherPortalWidth =
			{
				{
					Params =
					{
						{
							Type = "number",
							Name = "a_NewMaxWidth",
						},
					},
					Returns =
					{
					},
				},
			},
			SetMaxViewDistance =
			{
				{
					Params =
					{
						{
							Type = "number",
							Name = "a_MaxViewDistance",
						},
					},
					Returns =
					{
					},
				},
			},
			SetMinNetherPortalHeight =
			{
				{
					Params =
					{
						{
							Type = "number",
							Name = "a_NewMinHeight",
						},
					},
					Returns =
					{
					},
				},
			},
			SetMinNetherPortalWidth =
			{
				{
					Params =
					{
						{
							Type = "number",
							Name = "a_NewMinWidth",
						},
					},
					Returns =
					{
					},
				},
			},
			SetNextBlockToTick =
			{
				{
					Params =
					{
						{
							Type = "Vector3d",
							Name = "a_BlockPos",
						},
					},
					Returns =
					{
					},
					Desc = "Causes the specified block to be ticked on the next Tick() call.\
	Only one block coord per chunk may be set, a second call overwrites the first call",
				},
			},
			SetSavingEnabled =
			{
				{
					Params =
					{
						{
							Type = "boolean",
							Name = "a_IsSavingEnabled",
						},
					},
					Returns =
					{
					},
					Desc = "Set whether saving chunks is enabled",
				},
			},
			SetShouldUseChatPrefixes =
			{
				{
					Params =
					{
						{
							Type = "boolean",
							Name = "a_Flag",
						},
					},
					Returns =
					{
					},
				},
			},
			SetSpawn =
			{
				{
					Params =
					{
						{
							Type = "number",
							Name = "a_X",
						},
						{
							Type = "number",
							Name = "a_Y",
						},
						{
							Type = "number",
							Name = "a_Z",
						},
					},
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Desc = "Set default spawn at the given coordinates.\
	Returns false if the new spawn couldn't be stored in the INI file.",
				},
			},
			SetTNTShrapnelLevel =
			{
				{
					Params =
					{
						{
							Type = "eShrapnelLevel",
							Name = "a_Flag",
						},
					},
					Returns =
					{
					},
				},
			},
			SetTicksUntilWeatherChange =
			{
				{
					Params =
					{
						{
							Type = "number",
							Name = "a_Weathernumbererval",
						},
					},
					Returns =
					{
					},
				},
			},
			SetTrapdoorOpen =
			{
				{
					Params =
					{
						{
							Type = "number",
							Name = "a_BlockX",
						},
						{
							Type = "number",
							Name = "a_BlockY",
						},
						{
							Type = "number",
							Name = "a_BlockZ",
						},
						{
							Type = "boolean",
							Name = "a_Open",
						},
					},
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Desc = "Set the state of a trapdoor. Returns true if the trapdoor was updated, false if there was no trapdoor at those coords.",
				},
			},
			SetWeather =
			{
				{
					Params =
					{
						{
							Type = "eWeather",
							Name = "a_NewWeather",
						},
					},
					Returns =
					{
					},
					Desc = "Sets the specified weather; resets weather numbererval; asks and notifies plugins of the change",
				},
			},
			ShouldBroadcastAchievementMessages =
			{
				{
					Params =
					{
					},
					Returns =
					{
						{
							Type = "boolean",
						},
					},
				},
			},
			ShouldBroadcastDeathMessages =
			{
				{
					Params =
					{
					},
					Returns =
					{
						{
							Type = "boolean",
						},
					},
				},
			},
			ShouldLavaSpawnFire =
			{
				{
					Params =
					{
					},
					Returns =
					{
						{
							Type = "boolean",
						},
					},
				},
			},
			ShouldUseChatPrefixes =
			{
				{
					Params =
					{
					},
					Returns =
					{
						{
							Type = "boolean",
						},
					},
				},
			},
			SpawnEnderCrystal =
			{
				{
					Params =
					{
						{
							Type = "Vector3d",
							Name = "a_Pos",
						},
						{
							Type = "boolean",
							Name = "a_ShowBottom",
						},
					},
					Returns =
					{
						{
							Type = "number",
						},
					},
					Desc = "Spawns a new ender crystal at the specified block coords.\
	Returns the UniqueID of the created entity, or cEntity::INVALID_ID on failure.",
				},
			},
			SpawnExperienceOrb =
			{
				{
					Params =
					{
						{
							Type = "Vector3d",
							Name = "a_Pos",
						},
						{
							Type = "number",
							Name = "a_Reward",
						},
					},
					Returns =
					{
						{
							Type = "number",
						},
					},
					Desc = "Spawns an experience orb at the given location with the given reward.\
	Returns the UniqueID of the spawned experience orb, or cEntity::INVALID_ID on failure.",
				},
				{
					Params =
					{
						{
							Type = "number",
							Name = "a_X",
						},
						{
							Type = "number",
							Name = "a_Y",
						},
						{
							Type = "number",
							Name = "a_Z",
						},
						{
							Type = "number",
							Name = "a_Reward",
						},
					},
					Returns =
					{
						{
							Type = "number",
						},
					},
					Desc = "OBSOLETE, use the Vector3d-based overload instead.\
	Spawns an experience orb at the given location with the given reward.\
	Returns the UniqueID of the spawned experience orb, or cEntity::INVALID_ID on failure.",
				},
			},
			SpawnItemPickup =
			{
				{
					Params =
					{
						{
							Type = "Vector3d",
							Name = "a_Pos",
						},
						{
							Type = "cItem",
							Name = "a_Item",
						},
						{
							Type = "Vector3f",
							Name = "a_Speed",
						},
						{
							Type = "number",
							Name = "a_LifetimeTicks",
						},
						{
							Type = "boolean",
							Name = "a_CanCombine",
						},
					},
					Returns =
					{
						{
							Type = "number",
						},
					},
					Desc = "Spawns a single pickup containing the specified item.",
				},
				{
					Params =
					{
						{
							Type = "number",
							Name = "a_PosX",
						},
						{
							Type = "number",
							Name = "a_PosY",
						},
						{
							Type = "number",
							Name = "a_PosZ",
						},
						{
							Type = "cItem",
							Name = "a_Item",
						},
						{
							Type = "number",
							Name = "a_SpeedX",
						},
						{
							Type = "number",
							Name = "a_SpeedY",
						},
						{
							Type = "number",
							Name = "a_SpeedZ",
						},
						{
							Type = "number",
							Name = "a_LifetimeTicks",
						},
						{
							Type = "boolean",
							Name = "a_CanCombine",
						},
					},
					Returns =
					{
						{
							Type = "number",
						},
					},
					Desc = "OBSOLETE, use the Vector3d-based overload instead.\
	Spawns a single pickup containing the specified item.",
				},
			},
			SpawnItemPickups =
			{
				{
					Params =
					{
						{
							Type = "cItems",
							Name = "a_Pickups",
						},
						{
							Type = "Vector3d",
							Name = "a_BlockPos",
						},
						{
							Type = "number",
							Name = "a_FlyAwaySpeed",
						},
						{
							Type = "boolean",
							Name = "a_IsPlayerCreated",
						},
					},
					Returns =
					{
					},
					Desc = "Spawns item pickups for each item in the list.\
	The initial position of the pickups is at the center of the specified block, with a small random offset.\
	May compress pickups if too many entities.",
				},
				{
					Params =
					{
						{
							Type = "cItems",
							Name = "a_Pickups",
						},
						{
							Type = "Vector3d",
							Name = "a_Pos",
						},
						{
							Type = "number",
							Name = "a_FlyAwaySpeed",
						},
						{
							Type = "boolean",
							Name = "a_IsPlayerCreated",
						},
					},
					Returns =
					{
					},
					Desc = "Spawns item pickups for each item in the list.\
	May compress pickups if too many entities.",
				},
				{
					Params =
					{
						{
							Type = "cItems",
							Name = "a_Pickups",
						},
						{
							Type = "number",
							Name = "a_BlockX",
						},
						{
							Type = "number",
							Name = "a_BlockY",
						},
						{
							Type = "number",
							Name = "a_BlockZ",
						},
						{
							Type = "number",
							Name = "a_FlyAwaySpeed",
						},
						{
							Type = "boolean",
							Name = "a_IsPlayerCreated",
						},
					},
					Returns =
					{
					},
					Desc = "OBSOLETE, use the Vector3d-based overload instead.\
	Spawns item pickups for each item in the list.\
	May compress pickups if too many entities.",
				},
				{
					Params =
					{
						{
							Type = "cItems",
							Name = "a_Pickups",
						},
						{
							Type = "Vector3d",
							Name = "a_Pos",
						},
						{
							Type = "Vector3d",
							Name = "a_Speed",
						},
						{
							Type = "boolean",
							Name = "a_IsPlayerCreated",
						},
					},
					Returns =
					{
					},
					Desc = "Spawns item pickups for each item in the list. May compress pickups if too many entities. All pickups get the speed specified.",
				},
				{
					Params =
					{
						{
							Type = "cItems",
							Name = "a_Pickups",
						},
						{
							Type = "number",
							Name = "a_BlockX",
						},
						{
							Type = "number",
							Name = "a_BlockY",
						},
						{
							Type = "number",
							Name = "a_BlockZ",
						},
						{
							Type = "number",
							Name = "a_SpeedX",
						},
						{
							Type = "number",
							Name = "a_SpeedY",
						},
						{
							Type = "number",
							Name = "a_SpeedZ",
						},
						{
							Type = "boolean",
							Name = "a_IsPlayerCreated",
						},
					},
					Returns =
					{
					},
					Desc = "OBSOLETE, use the Vector3d-based overload instead.\
	Spawns item pickups for each item in the list. May compress pickups if too many entities. All pickups get the speed specified.",
				},
			},
			SpawnMob =
			{
				{
					Params =
					{
						{
							Type = "number",
							Name = "a_PosX",
						},
						{
							Type = "number",
							Name = "a_PosY",
						},
						{
							Type = "number",
							Name = "a_PosZ",
						},
						{
							Type = "eMonsterType",
							Name = "a_MonsterType",
						},
						{
							Type = "boolean",
							Name = "a_Baby",
						},
					},
					Returns =
					{
						{
							Type = "number",
						},
					},
					Desc = "Spawns a mob of the specified type. Returns the mob's UniqueID if recognized and spawned, cEntity::INVALID_ID otherwise",
				},
			},
			SpawnPrimedTNT =
			{
				{
					Params =
					{
						{
							Type = "number",
							Name = "a_X",
						},
						{
							Type = "number",
							Name = "a_Y",
						},
						{
							Type = "number",
							Name = "a_Z",
						},
						{
							Type = "number",
							Name = "a_FuseTimeInSec",
						},
						{
							Type = "number",
							Name = "a_InitialVelocityCoeff",
						},
						{
							Type = "boolean",
							Name = "a_ShouldPlayFuseSound",
						},
					},
					Returns =
					{
						{
							Type = "number",
						},
					},
					Desc = "OBSOLETE, use the Vector3d-based overload instead.\
	Spawns experience orbs of the specified total value at the given location. The orbs' values are split according to regular Minecraft rules.\
	Returns an vector of UniqueID of all the orbs.",
				},
				{
					Params =
					{
						{
							Type = "Vector3d",
							Name = "a_Pos",
						},
						{
							Type = "number",
							Name = "a_FuseTimeInSec",
						},
						{
							Type = "number",
							Name = "a_InitialVelocityCoeff",
						},
						{
							Type = "boolean",
							Name = "a_ShouldPlayFuseSound",
						},
					},
					Returns =
					{
						{
							Type = "number",
						},
					},
					Desc = "Spawns a new primed TNT entity at the specified block coords and specified fuse duration.\
	Initial velocity is given based on the relative coefficient provided.\
	Returns the UniqueID of the created entity, or cEntity::INVALID_ID on failure.",
				},
			},
			UseBlockEntity =
			{
				{
					Params =
					{
						{
							Type = "cPlayer",
							Name = "a_Player",
						},
						{
							Type = "number",
							Name = "a_BlockX",
						},
						{
							Type = "number",
							Name = "a_BlockY",
						},
						{
							Type = "number",
							Name = "a_BlockZ",
						},
					},
					Returns =
					{
					},
					Desc = "a_Player is using block entity at [x, y, z], handle that:",
				},
			},
			VillagersShouldHarvestCrops =
			{
				{
					Params =
					{
					},
					Returns =
					{
						{
							Type = "boolean",
						},
					},
				},
			},
			WakeUpSimulators =
			{
				{
					Params =
					{
						{
							Type = "Vector3d",
							Name = "a_Block",
						},
					},
					Returns =
					{
					},
					Desc = "Wakes up the simulators for the specified block",
				},
			},
			WakeUpSimulatorsInArea =
			{
				{
					Params =
					{
						{
							Type = "cCuboid",
							Name = "a_Area",
						},
					},
					Returns =
					{
					},
					Desc = "Wakes up the simulators for the specified area of blocks",
				},
			},
		},
	},
}
