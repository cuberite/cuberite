
-- Info.lua

-- Implements the g_PluginInfo standard plugin description





g_PluginInfo =
{
	Name = "Debuggers",
	Version = "14",
	Date = "2014-12-11",
	Description = [[Contains code for testing and debugging the server. Should not be enabled on a production server!]],

	Commands =
	{
		["/arr"] =
		{
			Permission = "debuggers",
			Handler = HandleArrowCmd,
			HelpString = "Creates an arrow going away from the player"
		},

		["/blk"] =
		{
			Permission = "debuggers",
			Handler = HandleBlkCmd,
			HelpString = "Gets info about the block you are looking at"
		},

		["/boom"] =
		{
			Permission = "debuggers",
			Handler = HandleBoomCmd,
			HelpString = "Playes a sound and displays an effect at the player's position",
		},

		["/cake"] =
		{
			Permission = "debuggers",
			Handler = HandleCakeCmd,
			HelpString = "Throws a cake in the direction the player's looking, in a slow arc.",
		},

		["/clientversion"] =
		{
			Permission = "debuggers",
			Handler = HandleClientVersionCmd,
			HelpString = "Shows your client's protocol version",
		},
		["/come"] =
		{
			Permission = "debuggers.come",
			Handler = HandleComeCmd,
			HelpString = "Instruct the mob you're looking at to navigate to your position",
		},
		["/compo"] =
		{
			Permission = "debuggers",
			Handler = HandleCompo,
			HelpString = "Tests the cCompositeChat bindings"
		},
		["/cstay"] =
		{
			Permission = "debuggers",
			Handler = HandleChunkStay,
			HelpString = "Tests the ChunkStay Lua integration for the specified chunk coords"
		},
		["/dash"] =
		{
			Permission = "debuggers",
			Handler = HandleDashCmd,
			HelpString = "Switches between fast and normal sprinting speed"
		},
		["/ench"] =
		{
			Permission = "debuggers",
			Handler = HandleEnchCmd,
			HelpString = "Provides an instant dummy enchantment window"
		},
		["/fast"] =
		{
			Permission = "debuggers",
			Handler = HandleFastCmd,
			HelpString = "Switches between fast and normal movement speed"
		},
		["/fb"] =
		{
			Permission = "debuggers",
			Handler = HandleFireballCmd,
			HelpString = "Creates a ghast fireball as if shot by the player"
		},
		["/ff"] =
		{
			Permission = "debuggers",
			Handler = HandleFurnaceFuel,
			HelpString = "Shows how long the currently held item would burn in a furnace"
		},
		["/fill"] =
		{
			Permission = "debuggers",
			Handler = HandleFill,
			HelpString = "Fills all block entities in current chunk with junk"
		},
		["/fl"] =
		{
			Permission = "debuggers",
			Handler = HandleFoodLevelCmd,
			HelpString = "Sets the food level to the given value"
		},
		["/fr"] =
		{
			Permission = "debuggers",
			Handler = HandleFurnaceRecipe,
			HelpString = "Shows the furnace recipe for the currently held item"
		},
		["/fs"] =
		{
			Permission = "debuggers",
			Handler = HandleFoodStatsCmd,
			HelpString = "Turns regular foodstats message on or off"
		},
		["/gc"] =
		{
			Permission = "debuggers",
			Handler = HandleGCCmd,
			HelpString = "Activates the Lua garbage collector"
		},
		["/genrails"] =
		{
			Permission = "debuggers",
			Handler = HandleGenRailsCmd,
			HelpString = "Generates rail blocks with all metas from current block towards X+",
		},
		["/getcustomname"] =
		{
			Permission = "debuggers",
			Handler = HandleGetCustomNameCmd,
			HelpString = "Displays the exact custom name of currently held item (including non-printables)",
		},
		["/getlore"] =
		{
			Permission = "debuggers",
			Handler = HandleGetLoreCmd,
			HelpString = "Displays the exact Lore of currently held item (including non-printables)",
		},
		["/getprop"] =
		{
			Permission = "debuggers",
			Handler = HandleGetPropCmd,
			HelpString = "Displays the custom cItem property of the currently held item",
		},
		["/hunger"] =
		{
			Permission = "debuggers",
			Handler = HandleHungerCmd,
			HelpString = "Lists the current hunger-related variables"
		},
		["/ke"] =
		{
			Permission = "debuggers",
			Handler = HandleKillEntitiesCmd,
			HelpString = "Kills all the loaded entities"
		},
		["/le"] =
		{
			Permission = "debuggers",
			Handler = HandleListEntitiesCmd,
			HelpString = "Shows a list of all the loaded entities"
		},
		["/nick"] =
		{
			Permission = "debuggers",
			Handler = HandleNickCmd,
			HelpString = "Gives you a custom name",
		},
		["/pickups"] =
		{
			Permission = "debuggers",
			Handler = HandlePickups,
			HelpString = "Spawns random pickups around you"
		},
		["/plugmsg"] =
		{
			Permission = "debuggers",
			Handler = HandlePlugMsg,
			HelpString = "Sends a test plugin message to the client",
		},
		["/poison"] =
		{
			Permission = "debuggers",
			Handler = HandlePoisonCmd,
			HelpString = "Sets food-poisoning for 15 seconds"
		},
		["/poof"] =
		{
			Permission = "debuggers",
			Handler = HandlePoof,
			HelpString = "Nudges pickups close to you away from you"
		},
		["/rmitem"] =
		{
			Permission = "debuggers",
			Handler = HandleRMItem,
			HelpString = "Remove the specified item from the inventory."
		},
		["/sb"] =
		{
			Permission = "debuggers",
			Handler = HandleSetBiome,
			HelpString = "Sets the biome around you to the specified one"
		},
		["/sched"] =
		{
			Permission = "debuggers",
			Handler = HandleSched,
			HelpString = "Schedules a simple countdown using cWorld:ScheduleTask()"
		},
		["/setcustomname"] =
		{
			Permission = "debuggers",
			Handler = HandleSetCustomNameCmd,
			HelpString = "Sets the custom name for the item currently in hand",
		},
		["/setlore"] =
		{
			Permission = "debuggers",
			Handler = HandleSetLoreCmd,
			HelpString = "Sets the lore for the item currently in hand",
		},
		["/setprop"] =
		{
			Permission = "debuggers",
			Handler = HandleSetPropCmd,
			HelpString = "Sets the custom property for the item currently in hand",
		},
		["/spidey"] =
		{
			Permission = "debuggers",
			Handler = HandleSpideyCmd,
			HelpString = "Shoots a line of web blocks until it hits non-air"
		},
		["/starve"] =
		{
			Permission = "debuggers",
			Handler = HandleStarveCmd,
			HelpString = "Sets the food level to zero"
		},
		["/teams"] =
		{
			Permission = "debuggers",
			Handler = HandleTeamsCmd,
			HelpString = "List the teams"
		},
		["/testwnd"] =
		{
			Permission = "debuggers",
			Handler = HandleTestWndCmd,
			HelpString = "Opens up a window using plugin API"
		},

		["/vector"] =
		{
			Permission = "debuggers",
			Handler = HandleVectorCmd,
			HelpString = "Tests the Vector3 API",
		},

		["/wesel"] =
		{
			Permission = "debuggers",
			Handler = HandleWESel,
			HelpString = "Expands the current WE selection by 1 block in X/Z"
		},
		["/wool"] =
		{
			Permission = "debuggers",
			Handler = HandleWoolCmd,
			HelpString = "Sets all your armor to blue wool"
		},
		["/xpa"] =
		{
			Permission = "debuggers",
			Handler = HandleAddExperience,
			HelpString = "Adds 200 experience to the player"
		},
		["/xpr"] =
		{
			Permission = "debuggers",
			Handler = HandleRemoveXp,
			HelpString = "Remove all xp"
		},
	},  -- Commands

	ConsoleCommands =
	{
		["bbox"] =
		{
			Handler = HandleConsoleBBox,
			HelpString = "Performs cBoundingBox API tests",
		},

		["deadlock"] =
		{
			Handler = HandleConsoleDeadlock,
			HelpString = "Simulates a deadlock, either on the command execution thread, or on a world tick thread",
		},

		["download"] =
		{
			Handler = HandleConsoleDownload,
			HelpString = "Downloads a file from a specified URL",
		},

		["hash"] =
		{
			Handler = HandleConsoleHash,
			HelpString = "Tests the crypto hashing functions",
		},

		["hittrace"] =
		{
			Handler = HandleConsoleHitTrace,
			HelpString = "Tests the FirstSolidHit trace",
		},

		["inh"] =
		{
			Handler = HandleConsoleInh,
			HelpString = "Tests the bindings of the cEntity inheritance",
		},

		["loadchunk"] =
		{
			Handler = HandleConsoleLoadChunk,
			HelpString = "Loads the specified chunk into memory",
		},

		["lostrace"] =
		{
			Handler = HandleConsoleLosTrace,
			HelpString = "Tests a LineOfSight trace",
		},

		["pluginstats"] =
		{
			Handler = HandleConsolePluginStats,
			HelpString = "Shows the stats for each plugin",
		},

		["preparechunk"] =
		{
			Handler = HandleConsolePrepareChunk,
			HelpString = "Prepares the specified chunk completely (load / gen / light)",
		},

		["sched"] =
		{
			Handler = HandleConsoleSchedule,
			HelpString = "Tests the world scheduling",
		},

		["testbbox"] =
		{
			Handler = HandleConsoleTestBbox,
			HelpString = "Tests cBoundingBox API"
		},

		["testcall"] =
		{
			Handler = HandleConsoleTestCall,
			HelpString = "Tests inter-plugin calls with various values"
		},

		["testerr"] =
		{
			Handler = HandleConsoleTestErr,
			HelpString = "Tests the server's ability to recover from errors in callbacks (GH #3733)",
		},

		["testjson"] =
		{
			Handler = HandleConsoleTestJson,
			HelpString = "Tests the cJson parser and serializer",
		},

		["testtracer"] =
		{
			Handler = HandleConsoleTestTracer,
			HelpString = "Tests the cLineBlockTracer",
		},

		["testtracerdeprecated"] =
		{
			Handler = HandleConsoleTestTracerDeprecated,
			HelpString = "Tests the cLineBlockTracer's deprecated API",
		},

		["testurlclient"] =
		{
			Handler = HandleConsoleTestUrlClient,
			HelpString = "Tests the cUrlClient",
		},

		["testurlparser"] =
		{
			Handler = HandleConsoleTestUrlParser,
			HelpString = "Tests the cUrlParser",
		},

		["uuid"] =
		{
			Handler = HandleConsoleUuid,
			HelpString = "Queries the cMojangAPI for a player's UUID",
		}
	},  -- ConsoleCommands
}  -- g_PluginInfo



