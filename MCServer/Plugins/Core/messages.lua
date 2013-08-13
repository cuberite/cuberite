-- Use prefixes or not.
-- If set to true, messages are prefixed, e. g. "[FATAL]". If false, messages are colored.
g_UsePrefixes = true

function SendMessage(a_Player, a_Message)
	if (g_UsePrefixes) then
		a_Player:SendMessage(cChatColor.Yellow .. "[INFO] " .. cChatColor.White .. a_Message)
	else
		a_Player:SendMessage(cChatColor.Yellow .. a_Message)
	end
end

function SendMessageSuccess(a_Player, a_Message)
	if (g_UsePrefixes) then
		a_Player:SendMessage(cChatColor.Green .. "[INFO] " .. cChatColor.White .. a_Message)
	else
		a_Player:SendMessage(cChatColor.Green .. a_Message)
	end
end

function SendMessageFailure(a_Player, a_Message)
	if (g_UsePrefixes) then
		a_Player:SendMessage(cChatColor.Red .. "[INFO] " .. cChatColor.White .. a_Message)
	else
		a_Player:SendMessage(cChatColor.Red .. a_Message)
	end
end
