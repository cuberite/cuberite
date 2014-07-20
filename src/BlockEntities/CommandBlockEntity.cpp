
// CommandBlockEntity.cpp

// Implements the cCommandBlockEntity class representing a single command block in the world

#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules
#include "json/json.h"
#include "CommandBlockEntity.h"
#include "../Entities/Player.h"
#include "../WorldStorage/FastNBT.h"

#include "../CommandOutput.h"
#include "../Root.h"
#include "../Server.h"  // ExecuteConsoleCommand()
#include "../Chunk.h"





cCommandBlockEntity::cCommandBlockEntity(int a_X, int a_Y, int a_Z, cWorld * a_World) :
	super(E_BLOCK_COMMAND_BLOCK, a_X, a_Y, a_Z, a_World),
	m_ShouldExecute(false),
	m_IsPowered(false),
	m_Result(0)
{}






void cCommandBlockEntity::UsedBy(cPlayer * a_Player)
{
	// Nothing to do
	UNUSED(a_Player);
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





bool cCommandBlockEntity::LoadFromJson(const Json::Value & a_Value)
{
	m_PosX = a_Value.get("x", 0).asInt();
	m_PosY = a_Value.get("y", 0).asInt();
	m_PosZ = a_Value.get("z", 0).asInt();

	m_Command    = a_Value.get("Command",     "").asString();
	m_LastOutput = a_Value.get("LastOutput",  "").asString();
	m_Result     = (NIBBLETYPE)a_Value.get("SuccessCount", 0).asInt();

	return true;
}





void cCommandBlockEntity::SaveToJson(Json::Value & a_Value)
{
	a_Value["x"] = m_PosX;
	a_Value["y"] = m_PosY;
	a_Value["z"] = m_PosZ;

	a_Value["Command"]      = m_Command;
	a_Value["LastOutput"]   = m_LastOutput;
	a_Value["SuccessCount"] = m_Result;
}





void cCommandBlockEntity::Execute()
{
	if (m_World != NULL)
	{
		if (!m_World->AreCommandBlocksEnabled())
		{
			return;
		}
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
			m_CmdBlock->SetLastOutput(a_Text);
		}
	} CmdBlockOutCb(this);

	LOGD("cCommandBlockEntity: Executing command %s", m_Command.c_str());

	cServer * Server = cRoot::Get()->GetServer();

	Server->ExecuteConsoleCommand(m_Command, CmdBlockOutCb);

	// TODO 2014-01-18 xdot: Update the signal strength.
	m_Result = 0;
}




