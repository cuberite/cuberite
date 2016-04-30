
// CommandBlockEntity.cpp

// Implements the cCommandBlockEntity class representing a single command block in the world

#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules
#include "CommandBlockEntity.h"

#include "../CommandOutput.h"
#include "../Root.h"
#include "../Server.h"  // ExecuteConsoleCommand()
#include "../ChatColor.h"
#include "../World.h"
#include "../ClientHandle.h"





cCommandBlockEntity::cCommandBlockEntity(int a_X, int a_Y, int a_Z, cWorld * a_World) :
	super(E_BLOCK_COMMAND_BLOCK, a_X, a_Y, a_Z, a_World),
	m_ShouldExecute(false),
	m_Result(0)
{}






bool cCommandBlockEntity::UsedBy(cPlayer * a_Player)
{
	// Nothing to do
	UNUSED(a_Player);
	return true;
}





void cCommandBlockEntity::SetCommand(const AString & a_Cmd)
{
	m_Command = a_Cmd;

	/*
	Vanilla requires that the server send a Block Entity Update after a command has been set
	Therefore, command blocks don't support on-the-fly (when window is open) updating of a command and therefore...
	...the following code can't be put in UsedBy just before the window opens

	Just documenting my experience in getting this to work :P
	*/
	m_World->BroadcastBlockEntity(GetPosX(), GetPosY(), GetPosZ());
}





void cCommandBlockEntity::SetLastOutput(const AString & a_LastOut)
{
	m_World->BroadcastBlockEntity(GetPosX(), GetPosY(), GetPosZ());
	m_LastOutput = a_LastOut;
}





void cCommandBlockEntity::SetResult(const NIBBLETYPE a_Result)
{
	m_Result = a_Result;
}





const AString & cCommandBlockEntity::GetCommand(void) const
{
	return m_Command;
}





const AString & cCommandBlockEntity::GetLastOutput(void) const
{
	return m_LastOutput;
}





NIBBLETYPE cCommandBlockEntity::GetResult(void) const
{
	return m_Result;
}





void cCommandBlockEntity::Activate(void)
{
	m_ShouldExecute = true;
}





bool cCommandBlockEntity::Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	UNUSED(a_Dt);
	UNUSED(a_Chunk);
	if (!m_ShouldExecute)
	{
		return false;
	}

	m_ShouldExecute = false;
	Execute();
	return true;
}





void cCommandBlockEntity::SendTo(cClientHandle & a_Client)
{
	a_Client.SendUpdateBlockEntity(*this);
}





void cCommandBlockEntity::Execute()
{
	ASSERT(m_World != nullptr);  // Execute should not be called before the command block is attached to a world

	if (!m_World->AreCommandBlocksEnabled())
	{
		return;
	}

	class CommandBlockOutCb :
		public cCommandOutputCallback
	{
		cCommandBlockEntity * m_CmdBlock;

	public:
		CommandBlockOutCb(cCommandBlockEntity * a_CmdBlock) : m_CmdBlock(a_CmdBlock) {}

		virtual void Out(const AString & a_Text)
		{
			// Overwrite field
			m_CmdBlock->SetLastOutput(cClientHandle::FormatChatPrefix(m_CmdBlock->GetWorld()->ShouldUseChatPrefixes(), "SUCCESS", cChatColor::Green, cChatColor::White) + a_Text);
		}
	} CmdBlockOutCb(this);

	// Administrator commands are not executable by command blocks:
	if (
		(m_Command != "stop") &&
		(m_Command != "restart") &&
		(m_Command != "kick") &&
		(m_Command != "ban") &&
		(m_Command != "ipban")
	)
	{
		auto & Server = cRoot::Get()->GetServer();
		LOGD("cCommandBlockEntity: Executing command %s", m_Command.c_str());
		Server.ExecuteConsoleCommand(m_Command, CmdBlockOutCb);
	}
	else
	{
		SetLastOutput(cClientHandle::FormatChatPrefix(GetWorld()->ShouldUseChatPrefixes(), "FAILURE", cChatColor::Rose, cChatColor::White) + "Adminstration commands can not be executed");
		LOGD("cCommandBlockEntity: Prevented execution of administration command %s", m_Command.c_str());
	}

	// TODO 2014-01-18 xdot: Update the signal strength.
	m_Result = 0;
}




