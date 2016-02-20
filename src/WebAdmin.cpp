
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "WebAdmin.h"
#include "Bindings/WebPlugin.h"

#include "Bindings/PluginManager.h"
#include "Bindings/Plugin.h"

#include "World.h"
#include "Entities/Player.h"
#include "Server.h"
#include "Root.h"

#include "HTTP/HTTPServerConnection.h"
#include "HTTP/HTTPFormParser.h"





static const char DEFAULT_WEBADMIN_PORTS[] = "8080";





////////////////////////////////////////////////////////////////////////////////
// cPlayerAccum:

/** Helper class - appends all player names together in an HTML list */
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





////////////////////////////////////////////////////////////////////////////////
// cWebadminRequestData

/** The form parser callbacks for requests in the "/webadmin" and "/~webadmin" paths */
class cWebadminRequestData :
	public cHTTPFormParser::cCallbacks,
	public cHTTPIncomingRequest::cUserData
{
public:
	cHTTPFormParser m_Form;


	cWebadminRequestData(const cHTTPIncomingRequest & a_Request):
		m_Form(a_Request, *this)
	{
	}

	// cHTTPFormParser::cCallbacks overrides. Files are ignored:
	virtual void OnFileStart(cHTTPFormParser &, const AString & a_FileName) override
	{
		UNUSED(a_FileName);
	}
	virtual void OnFileData(cHTTPFormParser &, const char * a_Data, size_t a_Size) override
	{
		UNUSED(a_Data);
		UNUSED(a_Size);
	}
	virtual void OnFileEnd(cHTTPFormParser &) override {}
} ;





////////////////////////////////////////////////////////////////////////////////
// cWebAdmin:

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
		m_IniFile.AddHeaderComment(" This file controls the webadmin feature of Cuberite");
		m_IniFile.AddHeaderComment(" Username format: [User:*username*]");
		m_IniFile.AddHeaderComment(" Password format: Password=*password*; for example:");
		m_IniFile.AddHeaderComment(" [User:admin]");
		m_IniFile.AddHeaderComment(" Password=admin");
		m_IniFile.SetValue("WebAdmin", "Ports", DEFAULT_WEBADMIN_PORTS);
		m_IniFile.WriteFile("webadmin.ini");
	}

	if (!m_IniFile.GetValueSetB("WebAdmin", "Enabled", true))
	{
		// WebAdmin is disabled, bail out faking a success
		return true;
	}

	LOGD("Initialising WebAdmin...");

	// Initialize the WebAdmin template script and load the file
	m_TemplateScript.Create();
	m_TemplateScript.RegisterAPILibs();
	if (!m_TemplateScript.LoadFile(FILE_IO_PREFIX "webadmin/template.lua"))
	{
		LOGWARN("Could not load WebAdmin template \"%s\". WebAdmin disabled!", FILE_IO_PREFIX "webadmin/template.lua");
		m_TemplateScript.Close();
		m_HTTPServer.Stop();
		return false;
	}

	// Load the login template, provide a fallback default if not found:
	if (!LoadLoginTemplate())
	{
		LOGWARN("Could not load WebAdmin login template \"%s\", using fallback template.", FILE_IO_PREFIX "webadmin/login_template.html");

		// Sets the fallback template:
		m_LoginTemplate = \
		"<h1>Cuberite WebAdmin</h1>" \
		"<center>" \
		"<form method='get' action='webadmin/'>" \
		"<input type='submit' value='Log in'>" \
		"</form>" \
		"</center>";
	}

	// Read the ports to be used:
	// Note that historically the ports were stored in the "Port" and "PortsIPv6" values
	m_Ports = ReadUpgradeIniPorts(m_IniFile, "WebAdmin", "Ports", "Port", "PortsIPv6", DEFAULT_WEBADMIN_PORTS);

	if (!m_HTTPServer.Initialize())
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

	m_IsRunning = m_HTTPServer.Start(*this, m_Ports);
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





bool cWebAdmin::LoadLoginTemplate(void)
{
	cFile File(FILE_IO_PREFIX "webadmin/login_template.html", cFile::fmRead);
	if (!File.IsOpen())
	{
		return false;
	}

	AString TemplateContent;
	if (File.ReadRestOfFile(TemplateContent) == -1)
	{
		return false;
	}

	m_LoginTemplate = TemplateContent;
	return true;
}





void cWebAdmin::HandleWebadminRequest(cHTTPServerConnection & a_Connection, cHTTPIncomingRequest & a_Request)
{
	if (!a_Request.HasAuth())
	{
		a_Connection.SendNeedAuth("Cuberite WebAdmin");
		return;
	}

	// Check auth:
	AString UserPassword = m_IniFile.GetValue("User:" + a_Request.GetAuthUsername(), "Password", "");
	if ((UserPassword == "") || (a_Request.GetAuthPassword() != UserPassword))
	{
		a_Connection.SendNeedAuth("Cuberite WebAdmin - bad username or password");
		return;
	}

	// Check if the contents should be wrapped in the template:
	auto BareURL = a_Request.GetURLPath();
	ASSERT(BareURL.length() > 0);
	bool ShouldWrapInTemplate = ((BareURL.length() > 1) && (BareURL[1] != '~'));

	// Retrieve the request data:
	auto Data = std::static_pointer_cast<cWebadminRequestData>(a_Request.GetUserData());
	if (Data == nullptr)
	{
		a_Connection.SendStatusAndReason(500, "Bad UserData");
		return;
	}

	// Wrap it all up for the Lua call:
	AString Template;
	HTTPTemplateRequest TemplateRequest;
	TemplateRequest.Request.URL = a_Request.GetURL();
	TemplateRequest.Request.Username = a_Request.GetAuthUsername();
	TemplateRequest.Request.Method = a_Request.GetMethod();
	TemplateRequest.Request.Path = BareURL.substr(1);

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
			cHTTPOutgoingResponse Resp;
			Resp.SetContentType("text/html");
			a_Connection.Send(Resp);
			a_Connection.Send(Template.c_str(), Template.length());
			return;
		}
		a_Connection.SendStatusAndReason(500, "m_TemplateScript failed");
		return;
	}

	AString BaseURL = GetBaseURL(BareURL);
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
		ReplaceString(Template, "{MEM}",       Printf("%.02f", static_cast<double>(MemUsageKiB) / 1024));
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
	ReplaceString(Template, "{TITLE}",       "Cuberite");

	AString NumChunks;
	Printf(NumChunks, "%d", cRoot::Get()->GetTotalChunkCount());
	ReplaceString(Template, "{NUMCHUNKS}", NumChunks);

	cHTTPOutgoingResponse Resp;
	Resp.SetContentType("text/html");
	a_Connection.Send(Resp);
	a_Connection.Send(Template.c_str(), Template.length());
	a_Connection.FinishResponse();
}





void cWebAdmin::HandleRootRequest(cHTTPServerConnection & a_Connection, cHTTPIncomingRequest & a_Request)
{
	UNUSED(a_Request);

	cHTTPOutgoingResponse Resp;
	Resp.SetContentType("text/html");
	a_Connection.Send(Resp);
	a_Connection.Send(m_LoginTemplate);
	a_Connection.FinishResponse();
}





void cWebAdmin::HandleFileRequest(cHTTPServerConnection & a_Connection, cHTTPIncomingRequest & a_Request)
{
	AString FileURL = a_Request.GetURL();
	std::replace(FileURL.begin(), FileURL.end(), '\\', '/');

	// Remove all leading backslashes:
	if (FileURL[0] == '/')
	{
		size_t FirstCharToRead = FileURL.find_first_not_of('/');
		if (FirstCharToRead != AString::npos)
		{
			FileURL = FileURL.substr(FirstCharToRead);
		}
	}

	// Remove all "../" strings:
	ReplaceString(FileURL, "../", "");

	bool LoadedSuccessfull = false;
	AString Content = "<h2>404 Not Found</h2>";
	AString Path = Printf(FILE_IO_PREFIX "webadmin/files/%s", FileURL.c_str());
	if (cFile::IsFile(Path))
	{
		cFile File(Path, cFile::fmRead);
		AString FileContent;
		if (File.IsOpen() && (File.ReadRestOfFile(FileContent) != -1))
		{
			LoadedSuccessfull = true;
			Content = FileContent;
		}
	}

	// Find content type (The currently method is very bad. We should change it later)
	AString ContentType = "text/html";
	size_t LastPointPosition = Path.find_last_of('.');
	if (LoadedSuccessfull && (LastPointPosition != AString::npos) && (LastPointPosition < Path.length()))
	{
		AString FileExtension = Path.substr(LastPointPosition + 1);
		ContentType = GetContentTypeFromFileExt(FileExtension);
	}

	// Send the response:
	cHTTPOutgoingResponse Resp;
	Resp.SetContentType(ContentType);
	a_Connection.Send(Resp);
	a_Connection.Send(Content);
	a_Connection.FinishResponse();
}





AString cWebAdmin::GetContentTypeFromFileExt(const AString & a_FileExtension)
{
	static bool IsInitialized = false;
	static std::map<AString, AString> ContentTypeMap;
	if (!IsInitialized)
	{
		// Initialize the ContentTypeMap:
		ContentTypeMap["png"]  = "image/png";
		ContentTypeMap["fif"]  = "image/fif";
		ContentTypeMap["gif"]  = "image/gif";
		ContentTypeMap["jpeg"] = "image/jpeg";
		ContentTypeMap["jpg"]  = "image/jpeg";
		ContentTypeMap["jpe"]  = "image/jpeg";
		ContentTypeMap["tiff"] = "image/tiff";
		ContentTypeMap["ico"]  = "image/ico";
		ContentTypeMap["csv"]  = "image/comma-separated-values";
		ContentTypeMap["css"]  = "text/css";
		ContentTypeMap["js"]   = "text/javascript";
		ContentTypeMap["txt"]  = "text/plain";
		ContentTypeMap["rtx"]  = "text/richtext";
		ContentTypeMap["xml"]  = "text/xml";
	}

	AString FileExtension = StrToLower(a_FileExtension);
	if (ContentTypeMap.find(a_FileExtension) == ContentTypeMap.end())
	{
		return "text/html";
	}
	return ContentTypeMap[FileExtension];
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
				Page.Content = (*itr)->HandleWebRequest(a_Request);
				cWebPlugin * WebPlugin = *itr;
				FoundPlugin = WebPlugin->GetWebTitle();
				AString TabName = WebPlugin->GetTabNameForRequest(a_Request).first;
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

	// Display a list of all plugins:
	Content += "<h4>Plugins:</h4><ul>";
	struct cPluginCallback:
		public cPluginManager::cPluginCallback
	{
		AString & m_Content;

		cPluginCallback(AString & a_Content):
			m_Content(a_Content)
		{
		}

		virtual bool Item(cPlugin * a_Plugin) override
		{
			if (a_Plugin->IsLoaded())
			{
				AppendPrintf(m_Content, "<li>%s V.%i</li>", a_Plugin->GetName().c_str(), a_Plugin->GetVersion());
			}
			return false;
		}
	} Callback(Content);
	cPluginManager::Get()->ForEachPlugin(Callback);
	Content += "</ul>";

	// Display a list of all players:
	Content += "<h4>Players:</h4><ul>";
	cPlayerAccum PlayerAccum;
	cWorld * World = cRoot::Get()->GetDefaultWorld();  // TODO - Create a list of worlds and players
	if (World != nullptr)
	{
		World->ForEachPlayer(PlayerAccum);
		Content.append(PlayerAccum.m_Contents);
	}
	Content += "</ul><br>";
	return Content;
}





AString cWebAdmin::GetBaseURL(const AString & a_URL)
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





AString cWebAdmin::GetURLEncodedString(const AString & a_Input)
{
	// Translation table from nibble to hex:
	static const char Hex[] = "0123456789abcdef";

	// Preallocate the output to match input:
	AString dst;
	size_t len = a_Input.length();
	dst.reserve(len);

	// Loop over input and substitute whatever is needed:
	for (size_t i = 0; i < len; i++)
	{
		char ch = a_Input[i];
		if (isalnum(ch) || (ch == '-') || (ch == '_') || (ch == '.') || (ch == '~'))
		{
			dst.push_back(ch);
		}
		else
		{
			dst.push_back('%');
			dst.push_back(Hex[(ch >> 4) & 0x0f]);
			dst.push_back(Hex[ch & 0x0f]);
		}
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





void cWebAdmin::OnRequestBegun(cHTTPServerConnection & a_Connection, cHTTPIncomingRequest & a_Request)
{
	UNUSED(a_Connection);
	const AString & URL = a_Request.GetURL();
	if (
		(strncmp(URL.c_str(), "/webadmin", 9) == 0) ||
		(strncmp(URL.c_str(), "/~webadmin", 10) == 0)
	)
	{
		a_Request.SetUserData(std::make_shared<cWebadminRequestData>(a_Request));
		return;
	}
	if (URL == "/")
	{
		// The root needs no body handler and is fully handled in the OnRequestFinished() call
		return;
	}
	// TODO: Handle other requests
}





void cWebAdmin::OnRequestBody(cHTTPServerConnection & a_Connection, cHTTPIncomingRequest & a_Request, const char * a_Data, size_t a_Size)
{
	UNUSED(a_Connection);
	auto Data = std::static_pointer_cast<cWebadminRequestData>(a_Request.GetUserData());
	if (Data == nullptr)
	{
		return;
	}
	Data->m_Form.Parse(a_Data, a_Size);
}





void cWebAdmin::OnRequestFinished(cHTTPServerConnection & a_Connection, cHTTPIncomingRequest & a_Request)
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
		HandleFileRequest(a_Connection, a_Request);
	}
}





