-- Describes the cRankManager class API

return
{
	cRankManager =
	{
		Desc = [[
			Manages the players' permissions. The players are assigned a single rank, which contains groups of
			permissions and restrictions. The functions in this class query or modify these.</p>
			<p>
			All the functions are static, call them using the <code>cRankManager:Function()</code> convention.</p>
			<p>
			The players are identified by their UUID, to support player renaming.</p>
			<p>
			The rank also contains specific "mesage visuals" - bits that are used for formatting messages from the
			players. There's a message prefix, which is put in front of every message the player sends, and the
			message suffix that is appended to each message. There's also a PlayerNameColorCode, which holds the
			color that is used for the player's name in the messages.</p>
			<p>
			Each rank can contain any number of permission groups. These groups allow for an easier setup of the
			permissions - you can share groups among ranks, so the usual approach is to group similar permissions
			together and add that group to any rank that should use those permissions.</p>
			<p>
			Permissions are added to individual groups. Each group can support unlimited permissions. Note that
			adding a permission to a group will make the permission available to all the ranks that contain that
			permission group.</p>
			<p>
			One rank is reserved as the Default rank. All players that don't have an explicit rank assigned to them
			will behave as if assigned to this rank. The default rank can be changed to any other rank at any time.
			Note that the default rank cannot be removed from the RankManager - RemoveRank() will change the default
			rank to the replacement rank, if specified, and fail if no replacement rank is specified. Renaming the
			default rank using RenameRank() will change the default rank to the new name.
		]],
		Functions =
		{
			AddGroup =
			{
				IsStatic = true,
				Params =
				{
					{
						Name = "GroupName",
						Type = "string",
					},
				},
				Notes = "Adds the group of the specified name. Logs a warning and does nothing if the group already exists.",
			},
			AddGroupToRank =
			{
				IsStatic = true,
				Params =
				{
					{
						Name = "GroupName",
						Type = "string",
					},
					{
						Name = "RankName",
						Type = "string",
					},
				},
				Returns =
				{
					{
						Type = "boolean",
					},
				},
				Notes = "Adds the specified group to the specified rank. Returns true on success, false on failure - if the group name or the rank name is not found.",
			},
			AddPermissionToGroup =
			{
				IsStatic = true,
				Params =
				{
					{
						Name = "Permission",
						Type = "string",
					},
					{
						Name = "GroupName",
						Type = "string",
					},
				},
				Returns =
				{
					{
						Type = "boolean",
					},
				},
				Notes = "Adds the specified permission to the specified group. Returns true on success, false on failure - if the group name is not found.",
			},
			AddRank =
			{
				IsStatic = true,
				Params =
				{
					{
						Name = "RankName",
						Type = "string",
					},
					{
						Name = "MsgPrefix",
						Type = "string",
					},
					{
						Name = "MsgSuffix",
						Type = "string",
					},
					{
						Name = "MsgNameColorCode",
						Type = "string",
					},
				},
				Notes = "Adds a new rank of the specified name and with the specified message visuals. Logs an info message and does nothing if the rank already exists.",
			},
			AddRestrictionToGroup =
			{
				IsStatic = true,
				Params =
				{
					{
						Name = "Permission",
						Type = "string",
					},
					{
						Name = "GroupName",
						Type = "string",
					},
				},
				Returns =
				{
					{ Type = "boolean" },
				},
				Notes = "Adds a new restriction to the specified group. Returns true if successful, false if it fails (no such group). No action if the group already has the restriction.",
			},
			ClearPlayerRanks =
			{
				IsStatic = true,
				Notes = "Removes all player ranks from the database. Note that this doesn't change the cPlayer instances for the already connected players, you need to update all the instances manually.",
			},
			GetAllGroups =
			{
				IsStatic = true,
				Returns =
				{
					{
						Type = "table",
					},
				},
				Notes = "Returns an array-table containing the names of all the groups that are known to the manager.",
			},
			GetAllPermissions =
			{
				IsStatic = true,
				Returns =
				{
					{
						Type = "table",
					},
				},
				Notes = "Returns an array-table containing all the permissions that are known to the manager.",
			},
			GetAllPermissionsRestrictions =
			{
				IsStatic = true,
				Returns =
				{
					{
						Type = "table",
					},
				},
				Notes = "Returns an array-table containing all the permissions and restrictions (intermixed together) that are known to the manager.",
			},
			GetAllPlayerUUIDs =
			{
				IsStatic = true,
				Returns =
				{
					{
						Type = "table",
					},
				},
				Notes = "Returns the short uuids of all players stored in the rank DB, sorted by the players' names (case insensitive).",
			},
			GetAllRanks =
			{
				IsStatic = true,
				Returns =
				{
					{
						Type = "table",
					},
				},
				Notes = "Returns an array-table containing the names of all the ranks that are known to the manager.",
			},
			GetAllRestrictions =
			{
				IsStatic = true,
				Returns =
				{
					{
						Type = "table",
					},
				},
				Notes = "Returns an array-table containing all the restrictions that are known to the manager.",
			},
			GetDefaultRank =
			{
				IsStatic = true,
				Returns =
				{
					{
						Type = "string",
					},
				},
				Notes = "Returns the name of the default rank. ",
			},
			GetGroupPermissions =
			{
				IsStatic = true,
				Params =
				{
					{
						Name = "GroupName",
						Type = "string",
					},
				},
				Returns =
				{
					{
						Type = "table",
					},
				},
				Notes = "Returns an array-table containing the permissions that the specified group contains.",
			},
			GetGroupRestrictions =
			{
				IsStatic = true,
				Params =
				{
					{
						Name = "GroupName",
						Type = "string",
					},
				},
				Returns =
				{
					{
						Type = "table",
					},
				},
				Notes = "Returns an array-table containing the restrictions that the specified group contains.",
			},
			GetPlayerGroups =
			{
				IsStatic = true,
				Params =
				{
					{
						Name = "PlayerUUID",
						Type = "cUUID",
					},
				},
				Returns =
				{
					{
						Type = "table",
					},
				},
				Notes = "Returns an array-table of the names of the groups that are assigned to the specified player through their rank. Returns an empty table if the player is not known or has no rank or groups assigned to them.",
			},
			GetPlayerMsgVisuals =
			{
				IsStatic = true,
				Params =
				{
					{
						Name = "PlayerUUID",
						Type = "cUUID",
					},
				},
				Returns =
				{
					{
						Name = "MsgPrefix",
						Type = "string",
					},
					{
						Name = "MsgSuffix",
						Type = "string",
					},
					{
						Name = "MsgNameColorCode",
						Type = "string",
					},
				},
				Notes = "Returns the message visuals assigned to the player. If the player is not explicitly assigned a rank, the default rank's visuals are returned. If there is an error, no value is returned at all.",
			},
			GetPlayerName =
			{
				IsStatic = true,
				Params =
				{
					{
						Name = "PlayerUUID",
						Type = "cUUID",
					},
				},
				Returns =
				{
					{
						Name = "PlayerName",
						Type = "string",
					},
				},
				Notes = "Returns the last name that the specified player has, for a player in the ranks database. An empty string is returned if the player isn't in the database.",
			},
			GetPlayerPermissions =
			{
				IsStatic = true,
				Params =
				{
					{
						Name = "PlayerUUID",
						Type = "cUUID",
					},
				},
				Returns =
				{
					{
						Type = "table",
					},
				},
				Notes = "Returns an array-table containing all permissions that the specified player is assigned through their rank. Returns the default rank's permissions if the player has no explicit rank assigned to them. Returns an empty array on error.",
			},
			GetPlayerRankName =
			{
				IsStatic = true,
				Params =
				{
					{
						Name = "PlayerUUID",
						Type = "cUUID",
					},
				},
				Returns =
				{
					{
						Name = "RankName",
						Type = "string",
					},
				},
				Notes = "Returns the name of the rank that is assigned to the specified player. An empty string (NOT the default rank) is returned if the player has no rank assigned to them.",
			},
			GetRankGroups =
			{
				IsStatic = true,
				Params =
				{
					{
						Name = "RankName",
						Type = "string",
					},
				},
				Returns =
				{
					{
						Type = "table",
					},
				},
				Notes = "Returns an array-table of the names of all the groups that are assigned to the specified rank. Returns an empty table if there is no such rank.",
			},
			GetRankPermissions =
			{
				IsStatic = true,
				Params =
				{
					{
						Name = "RankName",
						Type = "string",
					},
				},
				Returns =
				{
					{
						Type = "table",
					},
				},
				Notes = "Returns an array-table of all the permissions that are assigned to the specified rank through its groups. Returns an empty table if there is no such rank.",
			},
			GetRankRestrictions =
			{
				IsStatic = true,
				Params =
				{
					{
						Name = "RankName",
						Type = "string",
					},
				},
				Returns =
				{
					{
						Type = "table",
					},
				},
				Notes = "Returns an array-table of all the restrictions that are assigned to the specified rank through its groups. Returns an empty table if there is no such rank.",
			},
			GetRankVisuals =
			{
				IsStatic = true,
				Params =
				{
					{
						Name = "RankName",
						Type = "string",
					},
				},
				Returns =
				{
					{
						Name = "MsgPrefix",
						Type = "string",
					},
					{
						Name = "MsgSuffix",
						Type = "string",
					},
					{
						Name = "MsgNameColorCode",
						Type = "string",
					},
				},
				Notes = "Returns the message visuals for the specified rank. Returns no value if the specified rank does not exist.",
			},
			GroupExists =
			{
				IsStatic = true,
				Params =
				{
					{
						Name = "GroupName",
						Type = "string",
					},
				},
				Returns =
				{
					{
						Type = "boolean",
					},
				},
				Notes = "Returns true iff the specified group exists.",
			},
			IsGroupInRank =
			{
				IsStatic = true,
				Params =
				{
					{
						Name = "GroupName",
						Type = "string",
					},
					{
						Name = "RankName",
						Type = "string",
					},
				},
				Returns =
				{
					{
						Type = "boolean",
					},
				},
				Notes = "Returns true iff the specified group is assigned to the specified rank.",
			},
			IsPermissionInGroup =
			{
				IsStatic = true,
				Params =
				{
					{
						Name = "Permission",
						Type = "string",
					},
					{
						Name = "GroupName",
						Type = "string",
					},
				},
				Returns =
				{
					{
						Type = "boolean",
					},
				},
				Notes = "Returns true iff the specified permission is assigned to the specified group.",
			},
			IsPlayerRankSet =
			{
				IsStatic = true,
				Params =
				{
					{
						Name = "PlayerUUID",
						Type = "cUUID",
					},
				},
				Returns =
				{
					{
						Type = "boolean",
					},
				},
				Notes = "Returns true iff the specified player has a rank assigned to them.",
			},
			IsRestrictionInGroup =
			{
				IsStatic = true,
				Params =
				{
					{
						Name = "Restriction",
						Type = "string",
					},
					{
						Name = "GroupName",
						Type = "string",
					},
				},
				Returns =
				{
					{
						Type = "boolean",
					},
				},
				Notes = "Returns true iff the specified restriction is assigned to the specified group.",
			},
			RankExists =
			{
				IsStatic = true,
				Params =
				{
					{
						Name = "RankName",
						Type = "string",
					},
				},
				Returns =
				{
					{
						Type = "boolean",
					},
				},
				Notes = "Returns true iff the specified rank exists.",
			},
			RemoveGroup =
			{
				IsStatic = true,
				Params =
				{
					{
						Name = "GroupName",
						Type = "string",
					},
				},
				Notes = "Removes the specified group completely. The group will be removed from all the ranks using it and then erased from the manager. Logs an info message and does nothing if the group doesn't exist.",
			},
			RemoveGroupFromRank =
			{
				IsStatic = true,
				Params =
				{
					{
						Name = "GroupName",
						Type = "string",
					},
					{
						Name = "RankName",
						Type = "string",
					},
				},
				Notes = "Removes the specified group from the specified rank. The group will still exist, even if it isn't assigned to any rank. Logs an info message and does nothing if the group or rank doesn't exist.",
			},
			RemovePermissionFromGroup =
			{
				IsStatic = true,
				Params =
				{
					{
						Name = "Permission",
						Type = "string",
					},
					{
						Name = "GroupName",
						Type = "string",
					},
				},
				Notes = "Removes the specified permission from the specified group. Logs an info message and does nothing if the group doesn't exist.",
			},
			RemovePlayerRank =
			{
				IsStatic = true,
				Params =
				{
					{
						Name = "PlayerUUID",
						Type = "cUUID",
					},
				},
				Notes = "Removes the player's rank; the player's left without a rank. Note that this doesn't change the {{cPlayer}} instances for the already connected players, you need to update all the instances manually. No action if the player has no rank assigned to them already.",
			},
			RemoveRank =
			{
				IsStatic = true,
				Params =
				{
					{
						Name = "RankName",
						Type = "string",
					},
					{
						Name = "ReplacementRankName",
						Type = "string",
						IsOptional = true,
					},
				},
				Notes = "Removes the specified rank. If ReplacementRankName is given, the players that have RankName will get their rank set to ReplacementRankName. If it isn't given, or is an invalid rank, the players will be removed from the manager, their ranks will be unset completely. Logs an info message and does nothing if the rank is not found.",
			},
			RemoveRestrictionFromGroup =
			{
				IsStatic = true,
				Params =
				{
					{
						Name = "Restriction",
						Type = "string",
					},
					{
						Name = "GroupName",
						Type = "string",
					},
				},
				Notes = "Removes the specified restriction from the specified group.",
			},
			RenameGroup =
			{
				IsStatic = true,
				Params =
				{
					{
						Name = "OldName",
						Type = "string",
					},
					{
						Name = "NewName",
						Type = "string",
					},
				},
				Notes = "Renames the specified group. Logs an info message and does nothing if the group is not found or the new name is already used.",
			},
			RenameRank =
			{
				IsStatic = true,
				Params =
				{
					{
						Name = "OldName",
						Type = "string",
					},
					{
						Name = "NewName",
						Type = "string",
					},
				},
				Notes = "Renames the specified rank. Logs an info message and does nothing if the rank is not found or the new name is already used.",
			},
			SetDefaultRank =
			{
				IsStatic = true,
				Params =
				{
					{
						Name = "RankName",
						Type = "string",
					},
				},
				Returns =
				{
					{
						Type = "boolean",
					},
				},
				Notes = "Sets the specified rank as the default rank. Returns true on success, false on failure (rank doesn't exist).",
			},
			SetPlayerRank =
			{
				IsStatic = true,
				Params =
				{
					{
						Name = "PlayerUUID",
						Type = "cUUID",
					},
					{
						Name = "PlayerName",
						Type = "string",
					},
					{
						Name = "RankName",
						Type = "string",
					},
				},
				Notes = "Updates the rank for the specified player. The player name is provided for reference, the UUID is used for identification. Logs a warning and does nothing if the rank is not found.",
			},
			SetRankVisuals =
			{
				IsStatic = true,
				Params =
				{
					{
						Name = "RankName",
						Type = "string",
					},
					{
						Name = "MsgPrefix",
						Type = "string",
					},
					{
						Name = "MsgSuffix",
						Type = "string",
					},
					{
						Name = "MsgNameColorCode",
						Type = "string",
					},
				},
				Notes = "Updates the rank's message visuals. Logs an info message and does nothing if rank not found.",
			},
		},
	},
}
