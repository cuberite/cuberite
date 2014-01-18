
// CommandBlockEntity.cpp

// Implements the cCommandBlockEntity class representing a single command block in the world

#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules
#include "json/json.h"
#include "CommandBlockEntity.h"
#include "../Entities/Player.h"





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
		//OpenWindow(new cDropSpenserWindow(m_PosX, m_PosY, m_PosZ, this));
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
	m_Command = a_Cmd;LOGD("Hrey %s", a_Cmd.c_str());
}





const AString & cCommandBlockEntity::GetCommand(void) const
{
	return m_Command;
}





const AString & cCommandBlockEntity::GetLastOutput(void) const
{
	return m_LastOutput;
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
	// TODO: Parse arguments and dispatch command
	LOGD("Command: %s", m_Command.c_str());
	m_LastOutput = "";
}




