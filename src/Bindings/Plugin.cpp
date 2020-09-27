
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Plugin.h"





cPlugin::cPlugin(const AString & a_FolderName) :
	m_Status(cPluginManager::psDisabled),
	m_Name(a_FolderName),
	m_Version(0),
	m_FolderName(a_FolderName)
{
}





cPlugin::~cPlugin()
{
	LOGD("Destroying plugin \"%s\".", m_Name.c_str());
}





void cPlugin::Unload(void)
{
	auto pm = cPluginManager::Get();
	pm->RemovePluginCommands(this);
	pm->RemovePluginConsoleCommands(this);
	pm->RemoveHooks(this);
	OnDisable();
	m_Status = cPluginManager::psUnloaded;
	m_LoadError.clear();
}





AString cPlugin::GetLocalFolder(void) const
{
	return "Plugins" + cFile::GetPathSeparator() + m_FolderName;
}





void cPlugin::SetLoadError(const AString & a_LoadError)
{
	m_Status = cPluginManager::psError;
	m_LoadError = a_LoadError;
}




