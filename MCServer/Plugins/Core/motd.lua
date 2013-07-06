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
				local Color, Char = ReturnColor(TempMessage, I + 1)
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
	if string.sub( Split, char, char ) == "0" then
		return cChatColor.Black, 0
	elseif string.sub( Split, char, char ) == "1" then
		return cChatColor.Navy, 1
	elseif string.sub( Split, char, char ) == "2" then
		return cChatColor.Green, 2
	elseif string.sub( Split, char, char ) == "3" then
		return cChatColor.Blue, 3
	elseif string.sub( Split, char, char ) == "4" then
		return cChatColor.Red, 4
	elseif string.sub( Split, char, char ) == "5" then
		return cChatColor.Purple, 5
	elseif string.sub( Split, char, char ) == "6" then
		return cChatColor.Gold, 6
	elseif string.sub( Split, char, char ) == "7" then
		return cChatColor.LightGray, 7
	elseif string.sub( Split, char, char ) == "8" then
		return cChatColor.Gray, 8
	elseif string.sub( Split, char, char ) == "9" then
		return cChatColor.DarkPurple, 9
	elseif string.sub( Split, char, char ) == "a" then
		return cChatColor.LightGreen, "a"
	elseif string.sub( Split, char, char ) == "b" then
		return cChatColor.LightBlue, "b"
	elseif string.sub( Split, char, char ) == "c" then
		return cChatColor.Rose, "c"
	elseif string.sub( Split, char, char ) == "d" then
		return cChatColor.LightPurple, "d"
	elseif string.sub( Split, char, char ) == "e" then
		return cChatColor.Yellow, "e"
	elseif string.sub( Split, char, char ) == "f" then
		return cChatColor.White, "f"
	else
		return ""
	end
end