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
	for line in io.lines("motd.txt") do
		local TempMessage = line
		for I=1, string.len(TempMessage) do
			if string.sub(TempMessage, I, I) == "@" then
				local Color, Char = ReturnColor(TempMessage, string.sub( TempMessage, I + 1, I + 1 ))
				if (Color ~= "") then
					TempMessage = string.gsub(TempMessage, "@" .. Char, Color)
				end
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
	if char == "0" then
		return cChatColor.Black, 0
	elseif char == "1" then
		return cChatColor.Navy, 1
	elseif char == "2" then
		return cChatColor.Green, 2
	elseif char == "3" then
		return cChatColor.Blue, 3
	elseif char == "4" then
		return cChatColor.Red, 4
	elseif char == "5" then
		return cChatColor.Purple, 5
	elseif char == "6" then
		return cChatColor.Gold, 6
	elseif char == "7" then
		return cChatColor.LightGray, 7
	elseif char == "8" then
		return cChatColor.Gray, 8
	elseif char == "9" then
		return cChatColor.DarkPurple, 9
	elseif char == "a" then
		return cChatColor.LightGreen, "a"
	elseif char == "b" then
		return cChatColor.LightBlue, "b"
	elseif char == "c" then
		return cChatColor.Rose, "c"
	elseif char == "d" then
		return cChatColor.LightPurple, "d"
	elseif char == "e" then
		return cChatColor.Yellow, "e"
	elseif char == "f" then
		return cChatColor.White, "f"
	else
		return ""
	end
end