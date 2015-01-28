
-- Info.lua

-- Implements the g_PluginInfo standard plugin description

g_PluginInfo =
{
	Name = "NetworkTest",
	Version = "1",
	Date = "2015-01-28",
	Description = [[Implements test code (and examples) for the cNetwork API]],
	
	Commands =
	{
	},
	
	ConsoleCommands =
	{
		net =
		{
			Subcommands =
			{
				client =
				{
					HelpString = "Connects, as a client, to a specified webpage (google.com by default) and downloads its front page using HTTP",
					Handler = HandleConsoleNetClient,
					ParameterCombinations =
					{
						{
							Params = "",
							Help = "Connects, as a client, to google.com and downloads its front page using HTTP",
						},
						{
							Params = "host [port]",
							Help = "Connects, as a client, to the specified host and downloads its front page using HTTP",
						},
					},  -- ParameterCombinations
				},  -- client
			},  -- Subcommands
		},  -- net
	},
}




