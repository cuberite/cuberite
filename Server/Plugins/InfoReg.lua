
-- InfoReg.lua

--[[
Implements functions that process the standard PluginInfo description and register command handlers.
The description is either given as a parameter to the registration functions, or read from the standard
names gPluginInfo or g_PluginInfo.
--]]





--- Lists all the subcommands that the player has permissions for
local function ListSubcommands(a_Player, a_Subcommands, a_CmdString)
	if (a_Player == nil) then
		LOGINFO("The " .. a_CmdString .. " command requires another verb:")
	else
		a_Player:SendMessage("The " .. a_CmdString .. " command requires another verb:")
	end

	-- Enum all the subcommands:
	local Verbs = {}
	for cmd, info in pairs(a_Subcommands) do
		if ((a_Player == nil) or (a_Player:HasPermission(info.Permission or ""))) then
			table.insert(Verbs, a_CmdString .. " " .. cmd)
		end
	end
	table.sort(Verbs)

	-- Send the list:
	if (a_Player == nil) then
		for idx, verb in ipairs(Verbs) do
			LOGINFO("  " .. verb)
		end
	else
		for idx, verb in ipairs(Verbs) do
			a_Player:SendMessage(cCompositeChat("  ", mtInfo):AddSuggestCommandPart(verb, verb))
		end
	end
end





--- This is a generic command callback used for handling multicommands' parent commands
-- For example, if there are "/gal save" and "/gal load" commands, this callback handles the "/gal" command
-- It is used for both console and in-game commands; the console version has a_Player set to nil
local function MultiCommandHandler(a_Split, a_Player, a_CmdString, a_CmdInfo, a_Level, a_EntireCommand)
	local Verb = a_Split[a_Level + 1]
	if (Verb == nil) then
		-- No verb was specified. If there is a handler for the upper level command, call it:
		if (a_CmdInfo.Handler ~= nil) then
			return a_CmdInfo.Handler(a_Split, a_Player, a_EntireCommand)
		end
		-- Let the player know they need to give a subcommand:
		assert(type(a_CmdInfo.Subcommands) == "table", "Info.lua error: There is no handler for command \"" .. a_CmdString .. "\" and there are no subcommands defined at level " .. a_Level)
		ListSubcommands(a_Player, a_CmdInfo.Subcommands, a_CmdString)
		return true
	end

	-- A verb was specified, look it up in the subcommands table:
	local Subcommand = a_CmdInfo.Subcommands[Verb]
	if (Subcommand == nil) then
		if (a_Level + 1 > 1) then
			-- This is a true subcommand, display the message and make MCS think the command was handled
			-- Otherwise we get weird behavior: for "/cmd verb" we get "unknown command /cmd" although "/cmd" is valid
			if (a_Player == nil) then
				LOGWARNING("The " .. a_CmdString .. " command doesn't support verb " .. Verb)
			else
				a_Player:SendMessage("The " .. a_CmdString .. " command doesn't support verb " .. Verb)
			end
			return true
		end
		-- This is a top-level command, let MCS handle the unknown message
		return false;
	end

	-- Check the permission:
	if (a_Player ~= nil) then
		if not(a_Player:HasPermission(Subcommand.Permission or "")) then
			a_Player:SendMessage("You don't have permission to execute this command")
			return true
		end
	end

	-- First check if the subcommand has subcommands
	if (Subcommand.Subcommands ~= nil) then
		-- Next sublevel
		return MultiCommandHandler(a_Split, a_Player, a_CmdString .. " " .. Verb, Subcommand, a_Level + 1, a_EntireCommand)
	elseif (Subcommand.Handler == nil) then
		-- Subcommand has no subcommands and the handler is not found, report error
		LOGWARNING("Cannot find handler for command " .. a_CmdString .. " " .. Verb)
		return false
	end

	-- Execute:
	return Subcommand.Handler(a_Split, a_Player, a_EntireCommand)
end





--- Registers all commands specified in the aPluginInfo.Commands
-- If aPluginInfo is not given, gPluginInfo or g_PluginInfo is used
function RegisterPluginInfoCommands(aPluginInfo)
	-- If no info given, assume the default name for the global variable
	aPluginInfo = aPluginInfo or g_PluginInfo or gPluginInfo

	-- A sub-function that registers all subcommands of a single command, using the command's Subcommands table
	-- The a_Prefix param already contains the space after the previous command
	-- a_Level is the depth of the subcommands being registered, with 1 being the top level command
	local function RegisterSubcommands(a_Prefix, a_Subcommands, a_Level)
		assert(a_Subcommands ~= nil)

		-- A table that will hold aliases to subcommands temporarily, during subcommand iteration
		local AliasTable = {}

		-- Iterate through the subcommands, register them, and accumulate aliases:
		for cmd, info in pairs(a_Subcommands) do
			local CmdName = a_Prefix .. cmd
			local Handler = info.Handler
			-- Provide a special handler for multicommands:
			if (info.Subcommands ~= nil) then
				Handler = function(a_Split, a_Player, a_EntireCommand)
					return MultiCommandHandler(a_Split, a_Player, CmdName, info, a_Level, a_EntireCommand)
				end
			end

			if (Handler == nil) then
				LOGWARNING(aPluginInfo.Name .. ": Invalid handler for command " .. CmdName .. ", command will not be registered.")
			else
				local HelpString
				if (info.HelpString ~= nil) then
					HelpString = " - " .. info.HelpString
				else
					HelpString = ""
				end
				cPluginManager:BindCommand(CmdName, info.Permission or "", Handler, HelpString)
				-- Register all aliases for the command:
				if (info.Alias ~= nil) then
					if (type(info.Alias) == "string") then
						info.Alias = {info.Alias}
					end
					for idx, alias in ipairs(info.Alias) do
						cPluginManager:BindCommand(a_Prefix .. alias, info.Permission or "", Handler, HelpString)
						-- Also copy the alias's info table as a separate subcommand,
						-- so that MultiCommandHandler() handles it properly. Need to off-load into a separate table
						-- than the one we're currently iterating and join after the iterating.
						AliasTable[alias] = info
					end
				end
			end  -- else (if Handler == nil)

			-- Recursively register any subcommands:
			if (info.Subcommands ~= nil) then
				RegisterSubcommands(a_Prefix .. cmd .. " ", info.Subcommands, a_Level + 1)
			end
		end  -- for cmd, info - a_Subcommands[]

		-- Add the subcommand aliases that were off-loaded during registration:
		for alias, info in pairs(AliasTable) do
			a_Subcommands[alias] = info
		end
		AliasTable = {}
	end

	-- Loop through all commands in the plugin info, register each:
	if (aPluginInfo.Commands) then
		RegisterSubcommands("", aPluginInfo.Commands, 1)
	end
end





--- Registers all console commands specified in the aPluginInfo.ConsoleCommands
-- If aPluginInfo is not given, gPluginInfo or g_PluginInfo is used
function RegisterPluginInfoConsoleCommands(aPluginInfo)
	-- If no info given, assume the default name for the global variable
	aPluginInfo = aPluginInfo or g_PluginInfo or gPluginInfo

	-- A sub-function that registers all subcommands of a single command, using the command's Subcommands table
	-- The a_Prefix param already contains the space after the previous command
	local function RegisterSubcommands(a_Prefix, a_Subcommands, a_Level)
		assert(a_Subcommands ~= nil)

		for cmd, info in pairs(a_Subcommands) do
			local CmdName = a_Prefix .. cmd
			local Handler = info.Handler
			if (Handler == nil) then
				Handler = function(a_Split, a_EntireCommand)
					return MultiCommandHandler(a_Split, nil, CmdName, info, a_Level, a_EntireCommand)
				end
			end
			cPluginManager:BindConsoleCommand(CmdName, Handler, info.HelpString or "")
			-- Recursively register any subcommands:
			if (info.Subcommands ~= nil) then
				RegisterSubcommands(a_Prefix .. cmd .. " ", info.Subcommands, a_Level + 1)
			end
		end
	end

	-- Loop through all commands in the plugin info, register each:
	if (aPluginInfo.ConsoleCommands) then
		RegisterSubcommands("", aPluginInfo.ConsoleCommands, 1)
	end
end





function RegisterWebController(a_Page, a_UrlPath, a_Controller)
	cWebAdmin:AddWebTab(a_Page, a_UrlPath, function(a_Request, a_UrlPath)
		local controller = a_Controller:new();
		local action = a_Request.Params['action'] or 'Index';
		return controller[a_Request.Method .. action](controller, a_Request, a_UrlPath)
	end)
end





-- Register all the webadmin pages inside g_PluginInfo.WebAdminPages
function RegisterPluginInfoWebAdminPages()
	if (g_PluginInfo.WebAdminPages) then
		for idx, page in ipairs(g_PluginInfo.WebAdminPages) do
			RegisterWebController(page.Title, page.UrlPath, page.WebController)
		end
	end
end





-- Quick and dirty templating engine.
-- Allows lua code to be executed by putting it between {={ }=}.
-- Echoing can be done by putting something between {{ }}
local function CompileView(a_Content)
	content = 'return table.concat({[===[' .. a_Content .. ']===]})';
	content = content:gsub("%{=%{(.-)%}=%}", function(logic)
		logic = logic
			:gsub("<>(.-)</>", "table.insert(__RESULTING_CONTENT__, [===[%1]===])")
			:gsub("%{%{(.-)%}%}", "table.insert(__RESULTING_CONTENT__, cWebAdmin:GetHTMLEscapedString(%1))");
			
		return [[]===], (
				function() 
					local __RESULTING_CONTENT__ = {};
				]] .. logic .. [[
					return table.concat(__RESULTING_CONTENT__)
				end
			)(), [===[
		]];
	end)
	
	content = content:gsub("%{%{(.-)%}%}", 
		function(match) 
			return ']===], cWebAdmin:GetHTMLEscapedString(' .. match .. '), [===[' 
		end
	)
	return content;
end





-- A base class for a controller. Other controllers can inherit from it to make use of it's functionality
WebControllerBase = {}





-- Base constructor for the web controller.
function WebControllerBase:new()
	local obj = setmetatable({}, WebControllerBase);
	return obj
end





-- Loads the requested view into Lua bytecode to be executed.
-- The view should be located inside the plugin's folder in an additional folder called 'webviews'.
function WebControllerBase:compileView(a_ViewName)
	local viewPath = cPluginManager:GetCurrentPlugin():GetLocalFolder() .. '/webviews/' .. a_ViewName .. '.html';
	if (not cFile:IsFile(viewPath)) then
		error("View does not exist");
	end
	
	local content = cFile:ReadWholeFile(viewPath);
	content = CompileView(content);
	
	local view, err = loadstring(content);
	if (not view) then
		error(err)
	end
	return view;
end





-- Serializes provided object to json and returns it together with the content type.
function WebControllerBase:json(a_Obj, a_Options)
	return cJson:Serialize(a_Obj, a_Options), "application/json";
end





-- Reads the provided path and returns it's content.
function WebControllerBase:file(a_Path)
	if (not cFile:IsFile(a_Path)) then
		error("The requested file does not exist");
	end
	return cFile:ReadWholeFile(a_Path);
end





-- Compiles the requested view, executes it and returns it's content.
function WebControllerBase:view(a_ViewName, a_Model)
	-- ToDo: cache view so we don't have to recompile it every time.
	local view = self:compileView(a_ViewName);
	
	-- Set the environment of the view to that of the model with the global environment as fallback.
	-- This way variables inside the model can be accessed directly.
	setfenv(view, setmetatable(a_Model or {}, {__index = _G}));
	
	-- Execute and return the content of the view.
	return view();
end




