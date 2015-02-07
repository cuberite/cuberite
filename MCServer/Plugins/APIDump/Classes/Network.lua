
-- Network.lua

-- Defines the documentation for the cNetwork-related classes





return
{
	cNetwork =
	{
		Desc =
		[[
			This is the namespace for high-level network-related operations. Allows plugins to make TCP
			connections to the outside world using a callback-based API.</p>
			<p>
			All functions in this namespace are static, they should be called on the cNetwork class itself:
<pre class="prettyprint lang-lua">
local Server = cNetwork:Listen(1024, ListenCallbacks);
</pre></p>
		]],
		AdditionalInfo =
		{
			{
				Header = "Using callbacks",
				Contents =
				[[
					The entire Networking API is callback-based. Whenever an event happens on the network object, a
					specific plugin-provided function is called. The callbacks are stored in tables which are passed
					to the API functions, each table contains multiple callbacks for the various situations.</p>
					<p>
					There are three different callback variants used: LinkCallbacks, LookupCallbacks and
					ListenCallbacks. Each is used in the situation appropriate by its name - LinkCallbacks are used
					for handling the traffic on a single network link (plus additionally creation of such link when
					connecting as a client), LookupCallbacks are used when doing DNS and reverse-DNS lookups, and
					ListenCallbacks are used for handling incoming connections as a server.</p>
					<p>
					LinkCallbacks have the following structure:<br/>
<pre class="prettyprint lang-lua">
local LinkCallbacks =
{
	OnConnected = function ({{cTCPLink|a_TCPLink}})
		-- The specified {{cTCPLink|link}} has succeeded in connecting to the remote server.
		-- Only called if the link is being connected as a client (using cNetwork:Connect() )
		-- Not used for incoming server links
		-- All returned values are ignored
	end,
	
	OnError = function ({{cTCPLink|a_TCPLink}}, a_ErrorCode, a_ErrorMsg)
		-- The specified error has occured on the {{cTCPLink|link}}
		-- No other callback will be called for this link from now on
		-- For a client link being connected, this reports a connection error (destination unreachable etc.)
		-- It is an Undefined Behavior to send data to a_TCPLink in or after this callback
		-- All returned values are ignored
	end,
	
	OnReceivedData = function ({{cTCPLink|a_TCPLink}}, a_Data)
		-- Data has been received on the {{cTCPLink|link}}
		-- Will get called whenever there's new data on the {{cTCPLink|link}}
		-- a_Data contains the raw received data, as a string
		-- All returned values are ignored
	end,
	
	OnRemoteClosed = function ({{cTCPLink|a_TCPLink}})
		-- The remote peer has closed the {{cTCPLink|link}}
		-- The link is already closed, any data sent to it now will be lost
		-- No other callback will be called for this link from now on
		-- All returned values are ignored
	end,
}
</pre></p>
				<p>
				LookupCallbacks have the following structure:<br/>
<pre class="prettyprint lang-lua">
local LookupCallbacks =
{
	OnError = function (a_Query, a_ErrorCode, a_ErrorMsg)
		-- The specified error has occured while doing the lookup
		-- a_Query is the hostname or IP being looked up
		-- No other callback will be called for this lookup from now on
		-- All returned values are ignored
	end,

	OnFinished = function (a_Query)
		-- There are no more DNS records for this query
		-- a_Query is the hostname or IP being looked up
		-- No other callback will be called for this lookup from now on
		-- All returned values are ignored
	end,

	OnNameResolved = function (a_Hostname, a_IP)
		-- A DNS record has been found, the specified hostname resolves to the IP
		-- Called for both Hostname -&gt; IP and IP -&gt; Hostname lookups
		-- May be called multiple times if a hostname resolves to multiple IPs
		-- All returned values are ignored
	end,
}
</pre></p>
				<p>
				ListenCallbacks have the following structure:<br/>
<pre class="prettyprint lang-lua">
local ListenCallbacks =
{
	OnAccepted = function ({{cTCPLink|a_TCPLink}})
		-- A new connection has been accepted and a {{cTCPLink|Link}} for it created
		-- It is safe to send data to the link now
		-- All returned values are ignored
	end,
	
	OnError = function (a_ErrorCode, a_ErrorMsg)
		-- The specified error has occured while trying to listen
		-- No other callback will be called for this lookup from now on
		-- This callback is called before the cNetwork:Listen() call returns
		-- All returned values are ignored
	end,

	OnIncomingConnection = function (a_RemoteIP, a_RemotePort, a_LocalPort)
		-- A new connection is being accepted, from the specified remote peer
		-- This function needs to return either nil to drop the connection,
		-- or valid LinkCallbacks to use for the new connection's {{cTCPLink|TCPLink}} object
		return SomeLinkCallbacks
	end,
}
</pre></p>
				]],
			},
			
			{
				Header = "Example client connection",
				Contents =
				[[
					The following example, adapted from the NetworkTest plugin, shows a simple example of a client
					connection using the cNetwork API. It connects to www.google.com on port 80 (http) and sends a http
					request for the front page. It dumps the received data to the console.</p>
					<p>
					First, the callbacks are defined in a table. The OnConnected() callback takes care of sending the http
					request once the socket is connected. The OnReceivedData() callback sends all data to the console. The
					OnError() callback logs any errors. Then, the connection is initiated using the cNetwork::Connect() API
					function.</p>
					<p>
<pre class="prettyprint lang-lua">
-- Define the callbacks to use for the client connection:
local ConnectCallbacks =
{
	OnConnected = function (a_Link)
		-- Connection succeeded, send the http request:
		a_Link:Send("GET / HTTP/1.0\r\nHost: www.google.com\r\n\r\n")
	end,
	
	OnError = function (a_Link, a_ErrorCode, a_ErrorMsg)
		-- Log the error to console:
		LOG("An error has occurred while talking to google.com: " .. a_ErrorCode .. " (" .. a_ErrorMsg .. ")")
	end,
	
	OnReceivedData = function (a_Link, a_Data)
		-- Log the received data to console:
		LOG("Incoming http data:\r\n" .. a_Data)
	end,
	
	OnRemoteClosed = function (a_Link)
		-- Log the event into the console:
		LOG("Connection to www.google.com closed")
	end,
}

-- Connect:
if not(cNetwork:Connect("www.google.com", 80, ConnectCallbacks)) then
	-- Highly unlikely, but better check for errors
	LOG("Cannot queue connection to www.google.com")
end
-- Note that the connection is being made on the background,
-- there's no guarantee that it's already connected at this point in code
</pre>
				]],
			},
			
			{
				Header = "Example server implementation",
				Contents =
				[[
					The following example, adapted from the NetworkTest plugin, shows a simple example of creating a
					server listening on a TCP port using the cNetwork API. The server listens on port 9876 and for
					each incoming connection it sends a welcome message and then echoes back whatever the client has
					sent ("echo server").</p>
					<p>
					First, the callbacks for the listening server are defined. The most important of those is the
					OnIncomingConnection() callback that must return the LinkCallbacks that will be used for the new
					connection. In our simple scenario each connection uses the same callbacks, so a predefined
					callbacks table is returned; it is, however, possible to define different callbacks for each
					connection. This allows the callbacks to be "personalised", for example by the remote IP or the
					time of connection. The OnAccepted() and OnError() callbacks only log that they occurred, there's
					no processing needed for them.</p>
					<p>
					Finally, the cNetwork:Listen() API function is used to create the listening server. The status of
					the server is checked and if it is successfully listening, it is stored in a global variable, so
					that Lua doesn't garbage-collect it until we actually want the server closed.</p>
					<p>
<pre class="prettyprint lang-lua">
-- Define the callbacks used for the incoming connections:
local EchoLinkCallbacks =
{
	OnConnected = function (a_Link)
		-- This will not be called for a server connection, ever
		assert(false, "Unexpected Connect callback call")
	end,
	
	OnError = function (a_Link, a_ErrorCode, a_ErrorMsg)
		-- Log the error to console:
		local RemoteName = "'" .. a_Link:GetRemoteIP() .. ":" .. a_Link:GetRemotePort() .. "'"
		LOG("An error has occurred while talking to " .. RemoteName .. ": " .. a_ErrorCode .. " (" .. a_ErrorMsg .. ")")
	end,
	
	OnReceivedData = function (a_Link, a_Data)
		-- Send the received data back to the remote peer
		a_Link:Send(Data)
	end,
	
	OnRemoteClosed = function (a_Link)
		-- Log the event into the console:
		local RemoteName = "'" .. a_Link:GetRemoteIP() .. ":" .. a_Link:GetRemotePort() .. "'"
		LOG("Connection to '" .. RemoteName .. "' closed")
	end,
}

-- Define the callbacks used by the server:
local ListenCallbacks =
{
	OnAccepted = function (a_Link)
		-- No processing needed, just log that this happened:
		LOG("OnAccepted callback called")
	end,
	
	OnError = function (a_ErrorCode, a_ErrorMsg)
		-- An error has occured while listening for incoming connections, log it:
		LOG("Cannot listen, error " .. a_ErrorCode .. " (" .. a_ErrorMsg .. ")"
	end,

	OnIncomingConnection = function (a_RemoteIP, a_RemotePort, a_LocalPort)
		-- A new connection is being accepted, give it the EchoCallbacks
		return EchoLinkCallbacks
	end,
}

-- Start the server:
local Server = cNetwork:Listen(9876, ListenCallbacks)
if not(Server:IsListening()) then
	-- The error has been already printed in the OnError() callbacks
	-- Just bail out
	return;
end

-- Store the server globally, so that it stays open:
g_Server = Server

...

-- Elsewhere in the code, when terminating:
-- Close the server and let it be garbage-collected:
g_Server:Close()
g_Server = nil
</pre>
				]],
			},
		},  -- AdditionalInfo
		
		Functions =
		{
				Connect = { Params = "Host, Port, LinkCallbacks", Return = "bool", Notes = "(STATIC) Begins establishing a (client) TCP connection to the specified host. Uses the LinkCallbacks table to report progress, success, errors and incoming data. Returns false if it fails immediately (bad port value, bad hostname format), true otherwise. Host can be either an IP address or a hostname." },
				HostnameToIP = { Params = "Host, LookupCallbacks", Return = "bool", Notes = "(STATIC) Begins a DNS lookup to find the IP address(es) for the specified host. Uses the LookupCallbacks table to report progress, success or errors. Returns false if it fails immediately (bad hostname format), true if the lookup started successfully. Host can be either a hostname or an IP address." },
				IPToHostname = { Params = "Address, LookupCallbacks", Return = "bool", Notes = "(STATIC) Begins a reverse-DNS lookup to find out the hostname for the specified IP address. Uses the LookupCallbacks table to report progress, success or errors. Returns false if it fails immediately (bad address format), true if the lookup started successfully." },
				Listen = { Params = "Port, ListenCallbacks", Return = "{{cServerHandle|ServerHandle}}", Notes = "(STATIC) Starts listening on the specified port. Uses the ListenCallbacks to report incoming connections or errors. Returns a {{cServerHandle}} object representing the server. If the listen operation failed, the OnError callback is called with the error details and the returned server handle will report IsListening() == false. The plugin needs to store the server handle object for as long as it needs the server running, if the server handle is garbage-collected by Lua, the listening socket will be closed and all current connections dropped." },
		},
	},  -- cNetwork
	
	cTCPLink =
	{
		Desc =
		[[
			This class wraps a single TCP connection, that has been established. Plugins can create these by
			calling {{cNetwork}}:Connect() to connect to a remote server, or by listening using
			{{cNetwork}}:Listen() and accepting incoming connections. The links are callback-based - when an event
			such as incoming data or remote disconnect happens on the link, a specific callback is called. See the
			additional information in {{cNetwork}} documentation for details.
		]],
		
		Functions =
		{
			GetLocalIP = { Params = "", Return = "string", Notes = "Returns the IP address of the local endpoint of the TCP connection." },
			GetLocalPort = { Params = "", Return = "number", Notes = "Returns the port of the local endpoint of the TCP connection." },
			GetRemoteIP = { Params = "", Return = "string", Notes = "Returns the IP address of the remote endpoint of the TCP connection." },
			GetRemotePort = { Params = "", Return = "number", Notes = "Returns the port of the remote endpoint of the TCP connection." },
			Send = { Params = "Data", Return = "", Notes = "Sends the data (raw string) to the remote peer. The data is sent asynchronously and there is no report on the success of the send operation, other than the connection being closed or reset by the underlying OS." },
		},
	},  -- cTCPLink

	cServerHandle =
	{
		Desc =
		[[
			This class provides an interface for TCP sockets listening for a connection. In order to listen, the
			plugin needs to use the {{cNetwork}}:Listen() function to create the listening socket.</p>
			<p>
			Note that when Lua garbage-collects this class, the listening socket is closed. Therefore the plugin
			should keep it referenced in a global variable for as long as it wants the server running.
		]],
		
		Functions =
		{
			Close = { Params = "", Return = "", Notes = "Closes the listening socket. No more connections will be accepted, and all current connections will be closed." },
			IsListening = { Params = "", Return = "bool", Notes = "Returns true if the socket is listening." },
		},

	},  -- cServerHandle
}




