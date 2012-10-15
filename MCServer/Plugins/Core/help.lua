function HandleHelpCommand( Split, Player )
	local PluginManager = cRoot:Get():GetPluginManager()
	
	local LinesPerPage = 9
	local CurrentPage = 1
	local CurrentLine = 0
	
	if( #Split == 2 ) then
		CurrentPage = tonumber(Split[2])
	end
	
	local Pages = {}

	local PluginList = PluginManager:GetAllPlugins()
	for k, Plugin in pairs(PluginList) do
		if( Plugin ) then
			local Commands = Plugin:GetCommands()
			for i, v in ipairs( Commands ) do
				if( Player:HasPermission( v.Permission ) ) then
					local PageNum = math.floor( CurrentLine/LinesPerPage )+1
					if( Pages[ PageNum ] == nil ) then Pages[ PageNum ] = {} end -- Create page
					
					if( Pages[ PageNum ].ShownName ~= Plugin:GetName() and SHOW_PLUGIN_NAMES == true ) then
						if( CurrentLine == LinesPerPage * PageNum -1 ) then	-- Don't add if it's the last line of the page, it looks silly
							-- Add it to the next page instead
							CurrentLine = CurrentLine+1
							PageNum = math.floor( CurrentLine/LinesPerPage )+1
							
							if( Pages[ PageNum ] == nil ) then Pages[ PageNum ] = {} end -- Create page
							table.insert( Pages[ PageNum ], cChatColor.Gold .. Plugin:GetName() )
						else
							Pages[ PageNum ].ShownName = Plugin:GetName()
							table.insert( Pages[ PageNum ], cChatColor.Gold .. Plugin:GetName() )
						end
						CurrentLine = CurrentLine+1
						PageNum = math.floor( CurrentLine/LinesPerPage )+1
						if( Pages[ PageNum ] == nil ) then Pages[ PageNum ] = {} end -- Create page
					end
					local Message = cChatColor.Blue .. v.Command .. v.Description;
					table.insert( Pages[ PageNum ], Message )
					CurrentLine = CurrentLine+1
				end
			end
		end
	end
	
	Player:SendMessage( cChatColor.Purple .. "- All commands - " .. cChatColor.Gold .. "[Page " .. (CurrentPage) .."/"..#Pages.."]" )
	
	if( Pages[CurrentPage] ~= nil ) then
		for i, v in ipairs(Pages[CurrentPage]) do
			Player:SendMessage( v )
		end
	end
	
	return true
end