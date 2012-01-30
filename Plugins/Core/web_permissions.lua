local function ShowUsersTable()
	local Content = "<h4>Users</h4>"

	local UsersIni = cIniFile("users.ini")
	if( UsersIni:ReadFile() == false ) then
		return "Could not read users.ini!"
	end
	
	local NumUsers = UsersIni:GetNumKeys()
	
	Content = Content .. "<table>"
	
	if( NumUsers > 0 ) then
		Content = Content .. "<tr><td></td><td>User</td><td>Groups</td></tr>"
	
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
	
	local GroupsIni = cIniFile("groups.ini")
	if( GroupsIni:ReadFile() == false ) then
		return "Could not read groups.ini!"
	end
	
	local NumGroups = GroupsIni:GetNumKeys()
	
	Content = Content .. "<table>"
	if( NumGroups > 0 ) then
		Content = Content .. "<tr><td></td><td>Name</td><td>Permissions</td><td>Color</td></tr>"
		
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

function HandleRequest_Permissions( Request )
	local Content = ""
	
	Content = Content .. ShowGroupsTable()
	Content = Content .. ShowUsersTable()
	
	return Content
end