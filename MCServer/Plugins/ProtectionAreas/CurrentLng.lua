
-- CurrentLng.lua
-- This file provides all the translatable strings
-- The expectation is that the translators will create copies of this file, translate the texts and then the users will overwrite this file with a specific language version
-- Note that the individual languages must not have ".lua" extension, otherwise MCServer will load them and the plugin won't work!




-- Individual commands, and their help strings. Don't touch the first symbol on each line!

function CommandReg()
	return {
		-- Handler function  | Command          | Permission    | Help text
		{HandleAddArea,       "/ProtAdd",        "Prot.Add",     "Adds a new protected area"},
		{HandleAddAreaCoords, "/ProtAddCoords",  "Prot.Add",     "Adds a new protected area by coords"},
		{HandleAddAreaUser,   "/ProtAddUser",    "Prot.AddUser", "Adds a new user to an existing protected area"},
		{HandleDelArea,       "/ProtDelID",      "Prot.Del",     "Deletes a protected area by ID"},
		{HandleGiveWand,      "/ProtWand",       "Prot.Wand",    "Gives you the wand used for protection"},
		{HandleListAreas,     "/ProtList",       "Prot.List",    "Lists all areas for the marked block"},
		{HandleRemoveUser,    "/ProtRemUser",    "Prot.RemUser", "Removes a user from the protected area"},
		{HandleRemoveUserAll, "/ProtRemUserAll", "Prot.RemUser", "Removes a user from all protected areas"},
	};
end;



