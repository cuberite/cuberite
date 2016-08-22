
// LuaNameLookup.cpp

// Implements the cLuaNameLookup class used as the cNetwork API callbacks for name and IP lookups from Lua

#include "Globals.h"
#include "LuaNameLookup.h"





cLuaNameLookup::cLuaNameLookup(const AString & a_Query, cLuaState::cTableRefPtr && a_Callbacks):
	m_Callbacks(std::move(a_Callbacks)),
	m_Query(a_Query)
{
}





void cLuaNameLookup::OnNameResolved(const AString & a_Name, const AString & a_IP)
{
	m_Callbacks->CallTableFn("OnNameResolved", a_Name, a_IP);
}





void cLuaNameLookup::OnError(int a_ErrorCode, const AString & a_ErrorMsg)
{
	m_Callbacks->CallTableFn("OnError", m_Query, a_ErrorCode, a_ErrorMsg);
}





void cLuaNameLookup::OnFinished(void)
{
	m_Callbacks->CallTableFn("OnFinished", m_Query);
}




