
-- Network.lua

-- Defines the documentation for the cNetwork-related classes and cUrlClient





return
{
	cNetwork =
	{
		Desc = [[
			This is the namespace for high-level network-related operations. Allows plugins to make TCP
			connections to the outside world using a callback-based API.</p>
			<p>
			All functions in this namespace are static, they should be called on the cNetwork class itself:
<pre class="prettyprint lang-lua">
local Server = cNetwork:Listen(1024, ListenCallbacks);
</pre></p>
		]],
		Functions =
		{
			Connect =
			{
				IsStatic = true,
				Params =
				{
					{
						Name = "Host",
						Type = "string",
					},
					{
						Name = "Port",
						Type = "number",
					},
					{
						Name = "LinkCallbacks",
						Type = "table",
					},
				},
				Returns =
				{
					{
						Type = "boolean",
					},
				},
				Notes = "Begins establishing a (client) TCP connection to the specified host. Uses the LinkCallbacks table to report progress, success, errors and incoming data. Returns false if it fails immediately (bad port value, bad hostname format), true otherwise. Host can be either an IP address or a hostname.",
			},
			CreateUDPEndpoint =
			{
				IsStatic = true,
				Params =
				{
					{
						Name = "Port",
						Type = "number",
					},
					{
						Name = "UDPCallbacks",
						Type = "table",
					},
				},
				Returns =
				{
					{
						Type = "cUDPEndpoint",
					},
				},
				Notes = "Creates a UDP endpoint that listens for incoming datagrams on the specified port, and can be used to send or broadcast datagrams. Uses the UDPCallbacks to report incoming datagrams or errors. If the endpoint cannot be created, the OnError callback is called with the error details and the returned endpoint will report IsOpen() == false. The plugin needs to store the returned endpoint object for as long as it needs the UDP port open; if the endpoint is garbage-collected by Lua, the socket will be closed and no more incoming data will be reported.<br>If the Port is zero, the OS chooses an available UDP port for the endpoint; use {{cUDPEndpoint}}:GetPort() to query the port number in such case.",
			},
			EnumLocalIPAddresses =
			{
				IsStatic = true,
				Returns =
				{
					{
						Type = "table",
					},
				},
				Notes = "Returns all local IP addresses for network interfaces currently available on the machine, as an array-table of strings.",
			},
			HostnameToIP =
			{
				IsStatic = true,
				Params =
				{
					{
						Name = "Host",
						Type = "string",
					},
					{
						Name = "LookupCallbacks",
						Type = "table",
					},
				},
				Returns =
				{
					{
						Type = "boolean",
					},
				},
				Notes = "Begins a DNS lookup to find the IP address(es) for the specified host. Uses the LookupCallbacks table to report progress, success or errors. Returns false if it fails immediately (bad hostname format), true if the lookup started successfully. Host can be either a hostname or an IP address.",
			},
			IPToHostname =
			{
				IsStatic = true,
				Params =
				{
					{
						Name = "Address",
						Type = "string",
					},
					{
						Name = "LookupCallbacks",
						Type = "table",
					},
				},
				Returns =
				{
					{
						Type = "boolean",
					},
				},
				Notes = "Begins a reverse-DNS lookup to find out the hostname for the specified IP address. Uses the LookupCallbacks table to report progress, success or errors. Returns false if it fails immediately (bad address format), true if the lookup started successfully.",
			},
			Listen =
			{
				IsStatic = true,
				Params =
				{
					{
						Name = "Port",
						Type = "number",
					},
					{
						Name = "ListenCallbacks",
						Type = "table",
					},
				},
				Returns =
				{
					{
						Type = "cServerHandle",
					},
				},
				Notes = "Starts listening on the specified port. Uses the ListenCallbacks to report incoming connections or errors. Returns a {{cServerHandle}} object representing the server. If the listen operation failed, the OnError callback is called with the error details and the returned server handle will report IsListening() == false. The plugin needs to store the server handle object for as long as it needs the server running, if the server handle is garbage-collected by Lua, the listening socket will be closed and all current connections dropped.",
			},
		},
		AdditionalInfo =
		{
			{
				Header = "Using callbacks",
				Contents = [[
					The entire Networking API is callback-based. Whenever an event happens on the network object, a
					specific plugin-provided function is called. The callbacks are stored in tables which are passed
					to the API functions, each table contains multiple callbacks for the various situations.</p>
					<p>
					There are four different callback variants used: LinkCallbacks, LookupCallbacks, ListenCallbacks
					and UDPCallbacks. Each is used in the situation appropriate by its name - LinkCallbacks are used
					for handling the traffic on a single network link (plus additionally creation of such link when
					connecting as a client), LookupCallbacks are used when doing DNS and reverse-DNS lookups,
					ListenCallbacks are used for handling incoming connections as a server and UDPCallbacks are used
					for incoming UDP datagrams.</p>
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
		-- No other callback will be called for this server handle from now on
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
				<p>
				UDPCallbacks have the following structure:<br/>
<pre class="prettyprint lang-lua">
local UDPCallbacks =
{
	OnError = function (a_ErrorCode, a_ErrorMsg)
		-- The specified error has occured when trying to listen for incoming UDP datagrams
		-- No other callback will be called for this endpoint from now on
		-- This callback is called before the cNetwork:CreateUDPEndpoint() call returns
		-- All returned values are ignored
	end,

	OnReceivedData = function ({{cUDPEndpoint|a_UDPEndpoint}}, a_Data, a_RemotePeer, a_RemotePort)
		-- A datagram has been received on the {{cUDPEndpoint|endpoint}} from the specified remote peer
		-- a_Data contains the raw received data, as a string
		-- All returned values are ignored
	end,
}
</pre>
				]],
			},
			{
				Header = "Example client connection",
				Contents = [[
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
				Contents = [[
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
		},
	},
	cServerHandle =
	{
		Desc = [[
			This class provides an interface for TCP sockets listening for a connection. In order to listen, the
			plugin needs to use the {{cNetwork}}:Listen() function to create the listening socket.</p>
			<p>
			Note that when Lua garbage-collects this class, the listening socket is closed. Therefore the plugin
			should keep it referenced in a global variable for as long as it wants the server running.
		]],
		Functions =
		{
			Close =
			{
				Notes = "Closes the listening socket. No more connections will be accepted, and all current connections will be closed.",
			},
			IsListening =
			{
				Returns =
				{
					{
						Type = "boolean",
					},
				},
				Notes = "Returns true if the socket is listening.",
			},
		},
	},
	cTCPLink =
	{
		Desc = [[
			This class wraps a single TCP connection, that has been established. Plugins can create these by
			calling {{cNetwork}}:Connect() to connect to a remote server, or by listening using
			{{cNetwork}}:Listen() and accepting incoming connections. The links are callback-based - when an event
			such as incoming data or remote disconnect happens on the link, a specific callback is called. See the
			additional information in {{cNetwork}} documentation for details.</p>
			<p>
			The link can also optionally perform TLS encryption. Plugins can use the StartTLSClient() function to
			start the TLS handshake as the client side. Since that call, the OnReceivedData() callback is
			overridden internally so that the data is first routed through the TLS decryptor, and the plugin's
			callback is only called for the decrypted data, once it starts arriving. The Send function changes its
			behavior so that the data written by the plugin is first encrypted and only then sent over the
			network. Note that calling Send() before the TLS handshake finishes is supported, but the data is
			queued internally and only sent once the TLS handshake is completed.
		]],
		Functions =
		{
			Close =
			{
				Notes = "Closes the link forcefully (TCP RST). There's no guarantee that the last sent data is even being delivered. See also the Shutdown() method.",
			},
			GetLocalIP =
			{
				Returns =
				{
					{
						Type = "string",
					},
				},
				Notes = "Returns the IP address of the local endpoint of the TCP connection.",
			},
			GetLocalPort =
			{
				Returns =
				{
					{
						Type = "number",
					},
				},
				Notes = "Returns the port of the local endpoint of the TCP connection.",
			},
			GetRemoteIP =
			{
				Returns =
				{
					{
						Type = "string",
					},
				},
				Notes = "Returns the IP address of the remote endpoint of the TCP connection.",
			},
			GetRemotePort =
			{
				Returns =
				{
					{
						Type = "number",
					},
				},
				Notes = "Returns the port of the remote endpoint of the TCP connection.",
			},
			Send =
			{
				Params =
				{
					{
						Name = "Data",
						Type = "string",
					},
				},
				Notes = "Sends the data (raw string) to the remote peer. The data is sent asynchronously and there is no report on the success of the send operation, other than the connection being closed or reset by the underlying OS.",
			},
			Shutdown =
			{
				Notes = "Shuts the socket down for sending data. Notifies the remote peer that there will be no more data coming from us (TCP FIN). The data that is in flight will still be delivered. The underlying socket will be closed when the remote end shuts down as well, or after a timeout.",
			},
			StartTLSClient =
			{
				Params =
				{
					{
						Name = "OwnCert",
						Type = "string",
					},
					{
						Name = "OwnPrivateKey",
						Type = "string",
					},
					{
						Name = "OwnPrivateKeyPassword",
						Type = "string",
					},
				},
				Returns =
				{
					{
						Name = "IsSuccess",
						Type = "boolean",
					},
					{
						Name = "ErrorMessage",
						Type = "string",
						IsOptional = true,
					},
				},
				Notes = "Starts a TLS handshake on the link, as a client side of the TLS. The Own___ parameters specify the client certificate and its corresponding private key and password; all three parameters are optional and no client certificate is presented to the remote peer if they are not used or all empty. Once the TLS handshake is started by this call, all incoming data is first decrypted before being sent to the OnReceivedData callback, and all outgoing data is queued until the TLS handshake completes, and then sent encrypted over the link. Returns true on success, nil and optional error message on immediate failure.<br/><b>NOTE:</b> The TLS support in the API is currently experimental and shouldn't be considered safe - there's no peer certificate verification and the error reporting is only basic.",
			},
			StartTLSServer =
			{
				Params =
				{
					{
						Name = "Certificate",
						Type = "string",
					},
					{
						Name = "PrivateKey",
						Type = "string",
					},
					{
						Name = "PrivateKeyPassword",
						Type = "string",
					},
					{
						Name = "StartTLSData",
						Type = "string",
					},
				},
				Returns =
				{
					{
						Name = "IsSuccess",
						Type = "boolean",
					},
					{
						Name = "ErrorMessage",
						Type = "string",
						IsOptional = true,
					},
				},
				Notes = "Starts a TLS handshake on the link, as a server side of the TLS. The plugin needs to specify the server certificate and its corresponding private key and password. The StartTLSData can contain data that the link has already reported as received but it should be used as part of the TLS handshake. Once the TLS handshake is started by this call, all incoming data is first decrypted before being sent to the OnReceivedData callback, and all outgoing data is queued until the TLS handshake completes, and then sent encrypted over the link. Returns true on success, nil and optional error message on immediate failure.<br/><b>NOTE:</b> The TLS support in the API is currently experimental and shouldn't be considered safe - there's no peer certificate verification and the error reporting is only basic.",
			},
		},
	},
	cUDPEndpoint =
	{
		Desc = [[
			Represents a UDP socket that is listening for incoming datagrams on a UDP port and can send or broadcast datagrams to other peers on the network. Plugins can create an instance of the endpoint by calling {{cNetwork}}:CreateUDPEndpoint(). The endpoints are callback-based - when a datagram is read from the network, the OnRececeivedData() callback is called with details about the datagram. See the additional information in {{cNetwork}} documentation for details.</p>
			<p>
			Note that when Lua garbage-collects this class, the listening socket is closed. Therefore the plugin should keep this object referenced in a global variable for as long as it wants the endpoint open.
		]],
		Functions =
		{
			Close =
			{
				Notes = "Closes the UDP endpoint. No more datagrams will be reported through the callbacks, the UDP port will be closed.",
			},
			EnableBroadcasts =
			{
				Notes = "Some OSes need this call before they allow UDP broadcasts on an endpoint.",
			},
			GetPort =
			{
				Returns =
				{
					{
						Type = "number",
					},
				},
				Notes = "Returns the local port number of the UDP endpoint listening for incoming datagrams. Especially useful if the UDP endpoint was created with auto-assign port (0).",
			},
			IsOpen =
			{
				Returns =
				{
					{
						Type = "boolean",
					},
				},
				Notes = "Returns true if the UDP endpoint is listening for incoming datagrams.",
			},
			Send =
			{
				Params =
				{
					{
						Name = "RawData",
						Type = "string",
					},
					{
						Name = "RemoteHost",
						Type = "string",
					},
					{
						Name = "RemotePort",
						Type = "number",
					},
				},
				Returns =
				{
					{
						Type = "boolean",
					},
				},
				Notes = "Sends the specified raw data (string) to the specified remote host. The RemoteHost can be either a hostname or an IP address; if it is a hostname, the endpoint will queue a DNS lookup first, if it is an IP address, the send operation is executed immediately. Returns true if there was no immediate error, false on any failure. Note that the return value needn't represent whether the packet was actually sent, only if it was successfully queued.",
			},
		},
	},
	cUrlClient =
	{
		Desc = [[
			Implements high-level asynchronous access to URLs, such as downloading webpages over HTTP(S).</p>
			<p>
			Note that unlike other languages' URL access libraries, this class implements asynchronous requests.
			This means that the functions only start a request and return immediately. The request is then
			fulfilled in the background, while the server continues to run. The response is delivered back to the
			plugin using callbacks. This allows the plugin to start requests and not block the server until the
			response is received.</p>
			<p>
			The functions that make network requests are all static and have a dual interface. Either you can use
			a single callback function, which gets called once the entire response is received or an error is
			encountered. Or you can use a table of callback functions, each function being called whenever the
			specific event happens during the request and response lifetime. See the Simple Callback and Callback
			Table chapters later on this page for details and examples.</p>
			<p>
			All the request function also support optional parameters for further customization of the request -
			the Headers parameter specifies additional HTTP headers that are to be sent (as a dictionary-table of
			key -> value), the RequestBody parameter specifying the optional body of the request (used mainly for
			POST and PUT requests), and an Options parameter specifying additional options specific to the protocol
			used.
		]],
		Functions =
		{
			Delete =
			{
				{
					IsStatic = true,
					Params =
					{
						{
							Name = "URL",
							Type = "string",
						},
						{
							Name = "Callbacks",
							Type = "table",
						},
						{
							Name = "Headers",
							Type = "table",
							IsOptional = true,
						},
						{
							Name = "RequestBody",
							Type = "string",
							IsOptional = true,
						},
						{
							Name = "Options",
							Type = "table",
							IsOptional = true,
						},
					},
					Returns =
					{
						{
							Name = "IsSuccess",
							Type = "boolean",
						},
						{
							Name = "ErrorMessagge",
							Type = "string",
							IsOptional = true,
						},
					},
					Notes = "Starts a HTTP DELETE request. Alias for Request(\"DELETE\", ...). Returns true on succes, false and error message on immediate failure (unparsable URL etc.).",
				},
				{
					IsStatic = true,
					Params =
					{
						{
							Name = "URL",
							Type = "string",
						},
						{
							Name = "Callbacks",
							Type = "function",
						},
					},
					Returns =
					{
						{
							Name = "IsSuccess",
							Type = "boolean",
						},
						{
							Name = "ErrorMessagge",
							Type = "string",
							IsOptional = true,
						},
					},
					Notes = "Starts a HTTP DELETE request. Alias for Request(\"DELETE\", ...). Returns true on succes, false and error message on immediate failure (unparsable URL etc.).",
				},
			},
			Get =
			{
				{
					IsStatic = true,
					Params =
					{
						{
							Name = "URL",
							Type = "string",
						},
						{
							Name = "Callbacks",
							Type = "table",
						},
						{
							Name = "Headers",
							Type = "table",
							IsOptional = true,
						},
						{
							Name = "RequestBody",
							Type = "string",
							IsOptional = true,
						},
						{
							Name = "Options",
							Type = "table",
							IsOptional = true,
						},
					},
					Returns =
					{
						{
							Name = "IsSuccess",
							Type = "boolean",
						},
						{
							Name = "ErrMsg",
							Type = "string",
							IsOptional = true,
						},
					},
					Notes = "Starts a HTTP GET request. Alias for Request(\"GET\", ...). Returns true on succes, false and error message on immediate failure (unparsable URL etc.).",
				},
				{
					IsStatic = true,
					Params =
					{
						{
							Name = "URL",
							Type = "string",
						},
						{
							Name = "Callbacks",
							Type = "function",
						},
					},
					Returns =
					{
						{
							Name = "IsSuccess",
							Type = "boolean",
						},
						{
							Name = "ErrMsg",
							Type = "string",
							IsOptional = true,
						},
					},
					Notes = "Starts a HTTP GET request. Alias for Request(\"GET\", ...). Returns true on succes, false and error message on immediate failure (unparsable URL etc.).",
				}
			},
			Post =
			{
				{
					IsStatic = true,
					Params =
					{
						{
							Name = "URL",
							Type = "string",
						},
						{
							Name = "Callbacks",
							Type = "table",
						},
						{
							Name = "Headers",
							Type = "table",
							IsOptional = true,
						},
						{
							Name = "RequestBody",
							Type = "string",
							IsOptional = true,
						},
						{
							Name = "Options",
							Type = "table",
							IsOptional = true,
						},
					},
					Returns =
					{
						{
							Name = "IsSuccess",
							Type = "boolean",
						},
						{
							Name = "ErrMsg",
							Type = "string",
							IsOptional = true,
						},
					},
					Notes = "Starts a HTTP POST request. Alias for Request(\"POST\", ...). Returns true on succes, false and error message on immediate failure (unparsable URL etc.).",
				},
				{
					IsStatic = true,
					Params =
					{
						{
							Name = "URL",
							Type = "string",
						},
						{
							Name = "Callbacks",
							Type = "function",
						},
					},
					Returns =
					{
						{
							Name = "IsSuccess",
							Type = "boolean",
						},
						{
							Name = "ErrMsg",
							Type = "string",
							IsOptional = true,
						},
					},
					Notes = "Starts a HTTP POST request. Alias for Request(\"POST\", ...). Returns true on succes, false and error message on immediate failure (unparsable URL etc.).",
				},
			},
			Put =
			{
				{
					IsStatic = true,
					Params =
					{
						{
							Name = "URL",
							Type = "string",
						},
						{
							Name = "Callbacks",
							Type = "table",
						},
						{
							Name = "Headers",
							Type = "table",
							IsOptional = true,
						},
						{
							Name = "RequestBody",
							Type = "string",
							IsOptional = true,
						},
						{
							Name = "Options",
							Type = "table",
							IsOptional = true,
						},
					},
					Returns =
					{
						{
							Name = "IsSuccess",
							Type = "boolean",
						},
						{
							Name = "ErrMsg",
							Type = "string",
							IsOptional = true,
						},
					},
					Notes = "Starts a HTTP PUT request. Alias for Request(\"PUT\", ...). Returns true on succes, false and error message on immediate failure (unparsable URL etc.).",
				},
				{
					IsStatic = true,
					Params =
					{
						{
							Name = "URL",
							Type = "string",
						},
						{
							Name = "Callbacks",
							Type = "function",
						},
					},
					Returns =
					{
						{
							Name = "IsSuccess",
							Type = "boolean",
						},
						{
							Name = "ErrMsg",
							Type = "string",
							IsOptional = true,
						},
					},
					Notes = "Starts a HTTP PUT request. Alias for Request(\"PUT\", ...). Returns true on succes, false and error message on immediate failure (unparsable URL etc.).",
				},
			},
			Request =
			{
				IsStatic = true,
				Params =
				{
					{
						Name = "Method",
						Type = "string",
					},
					{
						Name = "URL",
						Type = "string",
					},
					{
						Name = "Callbacks",
						Type = "table",
					},
					{
						Name = "Headers",
						Type = "table",
						IsOptional = true,
					},
					{
						Name = "RequestBody",
						Type = "string",
						IsOptional = true,
					},
					{
						Name = "Options",
						Type = "table",
						IsOptional = true,
					},
				},
				Returns =
				{
					{
						Name = "IsSuccess",
						Type = "boolean",
					},
					{
						Name = "ErrMsg",
						Type = "string",
						IsOptional = true,
					},
				},
				Notes = "Starts a request with the specified Method. Returns true on succes, false and error message on immediate failure (unparsable URL etc.).",
			},
		},
		AdditionalInfo =
		{
			{
				Header = "Simple Callback",
				Contents = [[
					When you don't need fine control for receiving the requests and are interested only in the result,
					you can use the simple callback approach. Pass a single function as the Callback parameter, the
					function will get called when the response is fully processed, either with the body of the response,
					or with an error message:
<pre class="prettyprint lang-lua">
cUrlClient:Get(url,
	function (a_Body, a_Data)
		if (a_Body) then
			-- Response received correctly, a_Body contains the entire response body,
			-- a_Data is a dictionary-table of the response's HTTP headers
		else
			-- There was an error, a_Data is the error message string
		end
	end
)
</pre>
				]],
			},
			{
				Header = "Callback Table",
				Contents = [[
					To provide complete control over the request and response handling, Cuberite allows plugins to pass
					a table of callbacks as the Callback parameter. Then the respective functions are called for their
					respective events during the lifetime of the request and response. This way it is possible to
					process huge downloads that wouldn't fit into memory otherwise, or display detailed progress.</p>
					Each callback function receives a "self" as its first parameter, this allows the functions to
					access the Callback table and any of its other members, allowing the use of Lua object idiom for
					the table. See <a href="https://forum.cuberite.org/thread-2062.html">this forum post</a> for an
					example.
					<p>
					The following callback functions are used by Cuberite. Any callback that is not assigned is
					silently ignored. The table may also contain other functions and other values, those are silently
					ignored.</p>
					<table>
						<tr><th>Callback</th><th>Params</th><th>Notes</th></tr>
						<tr><td>OnConnected</td><td>self, {{cTCPLink}}, RemoteIP, RemotePort</td><td>Called when the connection to the remote host is established. <i>Note that current implementation doesn't provide the {{cTCPLink}} parameter and passes nil instead.</i></td></tr>
						<tr><td>OnCertificateReceived</td><td>self</td><td>Called for HTTPS URLs when the server's certificate is received. If the callback returns anything else than true, the connection is aborted. <i>Note that the current implementation doesn't provide the certificate because there is no representation for the cert in Lua.</i></td></tr>
						<tr><td>OnTlsHandshakeCompleted</td><td>self</td><td>Called for HTTPS URLs when the TLS is established on the connection.</td></tr>
						<tr><td>OnRequestSent</td><td>self</td><td>Called after the entire request is sent to the server.</td></tr>
						<tr><td>OnStatusLine</td><td>self, HttpVersion, StatusCode, Rest</td><td>The initial line of the response has been parsed. HttpVersion is typically "HTTP/1.1", StatusCode is the numerical HTTP status code reported by the server (200 being OK), Rest is the rest of the line, usually a short message in case of an error.</td></tr>
						<tr><td>OnHeader</td><td>self, Name, Value</td><td>A new HTTP response header line has been received.</td></tr>
						<tr><td>OnHeadersFinished</td><td>self, AllHeaders</td><td>All HTTP response headers have been parsed. AllHeaders is a dictionary-table containing all the headers received.</td></tr>
						<tr><td>OnBodyData</td><td>self, Data</td><td>A piece of the response body has been received. This callback is called repeatedly until the entire body is reported through its Data parameter.</td></tr>
						<tr><td>OnBodyFinished</td><td>self</td><td>The entire response body has been reported by OnBodyData(), the response has finished.</td></tr>
						<tr><td>OnError</td><td>self, ErrorMsg</td><td>Called whenever an error is detected. After this call, no other callback will get called.</td></tr>
						<tr><td>OnRedirecting</td><td>self, NewUrl</td><td>Called if the server returned a valid redirection HTTP status code and a Location header, and redirection is allowed by the Options.</td></tr>
					</table>
					<p>
					The following example is adapted from the Debuggers plugin's "download" command, it downloads the
					contents of an URL into a file.
<pre class="prettyprint lang-lua">
function HandleConsoleDownload(a_Split)  -- Console command handler
	-- Read the params from the command:
	local url = a_Split[2]
	local fnam = a_Split[3]
	if (not(url) or not(fnam)) then
		return true, "Missing parameters. Usage: download <url> <filename>"
	end

	-- Define the cUrlClient callbacks
	local callbacks =
	{
		OnStatusLine = function (self, a_HttpVersion, a_Status, a_Rest)
			-- Only open the output file if the server reports a success:
			if (a_Status ~= 200) then
				LOG("Cannot download " .. url .. ", HTTP error code " .. a_Status)
				return
			end
			local f, err = io.open(fnam, "wb")
			if not(f) then
				LOG("Cannot download " .. url .. ", error opening the file " .. fnam .. ": " .. (err or "<no message>"))
				return
			end
			self.m_File = f
		end,

		OnBodyData = function (self, a_Data)
			-- If the file has been opened, write the data:
			if (self.m_File) then
				self.m_File:write(a_Data)
			end
		end,

		OnBodyFinished = function (self)
			-- If the file has been opened, close it and report success
			if (self.m_File) then
				self.m_File:close()
				LOG("File " .. fnam .. " has been downloaded.")
			end
		end,
	}

	-- Start the URL download:
	local isSuccess, msg = cUrlClient:Get(url, callbacks)
	if not(isSuccess) then
		LOG("Cannot start an URL download: " .. (msg or "<no message>"))
		return true
	end
	return true
end
</pre>
]],
			},
			{
				Header = "Options",
				Contents = [[
					The requests support the following options, specified in the optional Options table parameter:
					<table>
						<tr><th>Option name</th><th>Description</th></tr>
						<tr><td>MaxRedirects</td><td>Maximum number of HTTP redirects that the cUrlClient will follow. If the server still reports a redirect after reaching this many redirects, the cUrlClient reports an error. May be specified as either a number or a string parsable into a number. Default: 30.</td></tr>
						<tr><td>OwnCert</td><td>The client certificate to use, if requested by the server. A string containing a PEM- or DER-encoded cert is expected.</td></tr>
						<tr><td>OwnPrivKey</td><td>The private key appropriate for OwnCert. A string containing a PEM- or DER-encoded private key is expected.</td></tr>
						<tr><td>OwnPrivKeyPassword</td><td>The password for OwnPrivKey. If not present or empty, no password is assumed.</td></tr>
					</table>
					<p>
					Redirection:
					<ul>
						<li>If a redirect is received, and redirection is allowed by MaxRedirects, the redirection is
						reported via OnRedirecting() callback and the request is restarted at the redirect URL, without
						reporting any of the redirect's headers nor body.</li>
						<li>If a redirect is received and redirection is not allowed (maximum redirection attempts have
						been reached), the OnRedirecting() callback is called with the redirect URL and then the request
						terminates with an OnError() callback, without reporting the redirect's headers nor body.</li>
					</ul>
				]],
			},
		},
	},
}
