
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "SquirrelBindings.h"
#if USE_SQUIRREL
#pragma warning(disable:4100) // Getting A LOT of these warnings from SqPlus
#pragma warning(disable:4127)

#include <sqplus/sqplus.h>
#include <sqplus/SquirrelObject.h>
#include <../squirrel/sqstate.h>
#include <../squirrel/sqvm.h>
#include <stdarg.h>

#include "cPlugin.h"
#include "cPluginManager.h"
#include "cRoot.h"
#include "cPlayer.h"





bool SquirrelBindings::IsBound = false;

bool IsTopClosure( HSQUIRRELVM v )
{
	return ( v->_stack[0]._type == OT_CLOSURE );
}

class __Squirrel_Base_Class	// All inheritable classes should extend this class, as it allows virtual functions to call Squirrel
{
public:
	template<typename T>
	static int ConstructAndDestruct(HSQUIRRELVM v, T* a_Instance, SQRELEASEHOOK a_ReleaseHook )
	{
		LOG("ConstructAndDestruct()");

		StackHandler sa(v);
		HSQOBJECT ho = sa.GetObjectHandle(1); // OT_INSTANCE
		SquirrelObject instance(ho);
		SqPlus::PopulateAncestry(v, instance, a_Instance);
		a_Instance->vm = v;
		a_Instance->obj = instance;

		sq_setinstanceup(v, 1, a_Instance);
		sq_setreleasehook(v, 1, a_ReleaseHook);
		return TRUE;
	}

	HSQUIRRELVM vm;
	SquirrelObject obj;
};

class cPlugin__Squirrel : public cPlugin, public __Squirrel_Base_Class
{
public:
	cPlugin__Squirrel() { SetLanguage( cPlugin::E_SQUIRREL ); }

	bool Initialize() // This is a pure virtual function, so it NEEDS an implementation on the script side or it would be an illegal instance
	{ 
		SqPlus::SquirrelFunction<bool> InitFunc(obj, "Initialize");
		if( !InitFunc.func.IsNull() )
			return InitFunc();
		LOGWARN("cPlugin__Squirrel::Initialize() Pure virtual function called!");	// Spam some errorz to make it clear this function needs to be implemented
		return false;
	}

	static int constructor(HSQUIRRELVM v) {	return ConstructAndDestruct( v, new cPlugin__Squirrel, SqPlus::ReleaseClassPtr<cPlugin__Squirrel>::release ); }

	virtual bool OnChat( const char* a_Chat, cPlayer* a_Player )
	{
		if( !IsTopClosure(vm) ) // Avoid recursion (TODO: FIXME: THIS NEEDS MORE RESEARCH!)
		{	//Called from C++
			return SqPlus::SquirrelFunction<bool>(obj, "OnChat")(a_Chat, a_Player);
		}
		else // Called from Squirrel
		{
			return cPlugin::OnChat(a_Chat, a_Player);
		}
	}
};

static void printFunc(HSQUIRRELVM v,const SQChar * s,...)
{
	(void)v;
	va_list vl;
	va_start(vl,s);
	cMCLogger::GetInstance()->Log( s, vl );
	va_end(vl);
}

DECLARE_INSTANCE_TYPE( cRoot );
DECLARE_INSTANCE_TYPE( cPluginManager );
DECLARE_ENUM_TYPE( cPluginManager::PluginHook );
DECLARE_INSTANCE_TYPE( cPlugin );
DECLARE_INSTANCE_TYPE( cPlugin__Squirrel );

DECLARE_INSTANCE_TYPE( cEntity );
DECLARE_INSTANCE_TYPE( cPawn );
DECLARE_INSTANCE_TYPE( cPlayer );

void SquirrelBindings::Bind( HSQUIRRELVM a_SquirrelVM )
{
	IsBound = true;

	sq_setprintfunc(a_SquirrelVM, printFunc, printFunc);


	SqPlus::SQClassDefNoConstructor<cEntity>("cEntity");
	SqPlus::SQClassDefNoConstructor<cPawn, cEntity>("cPawn", "cEntity");
	SqPlus::SQClassDefNoConstructor<cPlayer, cPawn>("cPlayer", "cPawn"). // All NoConstructor because they need a custom one
		func(&cPlayer::GetName, "GetName");	

	SqPlus::SQClassDefNoConstructor<cPlugin>("cPlugin").
		func(&cPlugin::SetName, "SetName").
		func(&cPlugin::GetName, "GetName").
		func(&cPlugin::GetVersion, "GetVersion").
		func(&cPlugin::OnChat, "OnChat");


	SqPlus::SQClassDef<cPlugin__Squirrel, cPlugin>("cPlugin__Squirrel", "cPlugin").
		staticFunc(&cPlugin__Squirrel::constructor, "constructor");
	

	SqPlus::SQClassDefNoConstructor<cRoot>("cRoot").
		staticFunc(&cRoot::Get, "Get").
		func(static_cast<cPluginManager* (__thiscall cRoot::*)(void)>(&cRoot::GetPluginManager), "GetPluginManager");

	
	SqPlus::SQClassDefNoConstructor<cPluginManager>("cPluginManager").
		overloadFunc<bool (cPluginManager::*)(cPlugin*)>(&cPluginManager::AddPlugin, "AddPlugin").
		func(&cPluginManager::GetPlugin, "GetPlugin").
		func(&cPluginManager::AddHook, "AddHook").
		enumInt( cPluginManager::E_PLUGIN_CHAT, "E_PLUGIN_CHAT");

	

}

#endif
