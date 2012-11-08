
#pragma once




#ifdef USE_SQUIRREL





#include "SquirrelBindings.h"
#include "../Plugin_Squirrel.h"
#include "../PluginManager.h"
#include "../Root.h"
#include "../SquirrelCommandBinder.h"





// The baseclass for squirrel plugins
class cSquirrelBaseClass
{
public:
	cSquirrelBaseClass()
		: m_Instance(0)
	{
	}

	void setInstance(cPlugin_Squirrel *a_Instance)
	{
		m_Instance = a_Instance;
	}
	
	void AddHook(short a_Hook)
	{
		if(m_Instance)
			cRoot::Get()->GetPluginManager()->AddHook(m_Instance, (cPluginManager::PluginHook) a_Hook);
	}

	void AddCommand( std::string a_Command, std::string a_Description, std::string a_Permission )
	{
		if(m_Instance) m_Instance->AddCommand(a_Command, a_Description, a_Permission);
	}

	bool BindCommand( const std::string a_Command, const std::string a_Permission, Sqrat::Function a_Callback)
	{
		if(!m_Instance) return false;
		return cRoot::Get()->GetPluginManager()->GetSquirrelCommandBinder()->BindCommand(a_Command, a_Permission, m_Instance, a_Callback);
	}

protected:
	cPlugin_Squirrel *m_Instance;
};





#endif  // USE_SQUIRREL




