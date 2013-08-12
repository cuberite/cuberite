-- This plugin copyright Alexander Harkness 2013, licensed under the MIT license.

-- Configuration
g_ServerLang  = "english"
g_ConsoleLang = "english"

-- Global Variables
g_Plugin = nil
g_PluginManager = nil
g_PluginDir = nil
g_UserData

-- START WITH DA AWESOME!
function Initialize( Plugin )

	-- Set up the globals.
	g_Plugin        = Plugin
	g_PluginManager = cRoot:Get():GetPluginManager()
	g_PluginDir     = Plugin:GetDirectory()

	-- Set up the plugin details.
	Plugin:SetName( "TransAPI" )
	Plugin:SetVersion( 1 )

	-- This is the place for commands!
	PluginManager:BindCommand("/language", "transapi.setlang", HandleLanguageCommand, " - Set your preferred language.")

	-- Load the userdata file.


	LOG( "Initialized " .. Plugin:GetName() .. " v." .. Plugin:GetVersion() )

	return true

end

function GetLanguage( user )

	-- Returns a language to use.

end

function GetConsoleLanguage()
	-- Return the language to use for console messages.
	return g_ConsoleLang
end

function HandleLanguageCommand ( Split, Player )
	return true
end

function OnDisable()
	LOG( "Disabled TransAPI!" )
end
