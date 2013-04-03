function HandleListGroupsCommand( Split, Player )
	local GroupsIni = cIniFile("groups.ini")
	if GroupsIni:ReadFile() == false then
		Player:SendMessage( cChatColor.Green .. "No groups found" )
	end
	Number = GroupsIni:NumKeys()
	Groups = {}
	for i=0, Number do
		table.insert( Groups, GroupsIni:KeyName(i) )
	end
	Player:SendMessage( cChatColor.Green .. "Groups:" )
	Player:SendMessage( cChatColor.Green .. table.concat( Groups, ", " ) )
	return true
end