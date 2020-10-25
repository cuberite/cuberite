
// WebAdmin.h

// Declares the cWebAdmin class representing the admin interface over http protocol, and related services (API)

#pragma once

#include "Bindings/LuaState.h"
#include "IniFile.h"
#include "HTTP/HTTPServer.h"
#include "HTTP/HTTPMessage.h"





// Disable MSVC warnings:
#if defined(_MSC_VER)
	#pragma warning(push)
	#pragma warning(disable:4355)  // 'this' : used in base member initializer list
#endif





// tolua_begin
struct HTTPFormData
{
	std::string Name;
	std::string Value;
	std::string Type;
} ;
// tolua_end




// tolua_begin
struct HTTPRequest
{
	typedef std::map< std::string, std::string > StringStringMap;
	typedef std::map< std::string, HTTPFormData > FormDataMap;

	/** The entire URL presented to the HTTP server. */
	AString URL;

	/** HTTP method used for the request ("GET", "POST" etc.) */
	AString Method;

	/** The Path part of the request's URL (excluding GET params). */
	AString Path;

	/** Name of the logged-in user. Empty if not logged in. */
	AString Username;

	// tolua_end

	/** Parameters given in the URL, after the questionmark */
	StringStringMap Params;     // >> EXPORTED IN MANUALBINDINGS <<

	/** Parameters posted as a part of a form - either in the URL (GET method) or in the body (POST method) */
	StringStringMap PostParams;  // >> EXPORTED IN MANUALBINDINGS <<

	/** Same as PostParams */
	FormDataMap FormData;       // >> EXPORTED IN MANUALBINDINGS <<
} ;  // tolua_export





// tolua_begin
struct HTTPTemplateRequest
{
	HTTPRequest Request;
} ;
// tolua_end





struct sWebAdminPage
{
	AString Content;
	AString PluginName;
	AString TabTitle;
	AString TabUrlPath;
	AString ContentType;
};





// tolua_begin
class cWebAdmin :
	// tolua_end
	public cHTTPServer::cCallbacks
	// tolua_begin
{
public:
	// tolua_end

	/** Interface for getting the content of a single WebTab. */
	class cWebTabCallback abstract
	{
	public:
		// Force a virtual destructor in descendants
		virtual ~cWebTabCallback() {}

		/** Returns the contents for the specified request.
		Returns true if the call was successful, false on an error.
		a_Request is the full HTTP request object, as received from the client.
		a_UrlPath is the UrlPath of the WebTab registered for this request, as parsed from a_Request.
		Descendants should fill a_Content with the page contents
		and optionally set a_ContentType [defaults to "text/html"] */
		virtual bool Call(
			const HTTPRequest & a_Request,
			const AString & a_UrlPath,
			AString & a_Content,
			AString & a_ContentType
		) = 0;
	};


	/** Container for a single web tab.
	Each web tab has a title, URL path and an associated plugin's name.
	Each web tab is registered with a callback to provide the content. */
	class cWebTab
	{
	public:
		AString m_Title;
		AString m_UrlPath;
		AString m_PluginName;
		std::shared_ptr<cWebTabCallback> m_Callback;

		cWebTab(const AString & a_Title, const AString & a_UrlPath, const AString & a_PluginName, std::shared_ptr<cWebTabCallback> a_Callback):
			m_Title(a_Title),
			m_UrlPath(a_UrlPath),
			m_PluginName(a_PluginName),
			m_Callback(std::move(a_Callback))
		{
		}
	};
	typedef std::shared_ptr<cWebTab> cWebTabPtr;
	typedef std::vector<cWebTabPtr> cWebTabPtrs;


	cWebAdmin(void);
	virtual ~cWebAdmin() override;

	/** Initializes the object. Returns true if successfully initialized and ready to start */
	bool Init(void);

	/** Starts the HTTP server taking care of the webadmin. Returns true if successful */
	bool Start(void);

	/** Stops the HTTP server, if it was started. */
	void Stop(void);

	/** Loads the login template into m_LoginPage.
	Returns true if the loading succeeds, false if not. */
	bool LoadLoginPage(void);

	/** Returns a copy of all the registered web tabs.
	Exported to Lua in ManualBindings.cpp. */
	cWebTabPtrs GetAllWebTabs(void) { return m_WebTabs; }

	/** Removes all WebTabs registered by the specified plugin. */
	void RemoveAllPluginWebTabs(const AString & a_PluginName);

	/** Returns the (inner) page contents for the specified request.
	Calls the appropriate WebTab handler to get the contents.
	Exported to Lua in ManualBindings.cpp. */
	sWebAdminPage GetPage(const HTTPRequest & a_Request);

	// tolua_begin

	/** Reloads m_IniFile, m_LoginPage and m_TemplateScript.
	Note that reloading will not change the "enabled" state of the server, and it will not update listening ports. */
	void Reload(void);

	/** Returns the list of ports on which the webadmin is configured to listen. */
	AString GetPorts(void) const { return StringsConcat(m_Ports, ','); }

	/** Returns the prefix needed for making a link point to the webadmin root from the given URL ("../../../webadmin"-style). */
	static AString GetBaseURL(const AString & a_URL);

	/** Returns the content type from the file extension.
	If the extension isn't in the list, the function returns an empty string. */
	static AString GetContentTypeFromFileExt(const AString & a_FileExtension);

	/** Escapes text passed into it, so it can be embedded into html. */
	static AString GetHTMLEscapedString(const AString & a_Input);
	// tolua_end

	/** Adds a new WebTab handler.
	a_Title is the display title of the tab
	a_UrlPath is the part of the URL that uniquely identifies this tab.
	a_PluginName is the display name of the plugin creating this tab.
	a_Callback is used to provide the actual WebTab contents, when requested.
	Exported in ManualBindings.cpp. */
	void AddWebTab(
		const AString & a_Title,
		const AString & a_UrlPath,
		const AString & a_PluginName,
		std::shared_ptr<cWebTabCallback> a_Callback
	);

	/** Removes the WebTab with the specified URL path.
	Returns true if WebTab was found and removed, false if not found.
	Exported in ManualBindings.cpp */
	bool DelWebTab(const AString & a_UrlPath);

	/** Escapes the string for use in an URL
	Exported to Lua in ManualBindings.cpp. */
	static AString GetURLEncodedString(const AString & a_Input);

	/** Returns the prefix needed for making a link point to the webadmin root from the given URL ("../../../webadmin"-style) */
	static AString GetBaseURL(const AStringVector & a_URLSplit);

protected:

	/** Protects m_WebTabs, m_TemplateScript, m_LoginTemplate and m_IniFile against multithreaded access. */
	cCriticalSection m_CS;

	/** All registered WebTab handlers.
	Protected against multithreaded access by m_CS. */
	cWebTabPtrs m_WebTabs;

	/** The Lua template script to provide templates.
	Protected against multithreaded access by m_CS. */
	cLuaState m_TemplateScript;

	/** The HTML page that provides the login.
	Protected against multithreaded access by m_CS. */
	AString m_LoginPage;

	/** The webadmin.ini file, used for the settings and allowed logins.
	Protected against multithreaded access by m_CS. */
	cIniFile m_IniFile;

	/** Set to true if Init() succeeds and the webadmin isn't to be disabled */
	bool m_IsInitialized;

	/** Set to true if Start() succeeds in starting the server, reset back to false in Stop(). */
	bool m_IsRunning;

	/** The ports on which the webadmin is running. */
	AStringVector m_Ports;

	/** The HTTP server which provides the underlying HTTP parsing, serialization and events */
	cHTTPServer m_HTTPServer;


	/** Loads webadmin.ini into m_IniFile.
	Creates a default file if it doesn't exist.
	Returns true if webadmin is enabled, false if disabled. */
	bool LoadIniFile(void);

	/** Handles requests coming to the "/webadmin" or "/~webadmin" URLs */
	void HandleWebadminRequest(cHTTPServerConnection & a_Connection, cHTTPIncomingRequest & a_Request);

	/** Handles requests for the root page */
	void HandleRootRequest(cHTTPServerConnection & a_Connection, cHTTPIncomingRequest & a_Request);

	/** Handles requests for a file */
	void HandleFileRequest(cHTTPServerConnection & a_Connection, cHTTPIncomingRequest & a_Request);

	// cHTTPServer::cCallbacks overrides:
	virtual void OnRequestBegun   (cHTTPServerConnection & a_Connection, cHTTPIncomingRequest & a_Request) override;
	virtual void OnRequestBody    (cHTTPServerConnection & a_Connection, cHTTPIncomingRequest & a_Request, const char * a_Data, size_t a_Size) override;
	virtual void OnRequestFinished(cHTTPServerConnection & a_Connection, cHTTPIncomingRequest & a_Request) override;
} ;  // tolua_export





// Revert MSVC warnings back to orignal state:
#if defined(_MSC_VER)
	#pragma warning(pop)
#endif




