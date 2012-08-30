local function Buttons_Player( Name )
	return "<form method='POST'><input type='hidden' name='PlayerName' value='"..Name.."'><input type='submit' name='PlayerExact' value='Exact'><input type='submit' name='Player3x3' value='3x3'></form>"
end

local function Button_World( Name )
	return "<form method='POST'><input type='hidden' name='WorldName' value='"..Name.."'><input type='submit' name='SelectWorld' value='Select'></form>"
end

function HandleRequest_Generation( Request )
	local Content = ""
	
	if ( Request.PostParams["FormSetWorld"] ) then
		WORK_WORLD = Request.PostParams["FormWorldName"]
		WW_instance = cRoot:Get():GetWorld(WORK_WORLD)
	end
	
	if( Request.PostParams["SelectWorld"] ~= nil
	and Request.PostParams["WorldName"] ~= nil ) then		-- World is selected!
		WORK_WORLD = Request.PostParams["WorldName"]
		WW_instance = cRoot:Get():GetWorld(WORK_WORLD)
	end
	
	if(Request.PostParams["OperationGenerate"] ~= nil) then
		OPERATION_CODE = 0
	end
	if(Request.PostParams["OperationReGenerate"] ~= nil) then
		OPERATION_CODE = 1
	end
	
	if (GENERATION_STATE == 0) then
		if( Request.PostParams["FormAreaStartX"] ~= nil
		and Request.PostParams["FormAreaStartZ"] ~= nil
		and Request.PostParams["FormAreaEndX"] ~= nil
		and Request.PostParams["FormAreaEndZ"] ~= nil ) then	--(Re)Generation valid!
			-- COMMON (Re)gen
			if( Request.PostParams["StartArea"]) then
				AreaStartX = tonumber(Request.PostParams["FormAreaStartX"])
				AreaStartZ = tonumber(Request.PostParams["FormAreaStartZ"])
				AreaEndX = tonumber(Request.PostParams["FormAreaEndX"])
				AreaEndZ = tonumber(Request.PostParams["FormAreaEndZ"])
				
				PLUGIN.IniFile:DeleteValue("Area data", "StartX")
				PLUGIN.IniFile:DeleteValue("Area data", "StartZ")
				PLUGIN.IniFile:DeleteValue("Area data", "EndX")
				PLUGIN.IniFile:DeleteValue("Area data", "EndZ")
				PLUGIN.IniFile:SetValueI("Area data", "StartX", AreaStartX)
				PLUGIN.IniFile:SetValueI("Area data", "StartZ", AreaStartZ)
				PLUGIN.IniFile:SetValueI("Area data", "EndX", AreaEndX)
				PLUGIN.IniFile:SetValueI("Area data", "EndZ", AreaEndZ)
				if (OPERATION_CODE == 0) then
					GENERATION_STATE = 1
				elseif (OPERATION_CODE == 1) then
					GENERATION_STATE = 3
				end
				PLUGIN.IniFile:WriteFile()
			end
		end
		if( Request.PostParams["FormRadialX"] ~= nil
		and Request.PostParams["FormRadialZ"] ~= nil
		and Request.PostParams["FormRadius"] ~= nil ) then	--(Re)Generation valid!
			-- COMMON (Re)gen
			if( Request.PostParams["StartRadial"]) then
				RadialX =  tonumber(Request.PostParams["FormRadialX"])
				RadialZ =  tonumber(Request.PostParams["FormRadialZ"])
				Radius =  tonumber(Request.PostParams["FormRadius"])
				AreaStartX = RadialX - Radius
				AreaStartZ = RadialZ - Radius
				AreaEndX = RadialX + Radius
				AreaEndZ = RadialZ + Radius
				
				PLUGIN.IniFile:DeleteValue("Radial data", "RadialX")
				PLUGIN.IniFile:DeleteValue("Radial data", "RadialZ")
				PLUGIN.IniFile:DeleteValue("Radial data", "Radius")
				PLUGIN.IniFile:SetValueI("Radial data", "RadialX", RadialX)
				PLUGIN.IniFile:SetValueI("Radial data", "RadialZ", RadialZ)
				PLUGIN.IniFile:SetValueI("Radial data", "Radius", Radius)
				if (OPERATION_CODE == 0) then
					GENERATION_STATE = 1
				elseif (OPERATION_CODE == 1) then
					GENERATION_STATE = 3
				end
				PLUGIN.IniFile:WriteFile()
			end
		end
		-- POINT REGEN!
		if( Request.PostParams["FormPointX"] ~= nil
		and Request.PostParams["FormPointZ"] ~= nil ) then	--ReGeneration valid!
			-- EXACT
			if ( Request.PostParams["PointExact"] ~= nil) then
				AreaStartX = tonumber(Request.PostParams["FormPointX"])
				AreaStartZ = tonumber(Request.PostParams["FormPointZ"])
				AreaEndX = AreaStartX
				AreaEndZ = AreaStartZ
				GENERATION_STATE = 3
			end
			-- 3x3
			if ( Request.PostParams["Point3x3"] ~= nil) then
				AreaStartX = tonumber(Request.PostParams["FormPointX"]) - 1
				AreaStartZ = tonumber(Request.PostParams["FormPointZ"]) - 1
				AreaEndX = AreaStartX + 2
				AreaEndZ = AreaStartZ + 2
				GENERATION_STATE = 3
			end
		end
		
		local GetAreaByPlayer = function(Player)
			-- Player is valid only within this function, it cannot be stord and used later!
			AreaStartX = Player:GetChunkX()
			AreaStartZ = Player:GetChunkZ()
		end
		-- PLAYERS REGEN!
		if( Request.PostParams["PlayerExact"] ~= nil
		and Request.PostParams["PlayerName"] ~= nil ) then		-- Making BOOM! I meant, regenereate...
			cRoot:Get():GetWorld(WORK_WORLD):DoWithPlayer(Request.PostParams["PlayerName"],GetAreaByPlayer)
			AreaEndX = AreaStartX
			AreaEndZ = AreaStartZ
			GENERATION_STATE = 3
		end
		if( Request.PostParams["Player3x3"] ~= nil
		and Request.PostParams["PlayerName"] ~= nil ) then		-- Making BOOM! I meant, regenereate...
			cRoot:Get():GetWorld(WORK_WORLD):DoWithPlayer(Request.PostParams["PlayerName"],GetAreaByPlayer)
			AreaStartX = AreaStartX - 1
			AreaStartZ = AreaStartZ - 1
			AreaEndX = AreaStartX + 2
			AreaEndZ = AreaStartZ + 2
			GENERATION_STATE = 3
		end
	end
	
	--Content = Content .. "<h4>World for operations: " .. WORK_WORLD .. "</h4>"
	--Content = Content .. "<form method='POST'>"
	--Content = Content .. "<input type='text' name='FormWorldName' value='Input world name here'><input type='submit' name='FormSetWorld' value='Set world'>"
	--Content = Content .. "</form>"
	
	-- SELECTING WORK_WORLD
	Content = Content .. "<h4>World for operations: " .. WORK_WORLD .. "</h4>"
	Content = Content .. "<table>"
	local WorldNum = 0
	local AddWorldToTable = function(World)
		WorldNum = WorldNum + 1
		Content = Content .. "<tr>"
		Content = Content .. "<td style='width: 10px;'>" .. WorldNum .. ".</td>"
		Content = Content .. "<td>" .. World:GetName() .. "</td>"
		Content = Content .. "<td>" .. Button_World(World:GetName()) .. "</td>"
		Content = Content .. "</tr>"
	end
	cRoot:Get():ForEachWorld(AddWorldToTable)
	if( WorldNum == 0 ) then
		Content = Content .. "<tr><td>No worlds! O_O</td></tr>"
	end
	Content = Content .. "</table>"
	Content = Content .. "<br>"
	
	-- SELECTING OPERATION
	if (OPERATION_CODE == 0) then
		Content = Content .. "<h4>Operation: Generation</h4>"
	elseif (OPERATION_CODE == 1) then
		Content = Content .. "<h4>Operation: Regeneration</h4>"
	end
	Content = Content .. "<form method='POST'>"
	Content = Content .. "<input type='submit' name='OperationGenerate' value='Generation'>"
	Content = Content .. "<input type='submit' name='OperationReGenerate' value='Regeneration'>"
	Content = Content .. "</form>"
	
	-- SELECTING AREA
	Content = Content .. "<h4>Area:   </h4>Start X, Start Z;    End X, End Z"
	Content = Content .. "<form method='POST'>"
	Content = Content .. "<input type='text' name='FormAreaStartX' value='" .. AreaStartX .. "'><input type='text' name='FormAreaStartZ' value='" .. AreaStartZ .. "'>"
	Content = Content .. "<input type='text' name='FormAreaEndX' value='" .. AreaEndX .. "'><input type='text' name='FormAreaEndZ' value='" .. AreaEndZ .. "'>"
	Content = Content .. "<input type='submit' name='StartArea' value='Start'>"
	Content = Content .. "</form>"
	
	-- SELECTING RADIAL
	Content = Content .. "<h4>Radial:   </h4>Center X, Center Z, Raduis (0 to any)"
	Content = Content .. "<form method='POST'>"
	Content = Content .. "<input type='text' name='FormRadialX' value='" .. RadialX .. "'><input type='text' name='FormRadialZ' value='" .. RadialZ .. "'><input type='text' name='FormRadius' value='" .. Radius .. "'>"
	Content = Content .. "<input type='submit' name='StartRadial' value='Start'>"
	Content = Content .. "</form>"
	Content = Content .. "<br>"
	Content = Content .. "<br>"
	Content = Content .. "<br>"
	
	-- SELECTING POINT
	Content = Content .. "<h4>Point regeneration:</h4> X, Z"
	Content = Content .. "<form method='POST'>"
	Content = Content .. "<input type='text' name='FormPointX' value='0'><input type='text' name='FormPointZ' value='0'>"
	Content = Content .. "<input type='submit' name='PointExact' value='Exact'>"
	Content = Content .. "<input type='submit' name='Point3x3' value='3x3'>"
	Content = Content .. "</form>"
	
	-- SELECTING PLAYERS
	Content = Content .. "<h4>Player-based regeneration:</h4>"
	Content = Content .. "<table>"
	local PlayerNum = 0
	local AddPlayerToTable = function( Player )
		PlayerNum = PlayerNum + 1
		Content = Content .. "<tr>"
		Content = Content .. "<td style='width: 10px;'>" .. PlayerNum .. ".</td>"
		Content = Content .. "<td>" .. Player:GetName() .. "</td>"
		Content = Content .. "<td>" .. Buttons_Player(Player:GetName()) .. "</td>"
		Content = Content .. "</tr>"
	end
	if (cRoot:Get():GetWorld(WORK_WORLD) == nil) then
		Content = Content .. "<tr><td>Incorrect world selection</td></tr>"
	else
		cRoot:Get():GetWorld(WORK_WORLD):ForEachPlayer( AddPlayerToTable )
		if( PlayerNum == 0 ) then
			Content = Content .. "<tr><td>No connected players</td></tr>"
		end
	end
	Content = Content .. "</table>"
	Content = Content .. "<br>"
	
	
	return Content
end