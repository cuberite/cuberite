
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cWebPlugin.h"
#include "cWebAdmin.h"
#include "cServer.h"
#include "cRoot.h"





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