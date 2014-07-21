
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "WebAdmin.h"
#include "Bindings/WebPlugin.h"

#include "Bindings/PluginManager.h"
#include "Bindings/Plugin.h"

#include "World.h"
#include "Entities/Player.h"
#include "Server.h"
#include "Root.h"

#include "HTTPServer/HTTPMessage.h"
#include "HTTPServer/HTTPConnection.h"





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





cWebAdmin::cWebAdmin(void) :
	m_IsInitialized(false),
	m_IsRunning(false),
	m_TemplateScript("<webadmin_template>")
{
}





cWebAdmin::~cWebAdmin()
{
	ASSERT(!m_IsRunning);  // Was the HTTP server stopped properly?
}





void cWebAdmin::AddPlugin(cWebPlugin * a_Plugin)
{
	m_Plugins.remove(a_Plugin);
	m_Plugins.push_back(a_Plugin);
}





void cWebAdmin::RemovePlugin(cWebPlugin * a_Plugin)
{
	m_Plugins.remove(a_Plugin);
}





bool cWebAdmin::Init(void)
{
	if (!m_IniFile.ReadFile("webadmin.ini"))
	{
		LOGWARN("Regenerating webadmin.ini, all settings will be reset");
		m_IniFile.AddHeaderComment(" This file controls the webadmin feature of MCServer");
		m_IniFile.AddHeaderComment(" Username format: [User:*username*]");
		m_IniFile.AddHeaderComment(" Password format: Password=*password*; for example:");
		m_IniFile.AddHeaderComment(" [User:admin]");
		m_IniFile.AddHeaderComment(" Password=admin");
		m_IniFile.WriteFile("webadmin.ini");
	}

	if (!m_IniFile.GetValueSetB("WebAdmin", "Enabled", true))
	{
		// WebAdmin is disabled, bail out faking a success
		return true;
	}

	LOGD("Initialising WebAdmin...");

	m_PortsIPv4 = m_IniFile.GetValueSet("WebAdmin", "Port", "8080");
	m_PortsIPv6 = m_IniFile.GetValueSet("WebAdmin", "PortsIPv6", "");

	if (!m_HTTPServer.Initialize(m_PortsIPv4, m_PortsIPv6))
	{
		return false;
	}
	m_IsInitialized = true;
	m_IniFile.WriteFile("webadmin.ini");
	return true;
}





bool cWebAdmin::Start(void)
{
	if (!m_IsInitialized)
	{
		// Not initialized
		return false;
	}

	LOGD("Starting WebAdmin...");

	// Initialize the WebAdmin template script and load the file
	m_TemplateScript.Create();
	m_TemplateScript.RegisterAPILibs();
	if (!m_TemplateScript.LoadFile(FILE_IO_PREFIX "webadmin/template.lua"))
	{
		LOGWARN("Could not load WebAdmin template \"%s\", using default template.", FILE_IO_PREFIX "webadmin/template.lua");
		m_TemplateScript.Close();
	}

	m_IsRunning = m_HTTPServer.Start(*this);
	return m_IsRunning;
}





void cWebAdmin::Stop(void)
{
	if (!m_IsRunning)
	{
		return;
	}
	
	LOGD("Stopping WebAdmin...");
	m_HTTPServer.Stop();
	m_IsRunning = false;
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





void cWebAdmin::HandleWebadminRequest(cHTTPConnection & a_Connection, cHTTPRequest & a_Request)
{
	if (!a_Request.HasAuth())
	{
		a_Connection.SendNeedAuth("MCServer WebAdmin");
		return;
	}

	// Check auth:
	AString UserPassword = m_IniFile.GetValue("User:" + a_Request.GetAuthUsername(), "Password", "");
	if ((UserPassword == "") || (a_Request.GetAuthPassword() != UserPassword))
	{
		a_Connection.SendNeedAuth("MCServer WebAdmin - bad username or password");
		return;
	}

	// Check if the contents should be wrapped in the template:
	AString URL = a_Request.GetBareURL();
	ASSERT(URL.length() > 0);
	bool ShouldWrapInTemplate = ((URL.length() > 1) && (URL[1] != '~'));

	// Retrieve the request data:
	cWebadminRequestData * Data = (cWebadminRequestData *)(a_Request.GetUserData());
	if (Data == NULL)
	{
		a_Connection.SendStatusAndReason(500, "Bad UserData");
		return;
	}

	// Wrap it all up for the Lua call:
	AString Template;
	HTTPTemplateRequest TemplateRequest;
	TemplateRequest.Request.Username = a_Request.GetAuthUsername();
	TemplateRequest.Request.Method = a_Request.GetMethod();
	TemplateRequest.Request.Path = URL.substr(1);

	if (Data->m_Form.Finish())
	{
		for (cHTTPFormParser::const_iterator itr = Data->m_Form.begin(), end = Data->m_Form.end(); itr != end; ++itr)
		{
			HTTPFormData HTTPfd;
			HTTPfd.Value = itr->second;
			HTTPfd.Type = "";
			HTTPfd.Name = itr->first;
			TemplateRequest.Request.FormData[itr->first] = HTTPfd;
			TemplateRequest.Request.PostParams[itr->first] = itr->second;
		}  // for itr - Data->m_Form[]

		// Parse the URL into individual params:
		const AString & URL = a_Request.GetURL();
		size_t idxQM = URL.find('?');
		if (idxQM != AString::npos)
		{
			cHTTPFormParser URLParams(cHTTPFormParser::fpkURL, URL.c_str() + idxQM + 1, URL.length() - idxQM - 1, *Data);
			URLParams.Finish();
			for (cHTTPFormParser::const_iterator itr = URLParams.begin(), end = URLParams.end(); itr != end; ++itr)
			{
				TemplateRequest.Request.Params[itr->first] = itr->second;
			}  // for itr - URLParams[]
		}
	}

	// Try to get the template from the Lua template script
	if (ShouldWrapInTemplate)
	{
		if (m_TemplateScript.Call("ShowPage", this, &TemplateRequest, cLuaState::Return, Template))
		{
			cHTTPResponse Resp;
			Resp.SetContentType("text/html");
			a_Connection.Send(Resp);
			a_Connection.Send(Template.c_str(), Template.length());
			return;
		}
		a_Connection.SendStatusAndReason(500, "m_TemplateScript failed");
		return;
	}

	AString BaseURL = GetBaseURL(URL);
	AString Menu;
	Template = "{CONTENT}";
	AString FoundPlugin;

	for (PluginList::iterator itr = m_Plugins.begin(); itr != m_Plugins.end(); ++itr)
	{
		cWebPlugin * WebPlugin = *itr;
		std::list< std::pair<AString, AString> > NameList = WebPlugin->GetTabNames();
		for (std::list< std::pair<AString, AString> >::iterator Names = NameList.begin(); Names != NameList.end(); ++Names)
		{
			Menu += "<li><a href='" + BaseURL + WebPlugin->GetWebTitle().c_str() + "/" + (*Names).second + "'>" + (*Names).first + "</a></li>";
		}
	}

	sWebAdminPage Page = GetPage(TemplateRequest.Request);
	AString Content = Page.Content;
	FoundPlugin = Page.PluginName;
	if (!Page.TabName.empty())
	{
		FoundPlugin += " - " + Page.TabName;
	}

	if (FoundPlugin.empty())  // Default page
	{
		Content = GetDefaultPage();
	}

	int MemUsageKiB = cRoot::GetPhysicalRAMUsage();
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
	ReplaceString(Template, "{USERNAME}",    a_Request.GetAuthUsername());
	ReplaceString(Template, "{MENU}",        Menu);
	ReplaceString(Template, "{PLUGIN_NAME}", FoundPlugin);
	ReplaceString(Template, "{CONTENT}",     Content);
	ReplaceString(Template, "{TITLE}",       "MCServer");

	AString NumChunks;
	Printf(NumChunks, "%d", cRoot::Get()->GetTotalChunkCount());
	ReplaceString(Template, "{NUMCHUNKS}", NumChunks);

	cHTTPResponse Resp;
	Resp.SetContentType("text/html");
	a_Connection.Send(Resp);
	a_Connection.Send(Template.c_str(), Template.length());
}





void cWebAdmin::HandleRootRequest(cHTTPConnection & a_Connection, cHTTPRequest & a_Request)
{
	UNUSED(a_Request);
	static const char LoginForm[] = \
	"<h1>MCServer WebAdmin</h1>" \
	"<center>" \
	"<form method='get' action='webadmin/'>" \
	"<input type='submit' value='Log in'>" \
	"</form>" \
	"</center>";
	cHTTPResponse Resp;
	Resp.SetContentType("text/html");
	a_Connection.Send(Resp);
	a_Connection.Send(LoginForm, sizeof(LoginForm) - 1);
	a_Connection.FinishResponse();
}





sWebAdminPage cWebAdmin::GetPage(const HTTPRequest & a_Request)
{
	sWebAdminPage Page;
	AStringVector Split = StringSplit(a_Request.Path, "/");

	// Find the plugin that corresponds to the requested path
	AString FoundPlugin;
	if (Split.size() > 1)
	{
		for (PluginList::iterator itr = m_Plugins.begin(); itr != m_Plugins.end(); ++itr)
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





AString cWebAdmin::GetDefaultPage(void)
{
	AString Content;
	Content += "<h4>Server Name:</h4>";
	Content += "<p>" + AString( cRoot::Get()->GetServer()->GetServerID()) + "</p>";

	Content += "<h4>Plugins:</h4><ul>";
	cPluginManager * PM = cPluginManager::Get();
	const cPluginManager::PluginMap & List = PM->GetAllPlugins();
	for (cPluginManager::PluginMap::const_iterator itr = List.begin(); itr != List.end(); ++itr)
	{
		if (itr->second == NULL)
		{
			continue;
		}
		AppendPrintf(Content, "<li>%s V.%i</li>", itr->second->GetName().c_str(), itr->second->GetVersion());
	}
	Content += "</ul>";
	Content += "<h4>Players:</h4><ul>";

	cPlayerAccum PlayerAccum;
	cWorld * World = cRoot::Get()->GetDefaultWorld();  // TODO - Create a list of worlds and players
	if (World != NULL)
	{
		World->ForEachPlayer(PlayerAccum);
		Content.append(PlayerAccum.m_Contents);
	}
	Content += "</ul><br>";
	return Content;
}




AString cWebAdmin::GetBaseURL( const AString& a_URL)
{
	return GetBaseURL(StringSplit(a_URL, "/"));
}





AString cWebAdmin::GetHTMLEscapedString(const AString & a_Input)
{
	AString dst;
	dst.reserve(a_Input.length());

	// Loop over input and substitute HTML characters for their alternatives:
	size_t len = a_Input.length();
	for (size_t i = 0; i < len; i++)
	{
		switch (a_Input[i])
		{
			case '&':  dst.append("&amp;");  break;
			case '\'': dst.append("&apos;"); break;
			case '"':  dst.append("&quot;"); break;
			case '<':  dst.append("&lt;");   break;
			case '>':  dst.append("&gt;");   break;
			default:
			{
				dst.push_back(a_Input[i]);
				break;
			}
		}  // switch (a_Input[i])
	}  // for i - a_Input[]

	return dst;
}





AString cWebAdmin::GetBaseURL(const AStringVector & a_URLSplit)
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





void cWebAdmin::OnRequestBegun(cHTTPConnection & a_Connection, cHTTPRequest & a_Request)
{
	UNUSED(a_Connection);
	const AString & URL = a_Request.GetURL();
	if (
		(strncmp(URL.c_str(), "/webadmin", 9) == 0) ||
		(strncmp(URL.c_str(), "/~webadmin", 10) == 0)
	)
	{
		a_Request.SetUserData(new cWebadminRequestData(a_Request));
		return;
	}
	if (URL == "/")
	{
		// The root needs no body handler and is fully handled in the OnRequestFinished() call
		return;
	}
	// TODO: Handle other requests
}





void cWebAdmin::OnRequestBody(cHTTPConnection & a_Connection, cHTTPRequest & a_Request, const char * a_Data, size_t a_Size)
{
	UNUSED(a_Connection);
	cRequestData * Data = (cRequestData *)(a_Request.GetUserData());
	if (Data == NULL)
	{
		return;
	}
	Data->OnBody(a_Data, a_Size);
}





void cWebAdmin::OnRequestFinished(cHTTPConnection & a_Connection, cHTTPRequest & a_Request)
{
	const AString & URL = a_Request.GetURL();
	if (
		(strncmp(URL.c_str(), "/webadmin", 9) == 0) ||
		(strncmp(URL.c_str(), "/~webadmin", 10) == 0)
	)
	{
		HandleWebadminRequest(a_Connection, a_Request);
	}
	else if (URL == "/")
	{
		// The root needs no body handler and is fully handled in the OnRequestFinished() call
		HandleRootRequest(a_Connection, a_Request);
	}
	else
	{
		// TODO: Handle other requests
	}

	// Delete any request data assigned to the request:
	cRequestData * Data = (cRequestData *)(a_Request.GetUserData());
	delete Data;
	Data = NULL;
}





////////////////////////////////////////////////////////////////////////////////
// cWebAdmin::cWebadminRequestData

void cWebAdmin::cWebadminRequestData::OnBody(const char * a_Data, size_t a_Size)
{
	m_Form.Parse(a_Data, a_Size);
}




