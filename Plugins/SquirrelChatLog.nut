class SquirrelChatLog extends cPlugin__Squirrel
{
	constructor()
	{
		base.constructor();
	}
	
	function Initialize()
	{
		::print("SquirrelChatLog initialize()");
		this.SetName("SquirrelChatLog");
		
		local PluginManager = cRoot.Get().GetPluginManager();
		PluginManager.AddHook( this, PluginManager.E_PLUGIN_CHAT );
		return true;
	}

	function OnChat( Message, Player )
	{
		::print(Player.GetName() + ": " + Message);
	}
}


Plugin <- SquirrelChatLog();
cRoot.Get().GetPluginManager().AddPlugin( Plugin );
