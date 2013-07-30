
-- Global variables
PLUGIN = {};	-- Reference to own plugin object

function Initialize(Plugin)
	PLUGIN = Plugin
	
	Plugin:SetName("DumpAPI")
	Plugin:SetVersion(1)
	
	PluginManager = cRoot:Get():GetPluginManager()
	LOG("Initialized " .. Plugin:GetName() .. " v." .. Plugin:GetVersion())

	-- dump all available API functions and objects:
	DumpAPI();

	return true
end

function DumpAPI()
	LOG("Dumping all available functions to API.txt...");
	function dump (prefix, a, Output)
		for i, v in pairs (a) do
			if (type(v) == "table") then
				if (GetChar(i, 1) ~= ".") then
					if (v == _G) then
						LOG(prefix .. i .. " == _G, CYCLE, ignoring");
					elseif (v == _G.package) then
						LOG(prefix .. i .. " == _G.package, ignoring");
					else
						dump(prefix .. i .. ".", v, Output)
					end
				end
			elseif (type(v) == "function") then
				if (string.sub(i, 1, 2) ~= "__") then
					table.insert(Output, prefix .. i .. "()");
				end
			end
		end
	end

	local Output = {};
	dump("", _G, Output);

	table.sort(Output);
	local f = io.open("API.txt", "w");
	for i, n in ipairs(Output) do
		f:write(n, "\n");
	end
	f:close();
	LOG("API.txt written.");
end
