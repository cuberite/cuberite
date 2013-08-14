function HandleDoCommand( Split, Player )

	if #Split < 3 then
		SendMessage( "Usage: /do <player> <command> [arguments]" )
		return true
	end

	-- Get the command and arguments.
	local newSplit = table.concat( Split, " ", 3 )

	local pluginManager = cRoot:Get():GetPluginManager()
	pluginManager:ExecuteCommand( Split[2], newSplit )

end

function HandleSudoCommand ( Split, Player )

	if #Split < 3 then
		SendMessage( "Usage: /sudo <player> <command> [arguments]" )
		return true
	end

	-- Get the command and arguments.
	local newSplit = table.concat( Split, " ", 3 )

	local pluginManager = cRoot:Get():GetPluginManager()
	pluginManager:ForceExecuteCommand( Split[2], newSplit )

end
