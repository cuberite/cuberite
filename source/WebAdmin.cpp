
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "WebAdmin.h"
#include "StringMap.h"

#include "WebPlugin.h"

#include "PluginManager.h"
#include "Plugin.h"

#include "World.h"
#include "Entities/Player.h"
#include "Server.h"
#include "Root.h"

#include "../iniFile/iniFile.h"

#ifdef _WIN32
	#include <psapi.h>
#elif defined(__linux__)
	#include <fstream>
#elif defined(__APPLE__)
	#include <mach/mach.h>
#endif





/// Helper class - appends all player names together in a HTML list
class cPlayerAccum :
	public cPlayerListCallback
{
	virtual bool Item(cPlayer * a_Player) override
	{
		m_Contents.append("<li>");
		m_Contents.append(a_Player->GetName());
		m_Contents.append("</li>");
		return false;
	}
	
public:

	AString m_Contents;
} ;





cWebAdmin * WebAdmin = NULL;





cWebAdmin::cWebAdmin( int a_Port /* = 8080 */ ) :
	m_Port(a_Port),
	m_bConnected(false),
	m_TemplateScript("<webadmin_template>")
{
	WebAdmin = this;
	m_Event = new cEvent();
	Init( m_Port );
}





cWebAdmin::~cWebAdmin()
{

	WebAdmin = 0;
	m_WebServer->Stop();

	delete m_WebServer;
	delete m_IniFile;

	m_Event->Wait();
	delete m_Event;
}





void cWebAdmin::AddPlugin( cWebPlugin * a_Plugin )
{
	m_Plugins.remove( a_Plugin );
	m_Plugins.push_back( a_Plugin );
}





void cWebAdmin::RemovePlugin( cWebPlugin * a_Plugin )
{
	m_Plugins.remove( a_Plugin );
}





void cWebAdmin::Request_Handler(webserver::http_request* r)
{
	if( WebAdmin == 0 ) return;
	LOG("Path: %s", r->path_.c_str() );

	if (r->path_ == "/")
	{
		r->answer_ += "<h1>MCServer WebAdmin</h1>";
		r->answer_ += "<center>";
		r->answer_ += "<form method='get' action='webadmin/'>";
		r->answer_ += "<input type='submit' value='Log in'>";
		r->answer_ += "</form>";
		r->answer_ += "</center>";
		return;
	}

	if (r->path_.empty() || r->path_[0] != '/')
	{
		r->answer_ += "<h1>Bad request</h1>";
		r->answer_ += "<p>";
		r->answer_ = r->path_;  // TODO: Shouldn't we sanitize this? Possible security issue.
		r->answer_ += "</p>";
		return;
	}
	
	AStringVector Split = StringSplit(r->path_.substr(1), "/");

	if (Split.empty() || (Split[0] != "webadmin" && Split[0] != "~webadmin"))
	{
		r->answer_ += "<h1>Bad request</h1>";
		return;
	}
	
	if (!r->authentication_given_)
	{
		r->answer_ += "no auth";
		r->auth_realm_ = "MCServer WebAdmin";
	}

	bool bDontShowTemplate = false;
	if (Split[0] == "~webadmin")
	{
		bDontShowTemplate = true;
	}
	
	AString UserPassword = WebAdmin->m_IniFile->GetValue( "User:"+r->username_, "Password", "");
	if ((UserPassword != "") && (r->password_ == UserPassword))
	{
		AString Template;

		HTTPTemplateRequest TemplateRequest;
		TemplateRequest.Request.Username = r->username_;
		TemplateRequest.Request.Method = r->method_;
		TemplateRequest.Request.Params = r->params_;
		TemplateRequest.Request.PostParams = r->params_post_;
		TemplateRequest.Request.Path = r->path_.substr(1);

		for( unsigned int i = 0; i < r->multipart_formdata_.size(); ++i )
		{
			webserver::formdata& fd = r->multipart_formdata_[i];

			HTTPFormData HTTPfd;//( fd.value_ );
			HTTPfd.Value = fd.value_;
			HTTPfd.Type = fd.content_type_;
			HTTPfd.Name = fd.name_;
			LOGINFO("Form data name: %s", fd.name_.c_str() );
			TemplateRequest.Request.FormData[ fd.name_ ] = HTTPfd;
		}

		// Try to get the template from the Lua template script
		bool bLuaTemplateSuccessful = false;
		if (!bDontShowTemplate)
		{
			bLuaTemplateSuccessful = WebAdmin->m_TemplateScript.Call("ShowPage", WebAdmin, &TemplateRequest, cLuaState::Return, Template);
		}
		
		if (!bLuaTemplateSuccessful)
		{
			AString BaseURL = WebAdmin->GetBaseURL(Split);
			AString Menu;
			Template = bDontShowTemplate ? "{CONTENT}" : WebAdmin->GetTemplate();
			AString FoundPlugin;

			for (PluginList::iterator itr = WebAdmin->m_Plugins.begin(); itr != WebAdmin->m_Plugins.end(); ++itr)
			{
				cWebPlugin* WebPlugin = *itr;
				std::list< std::pair<AString, AString> > NameList = WebPlugin->GetTabNames();
				for( std::list< std::pair<AString, AString> >::iterator Names = NameList.begin(); Names != NameList.end(); ++Names )
				{
					Menu += "<li><a href='" + BaseURL + WebPlugin->GetWebTitle().c_str() + "/" + (*Names).second + "'>" + (*Names).first + "</a></li>";
				}
			}

			sWebAdminPage Page = WebAdmin->GetPage(TemplateRequest.Request);
			AString Content = Page.Content;
			FoundPlugin = Page.PluginName;
			if (!Page.TabName.empty())
				FoundPlugin += " - " + Page.TabName;

			if( FoundPlugin.empty() )	// Default page
			{
				Content.clear();
				FoundPlugin = "Current Game";
				Content += "<h4>Server Name:</h4>";
				Content += "<p>" + AString( cRoot::Get()->GetServer()->GetServerID() ) + "</p>";

				Content += "<h4>Plugins:</h4><ul>";
				cPluginManager* PM = cRoot::Get()->GetPluginManager();
				if( PM )
				{
					const cPluginManager::PluginMap & List = PM->GetAllPlugins();
					for( cPluginManager::PluginMap::const_iterator itr = List.begin(); itr != List.end(); ++itr )
					{
						if( itr->second == NULL ) continue;
						AString VersionNum;
						AppendPrintf(Content, "<li>%s V.%i</li>", itr->second->GetName().c_str(), itr->second->GetVersion());
					}
				}
				Content += "</ul>";
				Content += "<h4>Players:</h4><ul>";

				cPlayerAccum PlayerAccum;
				cWorld * World = cRoot::Get()->GetDefaultWorld(); // TODO - Create a list of worlds and players
				if( World != NULL )
				{
					World->ForEachPlayer(PlayerAccum);
					Content.append(PlayerAccum.m_Contents);
				}
				Content += "</ul><br>";
			}

			

			if (!bDontShowTemplate && (Split.size() > 1))
			{
				Content += "\n<p><a href='" + BaseURL + "'>Go back</a></p>";
			}

			int MemUsageKiB = GetMemoryUsage();
			if (MemUsageKiB > 0)
			{
				ReplaceString(Template, "{MEM}",       Printf("%.02f", (double)MemUsageKiB / 1024));
				ReplaceString(Template, "{MEMKIB}",    Printf("%d", MemUsageKiB));
			}
			else
			{
				ReplaceString(Template, "{MEM}",       "unknown");
				ReplaceString(Template, "{MEMKIB}",    "unknown");
			}
			ReplaceString(Template, "{USERNAME}",    r->username_);
			ReplaceString(Template, "{MENU}",        Menu);
			ReplaceString(Template, "{PLUGIN_NAME}", FoundPlugin);
			ReplaceString(Template, "{CONTENT}",     Content);
			ReplaceString(Template, "{TITLE}",       "MCServer");

			AString NumChunks;
			Printf(NumChunks, "%d", cRoot::Get()->GetTotalChunkCount());
			ReplaceString(Template, "{NUMCHUNKS}", NumChunks);
		}

		r->answer_ = Template;
	}
	else
	{
		r->answer_ += "Wrong username/password";
		r->auth_realm_ = "MCServer WebAdmin";
	}
}





bool cWebAdmin::Init(int a_Port)
{
	m_Port = a_Port;

	m_IniFile = new cIniFile("webadmin.ini");
	if (m_IniFile->ReadFile())
	{
		m_Port = m_IniFile->GetValueI("WebAdmin", "Port", 8080);
	}

	// Initialize the WebAdmin template script and load the file
	m_TemplateScript.Create();
	if (!m_TemplateScript.LoadFile(FILE_IO_PREFIX "webadmin/template.lua"))
	{
		LOGWARN("Could not load WebAdmin template \"%s\", using default template.", FILE_IO_PREFIX "webadmin/template.lua");
		m_TemplateScript.Close();
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
	if (!self->m_WebServer->Begin())
	{
		LOGWARN("WebServer failed to start! WebAdmin is disabled");
	}

	self->m_Event->Set();
	return 0;
}





AString cWebAdmin::GetTemplate()
{
	AString retVal = "";

	char SourceFile[] = "webadmin/template.html";

	cFile f;
	if (!f.Open(SourceFile, cFile::fmRead))
	{
		return "";
	}

	// copy the file into the buffer:
	f.ReadRestOfFile(retVal);

	return retVal;
}





sWebAdminPage cWebAdmin::GetPage(const HTTPRequest& a_Request)
{
	sWebAdminPage Page;
	AStringVector Split = StringSplit(a_Request.Path, "/");

	// Find the plugin that corresponds to the requested path
	AString FoundPlugin;
	if (Split.size() > 1)
	{
		for (PluginList::iterator itr = WebAdmin->m_Plugins.begin(); itr != WebAdmin->m_Plugins.end(); ++itr)
		{
			if ((*itr)->GetWebTitle() == Split[1])
			{
				Page.Content = (*itr)->HandleWebRequest(&a_Request);
				cWebPlugin * WebPlugin = *itr;
				FoundPlugin = WebPlugin->GetWebTitle();
				AString TabName = WebPlugin->GetTabNameForRequest(&a_Request).first;
				Page.PluginName = FoundPlugin;
				Page.TabName = TabName;
				break;
			}
		}
	}

	// Return the page contents
	return Page;
}





AString cWebAdmin::GetBaseURL( const AString& a_URL )
{
	return GetBaseURL(StringSplit(a_URL, "/"));
}





AString cWebAdmin::GetBaseURL( const AStringVector& a_URLSplit )
{
	AString BaseURL = "./";
	if (a_URLSplit.size() > 1)
	{
		for (unsigned int i = 0; i < a_URLSplit.size(); i++)
		{
			BaseURL += "../";
		}
		BaseURL += "webadmin/";
	}
	return BaseURL;
}





int cWebAdmin::GetMemoryUsage(void)
{
	#ifdef _WIN32
		PROCESS_MEMORY_COUNTERS pmc;
		if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc)))
		{
			return (int)(pmc.WorkingSetSize / 1024);
		}
		return -1;
	#elif defined(__linux__)
		// Code adapted from http://stackoverflow.com/questions/63166/how-to-determine-cpu-and-memory-consumption-from-inside-a-process
		std::ifstream StatFile("/proc/self/status");
		if (!StatFile.good())
		{
			return -1;
		}
		while (StatFile.good())
		{
			AString Line;
			std::getline(StatFile, Line);
			if (strncmp(Line.c_str(), "VmSize:", 7) == 0)
			{
				int res = atoi(Line.c_str() + 8);
				return (res == 0) ? -1 : res;  // If parsing failed, return -1
			}
		}
		return -1;
	#elif defined (__APPLE__)
		// Code adapted from http://stackoverflow.com/questions/63166/how-to-determine-cpu-and-memory-consumption-from-inside-a-process
		struct task_basic_info t_info;
		mach_msg_type_number_t t_info_count = TASK_BASIC_INFO_COUNT;

		if (KERN_SUCCESS == task_info(
			mach_task_self(),
			TASK_BASIC_INFO,
			(task_info_t)&t_info,
			&t_info_count
		))
		{
		    return (int)(t_info.resident_size / 1024);
		}
		return -1;
	#else
		LOGINFO("%s: Unknown platform, cannot query memory usage", __FUNCTION__);
		return -1;
	#endif
}




