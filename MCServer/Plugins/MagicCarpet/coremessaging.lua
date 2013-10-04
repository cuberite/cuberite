IniFile = cIniFile( "settings.ini" )
IniFile:ReadFile()
UsePrefixes = IniFile:GetValueSet( "Messaging", "Prefixes", "true" )
IniFile:WriteFile()

function SendMessage(a_Player, a_Message)
	if (UsePrefixes) then
		a_Player:SendMessage(cChatColor.Yellow .. "[INFO] " .. cChatColor.White .. a_Message)
	else
		a_Player:SendMessage(cChatColor.Yellow .. a_Message)
	end
end

function SendMessageSuccess(a_Player, a_Message)
	if (UsePrefixes) then
		a_Player:SendMessage(cChatColor.Green .. "[INFO] " .. cChatColor.White .. a_Message)
	else
		a_Player:SendMessage(cChatColor.Green .. a_Message)
	end
end

function SendMessageFailure(a_Player, a_Message)
	if (UsePrefixes) then
		a_Player:SendMessage(cChatColor.Red .. "[INFO] " .. cChatColor.White .. a_Message)
	else
		a_Player:SendMessage(cChatColor.Red .. a_Message)
	end
end