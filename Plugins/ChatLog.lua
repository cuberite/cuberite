local ChatLogPlugin = {}
ChatLogPlugin.__index = ChatLogPlugin

function ChatLogPlugin:new()
   local t = {}
   setmetatable(t, ChatLogPlugin)
   local w = Lua__cPlugin:new()
   tolua.setpeer(w, t)
   w:tolua__set_instance(w)
   return w
end

function ChatLogPlugin:OnDisable()
	Log( self:GetName() .. " v." .. self:GetVersion() .. " is shutting down..." )
end

function ChatLogPlugin:Initialize()
	self:SetName( "ChatLog" )
	self:SetVersion( 1 )

	PluginManager = cRoot:Get():GetPluginManager()
	PluginManager:AddHook( self, cPluginManager.E_PLUGIN_CHAT )

	self.Logger = cMCLogger:new_local("ChatLog"..GetTime()..".txt")
	self.Logger:LogSimple("--- ChatLog started ---", 1);

	Log( "Initialized " .. self:GetName() .. " v." .. self:GetVersion() )
	return true
end

function ChatLogPlugin:OnChat( Message, Player )
	Server = cRoot:Get():GetServer()

	-- Lets get loggin'
	self.Logger:LogSimple(Player:GetName() .. ": " .. Message, 1);

	return false
end

Plugin = ChatLogPlugin:new()
cRoot:Get():GetPluginManager():AddPlugin( Plugin )
