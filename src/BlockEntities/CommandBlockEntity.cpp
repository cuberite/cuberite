
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
#include "../Entities/Player.h"





cCommandBlockEntity::cCommandBlockEntity(BlockState a_Block, Vector3i a_Pos, cWorld * a_World):
	Super(a_Block, a_Pos, a_World),
	m_ShouldExecute(false),
	m_Result(0)
{
	ASSERT((a_Block.Type() == BlockType::CommandBlock) || (a_Block.Type() == BlockType::ChainCommandBlock) || (a_Block.Type() == BlockType::RepeatingCommandBlock));
}





bool cCommandBlockEntity::UsedBy(cPlayer * a_Player)
{
	a_Player->GetClientHandle()->SendUpdateBlockEntity(*this);  // Necessary to send an empty block entity update to enable the command block GUI
	return true;
}





void cCommandBlockEntity::SetCommand(const AString & a_Cmd)
{
	m_Command = a_Cmd;
}





void cCommandBlockEntity::SetLastOutput(const AString & a_LastOut)
{
	m_LastOutput = a_LastOut;
}





void cCommandBlockEntity::SetResult(const unsigned char a_Result)
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





unsigned char cCommandBlockEntity::GetResult(void) const
{
	return m_Result;
}





void cCommandBlockEntity::Activate(void)
{
	m_ShouldExecute = true;
}





void cCommandBlockEntity::CopyFrom(const cBlockEntity & a_Src)
{
	Super::CopyFrom(a_Src);
	auto & src = static_cast<const cCommandBlockEntity &>(a_Src);
	m_Command = src.m_Command;
	m_LastOutput = src.m_LastOutput;
	m_Result = src.m_Result;
	m_ShouldExecute = src.m_ShouldExecute;
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

	if (m_Command.empty())
	{
		return;
	}

	class CommandBlockOutCb :
		public cLogCommandDeleteSelfOutputCallback
	{
		cCommandBlockEntity * m_CmdBlock;

	public:
		CommandBlockOutCb(cCommandBlockEntity * a_CmdBlock) : m_CmdBlock(a_CmdBlock) {}

		virtual void Out(const AString & a_Text) override
		{
			// Overwrite field
			m_CmdBlock->SetLastOutput(cClientHandle::FormatChatPrefix(m_CmdBlock->GetWorld()->ShouldUseChatPrefixes(), "SUCCESS", cChatColor::Green, cChatColor::White) + a_Text);
		}
	};

	AString RealCommand = m_Command;

	// Remove leading slash if it exists, since console commands don't use them
	if (RealCommand[0] == '/')
	{
		RealCommand = RealCommand.substr(1, RealCommand.length());
	}

	// Administrator commands are not executable by command blocks:
	if (
		(RealCommand != "stop") &&
		(RealCommand != "restart") &&
		(RealCommand != "kick") &&
		(RealCommand != "ban") &&
		(RealCommand != "ipban")
	)
	{
		cServer * Server = cRoot::Get()->GetServer();
		LOGD("cCommandBlockEntity: Executing command %s", m_Command.c_str());
		Server->QueueExecuteConsoleCommand(RealCommand, *new CommandBlockOutCb(this));
	}
	else
	{
		SetLastOutput(cClientHandle::FormatChatPrefix(GetWorld()->ShouldUseChatPrefixes(), "FAILURE", cChatColor::Rose, cChatColor::White) + "Adminstration commands can not be executed");
		LOGD("cCommandBlockEntity: Prevented execution of administration command %s", m_Command.c_str());
	}

	// TODO 2014-01-18 xdot: Update the signal strength.
	m_Result = 0;
}




