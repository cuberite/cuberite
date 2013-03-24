local function ShowUsersTable()
	local Content = "<h4>Users</h4>"

	local NumUsers = UsersIni:GetNumKeys()
	
	Content = Content .. "<table>"
	
	if( NumUsers > 0 ) then
		Content = Content .. "<tr><th></th><th>User</th><th>Groups</th></tr>"
	
		for i=0, NumUsers-1 do
			local UserName = UsersIni:GetKeyName( i )
		
			Content = Content .. "<tr>"
			Content = Content .. "<td style='width: 10px;'>" .. i .. ".</td>"
			Content = Content .. "<td>" .. UserName .. "</td>"
			Content = Content .. "<td>"
			Content = Content .. UsersIni:GetValue( UserName, "Groups", "-" )
			Content = Content .. "</td>"
			Content = Content .. "</tr>"
		end
	else
		Content = Content .. "<tr><td>None</td></tr>"
	end
	Content = Content .. "</table>"
	
	
	return Content
end

local function ShowGroupsTable()
	local Content = "<h4>Groups</h4>"
	
	local NumGroups = GroupsIni:GetNumKeys()
	
	Content = Content .. "<table>"
	if( NumGroups > 0 ) then
		Content = Content .. "<tr><th></th><th>Name</th><th>Permissions</th><th>Color</th></tr>"
		
		for i=0, NumGroups-1 do
			local GroupName = GroupsIni:GetKeyName( i )
		
			Content = Content .. "<tr>"
			Content = Content .. "<td style='width: 10px;'>" .. i .. ".</td>"
			Content = Content .. "<td>" .. GroupName .. "</td>"
			Content = Content .. "<td>"
			Content = Content .. GroupsIni:GetValue( GroupName, "Permissions", "-" )
			Content = Content .. "</td>"
			Content = Content .. "<td>"
			Content = Content .. GroupsIni:GetValue( GroupName, "Color", "-" )
			Content = Content .. "</td>"
			Content = Content .. "</tr>"
		end
	else
		Content = Content .. "<tr><td>None</td></tr>"
	end
	Content = Content .. "</table>"
	
	return Content
end

local function AddPlayers( Request )
	local Content = "<h4>Add Players</h4>"
	if( Request.PostParams["AddPlayerToGroup"] ~= nil ) then
		if Request.PostParams["AddPlayer"] ~= "" then
			if Request.PostParams["AddGroups"] ~= "" then
				if GroupsIni:FindKey(Request.PostParams["AddGroup"]) == -1 then
					return "Group does not exist"
				end
				UsersIni:DeleteKey(Request.PostParams["AddPlayer"])
				UsersIni:GetValueSet(Request.PostParams["AddPlayer"], "Groups", Request.PostParams["AddGroup"])
				UsersIni:WriteFile()
				local loopPlayers = function( Player )
					if Player:GetName() == Request.PostParams["AddPlayer"] then
						Player:SendMessage( cChatColor.Green .. "You were moved to group " .. Request.PostParams["AddGroup"] )
						Player:LoadPermissionsFromDisk()
					end
				end
				local loopWorlds = function ( World )
					World:ForEachPlayer( loopPlayers )
				end
				cRoot:Get():ForEachWorld( loopWorlds )
			end
		end
	end
	Content = Content .. [[
	<form method="POST">
	<table>
	<tr><td style="width: 20%;">Player:</td>
	<td><input type="text" name="AddPlayer" value=""></td></tr><br>
	<tr><td style="width: 20%;">Group:</td>
	<td><input type="text" name="AddGroup" value=""></td></tr>
	</table>
	<input type="submit" value="Add Player" name="AddPlayerToGroup">]]
	return Content
end

function HandleRequest_Permissions( Request )
	GroupsIni = cIniFile("groups.ini")
	if( GroupsIni:ReadFile() == false ) then
		return "Could not read groups.ini!"
	end
	UsersIni = cIniFile("users.ini")
	if( UsersIni:ReadFile() == false ) then
		return "Could not read users.ini!"
	end
	
	local Content = ""
	
	Content = Content .. AddPlayers( Request )
	Content = Content .. ShowGroupsTable()
	Content = Content .. ShowUsersTable()
	
	return Content
end