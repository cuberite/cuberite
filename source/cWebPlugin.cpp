#include "cWebPlugin.h"
#include "cWebAdmin.h"
#include "cServer.h"
#include "cMCLogger.h"
#include "cRoot.h"

#include "MemoryLeak.h"

cWebPlugin::cWebPlugin( lua_State* L )
{
	LOG("cWebPlugin::cWebPlugin()");
	m_LuaState = L;
	cWebAdmin* WebAdmin = cRoot::Get()->GetWebAdmin();
	if( WebAdmin ) WebAdmin->AddPlugin( this );
}

cWebPlugin::~cWebPlugin()
{
	LOG("~cWebPlugin::cWebPlugin()");
	cWebAdmin* WebAdmin = cRoot::Get()->GetWebAdmin();
	if( WebAdmin ) WebAdmin->RemovePlugin( this );
}