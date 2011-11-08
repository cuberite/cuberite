#include "cWebAdmin.h"
#include "cMCLogger.h"
#include "cStringMap.h"

#include "cWebPlugin.h"

#include "cPluginManager.h"
#include "cPlugin.h"

#include "cEvent.h"
#include "cWorld.h"
#include "cPlayer.h"
#include "cServer.h"
#include "cRoot.h"

#include <vector>

#include "../iniFile/iniFile.h"

#include "MemoryLeak.h"
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
				std::string Menu;
				std::string Content;
				std::string Template = WebAdmin->GetTemplate();

				Content += "<h3>Current Game</h3>";
				Content += "<h4>Server Name:</h4>";
				Content += "<p>" + std::string( cRoot::Get()->GetServer()->GetServerID() ) + "</p>";

				Content += "<h4>Plugins:</h4><p>";
				cPluginManager* PM = cRoot::Get()->GetPluginManager();
				const cPluginManager::PluginList & List = PM->GetAllPlugins();
				for( cPluginManager::PluginList::const_iterator itr = List.begin(); itr != List.end(); ++itr )
				{
					Content += std::string( (*itr)->GetName() ) + "<br>";
				}
				Content += "</p>";
				Content += "<h4>Players:</h4><p>";

				cWorld* World = cRoot::Get()->GetWorld(); // TODO - Create a list of worlds and players
				cWorld::PlayerList PlayerList = World->GetAllPlayers();
				for( cWorld::PlayerList::iterator itr = PlayerList.begin(); itr != PlayerList.end(); ++itr )
				{
					Content += std::string( (*itr)->GetName() ) + "<br>";
				}
				Content += "</p>";

				for( PluginList::iterator itr = WebAdmin->m_Plugins.begin(); itr != WebAdmin->m_Plugins.end(); ++itr )
				{
					Menu += "<li><a href='" + (*itr)->GetName() + "'>" + (*itr)->GetName() + "</a></li>";
				}

				HTTPRequest Request;
				Request.Username = r->username_;
				Request.Method = r->method_;
				Request.Params = new cStringMap(r->params_);
				Request.Path = r->path_;

				if( Split.size() > 1 )
				{
					std::string FoundPlugin = "";
					for( PluginList::iterator itr = WebAdmin->m_Plugins.begin(); itr != WebAdmin->m_Plugins.end(); ++itr )
					{
						if( (*itr)->GetName() == Split[1] )
						{
							Content = (*itr)->HandleRequest( &Request );
							FoundPlugin = (*itr)->GetName();
							break;
						}
					}

					if( FoundPlugin.compare("") != 0 )	// Add some header
					{
						Content = "<h3>" + FoundPlugin + "</h3>\n<p>" + Content + "</p>";
					}
				}

				delete Request.Params;

				if( Split.size() > 1 )
				{
					Content += "\n<p><a href='";
					for( unsigned int i = 0; i < Split.size(); i++)
					{
						Content += "../";
					}
					Content += "webadmin/";
					Content += "'>Go back</a></p>";
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

	FILE* f;
#ifdef _WIN32
	if( fopen_s(&f, SourceFile, "rb" ) == 0 )	// no error
#else
	if( (f = fopen(SourceFile, "rb" ) ) != 0 )	// no error
#endif
	{
		// obtain file size:
		fseek (f , 0 , SEEK_END);
		long lSize = ftell (f);
		rewind (f);

		// allocate memory to contain the whole file:
		char* buffer = (char*) malloc (sizeof(char)*lSize);

		// copy the file into the buffer:
		size_t result = fread (buffer, 1, lSize, f);
		if ((long)result != lSize)
		{
			LOG ("WEBADMIN: Could not read file %s", SourceFile);
			free( buffer );
			return "";
		}

		retVal.assign( buffer, lSize );

		free( buffer );
		fclose(f);
	}
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
