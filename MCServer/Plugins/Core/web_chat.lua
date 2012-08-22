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
					catch (err1) 
					{
						request = false;
					}
						}
					}
				return request;
			}
		
			function loadWholePage( url )
			{
				var storage = document.getElementById('ChatDiv');
				var xhr = createXHR();
				xhr.onreadystatechange=function()
				{ 
					if(xhr.readyState == 4)
					{
						//alert( xhr.status + " " + xhr.statusText );
						//if(xhr.status == 200)
						{
							storage.innerHTML = xhr.responseText;//getBody(xhr.responseText);
							storage.scrollTop = storage.scrollHeight;
						}
					} 
				}; 
				xhr.open("GET", url , true);
				xhr.send(null); 
				
				return false;
			}
			
			function SendChatMessage()
			{
				var MessageContainer = document.getElementById('ChatMessage');
				if( MessageContainer.value == "" ) return;
				
				var xhr = createXHR();
				xhr.onreadystatechange=function()
				{ 
					if(xhr.readyState == 4)
					{
						//alert( xhr.status + " " + xhr.statusText );
						RefreshChat();
					} 
				}; 
				xhr.open("GET", "/~webadmin/Core/Chat/?ChatMessage=" + MessageContainer.value, true);
				xhr.send(null); 
				MessageContainer.value = "";
			}
			
			function RefreshChat()
			{
				loadWholePage('/~webadmin/Core/Chat/?JustChat=true');
			}
			
			setInterval(RefreshChat, 1000);
			window.onload = RefreshChat;
			
		</script>
]]

local ChatLogMessages = {}

function AddMessage( PlayerName, Message )
	table.insert( ChatLogMessages, { name = PlayerName, message = Message } )
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