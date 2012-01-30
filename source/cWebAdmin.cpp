
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cWebAdmin.h"
#include "cStringMap.h"

#include "cWebPlugin.h"
#include "cWebPlugin_Lua.h"

#include "cPluginManager.h"
#include "cPlugin.h"

#include "cWorld.h"
#include "cPlayer.h"
#include "cServer.h"
#include "cRoot.h"

#include "../iniFile/iniFile.h"

#ifdef _WIN32
	#include <psapi.h>
#else
	#include <sys/resource.h>
#endif





extern std::vector<std::string> StringSplit(std::string str, std::string delim);

cWebAdmin* WebAdmin = 0;

cWebAdmin::cWebAdmin( int a_Port /* = 8080 */ )
	: m_Port( a_Port )
	, m_bConnected( false )
{
	WebAdmin = this;
	m_Event = new cEvent();
	Init( m_Port );
}

cWebAdmin::~cWebAdmin()
{
	WebAdmin = 0;
	m_WebServer->Stop();

	while( m_Plugins.begin() != m_Plugins.end() )
	{
		delete *m_Plugins.begin();
		//m_Plugins.remove( *m_Plugins.begin() );
	}
	delete m_WebServer;
	delete m_IniFile;

	m_Event->Wait();
	delete m_Event;
}

void ReplaceString( std::string & a_HayStack, const std::string & a_Needle, const std::string & a_ReplaceWith )
{
	size_t pos1 = a_HayStack.find( a_Needle );
	a_HayStack.replace( pos1, a_Needle.size(), a_ReplaceWith );
}

void cWebAdmin::AddPlugin( cWebPlugin* a_Plugin )
{
	m_Plugins.remove( a_Plugin );
	m_Plugins.push_back( a_Plugin );
}

void cWebAdmin::RemovePlugin( cWebPlugin* a_Plugin )
{
	m_Plugins.remove( a_Plugin );
}

void cWebAdmin::Request_Handler(webserver::http_request* r)
{
	if( WebAdmin == 0 ) return;
	LOG("Path: %s", r->path_.c_str() );

	std::vector< std::string > Split = StringSplit( r->path_, "/" );

	if(r->path_ == "/")
	{
		r->answer_ += "<center>";
		r->answer_ += "MCServer WebAdmin";
		r->answer_ += "<br>";
		r->answer_ += "<form method='get' action='webadmin/'>";
		r->answer_ += "<input type='submit' value='Log in'>";
		r->answer_ += "</form>";
		r->answer_ += "</center>";
		return;
	}
	else if( Split.size() > 0 && Split[0] == "webadmin" )
	{
		if( r->authentication_given_ )
		{
			std::string UserPassword = WebAdmin->m_IniFile->GetValue( "User:"+r->username_, "Password", "");
			if (UserPassword != "" && r->password_ == UserPassword)
			{
				std::string BaseURL = "./";
				if( Split.size() > 1 )
				{
					for( unsigned int i = 0; i < Split.size(); i++)
					{
						BaseURL += "../";
					}
					BaseURL += "webadmin/";
				}

				std::string Menu;
				std::string Content;
				std::string Template = WebAdmin->GetTemplate();
				std::string FoundPlugin;

				for( PluginList::iterator itr = WebAdmin->m_Plugins.begin(); itr != WebAdmin->m_Plugins.end(); ++itr )
				{
					cWebPlugin* WebPlugin = *itr;
					cWebPlugin_Lua* LuaPlugin = dynamic_cast< cWebPlugin_Lua* >( WebPlugin );
					if( LuaPlugin )
					{
						std::list< std::string > NameList = LuaPlugin->GetTabNames();
						for( std::list< std::string >::iterator Name = NameList.begin(); Name != NameList.end(); ++Name )
						{
							Menu += "<li><a href='" + BaseURL + WebPlugin->GetName() + "/" + (*Name) + "'>" + (*Name) + "</a></li>";
						}
					}
					else
					{
						Menu += "<li><a href='" + BaseURL + WebPlugin->GetName() + "'>" + WebPlugin->GetName() + "</a></li>";
					}
				}

				HTTPRequest Request;
				Request.Username = r->username_;
				Request.Method = r->method_;
				Request.Params = new cStringMap(r->params_);
				Request.Path = r->path_;

				if( Split.size() > 1 )
				{
					for( PluginList::iterator itr = WebAdmin->m_Plugins.begin(); itr != WebAdmin->m_Plugins.end(); ++itr )
					{
						if( (*itr)->GetName() == Split[1] )
						{
							Content = (*itr)->HandleRequest( &Request );
							cWebPlugin* WebPlugin = *itr;
							FoundPlugin = WebPlugin->GetName();
							cWebPlugin_Lua* LuaPlugin = dynamic_cast< cWebPlugin_Lua* >( WebPlugin );
							if( LuaPlugin )
							{
								FoundPlugin += " - " + LuaPlugin->GetTabNameForRequest( &Request );
							}
							break;
						}
					}
				}

				delete Request.Params;

				if( FoundPlugin.empty() )	// Default page
				{
					Content.clear();
					FoundPlugin = "Current Game";
					Content += "<h4>Server Name:</h4>";
					Content += "<p>" + std::string( cRoot::Get()->GetServer()->GetServerID() ) + "</p>";

					Content += "<h4>Plugins:</h4><ul>";
					cPluginManager* PM = cRoot::Get()->GetPluginManager();
					if( PM )
					{
						const cPluginManager::PluginList & List = PM->GetAllPlugins();
						for( cPluginManager::PluginList::const_iterator itr = List.begin(); itr != List.end(); ++itr )
						{
							char c_VersionNum[32]; // 32 digits should be enough? XD
							sprintf_s( c_VersionNum, 32, "%i", (*itr)->GetVersion() );
							Content += std::string("<li>") + std::string( (*itr)->GetName() ) + " V. " + std::string( c_VersionNum ) + "</li>";
							
						}
					}
					Content += "</ul>";
					Content += "<h4>Players:</h4><ul>";

					cWorld* World = cRoot::Get()->GetWorld(); // TODO - Create a list of worlds and players
					cWorld::PlayerList PlayerList = World->GetAllPlayers();
					for( cWorld::PlayerList::iterator itr = PlayerList.begin(); itr != PlayerList.end(); ++itr )
					{
						Content += std::string("<li>") + std::string( (*itr)->GetName() ) + "</li>";
					}
					Content += "</ul><br>";
				}

				

				if( Split.size() > 1 )
				{
					Content += "\n<p><a href='" + BaseURL + "'>Go back</a></p>";
				}

				// mem usage
#ifndef _WIN32
				rusage resource_usage;
				if (getrusage(RUSAGE_SELF, &resource_usage) != 0)
				{
					ReplaceString( Template, std::string("{MEM}"), "Error :(" );
				}
				else
				{
					char MemUsage[32];
					sprintf( MemUsage, "%0.2f", ((double)resource_usage.ru_maxrss / 1024 / 1024) );
					ReplaceString( Template, std::string("{MEM}"), MemUsage );
				}
#else
				HANDLE hProcess = GetCurrentProcess();
				PROCESS_MEMORY_COUNTERS pmc;
				if( GetProcessMemoryInfo( hProcess, &pmc, sizeof(pmc) ) )
				{
					char MemUsage[32];
					sprintf( MemUsage, "%0.2f", (pmc.WorkingSetSize / 1024.f / 1024.f) );
					ReplaceString( Template, std::string("{MEM}"), MemUsage );
				}
#endif
				// end mem usage

				ReplaceString( Template, std::string("{USERNAME}"), r->username_ );
				ReplaceString( Template, std::string("{MENU}"), Menu );
				ReplaceString( Template, std::string("{PLUGIN_NAME}"), FoundPlugin );
				ReplaceString( Template, std::string("{CONTENT}"), Content );
				ReplaceString( Template, std::string("{TITLE}"), "MCServer" );

				r->answer_ = Template;
			}
			else
			{
				r->answer_ += "Wrong username/password";
				r->auth_realm_ = "MCServer WebAdmin";
			}
		}
		else
		{
			r->answer_ += "no auth";
			r->auth_realm_ = "MCServer WebAdmin";
		}
	}
}





bool cWebAdmin::Init( int a_Port )
{
	m_Port = a_Port;

	m_IniFile = new cIniFile("webadmin.ini");
	if( m_IniFile->ReadFile() )
	{
		m_Port = m_IniFile->GetValueI("WebAdmin", "Port", 8080 );
	}

	LOG("Starting WebAdmin on port %i", m_Port);

#ifdef _WIN32
	HANDLE hThread = CreateThread(
		NULL,              // default security
		0,                 // default stack size
		ListenThread,   // name of the thread function
		this,	                // thread parameters
		0,                 // default startup flags
		NULL);
	CloseHandle( hThread ); // Just close the handle immediately
#else
	pthread_t LstnThread;
	pthread_create( &LstnThread, 0, ListenThread, this);
#endif

	return true;
}

#ifdef _WIN32
DWORD WINAPI cWebAdmin::ListenThread(LPVOID lpParam)
#else
void *cWebAdmin::ListenThread( void *lpParam )
#endif
{
	cWebAdmin* self = (cWebAdmin*)lpParam;

	self->m_WebServer = new webserver(self->m_Port, Request_Handler );
	self->m_WebServer->Begin();

	self->m_Event->Set();
	return 0;
}





std::string cWebAdmin::GetTemplate()
{
	std::string retVal = "";

	char SourceFile[] = "webadmin/template.html";

	cFile f;
	if (!f.Open(SourceFile, cFile::fmRead))
	{
		return "";
	}

	// obtain file size:
	int lSize = f.GetSize();

	// allocate memory to contain the whole file:
	std::auto_ptr<char> buffer(new char[lSize]);  // auto_ptr deletes the memory in its destructor

	// copy the file into the buffer:
	if (f.Read(buffer.get(), lSize) != lSize)
	{
		LOG ("WEBADMIN: Could not read file \"%s\"", SourceFile);
		return "";
	}

	retVal.assign(buffer.get(), lSize );

	return retVal;
}





void cWebAdmin::RemovePlugin( lua_State* L )
{
	for( PluginList::iterator itr = m_Plugins.begin(); itr != m_Plugins.end(); )
	{
		if( (*itr)->GetLuaState() == L )
		{
			PluginList::iterator prev = itr++;
			delete *prev; // deleting a dereferenced iterator also takes it out of the list, so no need for erase()
		}
		else
			++itr;
	}
}
