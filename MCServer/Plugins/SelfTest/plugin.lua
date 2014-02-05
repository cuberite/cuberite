
function Initialize(Plugin)
	Plugin:SetName("SelfTest")
	Plugin:SetVersion(1)

	cPluginManager.AddHook(cPluginManager.HOOK_WORLD_STARTED, OnWorldStarted)

	LOG("Initialized " .. Plugin:GetName() .. " v." .. Plugin:GetVersion())
	return true
end





function OnWorldStarted(World)
	LOG("Stopping")
	cRoot:Get():QueueExecuteConsoleCommand("stop");
	return false
end
