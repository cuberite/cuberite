function HandleMOTDCommand( Split, Player )
	ShowMOTDTo( Player )
	return true
end

function LoadMotd()
	local File = io.open("motd.txt", "r")
	if not File then
		CreateFile = io.open("motd.txt", "w")
		CreateFile:write("@6Welcome to the MCServer test server!\n@6http://www.mc-server.org/\n@6Type /help for all commands")
		CreateFile:close()
	else
		File:close()
	end
	local Lines = 0
	for _ in io.lines'motd.txt' do
	  Lines = Lines + 1
	end
	for line in io.lines("motd.txt") do
		Split = StringSplit( line, " " )
		local TempMessage = ""
		for i=1, #Split do
			if string.find( Split[i], "@" ) ~= nil then
				local TempWord = ""
				for I=1, string.len(Split[i]) do	
					if string.sub( Split[i], I, I ) == "@" then
						local Color, Char = ReturnColor( string.sub( Split[i], I + 1, I + 1 ), 0 )
						TempWord = TempWord .. Color
						StopNextChar = true
					else
						if StopNextChar ~= true then
							TempWord = TempWord .. string.sub( Split[i], I, I )							
						end
						StopNextChar = false
					end
				end
				TempMessage = TempMessage .. TempWord .. " "
			else
				TempMessage = TempMessage .. Split[i] .. " "
			end
		end
		Messages[#Messages + 1] = TempMessage
	end
end

function ShowMOTDTo( Player )
	for I=1, #Messages do
		Player:SendMessage(Messages[I])
	end
end

function ReturnColor( Split, char )
	if string.sub( Split, char + 1, char + 1 ) == "0" then
		return cChatColor.Black, 0
	elseif string.sub( Split, char + 1, char + 1 ) == "1" then
		return cChatColor.Navy, 1
	elseif string.sub( Split, char + 1, char + 1 ) == "2" then
		return cChatColor.Green, 2
	elseif string.sub( Split, char + 1, char + 1 ) == "3" then
		return cChatColor.Blue, 3
	elseif string.sub( Split, char + 1, char + 1 ) == "4" then
		return cChatColor.Red, 4
	elseif string.sub( Split, char + 1, char + 1 ) == "5" then
		return cChatColor.Purple, 5
	elseif string.sub( Split, char + 1, char + 1 ) == "6" then
		return cChatColor.Gold, 6
	elseif string.sub( Split, char + 1, char + 1 ) == "7" then
		return cChatColor.LightGray, 7
	elseif string.sub( Split, char + 1, char + 1 ) == "8" then
		return cChatColor.Gray, 8
	elseif string.sub( Split, char + 1, char + 1 ) == "9" then
		return cChatColor.DarkPurple, 9
	elseif string.sub( Split, char + 1, char + 1 ) == "a" then
		return cChatColor.LightGreen, "a"
	elseif string.sub( Split, char + 1, char + 1 ) == "b" then
		return cChatColor.LightBlue, "b"
	elseif string.sub( Split, char + 1, char + 1 ) == "c" then
		return cChatColor.Rose, "c"
	elseif string.sub( Split, char + 1, char + 1 ) == "d" then
		return cChatColor.LightPurple, "d"
	elseif string.sub( Split, char + 1, char + 1 ) == "e" then
		return cChatColor.Yellow, "e"
	elseif string.sub( Split, char + 1, char + 1 ) == "f" then
		return cChatColor.White, "f"
	else
		return ""
	end
end