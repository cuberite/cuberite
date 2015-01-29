
// LuaNameLookup.cpp

// Implements the cLuaNameLookup class used as the cNetwork API callbacks for name and IP lookups from Lua

#include "Globals.h"
#include "LuaNameLookup.h"





cLuaNameLookup::cLuaNameLookup(const AString & a_Query, cPluginLua & a_Plugin, int a_CallbacksTableStackPos):
	m_Plugin(a_Plugin),
	m_Callbacks(a_Plugin.GetLuaState(), a_CallbacksTableStackPos),
	m_Query(a_Query)
{
}





void cLuaNameLookup::OnNameResolved(const AString & a_Name, const AString & a_IP)
{
	// Check if we're still valid:
	if (!m_Callbacks.IsValid())
	{
		return;
	}

	// Call the callback:
	cPluginLua::cOperation Op(m_Plugin);
	if (!Op().Call(cLuaState::cTableRef(m_Callbacks, "OnNameResolved"), a_Name, a_IP))
	{
		LOGINFO("cNetwork name lookup OnNameResolved callback failed in plugin %s looking up %s. %s resolves to %s.",
			m_Plugin.GetName().c_str(), m_Query.c_str(), a_Name.c_str(), a_IP.c_str()
		);
	}
}





void cLuaNameLookup::OnError(int a_ErrorCode, const AString & a_ErrorMsg)
{
	// Check if we're still valid:
	if (!m_Callbacks.IsValid())
	{
		return;
	}

	// Call the callback:
	cPluginLua::cOperation Op(m_Plugin);
	if (!Op().Call(cLuaState::cTableRef(m_Callbacks, "OnError"), m_Query, a_ErrorCode, a_ErrorMsg))
	{
		LOGINFO("cNetwork name lookup OnError callback failed in plugin %s looking up %s. The error is %d (%s)",
			m_Plugin.GetName().c_str(), m_Query.c_str(), a_ErrorCode, a_ErrorMsg.c_str()
		);
	}
}





void cLuaNameLookup::OnFinished(void)
{
	// Check if we're still valid:
	if (!m_Callbacks.IsValid())
	{
		return;
	}

	// Call the callback:
	cPluginLua::cOperation Op(m_Plugin);
	if (!Op().Call(cLuaState::cTableRef(m_Callbacks, "OnFinished"), m_Query))
	{
		LOGINFO("cNetwork name lookup OnFinished callback failed in plugin %s, looking up %s.",
			m_Plugin.GetName().c_str(), m_Query.c_str()
		);
	}
}




