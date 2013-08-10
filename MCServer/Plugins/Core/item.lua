function HandleItemCommand(Split, Player)

	if ((#Split ~= 2) and (#Split ~=3)) then
		Player:SendMessage(cChatColor.Yellow .. "[INFO] " .. cChatColor.White .. "Usage: /i <item>[:meta] [amount]" )
		return true
	end

	local itemSplit = StringSplit(Split[2], ":")
	local newSplit = {}

	newSplit[1] = "/give"
	newSplit[2] = Player:GetName()
	newSplit[3] = itemSplit[1]
	if Split[3] ~= nil then
		newSplit[4] = Split[3]
	else
		newSplit[4] = 1
	end
	if itemSplit[2] ~= nil then
		newSplit[5] = itemSplit[2]
	end

	HandleGiveCommand(newSplit, Player)
	return true
	
end