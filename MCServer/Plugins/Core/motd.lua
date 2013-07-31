function HandleMOTDCommand( Split, Player )
	ShowMOTDTo( Player )
	return true
end

function LoadMotd()
	local File = io.open("motd.txt", "r")
	-- Check if the file 'motd.txt' exists, else create it.
	if not File then
		CreateFile = io.open("motd.txt", "w")
		CreateFile:write("@6Welcome to the MCServer test server!\n@6http://www.mc-server.org/\n@6Type /help for all commands")
		CreateFile:close()
	else
		File:close()
	end
	for line in io.lines("motd.txt") do
		local TempMessage = line
		-- Do a for loop that goes to each char in the line.
		for I=1, string.len(TempMessage) do
			-- If the char is a '@' then check if the next char represents a color.
			if string.sub(TempMessage, I, I) == "@" then
				local Char = string.sub(TempMessage, I + 1, I + 1)
				local Color = ReturnColorFromChar(TempMessage, Char)
				-- If the next char represented a color then put the color in the string.
				if (Color ~= nil) then
					TempMessage = string.gsub(TempMessage, "@" .. Char, Color)
				end
			end
		end
		-- Add the message to the list of messages.
		Messages[#Messages + 1] = TempMessage
	end
end

function ShowMOTDTo( Player )
	for I=1, #Messages do
		Player:SendMessage(Messages[I])
	end
end

function ReturnColorFromChar( Split, char )
	-- Check if the char represents a color. Else return nil.
	if char == "0" then
		return cChatColor.Black
	elseif char == "1" then
		return cChatColor.Navy
	elseif char == "2" then
		return cChatColor.Green
	elseif char == "3" then
		return cChatColor.Blue
	elseif char == "4" then
		return cChatColor.Red
	elseif char == "5" then
		return cChatColor.Purple
	elseif char == "6" then
		return cChatColor.Gold
	elseif char == "7" then
		return cChatColor.LightGray
	elseif char == "8" then
		return cChatColor.Gray
	elseif char == "9" then
		return cChatColor.DarkPurple
	elseif char == "a" then
		return cChatColor.LightGreen
	elseif char == "b" then
		return cChatColor.LightBlue
	elseif char == "c" then
		return cChatColor.Rose
	elseif char == "d" then
		return cChatColor.LightPurple
	elseif char == "e" then
		return cChatColor.Yellow
	elseif char == "f" then
		return cChatColor.White
	elseif char == "k" then
		return cChatColor.Random
	elseif char == "l" then
		return cChatColor.Bold
	elseif char == "m" then
		return cChatColor.Strikethrough
	elseif char == "n" then
		return cChatColor.Underlined
	elseif char == "o" then
		return cChatColor.Italic
	elseif char == "r" then
		return cChatColor.Plain
	end
end