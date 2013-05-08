-- Some HTML helper functions
local function HTML_Option( value, text, selected )
	if( selected == true ) then
		return [[<option value="]] .. value .. [[" selected>]] .. text .. [[</option>]]
	else
		return [[<option value="]] .. value .. [[">]] .. text .. [[</option>"]]
	end
end

local function HTML_Select_On_Off( name, defaultValue )
	return [[<select name="]] .. name .. [[">]]
		.. HTML_Option("1", "On",  defaultValue == 1 )
		.. HTML_Option("0", "Off", defaultValue == 0 )
		.. [[</select>]]
end

local function HTML_Select_Version( name, defaultValue )
	return [[<select name="]] .. name .. [[">]]
		.. HTML_Option("0",  "Latest Version", defaultValue == 0 )
		.. HTML_Option("61", "1.5.2", defaultValue == 1 )
		.. HTML_Option("60", "1.5.0", defaultValue == 2 )
		.. HTML_Option("49", "1.4.5", defaultValue == 3 )
		.. HTML_Option("47", "1.4.2", defaultValue == 4 )
		.. HTML_Option("39", "1.3.2", defaultValue == 5 )
		.. HTML_Option("29", "1.2.5", defaultValue == 6 )
		.. [[</select>]]
end


local function ShowGeneralSettings( Request )
	local Content = ""
	local InfoMsg = nil
	
	local SettingsIni = cIniFile("settings.ini")
	if( SettingsIni:ReadFile() == false ) then
		InfoMsg = "<b style=\"color: red;\">ERROR: Could not read settings.ini!</b>"
	end
	
	if( Request.PostParams["general_submit"] ~= nil ) then
		
		SettingsIni:SetValue("Server", "Description",Request.PostParams["Server_Description"],false )
		if( tonumber( Request.PostParams["Server_MaxPlayers"] ) ~= nil ) then
			SettingsIni:SetValue("Server", "MaxPlayers", Request.PostParams["Server_MaxPlayers"], false )
		end
		if( tonumber( Request.PostParams["Server_Port"] ) ~= nil ) then
			SettingsIni:SetValue("Server", "Port", Request.PostParams["Server_Port"], false )
		end
		if( tonumber( Request.PostParams["Server_PortsIPv6"] ) ~= nil ) then
			SettingsIni:SetValue("Server", "PortsIPv6", Request.PostParams["Server_PortsIPv6"], false )
		end
		if( tonumber( Request.PostParams["Server_Version"] ) ~= nil ) then
			SettingsIni:SetValue("Server", "PrimaryServerVersion", Request.PostParams["Server_Version"],    false )
		end
		if( tonumber( Request.PostParams["Authentication_Authenticate"] ) ~= nil ) then
			SettingsIni:SetValue("Authentication", "Authenticate", Request.PostParams["Authentication_Authenticate"], false )
		end
		if( tonumber( Request.PostParams["Limit_World"] ) ~= nil ) then
			SettingsIni:SetValue("Worlds", "LimitWorld", Request.PostParams["Limit_World"], false )
		end
		if( tonumber( Request.PostParams["LimitWorldWidth"] ) ~= nil ) then
			SettingsIni:SetValue("Worlds", "LimitWorldWidth", Request.PostParams["LimitWorldWidth"], false )
		end

		if( SettingsIni:WriteFile() == false ) then
			InfoMsg =  [[<b style="color: red;">ERROR: Could not write to settings.ini!</b>]]
		else
			InfoMsg = [[<b style="color: green;">INFO: Successfully saved changes to settings.ini</b>]]
		end
	end
	
	
	Content = Content .. [[
	<form method="POST">
	<h4>General Settings</h4>]]
	
	if( InfoMsg ~= nil ) then
		Content = Content .. "<p>" .. InfoMsg .. "</p>"
	end
	Content = Content .. [[
	<table>
	<th colspan="2">Server</th>
	<tr><td style="width: 50%;">Description:</td>
	<td><input type="text" name="Server_Description" value="]] .. SettingsIni:GetValue("Server", "Description") .. [["></td></tr>
	<tr><td>Max Players:</td>
	<td><input type="text" name="Server_MaxPlayers" value="]] .. SettingsIni:GetValue("Server", "MaxPlayers") .. [["></td></tr>
	<tr><td>Port:</td>
	<td><input type="text" name="Server_Port" value="]] .. SettingsIni:GetValue("Server", "Port") .. [["></td></tr>
	<tr><td>PortsIPv6:</td>
	<td><input type="text" name="Server_PortsIPv6" value="]] .. SettingsIni:GetValue("Server", "PortsIPv6") .. [["></td></tr>
	<tr><td>Shown Version:</td>
	<td>]] .. HTML_Select_Version("Server_Version", SettingsIni:GetValueI("Server", "PrimaryServerVersion") ) .. [[</td></tr>
	</table><br />
	
	<table>
	<th colspan="2">Authentication</th>
	<tr><td style="width: 50%;">Authenticate:</td>
	<td>]] .. HTML_Select_On_Off("Authentication_Authenticate", SettingsIni:GetValueI("Authentication", "Authenticate") ) .. [[</td></tr>
	</table><br />
	
	<table>
	<th colspan="2">LimitWorld</th>
	<tr><td style="width: 50%;">Limit World:</td>
	<td>]] .. HTML_Select_On_Off("Limit_World", SettingsIni:GetValueI("Worlds", "LimitWorld") ) .. [[</td></tr>
	<tr><td>Max Chunks from spawn:</td>
	<td><input type="text" name="LimitWorldWidth" value="]] .. SettingsIni:GetValue("Worlds", "LimitWorldWidth") .. [["></td></tr>
	</table><br />
	<input type="submit" value="Save Settings" name="general_submit"> WARNING: Any changes made here might require a server restart in order to be applied!
	</form>]]
	
	return Content
end


local function ShowMonstersSettings( Request )
	local Content = ""
	local InfoMsg = nil
	
	local SettingsIni = cIniFile("settings.ini")
	if( SettingsIni:ReadFile() == false ) then
		InfoMsg = "<b style=\"color: red;\">ERROR: Could not read settings.ini!</b>"
	end
	
	if( Request.PostParams["monsters_submit"] ~= nil ) then
		
		if( tonumber( Request.PostParams["Monsters_AnimalsOn"] ) ~= nil ) then
			SettingsIni:SetValue("Monsters", "AnimalsOn", Request.PostParams["Monsters_AnimalsOn"], false )
		end
		if( tonumber( Request.PostParams["Monsters_AnimalSpawnInterval"] ) ~= nil ) then
			SettingsIni:SetValue("Monsters", "AnimalSpawnInterval", Request.PostParams["Monsters_AnimalSpawnInterval"], false )
		end
		SettingsIni:SetValue("Monsters", "Types", Request.PostParams["Monsters_Types"], false )
		if( SettingsIni:WriteFile() == false ) then
			InfoMsg =  "<b style=\"color: red;\">ERROR: Could not write to settings.ini!</b>"
		else
			InfoMsg = "<b style=\"color: green;\">INFO: Successfully saved changes to settings.ini</b>"
		end
	end
	
	
	Content = Content .. "<form method=\"POST\">"
	
	Content = Content .. "<h4>Monsters Settings</h4>"
	if( InfoMsg ~= nil ) then
		Content = Content .. "<p>" .. InfoMsg .. "</p>"
	end
	
	Content = Content .. [[
	<table>
	<th colspan="2">Monsters</th>
	<tr><td style="width: 50%;">Animals On:</td>
	<td>]] .. HTML_Select_On_Off("Monsters_AnimalsOn", SettingsIni:GetValueI("Monsters", "AnimalsOn") ) .. [[</td></tr>
	<tr><td>Animal Spawn Interval:</td>
	<td><input type="text" name="Monsters_AnimalSpawnInterval" value="]] .. SettingsIni:GetValue("Monsters", "AnimalSpawnInterval") .. [["></td></tr>
	<tr><td>Monster Types:</td>
	<td><input type="text" name="Monsters_Types" value="]] .. SettingsIni:GetValue("Monsters", "Types") .. [["></td></tr>
	</table><br />
	<input type="submit" value="Save Settings" name="monsters_submit"> WARNING: Any changes made here might require a server restart in order to be applied!
	</form>]]
	
	return Content
end

local function ShowWorldsSettings( Request )
	local Content = ""
	local InfoMsg = nil
	local bSaveIni = false
	
	local SettingsIni = cIniFile("settings.ini")
	if( SettingsIni:ReadFile() == false ) then
		InfoMsg = [[<b style="color: red;">ERROR: Could not read settings.ini!</b>]]
	end
	
	if( Request.PostParams["RemoveWorld"] ~= nil ) then
		Content = Content .. Request.PostParams["RemoveWorld"]
		local WorldIdx = string.sub( Request.PostParams["RemoveWorld"], string.len("Remove ") )
		local KeyIdx = SettingsIni:FindKey("Worlds")
		local WorldName = SettingsIni:GetValue( KeyIdx, WorldIdx )
		if( SettingsIni:DeleteValueByID( KeyIdx, WorldIdx ) == true ) then
			InfoMsg = "<b style=\"color: green;\">INFO: Successfully removed world " .. WorldName .. "!</b><br />"
			bSaveIni = true
		end
	end
	
	if( Request.PostParams["AddWorld"] ~= nil ) then
		if( Request.PostParams["WorldName"] ~= nil and Request.PostParams["WorldName"] ~= "" ) then
			SettingsIni:SetValue("Worlds", "World", Request.PostParams["WorldName"], true )
			InfoMsg = "<b style=\"color: green;\">INFO: Successfully added world " .. Request.PostParams["WorldName"] .. "!</b><br />"
			bSaveIni = true
		end
	end
	
	if( Request.PostParams["worlds_submit"] ~= nil ) then
		SettingsIni:SetValue("Worlds", "DefaultWorld", Request.PostParams["Worlds_DefaultWorld"], false )
		if( Request.PostParams["Worlds_World"] ~= nil ) then
			SettingsIni:SetValue("Worlds", "World", Request.PostParams["Worlds_World"], true )
		end
		bSaveIni = true
	end
	
	if( bSaveIni == true ) then
		if( InfoMsg == nil ) then InfoMsg = "" end
		if( SettingsIni:WriteFile() == false ) then
			InfoMsg = InfoMsg .. "<b style=\"color: red;\">ERROR: Could not write to settings.ini!</b>"
		else
			InfoMsg = InfoMsg .. "<b style=\"color: green;\">INFO: Successfully saved changes to settings.ini</b>"
		end
	end
	
	Content = Content .. "<h4>Worlds Settings</h4>"
	if( InfoMsg ~= nil ) then
		Content = Content .. "<p>" .. InfoMsg .. "</p>"
	end
	
	Content = Content .. [[
	<form method="POST">
	<table>
	<th colspan="2">Worlds</th>
	<tr><td style="width: 50%;">Default World:</td>
	<td><input type="Submit" name="Worlds_DefaultWorld" value="]] .. SettingsIni:GetValue("Worlds", "DefaultWorld") .. [["></td></tr>]]
	
	local KeyIdx = SettingsIni:FindKey("Worlds")
	local NumValues = SettingsIni:GetNumValues( KeyIdx )
	for i = 0, NumValues-1 do
		local ValueName = SettingsIni:GetValueName(KeyIdx, i )
		if( ValueName == "World" ) then
			local WorldName = SettingsIni:GetValue(KeyIdx, i)
			Content = Content .. [[
			<tr><td>]] .. ValueName .. [[:</td><td><div style="width: 100px; display: inline-block;">]] .. WorldName .. [[</div><input type="submit" value="Remove ]] .. i .. [[" name="RemoveWorld"></td></tr>]]
		end
	end
	
	Content = Content .. [[
	<tr><td>Add World:</td>
	<td><input type='text' name='WorldName'><input type='submit' name='AddWorld' value='Add World'></td></tr>
	</table><br />
	
	<input type="submit" value="Save Settings" name="worlds_submit"> WARNING: Any changes made here might require a server restart in order to be applied!
	</form>]]
	return Content
end

local function SelectWorldButton( WorldName )
	return "<form method='POST'><input type='hidden' name='WorldName' value='"..WorldName.."'><input type='submit' name='SelectWorld' value='Select'></form>"
end

local function HTML_Select_Dimension( name, defaultValue )
	return [[<select name="]] .. name .. [[">]]
		.. HTML_Option("0", "Overworld",  defaultValue == 0 )
		.. HTML_Option("-1", "Nether", defaultValue == 1 )
		.. HTML_Option("1", "The End",  defaultValue == 2 )
		.. [[</select>]]
end

local function HTML_Select_Scheme( name, defaultValue )
	return [[<select name="]] .. name .. [[">]]
		.. HTML_Option("Default", "Default",  defaultValue == "Default" )
		.. HTML_Option("Forgetful", "Forgetful", defaultValue == "Forgetful" )
		.. HTML_Option("Compact", "Compact",  defaultValue == "Compact" )
		.. [[</select>]]
end

local function HTML_Select_GameMode( name, defaultValue )
	return [[<select name="]] .. name .. [[">]]
		.. HTML_Option("0", "Survival", defaultValue == 0 )
		.. HTML_Option("1", "Creative",  defaultValue == 1 )
		.. HTML_Option("2", "Adventure",  defaultValue == 2 )
		.. [[</select>]]
end

local function HTML_Select_Simulator( name, defaultValue )
	return [[<select name="]] .. name .. [[">]]
		.. HTML_Option("Floody", "Floody", defaultValue == 0 )
		.. HTML_Option("Noop", "Noop",  defaultValue == 1 )
		.. HTML_Option("Vaporize", "Vaporize",  defaultValue == 2 )
		.. [[</select>]]
end

local function HTML_Select_BiomeGen( name, defaultValue )
	return [[<select name="]] .. name .. [[">]]
		.. HTML_Option("MultiStepMap", "MultiStepMap", defaultValue == "MultiStepMap" )
		.. HTML_Option("DistortedVoronoi", "DistortedVoronoi",  defaultValue == "DistortedVoronoi" )
		.. HTML_Option("Voronoi", "Voronoi",  defaultValue == "Voronoi" )
		.. HTML_Option("CheckerBoard", "CheckerBoard",  defaultValue == "CheckerBoard" )
		.. HTML_Option("Constant", "Constant",  defaultValue == "Constant" )
		.. [[</select>]]
end

local function HTML_Select_HeightGen( name, defaultValue )
	return [[<select name="]] .. name .. [[">]]
		.. HTML_Option("Noise3D", "Noise3D", defaultValue == "Noise3D" )
		.. HTML_Option("Biomal", "Biomal", defaultValue == "Biomal" )
		.. HTML_Option("Classic", "Classic",  defaultValue == "Classic" )
		.. HTML_Option("Flat", "Flat",  defaultValue == "Flat" )
		.. [[</select>]]
end

local function HTML_Select_CompositionGen( name, defaultValue )
	return [[<select name="]] .. name .. [[">]]
		.. HTML_Option("Noise3D", "Noise3D", defaultValue == "Noise3D" )
		.. HTML_Option("Biomal", "Biomal", defaultValue == "Biomal" )
		.. HTML_Option("Classic", "Classic",  defaultValue == "Classic" )
		.. HTML_Option("SameBlock", "SameBlock",  defaultValue == "SameBlock" )
		.. HTML_Option("DebugBiomes", "DebugBiomes",  defaultValue == "DebugBiomes" )
		.. [[</select>]]
end

local function HTML_Select_Generator( name, defaultValue )
	return [[<select name="]] .. name .. [[">]]
		.. HTML_Option("Composable", "Composable", defaultValue == "Composable" )
		.. [[</select>]]
end

local function HTML_Select_Biome( name, defaultValue )
	return [[<select name="]] .. name .. [[">]]
		.. HTML_Option("Ocean", "Ocean", defaultValue == "Ocean" )
		.. HTML_Option("Plains", "Plains",  defaultValue == "Plains" )
		.. HTML_Option("Extreme Hills", "Extreme Hills",  defaultValue == "Extreme Hills" )
		.. HTML_Option("Forest", "Forest",  defaultValue == "Forest" )
		.. HTML_Option("Taiga", "Taiga",  defaultValue == "Taiga" )
		.. HTML_Option("Swampland", "Swampland",  defaultValue == "Swampland" )
		.. HTML_Option("River", "River",  defaultValue == "River" )
		.. HTML_Option("Hell", "Hell",  defaultValue == "Hell" )
		.. HTML_Option("Sky", "Sky",  defaultValue == "Sky" )
		.. HTML_Option("FrozenOcean", "FrozenOcean",  defaultValue == "FrozenOcean" )
		.. HTML_Option("FrozenRiver", "FrozenRiver",  defaultValue == "FrozenRiver" )
		.. HTML_Option("Ice Plains", "Ice Plains",  defaultValue == "Ice Plains" )
		.. HTML_Option("Ice Mountains", "Ice Mountains",  defaultValue == "Ice Mountains" )
		.. HTML_Option("MushroomIsland", "MushroomIsland",  defaultValue == "MushroomIsland" )
		.. HTML_Option("MushroomIslandShore", "MushroomIslandShore",  defaultValue == "MushroomIslandShore" )
		.. HTML_Option("Beach", "Beach",  defaultValue == "Beach" )
		.. HTML_Option("DesertHills", "DesertHills",  defaultValue == "DesertHills" )
		.. HTML_Option("ForestHills", "ForestHills",  defaultValue == "ForestHills" )
		.. HTML_Option("TaigaHills", "TaigaHills",  defaultValue == "TaigaHills" )
		.. HTML_Option("Extreme Hills Edge", "Extreme Hills Edge",  defaultValue == "Extreme Hills Edge" )
		.. HTML_Option("Jungle", "Jungle",  defaultValue == "Jungle" )
		.. HTML_Option("JungleHills", "JungleHills",  defaultValue == "JungleHills" )
		.. [[</select>]]
end

function ShowWorldSettings( Request )
	local Content = ""
	local InfoMsg = nil
	local SettingsIni = cIniFile("settings.ini")
	if( SettingsIni:ReadFile() == false ) then
		InfoMsg = [[<b style="color: red;">ERROR: Could not read settings.ini!</b>]]
	end
	if (Request.PostParams["SelectWorld"] ~= nil and Request.PostParams["WorldName"] ~= nil) then		-- World is selected!
		WORLD = Request.PostParams["WorldName"]
		SelectedWorld = cRoot:Get():GetWorld(WORLD)
	elseif SelectedWorld == nil then
		WORLD = SettingsIni:GetValue("Worlds", "DefaultWorld")
		SelectedWorld = cRoot:Get():GetWorld( WORLD )
	end
	local WorldIni = cIniFile(SelectedWorld:GetName() .. "/world.ini")
	WorldIni:ReadFile()
	if (Request.PostParams["world_submit"]) ~= nil then
		if( tonumber( Request.PostParams["World_Dimension"] ) ~= nil ) then
			WorldIni:DeleteValue( "General", "Dimension" )
			WorldIni:SetValue( "General", "Dimension", Request.PostParams["World_Dimension"] )
		end
		if( tonumber( Request.PostParams["World_Schema"] ) ~= nil ) then
			WorldIni:DeleteValue( "General", "Schema" )
			WorldIni:SetValue( "General", "Schema", Request.PostParams["World_Schema"] )
		end
		if( tonumber( Request.PostParams["World_SpawnX"] ) ~= nil ) then
			WorldIni:DeleteValue( "SpawnPosition", "X" )
			WorldIni:SetValue( "SpawnPosition", "X", Request.PostParams["World_SpawnX"] )
		end
		if( tonumber( Request.PostParams["World_SpawnY"] ) ~= nil ) then
			WorldIni:DeleteValue( "SpawnPosition", "Y" )
			WorldIni:SetValue( "SpawnPosition", "Y", Request.PostParams["World_SpawnY"] )
		end
		if( tonumber( Request.PostParams["World_SpawnZ"] ) ~= nil ) then
			WorldIni:DeleteValue( "SpawnPosition", "Z" )
			WorldIni:SetValue( "SpawnPosition", "Z", Request.PostParams["World_SpawnZ"] )
		end
		if( tonumber( Request.PostParams["World_Seed"] ) ~= nil ) then
			WorldIni:DeleteValue( "Seed", "Seed" )
			WorldIni:SetValue( "Seed", "Seed", Request.PostParams["World_Seed"] )
		end
		if( tonumber( Request.PostParams["World_PVP"] ) ~= nil ) then
			WorldIni:DeleteValue( "PVP", "Enabled" )
			WorldIni:SetValue( "PVP", "Enabled", Request.PostParams["World_PVP"] )
		end
		if( tonumber( Request.PostParams["World_GameMode"] ) ~= nil ) then
			WorldIni:DeleteValue( "GameMode", "GameMode" )
			WorldIni:SetValue( "GameMode", "GameMode", Request.PostParams["World_GameMode"] )
		end
		if( tonumber( Request.PostParams["World_DeepSnow"] ) ~= nil ) then
			WorldIni:DeleteValue( "Physics", "DeepSnow" )
			WorldIni:SetValue( "Physics", "DeepSnow", Request.PostParams["World_DeepSnow"] )
		end
		if( tonumber( Request.PostParams["World_SandInstantFall"] ) ~= nil ) then
			WorldIni:DeleteValue( "Physics", "SandInstantFall" )
			WorldIni:SetValue( "Physics", "SandInstantFall", Request.PostParams["World_SandInstantFall"] )
		end
		if( tonumber( Request.PostParams["World_WaterSimulator"] ) ~= nil ) then
			WorldIni:DeleteValue( "Physics", "WaterSimulator" )
			WorldIni:SetValue( "Physics", "WaterSimulator", Request.PostParams["World_WaterSimulator"] )
		end
		if( tonumber( Request.PostParams["World_LavaSimulator"] ) ~= nil ) then
			WorldIni:DeleteValue( "Physics", "LavaSimulator" )
			WorldIni:SetValue( "Physics", "LavaSimulator", Request.PostParams["World_LavaSimulator"] )
		end
		if( tonumber( Request.PostParams["World_MaxSugarcaneHeight"] ) ~= nil ) then
			WorldIni:DeleteValue( "Plants", "MaxSugarcaneHeight" )
			WorldIni:SetValue( "Plants", "MaxSugarcaneHeight", Request.PostParams["World_MaxSugarcaneHeight"] )
		end
		if( tonumber( Request.PostParams["World_MaxCactusHeight"] ) ~= nil ) then
			WorldIni:DeleteValue( "Plants", "MaxCactusHeight" )
			WorldIni:SetValue( "Plants", "MaxCactusHeight", Request.PostParams["World_MaxCactusHeight"] )
		end
		if( tonumber( Request.PostParams["World_CarrotsBonemealable"] ) ~= nil ) then
			WorldIni:DeleteValue( "Plants", "IsCarrotsBonemealable" )
			WorldIni:SetValue( "Plants", "IsCarrotsBonemealable", Request.PostParams["World_CarrotsBonemealable"] )
		end
		if( tonumber( Request.PostParams["World_CropsBonemealable"] ) ~= nil ) then
			WorldIni:DeleteValue( "Plants", "IsCropsBonemealable" )
			WorldIni:SetValue( "Plants", "IsCropsBonemealable", Request.PostParams["World_CropsBonemealable"] )
		end
		if( tonumber( Request.PostParams["World_GrassBonemealable"] ) ~= nil ) then
			WorldIni:DeleteValue( "Plants", "IsGrassBonemealable" )
			WorldIni:SetValue( "Plants", "IsGrassBonemealable", Request.PostParams["World_GrassBonemealable"] )
		end
		if( tonumber( Request.PostParams["World_SaplingBonemealable"] ) ~= nil ) then
			WorldIni:DeleteValue( "Plants", "IsSaplingBonemealable" )
			WorldIni:SetValue( "Plants", "IsSaplingBonemealable", Request.PostParams["World_SaplingBonemealable"] )
		end
		if( tonumber( Request.PostParams["World_MelonStemBonemealable"] ) ~= nil ) then
			WorldIni:DeleteValue( "Plants", "IsMelonStemBonemealable" )
			WorldIni:SetValue( "Plants", "IsMelonStemBonemealable", Request.PostParams["World_MelonStemBonemealable"] )
		end
		if( tonumber( Request.PostParams["World_MelonBonemealable"] ) ~= nil ) then
			WorldIni:DeleteValue( "Plants", "IsMelonBonemealable" )
			WorldIni:SetValue( "Plants", "IsMelonBonemealable", Request.PostParams["World_MelonBonemealable"] )
		end
		if( tonumber( Request.PostParams["World_PotatoesBonemealable"] ) ~= nil ) then
			WorldIni:DeleteValue( "Plants", "IsPotatoesBonemealable" )
			WorldIni:SetValue( "Plants", "IsPotatoesBonemealable", Request.PostParams["World_PotatoesBonemealable"] )
		end
		if( tonumber( Request.PostParams["World_PumpkinStemBonemealable"] ) ~= nil ) then
			WorldIni:DeleteValue( "Plants", "IsPumpkinStemBonemealable" )
			WorldIni:SetValue( "Plants", "IsPumpkinStemBonemealable", Request.PostParams["World_PumpkinStemBonemealable"] )
		end
		if( tonumber( Request.PostParams["World_PumpkinBonemealable"] ) ~= nil ) then
			WorldIni:DeleteValue( "Plants", "IsPumpkinBonemealable" )
			WorldIni:SetValue( "Plants", "IsPumpkinBonemealable", Request.PostParams["World_PumpkinBonemealable"] )
		end
		if( tonumber( Request.PostParams["World_SugarCaneBonemealable"] ) ~= nil ) then
			WorldIni:DeleteValue( "Plants", "IsSugarCaneBonemealable" )
			WorldIni:SetValue( "Plants", "IsSugarCaneBonemealable", Request.PostParams["World_SugarCaneBonemealable"] )
		end
		if( tonumber( Request.PostParams["World_CactusBonemealable"] ) ~= nil ) then
			WorldIni:DeleteValue( "Plants", "IsCactusBonemealable" )
			WorldIni:SetValue( "Plants", "IsCactusBonemealable", Request.PostParams["World_CactusBonemealable"] )
		end
		if( ( Request.PostParams["World_BiomeGen"] ) ~= nil ) then
			WorldIni:DeleteValue( "Generator", "BiomeGen" )
			WorldIni:SetValue( "Generator", "BiomeGen", Request.PostParams["World_BiomeGen"] )
		end
			if( ( Request.PostParams["World_Biome"] ) ~= nil ) then
				WorldIni:DeleteValue( "Generator", "ConstantBiome" )
				WorldIni:SetValue( "Generator", "ConstantBiome", Request.PostParams["World_Biome"] )
			end
			if( ( Request.PostParams["World_MultiStepMapOceanCellSize"] ) ~= nil ) then
				WorldIni:DeleteValue( "Generator", "MultiStepMapOceanCellSize" )
				WorldIni:SetValue( "Generator", "MultiStepMapOceanCellSize", Request.PostParams["World_MultiStepMapOceanCellSize"] )
			end
			if( ( Request.PostParams["World_MultiStepMapMushroomIslandSize"] ) ~= nil ) then
				WorldIni:DeleteValue( "Generator", "MultiStepMapMushroomIslandSize" )
				WorldIni:SetValue( "Generator", "MultiStepMapMushroomIslandSize", Request.PostParams["World_MultiStepMapMushroomIslandSize"] )
			end
			if( ( Request.PostParams["World_MultiStepMapRiverCellSize"] ) ~= nil ) then
				WorldIni:DeleteValue( "Generator", "MultiStepMapRiverCellSize" )
				WorldIni:SetValue( "Generator", "MultiStepMapRiverCellSize", Request.PostParams["World_MultiStepMapRiverCellSize"] )
			end
			if( ( Request.PostParams["World_MultiStepMapRiverWidth"] ) ~= nil ) then
				WorldIni:DeleteValue( "Generator", "MultiStepMapRiverWidth" )
				WorldIni:SetValue( "Generator", "MultiStepMapRiverWidth", Request.PostParams["World_MultiStepMapRiverWidth"] )
			end
			if( ( Request.PostParams["World_MultiStepMapLandBiomeSize"] ) ~= nil ) then
				WorldIni:DeleteValue( "Generator", "MultiStepMapLandBiomeSize" )
				WorldIni:SetValue( "Generator", "MultiStepMapLandBiomeSize", Request.PostParams["World_MultiStepMapLandBiomeSize"] )
			end
			if( ( Request.PostParams["World_DistortedVoronoiCellSize"] ) ~= nil ) then
				WorldIni:DeleteValue( "Generator", "DistortedVoronoiCellSize" )
				WorldIni:SetValue( "Generator", "DistortedVoronoiCellSize", Request.PostParams["World_DistortedVoronoiCellSize"] )
			end
			if( ( Request.PostParams["World_DistortedVoronoiBiomes"] ) ~= nil ) then
				WorldIni:DeleteValue( "Generator", "DistortedVoronoiBiomes" )
				WorldIni:SetValue( "Generator", "DistortedVoronoiBiomes", Request.PostParams["World_DistortedVoronoiBiomes"] )
			end
			if( ( Request.PostParams["World_VoronoiCellSize"] ) ~= nil ) then
				WorldIni:DeleteValue( "Generator", "VoronoiCellSize" )
				WorldIni:SetValue( "Generator", "VoronoiCellSize", Request.PostParams["World_VoronoiCellSize"] )
			end
			if( ( Request.PostParams["World_VoronoiBiomesdVoronoiBiomes"] ) ~= nil ) then
				WorldIni:DeleteValue( "Generator", "VoronoiBiomes" )
				WorldIni:SetValue( "Generator", "VoronoiBiomes", Request.PostParams["World_VoronoiBiomes"] )
			end
			if( ( Request.PostParams["World_CheckerBoardBiomes"] ) ~= nil ) then
				WorldIni:DeleteValue( "Generator", "CheckerBoardBiomes" )
				WorldIni:SetValue( "Generator", "CheckerBoardBiomes", Request.PostParams["World_CheckerBoardBiomes"] )
			end
			if( ( Request.PostParams["World_CheckerBoardBiomeSize"] ) ~= nil ) then
				WorldIni:DeleteValue( "Generator", "CheckerBoardBiomeSize" )
				WorldIni:SetValue( "Generator", "CheckerBoardBiomeSize", Request.PostParams["World_CheckerBoardBiomeSize"] )
			end
		if( ( Request.PostParams["World_HeightGen"] ) ~= nil ) then
			WorldIni:DeleteValue( "Generator", "HeightGen" )
			WorldIni:SetValue( "Generator", "HeightGen", Request.PostParams["World_HeightGen"] )
		end
			if( ( Request.PostParams["World_FlatHeight"] ) ~= nil ) then
				WorldIni:DeleteValue( "Generator", "FlatHeight" )
				WorldIni:SetValue( "Generator", "FlatHeight", Request.PostParams["World_FlatHeight"] )
			end
		if( ( Request.PostParams["World_CompositionGen"] ) ~= nil ) then
			WorldIni:DeleteValue( "Generator", "CompositionGen" )
			WorldIni:SetValue( "Generator", "CompositionGen", Request.PostParams["World_CompositionGen"] )
		end
			if( ( Request.PostParams["World_Noise3DSeaLevel"] ) ~= nil ) then
				WorldIni:DeleteValue( "Generator", "Noise3DSeaLevel" )
				WorldIni:SetValue( "Generator", "Noise3DSeaLevel", Request.PostParams["World_Noise3DSeaLevel"] )
			end
			if( ( Request.PostParams["World_Noise3DHeightAmplification"] ) ~= nil ) then
				WorldIni:DeleteValue( "Generator", "Noise3DHeightAmplification" )
				WorldIni:SetValue( "Generator", "Noise3DHeightAmplification", Request.PostParams["World_Noise3DHeightAmplification"] )
			end
			if( ( Request.PostParams["World_Noise3DMidPoint"] ) ~= nil ) then
				WorldIni:DeleteValue( "Generator", "Noise3DMidPoint" )
				WorldIni:SetValue( "Generator", "Noise3DMidPoint", Request.PostParams["World_Noise3DMidPoint"] )
			end
			if( ( Request.PostParams["World_Noise3DFrequencyX"] ) ~= nil ) then
				WorldIni:DeleteValue( "Generator", "Noise3DFrequencyX" )
				WorldIni:SetValue( "Generator", "Noise3DFrequencyX", Request.PostParams["World_Noise3DFrequencyX"] )
			end
			if( ( Request.PostParams["World_Noise3DFrequencyY"] ) ~= nil ) then
				WorldIni:DeleteValue( "Generator", "Noise3DFrequencyY" )
				WorldIni:SetValue( "Generator", "Noise3DFrequencyY", Request.PostParams["World_Noise3DFrequencyY"] )
			end
			if( ( Request.PostParams["World_Noise3DFrequencyZ"] ) ~= nil ) then
				WorldIni:DeleteValue( "Generator", "Noise3DFrequencyZ" )
				WorldIni:SetValue( "Generator", "Noise3DFrequencyZ", Request.PostParams["World_Noise3DFrequencyZ"] )
			end
			if( ( Request.PostParams["World_Noise3DAirThreshold"] ) ~= nil ) then
				WorldIni:DeleteValue( "Generator", "Noise3DAirThreshold" )
				WorldIni:SetValue( "Generator", "Noise3DAirThreshold", Request.PostParams["World_Noise3DAirThreshold"] )
			end
			if( ( Request.PostParams["World_ClassicSeaLevel"] ) ~= nil ) then
				WorldIni:DeleteValue( "Generator", "ClassicSeaLevel" )
				WorldIni:SetValue( "Generator", "ClassicSeaLevel", Request.PostParams["World_ClassicSeaLevel"] )
			end
			if( ( Request.PostParams["World_ClassicBeachHeight"] ) ~= nil ) then
				WorldIni:DeleteValue( "Generator", "ClassicBeachHeight" )
				WorldIni:SetValue( "Generator", "ClassicBeachHeight", Request.PostParams["World_ClassicBeachHeight"] )
			end
			if( ( Request.PostParams["World_ClassicBeachDepth"] ) ~= nil ) then
				WorldIni:DeleteValue( "Generator", "ClassicBeachDepth" )
				WorldIni:SetValue( "Generator", "ClassicBeachDepth", Request.PostParams["World_ClassicBeachDepth"] )
			end
			if( ( Request.PostParams["World_ClassicBlockTop"] ) ~= nil ) then
				WorldIni:DeleteValue( "Generator", "ClassicBlockTop" )
				WorldIni:SetValue( "Generator", "ClassicBlockTop", Request.PostParams["World_ClassicBlockTop"] )
			end
			if( ( Request.PostParams["World_ClassicBlockMiddle"] ) ~= nil ) then
				WorldIni:DeleteValue( "Generator", "ClassicBlockMiddle" )
				WorldIni:SetValue( "Generator", "ClassicBlockMiddle", Request.PostParams["World_ClassicBlockMiddle"] )
			end
			if( ( Request.PostParams["World_ClassicBlockBottom"] ) ~= nil ) then
				WorldIni:DeleteValue( "Generator", "ClassicBlockBottom" )
				WorldIni:SetValue( "Generator", "ClassicBlockBottom", Request.PostParams["World_ClassicBlockBottom"] )
			end
			if( ( Request.PostParams["World_ClassicBlockBeach"] ) ~= nil ) then
				WorldIni:DeleteValue( "Generator", "ClassicBlockBeach" )
				WorldIni:SetValue( "Generator", "ClassicBlockBeach", Request.PostParams["World_ClassicBlockBeach"] )
			end
			if( ( Request.PostParams["World_ClassicBlockBeachBottom"] ) ~= nil ) then
				WorldIni:DeleteValue( "Generator", "ClassicBlockBeachBottom" )
				WorldIni:SetValue( "Generator", "ClassicBlockBeachBottom", Request.PostParams["World_ClassicBlockBeachBottom"] )
			end
			if( ( Request.PostParams["World_ClassicBlockSea"] ) ~= nil ) then
				WorldIni:DeleteValue( "Generator", "ClassicBlockSea" )
				WorldIni:SetValue( "Generator", "ClassicBlockSea", Request.PostParams["World_ClassicBlockSea"] )
			end
			if( ( Request.PostParams["World_SameBlockType"] ) ~= nil ) then
				WorldIni:DeleteValue( "Generator", "SameBlockType" )
				WorldIni:SetValue( "Generator", "SameBlockType", Request.PostParams["World_SameBlockType"] )
			end
			if( ( Request.PostParams["World_SameBlockBedrocked"] ) ~= nil ) then
				WorldIni:DeleteValue( "Generator", "SameBlockBedrocked" )
				WorldIni:SetValue( "Generator", "SameBlockBedrocked", Request.PostParams["World_SameBlockBedrocked"] )
			end
		if( ( Request.PostParams["World_Structures"] ) ~= nil ) then
			WorldIni:DeleteValue( "Generator", "Structures" )
			WorldIni:SetValue( "Generator", "Structures", Request.PostParams["World_Structures"] )
		end
		if( ( Request.PostParams["World_Finishers"] ) ~= nil ) then
			WorldIni:DeleteValue( "Generator", "Finishers" )
			WorldIni:SetValue( "Generator", "Finishers", Request.PostParams["World_Finishers"] )
		end
		if( ( Request.PostParams["World_Generator"] ) ~= nil ) then
			WorldIni:DeleteValue( "Generator", "Generator" )
			WorldIni:SetValue( "Generator", "Generator", Request.PostParams["World_Generator"] )
		end
		if( ( Request.PostParams["World_MineShaftsGridSize"] ) ~= nil ) then
			WorldIni:DeleteValue( "Generator", "MineShaftsGridSize" )
			WorldIni:SetValue( "Generator", "MineShaftsGridSize", Request.PostParams["World_MineShaftsGridSize"] )
		end
		if( ( Request.PostParams["World_MineShaftsMaxSystemSize"] ) ~= nil ) then
			WorldIni:DeleteValue( "Generator", "MineShaftsMaxSystemSize" )
			WorldIni:SetValue( "Generator", "MineShaftsMaxSystemSize", Request.PostParams["World_MineShaftsMaxSystemSize"] )
		end
		if( ( Request.PostParams["World_MineShaftsChanceCorridor"] ) ~= nil ) then
			WorldIni:DeleteValue( "Generator", "MineShaftsChanceCorridor" )
			WorldIni:SetValue( "Generator", "MineShaftsChanceCorridor", Request.PostParams["World_MineShaftsChanceCorridor"] )
		end
		if( ( Request.PostParams["World_MineShaftsChanceCrossing"] ) ~= nil ) then
			WorldIni:DeleteValue( "Generator", "MineShaftsChanceCrossing" )
			WorldIni:SetValue( "Generator", "MineShaftsChanceCrossing", Request.PostParams["World_MineShaftsChanceCrossing"] )
		end
		if( ( Request.PostParams["World_MineShaftsChanceStaircase"] ) ~= nil ) then
			WorldIni:DeleteValue( "Generator", "MineShaftsChanceStaircase" )
			WorldIni:SetValue( "Generator", "MineShaftsChanceStaircase", Request.PostParams["World_MineShaftsChanceStaircase"] )
		end
		if( ( Request.PostParams["World_LavaLakesProbability"] ) ~= nil ) then
			WorldIni:DeleteValue( "Generator", "LavaLakesProbability" )
			WorldIni:SetValue( "Generator", "LavaLakesProbability", Request.PostParams["World_LavaLakesProbability"] )
		end
		if( ( Request.PostParams["World_WaterLakesProbability"] ) ~= nil ) then
			WorldIni:DeleteValue( "Generator", "WaterLakesProbability" )
			WorldIni:SetValue( "Generator", "WaterLakesProbability", Request.PostParams["World_WaterLakesProbability"] )
		end
		if( ( Request.PostParams["World_BottomLavaLevel"] ) ~= nil ) then
			WorldIni:DeleteValue( "Generator", "BottomLavaLevel" )
			WorldIni:SetValue( "Generator", "BottomLavaLevel", Request.PostParams["World_BottomLavaLevel"] )
		end
		
		WorldIni:WriteFile()
	end
	Content = Content .. "<h4>World for operations: " .. WORLD .. "</h4>"
	Content = Content .. "<table>"
	local WorldNum = 0
	local AddWorldToTable = function(World)
		WorldNum = WorldNum + 1
		Content = Content .. "<tr>"
		Content = Content .. "<td style='width: 10px;'>" .. WorldNum .. ".</td>"
		Content = Content .. "<td>" .. World:GetName() .. "</td>"
		Content = Content .. "<td>" .. SelectWorldButton(World:GetName()) .. "</td>"
		Content = Content .. "</tr>"
	end
	cRoot:Get():ForEachWorld(AddWorldToTable)
	Content = Content .. "</table>"
	
	
	Content = Content .. [[<table>
	<form method="POST">
	<br />
	<th colspan="2">General</th>
	<tr><td>Dimension:</td>
	<td>]] .. HTML_Select_Dimension("World_Dimension", WorldIni:GetValueI("General", "Dimension") ) .. [[</td></tr>
	</table>
	<br />
	<table>
	<th colspan="2">Storage</th>
	<tr><td>Schema:</td>
	<td>]] .. HTML_Select_Scheme("World_Schema", WorldIni:GetValueI("Storage", "Schema") ) .. [[</td></tr>
	</table>
	<br />
	<table>
	<th colspan="2">Spawn Position</th>
	<tr><td>X:</td>
	<td><input type="text" name="World_SpawnX" value="]] .. WorldIni:GetValue("SpawnPosition", "X") .. [["></td></tr>
	<tr><td>Y:</td>
	<td><input type="text" name="World_SpawnY" value="]] .. WorldIni:GetValue("SpawnPosition", "Y") .. [["></td></tr>
	<tr><td>Z:</td>
	<td><input type="text" name="World_SpawnZ" value="]] .. WorldIni:GetValue("SpawnPosition", "Z") .. [["></td></tr>
	</table>
	<br />
	<table>
	<th colspan="2">Seed</th>
	<tr><td>Seed:</td>
	<td><input type="text" name="World_Seed" value="]] .. WorldIni:GetValue("Seed", "Seed") .. [["></td></tr>
	</table>
	<br />
	<table>
	<th colspan="2">PVP</th>
	<tr><td style="width: 50%;">PVP:</td>
	<td>]] .. HTML_Select_On_Off("World_PVP", WorldIni:GetValueI("PVP", "Enabled") ) .. [[</td></tr>
	</table>
	<br />
	<table>
	<th colspan="2">GameMode</th>
	<tr><td style="width: 50%;">GameMode:</td>
	<td>]] .. HTML_Select_GameMode("World_GameMode", WorldIni:GetValueI("GameMode", "GameMode") ) .. [[</td></tr>
	</table>
	<br />
	<table>
	<th colspan="2">Physics</th>
	<tr><td style="width: 50%;">DeepSnow:</td>
	<td>]] .. HTML_Select_On_Off("World_DeepSnow", WorldIni:GetValueI("Physics", "DeepSnow") ) .. [[</td></tr>
	<tr><td style="width: 50%;">SandInstantFall:</td>
	<td>]] .. HTML_Select_On_Off("World_SandInstantFall", WorldIni:GetValueI("Physics", "SandInstantFall") ) .. [[</td></tr>
	<tr><td style="width: 50%;">WaterSimulator:</td>
	<td>]] .. HTML_Select_Simulator("World_WaterSimulator", WorldIni:GetValue("Physics", "WaterSimulator") ) .. [[</td></tr>
	<tr><td style="width: 50%;">LavaSimulator:</td>
	<td>]] .. HTML_Select_Simulator("World_LavaSimulator", WorldIni:GetValue("Physics", "LavaSimulator") ) .. [[</td></tr>
	</table>
	<br />
	<table>
	<th colspan="2">Plants</th>
	<tr><td>MaxCactusHeight:</td>
	<td><input type="text" name="World_MaxCactusHeight" value="]] .. WorldIni:GetValue("Plants", "MaxCactusHeight") .. [["></td></tr>
	<tr><td>MaxSugarcaneHeigh:</td>
	<td><input type="text" name="World_MaxSugarcaneHeight" value="]] .. WorldIni:GetValue("Plants", "MaxSugarcaneHeight") .. [["></td></tr>
	<tr><td style="width: 50%;">CarrotsBonemealable:</td>
	<td>]] .. HTML_Select_On_Off("World_CarrotsBonemealable", WorldIni:GetValueI("Plants", "IsCarrotsBonemealable") ) .. [[</td></tr>
	<tr><td style="width: 50%;">CropsBonemealable:</td>
	<td>]] .. HTML_Select_On_Off("World_CropsBonemealable", WorldIni:GetValueI("Plants", "IsCropsBonemealable") ) .. [[</td></tr>
	<tr><td style="width: 50%;">GrassBonemealabl:</td>
	<td>]] .. HTML_Select_On_Off("World_GrassBonemealable", WorldIni:GetValueI("Plants", "IsGrassBonemealable") ) .. [[</td></tr>
	<tr><td style="width: 50%;">SaplingBonemealable:</td>
	<td>]] .. HTML_Select_On_Off("World_SaplingBonemealable", WorldIni:GetValueI("Plants", "IsSaplingBonemealable") ) .. [[</td></tr>
	<tr><td style="width: 50%;">MelonStemBonemealable:</td>
	<td>]] .. HTML_Select_On_Off("World_MelonStemBonemealable", WorldIni:GetValueI("Plants", "IsMelonStemBonemealable") ) .. [[</td></tr>
	<tr><td style="width: 50%;">MelonBonemealable:</td>
	<td>]] .. HTML_Select_On_Off("World_MelonBonemealable", WorldIni:GetValueI("Plants", "IsMelonBonemealable") ) .. [[</td></tr>
	<tr><td style="width: 50%;">PotatoesBonemealable:</td>
	<td>]] .. HTML_Select_On_Off("World_PotatoesBonemealable", WorldIni:GetValueI("Plants", "IsPotatoesBonemealable") ) .. [[</td></tr>
	<tr><td style="width: 50%;">PumpkinStemBonemealable:</td>
	<td>]] .. HTML_Select_On_Off("World_PumpkinStemBonemealable", WorldIni:GetValueI("Plants", "IsPumpkinStemBonemealable") ) .. [[</td></tr>
	<tr><td style="width: 50%;">PumpkinBonemealable:</td>
	<td>]] .. HTML_Select_On_Off("World_PumpkinBonemealable", WorldIni:GetValueI("Plants", "IsPumpkinBonemealable") ) .. [[</td></tr>
	<tr><td style="width: 50%;">SugarcaneBonemealabl:</td>
	<td>]] .. HTML_Select_On_Off("World_SugarcaneBonemealable", WorldIni:GetValueI("Plants", "IsSugarcaneBonemealable") ) .. [[</td></tr>
	<tr><td style="width: 50%;">CactusBonemealable:</td>
	<td>]] .. HTML_Select_On_Off("World_CactusBonemealable", WorldIni:GetValueI("Plants", "IsCactusBonemealable") ) .. [[</td></tr>
	</table>
	<br />
	<table>
	<th colspan="2">Generator</th>
	<tr><td style="width: 50%;">BiomeGen:</td>
	<td>]] .. HTML_Select_BiomeGen("World_BiomeGen", WorldIni:GetValue("Generator", "BiomeGen") ) .. [[</td></tr>
	<tr><td style="width: 50%;">HeightGen:</td>
	<td>]] .. HTML_Select_HeightGen("World_HeightGen", WorldIni:GetValue("Generator", "HeightGen") ) .. [[</td></tr>
	<tr><td style="width: 50%;">CompositionGen:</td>
	<td>]] .. HTML_Select_CompositionGen("World_CompositionGen", WorldIni:GetValue("Generator", "CompositionGen") ) .. [[</td></tr>
	<tr><td>Structures:</td>
	<td><input type="text" size="50" name="World_Structures" value="]] .. WorldIni:GetValue("Generator", "Structures") .. [["></td></tr>
	<tr><td>Finishers:</td>
	<td><input type="text" size="50" name="World_Finishers" value="]] .. WorldIni:GetValue("Generator", "Finishers") .. [["></td></tr>
	<tr><td style="width: 50%;">Generator:</td>
	<td>]] .. HTML_Select_Generator("World_Generator", WorldIni:GetValue("Generator", "Generator") ) .. [[</td></tr>

	</table>
	<br />
	<table>
	<th colspan="1">Finetuning</th><br />
	</table>
	<table>
	]]	
	if WorldIni:GetValue( "Generator", "BiomeGen" ) ==  "Constant" then
		Content = Content .. [[
		<th colspan="2">Biome Generator</th>
		<tr><td style="width: 50%;">ConstantBiome:</td>
		<td>]] .. HTML_Select_Biome( "World_Biome", WorldIni:GetValue("Generator", "ConstantBiome" ) ) .. [[</td></tr>]]
	elseif WorldIni:GetValue( "Generator", "BiomeGen" ) == "MultiStepMap" then
		Content = Content .. [[
		<th colspan="2">Biome Generator</th>
		<tr><td>MultiStepMapOceanCellSize:</td>
		<td><input type="text" name="World_MultiStepMapOceanCellSize" value="]] .. WorldIni:GetValue("Generator", "MultiStepMapOceanCellSize") .. [["></td></tr>
		<tr><td>MultiStepMapOceanCellSize:</td>
		<td><input type="text" name="World_MultiStepMapMushroomIslandSize" value="]] .. WorldIni:GetValue("Generator", "MultiStepMapMushroomIslandSize") .. [["></td></tr>
		<tr><td>MultiStepMapOceanCellSize:</td>
		<td><input type="text" name="World_MultiStepMapRiverCellSize" value="]] .. WorldIni:GetValue("Generator", "MultiStepMapRiverCellSize") .. [["></td></tr>
		<tr><td>MultiStepMapOceanCellSize:</td>
		<td><input type="text" name="World_MultiStepMapRiverWidth" value="]] .. WorldIni:GetValue("Generator", "MultiStepMapRiverWidth") .. [["></td></tr>
		<tr><td>MultiStepMapOceanCellSize:</td>
		<td><input type="text" name="World_MultiStepMapLandBiomeSize" value="]] .. WorldIni:GetValue("Generator", "MultiStepMapLandBiomeSize") .. [["></td></tr>]]
	elseif WorldIni:GetValue( "Generator", "BiomeGen" ) == "DistortedVoronoi" then
		Content = Content .. [[
		<th colspan="2">Biome Generator</th>
		<tr><td>DistortedVoronoiCellSize:</td>
		<td><input type="text" name="World_DistortedVoronoiCellSize" value="]] .. WorldIni:GetValue("Generator", "DistortedVoronoiCellSize") .. [["></td></tr>
		<tr><td>DistortedVoronoiBiomes:</td>
		<td><input type="text" name="World_DistortedVoronoiBiomes" value="]] .. WorldIni:GetValue("Generator", "DistortedVoronoiBiomes") .. [["></td></tr>]]
	elseif WorldIni:GetValue( "Generator", "BiomeGen" ) == "Voronoi" then
		Content = Content .. [[
		<th colspan="2">Biome Generator</th>
		<tr><td>VoronoiCellSize:</td>
		<td><input type="text" name="World_VoronoiCellSize" value="]] .. WorldIni:GetValue("Generator", "VoronoiCellSize") .. [["></td></tr>
		<tr><td>VoronoiBiomes:</td>
		<td><input type="text" name="World_VoronoiBiomes" value="]] .. WorldIni:GetValue("Generator", "VoronoiBiomes") .. [["></td></tr>]]
	elseif WorldIni:GetValue( "Generator", "BiomeGen" ) == "CheckerBoard" then
		Content = Content .. [[
		<th colspan="2">Biome Generator</th>
		<tr><td>CheckerBoardBiomes:</td>
		<td><input type="text" name="World_CheckerBoardBiomes" value="]] .. WorldIni:GetValue("Generator", "CheckerBoardBiomes") .. [["></td></tr>
		<tr><td>CheckerBoardBiomeSize:</td>
		<td><input type="text" name="World_CheckerBoardBiomeSize" value="]] .. WorldIni:GetValue("Generator", "CheckerBoardBiomeSize") .. [["></td></tr>]]
	end
	
	if WorldIni:GetValue( "Generator", "CompositionGen" ) == "Noise3D" then
		Content = Content .. [[
		<th colspan="2">Composition Generator</th>
		<tr><td>Noise3DSeaLevel:</td>
		<td><input type="text" name="World_Noise3DSeaLevel" value="]] .. WorldIni:GetValue("Generator", "Noise3DSeaLevel") .. [["></td></tr>
		<tr><td>Noise3DHeightAmplification:</td>
		<td><input type="text" name="World_Noise3DHeightAmplification" value="]] .. WorldIni:GetValue("Generator", "Noise3DHeightAmplification") .. [["></td></tr>
		<tr><td>Noise3DMidPoint:</td>
		<td><input type="text" name="World_Noise3DMidPoint" value="]] .. WorldIni:GetValue("Generator", "Noise3DMidPoint") .. [["></td></tr>
		<tr><td>Noise3DFrequencyX:</td>
		<td><input type="text" name="World_Noise3DFrequencyX" value="]] .. WorldIni:GetValue("Generator", "Noise3DFrequencyX") .. [["></td></tr>
		<tr><td>Noise3DFrequencyY:</td>
		<td><input type="text" name="World_Noise3DFrequencyY" value="]] .. WorldIni:GetValue("Generator", "Noise3DFrequencyY") .. [["></td></tr>
		<tr><td>Noise3DFrequencyZ:</td>
		<td><input type="text" name="World_Noise3DFrequencyZ" value="]] .. WorldIni:GetValue("Generator", "Noise3DFrequencyZ") .. [["></td></tr>
		<tr><td>Noise3DAirThreshold:</td>
		<td><input type="text" name="World_Noise3DAirThreshold" value="]] .. WorldIni:GetValue("Generator", "Noise3DAirThreshold") .. [["></td></tr>]]
	elseif WorldIni:GetValue( "Generator", "CompositionGen" ) == "Classic" then
		Content = Content .. [[
		<th colspan="2">Composition Generator</th>
		<tr><td>ClassicSeaLevel:</td>
		<td><input type="text" name="World_ClassicSeaLevel" value="]] .. WorldIni:GetValue("Generator", "ClassicSeaLevel") .. [["></td></tr>
		<tr><td>ClassicBeachHeight:</td>
		<td><input type="text" name="World_ClassicBeachHeight" value="]] .. WorldIni:GetValue("Generator", "ClassicBeachHeight") .. [["></td></tr>
		<tr><td>ClassicBeachDepth:</td>
		<td><input type="text" name="World_ClassicBeachDepth" value="]] .. WorldIni:GetValue("Generator", "ClassicBeachDepth") .. [["></td></tr>
		<tr><td>ClassicBlockTop:</td>
		<td><input type="text" name="World_ClassicBlockTop" value="]] .. WorldIni:GetValue("Generator", "ClassicBlockTop") .. [["></td></tr>
		<tr><td>ClassicBlockMiddle:</td>
		<td><input type="text" name="World_ClassicBlockMiddle" value="]] .. WorldIni:GetValue("Generator", "ClassicBlockMiddle") .. [["></td></tr>
		<tr><td>ClassicBlockBottom:</td>
		<td><input type="text" name="World_ClassicBlockBottom" value="]] .. WorldIni:GetValue("Generator", "ClassicBlockBottom") .. [["></td></tr>
		<tr><td>ClassicBlockBeach:</td>
		<td><input type="text" name="World_ClassicBlockBeach" value="]] .. WorldIni:GetValue("Generator", "ClassicBlockBeach") .. [["></td></tr>
		<tr><td>ClassicBlockBeachBottom:</td>
		<td><input type="text" name="World_ClassicBlockBeachBottom" value="]] .. WorldIni:GetValue("Generator", "ClassicBlockBeachBottom") .. [["></td></tr>
		<tr><td>ClassicBlockSea:</td>
		<td><input type="text" name="World_ClassicBlockSea" value="]] .. WorldIni:GetValue("Generator", "ClassicBlockSea") .. [["></td></tr>]]
	elseif WorldIni:GetValue( "Generator", "CompositionGen" ) == "SameBlock" then
		Content = Content .. [[
		<th colspan="2">Composition Generator</th>
		<tr><td>SameBlockType:</td>
		<td><input type="text" name="World_SameBlockType" value="]] .. WorldIni:GetValue("Generator", "SameBlockType") .. [["></td></tr>
		<tr><td>SameBlockBedrocked:</td>
		<td><input type="text" name="World_SameBlockBedrocked" value="]] .. WorldIni:GetValue("Generator", "SameBlockBedrocked") .. [["></td></tr>]]		
	end
	if WorldIni:GetValue( "Generator", "HeightGen" ) == "Flat" then
		Content = Content .. [[
		<th colspan="2">Height Generator</th>
		<tr><td>FlatHeight:</td>
		<td><input type="text" name="World_FlatHeight" value="]] .. WorldIni:GetValue("Generator", "FlatHeight") .. [["></td></tr>]]
	end
	if string.find( WorldIni:GetValue( "Generator", "Structures" ), "MineShafts" ) ~= nil then
		Content = Content .. [[
		<th colspan="2">MineShafts</th>
		<tr><td>MineShaftsGridSize:</td>
		<td><input type="text" name="World_MineShaftsGridSize" value="]] .. WorldIni:GetValue("Generator", "MineShaftsGridSize") .. [["></td></tr>
		<tr><td>MineShaftsMaxSystemSize:</td>
		<td><input type="text" name="World_MineShaftsMaxSystemSize" value="]] .. WorldIni:GetValue("Generator", "MineShaftsMaxSystemSize") .. [["></td></tr>
		<tr><td>MineShaftsChanceCorridor:</td>
		<td><input type="text" name="World_MineShaftsChanceCorridor" value="]] .. WorldIni:GetValue("Generator", "MineShaftsChanceCorridor") .. [["></td></tr>
		<tr><td>MineShaftsChanceCrossing:</td>
		<td><input type="text" name="World_MineShaftsChanceCrossing" value="]] .. WorldIni:GetValue("Generator", "MineShaftsChanceCrossing") .. [["></td></tr>
		<tr><td>MineShaftsChanceStaircase:</td>
		<td><input type="text" name="World_MineShaftsChanceStaircase" value="]] .. WorldIni:GetValue("Generator", "MineShaftsChanceStaircase") .. [["></td></tr>]]
	end
	if string.find( WorldIni:GetValue( "Generator", "Structures" ), "LavaLakes" ) ~= nil then
		Content = Content .. [[
		<th colspan="2">LavaLakes</th>
		<tr><td>LavaLakesProbability:</td>
		<td><input type="text" name="World_LavaLakesProbability" value="]] .. WorldIni:GetValue("Generator", "LavaLakesProbability") .. [["></td></tr>]]
	end
	if string.find( WorldIni:GetValue( "Generator", "Structures" ), "WaterLakes" ) ~= nil then
			Content = Content .. [[
		<th colspan="2">WaterLakes</th>
		<tr><td>WaterLakesProbability:</td>
		<td><input type="text" name="World_WaterLakesProbability" value="]] .. WorldIni:GetValue("Generator", "WaterLakesProbability") .. [["></td></tr>]]
	end
	if string.find( WorldIni:GetValue( "Generator", "Finishers" ), "BottomLava" ) ~= nil then
		Content = Content .. [[
		<th colspan="2">BottomLavaLevel</th>
		<tr><td>BottomLavaLevel:</td>
		<td><input type="text" name="World_BottomLavaLevel" value="]] .. WorldIni:GetValue("Generator", "BottomLavaLevel") .. [["></td></tr>]]
	end
	Content = Content .. [[</table>]]
	
	Content = Content .. [[ <br />
	<input type="submit" value="Save Settings" name="world_submit"> </form>WARNING: Any changes made here might require a server restart in order to be applied!
	</form>]]
	return Content
end



function HandleRequest_ServerSettings( Request )
	local Content = ""

	Content = Content .. [[
	<p><b>Server Settings</b></p>
	<table>
	<tr>
	<td><a href="?tab=General">General</a></td>
	<td><a href="?tab=Monsters">Monsters</a></td>
	<td><a href="?tab=Worlds">Worlds</a></td>
	<td><a href="?tab=World">World</a></td>
	</tr>
	</table>
	<br />]]
	
	if( Request.Params["tab"] == "Monsters" ) then
		Content = Content .. ShowMonstersSettings( Request )
	elseif( Request.Params["tab"] == "Worlds" ) then
		Content = Content .. ShowWorldsSettings( Request )
	elseif( Request.Params["tab"] == "World" ) then
		Content = Content .. ShowWorldSettings( Request )
	else
		Content = Content .. ShowGeneralSettings( Request ) -- Default to general settings
	end
	
	return Content
end