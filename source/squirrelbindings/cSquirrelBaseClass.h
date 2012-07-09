#pragma once
#include "SquirrelBindings.h"
#include "../cPlugin_Squirrel.h"
#include "../cPluginManager.h"
#include "../cRoot.h"

//The baseclass for squirrel plugins
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

protected:
	cPlugin_Squirrel *m_Instance;
};