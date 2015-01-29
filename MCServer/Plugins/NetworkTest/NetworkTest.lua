
-- NetworkTest.lua

-- Implements a few tests for the cNetwork API





function Initialize()
	-- Use the InfoReg shared library to process the Info.lua file:
	dofile(cPluginManager:GetPluginsPath() .. "/InfoReg.lua")
	RegisterPluginInfoCommands()
	RegisterPluginInfoConsoleCommands()
	
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




