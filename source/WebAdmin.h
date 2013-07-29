#pragma once

#include "../WebServer/WebServer.h"
#include "OSSupport/Socket.h"

class cStringMap;
class cLuaScript;

struct HTTPFormData     // tolua_export
{                       // tolua_export
	std::string Name;   // tolua_export
	std::string Value;  // tolua_export
	std::string Type;   // tolua_export
};// tolua_export

struct HTTPRequest  // tolua_export
{                   // tolua_export
	typedef std::map< std::string, std::string > StringStringMap;
	typedef std::map< std::string, HTTPFormData > FormDataMap;
	AString Method;             // tolua_export
	AString Path;               // tolua_export
	StringStringMap Params;     // >> EXPORTED IN MANUALBINDINGS <<
	StringStringMap PostParams; // >> EXPORTED IN MANUALBINDINGS <<
	AString Username;           // tolua_export
	FormDataMap FormData;       // >> EXPORTED IN MANUALBINDINGS <<
}; // tolua_export

struct HTTPTemplateRequest // tolua_export
{                          // tolua_export
	HTTPRequest Request;   // tolua_export

}; // tolua_export

// tolua_begin
struct sWebAdminPage
{
	AString Content;
	AString PluginName;
	AString TabName;
};
// tolua_end

struct lua_State;
class cEvent;
class cIniFile;
class cWebPlugin;

class cWebAdmin // tolua_export
{               // tolua_export
public:         // tolua_export
	cWebAdmin( int a_Port = 8080 );
	~cWebAdmin();

	bool                Init( int a_Port );

	void                AddPlugin( cWebPlugin* a_Plugin );
	void                RemovePlugin( cWebPlugin* a_Plugin );

	typedef std::list< cWebPlugin* > PluginList;

	// TODO: Convert this to the auto-locking callback mechanism used for looping players in worlds and such
	PluginList          GetPlugins() const { return m_Plugins; } // >> EXPORTED IN MANUALBINDINGS <<

	static void         Request_Handler(webserver::http_request* r);

	int                 GetPort() { return m_Port; }  // tolua_export

	sWebAdminPage       GetPage(const HTTPRequest& a_Request);  // tolua_export
	AString             GetBaseURL(const AString& a_URL);  // tolua_export
	AString             GetBaseURL(const AStringVector& a_URLSplit);

	static AString      GetMemoryUsage(void);  // tolua_export
private:

#ifdef _WIN32
	static DWORD WINAPI ListenThread(LPVOID lpParam);
#else
	static void *       ListenThread( void *lpParam );
#endif

	AString             GetTemplate();

	cLuaScript*         m_pTemplate;

	int                 m_Port;

	bool                m_bConnected;
	cSocket             m_ListenSocket;

	cIniFile*           m_IniFile;
	PluginList          m_Plugins;

	cEvent*             m_Event;

	webserver*          m_WebServer;
}; // tolua_export