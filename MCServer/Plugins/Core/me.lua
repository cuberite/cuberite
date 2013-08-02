function HandleMeCommand( Split, Player )
  table.remove(Split, 1);
	local Message = "";
	for i, Text in ipairs(Split) do
		Message = Message .. " " .. Text;
	end
	if  (Split[1] == nil) then
		Player:SendMessage(cChatColor.Yellow .. "[INFO] " .. cChatColor.White .. "Usage: /me <action>")
		return true
	end
	if (Split[1] ~= nil) then
		cRoot:Get():GetServer():BroadcastChat(Player:GetName() .. "" .. Message);
		return true
	end
end
