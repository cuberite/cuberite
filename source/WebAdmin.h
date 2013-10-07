
// WebAdmin.h

// Declares the cWebAdmin class representing the admin interface over http protocol, and related services (API)

#pragma once

#include "OSSupport/Socket.h"
#include "LuaState.h"
#include "../iniFile/iniFile.h"
#include "HTTPServer/HTTPServer.h"
#include "HTTPServer/HTTPFormParser.h"





// Disable MSVC warnings:
#if defined(_MSC_VER)
	#pragma warning(push)
	#pragma warning(disable:4355)  // 'this' : used in base member initializer list
#endif





// fwd:
class cStringMap;
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
	
	AString Method;
	AString Path;
	AString Username;
	// tolua_end
	StringStringMap Params;     // >> EXPORTED IN MANUALBINDINGS <<
	StringStringMap PostParams; // >> EXPORTED IN MANUALBINDINGS <<
	FormDataMap FormData;       // >> EXPORTED IN MANUALBINDINGS <<
} ; // tolua_export





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
	public cHTTPServer::cCallbacks
{
public:
	// tolua_end
	
	typedef std::list< cWebPlugin* > PluginList;


	cWebAdmin(void);

	/// Initializes the object. Returns true if successfully initialized and ready to start
	bool Init(void);
	
	/// Starts the HTTP server taking care of the admin. Returns true if successful
	bool Start(void);

	void AddPlugin( cWebPlugin* a_Plugin );
	void RemovePlugin( cWebPlugin* a_Plugin );

	// TODO: Convert this to the auto-locking callback mechanism used for looping players in worlds and such
	PluginList          GetPlugins() const { return m_Plugins; } // >> EXPORTED IN MANUALBINDINGS <<

	// tolua_begin
	
	/// Returns the amount of currently used memory, in KiB, or -1 if it cannot be queried
	static int GetMemoryUsage(void);

	sWebAdminPage       GetPage(const HTTPRequest& a_Request);
	
	/// Returns the contents of the default page - the list of plugins and players
	AString GetDefaultPage(void);
	
	AString             GetBaseURL(const AString& a_URL);
	
	// tolua_end

	AString             GetBaseURL(const AStringVector& a_URLSplit);
	
protected:
	/// Common base class for request body data handlers
	class cRequestData
	{
	public:
		virtual ~cRequestData() {}  // Force a virtual destructor in all descendants
		
		/// Called when a new chunk of body data is received
		virtual void OnBody(const char * a_Data, int a_Size) = 0;
	} ;
	
	/// The body handler for requests in the "/webadmin" and "/~webadmin" paths
	class cWebadminRequestData :
		public cRequestData,
		public cHTTPFormParser::cCallbacks
	{
	public:
		cHTTPFormParser m_Form;
		
		
		cWebadminRequestData(cHTTPRequest & a_Request) :
			m_Form(a_Request, *this)
		{
		}
		
		// cRequestData overrides:
		virtual void OnBody(const char * a_Data, int a_Size) override;

		// cHTTPFormParser::cCallbacks overrides. Files are ignored:
		virtual void OnFileStart(cHTTPFormParser & a_Parser, const AString & a_FileName) override {}
		virtual void OnFileData(cHTTPFormParser & a_Parser, const char * a_Data, int a_Size) override {}
		virtual void OnFileEnd(cHTTPFormParser & a_Parser) override {}
	} ;
	
	
	/// Set to true if Init() succeeds and the webadmin isn't to be disabled
	bool m_IsInitialized;

	/// The webadmin.ini file, used for the settings and allowed logins
	cIniFile   m_IniFile;
	
	PluginList m_Plugins;

	/// The Lua template script to provide templates:
	cLuaState m_TemplateScript;
	
	/// The HTTP server which provides the underlying HTTP parsing, serialization and events
	cHTTPServer m_HTTPServer;


	AString GetTemplate(void);
	
	/// Handles requests coming to the "/webadmin" or "/~webadmin" URLs
	void HandleWebadminRequest(cHTTPConnection & a_Connection, cHTTPRequest & a_Request);
	
	/// Handles requests for the root page
	void HandleRootRequest(cHTTPConnection & a_Connection, cHTTPRequest & a_Request);
	
	// cHTTPServer::cCallbacks overrides:
	virtual void OnRequestBegun   (cHTTPConnection & a_Connection, cHTTPRequest & a_Request) override;
	virtual void OnRequestBody    (cHTTPConnection & a_Connection, cHTTPRequest & a_Request, const char * a_Data, int a_Size) override;
	virtual void OnRequestFinished(cHTTPConnection & a_Connection, cHTTPRequest & a_Request) override;
} ; // tolua_export





// Revert MSVC warnings back to orignal state:
#if defined(_MSC_VER)
	#pragma warning(pop)
#endif




