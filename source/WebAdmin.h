#pragma once

#include "../WebServer/WebServer.h"
#include "OSSupport/Socket.h"
#include "LuaState.h"




// fwd:
class cStringMap;
class cEvent;
class cIniFile;
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
class cWebAdmin
{
public:
	// tolua_end
	
	typedef std::list< cWebPlugin* > PluginList;


	cWebAdmin( int a_Port = 8080 );
	~cWebAdmin();

	bool                Init( int a_Port );

	void                AddPlugin( cWebPlugin* a_Plugin );
	void                RemovePlugin( cWebPlugin* a_Plugin );

	// TODO: Convert this to the auto-locking callback mechanism used for looping players in worlds and such
	PluginList          GetPlugins() const { return m_Plugins; } // >> EXPORTED IN MANUALBINDINGS <<

	static void         Request_Handler(webserver::http_request* r);

	// tolua_begin
	static AString      GetMemoryUsage(void);

	int                 GetPort() { return m_Port; }

	sWebAdminPage       GetPage(const HTTPRequest& a_Request);
	AString             GetBaseURL(const AString& a_URL);
	
	// tolua_end

	AString             GetBaseURL(const AStringVector& a_URLSplit);

	
private:
	int m_Port;

	bool    m_bConnected;
	cSocket m_ListenSocket;

	cIniFile * m_IniFile;
	PluginList m_Plugins;

	cEvent * m_Event;

	webserver * m_WebServer;

	/// The Lua template script to provide templates:
	cLuaState m_TemplateScript;


	#ifdef _WIN32
		static DWORD WINAPI ListenThread(LPVOID lpParam);
	#else
		static void *       ListenThread(void * lpParam);
	#endif

	AString GetTemplate();
} ; // tolua_export




