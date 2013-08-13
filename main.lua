-- This plugin copyright Alexander Harkness 2013, licensed under the MIT license.

-- Configuration
g_ServerLang    = "english"
g_ConsoleLang   = "english"

-- Global Variables
g_Plugin        = nil
g_PluginManager = nil
g_PluginDir     = nil
g_UserData      = nil

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
	g_PluginManager:BindCommand("/language", "transapi.setlang", HandleLanguageCommand, " - Set your preferred language.")

	-- Load the userdata file.
	g_UserData = cIniFile( g_PluginDir .. "/userdata.ini" )
	if g_UserData ~= true then
		LOGERROR( "TransAPI INI file could not be opened!" )
	end

	LOG( "Initialized " .. Plugin:GetName() .. " v." .. Plugin:GetVersion() )

	return true

end

function GetLanguage( Player )

	-- Returns a language to use.
	if g_UserData:ReadFile() == true then
		local userLang = g_UserData:GetValueSet( Player:GetName(), "language", "false" )
		g_UserData:WriteFile()
	end

	if userLang == "false" then
		return g_ServerLang
	else
		return userLang
	end

end

function GetConsoleLanguage()
	-- Return the language to use for console messages.
	return g_ConsoleLang
end

function HandleLanguageCommand ( Split, Player )

	-- If the user is not setting the language, tell them the currently selected one.
	if #Split ~= 2 then

		local userLang = g_UserData:GetValueSet( Player:GetName(), "language", "false" )
		if userLang == "false" then
			return g_ServerLang
		else
			return userLang
		end

	end

	-- Set the language.
	local success = g_UserData:SetValue( Player:GetName(), "language" Split[2] )
	g_UserData:WriteFile()

	if not success then
		Player:SendMessage( "Language could not be set!" )
	else
		Player:SendMessage( "Language set!" )
	end

	return true

end

function OnDisable()
	LOG( "Disabled TransAPI!" )
end
