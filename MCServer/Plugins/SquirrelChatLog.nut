class SquirrelChatLog extends Plugin
{		
	function Initialize()
	{		
		this.AddHook(Hook.Chat);
		return true;
	}

	function OnChat(Message, Player)
	{
		::print(Player.GetName() + ": " + Message);
	}
}
