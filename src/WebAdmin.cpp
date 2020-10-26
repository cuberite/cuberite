
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "WebAdmin.h"

#include "World.h"
#include "Entities/Player.h"
#include "Server.h"
#include "Root.h"

#include "HTTP/HTTPServerConnection.h"
#include "HTTP/HTTPFormParser.h"





static const char DEFAULT_WEBADMIN_PORTS[] = "8080";





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
	m_TemplateScript("<webadmin_template>"),
	m_IsInitialized(false),
	m_IsRunning(false)
{
}





cWebAdmin::~cWebAdmin()
{
	ASSERT(!m_IsRunning);  // Was the HTTP server stopped properly?
}





bool cWebAdmin::Init(void)
{
	if (!LoadIniFile())
	{
		// WebAdmin is disabled, bail out faking a success
		return true;
	}

	LOGD("Initialising WebAdmin...");

	Reload();

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





bool cWebAdmin::LoadLoginPage(void)
{
	cFile File("webadmin/login_template.html", cFile::fmRead);
	if (!File.IsOpen())
	{
		return false;
	}

	AString TemplateContent;
	if (File.ReadRestOfFile(TemplateContent) == -1)
	{
		return false;
	}

	cCSLock Lock(m_CS);
	m_LoginPage = TemplateContent;
	return true;
}





void cWebAdmin::RemoveAllPluginWebTabs(const AString & a_PluginName)
{
	cCSLock lock(m_CS);
	m_WebTabs.erase(std::remove_if(m_WebTabs.begin(), m_WebTabs.end(), [=](const cWebTabPtr & a_CBWebTab)
		{
			return (a_CBWebTab->m_PluginName == a_PluginName);
		}),
		m_WebTabs.end()
	);
}





void cWebAdmin::Reload(void)
{
	cCSLock lock(m_CS);
	if (!LoadIniFile())
	{
		// We are asked to disable the webadmin, cannot do that, so warn the admin:
		LOGWARNING(
			"WebAdmin was previously enabled and now the settings say to disable it."
			" This will not take effect until you restart the server."
		);
	}

	// Initialize the WebAdmin template script and reload the file:
	if (m_TemplateScript.IsValid())
	{
		m_TemplateScript.Close();
	}
	m_TemplateScript.Create();
	m_TemplateScript.RegisterAPILibs();
	if (!m_TemplateScript.LoadFile("webadmin/template.lua"))
	{
		LOGWARN("Could not load WebAdmin template \"%s\". WebAdmin will not work properly!", "webadmin/template.lua");
		m_TemplateScript.Close();
	}

	// Load the login template, provide a fallback default if not found:
	if (!LoadLoginPage())
	{
		LOGWARN("Could not load WebAdmin login page \"%s\", using fallback template.", "webadmin/login_template.html");

		// Set the fallback:
		m_LoginPage = \
		"<h1>Cuberite WebAdmin</h1>" \
		"<center>" \
		"<form method='get' action='webadmin/'>" \
		"<input type='submit' value='Log in'>" \
		"</form>" \
		"</center>";
	}
}





bool cWebAdmin::LoadIniFile(void)
{
	m_IniFile.Clear();
	if (!m_IniFile.ReadFile("webadmin.ini"))
	{
		LOGWARN("Regenerating webadmin.ini, all settings will be reset");
		m_IniFile.AddHeaderComment(" This file controls the webadmin feature of Cuberite");
		m_IniFile.AddHeaderComment(" It specifies whether webadmin is enabled, and what logins are allowed. ");
		m_IniFile.AddHeaderComment(" Username format: [User:*username*]");
		m_IniFile.AddHeaderComment(" Password format: Password=*password*; for example:");
		m_IniFile.AddHeaderComment(" [User:admin]");
		m_IniFile.AddHeaderComment(" Password=admin");
		m_IniFile.AddHeaderComment(" Please restart Cuberite to apply changes made in this file!");
		m_IniFile.SetValue("WebAdmin", "Ports", DEFAULT_WEBADMIN_PORTS);
		m_IniFile.WriteFile("webadmin.ini");
	}

	return m_IniFile.GetValueSetB("WebAdmin", "Enabled", true);
}





void cWebAdmin::HandleWebadminRequest(cHTTPServerConnection & a_Connection, cHTTPIncomingRequest & a_Request)
{
	if (!a_Request.HasAuth())
	{
		a_Connection.SendNeedAuth("Cuberite WebAdmin");
		return;
	}

	// Check auth:
	{
		cCSLock Lock(m_CS);
		AString UserPassword = m_IniFile.GetValue("User:" + a_Request.GetAuthUsername(), "Password", "");
		if ((UserPassword == "") || (a_Request.GetAuthPassword() != UserPassword))
		{
			a_Connection.SendNeedAuth("Cuberite WebAdmin - bad username or password");
			return;
		}
	}

	// Check if the contents should be wrapped in the template:
	auto BareURL = a_Request.GetURLPath();
	ASSERT(BareURL.length() > 0);
	bool ShouldWrapInTemplate = (!BareURL.empty() && (BareURL[1] != '~'));

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
		cCSLock LockSelf(m_CS);
		cLuaState::cLock LockTemplate(m_TemplateScript);
		if (m_TemplateScript.Call("ShowPage", this, &TemplateRequest, cLuaState::Return, Template))
		{
			cHTTPOutgoingResponse Resp;
			Resp.SetContentType("text/html");
			a_Connection.Send(Resp);
			a_Connection.Send(Template.c_str(), Template.length());
			a_Connection.FinishResponse();
			return;
		}
		a_Connection.SendStatusAndReason(500, "m_TemplateScript failed");
		return;
	}

	// Send the un-decorated page content:
	auto page = GetPage(TemplateRequest.Request);
	cHTTPOutgoingResponse resp;
	resp.SetContentType(page.ContentType);
	a_Connection.Send(resp);
	a_Connection.Send(page.Content.c_str(), page.Content.length());
	a_Connection.FinishResponse();
}





void cWebAdmin::HandleRootRequest(cHTTPServerConnection & a_Connection, cHTTPIncomingRequest & a_Request)
{
	UNUSED(a_Request);

	cHTTPOutgoingResponse Resp;
	Resp.SetContentType("text/html");
	a_Connection.Send(Resp);
	a_Connection.Send(m_LoginPage);
	a_Connection.FinishResponse();
}





void cWebAdmin::HandleFileRequest(cHTTPServerConnection & a_Connection, cHTTPIncomingRequest & a_Request)
{
	AString FileURL = a_Request.GetURL();
	std::replace(FileURL.begin(), FileURL.end(), '\\', '/');

	// Remove all leading backslashes:
	if (!FileURL.empty() && (FileURL[0] == '/'))
	{
		size_t FirstCharToRead = FileURL.find_first_not_of('/');
		if (FirstCharToRead != AString::npos)
		{
			FileURL = FileURL.substr(FirstCharToRead);
		}
	}

	// Read the file contents and guess its mime-type, based on the extension:
	AString Content = "<h2>404 Not Found</h2>";
	AString ContentType = "text/html";
	AString Path = Printf("webadmin/files/%s", FileURL.c_str());

	// Return 404 if the file is not found, or the URL contains '../' (for security reasons)
	if ((FileURL.find("../") == AString::npos) && cFile::IsFile(Path))
	{
		cFile File(Path, cFile::fmRead);
		AString FileContent;
		if (File.IsOpen() && (File.ReadRestOfFile(FileContent) != -1))
		{
			std::swap(Content, FileContent);
			size_t LastPointPosition = Path.find_last_of('.');
			if (LastPointPosition != AString::npos)
			{
				ContentType = GetContentTypeFromFileExt(Path.substr(LastPointPosition + 1));
			}
		}
		if (ContentType.empty())
		{
			ContentType = "application/unknown";
		}
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
	static AStringMap ContentTypeMap;
	if (!IsInitialized)
	{
		// Initialize the ContentTypeMap:
		ContentTypeMap["png"]   = "image/png";
		ContentTypeMap["fif"]   = "image/fif";
		ContentTypeMap["gif"]   = "image/gif";
		ContentTypeMap["jpeg"]  = "image/jpeg";
		ContentTypeMap["jpg"]   = "image/jpeg";
		ContentTypeMap["jpe"]   = "image/jpeg";
		ContentTypeMap["tiff"]  = "image/tiff";
		ContentTypeMap["ico"]   = "image/ico";
		ContentTypeMap["csv"]   = "text/csv";
		ContentTypeMap["css"]   = "text/css";
		ContentTypeMap["js"]    = "text/javascript";
		ContentTypeMap["txt"]   = "text/plain";
		ContentTypeMap["rtx"]   = "text/richtext";
		ContentTypeMap["rtf"]   = "text/richtext";
		ContentTypeMap["xml"]   = "text/xml";
		ContentTypeMap["html"]  = "text/html";
		ContentTypeMap["htm"]   = "text/html";
		ContentTypeMap["xhtml"] = "application/xhtml+xml";  // Not recomended for IE6, but no-one uses that anymore
	}

	auto itr = ContentTypeMap.find(StrToLower(a_FileExtension));
	if (itr == ContentTypeMap.end())
	{
		return AString();
	}
	return itr->second;
}





sWebAdminPage cWebAdmin::GetPage(const HTTPRequest & a_Request)
{
	sWebAdminPage page;
	auto split = StringSplit(a_Request.Path, "/");

	// If no specific page was requested, return an empty object:
	if (split.size() <= 2)
	{
		return page;
	}

	// Find the WebTab handler responsible for the request:
	cWebTabPtr tab;
	{
		cCSLock Lock(m_CS);
		for (auto & wt: m_WebTabs)
		{
			if (
				(wt->m_PluginName == split[1]) &&
				(wt->m_UrlPath == split[2])
			)
			{
				tab = wt;
				break;
			}
		}  // for wt - m_WebTabs[]
	}

	// If a WebTab handler was found, call it:
	if (tab != nullptr)
	{
		page.ContentType = "text/html";  // Default to HTML content type, unless overridden by a plugin
		if (!tab->m_Callback->Call(a_Request, split[1], page.Content, page.ContentType))
		{
			page.Content = GetHTMLEscapedString(Printf(
				"WebTab callback for plugin %s, page %s has failed.",
				tab->m_PluginName.c_str(), tab->m_Title.c_str()
			));
		}
		page.PluginName = tab->m_PluginName;
		page.TabTitle = tab->m_Title;
		page.TabUrlPath = split[1];
	}

	return page;
}





AString cWebAdmin::GetBaseURL(const AString & a_URL)
{
	return GetBaseURL(StringSplit(a_URL, "/"));
}





void cWebAdmin::AddWebTab(
	const AString & a_Title,
	const AString & a_UrlPath,
	const AString & a_PluginName,
	std::shared_ptr<cWebAdmin::cWebTabCallback> a_Callback
)
{
	cCSLock lock(m_CS);
	m_WebTabs.emplace_back(std::make_shared<cWebTab>(a_Title, a_UrlPath, a_PluginName, std::move(a_Callback)));
}





bool cWebAdmin::DelWebTab(const AString & a_UrlPath)
{
	cCSLock lock(m_CS);
	for (auto itr = m_WebTabs.begin(), end = m_WebTabs.end(); itr != end; ++itr)
	{
		if ((*itr)->m_UrlPath == a_UrlPath)
		{
			m_WebTabs.erase(itr);
			return true;
		}
	}  // for itr - m_WebTabs[]

	// Not found:
	return false;
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
	return URLEncode(a_Input);
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





