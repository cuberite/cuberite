Core = cPluginManager:Get():GetPlugin("Core")

function SendMessage(a_Player, a_Message)
	if (Core ~= nil) then
		Core:Call("SendMessage", a_Player, a_Message)
	end
end

function SendMessageSuccess(a_Player, a_Message)
	if (Core ~= nil) then
		Core:Call("SendMessageSuccess", a_Player, a_Message)
	end
end

function SendMessageFailure(a_Player, a_Message)
	if (Core ~= nil) then
		Core:Call("SendMessageFailure", a_Player, a_Message)
	end
end
