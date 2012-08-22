local CHAT_HISTORY = 50
local LastMessageID = 0

local JavaScript = [[
	<script type="text/javascript">
		function createXHR() 
		{
			var request = false;
			try {
				request = new ActiveXObject('Msxml2.XMLHTTP');
			}
			catch (err2) {
				try {
					request = new ActiveXObject('Microsoft.XMLHTTP');
				}
				catch (err3) {
					try {
						request = new XMLHttpRequest();
					}
					catch (err1) {
						request = false;
					}
				}
			}
			return request;
		}
		
		function OpenPage( url, postParams, callback ) 
		{
			var xhr = createXHR();
			xhr.onreadystatechange=function()
			{ 
				if (xhr.readyState == 4)
				{
					callback( xhr )
				} 
			}; 
			xhr.open( (postParams!=null)?"POST":"GET", url , true);
			if( postParams != null )
			{
				xhr.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded');
			}
			xhr.send(postParams); 
		}

		function LoadPageInto( url, postParams, storage ) 
		{
			OpenPage( url, postParams, function( xhr ) 
			{
				var ScrollBottom = storage.scrollTop + storage.offsetHeight;
				var bAutoScroll = (ScrollBottom >= storage.scrollHeight); // Detect whether we scrolled to the bottom of the div
				
				results = xhr.responseText.split("<<divider>>");
				if( results[2] != LastMessageID ) return; // Check if this message was meant for us
				
				LastMessageID = results[1];
				if( results[0] != "" )
				{
					storage.innerHTML += results[0];
					
					if( bAutoScroll == true )
					{
						storage.scrollTop = storage.scrollHeight;
					}
				}
			} );
			
			
			return false;
		}
		
		function SendChatMessage() 
		{
			var MessageContainer = document.getElementById('ChatMessage');
			if( MessageContainer.value == "" ) return;
			
			var postParams = "ChatMessage=" + MessageContainer.value;
			OpenPage( "/~webadmin/Core/Chat/", postParams, function( xhr ) 
			{
				RefreshChat();
			} );
			MessageContainer.value = "";
		}
		
		function RefreshChat() 
		{
			var postParams = "JustChat=true&LastMessageID=" + LastMessageID;
			LoadPageInto("/~webadmin/Core/Chat/", postParams, document.getElementById('ChatDiv'));
		}
		
		setInterval(RefreshChat, 1000);
		window.onload = RefreshChat;
		
		var LastMessageID = 0;
		
	</script>
]]

local ChatLogMessages = {}

function AddMessage( PlayerName, Message )
	LastMessageID = LastMessageID + 1
	table.insert( ChatLogMessages, { name = PlayerName, message = Message, id = LastMessageID } )
	while( #ChatLogMessages > CHAT_HISTORY ) do
		table.remove( ChatLogMessages, 1 )
	end
end

function OnChat( Player, Message )
	AddMessage( Player:GetName(), Message )
end
		
function HandleRequest_Chat( Request )
	if( Request.PostParams["JustChat"] ~= nil ) then
		local LastIdx = 0
		if( Request.PostParams["LastMessageID"] ~= nil ) then LastIdx = tonumber( Request.PostParams["LastMessageID"] ) end
		local Content = ""
		for key, value in pairs(ChatLogMessages) do 
			if( value.id > LastIdx ) then
				Content = Content .. "[" .. value.name .. "]: " .. value.message .. "<br>"
			end
		end
		Content = Content .. "<<divider>>" .. LastMessageID .. "<<divider>>" .. LastIdx
		return Content
	end
	
	if( Request.PostParams["ChatMessage"] ~= nil ) then
		local Message = "[WebAdmin]: " .. Request.PostParams["ChatMessage"]
		cRoot:Get():GetServer():SendMessage( Message )
		AddMessage("WebAdmin", Request.PostParams["ChatMessage"] )
		return ""
	end

	local Content = JavaScript
	Content = Content .. [[
	<div style="font-family: Courier; border: 1px solid #DDD; padding: 10px; width: 97%; height: 200px; overflow: scroll;" id="ChatDiv"></div>
	<input type="text" id="ChatMessage" onKeyPress="if (event.keyCode == 13) { SendChatMessage(); }"><input type="submit" value="Submit" onClick="SendChatMessage();">
	]]
	return Content
end