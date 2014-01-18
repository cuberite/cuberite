
// CommandBlockEntity.cpp

// Implements the cCommandBlockEntity class representing a single command block in the world

#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules
#include "json/json.h"
#include "CommandBlockEntity.h"
#include "../Entities/Player.h"

#include "CommandOutput.h"
#include "Root.h"
#include "Server.h" // ExecuteConsoleCommand()





cCommandBlockEntity::cCommandBlockEntity(int a_X, int a_Y, int a_Z, cWorld * a_World) :
	super(E_BLOCK_COMMAND_BLOCK, a_X, a_Y, a_Z, a_World),
	m_ShouldExecute(false),
	m_IsPowered(false)
{
	SetBlockEntity(this); // cBlockEntityWindowOwner
}






cCommandBlockEntity::~cCommandBlockEntity()
{
	// Tell window its owner is destroyed
	cWindow * Window = GetWindow();
	if (Window != NULL)
	{
		Window->OwnerDestroyed();
	}
}






void cCommandBlockEntity::UsedBy(cPlayer * a_Player)
{
	cWindow * Window = GetWindow();
	if (Window == NULL)
	{
		// TODO 2014-01-18 xdot: Open the appropriate window.
		// OpenWindow(new cCommandBlockWindow(m_PosX, m_PosY, m_PosZ, this));
		Window = GetWindow();
	}
	
	if (Window != NULL)
	{
		if (a_Player->GetWindow() != Window)
		{
			a_Player->OpenWindow(Window);
		}
	}
}





void cCommandBlockEntity::SetCommand(const AString & a_Cmd)
{
	m_Command = a_Cmd;
}





void cCommandBlockEntity::SetLastOutput(const AString & a_LastOut)
{
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





void cCommandBlockEntity::SetRedstonePower(bool a_IsPowered)
{
	if (a_IsPowered && !m_IsPowered)
	{
		Activate();
	}
	m_IsPowered = a_IsPowered;
}





bool cCommandBlockEntity::Tick(float a_Dt, cChunk & a_Chunk)
{
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
	// Nothing needs to be sent
	UNUSED(a_Client);
}





bool cCommandBlockEntity::LoadFromJson(const Json::Value & a_Value)
{
	m_Command = a_Value.get("Command", "").asString();

	m_LastOutput = a_Value.get("LastOutput", "").asString();

	return true;
}





void cCommandBlockEntity::SaveToJson(Json::Value & a_Value)
{
	a_Value["Command"] = m_Command;

	a_Value["LastOutput"] = m_LastOutput;
}





void cCommandBlockEntity::Execute()
{
	class CommandBlockOutCb :
		public cCommandOutputCallback
	{
		cCommandBlockEntity* m_CmdBlock;

	public:
		CommandBlockOutCb(cCommandBlockEntity* a_CmdBlock) : m_CmdBlock(a_CmdBlock) {}

		virtual void Out(const AString & a_Text)
		{
			ASSERT(m_CmdBlock != NULL);

			// Overwrite field
			m_CmdBlock->SetLastOutput(a_Text);
		}
	} CmdBlockOutCb(this);

	LOGD("cCommandBlockEntity: Executing command %s", m_Command.c_str());

	cServer* Server = cRoot::Get()->GetServer();

	Server->ExecuteConsoleCommand(m_Command, CmdBlockOutCb);

	// TODO 2014-01-18 xdot: Update the signal strength.
	m_Result = 0;
}




