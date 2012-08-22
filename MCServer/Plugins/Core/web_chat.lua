local CHAT_HISTORY = 50

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
		
		function OpenPage( url, callback ) 
		{
			var xhr = createXHR();
			xhr.onreadystatechange=function()
			{ 
				if (xhr.readyState == 4)
				{
					callback( xhr )
				} 
			}; 
			xhr.open("GET", url , true);
			xhr.send(null); 
		}

		function LoadPageInto( url, storage ) 
		{
			OpenPage( url, function( xhr ) 
			{
				var ScrollBottom = storage.scrollTop + storage.offsetHeight;
				var bAutoScroll = (ScrollBottom >= storage.scrollHeight); // Detect whether we scrolled to the bottom of the div
				
				storage.innerHTML = xhr.responseText;
				
				if( bAutoScroll == true )
				{
					storage.scrollTop = storage.scrollHeight;
				}
			} );
			
			return false;
		}
		
		function SendChatMessage() 
		{
			var MessageContainer = document.getElementById('ChatMessage');
			if( MessageContainer.value == "" ) return;
			
			OpenPage( "/~webadmin/Core/Chat/?ChatMessage=" + MessageContainer.value, function( xhr ) 
			{
				RefreshChat();
			} );
			MessageContainer.value = "";
		}
		
		function RefreshChat() 
		{
			LoadPageInto('/~webadmin/Core/Chat/?JustChat=true', document.getElementById('ChatDiv'));
		}
		
		setInterval(RefreshChat, 1000);
		window.onload = RefreshChat;
		
	</script>
]]

local ChatLogMessages = {}

function AddMessage( PlayerName, Message )
	table.insert( ChatLogMessages, { name = PlayerName, message = Message } )
	while( #ChatLogMessages > CHAT_HISTORY ) do
		table.remove( ChatLogMessages, 1 )
	end
end

function OnChat( Player, Message )
	AddMessage( Player:GetName(), Message )
end
		
function HandleRequest_Chat( Request )
	if( Request.Params["JustChat"] ~= nil ) then
		local Content = ""
		for key, value in pairs(ChatLogMessages) do 
			Content = Content .. "[" .. value.name .. "]: " .. value.message .. "<br>"
		end
		return Content
	end
	
	if( Request.Params["ChatMessage"] ~= nil ) then
		LOG("Chat msg: " .. Request.Params["ChatMessage"] )
		local Message = "[WebAdmin]: " .. Request.Params["ChatMessage"]
		cRoot:Get():GetServer():SendMessage( Message )
		AddMessage("WebAdmin", Request.Params["ChatMessage"] )
		return ""
	end

	local Content = JavaScript
	Content = Content .. [[
	<div style="font-family: Courier; border: 1px solid #DDD; padding: 10px; width: 97%; height: 200px; overflow: scroll;" id="ChatDiv">Chat messageessss</div>
	<input type="text" id="ChatMessage" onKeyPress="if (event.keyCode == 13) { SendChatMessage(); }"><input type="submit" value="Submit" onClick="SendChatMessage();">
	]]
	return Content
end