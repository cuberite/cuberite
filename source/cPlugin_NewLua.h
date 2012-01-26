#pragma once

#include "cPlugin.h"
#include <string>

typedef struct lua_State lua_State;

class cPlugin_NewLua : public cPlugin						//tolua_export
{															//tolua_export
public:														//tolua_export
	cPlugin_NewLua( const char* a_PluginName );
	~cPlugin_NewLua();

	virtual bool Initialize();								//tolua_export
	virtual void Tick(float a_Dt);							//tolua_export
	virtual bool OnPlayerJoin( cPlayer* a_Player );			//tolua_export
	virtual bool OnLogin( cPacket_Login* a_PacketData );	//tolua_export
	virtual bool OnBlockPlace( cPacket_BlockPlace* a_PacketData, cPlayer* a_Player ); // tolua_export
	virtual bool OnKilled( cPawn* a_Killed, cEntity* a_Killer ); //tolua_export

private:
	bool PushFunction( const char* a_FunctionName );
	bool CallFunction( int a_NumArgs, int a_NumResults, const char* a_FunctionName ); // a_FunctionName is only used for error messages, nothing else

	std::string m_Directory;
	lua_State* m_LuaState;
};//tolua_export