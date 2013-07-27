local function HTMLDeleteButton( name )
	return "<form method=\"POST\"><input type=\"hidden\" name=\"whitelist-delete\" value=\"".. name .."\"><input type=\"submit\" value=\"Remove from whitelist\"></form>"
end

function HandleRequest_WhiteList( Request )
	local UpdateMessage = ""
	if( Request.PostParams["whitelist-add"] ~= nil ) then
		local PlayerName = Request.PostParams["whitelist-add"]
		
		if( WhiteListIni:GetValueB("WhiteList", PlayerName, false) == true ) then
			UpdateMessage = "<b>".. PlayerName.."</b> is already on the whitelist"
		else
			WhiteListIni:SetValueB("WhiteList", PlayerName, true )
			UpdateMessage = "Added <b>" .. PlayerName .. "</b> to whitelist."
			WhiteListIni:WriteFile()
		end
	elseif( Request.PostParams["whitelist-delete"] ~= nil ) then
		local PlayerName = Request.PostParams["whitelist-delete"]
		WhiteListIni:DeleteValue( "WhiteList", PlayerName )
		UpdateMessage = "Removed <b>" .. PlayerName .. "</b> from whitelist."
		WhiteListIni:WriteFile()
	elseif( Request.PostParams["whitelist-reload"] ~= nil ) then
		WhiteListIni:Erase() -- Empty entire loaded ini first, otherwise weird shit goes down
		WhiteListIni:ReadFile()
		UpdateMessage = "Loaded from disk"
	elseif( Request.Params["whitelist-setenable"] ~= nil ) then
		local Enabled = Request.Params["whitelist-setenable"]
		local CreateNewValue = false
		if( WhiteListIni:FindValue( WhiteListIni:FindKey("WhiteListSettings"), "WhiteListOn" ) == cIniFile.noID ) then -- Find out whether the value is in the ini
			CreateNewValue = true
		end
		
		if( Enabled == "1" ) then
			WhiteListIni:SetValueB("WhiteListSettings", "WhiteListOn", true, CreateNewValue )
		else
			WhiteListIni:SetValueB("WhiteListSettings", "WhiteListOn", false, CreateNewValue )
		end
		WhiteListIni:WriteFile()
	end
	
	
	local Content = ""
	
	local WhiteListEnabled = WhiteListIni:GetValueB("WhiteListSettings", "WhiteListOn", false)
	if( WhiteListEnabled == false ) then
		Content = Content .. "<p>Whitelist is currently disabled! Click <a href='?whitelist-setenable=1'>here</a> to enable.</p>"
	end
	
	
	Content = Content .. "<h4>Whitelisted players</h4>"
	Content = Content .. "<table>"
	local KeyNum = WhiteListIni:FindKey("WhiteList")
	local NumValues = WhiteListIni:GetNumValues(KeyNum)
	if( NumValues > 0 ) then
		for Num = 0, NumValues-1 do
			if( WhiteListIni:GetValue(KeyNum, Num, "0") == "1" ) then
				local PlayerName = WhiteListIni:GetValueName(KeyNum, Num )
				Content = Content .. "<tr><td>" .. PlayerName .. "</td><td>" .. HTMLDeleteButton( PlayerName ) .. "</td></tr>"
			end
		end
	else
		Content = Content .. "<tr><td>None</td></tr>"
	end
	Content = Content .. "</table>"
	Content = Content .. "<br><h4>Add player to whitelist</h4>"
	Content = Content .. "<form method=\"POST\">"
	Content = Content .. "<input type=\"text\" name=\"whitelist-add\"><input type=\"submit\" value=\"Add player\">"
	Content = Content .. "</form>"
	Content = Content .. "<form method=\"POST\">"
	Content = Content .. "<input type=\"submit\" name=\"whitelist-reload\" value=\"Reload from disk\">"
	Content = Content .. "</form>"
	Content = Content .. "<br>"..UpdateMessage
	
	if( WhiteListEnabled == true ) then
		Content = Content .. "<br><br><p>Whitelist is currently enabled, click <a href='?whitelist-setenable=0'>here</a> to disable.</p>"
	end
	
	return Content
end