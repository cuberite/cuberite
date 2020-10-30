-- Describes the cWorld class API

return
{
	cWorld =
	{
		Desc = [[
			cWorld is the game world. It is the hub of all the information managed by individual classes,
			providing convenient access to them. Cuberite supports multiple worlds in any combination of
			world types. You can have two overworlds, three nethers etc. To enumerate all world the server
			provides, use the {{cRoot}}:ForEachWorld() function.</p>
			<p>
			The world data is held in individual chunks. Each chunk consists of 16 (x) * 16 (z) * 256 (y)
			blocks, each block is specified by its block type (8-bit) and block metadata (4-bit).
			Additionally, each block has two light values calculated - skylight (how much daylight it receives)
			and blocklight (how much light from light-emissive blocks it receives), both 4-bit.</p>
			<p>
			Each world runs several separate threads used for various housekeeping purposes, the most important
			of those is the Tick thread. This thread updates the game logic 20 times per second, and it is
			the thread where all the gameplay actions are evaluated. Liquid physics, entity interactions,
			player movement etc., all are applied in this thread.</p>
			<p>
			Additional threads include the generation thread (generates new chunks as needed, storage thread
			(saves and loads chunk from the disk), lighting thread (updates block light values) and the
			chunksender thread (compresses chunks to send to the clients).</p>
			<p>
			The world provides access to all its {{cPlayer|players}}, {{cEntity|entities}} and {{cBlockEntity|block
			entities}}. Because of multithreading issues, individual objects cannot be retrieved for indefinite
			handling, but rather must be modified in callbacks, within which they are guaranteed to stay valid.</p>
			<p>
			Physics for individual blocks are handled by the simulators. These will fire in each tick for all
			blocks that have been scheduled for simulator update ("simulator wakeup"). The simulators include
			liquid physics, falling blocks, fire spreading and extinguishing and redstone.</p>
			<p>
			Game time is also handled by the world. It provides the time-of-day and the total world age.
		]],
		Functions =
		{
			AreCommandBlocksEnabled =
			{
				Returns =
				{
					{
						Type = "boolean",
					},
				},
				Notes = "Returns whether command blocks are enabled on the (entire) server",
			},
			BroadcastBlockAction =
			{
				Params =
				{
					{
						Name = "BlockPos",
						Type = "Vector3i",
					},
					{
						Name = "ActionByte1",
						Type = "number",
					},
					{
						Name = "ActionByte2",
						Type = "number",
					},
					{
						Name = "BlockType",
						Type = "number",
					},
					{
						Name = "ExcludeClient",
						Type = "cClientHandle",
						IsOptional = true,
					},
				},
				Notes = "Broadcasts the BlockAction packet to all clients who have the appropriate chunk loaded (except ExcludeClient). The contents of the packet are specified by the parameters for the call, the blocktype needn't match the actual block that is present in the world data at the specified location.",
			},
			BroadcastBlockAction =
			{
				Params =
				{
					{
						Name = "BlockX",
						Type = "number",
					},
					{
						Name = "BlockY",
						Type = "number",
					},
					{
						Name = "BlockZ",
						Type = "number",
					},
					{
						Name = "ActionByte1",
						Type = "number",
					},
					{
						Name = "ActionByte2",
						Type = "number",
					},
					{
						Name = "BlockType",
						Type = "number",
					},
					{
						Name = "ExcludeClient",
						Type = "cClientHandle",
						IsOptional = true,
					},
				},
				Notes = "Broadcasts the BlockAction packet to all clients who have the appropriate chunk loaded (except ExcludeClient). The contents of the packet are specified by the parameters for the call, the blocktype needn't match the actual block that is present in the world data at the specified location. (DEPRECATED)",
			},
			BroadcastChat =
			{
				Params =
				{
					{
						Name = "Message",
						Type = "string",
					},
					{
						Name = "ExcludeClient",
						Type = "cClientHandle",
						IsOptional = true,
					},
					{
						Name = "ChatPrefix",
						Type = "eMessageType",
						IsOptional = true,
					},
				},
				Notes = "Sends the Message to all players in this world, except the optional ExcludeClient. No formatting is done by the server.",
			},
			BroadcastChatDeath =
			{
				Params =
				{
					{
						Name = "Message",
						Type = "string",
					},
					{
						Name = "ExcludeClient",
						Type = "cClientHandle",
						IsOptional = true,
					},
				},
				Notes = "Prepends Gray [DEATH] / colours entire text (depending on ShouldUseChatPrefixes()) and broadcasts message. For when a player dies.",
			},
			BroadcastChatFailure =
			{
				Params =
				{
					{
						Name = "Message",
						Type = "string",
					},
					{
						Name = "ExcludeClient",
						Type = "cClientHandle",
						IsOptional = true,
					},
				},
				Notes = "Prepends Rose [INFO] / colours entire text (depending on ShouldUseChatPrefixes()) and broadcasts message. For a command that failed to run because of insufficient permissions, etc.",
			},
			BroadcastChatFatal =
			{
				Params =
				{
					{
						Name = "Message",
						Type = "string",
					},
					{
						Name = "ExcludeClient",
						Type = "cClientHandle",
						IsOptional = true,
					},
				},
				Notes = "Prepends Red [FATAL] / colours entire text (depending on ShouldUseChatPrefixes()) and broadcasts message. For a plugin that crashed, or similar.",
			},
			BroadcastChatInfo =
			{
				Params =
				{
					{
						Name = "Message",
						Type = "string",
					},
					{
						Name = "ExcludeClient",
						Type = "cClientHandle",
						IsOptional = true,
					},
				},
				Notes = "Prepends Yellow [INFO] / colours entire text (depending on ShouldUseChatPrefixes()) and broadcasts message. For informational messages, such as command usage.",
			},
			BroadcastChatSuccess =
			{
				Params =
				{
					{
						Name = "Message",
						Type = "string",
					},
					{
						Name = "ExcludeClient",
						Type = "cClientHandle",
						IsOptional = true,
					},
				},
				Notes = "Prepends Green [INFO] / colours entire text (depending on ShouldUseChatPrefixes()) and broadcasts message. For success messages.",
			},
			BroadcastChatWarning =
			{
				Params =
				{
					{
						Name = "Message",
						Type = "string",
					},
					{
						Name = "ExcludeClient",
						Type = "cClientHandle",
						IsOptional = true,
					},
				},
				Notes = "Prepends Rose [WARN] / colours entire text (depending on ShouldUseChatPrefixes()) and broadcasts message. For concerning events, such as plugin reload etc.",
			},
			BroadcastEntityAnimation =
			{
				Params =
				{
					{
						Name = "TargetEntity",
						Type = "cEntity",
					},
					{
						Name = "Animation",
						Type = "number",
					},
					{
						Name = "ExcludeClient",
						Type = "cClientHandle",
						IsOptional = true,
					},
				},
				Notes = "Sends an animation of an entity to all clienthandles (except ExcludeClient if given)",
			},
			BroadcastParticleEffect =
			{
				Params =
				{
					{
						Name = "ParticleName",
						Type = "string",
					},
					{
						Name = "X",
						Type = "number",
					},
					{
						Name = "Y",
						Type = "number",
					},
					{
						Name = "Z",
						Type = "number",
					},
					{
						Name = "OffsetX",
						Type = "number",
					},
					{
						Name = "OffsetY",
						Type = "number",
					},
					{
						Name = "OffsetZ",
						Type = "number",
					},
					{
						Name = "ParticleData",
						Type = "number",
					},
					{
						Name = "ParticleAmount",
						Type = "number",
					},
					{
						Name = "ExcludeClient",
						Type = "cClientHandle",
						IsOptional = true,
					},
				},
				Notes = "Spawns the specified particles to all players in the world exept the optional ExeptClient. A list of available particles by thinkofdeath can be found {{https://gist.github.com/thinkofdeath/5110835|Here}}. <b>OBSOLETE</b>, use the vector-based overload instead",
			},
			BroadcastParticleEffect =
			{
				Params =
				{
					{
						Name = "ParticleName",
						Type = "string",
					},
					{
						Name = "SourcePos",
						Type = "Vector3f"
					},
					{
						Name = "Offset",
						Type = "Vector3f",
					},
					{
						Name = "ParticleData",
						Type = "number",
					},
					{
						Name = "ParticleAmount",
						Type = "number",
					},
					{
						Name = "ExcludeClient",
						Type = "cClientHandle",
						IsOptional = true,
					},
				},
				Notes = "Spawns the specified particles to all players in the world exept the optional ExeptClient. A list of available particles by thinkofdeath can be found {{https://gist.github.com/thinkofdeath/5110835|Here}}",
			},
			BroadcastSoundEffect =
			{
				Params =
				{
					{
						Name = "SoundName",
						Type = "string",
					},
					{
						Name = "Position",
						Type = "Vector3d",
					},
					{
						Name = "Volume",
						Type = "number",
					},
					{
						Name = "Pitch",
						Type = "number",
					},
					{
						Name = "ExcludeClient",
						Type = "cClientHandle",
						IsOptional = true,
					},
				},
				Notes = "Sends the specified sound effect to all players in this world, except the optional ExceptClient",
			},
			BroadcastSoundEffect =
			{
				Params =
				{
					{
						Name = "SoundName",
						Type = "string",
					},
					{
						Name = "X",
						Type = "number",
					},
					{
						Name = "Y",
						Type = "number",
					},
					{
						Name = "Z",
						Type = "number",
					},
					{
						Name = "Volume",
						Type = "number",
					},
					{
						Name = "Pitch",
						Type = "number",
					},
					{
						Name = "ExcludeClient",
						Type = "cClientHandle",
						IsOptional = true,
					},
				},
				Notes = "Sends the specified sound effect to all players in this world, except the optional ExceptClient (DEPRECATED, use vector-parametered version instead)",
			},
			BroadcastSoundParticleEffect =
			{
				Params =
				{
					{
						Name = "EffectID",
						Type = "number",
					},
					{
						Name = "X",
						Type = "number",
					},
					{
						Name = "Y",
						Type = "number",
					},
					{
						Name = "Z",
						Type = "number",
					},
					{
						Name = "EffectData",
						Type = "string",
					},
					{
						Name = "ExcludeClient",
						Type = "cClientHandle",
						IsOptional = true,
					},
				},
				Notes = "Sends the specified effect to all players in this world, except the optional ExceptClient. <b>OBSOLETE</b>, use the vector overload instead",
			},
			BroadcastSoundParticleEffect =
			{
				Params =
				{
					{
						Name = "EffectID",
						Type = "number",
					},
					{
						Name = "SourcePos",
						Type = "Vector3i"
					},
					{
						Name = "EffectData",
						Type = "string",
					},
					{
						Name = "ExcludeClient",
						Type = "cClientHandle",
						IsOptional = true,
					},
				},
				Notes = "Sends the specified effect to all players in this world, except the optional ExceptClient",
			},
			CastThunderbolt =
			{
				Params =
				{
					{
						Name = "Position",
						Type = "Vector3d",
					},
				},
				Notes = "Creates a thunderbolt at the specified coords",
			},
			CastThunderbolt =
			{
				Params =
				{
					{
						Name = "X",
						Type = "number",
					},
					{
						Name = "Y",
						Type = "number",
					},
					{
						Name = "Z",
						Type = "number",
					},
				},
				Notes = "Creates a thunderbolt at the specified coords (DEPRECATED, use vector-parametered version instead)",
			},
			ChangeWeather =
			{
				Notes = "Forces the weather to change in the next game tick. Weather is changed according to the normal rules: wSunny <-> wRain <-> wStorm",
			},
			ChunkStay =
			{
				Params =
				{
					{
						Name = "ChunkCoordTable",
						Type = "table",
					},
					{
						Name = "OnChunkAvailable",
						Type = "function",
						IsOptional = true,
					},
					{
						Name = "OnAllChunksAvailable",
						Type = "function",
						IsOptional = true,
					},
				},
				Notes = "Queues the specified chunks to be loaded or generated and calls the specified callbacks once they are loaded. ChunkCoordTable is an arra-table of chunk coords, each coord being a table of 2 numbers: { {Chunk1x, Chunk1z}, {Chunk2x, Chunk2z}, ...}. When any of those chunks are made available (including being available at the start of this call), the OnChunkAvailable() callback is called. When all the chunks are available, the OnAllChunksAvailable() callback is called. The function signatures are: <pre class=\"prettyprint lang-lua\">function OnChunkAvailable(ChunkX, ChunkZ)\
function OnAllChunksAvailable()</pre> All return values from the callbacks are ignored.",
			},
			CreateProjectile =
			{
				Params =
				{
					{
						Name = "X",
						Type = "number",
					},
					{
						Name = "Y",
						Type = "number",
					},
					{
						Name = "Z",
						Type = "number",
					},
					{
						Name = "ProjectileKind",
						Type = "cProjectileEntity#eKind",
					},
					{
						Name = "Creator",
						Type = "cEntity",
					},
					{
						Name = "Originating Item",
						Type = "cItem",
					},
					{
						Name = "Speed",
						Type = "Vector3d",
						IsOptional = true,
					},
				},
				Notes = "Creates a new projectile of the specified kind at the specified coords. The projectile's creator is set to Creator (may be nil). The item that created the projectile entity, commonly the {{cPlayer|player}}'s currently equipped item, is used at present for fireworks to correctly set their entity metadata. It is not used for any other projectile. Optional speed indicates the initial speed for the projectile.",
			},
			DigBlock =
			{
				{
					Params =
					{
						{
							Name = "X",
							Type = "number",
						},
						{
							Name = "Y",
							Type = "number",
						},
						{
							Name = "Z",
							Type = "number",
						},
						{
							Name = "Digger",
							Type = "cEntity",
							IsOptional = true,
						},
					},
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Replaces the specified block with air, without dropping the usual pickups for the block. Wakes up the simulators for the block and its neighbors. The optional Digger parameter specifies the entity who dug the block, usually a player. Returns true on success, or false if the chunk is not loaded or invalid coords. See also DropBlockAsPickups() for the version that drops pickups.",
				},
				{
					Params =
					{
						{
							Name = "BlockPos",
							Type = "Vector3i",
						},
						{
							Name = "Digger",
							Type = "cEntity",
							IsOptional = true,
						},
					},
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Replaces the specified block with air, without dropping the usual pickups for the block. Wakes up the simulators for the block and its neighbors. The optional Digger parameter specifies the entity who dug the block, usually a player. Returns true on success, or false if the chunk is not loaded or invalid coords. See also DropBlockAsPickups() for the version that drops pickups.",
				},
			},
			DoExplosionAt =
			{
				Params =
				{
					{
						Name = "Force",
						Type = "number",
					},
					{
						Name = "X",
						Type = "number",
					},
					{
						Name = "Y",
						Type = "number",
					},
					{
						Name = "Z",
						Type = "number",
					},
					{
						Name = "CanCauseFire",
						Type = "boolean",
					},
					{
						Name = "Source",
						Type = "eExplosionSource",
					},
					{
						Name = "SourceData",
						Type = "any",
					},
				},
				Notes = "Creates an explosion of the specified relative force in the specified position. If CanCauseFire is set, the explosion will set blocks on fire, too. The Source parameter specifies the source of the explosion, one of the esXXX constants. The SourceData parameter is specific to each source type, usually it provides more info about the source.",
			},
			DoWithBeaconAt =
			{
				Params =
				{
					{
						Name = "BlockX",
						Type = "number",
					},
					{
						Name = "BlockY",
						Type = "number",
					},
					{
						Name = "BlockZ",
						Type = "number",
					},
					{
						Name = "CallbackFunction",
						Type = "function",
					},
				},
				Returns =
				{
					{
						Type = "boolean",
					},
				},
				Notes = "If there is a beacon at the specified coords, calls the CallbackFunction with the {{cBeaconEntity}} parameter representing the beacon. The CallbackFunction has the following signature: <pre class=\"prettyprint lang-lua\">function Callback({{cBeaconEntity|BeaconEntity}})</pre> The function returns false if there is no beacon, or if there is, it returns the bool value that the callback has returned.",
			},
			DoWithBedAt =
			{
				Params =
				{
					{
						Name = "BlockX",
						Type = "number",
					},
					{
						Name = "BlockY",
						Type = "number",
					},
					{
						Name = "BlockZ",
						Type = "number",
					},
					{
						Name = "CallbackFunction",
						Type = "function",
					},
				},
				Returns =
				{
					{
						Type = "boolean",
					},
				},
				Notes = "If there is a bed at the specified coords, calls the CallbackFunction with the {{cBedEntity}} parameter representing the bed. The CallbackFunction has the following signature: <pre class=\"prettyprint lang-lua\">function Callback({{cBedEntity|cBedEntity}})</pre> The function returns false if there is no bed, or if there is, it returns the bool value that the callback has returned.",
			},
			DoWithBlockEntityAt =
			{
				Params =
				{
					{
						Name = "BlockX",
						Type = "number",
					},
					{
						Name = "BlockY",
						Type = "number",
					},
					{
						Name = "BlockZ",
						Type = "number",
					},
					{
						Name = "CallbackFunction",
						Type = "function",
					},
				},
				Returns =
				{
					{
						Type = "boolean",
					},
				},
				Notes = "If there is a block entity at the specified coords, calls the CallbackFunction with the {{cBlockEntity}} parameter representing the block entity. The CallbackFunction has the following signature: <pre class=\"prettyprint lang-lua\">function Callback({{cBlockEntity|BlockEntity}})</pre> The function returns false if there is no block entity, or if there is, it returns the bool value that the callback has returned.",
			},
			DoWithBrewingstandAt =
			{
				Params =
				{
					{
						Name = "BlockX",
						Type = "number",
					},
					{
						Name = "BlockY",
						Type = "number",
					},
					{
						Name = "BlockZ",
						Type = "number",
					},
					{
						Name = "CallbackFunction",
						Type = "function",
					},
				},
				Returns =
				{
					{
						Type = "boolean",
					},
				},
				Notes = "If there is a brewingstand at the specified coords, calls the CallbackFunction with the {{cBrewingstandEntity}} parameter representing the brewingstand. The CallbackFunction has the following signature: <pre class=\"prettyprint lang-lua\">function Callback({{cBrewingstandEntity|cBrewingstandEntity}})</pre> The function returns false if there is no brewingstand, or if there is, it returns the bool value that the callback has returned.",
			},
			DoWithChestAt =
			{
				Params =
				{
					{
						Name = "BlockX",
						Type = "number",
					},
					{
						Name = "BlockY",
						Type = "number",
					},
					{
						Name = "BlockZ",
						Type = "number",
					},
					{
						Name = "CallbackFunction",
						Type = "function",
					},
				},
				Returns =
				{
					{
						Type = "boolean",
					},
				},
				Notes = "If there is a chest at the specified coords, calls the CallbackFunction with the {{cChestEntity}} parameter representing the chest. The CallbackFunction has the following signature: <pre class=\"prettyprint lang-lua\">function Callback({{cChestEntity|ChestEntity}})</pre> The function returns false if there is no chest, or if there is, it returns the bool value that the callback has returned.",
			},
			DoWithCommandBlockAt =
			{
				Params =
				{
					{
						Name = "BlockX",
						Type = "number",
					},
					{
						Name = "BlockY",
						Type = "number",
					},
					{
						Name = "BlockZ",
						Type = "number",
					},
					{
						Name = "CallbackFunction",
						Type = "function",
					},
				},
				Returns =
				{
					{
						Type = "boolean",
					},
				},
				Notes = "If there is a command block at the specified coords, calls the CallbackFunction with the {{cCommandBlockEntity}} parameter representing the command block. The CallbackFunction has the following signature: <pre class=\"prettyprint lang-lua\">function Callback({{cCommandBlockEntity|CommandBlockEntity}})</pre> The function returns false if there is no command block, or if there is, it returns the bool value that the callback has returned.",
			},
			DoWithDispenserAt =
			{
				Params =
				{
					{
						Name = "BlockX",
						Type = "number",
					},
					{
						Name = "BlockY",
						Type = "number",
					},
					{
						Name = "BlockZ",
						Type = "number",
					},
					{
						Name = "CallbackFunction",
						Type = "function",
					},
				},
				Returns =
				{
					{
						Type = "boolean",
					},
				},
				Notes = "If there is a dispenser at the specified coords, calls the CallbackFunction with the {{cDispenserEntity}} parameter representing the dispenser. The CallbackFunction has the following signature: <pre class=\"prettyprint lang-lua\">function Callback({{cDispenserEntity|DispenserEntity}})</pre> The function returns false if there is no dispenser, or if there is, it returns the bool value that the callback has returned.",
			},
			DoWithDropperAt =
			{
				Params =
				{
					{
						Name = "BlockX",
						Type = "number",
					},
					{
						Name = "BlockY",
						Type = "number",
					},
					{
						Name = "BlockZ",
						Type = "number",
					},
					{
						Name = "CallbackFunction",
						Type = "function",
					},
				},
				Returns =
				{
					{
						Type = "boolean",
					},
				},
				Notes = "If there is a dropper at the specified coords, calls the CallbackFunction with the {{cDropperEntity}} parameter representing the dropper. The CallbackFunction has the following signature: <pre class=\"prettyprint lang-lua\">function Callback({{cDropperEntity|DropperEntity}})</pre> The function returns false if there is no dropper, or if there is, it returns the bool value that the callback has returned.",
			},
			DoWithDropSpenserAt =
			{
				Params =
				{
					{
						Name = "BlockX",
						Type = "number",
					},
					{
						Name = "BlockY",
						Type = "number",
					},
					{
						Name = "BlockZ",
						Type = "number",
					},
					{
						Name = "CallbackFunction",
						Type = "function",
					},
				},
				Returns =
				{
					{
						Type = "boolean",
					},
				},
				Notes = "If there is a dropper or a dispenser at the specified coords, calls the CallbackFunction with the {{cDropSpenserEntity}} parameter representing the dropper or dispenser. The CallbackFunction has the following signature: <pre class=\"prettyprint lang-lua\">function Callback({{cDropSpenserEntity|DropSpenserEntity}})</pre> Note that this can be used to access both dispensers and droppers in a similar way. The function returns false if there is neither dispenser nor dropper, or if there is, it returns the bool value that the callback has returned.",
			},
			DoWithEntityByID =
			{
				Params =
				{
					{
						Name = "EntityID",
						Type = "number",
					},
					{
						Name = "CallbackFunction",
						Type = "function",
					},
				},
				Returns =
				{
					{
						Type = "boolean",
					},
				},
				Notes = "If an entity with the specified ID exists, calls the callback with the {{cEntity}} parameter representing the entity. The CallbackFunction has the following signature: <pre class=\"prettyprint lang-lua\">function Callback({{cEntity|Entity}})</pre> The function returns false if the entity was not found, and it returns the same bool value that the callback has returned if the entity was found.",
			},
			DoWithFlowerPotAt =
			{
				Params =
				{
					{
						Name = "BlockX",
						Type = "number",
					},
					{
						Name = "BlockY",
						Type = "number",
					},
					{
						Name = "BlockZ",
						Type = "number",
					},
					{
						Name = "CallbackFunction",
						Type = "function",
					},
				},
				Returns =
				{
					{
						Type = "boolean",
					},
				},
				Notes = "If there is a flower pot at the specified coords, calls the CallbackFunction with the {{cFlowerPotEntity}} parameter representing the flower pot. The CallbackFunction has the following signature: <pre class=\"prettyprint lang-lua\">function Callback({{cFlowerPotEntity|FlowerPotEntity}})</pre> The function returns false if there is no flower pot, or if there is, it returns the bool value that the callback has returned.",
			},
			DoWithFurnaceAt =
			{
				Params =
				{
					{
						Name = "BlockX",
						Type = "number",
					},
					{
						Name = "BlockY",
						Type = "number",
					},
					{
						Name = "BlockZ",
						Type = "number",
					},
					{
						Name = "CallbackFunction",
						Type = "function",
					},
				},
				Returns =
				{
					{
						Type = "boolean",
					},
				},
				Notes = "If there is a furnace at the specified coords, calls the CallbackFunction with the {{cFurnaceEntity}} parameter representing the furnace. The CallbackFunction has the following signature: <pre class=\"prettyprint lang-lua\">function Callback({{cFurnaceEntity|FurnaceEntity}})</pre> The function returns false if there is no furnace, or if there is, it returns the bool value that the callback has returned.",
			},
			DoWithHopperAt =
			{
				Params =
				{
					{
						Name = "BlockX",
						Type = "number",
					},
					{
						Name = "BlockY",
						Type = "number",
					},
					{
						Name = "BlockZ",
						Type = "number",
					},
					{
						Name = "CallbackFunction",
						Type = "function",
					},
				},
				Returns =
				{
					{
						Type = "boolean",
					},
				},
				Notes = "If there is a hopper at the specified coords, calls the CallbackFunction with the {{cHopperEntity}} parameter representing the hopper. The CallbackFunction has the following signature: <pre class=\"prettyprint lang-lua\">function Callback({{cHopperEntity|cHopperEntity}})</pre> The function returns false if there is no hopper, or if there is, it returns the bool value that the callback has returned.",
			},
			DoWithMobHeadAt =
			{
				Params =
				{
					{
						Name = "BlockX",
						Type = "number",
					},
					{
						Name = "BlockY",
						Type = "number",
					},
					{
						Name = "BlockZ",
						Type = "number",
					},
					{
						Name = "CallbackFunction",
						Type = "function",
					},
				},
				Returns =
				{
					{
						Type = "boolean",
					},
				},
				Notes = "If there is a mob head at the specified coords, calls the CallbackFunction with the {{cMobHeadEntity}} parameter representing the furnace. The CallbackFunction has the following signature: <pre class=\"prettyprint lang-lua\">function Callback({{cMobHeadEntity|MobHeadEntity}})</pre> The function returns false if there is no mob head, or if there is, it returns the bool value that the callback has returned.",
			},
			DoWithNearestPlayer =
			{
				Params =
				{
					{
						Name = "Position",
						Type = "Vector3d",
					},
					{
						Name = "RangeLimit",
						Type = "number",
					},
					{
						Name = "CallbackFunction",
						Type = "function",
					},
					{
						Name = "CheckLineOfSight",
						Type = "boolean",
					},
					{
						Name = "IgnoreSpectator",
						Type = "boolean",
					},
				},
				Returns =
				{
					{
						Type = "boolean",
					},
				},
				Notes = "Calls the specified callback function with the {{cPlayer|player}} nearest to the specified position as its parameter, if they are still within the range limit. The CallbackFunction has the following signature: <pre class=\"prettyprint lang-lua\">function Callback({{cPlayer|Player}})</pre> The function returns false if the player was not found, or whatever bool value the callback returned if the player was found.",
			},
			DoWithNoteBlockAt =
			{
				Params =
				{
					{
						Name = "BlockX",
						Type = "number",
					},
					{
						Name = "BlockY",
						Type = "number",
					},
					{
						Name = "BlockZ",
						Type = "number",
					},
					{
						Name = "CallbackFunction",
						Type = "function",
					},
				},
				Returns =
				{
					{
						Type = "boolean",
					},
				},
				Notes = "If there is a note block at the specified coords, calls the CallbackFunction with the {{cNoteEntity}} parameter representing the note block. The CallbackFunction has the following signature: <pre class=\"prettyprint lang-lua\">function Callback({{cNoteEntity|NoteEntity}})</pre> The function returns false if there is no note block, or if there is, it returns the bool value that the callback has returned.",
			},
			DoWithPlayer =
			{
				Params =
				{
					{
						Name = "PlayerName",
						Type = "string",
					},
					{
						Name = "CallbackFunction",
						Type = "function",
					},
				},
				Returns =
				{
					{
						Type = "boolean",
					},
				},
				Notes = "If there is a player of the specified name (exact match), calls the CallbackFunction with the {{cPlayer}} parameter representing the player. The CallbackFunction has the following signature: <pre class=\"prettyprint lang-lua\">function Callback({{cPlayer|Player}})</pre> The function returns false if the player was not found, or whatever bool value the callback returned if the player was found.",
			},
			DoWithPlayerByUUID =
			{
				Params =
				{
					{
						Name = "PlayerUUID",
						Type = "cUUID",
					},
					{
						Name = "CallbackFunction",
						Type = "function",
					},
				},
				Returns =
				{
					{
						Type = "boolean",
					},
				},
				Notes = "If there is the player with the uuid, calls the CallbackFunction with the {{cPlayer}} parameter representing the player. The CallbackFunction has the following signature: <pre class=\"prettyprint lang-lua\">function Callback({{cPlayer|Player}})</pre> The function returns false if the player was not found, or whatever bool value the callback returned if the player was found.",
			},
			DropBlockAsPickups =
			{
				Params =
				{
					{
						Name = "BlockPos",
						Type = "Vector3i",
					},
					{
						Name = "Digger",
						Type = "cEntity",
						IsOptional = true,
					},
					{
						Name = "Tool",
						Type = "cItem",
						IsOptional = true,
					},
				},
				Returns =
				{
					{
						Name = "IsSuccess",
						Type = "boolean",
					}
				},
				Notes = "Digs up the specified block and spawns the appropriate pickups for it. The optional Digger parameter specifies the {{cEntity|entity}} who dug the block, usually a {{cPlayer|player}}. The optional Tool parameter specifies the tool used to dig the block, not present means an empty hand. Returns true on success, false if the chunk is not present. See also DigBlock() for the pickup-less version.",
			},
			FastSetBlock =
			{
				{
					Params =
					{
						{
							Name = "BlockX",
							Type = "number",
						},
						{
							Name = "BlockY",
							Type = "number",
						},
						{
							Name = "BlockZ",
							Type = "number",
						},
						{
							Name = "BlockType",
							Type = "number",
						},
						{
							Name = "BlockMeta",
							Type = "number",
						},
					},
					Notes = "Sets the block at the specified coords, without waking up the simulators or replacing the block entities for the previous block type. Do not use if the block being replaced has a block entity tied to it!",
				},
				{
					Params =
					{
						{
							Name = "BlockCoords",
							Type = "Vector3i",
						},
						{
							Name = "BlockType",
							Type = "number",
						},
						{
							Name = "BlockMeta",
							Type = "number",
						},
					},
					Notes = "Sets the block at the specified coords, without waking up the simulators or replacing the block entities for the previous block type. Do not use if the block being replaced has a block entity tied to it!",
				},
			},
			FindAndDoWithPlayer =
			{
				Params =
				{
					{
						Name = "PlayerName",
						Type = "string",
					},
					{
						Name = "CallbackFunction",
						Type = "function",
					},
				},
				Returns =
				{
					{
						Type = "boolean",
					},
				},
				Notes = "Calls the given callback function for the player with the name best matching the name string provided.<br>This function is case-insensitive and will match partial names.<br>Returns false if player not found or there is ambiguity, true otherwise. The CallbackFunction has the following signature: <pre class=\"prettyprint lang-lua\">function Callback({{cPlayer|Player}})</pre>",
			},
			ForEachBlockEntityInChunk =
			{
				Params =
				{
					{
						Name = "ChunkX",
						Type = "number",
					},
					{
						Name = "ChunkZ",
						Type = "number",
					},
					{
						Name = "CallbackFunction",
						Type = "function",
					},
				},
				Returns =
				{
					{
						Type = "boolean",
					},
				},
				Notes = "Calls the specified callback for each block entity in the chunk. Returns true if all block entities in the chunk have been processed (including when there are zero block entities), or false if the callback has aborted the enumeration by returning true. The CallbackFunction has the following signature: <pre class=\"prettyprint lang-lua\">function Callback({{cBlockEntity|BlockEntity}})</pre> The callback should return false or no value to continue with the next block entity, or true to abort the enumeration.",
			},
			ForEachBrewingstandInChunk =
			{
				Params =
				{
					{
						Name = "ChunkX",
						Type = "number",
					},
					{
						Name = "ChunkZ",
						Type = "number",
					},
					{
						Name = "CallbackFunction",
						Type = "function",
					},
				},
				Returns =
				{
					{
						Type = "boolean",
					},
				},
				Notes = "Calls the specified callback for each brewingstand in the chunk. Returns true if all brewingstands in the chunk have been processed (including when there are zero brewingstands), or false if the callback has aborted the enumeration by returning true. The CallbackFunction has the following signature: <pre class=\"prettyprint lang-lua\">function Callback({{cBrewingstandEntity|cBrewingstandEntity}})</pre> The callback should return false or no value to continue with the next brewingstand, or true to abort the enumeration.",
			},
			ForEachChestInChunk =
			{
				Params =
				{
					{
						Name = "ChunkX",
						Type = "number",
					},
					{
						Name = "ChunkZ",
						Type = "number",
					},
					{
						Name = "CallbackFunction",
						Type = "function",
					},
				},
				Returns =
				{
					{
						Type = "boolean",
					},
				},
				Notes = "Calls the specified callback for each chest in the chunk. Returns true if all chests in the chunk have been processed (including when there are zero chests), or false if the callback has aborted the enumeration by returning true. The CallbackFunction has the following signature: <pre class=\"prettyprint lang-lua\">function Callback({{cChestEntity|ChestEntity}})</pre> The callback should return false or no value to continue with the next chest, or true to abort the enumeration.",
			},
			ForEachEntity =
			{
				Params =
				{
					{
						Name = "CallbackFunction",
						Type = "function",
					},
				},
				Returns =
				{
					{
						Type = "boolean",
					},
				},
				Notes = "Calls the specified callback for each entity in the loaded world. Returns true if all the entities have been processed (including when there are zero entities), or false if the callback function has aborted the enumeration by returning true. The callback function has the following signature: <pre class=\"prettyprint lang-lua\">function Callback({{cEntity|Entity}})</pre> The callback should return false or no value to continue with the next entity, or true to abort the enumeration.",
			},
			ForEachEntityInBox =
			{
				Params =
				{
					{
						Name = "Box",
						Type = "cBoundingBox",
					},
					{
						Name = "CallbackFunction",
						Type = "function",
					},
				},
				Returns =
				{
					{
						Type = "boolean",
					},
				},
				Notes = "Calls the specified callback for each entity in the specified bounding box. Returns true if all the entities have been processed (including when there are zero entities), or false if the callback function has aborted the enumeration by returning true. If any chunk within the bounding box is not valid, it is silently skipped without any notification. The callback function has the following signature: <pre class=\"prettyprint lang-lua\">function Callback({{cEntity|Entity}})</pre> The callback should return false or no value to continue with the next entity, or true to abort the enumeration.",
			},
			ForEachEntityInChunk =
			{
				Params =
				{
					{
						Name = "ChunkX",
						Type = "number",
					},
					{
						Name = "ChunkZ",
						Type = "number",
					},
					{
						Name = "CallbackFunction",
						Type = "function",
					},
				},
				Returns =
				{
					{
						Type = "boolean",
					},
				},
				Notes = "Calls the specified callback for each entity in the specified chunk. Returns true if all the entities have been processed (including when there are zero entities), or false if the chunk is not loaded or the callback function has aborted the enumeration by returning true. The callback function has the following signature: <pre class=\"prettyprint lang-lua\">function Callback({{cEntity|Entity}})</pre> The callback should return false or no value to continue with the next entity, or true to abort the enumeration.",
			},
			ForEachFurnaceInChunk =
			{
				Params =
				{
					{
						Name = "ChunkX",
						Type = "number",
					},
					{
						Name = "ChunkZ",
						Type = "number",
					},
					{
						Name = "CallbackFunction",
						Type = "function",
					},
				},
				Returns =
				{
					{
						Type = "boolean",
					},
				},
				Notes = "Calls the specified callback for each furnace in the chunk. Returns true if all furnaces in the chunk have been processed (including when there are zero furnaces), or false if the callback has aborted the enumeration by returning true. The CallbackFunction has the following signature: <pre class=\"prettyprint lang-lua\">function Callback({{cFurnaceEntity|FurnaceEntity}})</pre> The callback should return false or no value to continue with the next furnace, or true to abort the enumeration.",
			},
			ForEachLoadedChunk =
			{
				Params =
				{
					{
						Name = "CallbackFunction",
						Type = "function",
					},
				},
				Returns =
				{
					{
						Type = "boolean",
					},
				},
				Notes = "Calls the specified callback for each loaded chunk in the world. Returns true if all chunks have been processed, or false if the callback has aborted the enumeration by returning true. The CallbackFunction has the following signature: <pre class=\"prettyprint lang-lua\">function Callback(ChunkX, ChunkZ)</pre> The callback should return false or no value to continue with the next chunk, or true to abort the enumeration.",
			},
			ForEachPlayer =
			{
				Params =
				{
					{
						Name = "CallbackFunction",
						Type = "function",
					},
				},
				Returns =
				{
					{
						Type = "boolean",
					},
				},
				Notes = "Calls the specified callback for each player in the loaded world. Returns true if all the players have been processed (including when there are zero players), or false if the callback function has aborted the enumeration by returning true. The callback function has the following signature: <pre class=\"prettyprint lang-lua\">function Callback({{cPlayer|Player}})</pre> The callback should return false or no value to continue with the next player, or true to abort the enumeration.",
			},
			GenerateChunk =
			{
				Params =
				{
					{
						Name = "ChunkX",
						Type = "number",
					},
					{
						Name = "ChunkZ",
						Type = "number",
					},
				},
				Notes = "Queues the specified chunk in the chunk generator. Ignored if the chunk is already generated (use RegenerateChunk() to force chunk re-generation).",
			},
			GetBiomeAt =
			{
				Params =
				{
					{
						Name = "BlockX",
						Type = "number",
					},
					{
						Name = "BlockZ",
						Type = "number",
					},
				},
				Returns =
				{
					{
						Name = "eBiome",
						Type = "EMCSBiome",
					},
				},
				Notes = "Returns the biome at the specified coords. Reads the biome from the chunk, if it is loaded, otherwise it uses the chunk generator to provide the biome value.",
			},
			GetBlock =
			{
				{
					Params =
					{
						{
							Name = "BlockX",
							Type = "number",
						},
						{
							Name = "BlockY",
							Type = "number",
						},
						{
							Name = "BlockZ",
							Type = "number",
						},
					},
					Returns =
					{
						{
							Name = "BLOCKTYPE",
							Type = "number",
						},
					},
					Notes = "Returns the block type of the block at the specified coords, or 0 if the appropriate chunk is not loaded.",
				},
				{
					Params =
					{
						{
							Name = "BlockCoords",
							Type = "Vector3i",
						},
					},
					Returns =
					{
						{
							Name = "BLOCKTYPE",
							Type = "number",
						},
					},
					Notes = "Returns the block type of the block at the specified coords, or 0 if the appropriate chunk is not loaded.",
				},
			},
			GetBlockBlockLight =
			{
				{
					Params =
					{
						{
							Name = "BlockX",
							Type = "number",
						},
						{
							Name = "BlockY",
							Type = "number",
						},
						{
							Name = "BlockZ",
							Type = "number",
						},
					},
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the amount of block light at the specified coords, or 0 if the appropriate chunk is not loaded.",
				},
				{
					Params =
					{
						{
							Name = "Pos",
							Type = "Vector3i",
						},
					},
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the amount of block light at the specified coords, or 0 if the appropriate chunk is not loaded.",
				},
			},
			GetBlockInfo =
			{
				Params =
				{
					{
						Name = "BlockX",
						Type = "number",
					},
					{
						Name = "BlockY",
						Type = "number",
					},
					{
						Name = "BlockZ",
						Type = "number",
					},
				},
				Returns =
				{
					{
						Name = "IsBlockValid",
						Type = "boolean",
					},
					{
						Name = "BlockType",
						Type = "number",
					},
					{
						Name = "BlockMeta",
						Type = "number",
					},
					{
						Name = "BlockSkyLight",
						Type = "number",
					},
					{
						Name = "BlockBlockLight",
						Type = "number",
					},
				},
				Notes = "Returns the complete block info for the block at the specified coords. The first value specifies if the block is in a valid loaded chunk, the other values are valid only if BlockValid is true.",
			},
			GetBlockMeta =
			{
				{
					Params =
					{
						{
							Name = "BlockX",
							Type = "number",
						},
						{
							Name = "BlockY",
							Type = "number",
						},
						{
							Name = "BlockZ",
							Type = "number",
						},
					},
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the block metadata of the block at the specified coords, or 0 if the appropriate chunk is not loaded.",
				},
				{
					Params =
					{
						{
							Name = "BlockCoords",
							Type = "Vector3i",
						},
					},
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the block metadata of the block at the specified coords, or 0 if the appropriate chunk is not loaded.",
				},
			},
			GetBlockSkyLight =
			{
				Params =
				{
					{
						Name = "BlockX",
						Type = "number",
					},
					{
						Name = "BlockY",
						Type = "number",
					},
					{
						Name = "BlockZ",
						Type = "number",
					},
				},
				Returns =
				{
					{
						Type = "number",
					},
				},
				Notes = "Returns the block skylight of the block at the specified coords, or 0 if the appropriate chunk is not loaded.",
			},
			GetBlockTypeMeta =
			{
				Params =
				{
					{
						Name = "BlockX",
						Type = "number",
					},
					{
						Name = "BlockY",
						Type = "number",
					},
					{
						Name = "BlockZ",
						Type = "number",
					},
				},
				Returns =
				{
					{
						Name = "IsBlockValid",
						Type = "boolean",
					},
					{
						Name = "BlockType",
						Type = "number",
					},
					{
						Name = "BlockMeta",
						Type = "number",
					},
				},
				Notes = "Returns the block type and metadata for the block at the specified coords. The first value specifies if the block is in a valid loaded chunk, the other values are valid only if BlockValid is true.",
			},
			GetDataPath =
			{
				Returns =
				{
					{
						Type = "string",
					},
				},
				Notes = "Returns the path to the root of the world data.",
			},
			GetDefaultWeatherInterval =
			{
				Params =
				{
					{
						Name = "Weather",
						Type = "eWeather",
					},
				},
				Returns =
				{
					{
						Type = "number",
					},
				},
				Notes = "Returns the default weather interval for the specific weather type. Returns -1 for any unknown weather.",
			},
			GetDimension =
			{
				Returns =
				{
					{
						Type = "eDimension",
					},
				},
				Notes = "Returns the dimension of the world - dimOverworld, dimNether or dimEnd.",
			},
			GetGameMode =
			{
				Returns =
				{
					{
						Type = "eGameMode",
					},
				},
				Notes = "Returns the gamemode of the world - gmSurvival, gmCreative or gmAdventure.",
			},
			GetGeneratorQueueLength =
			{
				Returns =
				{
					{
						Type = "number",
					},
				},
				Notes = "Returns the number of chunks that are queued in the chunk generator.",
			},
			GetHeight =
			{
				Params =
				{
					{
						Name = "BlockX",
						Type = "number",
					},
					{
						Name = "BlockZ",
						Type = "number",
					},
				},
				Returns =
				{
					{
						Type = "number",
					},
				},
				Notes = "Returns the maximum height of the particula block column in the world. If the chunk is not loaded, it waits for it to load / generate. <b>WARNING</b>: Do not use, Use TryGetHeight() instead for a non-waiting version, otherwise you run the risk of a deadlock!",
			},
			GetIniFileName =
			{
				Returns =
				{
					{
						Type = "string",
					},
				},
				Notes = "Returns the name of the world.ini file that the world uses to store the information.",
			},
			GetLightingQueueLength =
			{
				Returns =
				{
					{
						Type = "number",
					},
				},
				Notes = "Returns the number of chunks in the lighting thread's queue.",
			},
			GetLinkedEndWorldName =
			{
				Returns =
				{
					{
						Type = "string",
					},
				},
				Notes = "Returns the name of the end world this world is linked to.",
			},
			GetLinkedNetherWorldName =
			{
				Returns =
				{
					{
						Type = "string",
					},
				},
				Notes = "Returns the name of the Netherworld linked to this world.",
			},
			GetLinkedOverworldName =
			{
				Returns =
				{
					{
						Type = "string",
					},
				},
				Notes = "Returns the name of the world this world is linked to.",
			},
			GetMapManager =
			{
				Returns =
				{
					{
						Type = "cMapManager",
					},
				},
				Notes = "Returns the {{cMapManager|MapManager}} object used by this world.",
			},
			GetMaxCactusHeight =
			{
				Returns =
				{
					{
						Type = "number",
					},
				},
				Notes = "Returns the configured maximum height to which cacti will grow naturally.",
			},
			GetMaxNetherPortalHeight =
			{
				Returns =
				{
					{
						Type = "number",
					},
				},
				Notes = "Returns the maximum height for a nether portal",
			},
			GetMaxNetherPortalWidth =
			{
				Returns =
				{
					{
						Type = "number",
					},
				},
				Notes = "Returns the maximum width for a nether portal",
			},
			GetMaxSugarcaneHeight =
			{
				Returns =
				{
					{
						Type = "number",
					},
				},
				Notes = "Returns the configured maximum height to which sugarcane will grow naturally.",
			},
			GetMaxViewDistance =
			{
				Returns =
				{
					{
						Type = "number",
					},
				},
				Notes = "Returns the maximum viewdistance that players can see in this world. The view distance is the amount of chunks around the player that the player can see.",
			},
			GetMinNetherPortalHeight =
			{
				Returns =
				{
					{
						Type = "number",
					},
				},
				Notes = "Returns the minimum height for a nether portal",
			},
			GetMinNetherPortalWidth =
			{
				Returns =
				{
					{
						Type = "number",
					},
				},
				Notes = "Returns the minimum width for a nether portal",
			},
			GetName =
			{
				Returns =
				{
					{
						Type = "string",
					},
				},
				Notes = "Returns the name of the world, as specified in the settings.ini file.",
			},
			GetNumChunks =
			{
				Returns =
				{
					{
						Type = "number",
					},
				},
				Notes = "Returns the number of chunks currently loaded.",
			},
			GetNumUnusedDirtyChunks =
			{
				Returns =
				{
					{
						Type = "number",
					},
				},
				Notes = "Returns the number of unused dirty chunks. That's the number of chunks that we can save and then unload.",
			},
			GetScoreBoard =
			{
				Returns =
				{
					{
						Type = "cScoreboard",
					},
				},
				Notes = "Returns the {{cScoreboard|Scoreboard}} object used by this world. ",
			},
			GetSeed =
			{
				Returns =
				{
					{
						Type = "number",
					},
				},
				Notes = "Returns the seed of the world.",
			},
			GetSignLines =
			{
				Params =
				{
					{
						Name = "BlockX",
						Type = "number",
					},
					{
						Name = "BlockY",
						Type = "number",
					},
					{
						Name = "BlockZ",
						Type = "number",
					},
				},
				Returns =
				{
					{
						Name = "IsValid",
						Type = "boolean",
					},
					{
						Name = "Line1",
						Type = "string",
						IsOptional = true,
					},
					{
						Name = "Line2",
						Type = "string",
						IsOptional = true,
					},
					{
						Name = "Line3",
						Type = "string",
						IsOptional = true,
					},
					{
						Name = "Line4",
						Type = "string",
						IsOptional = true,
					},
				},
				Notes = "Returns true and the lines of a sign at the specified coords, or false if there is no sign at the coords.",
			},
			GetSpawnX =
			{
				Returns =
				{
					{
						Type = "number",
					},
				},
				Notes = "Returns the X coord of the default spawn",
			},
			GetSpawnY =
			{
				Returns =
				{
					{
						Type = "number",
					},
				},
				Notes = "Returns the Y coord of the default spawn",
			},
			GetSpawnZ =
			{
				Returns =
				{
					{
						Type = "number",
					},
				},
				Notes = "Returns the Z coord of the default spawn",
			},
			GetStorageLoadQueueLength =
			{
				Returns =
				{
					{
						Type = "number",
					},
				},
				Notes = "Returns the number of chunks queued up for loading",
			},
			GetStorageSaveQueueLength =
			{
				Returns =
				{
					{
						Type = "number",
					},
				},
				Notes = "Returns the number of chunks queued up for saving",
			},
			GetTicksUntilWeatherChange =
			{
				Returns =
				{
					{
						Type = "number",
					},
				},
				Notes = "Returns the number of ticks that will pass before the weather is changed",
			},
			GetTimeOfDay =
			{
				Returns =
				{
					{
						Type = "number",
					},
				},
				Notes = "Returns the number of ticks that have passed from the sunrise, 0 .. 24000.",
			},
			GetTNTShrapnelLevel =
			{
				Returns =
				{
					{
						Name = "ShrapnelLevel",
						Type = "eShrapnelLevel",
					},
				},
				Notes = "Returns the shrapnel level, representing the block types that are propelled outwards following an explosion. Based on this value and a random picker, blocks are selectively converted to physics entities (FallingSand) and flung outwards.",
			},
			GetWeather =
			{
				Returns =
				{
					{
						Type = "eWeather",
					},
				},
				Notes = "Returns the current weather in the world (wSunny, wRain, wStorm). To check for weather, use IsWeatherXXX() functions instead.",
			},
			GetWorldAge =
			{
				Returns =
				{
					{
						Type = "number",
					},
				},
				Notes = "Returns the total age of the world, in ticks. The age always grows, cannot be set by plugins and is unrelated to TimeOfDay.",
			},
			GrowCactus =
			{
				Params =
				{
					{
						Name = "BlockX",
						Type = "number",
					},
					{
						Name = "BlockY",
						Type = "number",
					},
					{
						Name = "BlockZ",
						Type = "number",
					},
					{
						Name = "NumBlocksToGrow",
						Type = "number",
					},
				},
				Returns =
				{
					{
						Type = "number",
					},
				},
				Notes = "OBSOLETE, use GrowPlantAt instead. Grows a cactus block at the specified coords, by up to the specified number of blocks. Adheres to the world's maximum cactus growth (GetMaxCactusHeight()). Returns the amount of blocks the cactus grew inside this call.",
			},
			GrowMelonPumpkin =
			{
				Params =
				{
					{
						Name = "BlockX",
						Type = "number",
					},
					{
						Name = "BlockY",
						Type = "number",
					},
					{
						Name = "BlockZ",
						Type = "number",
					},
					{
						Name = "StemBlockType",
						Type = "number",
					},
				},
				Returns =
				{
					{
						Type = "boolean",
					},
				},
				Notes = "OBSOLETE, use GrowPlantAt instead. Grows a melon or pumpkin, based on the stem block type specified (assumed to be at the coords provided). Checks for normal melon / pumpkin growth conditions - stem not having another produce next to it and suitable ground below. Returns true if the melon or pumpkin grew successfully.",
			},
			GrowPlantAt =
			{
				Params =
				{
					{
						Name = "BlockPos",
						Type = "Vector3i",
					},
					{
						Name = "NumStages",
						Type = "number",
					},
				},
				Returns =
				{
					{
						Type = "number",
					},
				},
				Notes = "Grows the plant at the specified block by the specified number of stages. Returns the number of stages actually grown. Returns zero for non-growable blocks.",
			},
			GrowRipePlant =
			{
				{
					Params =
					{
						{
							Name = "BlockX",
							Type = "number",
						},
						{
							Name = "BlockY",
							Type = "number",
						},
						{
							Name = "BlockZ",
							Type = "number",
						},
					},
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "OBSOLETE, use the Vector3-based overload instead. Grows the plant at the specified coords to its full. Returns true if the plant was grown, false if not.",
				},
				{
					Params =
					{
						{
							Name = "BlockPos",
							Type = "Vector3i",
						},
					},
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Grows the plant at the specified coords to its full. Returns true if the plant was grown, false if not.",
				},
			},
			GrowSugarcane =
			{
				Params =
				{
					{
						Name = "BlockX",
						Type = "number",
					},
					{
						Name = "BlockY",
						Type = "number",
					},
					{
						Name = "BlockZ",
						Type = "number",
					},
					{
						Name = "NumBlocksToGrow",
						Type = "number",
					},
				},
				Returns =
				{
					{
						Type = "number",
					},
				},
				Notes = "OBSOLETE, use GrowPlantAt instead. Grows a sugarcane block at the specified coords, by up to the specified number of blocks. Adheres to the world's maximum sugarcane growth (GetMaxSugarcaneHeight()). Returns the amount of blocks the sugarcane grew inside this call.",
			},
			GrowTree =
			{
				Params =
				{
					{
						Name = "BlockPos",
						Type = "Vector3i",
					},
				},
				Returns =
				{
					{
						Type = "boolean",
					},
				},
				Notes = "Grows a tree based at the specified coords. If there is a sapling there, grows the tree based on that sapling, otherwise chooses a tree image based on the biome. Returns true if the tree was grown, false if not (invalid chunk, insufficient space)",
			},
			GrowTreeByBiome =
			{
				Params =
				{
					{
						Name = "BlockPos",
						Type = "Vector3i",
					},
				},
				Returns =
				{
					{
						Type = "boolean",
					},
				},
				Notes = "Grows a tree based at the specified coords. The tree type is picked from types available for the biome at those coords. Returns true if the tree was grown, false if not (invalid chunk, insufficient space)",
			},
			GrowTreeFromSapling =
			{
				Params =
				{
					{
						Name = "BlockPos",
						Type = "Vector3i",
					},
				},
				Returns =
				{
					{
						Type = "boolean",
					},
				},
				Notes = "Grows a tree based at the specified coords. The tree type is determined from the sapling meta. If the sapling is part of a 2x2 sapling area, grows a large tree. Returns true if the tree was grown, false if not (invalid chunk, insufficient space)",
			},
			IsBlockDirectlyWatered =
			{
				Params =
				{
					{
						Name = "BlockX",
						Type = "number",
					},
					{
						Name = "BlockY",
						Type = "number",
					},
					{
						Name = "BlockZ",
						Type = "number",
					},
				},
				Returns =
				{
					{
						Type = "boolean",
					},
				},
				Notes = "Returns true if the specified block has a water block right next to it (on the X/Z axes)",
			},
			IsDaylightCycleEnabled =
			{
				Returns =
				{
					{
						Type = "boolean",
					},
				},
				Notes = "Returns true if the daylight cycle is enabled.",
			},
			IsDeepSnowEnabled =
			{
				Returns =
				{
					{
						Type = "boolean",
					},
				},
				Notes = "Returns whether the configuration has DeepSnow enabled.",
			},
			IsGameModeAdventure =
			{
				Returns =
				{
					{
						Type = "boolean",
					},
				},
				Notes = "Returns true if the current gamemode is gmAdventure.",
			},
			IsGameModeCreative =
			{
				Returns =
				{
					{
						Type = "boolean",
					},
				},
				Notes = "Returns true if the current gamemode is gmCreative.",
			},
			IsGameModeSpectator =
			{
				Returns =
				{
					{
						Type = "boolean",
					},
				},
				Notes = "Returns true if the current gamemode is gmSpectator.",
			},
			IsGameModeSurvival =
			{
				Returns =
				{
					{
						Type = "boolean",
					},
				},
				Notes = "Returns true if the current gamemode is gmSurvival.",
			},
			IsPVPEnabled =
			{
				Returns =
				{
					{
						Type = "boolean",
					},
				},
				Notes = "Returns whether PVP is enabled in the world settings.",
			},
			IsSavingEnabled =
			{
				Returns =
				{
					{
						Type = "boolean",
					},
				},
				Notes = "Returns whether or not saving chunk data is enabled. If disabled, the world will keep dirty chunks in memory forever, and will simply regenerate non-dirty chunks that are unloaded.",
			},
			IsTrapdoorOpen =
			{
				Params =
				{
					{
						Name = "BlockX",
						Type = "number",
					},
					{
						Name = "BlockY",
						Type = "number",
					},
					{
						Name = "BlockZ",
						Type = "number",
					},
				},
				Returns =
				{
					{
						Type = "boolean",
					},
				},
				Notes = "Returns false if there is no trapdoor there or if the block isn't a trapdoor or if the chunk wasn't loaded. Returns true if trapdoor is open.",
			},
			IsWeatherRain =
			{
				Returns =
				{
					{
						Type = "boolean",
					},
				},
				Notes = "Returns true if the current weather is rainy.",
			},
			IsWeatherRainAt =
			{
				Params =
				{
					{
						Name = "BlockX",
						Type = "number",
					},
					{
						Name = "BlockZ",
						Type = "number",
					},
				},
				Returns =
				{
					{
						Type = "boolean",
					},
				},
				Notes = "Returns true if it is rainy at the specified location. This takes into account biomes.",
			},
			IsWeatherStorm =
			{
				Returns =
				{
					{
						Type = "boolean",
					},
				},
				Notes = "Returns true if the current weather is stormy.",
			},
			IsWeatherStormAt =
			{
				Params =
				{
					{
						Name = "BlockX",
						Type = "number",
					},
					{
						Name = "BlockZ",
						Type = "number",
					},
				},
				Returns =
				{
					{
						Type = "boolean",
					},
				},
				Notes = "Returns true if it is stormy at the specified location. This takes into account biomes.",
			},
			IsWeatherSunny =
			{
				Returns =
				{
					{
						Type = "boolean",
					},
				},
				Notes = "Returns true if the current weather is sunny.",
			},
			IsWeatherSunnyAt =
			{
				Params =
				{
					{
						Name = "BlockX",
						Type = "number",
					},
					{
						Name = "BlockZ",
						Type = "number",
					},
				},
				Returns =
				{
					{
						Type = "boolean",
					},
				},
				Notes = "Returns true if it is sunny at the specified location. This takes into account biomes.",
			},
			IsWeatherWet =
			{
				Returns =
				{
					{
						Type = "boolean",
					},
				},
				Notes = "Returns true if the world currently has any precipitation - rain, storm or snow.",
			},
			IsWeatherWetAt =
			{
				Params =
				{
					{
						Name = "BlockX",
						Type = "number",
					},
					{
						Name = "BlockZ",
						Type = "number",
					},
				},
				Returns =
				{
					{
						Type = "boolean",
					},
				},
				Notes = "Returns true if it is raining or storming at the specified location. This takes into account biomes.",
			},
			IsWeatherWetAtXYZ =
			{
				Params =
				{
					{
						Name = "Pos",
						Type = "Vector3i",
					},
				},
				Returns =
				{
					{
						Type = "boolean",
					},
				},
				Notes = "Returns true if the specified location has wet weather (rain or storm), using the same logic as IsWeatherWetAt, except that any rain-blocking blocks above the specified position will block the precipitation and this function will return false.",
			},
			PickupsFromBlock =
			{
				Params =
				{
					{
						Name = "BlockPos",
						Type = "Vector3i",
					},
					{
						Name = "Digger",
						Type = "cEntity",
						IsOptional = true,
					},
					{
						Name = "Tool",
						Type = "cItem",
						IsOptional = true,
					},
				},
				Returns =
				{
					{
						Name = "Items",
						Type = "cItems",
					},
				},
				Notes = "Returns all the pickups that would result if the Digger dug up the block at BlockPos using Tool. Digger is usually a {{cPlayer}}, but can be nil for natural causes. Tool is usually the equipped {{cItem|item}}, can be nil for empty hand. Returns an empty {{cItems}} object if the chunk is not present."
			},
			PrepareChunk =
			{
				Params =
				{
					{
						Name = "ChunkX",
						Type = "number",
					},
					{
						Name = "ChunkZ",
						Type = "number",
					},
					{
						Name = "Callback",
						Type = "function",
						IsOptional = true,
					},
				},
				Notes = "Queues the chunk for preparing - making sure that it's generated and lit. It is legal to call with no callback. The callback function has the following signature: <pre class=\"prettyprint lang-lua\">function Callback(ChunkX, ChunkZ)</pre>",
			},
			QueueBlockForTick =
			{
				Params =
				{
					{
						Name = "BlockX",
						Type = "number",
					},
					{
						Name = "BlockY",
						Type = "number",
					},
					{
						Name = "BlockZ",
						Type = "number",
					},
					{
						Name = "TicksToWait",
						Type = "number",
					},
				},
				Notes = "Queues the specified block to be ticked after the specified number of gameticks.",
			},
			QueueSaveAllChunks =
			{
				Notes = "Queues all chunks to be saved in the world storage thread",
			},
			QueueTask =
			{
				Params =
				{
					{
						Name = "TaskFunction",
						Type = "function",
					},
				},
				Notes = [[
					Queues the specified function to be executed in the tick thread. This is the primary means
					of interaction with a cWorld from the WebAdmin page handlers (see {{WebWorldThreads}}). The function
					signature is <pre class=\"pretty-print lang-lua\">function({{cWorld|World}})</pre>All return values
					from the function are ignored. Note that this function is actually called *after* the QueueTask()
					function returns. Note that it is unsafe to store references to Cuberite objects, such as entities,
					across from the caller to the task handler function; store the EntityID instead.
				]],
			},
			QueueUnloadUnusedChunks =
			{
				Notes = "Queues a cTask that unloads chunks that are no longer needed and are saved.",
			},
			RegenerateChunk =
			{
				Params =
				{
					{
						Name = "ChunkX",
						Type = "number",
					},
					{
						Name = "ChunkZ",
						Type = "number",
					},
				},
				Notes = "Queues the specified chunk to be re-generated, overwriting the current data. To queue a chunk for generating only if it doesn't exist, use the GenerateChunk() instead.",
			},
			ScheduleTask =
			{
				Params =
				{
					{
						Name = "DelayTicks",
						Type = "number",
					},
					{
						Name = "TaskFunction",
						Type = "function",
					},
				},
				Notes = "Queues the specified function to be executed in the world's tick thread after a the specified number of ticks. This enables operations to be queued for execution in the future. The function signature is <pre class=\"pretty-print lang-lua\">function({{cWorld|World}})</pre>All return values from the function are ignored. Note that it is unsafe to store references to Cuberite objects, such as entities, across from the caller to the task handler function; store the EntityID instead.",
			},
			SendBlockTo =
			{
				Params =
				{
					{
						Name = "BlockX",
						Type = "number",
					},
					{
						Name = "BlockY",
						Type = "number",
					},
					{
						Name = "BlockZ",
						Type = "number",
					},
					{
						Name = "Player",
						Type = "cPlayer",
					},
				},
				Notes = "Sends the block at the specified coords to the specified player's client, as an UpdateBlock packet.",
			},
			SetAreaBiome =
			{
				{
					Params =
					{
						{
							Name = "MinX",
							Type = "number",
						},
						{
							Name = "MaxX",
							Type = "number",
						},
						{
							Name = "MinZ",
							Type = "number",
						},
						{
							Name = "MaxZ",
							Type = "number",
						},
						{
							Name = "Biome",
							Type = "EMCSBiome",
						},
					},
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Sets the biome in the rectangular area specified. Returns true if successful, false if any of the chunks were unloaded.",
				},
				{
					Params =
					{
						{
							Name = "Cuboid",
							Type = "cCuboid",
						},
						{
							Name = "Biome",
							Type = "EMCSBiome",
						},
					},
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Sets the biome in the cuboid specified. Returns true if successful, false if any of the chunks were unloaded. The cuboid needn't be sorted.",
				},
			},
			SetBiomeAt =
			{
				Params =
				{
					{
						Name = "BlockX",
						Type = "number",
					},
					{
						Name = "BlockZ",
						Type = "number",
					},
					{
						Name = "Biome",
						Type = "EMCSBiome",
					},
				},
				Returns =
				{
					{
						Type = "boolean",
					},
				},
				Notes = "Sets the biome at the specified block coords. Returns true if successful, false otherwise.",
			},
			SetBlock =
			{
				Params =
				{
					{
						Name = "BlockX",
						Type = "number",
					},
					{
						Name = "BlockY",
						Type = "number",
					},
					{
						Name = "BlockZ",
						Type = "number",
					},
					{
						Name = "BlockType",
						Type = "number",
					},
					{
						Name = "BlockMeta",
						Type = "number",
					},
				},
				Notes = "Sets the block at the specified coords, replaces the block entities for the previous block type, creates a new block entity for the new block, if appropriate, and wakes up the simulators. This is the preferred way to set blocks, as opposed to FastSetBlock(), which is only to be used under special circumstances.",
			},
			SetBlockMeta =
			{
				{
					Params =
					{
						{
							Name = "BlockX",
							Type = "number",
						},
						{
							Name = "BlockY",
							Type = "number",
						},
						{
							Name = "BlockZ",
							Type = "number",
						},
						{
							Name = "BlockMeta",
							Type = "number",
						},
					},
					Notes = "Sets the meta for the block at the specified coords. Any call to SetBlockMeta will not generate a simulator update (water, lava, redstone), consider using SetBlock instead.",
				},
				{
					Params =
					{
						{
							Name = "BlockCoords",
							Type = "Vector3i",
						},
						{
							Name = "BlockMeta",
							Type = "number",
						},
					},
					Notes = "Sets the meta for the block at the specified coords. Any call to SetBlockMeta will not generate a simulator update (water, lava, redstone), consider using SetBlock instead.",
				},
			},
			SetChunkAlwaysTicked =
			{
				Params =
				{
					{
						Name = "ChunkX",
						Type = "number",
					},
					{
						Name = "ChunkZ",
						Type = "number",
					},
					{
						Name = "IsAlwaysTicked",
						Type = "boolean",
					},
				},
				Notes = "Sets the chunk to always be ticked and loaded even when it doesn't contain any clients. IsAlwaysTicked set to true turns forced ticking on, set to false turns it off. Every call with 'true' should be paired with a later call with 'false', otherwise the ticking won't stop. Multiple actions can request ticking independently, the ticking will continue until the last call with 'false'."
			},
			SetCommandBlockCommand =
			{
				Params =
				{
					{
						Name = "BlockX",
						Type = "number",
					},
					{
						Name = "BlockY",
						Type = "number",
					},
					{
						Name = "BlockZ",
						Type = "number",
					},
					{
						Name = "Command",
						Type = "string",
					},
				},
				Returns =
				{
					{
						Type = "boolean",
					},
				},
				Notes = "Sets the command to be executed in a command block at the specified coordinates. Returns if command was changed.",
			},
			SetCommandBlocksEnabled =
			{
				Params =
				{
					{
						Name = "AreEnabled",
						Type = "boolean",
					},
				},
				Notes = "Sets whether command blocks should be enabled on the (entire) server.",
			},
			SetDaylightCycleEnabled =
			{
				Params =
				{
					{
						Name = "IsEnabled",
						Type = "boolean",
					},
				},
				Notes = "Starts or stops the daylight cycle.",
			},
			SetLinkedEndWorldName =
			{
				Params =
				{
					{
						Name = "WorldName",
						Type = "string",
					},
				},
				Notes = "Sets the name of the world that the end portal should link to.",
			},
			SetLinkedNetherWorldName =
			{
				Params =
				{
					{
						Name = "WorldName",
						Type = "string",
					},
				},
				Notes = "Sets the name of the world that the nether portal should link to.",
			},
			SetLinkedOverworldName =
			{
				Params =
				{
					{
						Name = "WorldName",
						Type = "string",
					},
				},
				Notes = "Sets the name of the world that the nether portal should link to?",
			},
			SetMaxNetherPortalHeight =
			{
				Params =
				{
					{
						Name = "Height",
						Type = "number",
					},
				},
				Notes = "Sets the maximum height for a nether portal",
			},
			SetMaxNetherPortalWidth =
			{
				Params =
				{
					{
						Name = "Width",
						Type = "number",
					},
				},
				Notes = "Sets the maximum width for a nether portal",
			},
			SetMaxViewDistance =
			{
				Params =
				{
					{
						Name = "MaxViewDistance",
						Type = "number",
					},
				},
				Notes = "Sets the maximum viewdistance of the players in the world. This maximum takes precedence over each player's ViewDistance setting.",
			},
			SetMinNetherPortalHeight =
			{
				Params =
				{
					{
						Name = "Height",
						Type = "number",
					},
				},
				Notes = "Sets the minimum height for a nether portal",
			},
			SetMinNetherPortalWidth =
			{
				Params =
				{
					{
						Name = "Width",
						Type = "number",
					},
				},
				Notes = "Sets the minimum width for a nether portal",
			},
			SetNextBlockTick =
			{
				Params =
				{
					{
						Name = "BlockX",
						Type = "number",
					},
					{
						Name = "BlockY",
						Type = "number",
					},
					{
						Name = "BlockZ",
						Type = "number",
					},
				},
				Notes = "DEPRECATED, use SetNextBlockToTick() instead.",
			},
			SetNextBlockToTick =
			{
				Params =
				{
					{
						Name = "BlockPos",
						Type = "Vector3i",
					},
				},
				Notes = "Requests that the specified block be ticked at the start of the next world tick. Only one block per chunk can be queued this way; a second call to the same chunk overwrites the previous call.",
			},
			SetSavingEnabled =
			{
				Params =
				{
					{
						Name = "SavingEnabled",
						Type = "boolean",
					},
				},
				Notes = "Sets whether saving chunk data is enabled. If disabled, dirty chunks will stay in memory forever, which may cause performance and stability issues.",
			},
			SetShouldUseChatPrefixes =
			{
				Params =
				{
					{
						Name = "ShouldUseChatPrefixes",
						Type = "boolean",
					},
				},
				Notes = "Sets whether coloured chat prefixes such as [INFO] is used with the SendMessageXXX() or BroadcastChatXXX(), or simply the entire message is coloured in the respective colour.",
			},
			SetSignLines =
			{
				Params =
				{
					{
						Name = "BlockX",
						Type = "number",
					},
					{
						Name = "BlockY",
						Type = "number",
					},
					{
						Name = "BlockZ",
						Type = "number",
					},
					{
						Name = "Line1",
						Type = "string",
					},
					{
						Name = "Line2",
						Type = "string",
					},
					{
						Name = "Line3",
						Type = "string",
					},
					{
						Name = "Line4",
						Type = "string",
					},
					{
						Name = "Player",
						Type = "cPlayer",
						IsOptional = true,
					},
				},
				Notes = "Sets the sign text at the specified coords. The sign-updating hooks are called for the change. The Player parameter is used to indicate the player from whom the change has come, it may be nil.",
			},
			SetSpawn =
			{
				Params =
				{
					{
						Name = "X",
						Type = "number",
					},
					{
						Name = "Y",
						Type = "number",
					},
					{
						Name = "Z",
						Type = "number",
					},
				},
				Returns =
				{
					{
						Type = "boolean",
					},
				},
				Notes = "Sets the default spawn at the specified coords. Returns false if the new spawn couldn't be stored in the INI file.",
			},
			SetTicksUntilWeatherChange =
			{
				Params =
				{
					{
						Name = "NumTicks",
						Type = "number",
					},
				},
				Notes = "Sets the number of ticks after which the weather will be changed.",
			},
			SetTimeOfDay =
			{
				Params =
				{
					{
						Name = "TimeOfDayTicks",
						Type = "number",
					},
				},
				Notes = "Sets the time of day, expressed as number of ticks past sunrise, in the range 0 .. 24000.",
			},
			SetTNTShrapnelLevel =
			{
				Params =
				{
					{
						Name = "ShrapnelLevel",
						Type = "eShrapnelLevel",
					},
				},
				Notes = "Sets the Shrapnel level of the world.",
			},
			SetTrapdoorOpen =
			{
				Params =
				{
					{
						Name = "BlockX",
						Type = "number",
					},
					{
						Name = "BlockY",
						Type = "number",
					},
					{
						Name = "BlockZ",
						Type = "number",
					},
					{
						Name = "IsOpen",
						Type = "boolean",
					},
				},
				Returns =
				{
					{
						Type = "boolean",
					},
				},
				Notes = "Opens or closes a trapdoor at the specific coordinates. Returns true on success, false if there is no trapdoor or it's already in the requested state.",
			},
			SetWeather =
			{
				Params =
				{
					{
						Name = "Weather",
						Type = "eWeather",
					},
				},
				Notes = "Sets the current weather (wSunny, wRain, wStorm) and resets the TicksUntilWeatherChange to the default value for the new weather. The normal weather-changing hooks are called for the change.",
			},
			ShouldBroadcastAchievementMessages =
			{
				Returns =
				{
					{
						Type = "boolean",
					},
				},
				Notes = "Returns true if the server should broadcast achievement messages in this world.",
			},
			ShouldBroadcastDeathMessages =
			{
				Returns =
				{
					{
						Type = "boolean",
					},
				},
				Notes = "Returns true if the server should broadcast death messages in this world.",
			},
			ShouldLavaSpawnFire =
			{
				Returns =
				{
					{
						Type = "boolean",
					},
				},
				Notes = "Returns true if the world is configured to spawn fires near lava (world.ini: [Physics].ShouldLavaSpawnFire value)",
			},
			ShouldUseChatPrefixes =
			{
				Returns =
				{
					{
						Type = "boolean",
					},
				},
				Notes = "Returns whether coloured chat prefixes are prepended to chat messages or the entire message is simply coloured.",
			},
			SpawnBoat =
			{
				{
					Params =
					{
						{
							Name = "Position",
							Type = "Vector3d",
						},
						{
							Name = "Material",
							Type = "cBoat#eMaterial",
						},
					},
					Returns =
					{
						{
							Name = "EntityID",
							Type = "number",
						},
					},
					Notes = "Spawns a {{cBoat|boat}} at the specific coordinates. Returns the EntityID of the new boat, or {{cEntity#INVALID_ID|cEntity#INVALID_ID}} if no boat was created.",
				},
				{
					Params =
					{
						{
							Name = "X",
							Type = "number",
						},
						{
							Name = "Y",
							Type = "number",
						},
						{
							Name = "Z",
							Type = "number",
						},
						{
							Name = "Material",
							Type = "cBoat#eMaterial",
						},
					},
					Returns =
					{
						{
							Name = "EntityID",
							Type = "number",
						},
					},
					Notes = "Spawns a {{cBoat|boat}} at the specific coordinates. Returns the EntityID of the new boat, or {{cEntity#INVALID_ID|cEntity#INVALID_ID}} if no boat was created. (DEPRECATED, use vector-parametered version)",
				},
			},
			SpawnEnderCrystal =
			{
				Params =
				{
					{
						Name = "Pos",
						Type = "Vector3d",
					},
					{
						Name = "ShowBottom",
						Type = "boolean",
					}
				},
				Returns =
				{
					{
						Name = "EntityID",
						Type = "number",
					},
				},
				Notes = "Spawns an {{cEnderCrystal|ender crystal}} at the specified coords. Returns the EntityID of the new ender crystal, or {{cEntity#INVALID_ID|cEntity#INVALID_ID}} if no ender crystal was created.",
			},
			SpawnExperienceOrb =
			{
				Params =
				{
					{
						Name = "X",
						Type = "number",
					},
					{
						Name = "Y",
						Type = "number",
					},
					{
						Name = "Z",
						Type = "number",
					},
					{
						Name = "Reward",
						Type = "number",
					},
				},
				Returns =
				{
					{
						Name = "EntityID",
						Type = "number",
					},
				},
				Notes = "Spawns an {{cExpOrb|experience orb}} at the specified coords, with the given reward. Returns the EntityID of the new experience orb, or {{cEntity#INVALID_ID|cEntity#INVALID_ID}} if no experience orb was created.",
			},
			SpawnFallingBlock =
			{
				{
					Params =
					{
						{
							Name = "X",
							Type = "number",
						},
						{
							Name = "Y",
							Type = "number",
						},
						{
							Name = "Z",
							Type = "number",
						},
						{
							Name = "BlockType",
							Type = "number",
						},
						{
							Name = "BlockMeta",
							Type = "number",
						},
					},
					Returns =
					{
						{
							Name = "EntityID",
							Type = "number",
						},
					},
					Notes = "OBSOLETE, use the Vector3-based overloads instead. Spawns a {{cFallingBlock|Falling Block}} entity at the specified coords with the given block type/meta. Returns the EntityID of the new falling block, or {{cEntity#INVALID_ID|cEntity#INVALID_ID}} if no falling block was created.",
				},
				{
					Params =
					{
						{
							Name = "BlockPos",
							Type = "Vector3i",
						},
						{
							Name = "BlockType",
							Type = "number",
						},
						{
							Name = "BlockMeta",
							Type = "number",
						},
					},
					Returns =
					{
						{
							Name = "EntityID",
							Type = "number",
						},
					},
					Notes = "Spawns a {{cFallingBlock|Falling Block}} entity in the middle of the specified block, with the given block type/meta. Returns the EntityID of the new falling block, or {{cEntity#INVALID_ID|cEntity#INVALID_ID}} if no falling block was created.",
				},
				{
					Params =
					{
						{
							Name = "Pos",
							Type = "Vector3d",
						},
						{
							Name = "BlockType",
							Type = "number",
						},
						{
							Name = "BlockMeta",
							Type = "number",
						},
					},
					Returns =
					{
						{
							Name = "EntityID",
							Type = "number",
						},
					},
					Notes = "Spawns a {{cFallingBlock|Falling Block}} entity at exactly the specified coords, with the given block type/meta. Returns the EntityID of the new falling block, or {{cEntity#INVALID_ID|cEntity#INVALID_ID}} if no falling block was created.",
				},
			},
			SpawnItemPickup =
			{
				Params =
				{
					{
						Name = "PosX",
						Type = "number",
					},
					{
						Name = "PosY",
						Type = "number",
					},
					{
						Name = "PosZ",
						Type = "number",
					},
					{
						Name = "Item",
						Type = "cItem",
					},
					{
						Name = "SpeedX",
						Type = "number",
						IsOptional = true,
						Notes = "Speed along X coordinate to spawn with. Default is 0.",
					},
					{
						Name = "SpeedY",
						Type = "number",
						IsOptional = true,
						Notes = "Speed along Y coordinate to spawn with. Default is 0.",
					},
					{
						Name = "SpeedZ",
						Type = "number",
						IsOptional = true,
						Notes = "Speed along Z coordinate to spawn with. Default is 0.",
					},
					{
						Name = "LifetimeTicks",
						Type = "number",
						IsOptional = true,
						Notes = "Length of the pickups lifetime, in ticks. Default 5 minutes (6000 ticks)",
					},
					{
						Name = "CanCombine",
						Type = "boolean",
						IsOptional = true,
						Notes = "Whether this pickup is allowed to combine with other similar pickups.",
					},
				},
				Returns =
				{
					{
						Name = "EntityID",
						Type = "number",
					}
				},
				Notes = "Creates a single pickup entity of the given item at the given position with the given speed, and returns the entities unique ID."
			},
			SpawnItemPickups =
			{
				{
					Params =
					{
						{
							Name = "Pickups",
							Type = "cItems",
						},
						{
							Name = "X",
							Type = "number",
						},
						{
							Name = "Y",
							Type = "number",
						},
						{
							Name = "Z",
							Type = "number",
						},
						{
							Name = "FlyAwaySpeed",
							Type = "number",
							IsOptional = true,
						},
						{
							Name = "IsPlayerCreated",
							Type = "boolean",
							IsOptional = true,
						},
					},
					Notes = "Spawns the specified pickups at the position specified. The FlyAwaySpeed is a coefficient (default: 1) used to initialize the random speed in which the pickups fly away from the spawn position. The IsPlayerCreated parameter (default: false) is used to initialize the created {{cPickup}} object's IsPlayerCreated value.",
				},
				{
					Params =
					{
						{
							Name = "Pickups",
							Type = "cItems",
						},
						{
							Name = "X",
							Type = "number",
						},
						{
							Name = "Y",
							Type = "number",
						},
						{
							Name = "Z",
							Type = "number",
						},
						{
							Name = "SpeedX",
							Type = "number",
						},
						{
							Name = "SpeedY",
							Type = "number",
						},
						{
							Name = "SpeedZ",
							Type = "number",
						},
						{
							Name = "IsPlayerCreated",
							Type = "boolean",
							IsOptional = true,
						},
					},
					Notes = "Spawns the specified pickups at the position specified. All the pickups fly away from the spawn position using the specified speed. The IsPlayerCreated parameter (default: false) is used to initialize the created {{cPickup}} object's IsPlayerCreated value.",
				},
			},
			SpawnMinecart =
			{
				Params =
				{
					{
						Name = "X",
						Type = "number",
					},
					{
						Name = "Y",
						Type = "number",
					},
					{
						Name = "Z",
						Type = "number",
					},
					{
						Name = "MinecartType",
						Type = "number",
					},
					{
						Name = "Item",
						Type = "cItem",
						IsOptional = true,
					},
					{
						Name = "BlockHeight",
						Type = "number",
						IsOptional = true,
					},
				},
				Returns =
				{
					{
						Name = "EntityID",
						Type = "number",
					},
				},
				Notes = "Spawns a minecart at the specific coordinates. MinecartType is the item type of the minecart. If the minecart is an empty minecart then the given Item (default: empty) is the block to be displayed inside the minecart, and BlockHeight (default: 1) is the relative distance of the block from the minecart. Returns the EntityID of the new minecart, or {{cEntity#INVALID_ID|cEntity#INVALID_ID}} if no minecart was created.",
			},
			SpawnMob =
			{
				Params =
				{
					{
						Name = "X",
						Type = "number",
					},
					{
						Name = "Y",
						Type = "number",
					},
					{
						Name = "Z",
						Type = "number",
					},
					{
						Name = "MonsterType",
						Type = "eMonsterType",
					},
					{
						Name = "IsBaby",
						Type = "boolean",
						IsOptional = true,
					},
				},
				Returns =
				{
					{
						Name = "EntityID",
						Type = "number",
					},
				},
				Notes = "Spawns the specified type of mob at the specified coords. If the Baby parameter is true, the mob will be a baby. Returns the EntityID of the created entity, or {{cEntity#INVALID_ID|cEntity#INVALID_ID}} on failure.",
			},
			SpawnPrimedTNT =
			{
				{
					Params =
					{
						{
							Name = "Position",
							Type = "Vector3d",
						},
						{
							Name = "FuseTicks",
							Type = "number",
						},
						{
							Name = "InitialVelocityCoeff",
							Type = "number",
						},
						{
							Name = "ShouldPlayFuseSound",
							Type = "boolean",
						},
					},
					Returns =
					{
						{
							Name = "EntityID",
							Type = "number",
						},
					},
					Notes = "Spawns a {{cTNTEntity|primed TNT entity}} at the specified coords, with the given fuse ticks. The entity gets a random speed multiplied by the InitialVelocityCoeff, 1 being the default value. Returns the EntityID of the new spawned primed tnt, or {{cEntity#INVALID_ID|cEntity#INVALID_ID}} if no primed tnt was created.",
				},
				{
					Params =
					{
						{
							Name = "X",
							Type = "number",
						},
						{
							Name = "Y",
							Type = "number",
						},
						{
							Name = "Z",
							Type = "number",
						},
						{
							Name = "FuseTicks",
							Type = "number",
						},
						{
							Name = "InitialVelocityCoeff",
							Type = "number",
						},
					},
					Returns =
					{
						{
							Name = "EntityID",
							Type = "number",
						},
					},
					Notes = "Spawns a {{cTNTEntity|primed TNT entity}} at the specified coords, with the given fuse ticks. The entity gets a random speed multiplied by the InitialVelocityCoeff, 1 being the default value. Returns the EntityID of the new spawned primed tnt, or {{cEntity#INVALID_ID|cEntity#INVALID_ID}} if no primed tnt was created. (DEPRECATED, use vector-parametered version)",
				},
			},
			SpawnSplitExperienceOrbs =
			{
				Params =
				{
					{
						Name = "Position",
						Type = "Vector3d",
					},
					{
						Name = "Reward",
						Type = "number",
					},
				},
				Returns =
				{
					{
						Name = "EntityID",
						Type = "table",
					},
				},
				Notes = "Spawns experience orbs of the specified total value at the given location. The orbs' values are split according to regular Minecraft rules. Returns an array-table of UniqueID of all the orbs.",
			},
			TryGetHeight =
			{
				Params =
				{
					{
						Name = "BlockX",
						Type = "number",
					},
					{
						Name = "BlockZ",
						Type = "number",
					},
				},
				Returns =
				{
					{
						Name = "IsValid",
						Type = "boolean",
					},
					{
						Name = "Height",
						Type = "number",
					},
				},
				Notes = "Returns true and height of the highest non-air block if the chunk is loaded, or false otherwise.",
			},
			UpdateSign =
			{
				Params =
				{
					{
						Name = "BlockX",
						Type = "number",
					},
					{
						Name = "BlockY",
						Type = "number",
					},
					{
						Name = "BlockZ",
						Type = "number",
					},
					{
						Name = "Line1",
						Type = "string",
					},
					{
						Name = "Line2",
						Type = "string",
					},
					{
						Name = "Line3",
						Type = "string",
					},
					{
						Name = "Line4",
						Type = "string",
					},
					{
						Name = "Player",
						Type = "cPlayer",
						IsOptional = true,
					},
				},
				Notes = "(<b>DEPRECATED</b>) Please use SetSignLines().",
			},
			UseBlockEntity =
			{
				Params =
				{
					{
						Name = "Player",
						Type = "cPlayer",
					},
					{
						Name = "BlockX",
						Type = "number",
					},
					{
						Name = "BlockY",
						Type = "number",
					},
					{
						Name = "BlockZ",
						Type = "number",
					},
				},
				Notes = "Makes the specified Player use the block entity at the specified coords (open chest UI, etc.) If the cords are in an unloaded chunk or there's no block entity, ignores the call.",
			},
			VillagersShouldHarvestCrops =
			{
				Notes = "Returns true if villagers can harvest crops.",
				Returns =
				{
					{
						Type = "boolean",
					},
				},
			},
			WakeUpSimulators =
			{
				{
					Params =
					{
						{
							Name = "Block",
							Type = "Vector3i",
						},
					},
					Notes = "Wakes up the simulators for the specified block.",
				},
				{
					Params =
					{
						{
							Name = "BlockX",
							Type = "number",
						},
						{
							Name = "BlockY",
							Type = "number",
						},
						{
							Name = "BlockZ",
							Type = "number",
						},
					},
					Notes = "Wakes up the simulators for the specified block. (DEPRECATED, use vector-parametered version)",
				},
			},
			WakeUpSimulatorsInArea =
			{
				{
					Params =
					{
						{
							Name = "Area",
							Type = "cCuboid",
						},
					},
					Notes = "Wakes up the simulators for all the blocks in the specified area (edges inclusive).",
				},
				{
					Params =
					{
						{
							Name = "MinBlockX",
							Type = "number",
						},
						{
							Name = "MaxBlockX",
							Type = "number",
						},
						{
							Name = "MinBlockY",
							Type = "number",
						},
						{
							Name = "MaxBlockY",
							Type = "number",
						},
						{
							Name = "MinBlockZ",
							Type = "number",
						},
						{
							Name = "MaxBlockZ",
							Type = "number",
						},
					},
					Notes = "Wakes up the simulators for all the blocks in the specified area (edges inclusive). (DEPRECATED, use vector-parametered version)",
				},
			},
		},
		AdditionalInfo =
		{
			{
				Header = "Using callbacks",
				Contents = [[
					To avoid problems with stale objects, the cWorld class will not let plugins get a direct pointer
					to an {{cEntity|entity}}, {{cBlockEntity|block entity}} or a {{cPlayer|player}}. Such an object
					could be modified or even destroyed by another thread while the plugin holds it, so it would be
					rather unsafe.</p>
					<p>
					Instead, the cWorld provides access to these objects using callbacks. The plugin provides a
					function that is called and receives the object as a parameter; cWorld guarantees that while
					the callback is executing, the object will stay valid. If a plugin needs to "remember" the
					object outside of the callback, it needs to store the entity ID, blockentity coords or player
					name.</p>
					<p>
					The following code examples show how to use the callbacks</p>
					<p>
					This code teleports player Player to another player named ToName in the same world:
<pre class="prettyprint lang-lua">
-- Player is a cPlayer object
-- ToName is a string
-- World is a cWorld object
World:ForEachPlayer(
	function (a_OtherPlayer)
	if (a_OtherPlayer:GetName() == ToName) then
		Player:TeleportToEntity(a_OtherPlayer);
	end
);
</pre></p>
<p>
This code fills each furnace in the chunk with 64 coals:
<pre class="prettyprint lang-lua">
-- Player is a cPlayer object
-- World is a cWorld object
World:ForEachFurnaceInChunk(Player:GetChunkX(), Player:GetChunkZ(),
	function (a_Furnace)
		a_Furnace:SetFuelSlot(cItem(E_ITEM_COAL, 64));
	end
);
</pre></p>
<p>
This code teleports all spiders up by 100 blocks:
<pre class="prettyprint lang-lua">
-- World is a cWorld object
World:ForEachEntity(
	function (a_Entity)
		if not(a_Entity:IsMob()) then
			return;
		end

		-- Now that we know the entity represents a mob, we can use cMonster functions:
		if (a_Entity:GetMobType() == mtSpider) then
			a_Entity:TeleportToCoords(a_Entity:GetPosX(), a_Entity:GetPosY() + 100, a_Entity:GetPosZ());
		end
	end
);
</pre></p>
				]],
			},
		},
	},
}
