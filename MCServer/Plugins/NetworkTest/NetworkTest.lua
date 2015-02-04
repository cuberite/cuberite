
-- NetworkTest.lua

-- Implements a few tests for the cNetwork API





--- Map of all servers currently open
-- g_Servers[PortNum] = cServerHandle
local g_Servers = {}

--- List of fortune messages for the fortune server
-- A random message is chosen for each incoming connection
-- The contents are loaded from the splashes.txt file on plugin startup
local g_Fortunes =
{
	"Empty splashes.txt",
}

--- Map of all services that can be run as servers
-- g_Services[ServiceName] = function() -> accept-callbacks
local g_Services =
{
	-- Echo service: each connection echoes back what has been sent to it
	echo = function (a_Port)
		return
		{
			-- A new connection has come, give it new link-callbacks:
			OnIncomingConnection = function (a_RemoteIP, a_RemotePort)
				return
				{
					OnError = function (a_Link, a_ErrorCode, a_ErrorMsg)
						LOG("EchoServer(" .. a_Port .. ": Connection to " .. a_Link:GetRemoteIP() .. ":" .. a_Link:GetRemotePort() .. " failed: " .. a_ErrorCode .. " (" .. a_ErrorMsg .. ")")
					end,
					
					OnReceivedData = function (a_Link, a_Data)
						-- Echo the received data back to the link:
						a_Link:Send(a_Data)
					end,
					
					OnRemoteClosed = function (a_Link)
					end
				}  -- Link callbacks
			end,  -- OnIncomingConnection()
			
			-- Send a welcome message to newly accepted connections:
			OnAccepted = function (a_Link)
				a_Link:Send("Hello, " .. a_Link:GetRemoteIP() .. ", welcome to the echo server @ MCServer-Lua\r\n")
			end,  -- OnAccepted()
			
			-- There was an error listening on the port:
			OnError = function (a_ErrorCode, a_ErrorMsg)
				LOGINFO("EchoServer(" .. a_Port .. ": Cannot listen: " .. a_ErrorCode .. " (" .. a_ErrorMsg .. ")")
			end,  -- OnError()
		}  -- Listen callbacks
	end,  -- echo
	
	-- Fortune service: each incoming connection gets a welcome message plus a random fortune text; all communication is ignored afterwards
	fortune = function (a_Port)
		return
		{
			-- A new connection has come, give it new link-callbacks:
			OnIncomingConnection = function (a_RemoteIP, a_RemotePort)
				return
				{
					OnError = function (a_Link, a_ErrorCode, a_ErrorMsg)
						LOG("FortuneServer(" .. a_Port .. ": Connection to " .. a_Link:GetRemoteIP() .. ":" .. a_Link:GetRemotePort() .. " failed: " .. a_ErrorCode .. " (" .. a_ErrorMsg .. ")")
					end,
					
					OnReceivedData = function (a_Link, a_Data)
						-- Ignore any received data
					end,
					
					OnRemoteClosed = function (a_Link)
					end
				}  -- Link callbacks
			end,  -- OnIncomingConnection()
			
			-- Send a welcome message and the fortune to newly accepted connections:
			OnAccepted = function (a_Link)
				a_Link:Send("Hello, " .. a_Link:GetRemoteIP() .. ", welcome to the fortune server @ MCServer-Lua\r\n\r\nYour fortune:\r\n")
				a_Link:Send(g_Fortunes[math.random(#g_Fortunes)] .. "\r\n")
			end,  -- OnAccepted()
			
			-- There was an error listening on the port:
			OnError = function (a_ErrorCode, a_ErrorMsg)
				LOGINFO("FortuneServer(" .. a_Port .. ": Cannot listen: " .. a_ErrorCode .. " (" .. a_ErrorMsg .. ")")
			end,  -- OnError()
		}  -- Listen callbacks
	end,  -- fortune
	
	-- TODO: Other services (daytime, ...)
}





function Initialize(a_Plugin)
	-- Load the splashes.txt file into g_Fortunes, overwriting current content:
	local idx = 1
	for line in io.lines(a_Plugin:GetLocalFolder() .. "/splashes.txt") do
		g_Fortunes[idx] = line
		idx = idx + 1
	end
	
	-- Use the InfoReg shared library to process the Info.lua file:
	dofile(cPluginManager:GetPluginsPath() .. "/InfoReg.lua")
	RegisterPluginInfoCommands()
	RegisterPluginInfoConsoleCommands()

	-- Seed the random generator:
	math.randomseed(os.time())
	
	return true
end





function HandleConsoleNetClient(a_Split)
	-- Get the address to connect to:
	local Host = a_Split[3] or "google.com"
	local Port = a_Split[4] or 80
	
	-- Create the callbacks "personalised" for the address:
	local Callbacks =
	{
		OnConnected = function (a_Link)
			LOG("Connected to " .. Host .. ":" .. Port .. ".")
			LOG("Connection stats: Remote address: " .. a_Link:GetRemoteIP() .. ":" .. a_Link:GetRemotePort() .. ", Local address: " .. a_Link:GetLocalIP() .. ":" .. a_Link:GetLocalPort())
			LOG("Sending HTTP request for front page.")
			a_Link:Send("GET / HTTP/1.0\r\nHost: " .. Host .. "\r\n\r\n")
		end,
		
		OnError = function (a_Link, a_ErrorCode, a_ErrorMsg)
			LOG("Connection to " .. Host .. ":" .. Port .. " failed: " .. a_ErrorCode .. " (" .. a_ErrorMsg .. ")")
		end,
		
		OnReceivedData = function (a_Link, a_Data)
			LOG("Received data from " .. Host .. ":" .. Port .. ":\r\n" .. a_Data)
		end,
		
		OnRemoteClosed = function (a_Link)
			LOG("Connection to " .. Host .. ":" .. Port .. " was closed by the remote peer.")
		end
	}
	
	-- Queue a connect request:
	local res = cNetwork:Connect(Host, Port, Callbacks)
	if not(res) then
		LOGWARNING("cNetwork:Connect call failed immediately")
		return true
	end
	
	return true, "Client connection request queued."
end





function HandleConsoleNetClose(a_Split)
	-- Get the port to close:
	local Port = tonumber(a_Split[3] or 1024)
	if not(Port) then
		return true, "Bad port number: \"" .. Port .. "\"."
	end

	-- Close the server, if there is one:
	if not(g_Servers[Port]) then
		return true, "There is no server currently listening on port " .. Port .. "."
	end
	g_Servers[Port]:Close()
	g_Servers[Port] = nil
	return true, "Port " .. Port .. " closed."
end





function HandleConsoleNetLookup(a_Split)
	-- Get the name to look up:
	local Addr = a_Split[3] or "google.com"
	
	-- Create the callbacks "personalised" for the host:
	local Callbacks =
	{
		OnNameResolved = function (a_Hostname, a_IP)
			LOG(a_Hostname .. " resolves to " .. a_IP)
		end,
		
		OnError = function (a_Query, a_ErrorCode, a_ErrorMsg)
			LOG("Failed to retrieve information for " .. a_Query .. ": " .. a_ErrorCode .. " (" .. a_ErrorMsg .. ")")
			assert(a_Query == Addr)
		end,
		
		OnFinished = function (a_Query)
			LOG("Resolving " .. a_Query .. " has finished.")
			assert(a_Query == Addr)
		end,
	}
	
	-- Queue both name and IP DNS queries;
	-- we don't distinguish between an IP and a hostname in this command so we don't know which one to use:
	local res = cNetwork:HostnameToIP(Addr, Callbacks)
	if not(res) then
		LOGWARNING("cNetwork:HostnameToIP call failed immediately")
		return true
	end
	res = cNetwork:IPToHostname(Addr, Callbacks)
	if not(res) then
		LOGWARNING("cNetwork:IPToHostname call failed immediately")
		return true
	end
	
	return true, "DNS query has been queued."
end





function HandleConsoleNetListen(a_Split)
	-- Get the params:
	local Port = tonumber(a_Split[3] or 1024)
	if not(Port) then
		return true, "Invalid port: \"" .. Port .. "\"."
	end
	local Service = string.lower(a_Split[4] or "echo")
	
	-- Create the callbacks specific for the service:
	if (g_Services[Service] == nil) then
		return true, "No such service: " .. Service
	end
	local Callbacks = g_Services[Service](Port)
	
	-- Start the server:
	local srv = cNetwork:Listen(Port, Callbacks)
	if not(srv:IsListening()) then
		-- The error message has already been printed in the Callbacks.OnError()
		return true
	end
	g_Servers[Port] = srv
	return true, Service .. " server started on port " .. Port
end




