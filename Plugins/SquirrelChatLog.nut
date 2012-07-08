class SquirrelChatLog extends Plugin
{
	name = "SquirrelChatLogger";
		
	function Initialize()
	{
		::print("SquirrelChatLog initialize()");
		
		this.AddHook(Hook.Chat);
		
		return true;
	}

	function OnChat( Message, Player )
	{
		::print("CHAT");
		::print(Player.GetName() + ": " + Message);
		
	}
}
