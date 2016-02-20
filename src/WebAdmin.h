
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





// fwd:
class cEvent;
class cWebPlugin;





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





// tolua_begin
struct sWebAdminPage
{
	AString Content;
	AString PluginName;
	AString TabName;
};
// tolua_end





// tolua_begin
class cWebAdmin :
	// tolua_end
	public cHTTPServer::cCallbacks
	// tolua_begin
{
public:
	// tolua_end

	typedef std::list< cWebPlugin* > PluginList;


	cWebAdmin(void);
	virtual ~cWebAdmin();

	/** Initializes the object. Returns true if successfully initialized and ready to start */
	bool Init(void);

	/** Starts the HTTP server taking care of the admin. Returns true if successful */
	bool Start(void);

	/** Stops the HTTP server, if it was started. */
	void Stop(void);

	/** Loads the login template. Returns true if the loading succeeds, false if not. */
	bool LoadLoginTemplate(void);

	void AddPlugin(cWebPlugin * a_Plugin);
	void RemovePlugin(cWebPlugin * a_Plugin);

	// TODO: Convert this to the auto-locking callback mechanism used for looping players in worlds and such
	PluginList GetPlugins() const { return m_Plugins; }  // >> EXPORTED IN MANUALBINDINGS <<

	// tolua_begin

	sWebAdminPage GetPage(const HTTPRequest & a_Request);

	/** Returns the contents of the default page - the list of plugins and players */
	AString GetDefaultPage(void);

	/** Returns the prefix needed for making a link point to the webadmin root from the given URL ("../../../webadmin"-style) */
	AString GetBaseURL(const AString & a_URL);

	/** Returns the list of ports used for the webadmin. */
	AString GetPorts(void) const { return StringsConcat(m_Ports, ','); }

	/** OBSOLETE: Returns the list of IPv4 ports used for the webadmin.
	Currently there is no distinction between IPv4 and IPv6; use GetPorts() instead. */
	AString GetIPv4Ports(void) const { return GetPorts(); }

	/** OBSOLETE: Returns the list of IPv6 ports used for the webadmin.
	Currently there is no distinction between IPv4 and IPv6; use GetPorts() instead. */
	AString GetIPv6Ports(void) const { return GetPorts(); }

	// tolua_end

	/** Escapes text passed into it, so it can be embedded into html. */
	static AString GetHTMLEscapedString(const AString & a_Input);

	/** Escapes the string for use in an URL */
	static AString GetURLEncodedString(const AString & a_Input);

	/** Returns the prefix needed for making a link point to the webadmin root from the given URL ("../../../webadmin"-style) */
	static AString GetBaseURL(const AStringVector & a_URLSplit);

	/** Returns the content type from the file extension. If the extension isn't in the list, the function returns "text/html" */
	static AString GetContentTypeFromFileExt(const AString & a_FileExtension);

protected:

	/** Set to true if Init() succeeds and the webadmin isn't to be disabled */
	bool m_IsInitialized;

	/** Set to true if Start() succeeds in starting the server, reset back to false in Stop(). */
	bool m_IsRunning;

	/** The webadmin.ini file, used for the settings and allowed logins */
	cIniFile   m_IniFile;

	PluginList m_Plugins;

	/** The ports on which the webadmin is running. */
	AStringVector m_Ports;

	/** The Lua template script to provide templates: */
	cLuaState m_TemplateScript;

	/** The template that provides the login site: */
	AString m_LoginTemplate;

	/** The HTTP server which provides the underlying HTTP parsing, serialization and events */
	cHTTPServer m_HTTPServer;

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




