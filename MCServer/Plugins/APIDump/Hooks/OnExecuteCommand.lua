return
{
	HOOK_EXECUTE_COMMAND =
	{
		CalledWhen = "A player executes an in-game command, or the admin issues a console command. Note that built-in console commands are exempt to this hook - they are always performed and the hook is not called.",
		DefaultFnName = "OnExecuteCommand",  -- also used as pagename
		Desc = [[
			A plugin may implement a callback for this hook to intercept both in-game commands executed by the
			players and console commands executed by the server admin. The function is called for every in-game
			command sent from any player and for those server console commands that are not built in in the
			server.</p>
			<p>
			If the command is in-game, the first parameter to the hook function is the {{cPlayer|player}} who's
			executing the command. If the command comes from the server console, the first parameter is nil.
		]],
		Params =
		{
			{ Name = "Player", Type = "{{cPlayer}}", Notes = "For in-game commands, the player who has sent the message. For console commands, nil" },
			{ Name = "Command", Type = "table of strings", Notes = "The command and its parameters, broken into a table by spaces" },
		},
		Returns = [[
			If the plugin returns true, the command will be blocked and none of the remaining hook handlers will
			be called. If the plugin returns false, MCServer calls all the remaining hook handlers and finally
			the command will be executed.
		]],
	},  -- HOOK_EXECUTE_COMMAND
}




