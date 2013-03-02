function HandleSaveAllCommand( Split, Player )
	cRoot:Get():SaveAllChunks();
	Player:SendMessage(cChatColor.Green .. "All the worlds are saved")
	return true;
end