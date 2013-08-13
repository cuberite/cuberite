function HandleMOTDCommand( Split, Player )
	ShowMOTDTo( Player )
	return true
end

function LoadMotd()

	local File = io.open( "motd.txt", "r" )

	-- Check if the file 'motd.txt' exists, else create it.
	if not File then
		CreateFile = io.open( "motd.txt", "w" )
		CreateFile:write("@6Welcome to the MCServer test server!\n@6http://www.mc-server.org/\n@6Type /help for all commands")
		CreateFile:close()
	else
		File:close()
	end

	for line in io.lines( "motd.txt" ) do
		local TempMessage = line
		-- Do a for loop that goes to each char in the line.
		for I=1, string.len( TempMessage ) do
			-- If the char is a '@' then check if the next char represents a color.
			if string.sub( TempMessage, I, I ) == "@" then
				local Char = string.sub( TempMessage, I + 1, I + 1 )
				local Color = ReturnColorFromChar( TempMessage, Char )
				-- If the next char represented a color then put the color in the string.
				if Color ~= nil then
					TempMessage = string.gsub( TempMessage, "@" .. Char, Color )
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

