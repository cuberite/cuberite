
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
				
				close =
				{
					HelpString = "Close a listening socket",
					Handler = HandleConsoleNetClose,
					ParameterCombinations =
					{
						{
							Params = "[Port]",
							Help = "Closes a socket listening on the specified port [1024]",
						},
					},  -- ParameterCombinations
				},  -- close
				
				listen =
				{
					HelpString = "Creates a new listening socket on the specified port with the specified service attached to it",
					Handler = HandleConsoleNetListen,
					ParameterCombinations =
					{
						{
							Params = "[Port] [Service]",
							Help = "Starts listening on the specified port [1024] providing the specified service [echo]",
						},
					},  -- ParameterCombinations
				},  -- listen
				
				lookup =
				{
					HelpString = "Looks up the IP addresses corresponding to the given hostname  (google.com by default)",
					Handler = HandleConsoleNetLookup,
					ParameterCombinations =
					{
						{
							Params = "",
							Help = "Looks up the IP addresses of google.com.",
						},
						{
							Params = "Hostname",
							Help = "Looks up the IP addresses of the specified hostname.",
						},
						{
							Params = "IP",
							Help = "Looks up the canonical name of the specified IP.",
						},
					},
				},  -- lookup
				
			},  -- Subcommands
		},  -- net
	},
}




