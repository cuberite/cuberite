

local PLUGIN_NAME = "MultiThreadTest";



function Initialize(a_Plugin)
	a_Plugin:SetName(PLUGIN_NAME)
	cPluginManager:BindConsoleCommand("startthread", HandleStartThreadCommand, "");
	LOG("Initialized");
	
	return true;
end




function Callback()
	cRoot:Get():GetDefaultWorld():QueueTask(function()
		print("From world thread");
	end);
end




function HandleStartThreadCommand(a_Split)
	cThread.new(function()
		print("Test");
		for i = 1, 5 do
			cThread.sleep(1);
			print("Testing", i);
			
		end
		-- Really ugly way to return to the default lua_State
		cPluginManager:CallPlugin(PLUGIN_NAME, "Callback");
	end)
	
	return true;
end
