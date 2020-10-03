
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "ManualBindings.h"
#undef TOLUA_TEMPLATE_BIND
#include <sstream>
#include <iomanip>
#include "tolua++/include/tolua++.h"
#include "mbedtls/md5.h"
#include "mbedtls/sha1.h"
#include "PluginLua.h"
#include "PluginManager.h"
#include "LuaWindow.h"
#include "../BlockArea.h"
#include "../BlockEntities/BeaconEntity.h"
#include "../BlockEntities/BrewingstandEntity.h"
#include "../BlockEntities/ChestEntity.h"
#include "../BlockEntities/CommandBlockEntity.h"
#include "../BlockEntities/DispenserEntity.h"
#include "../BlockEntities/DropperEntity.h"
#include "../BlockEntities/FurnaceEntity.h"
#include "../BlockEntities/HopperEntity.h"
#include "../BlockEntities/NoteEntity.h"
#include "../BlockEntities/MobHeadEntity.h"
#include "../BlockEntities/FlowerPotEntity.h"
#include "../BoundingBox.h"
#include "BuildInfo.h"
#include "../ClientHandle.h"
#include "../CommandOutput.h"
#include "../CompositeChat.h"
#include "../Entities/Player.h"
#include "../Generating/ChunkDesc.h"
#include "../HTTP/UrlParser.h"
#include "../Item.h"
#include "../LineBlockTracer.h"
#include "../Server.h"
#include "../Root.h"
#include "../StringCompression.h"
#include "../WebAdmin.h"
#include "../World.h"





// Hotpatching the Macro to prevent a Clang Warning (0 for pointer used)
#undef  lua_tostring
#define lua_tostring(L, i) lua_tolstring(L, (i), nullptr)





////////////////////////////////////////////////////////////////////////////////
// LuaCommandHandler:

/** Defines a bridge between cPluginManager::cCommandHandler and cLuaState::cCallback. */
class LuaCommandHandler:
	public cPluginManager::cCommandHandler
{
public:
	LuaCommandHandler(cLuaState::cCallbackPtr && a_Callback):
		m_Callback(std::move(a_Callback))
	{
	}

	virtual bool ExecuteCommand(
		const AStringVector & a_Split,
		cPlayer * a_Player,
		const AString & a_Command,
		cCommandOutputCallback * a_Output
	) override
	{
		bool res = false;
		AString s;
		if (!m_Callback->Call(a_Split, a_Player, a_Command, cLuaState::Return, res, s))
		{
			return false;
		}
		if (res && (a_Output != nullptr) && !s.empty())
		{
			a_Output->Out(s);
		}
		return res;
	}

protected:
	cLuaState::cCallbackPtr m_Callback;
};





////////////////////////////////////////////////////////////////////////////////
// cManualBindings:

// Better error reporting for Lua
int cManualBindings::tolua_do_error(lua_State * L, const char * a_pMsg, tolua_Error * a_pToLuaError)
{
	// Retrieve current function name
	lua_Debug entry;
	VERIFY(lua_getstack(L, 0, &entry));
	VERIFY(lua_getinfo(L, "n", &entry));

	// Insert function name into error msg
	AString msg(a_pMsg);
	ReplaceString(msg, "#funcname#", entry.name?entry.name:"?");

	// Send the error to Lua
	tolua_error(L, msg.c_str(), a_pToLuaError);
	return 0;
}





int cManualBindings::vlua_do_error(lua_State * L, const char * a_pFormat, fmt::printf_args a_ArgList)
{
	// Retrieve current function name
	lua_Debug entry;
	VERIFY(lua_getstack(L, 0, &entry));
	VERIFY(lua_getinfo(L, "n", &entry));

	// Insert function name into error msg
	AString msg(a_pFormat);
	ReplaceString(msg, "#funcname#", (entry.name != nullptr) ? entry.name : "?");

	// Copied from luaL_error and modified
	luaL_where(L, 1);
	AString FmtMsg = vPrintf(msg.c_str(), a_ArgList);
	lua_pushlstring(L, FmtMsg.data(), FmtMsg.size());
	lua_concat(L, 2);
	return lua_error(L);
}





// Lua bound functions with special return types
static int tolua_Clamp(lua_State * tolua_S)
{
	cLuaState LuaState(tolua_S);
	int NumArgs = lua_gettop(LuaState);
	if (NumArgs != 3)
	{
		return cManualBindings::lua_do_error(LuaState, "Error in function call '#funcname#': Requires 3 arguments, got %i", NumArgs);
	}

	if (!lua_isnumber(LuaState, 1) || !lua_isnumber(LuaState, 2) || !lua_isnumber(LuaState, 3))
	{
		return cManualBindings::lua_do_error(LuaState, "Error in function call '#funcname#': Expected a number for parameters #1, #2 and #3");
	}

	lua_Number Number = tolua_tonumber(LuaState, 1, 0);
	lua_Number Min = tolua_tonumber(LuaState, 2, 0);
	lua_Number Max = tolua_tonumber(LuaState, 3, 0);

	lua_Number Result = Clamp(Number, Min, Max);
	LuaState.Push(Result);
	return 1;
}





static int tolua_CompressStringZLIB(lua_State * tolua_S)
{
	cLuaState S(tolua_S);
	if (
		!S.CheckParamString(1) ||
		(
			!S.CheckParamNumber(2) &&
			!S.CheckParamEnd(2)
		)
	)
	{
		cLuaState::LogStackTrace(tolua_S);
		return 0;
	}

	// Get the params:
	AString ToCompress;
	int CompressionLevel = 5;
	S.GetStackValues(1, ToCompress, CompressionLevel);

	// Compress the string:
	AString res;
	CompressString(ToCompress.data(), ToCompress.size(), res, CompressionLevel);
	S.Push(res);
	return 1;
}





static int tolua_UncompressStringZLIB(lua_State * tolua_S)
{
	cLuaState S(tolua_S);
	if (
		!S.CheckParamString(1) ||
		!S.CheckParamNumber(2)
	)
	{
		cLuaState::LogStackTrace(tolua_S);
		return 0;
	}

	// Get the params:
	AString ToUncompress;
	size_t UncompressedSize = 0;
	S.GetStackValues(1, ToUncompress, UncompressedSize);

	// Compress the string:
	AString res;
	UncompressString(ToUncompress.data(), ToUncompress.size(), res, UncompressedSize);
	S.Push(res);
	return 1;
}





static int tolua_CompressStringGZIP(lua_State * tolua_S)
{
	cLuaState S(tolua_S);
	if (
		!S.CheckParamString(1) ||
		!S.CheckParamEnd(2)
	)
	{
		cLuaState::LogStackTrace(tolua_S);
		return 0;
	}

	// Get the params:
	AString ToCompress;
	S.GetStackValues(1, ToCompress);

	// Compress the string:
	AString res;
	CompressStringGZIP(ToCompress.data(), ToCompress.size(), res);
	S.Push(res);
	return 1;
}





static int tolua_UncompressStringGZIP(lua_State * tolua_S)
{
	cLuaState S(tolua_S);
	if (
		!S.CheckParamString(1) ||
		!S.CheckParamEnd(2)
	)
	{
		cLuaState::LogStackTrace(tolua_S);
		return 0;
	}

	// Get the params:
	AString ToUncompress;
	S.GetStackValues(1, ToUncompress);

	// Compress the string:
	AString res;
	UncompressStringGZIP(ToUncompress.data(), ToUncompress.size(), res);
	S.Push(res);
	return 1;
}





static int tolua_InflateString(lua_State * tolua_S)
{
	cLuaState S(tolua_S);
	if (
		!S.CheckParamString(1) ||
		!S.CheckParamEnd(2)
	)
	{
		cLuaState::LogStackTrace(tolua_S);
		return 0;
	}

	// Get the params:
	AString ToUncompress;
	S.GetStackValues(1, ToUncompress);

	// Compress the string:
	AString res;
	InflateString(ToUncompress.data(), ToUncompress.size(), res);
	S.Push(res);
	return 1;
}





static int tolua_StringSplit(lua_State * tolua_S)
{
	// Get the params:
	cLuaState LuaState(tolua_S);
	AString str, delim;
	LuaState.GetStackValues(1, str, delim);

	// Execute and push the result:
	LuaState.Push(StringSplit(str, delim));
	return 1;
}





static int tolua_StringSplitWithQuotes(lua_State * tolua_S)
{
	cLuaState S(tolua_S);

	AString str;
	AString delim;

	S.GetStackValues(1, str, delim);

	AStringVector Split = StringSplitWithQuotes(str, delim);
	S.Push(Split);
	return 1;
}





static int tolua_StringSplitAndTrim(lua_State * tolua_S)
{
	// Check params:
	cLuaState L(tolua_S);
	if (
		!L.CheckParamString(1, 2) ||
		!L.CheckParamEnd(3)
	)
	{
		return 0;
	}

	// Process:
	AString str, delim;
	L.GetStackValues(1, str, delim);
	L.Push(StringSplitAndTrim(str, delim));
	return 1;
}





/** Retrieves the log message from the first param on the Lua stack.
Can take either a string or a cCompositeChat.
*/
static void LogFromLuaStack(lua_State * tolua_S, eLogLevel a_LogLevel)
{
	tolua_Error err;
	if (tolua_isusertype(tolua_S, 1, "cCompositeChat", false, &err))
	{
		auto Msg = static_cast<cCompositeChat *>(tolua_tousertype(tolua_S, 1, nullptr))->ExtractText();
		Logger::LogSimple(Msg, a_LogLevel);
		return;
	}

	size_t len = 0;
	const char * str = lua_tolstring(tolua_S, 1, &len);
	Logger::LogSimple(std::string_view(str, len), a_LogLevel);
}





static int tolua_LOG(lua_State * tolua_S)
{
	// If there's no param, spit out an error message instead of crashing:
	if (lua_isnil(tolua_S, 1))
	{
		LOGWARNING("Attempting to LOG a nil value!");
		cLuaState::LogStackTrace(tolua_S);
		return 0;
	}

	// If the param is a cCompositeChat, read the log level from it:
	eLogLevel LogLevel = eLogLevel::Regular;
	tolua_Error err;
	if (tolua_isusertype(tolua_S, 1, "cCompositeChat", false, &err))
	{
		LogLevel = cCompositeChat::MessageTypeToLogLevel(
			static_cast<cCompositeChat *>(tolua_tousertype(tolua_S, 1, nullptr))->GetMessageType()
		);
	}

	// Log the message:
	LogFromLuaStack(tolua_S, LogLevel);
	return 0;
}





static int tolua_LOGINFO(lua_State * tolua_S)
{
	// If there's no param, spit out an error message instead of crashing:
	if (lua_isnil(tolua_S, 1))
	{
		LOGWARNING("Attempting to LOGINFO a nil value!");
		cLuaState::LogStackTrace(tolua_S);
		return 0;
	}

	LogFromLuaStack(tolua_S, eLogLevel::Info);
	return 0;
}





static int tolua_LOGWARN(lua_State * tolua_S)
{
	// If there's no param, spit out an error message instead of crashing:
	if (lua_isnil(tolua_S, 1))
	{
		LOGWARNING("Attempting to LOGWARN a nil value!");
		cLuaState::LogStackTrace(tolua_S);
		return 0;
	}

	LogFromLuaStack(tolua_S, eLogLevel::Warning);
	return 0;
}





static int tolua_LOGERROR(lua_State * tolua_S)
{
	// If there's no param, spit out an error message instead of crashing:
	if (lua_isnil(tolua_S, 1))
	{
		LOGWARNING("Attempting to LOGERROR a nil value!");
		cLuaState::LogStackTrace(tolua_S);
		return 0;
	}

	LogFromLuaStack(tolua_S, eLogLevel::Error);
	return 0;
}





static int tolua_Base64Encode(lua_State * tolua_S)
{
	cLuaState L(tolua_S);
	if (
		!L.CheckParamString(1) ||
		!L.CheckParamEnd(2)
	)
	{
		return 0;
	}

	AString Src;
	L.GetStackValue(1, Src);
	AString res = Base64Encode(Src);
	L.Push(res);
	return 1;
}





static int tolua_Base64Decode(lua_State * tolua_S)
{
	cLuaState L(tolua_S);
	if (
		!L.CheckParamString(1) ||
		!L.CheckParamEnd(2)
	)
	{
		return 0;
	}

	AString Src;
	L.GetStackValue(1, Src);
	AString res = Base64Decode(Src);
	L.Push(res);
	return 1;
}





cPluginLua * cManualBindings::GetLuaPlugin(lua_State * L)
{
	// Get the plugin identification out of LuaState:
	lua_getglobal(L, LUA_PLUGIN_INSTANCE_VAR_NAME);
	if (!lua_islightuserdata(L, -1))
	{
		LOGWARNING("%s: cannot get plugin instance, what have you done to my Lua state?", __FUNCTION__);
		lua_pop(L, 1);
		return nullptr;
	}
	cPluginLua * Plugin = static_cast<cPluginLua *>(const_cast<void*>(lua_topointer(L, -1)));
	lua_pop(L, 1);

	return Plugin;
}





static int tolua_cFile_ChangeFileExt(lua_State * tolua_S)
{
	// API signature:
	// ChangeFileExt(string, string) -> string

	// Check params:
	cLuaState L(tolua_S);
	if (
		!L.CheckParamUserTable(1, "cFile") ||
		!L.CheckParamString(2, 3) ||
		!L.CheckParamEnd(4)
	)
	{
		return 0;
	}

	// Execute:
	AString FileName, NewExt;
	VERIFY(L.GetStackValues(2, FileName, NewExt));
	L.Push(cFile::ChangeFileExt(FileName, NewExt));
	return 1;
}





static int tolua_cFile_Copy(lua_State * tolua_S)
{
	// API signature:
	// cFile:Copy(string, string) -> bool

	// Check params:
	cLuaState L(tolua_S);
	if (
		!L.CheckParamUserTable(1, "cFile") ||
		!L.CheckParamString(2, 3) ||
		!L.CheckParamEnd(4)
	)
	{
		return 0;
	}

	// Execute:
	AString SrcFile, DstFile;
	VERIFY(L.GetStackValues(2, SrcFile, DstFile));
	L.Push(cFile::Copy(SrcFile, DstFile));
	return 1;
}





static int tolua_cFile_CreateFolder(lua_State * tolua_S)
{
	// API signature:
	// cFile:CreateFolder(string) -> bool

	// Check params:
	cLuaState L(tolua_S);
	if (
		!L.CheckParamUserTable(1, "cFile") ||
		!L.CheckParamString(2) ||
		!L.CheckParamEnd(3)
	)
	{
		return 0;
	}

	// Execute:
	AString FolderPath;
	VERIFY(L.GetStackValues(2, FolderPath));
	L.Push(cFile::CreateFolder(FolderPath));
	return 1;
}





static int tolua_cFile_CreateFolderRecursive(lua_State * tolua_S)
{
	// API signature:
	// cFile:CreateFolderRecursive(string) -> bool

	// Check params:
	cLuaState L(tolua_S);
	if (
		!L.CheckParamUserTable(1, "cFile") ||
		!L.CheckParamString(2) ||
		!L.CheckParamEnd(3)
	)
	{
		return 0;
	}

	// Execute:
	AString FolderPath;
	VERIFY(L.GetStackValues(2, FolderPath));
	L.Push(cFile::CreateFolderRecursive(FolderPath));
	return 1;
}





static int tolua_cFile_Delete(lua_State * tolua_S)
{
	// API signature:
	// cFile:Delete(string) -> bool

	// Check params:
	cLuaState L(tolua_S);
	if (
		!L.CheckParamUserTable(1, "cFile") ||
		!L.CheckParamString(2) ||
		!L.CheckParamEnd(3)
	)
	{
		return 0;
	}

	// Execute:
	AString Path;
	VERIFY(L.GetStackValues(2, Path));
	L.Push(cFile::Delete(Path));
	return 1;
}





static int tolua_cFile_DeleteFile(lua_State * tolua_S)
{
	// API signature:
	// cFile:DeleteFile(string) -> bool

	// Check params:
	cLuaState L(tolua_S);
	if (
		!L.CheckParamUserTable(1, "cFile") ||
		!L.CheckParamString(2) ||
		!L.CheckParamEnd(3)
	)
	{
		return 0;
	}

	// Execute:
	AString Path;
	VERIFY(L.GetStackValues(2, Path));
	L.Push(cFile::DeleteFile(Path));
	return 1;
}





static int tolua_cFile_DeleteFolder(lua_State * tolua_S)
{
	// API signature:
	// cFile:DeleteFolder(string) -> bool

	// Check params:
	cLuaState L(tolua_S);
	if (
		!L.CheckParamUserTable(1, "cFile") ||
		!L.CheckParamString(2) ||
		!L.CheckParamEnd(3)
	)
	{
		return 0;
	}

	// Execute:
	AString Path;
	VERIFY(L.GetStackValues(2, Path));
	L.Push(cFile::DeleteFolder(Path));
	return 1;
}





static int tolua_cFile_DeleteFolderContents(lua_State * tolua_S)
{
	// API signature:
	// cFile:DeleteFolderContents(string) -> bool

	// Check params:
	cLuaState L(tolua_S);
	if (
		!L.CheckParamUserTable(1, "cFile") ||
		!L.CheckParamString(2) ||
		!L.CheckParamEnd(3)
	)
	{
		return 0;
	}

	// Execute:
	AString Path;
	VERIFY(L.GetStackValues(2, Path));
	L.Push(cFile::DeleteFolderContents(Path));
	return 1;
}





static int tolua_cFile_Exists(lua_State * tolua_S)
{
	// API signature:
	// cFile:Exists(string) -> bool

	// Obsolete, use IsFile() or IsFolder() instead
	cLuaState L(tolua_S);
	LOGWARNING("cFile:Exists() is obsolete, use cFile:IsFolder() or cFile:IsFile() instead!");
	L.LogStackTrace();

	// Check params:
	if (
		!L.CheckParamUserTable(1, "cFile") ||
		!L.CheckParamString(2) ||
		!L.CheckParamEnd(3)
	)
	{
		return 0;
	}

	// Execute:
	AString Path;
	VERIFY(L.GetStackValues(2, Path));
	L.Push(cFile::Exists(Path));
	return 1;
}





static int tolua_cFile_GetFolderContents(lua_State * tolua_S)
{
	// API signature:
	// cFile:GetFolderContents(string) -> {string, string, ...}

	// Check params:
	cLuaState LuaState(tolua_S);
	if (
		!LuaState.CheckParamUserTable(1, "cFile") ||
		!LuaState.CheckParamString   (2) ||
		!LuaState.CheckParamEnd      (3)
	)
	{
		return 0;
	}

	// Get params:
	AString Folder;
	VERIFY(LuaState.GetStackValues(2, Folder));

	// Execute and push result:
	LuaState.Push(cFile::GetFolderContents(Folder));
	return 1;
}





static int tolua_cFile_GetLastModificationTime(lua_State * tolua_S)
{
	// API signature:
	// cFile:GetLastModificationTime(string) -> number

	// Check params:
	cLuaState L(tolua_S);
	if (
		!L.CheckParamUserTable(1, "cFile") ||
		!L.CheckParamString(2) ||
		!L.CheckParamEnd(3)
	)
	{
		return 0;
	}

	// Execute:
	AString Path;
	VERIFY(L.GetStackValues(2, Path));
	L.Push(cFile::GetLastModificationTime(Path));
	return 1;
}





static int tolua_cFile_GetSize(lua_State * tolua_S)
{
	// API signature:
	// cFile:GetSize(string) -> number

	// Check params:
	cLuaState L(tolua_S);
	if (
		!L.CheckParamUserTable(1, "cFile") ||
		!L.CheckParamString(2) ||
		!L.CheckParamEnd(3)
	)
	{
		return 0;
	}

	// Execute:
	AString Path;
	VERIFY(L.GetStackValues(2, Path));
	L.Push(cFile::GetSize(Path));
	return 1;
}





static int tolua_cFile_IsFile(lua_State * tolua_S)
{
	// API signature:
	// cFile:IsFile(string) -> bool

	// Check params:
	cLuaState L(tolua_S);
	if (
		!L.CheckParamUserTable(1, "cFile") ||
		!L.CheckParamString(2) ||
		!L.CheckParamEnd(3)
	)
	{
		return 0;
	}

	// Execute:
	AString Path;
	VERIFY(L.GetStackValues(2, Path));
	L.Push(cFile::IsFile(Path));
	return 1;
}





static int tolua_cFile_IsFolder(lua_State * tolua_S)
{
	// API signature:
	// cFile:IsFolder(string) -> bool

	// Check params:
	cLuaState L(tolua_S);
	if (
		!L.CheckParamUserTable(1, "cFile") ||
		!L.CheckParamString(2) ||
		!L.CheckParamEnd(3)
	)
	{
		return 0;
	}

	// Execute:
	AString Path;
	VERIFY(L.GetStackValues(2, Path));
	L.Push(cFile::IsFolder(Path));
	return 1;
}





static int tolua_cFile_ReadWholeFile(lua_State * tolua_S)
{
	// API signature:
	// cFile:ReadWholeFile(string) -> string

	// Check params:
	cLuaState LuaState(tolua_S);
	if (
		!LuaState.CheckParamUserTable(1, "cFile") ||
		!LuaState.CheckParamString   (2) ||
		!LuaState.CheckParamEnd      (3)
	)
	{
		return 0;
	}

	// Get params:
	AString FileName;
	VERIFY(LuaState.GetStackValues(2, FileName));

	// Execute and push result:
	LuaState.Push(cFile::ReadWholeFile(FileName));
	return 1;
}





static int tolua_cFile_Rename(lua_State * tolua_S)
{
	// API signature:
	// cFile:Rename(string, string) -> bool

	// Check params:
	cLuaState L(tolua_S);
	if (
		!L.CheckParamUserTable(1, "cFile") ||
		!L.CheckParamString(2, 3) ||
		!L.CheckParamEnd(4)
	)
	{
		return 0;
	}

	// Execute:
	AString SrcPath, DstPath;
	VERIFY(L.GetStackValues(2, SrcPath, DstPath));
	L.Push(cFile::Rename(SrcPath, DstPath));
	return 1;
}





static int tolua_cPluginManager_GetAllPlugins(lua_State * tolua_S)
{
	// API function no longer available:
	LOGWARNING("cPluginManager:GetAllPlugins() is no longer available, use cPluginManager:ForEachPlugin() instead");
	cLuaState::LogStackTrace(tolua_S);

	// Return an empty table:
	lua_newtable(tolua_S);
	return 1;
}





static int tolua_cPluginManager_GetCurrentPlugin(lua_State * S)
{
	cPluginLua * Plugin = cManualBindings::GetLuaPlugin(S);
	if (Plugin == nullptr)
	{
		// An error message has already been printed in GetLuaPlugin()
		return 0;
	}
	tolua_pushusertype(S, Plugin, "cPluginLua");
	return 1;
}





static int tolua_cPluginManager_GetPlugin(lua_State * tolua_S)
{
	// API function no longer available:
	LOGWARNING("cPluginManager:GetPlugin() is no longer available. Use cPluginManager:DoWithPlugin() or cPluginManager:CallPlugin() instead.");
	cLuaState::LogStackTrace(tolua_S);
	return 0;
}





static int tolua_cPluginManager_LogStackTrace(lua_State * S)
{
	cLuaState::LogStackTrace(S);
	return 0;
}





static int tolua_cPluginManager_AddHook_FnRef(cPluginManager * a_PluginManager, cLuaState & S, int a_ParamIdx)
{
	// Helper function for cPluginmanager:AddHook() binding
	// Takes care of the new case (#121): args are HOOK_TYPE and CallbackFunction
	// The arg types have already been checked

	// Retrieve the cPlugin from the LuaState:
	cPluginLua * Plugin = cManualBindings::GetLuaPlugin(S);
	if (Plugin == nullptr)
	{
		// An error message has been already printed in GetLuaPlugin()
		return 0;
	}

	// Retrieve and check the hook type
	int HookType;
	if (!S.GetStackValue(a_ParamIdx, HookType))
	{
		LOGWARNING("cPluginManager.AddHook(): Cannot read the hook type.");
		S.LogStackTrace();
		return 0;
	}
	if (!a_PluginManager->IsValidHookType(HookType))
	{
		LOGWARNING("cPluginManager.AddHook(): Invalid HOOK_TYPE parameter: %d", HookType);
		S.LogStackTrace();
		return 0;
	}

	// Add the hook to the plugin
	cLuaState::cCallbackPtr callback;
	if (!S.GetStackValue(a_ParamIdx + 1, callback))
	{
		LOGWARNING("cPluginManager.AddHook(): Cannot read the callback parameter");
		S.LogStackTrace();
		return 0;
	}
	if (!Plugin->AddHookCallback(HookType, std::move(callback)))
	{
		LOGWARNING("cPluginManager.AddHook(): Cannot add hook %d, unknown error.", HookType);
		S.LogStackTrace();
		return 0;
	}
	a_PluginManager->AddHook(Plugin, HookType);

	// Success
	return 0;
}





static int tolua_cPluginManager_AddHook_DefFn(cPluginManager * a_PluginManager, cLuaState & S, int a_ParamIdx)
{
	// Helper function for cPluginmanager:AddHook() binding
	// Takes care of the old case (#121): args are cPluginLua and HOOK_TYPE
	// The arg types have already been checked

	// Retrieve and check the cPlugin parameter
	cPluginLua * Plugin = static_cast<cPluginLua *>(tolua_tousertype(S, a_ParamIdx, nullptr));
	if (Plugin == nullptr)
	{
		LOGWARNING("cPluginManager.AddHook(): Invalid Plugin parameter, expected a valid cPlugin object. Hook not added");
		S.LogStackTrace();
		return 0;
	}
	if (Plugin != cManualBindings::GetLuaPlugin(S))
	{
		// The plugin parameter passed to us is not our stored plugin. Disallow this!
		LOGWARNING("cPluginManager.AddHook(): Invalid Plugin parameter, cannot add hook to foreign plugins. Hook not added.");
		S.LogStackTrace();
		return 0;
	}

	// Retrieve and check the hook type
	int HookType = static_cast<int>(tolua_tonumber(S, a_ParamIdx + 1, -1));
	if (!a_PluginManager->IsValidHookType(HookType))
	{
		LOGWARNING("cPluginManager.AddHook(): Invalid HOOK_TYPE parameter: %d", HookType);
		S.LogStackTrace();
		return 0;
	}

	// Get the standard name for the callback function:
	const char * FnName = cPluginLua::GetHookFnName(HookType);
	if (FnName == nullptr)
	{
		LOGWARNING("cPluginManager.AddHook(): Unknown hook type (%d). Hook not added.", HookType);
		S.LogStackTrace();
		return 0;
	}

	// Retrieve the function to call and add it to the plugin:
	cLuaState::cCallbackPtr callback;
	lua_getglobal(S, FnName);
	bool res = S.GetStackValue(-1, callback);
	lua_pop(S, 1);
	if (!res || !callback->IsValid())
	{
		LOGWARNING("cPluginManager.AddHook(): Function %s not found. Hook not added.", FnName);
		S.LogStackTrace();
		return 0;
	}
	a_PluginManager->AddHook(Plugin, HookType);

	// Success
	return 0;
}





static int tolua_cPluginManager_AddHook(lua_State * tolua_S)
{
	/*
	Function signatures:
	cPluginManager:AddHook(HOOK_TYPE, CallbackFunction)        -- (1) recommended
	cPluginManager.AddHook(HOOK_TYPE, CallbackFunction)        -- (2) accepted silently (#401 deprecates this)
	cPluginManager:Get():AddHook(HOOK_TYPE, CallbackFunction)  -- (3) accepted silently
	cPluginManager:Get():AddHook(Plugin, HOOK_TYPE)            -- (4) old style (#121), accepted but complained about in the console
	cPluginManager.AddHook(Plugin, HOOK_TYPE)                  -- (5) old style (#121) mangled, accepted but complained about in the console
	*/

	cLuaState S(tolua_S);
	cPluginManager * PlgMgr = cPluginManager::Get();

	// If the first param is a cPluginManager instance, use it instead of the global one:
	int ParamIdx = 1;
	tolua_Error err;
	if (tolua_isusertype(S, 1, "cPluginManager", 0, &err))
	{
		// Style 2 or 3, retrieve the PlgMgr instance
		PlgMgr = static_cast<cPluginManager *>(tolua_tousertype(S, 1, nullptr));
		if (PlgMgr == nullptr)
		{
			LOGWARNING("Malformed plugin, use cPluginManager.AddHook(HOOK_TYPE, CallbackFunction). Fixing the call for you.");
			S.LogStackTrace();
			PlgMgr = cPluginManager::Get();
		}
		ParamIdx += 1;
	}
	else if (tolua_isusertable(S, 1, "cPluginManager", 0, &err))
	{
		// Style 1, use the global PlgMgr, but increment ParamIdx
		ParamIdx++;
	}

	if (lua_isnumber(S, ParamIdx) && lua_isfunction(S, ParamIdx + 1))
	{
		// The next params are a number and a function, assume style 1 or 2
		return tolua_cPluginManager_AddHook_FnRef(PlgMgr, S, ParamIdx);
	}
	else if (tolua_isusertype(S, ParamIdx, "cPlugin", 0, &err) && lua_isnumber(S, ParamIdx + 1))
	{
		// The next params are a cPlugin and a number, assume style 3 or 4
		LOGINFO("cPluginManager.AddHook(): Deprecated format used, use cPluginManager.AddHook(HOOK_TYPE, CallbackFunction) instead. Fixing the call for you.");
		S.LogStackTrace();
		return tolua_cPluginManager_AddHook_DefFn(PlgMgr, S, ParamIdx);
	}

	AString ParamDesc;
	Printf(ParamDesc, "%s, %s, %s", S.GetTypeText(1).c_str(), S.GetTypeText(2).c_str(), S.GetTypeText(3).c_str());
	LOGWARNING("cPluginManager:AddHook(): bad parameters. Expected HOOK_TYPE and CallbackFunction, got %s. Hook not added.", ParamDesc.c_str());
	S.LogStackTrace();
	return 0;
}





static int tolua_cPluginManager_ForEachCommand(lua_State * tolua_S)
{
	/*
	Function signature:
	cPluginManager:Get():ForEachCommand(a_CallbackFn) -> bool
	*/

	// Check params:
	cLuaState L(tolua_S);
	if (
		!L.CheckParamUserType(1, "cPluginManager") ||
		!L.CheckParamFunction(2) ||
		!L.CheckParamEnd(3)
	)
	{
		return 0;
	}

	// Get the params:
	cLuaState::cRef FnRef;
	L.GetStackValues(2, FnRef);
	if (!FnRef.IsValid())
	{
		LOGWARN("Error in function call 'ForEachCommand': Could not get function reference of parameter #1");
		return 0;
	}

	// Callback for enumerating all commands:
	class cLuaCallback : public cPluginManager::cCommandEnumCallback
	{
	public:
		cLuaCallback(cLuaState & a_LuaState, cLuaState::cRef & a_FnRef):
			m_LuaState(a_LuaState),
			m_FnRef(a_FnRef)
		{
		}

	private:
		virtual bool Command(const AString & a_Command, const cPlugin * a_Plugin, const AString & a_Permission, const AString & a_HelpString) override
		{
			UNUSED(a_Plugin);
			bool ret = false;
			m_LuaState.Call(m_FnRef, a_Command, a_Permission, a_HelpString, cLuaState::Return, ret);
			return ret;
		}
		cLuaState & m_LuaState;
		cLuaState::cRef & m_FnRef;
	} Callback(L, FnRef);

	// Execute and push the returned value:
	L.Push(cPluginManager::Get()->ForEachCommand(Callback));
	return 1;
}





static int tolua_cPluginManager_ForEachConsoleCommand(lua_State * tolua_S)
{
	/*
	Function signature:
	cPluginManager:Get():ForEachConsoleCommand(a_CallbackFn) -> bool
	*/

	// Check params:
	cLuaState L(tolua_S);
	if (
		!L.CheckParamUserType(1, "cPluginManager") ||
		!L.CheckParamFunction(2) ||
		!L.CheckParamEnd(3)
	)
	{
		return 0;
	}

	// Get the params:
	cLuaState::cRef FnRef;
	L.GetStackValues(2, FnRef);
	if (!FnRef.IsValid())
	{
		LOGWARN("Error in function call 'ForEachConsoleCommand': Could not get function reference of parameter #1");
		return 0;
	}

	// Callback for enumerating all commands:
	class cLuaCallback : public cPluginManager::cCommandEnumCallback
	{
	public:
		cLuaCallback(cLuaState & a_LuaState, cLuaState::cRef & a_FnRef):
			m_LuaState(a_LuaState),
			m_FnRef(a_FnRef)
		{
		}

	private:
		virtual bool Command(const AString & a_Command, const cPlugin * a_Plugin, const AString & a_Permission, const AString & a_HelpString) override
		{
			UNUSED(a_Plugin);
			UNUSED(a_Permission);
			bool ret = false;
			m_LuaState.Call(m_FnRef, a_Command, a_HelpString, cLuaState::Return, ret);
			return ret;
		}
		cLuaState & m_LuaState;
		cLuaState::cRef & m_FnRef;
	} Callback(L, FnRef);

	// Execute and push the returned value:
	L.Push(cPluginManager::Get()->ForEachConsoleCommand(Callback));
	return 1;
}





static int tolua_cPluginManager_BindCommand(lua_State * a_LuaState)
{
	/* Function signatures:
		cPluginManager:Get():BindCommand(Command, Permission, Function, HelpString)  -- regular
		cPluginManager:BindCommand(Command, Permission, Function, HelpString)        -- static
		cPluginManager.BindCommand(Command, Permission, Function, HelpString)        -- without the "self" param
	*/
	cLuaState L(a_LuaState);
	cPluginLua * Plugin = cManualBindings::GetLuaPlugin(L);
	if (Plugin == nullptr)
	{
		return 0;
	}

	// Read the arguments to this API call:
	tolua_Error tolua_err;
	int idx = 1;
	if (
		tolua_isusertype (L, 1, "cPluginManager", 0, &tolua_err) ||
		tolua_isusertable(L, 1, "cPluginManager", 0, &tolua_err)
	)
	{
		idx++;
	}
	if (
		!tolua_iscppstring(L, idx,     0, &tolua_err) ||
		!tolua_iscppstring(L, idx + 1, 0, &tolua_err) ||
		!tolua_iscppstring(L, idx + 3, 0, &tolua_err) ||
		!tolua_isnoobj    (L, idx + 4, &tolua_err)
	)
	{
		tolua_error(L, "#ferror in function 'BindCommand'.", &tolua_err);
		return 0;
	}
	if (!lua_isfunction(L, idx + 2))
	{
		luaL_error(L, "\"BindCommand\" function expects a function as its 3rd parameter. Command-binding aborted.");
		return 0;
	}
	cPluginManager * self = cPluginManager::Get();
	AString Command, Permission, HelpString;
	cLuaState::cCallbackPtr Handler;
	L.GetStackValues(idx, Command, Permission, Handler, HelpString);
	if (!Handler->IsValid())
	{
		LOGERROR("\"BindCommand\": Cannot create a function reference. Command \"%s\" not bound.", Command.c_str());
		return 0;
	}

	auto CommandHandler = std::make_shared<LuaCommandHandler>(std::move(Handler));
	if (!self->BindCommand(Command, Plugin, CommandHandler, Permission, HelpString))
	{
		// Refused. Possibly already bound. Error message has been given, display the callstack:
		L.LogStackTrace();
		return 0;
	}

	L.Push(true);
	return 1;
}





static int tolua_cPluginManager_BindConsoleCommand(lua_State * a_LuaState)
{
	/* Function signatures:
		cPluginManager:Get():BindConsoleCommand(Command, Function, HelpString)  -- regular
		cPluginManager:BindConsoleCommand(Command, Function, HelpString)        -- static
		cPluginManager.BindConsoleCommand(Command, Function, HelpString)        -- without the "self" param
	*/

	// Get the plugin identification out of LuaState:
	cLuaState L(a_LuaState);
	cPluginLua * Plugin = cManualBindings::GetLuaPlugin(L);
	if (Plugin == nullptr)
	{
		return 0;
	}

	// Read the arguments to this API call:
	tolua_Error tolua_err;
	int idx = 1;
	if (
		tolua_isusertype(L, 1, "cPluginManager", 0, &tolua_err) ||
		tolua_isusertable(L, 1, "cPluginManager", 0, &tolua_err)
	)
	{
		idx++;
	}
	if (
		!tolua_iscppstring(L, idx,     0, &tolua_err) ||  // Command
		!tolua_iscppstring(L, idx + 2, 0, &tolua_err) ||  // HelpString
		!tolua_isnoobj    (L, idx + 3, &tolua_err)
	)
	{
		tolua_error(L, "#ferror in function 'BindConsoleCommand'.", &tolua_err);
		return 0;
	}
	if (!lua_isfunction(L, idx + 1))
	{
		luaL_error(L, "\"BindConsoleCommand\" function expects a function as its 2nd parameter. Command-binding aborted.");
		return 0;
	}
	cPluginManager * self = cPluginManager::Get();
	AString Command, HelpString;
	cLuaState::cCallbackPtr Handler;
	L.GetStackValues(idx, Command, Handler, HelpString);
	if (!Handler->IsValid())
	{
		LOGERROR("\"BindConsoleCommand\": Cannot create a function reference. Console command \"%s\" not bound.", Command.c_str());
		return 0;
	}

	auto CommandHandler = std::make_shared<LuaCommandHandler>(std::move(Handler));
	if (!self->BindConsoleCommand(Command, Plugin, CommandHandler, HelpString))
	{
		// Refused. Possibly already bound. Error message has been given, display the callstack:
		L.LogStackTrace();
		return 0;
	}
	L.Push(true);
	return 1;
}





static int tolua_cPluginManager_CallPlugin(lua_State * tolua_S)
{
	/*
	Function signature:
	cPluginManager:CallPlugin("PluginName", "FunctionName", args...)
	*/

	// Check the parameters:
	cLuaState L(tolua_S);
	if (
		!L.CheckParamUserTable(1, "cPluginManager") ||
		!L.CheckParamString(2, 3))
	{
		return 0;
	}

	// Retrieve the plugin name and function name
	AString PluginName, FunctionName;
	L.ToString(2, PluginName);
	L.ToString(3, FunctionName);
	if (PluginName.empty() || FunctionName.empty())
	{
		LOGWARNING("cPluginManager:CallPlugin(): Invalid plugin name or function name");
		L.LogStackTrace();
		return 0;
	}

	// If requesting calling the current plugin, refuse:
	cPluginLua * ThisPlugin = cManualBindings::GetLuaPlugin(L);
	if (ThisPlugin == nullptr)
	{
		return 0;
	}
	if (ThisPlugin->GetName() == PluginName)
	{
		LOGWARNING("cPluginManager::CallPlugin(): Calling self is not implemented (why would it?)");
		L.LogStackTrace();
		return 0;
	}

	// Call the destination plugin using a plugin callback:
	int NumReturns = 0;
	auto PluginCallback = [&](cPlugin & a_Plugin)
		{
			if (!a_Plugin.IsLoaded())
			{
				return false;
			}
			NumReturns = static_cast<cPluginLua &>(a_Plugin).CallFunctionFromForeignState(
				FunctionName, L, 4, lua_gettop(L)
			);
			return true;
		};

	if (!cPluginManager::Get()->DoWithPlugin(PluginName, PluginCallback))
	{
		return 0;
	}
	if (NumReturns < 0)
	{
		// The call has failed, there are zero return values. Do NOT return negative number (Lua considers that a "yield")
		return 0;
	}
	return NumReturns;
}





static int tolua_cPluginManager_ExecuteConsoleCommand(lua_State * tolua_S)
{
	/*
	Function signature:
	cPluginManager:ExecuteConsoleCommand(EntireCommandStr) -> OutputString
	*/

	// Check params:
	cLuaState L(tolua_S);
	if (
		!L.CheckParamUserTable(1, "cPluginManager") ||
		!L.CheckParamString(2) ||
		!L.CheckParamEnd(3)
	)
	{
		return 0;
	}

	// Get the params:
	AString Command;
	L.GetStackValues(2, Command);
	auto Split = StringSplit(Command, " ");

	// Store the command output in a string:
	cStringAccumCommandOutputCallback CommandOutput;
	L.Push(cPluginManager::Get()->ExecuteConsoleCommand(Split, CommandOutput, Command));
	L.Push(CommandOutput.GetAccum());
	return 2;
}





static int tolua_cPluginManager_FindPlugins(lua_State * tolua_S)
{
	// API function no longer exists:
	LOGWARNING("cPluginManager:FindPlugins() is obsolete, use cPluginManager:RefreshPluginList() instead!");
	cLuaState::LogStackTrace(tolua_S);

	// Still, do the actual work performed by the API function when it existed:
	cPluginManager::Get()->RefreshPluginList();
	return 0;
}





static int tolua_cPlayer_GetPermissions(lua_State * tolua_S)
{
	// Function signature: cPlayer:GetPermissions() -> {permissions-array}

	// Check the params:
	cLuaState L(tolua_S);
	if (
		!L.CheckParamUserType(1, "cPlayer") ||
		!L.CheckParamEnd     (2)
	)
	{
		return 0;
	}

	// Get the params:
	cPlayer * self = static_cast<cPlayer *>(tolua_tousertype(tolua_S, 1, nullptr));
	if (self == nullptr)
	{
		LOGWARNING("%s: invalid self (%p)", __FUNCTION__, static_cast<void *>(self));
		return 0;
	}

	// Push the permissions:
	L.Push(self->GetPermissions());
	return 1;
}





static int tolua_cPlayer_GetRestrictions(lua_State * tolua_S)
{
	// Function signature: cPlayer:GetRestrictions() -> {restrictions-array}

	// Check the params:
	cLuaState L(tolua_S);
	if (
		!L.CheckParamUserType(1, "cPlayer") ||
		!L.CheckParamEnd     (2)
	)
	{
		return 0;
	}

	// Get the params:
	cPlayer * self = static_cast<cPlayer *>(tolua_tousertype(tolua_S, 1, nullptr));
	if (self == nullptr)
	{
		LOGWARNING("%s: invalid self (%p)", __FUNCTION__, static_cast<void *>(self));
		return 0;
	}

	// Push the permissions:
	L.Push(self->GetRestrictions());
	return 1;
}





static int tolua_cPlayer_PermissionMatches(lua_State * tolua_S)
{
	// Function signature: cPlayer:PermissionMatches(PermissionStr, TemplateStr) -> bool

	// Check the params:
	cLuaState L(tolua_S);
	if (
		!L.CheckParamUserTable(1, "cPlayer") ||
		!L.CheckParamString   (2, 3) ||
		!L.CheckParamEnd      (4)
	)
	{
		return 0;
	}

	// Get the params:
	AString Permission, Template;
	L.GetStackValues(2, Permission, Template);

	// Push the result of the match:
	L.Push(cPlayer::PermissionMatches(StringSplit(Permission, "."), StringSplit(Template, ".")));
	return 1;
}





static int tolua_cPlayer_GetUUID(lua_State * tolua_S)
{
	// Check the params:
	cLuaState L(tolua_S);
	if (!L.CheckParamSelf("cPlayer"))
	{
		return 0;
	}

	// Get the params:
	cPlayer * Self = nullptr;
	L.GetStackValue(1, Self);

	// Return the UUID as a string
	L.Push(Self->GetUUID().ToShortString());
	return 1;
}





template <
	class OBJTYPE,
	void (OBJTYPE::*SetCallback)(cLuaState::cCallbackPtr && a_CallbackFn)
>
static int tolua_SetObjectCallback(lua_State * tolua_S)
{
	// Function signature: OBJTYPE:SetWhateverCallback(CallbackFunction)

	// Get the parameters - self and the function reference:
	cLuaState L(tolua_S);
	OBJTYPE * self;
	cLuaState::cCallbackPtr callback;
	if (!L.GetStackValues(1, self, callback))
	{
		LOGWARNING("%s: Cannot get parameters", __FUNCTION__);
		L.LogStackTrace();
		return 0;
	}

	// Set the callback
	(self->*SetCallback)(std::move(callback));
	return 0;
}





// Callback class used for the WebTab:
class cWebTabCallback:
	public cWebAdmin::cWebTabCallback
{
public:
	/** The Lua callback to call to generate the page contents. */
	cLuaState::cCallback m_Callback;

	virtual bool Call(
		const HTTPRequest & a_Request,
		const AString & a_UrlPath,
		AString & a_Content,
		AString & a_ContentType
	) override
	{
		AString content, contentType;
		return m_Callback.Call(const_cast<HTTPRequest *>(&a_Request), a_UrlPath, cLuaState::Return, a_Content, a_ContentType);
	}
};





static int tolua_cPluginLua_AddWebTab(lua_State * tolua_S)
{
	// OBSOLETE, use cWebAdmin:AddWebTab() instead!
	// Function signature:
	// cPluginLua:AddWebTab(Title, CallbackFn, [UrlPath])

	// TODO: Warn about obsolete API usage
	// Only implement after merging the new API change and letting some time for changes in the plugins

	// Check params:
	cLuaState LuaState(tolua_S);
	cPluginLua * self = cManualBindings::GetLuaPlugin(tolua_S);
	if (self == nullptr)
	{
		return 0;
	}
	if (
		!LuaState.CheckParamString(2) ||
		!LuaState.CheckParamFunction(3) ||
		// Optional string as param 4
		!LuaState.CheckParamEnd(5)
	)
	{
		return 0;
	}

	// Read the params:
	AString title, urlPath;
	auto callback = std::make_shared<cWebTabCallback>();
	if (!LuaState.GetStackValues(2, title, callback->m_Callback))
	{
		LOGWARNING("cPlugin:AddWebTab(): Cannot read required parameters");
		return 0;
	}
	if (!LuaState.GetStackValue(4, urlPath))
	{
		urlPath = cWebAdmin::GetURLEncodedString(title);
	}

	cRoot::Get()->GetWebAdmin()->AddWebTab(title, urlPath, self->GetName(), callback);

	return 0;
}





static int tolua_cPlugin_GetDirectory(lua_State * tolua_S)
{
	cLuaState L(tolua_S);

	// Log the obsoletion warning:
	LOGWARNING("cPlugin:GetDirectory() is obsolete, use cPlugin:GetFolderName() instead.");
	L.LogStackTrace();

	// Retrieve the params:
	cPlugin * Plugin = static_cast<cPluginLua *>(tolua_tousertype(tolua_S, 1, nullptr));

	// Get the folder name:
	L.Push(Plugin->GetFolderName());
	return 1;
}





static int tolua_cPlugin_GetLocalDirectory(lua_State * tolua_S)
{
	cLuaState L(tolua_S);

	// Log the obsoletion warning:
	LOGWARNING("cPlugin:GetLocalDirectory() is obsolete, use cPlugin:GetLocalFolder() instead.");
	L.LogStackTrace();

	// Retrieve the params:
	cPlugin * Plugin = static_cast<cPluginLua *>(tolua_tousertype(tolua_S, 1, nullptr));

	// Get the folder:
	L.Push(Plugin->GetLocalFolder());
	return 1;
}





static int tolua_md5(lua_State * tolua_S)
{
	// Calculate the raw md5 checksum byte array:
	unsigned char Output[16];
	size_t len = 0;
	const unsigned char * SourceString = reinterpret_cast<const unsigned char *>(lua_tolstring(tolua_S, 1, &len));
	if (SourceString == nullptr)
	{
		return 0;
	}
	mbedtls_md5(SourceString, len, Output);
	lua_pushlstring(tolua_S, reinterpret_cast<const char *>(Output), ARRAYCOUNT(Output));
	return 1;
}





/** Does the same as tolua_md5, but reports that the usage is obsolete and the plugin should use cCrypto.md5(). */
static int tolua_md5_obsolete(lua_State * tolua_S)
{
	LOGWARNING("Using md5() is obsolete, please change your plugin to use cCryptoHash.md5()");
	cLuaState::LogStackTrace(tolua_S);
	return tolua_md5(tolua_S);
}





static int tolua_md5HexString(lua_State * tolua_S)
{
	// Calculate the raw md5 checksum byte array:
	unsigned char md5Output[16];
	size_t len = 0;
	const unsigned char * SourceString = reinterpret_cast<const unsigned char *>(lua_tolstring(tolua_S, 1, &len));
	if (SourceString == nullptr)
	{
		return 0;
	}
	mbedtls_md5(SourceString, len, md5Output);

	// Convert the md5 checksum to hex string:
	std::stringstream Output;
	Output << std::hex << std::setfill('0');
	for (size_t i = 0; i < ARRAYCOUNT(md5Output); i++)
	{
		Output << std::setw(2) << static_cast<unsigned short>(md5Output[i]);  // Need to cast to a number, otherwise a char is output
	}
	lua_pushlstring(tolua_S, Output.str().c_str(), Output.str().size());
	return 1;
}





static int tolua_sha1(lua_State * tolua_S)
{
	// Calculate the raw SHA1 checksum byte array from the input string:
	unsigned char Output[20];
	size_t len = 0;
	const unsigned char * SourceString = reinterpret_cast<const unsigned char *>(lua_tolstring(tolua_S, 1, &len));
	if (SourceString == nullptr)
	{
		return 0;
	}
	mbedtls_sha1(SourceString, len, Output);
	lua_pushlstring(tolua_S, reinterpret_cast<const char *>(Output), ARRAYCOUNT(Output));
	return 1;
}





static int tolua_sha1HexString(lua_State * tolua_S)
{
	// Calculate the raw SHA1 checksum byte array from the input string:
	unsigned char sha1Output[20];
	size_t len = 0;
	const unsigned char * SourceString = reinterpret_cast<const unsigned char *>(lua_tolstring(tolua_S, 1, &len));
	if (SourceString == nullptr)
	{
		return 0;
	}
	mbedtls_sha1(SourceString, len, sha1Output);

	// Convert the sha1 checksum to hex string:
	std::stringstream Output;
	Output << std::hex << std::setfill('0');
	for (size_t i = 0; i < ARRAYCOUNT(sha1Output); i++)
	{
		Output << std::setw(2) << static_cast<unsigned short>(sha1Output[i]);  // Need to cast to a number, otherwise a char is output
	}
	lua_pushlstring(tolua_S, Output.str().c_str(), Output.str().size());
	return 1;
}





static int tolua_get_HTTPRequest_Params(lua_State * a_LuaState)
{
	cLuaState L(a_LuaState);
	HTTPRequest * self;
	if (!L.GetStackValues(1, self))
	{
		tolua_Error err;
		tolua_error(a_LuaState, "Invalid self parameter, expected a HTTPRequest instance", &err);
		return 0;
	}
	L.Push(self->Params);
	return 1;
}





static int tolua_get_HTTPRequest_PostParams(lua_State * a_LuaState)
{
	cLuaState L(a_LuaState);
	HTTPRequest * self;
	if (!L.GetStackValues(1, self))
	{
		tolua_Error err;
		tolua_error(a_LuaState, "Invalid self parameter, expected a HTTPRequest instance", &err);
		return 0;
	}
	L.Push(self->PostParams);
	return 1;
}





static int tolua_get_HTTPRequest_FormData(lua_State* a_LuaState)
{
	cLuaState L(a_LuaState);
	HTTPRequest * self;
	if (!L.GetStackValues(1, self))
	{
		tolua_Error err;
		tolua_error(a_LuaState, "Invalid self parameter, expected a HTTPRequest instance", &err);
		return 0;
	}

	auto & FormData = self->FormData;
	lua_newtable(a_LuaState);
	int top = lua_gettop(a_LuaState);
	for (auto & Data : FormData)
	{
		lua_pushstring(a_LuaState, Data.first.c_str());
		tolua_pushusertype(a_LuaState, &Data.second, "HTTPFormData");
		// lua_pushlstring(a_LuaState, Data.second.Value.c_str(), Data.second.Value.size());  // Might contain binary data
		lua_settable(a_LuaState, top);
	}

	return 1;
}





static int tolua_cUrlParser_GetDefaultPort(lua_State * a_LuaState)
{
	// API function signature:
	// cUrlParser:GetDefaultPort("scheme") -> number

	// Check params:
	cLuaState L(a_LuaState);
	if (
		!L.CheckParamUserTable(1, "cUrlParser") ||
		!L.CheckParamString(2) ||
		!L.CheckParamEnd(3)
	)
	{
		return 0;
	}

	// Read params from Lua:
	AString scheme;
	L.GetStackValue(2, scheme);

	// Execute and push result:
	L.Push(cUrlParser::GetDefaultPort(scheme));
	return 1;
}





static int tolua_cUrlParser_IsKnownScheme(lua_State * a_LuaState)
{
	// API function signature:
	// cUrlParser:IsKnownScheme("scheme") -> bool

	// Check params:
	cLuaState L(a_LuaState);
	if (
		!L.CheckParamUserTable(1, "cUrlParser") ||
		!L.CheckParamString(2) ||
		!L.CheckParamEnd(3)
	)
	{
		return 0;
	}

	// Read params from Lua:
	AString scheme;
	L.GetStackValue(2, scheme);

	// Execute and push result:
	L.Push(cUrlParser::IsKnownScheme(scheme));
	return 1;
}





static int tolua_cUrlParser_Parse(lua_State * a_LuaState)
{
	// API function signature:
	// cUrlParser:Parse("url") -> "scheme", "user", "password", "host", portnum, "path", "query", "fragment"
	// On error, returns nil and error message

	// Check params:
	cLuaState L(a_LuaState);
	if (
		!L.CheckParamUserTable(1, "cUrlParser") ||
		!L.CheckParamString(2) ||
		!L.CheckParamEnd(3)
	)
	{
		return 0;
	}

	// Read params from Lua:
	AString url;
	L.GetStackValue(2, url);

	// Execute and push result:
	AString scheme, username, password, host, path, query, fragment;
	UInt16 port;
	auto res = cUrlParser::Parse(url, scheme, username, password, host, port, path, query, fragment);
	if (!res.first)
	{
		// Error, return nil and error msg:
		L.Push(cLuaState::Nil, res.second);
		return 2;
	}
	L.Push(scheme, username, password, host, port, path, query, fragment);
	return 8;
}





static int tolua_cUrlParser_ParseAuthorityPart(lua_State * a_LuaState)
{
	// API function signature:
	// cUrlParser:ParseAuthorityPart("authority") -> "user", "password", "host", portnum
	// On error, returns nil and error message
	// Parts not specified in the "authority" are left empty / zero

	// Check params:
	cLuaState L(a_LuaState);
	if (
		!L.CheckParamUserTable(1, "cUrlParser") ||
		!L.CheckParamString(2) ||
		!L.CheckParamEnd(3)
	)
	{
		return 0;
	}

	// Read params from Lua:
	AString authPart;
	L.GetStackValue(2, authPart);

	// Execute and push result:
	AString username, password, host;
	UInt16 port;
	auto res = cUrlParser::ParseAuthorityPart(authPart, username, password, host, port);
	if (!res.first)
	{
		// Error, return nil and error msg:
		L.Push(cLuaState::Nil, res.second);
		return 2;
	}
	L.Push(username, password, host, port);
	return 4;
}





static int tolua_cUrlParser_UrlDecode(lua_State * tolua_S)
{
	// Check the param types:
	cLuaState S(tolua_S);
	if (
		// Don't care about the first param
		!S.CheckParamString(2) ||
		!S.CheckParamEnd(3)
	)
	{
		return 0;
	}

	// Get the parameters:
	AString Input;
	S.GetStackValue(2, Input);

	// Convert and return:
	auto res = URLDecode(Input);
	if (res.first)
	{
		S.Push(res.second);
	}
	else
	{
		S.Push(cLuaState::Nil);
	}
	return 1;
}





static int tolua_cUrlParser_UrlEncode(lua_State * tolua_S)
{
	// Check the param types:
	cLuaState S(tolua_S);
	if (
		// Don't care about the first param
		!S.CheckParamString(2) ||
		!S.CheckParamEnd(3)
	)
	{
		return 0;
	}

	// Get the parameters:
	AString Input;
	S.GetStackValue(2, Input);

	// Convert and return:
	S.Push(URLEncode(Input));
	return 1;
}





static int tolua_cWebAdmin_AddWebTab(lua_State * tolua_S)
{
	// Function signatures:
	// cWebAdmin:AddWebTab(Title, UrlPath, CallbackFn)

	// Check params:
	cLuaState LuaState(tolua_S);
	cPluginLua * self = cManualBindings::GetLuaPlugin(tolua_S);
	if (self == nullptr)
	{
		return 0;
	}
	if (
		// Don't care whether the first param is a cWebAdmin instance or class
		!LuaState.CheckParamString(2, 3) ||
		!LuaState.CheckParamFunction(4) ||
		!LuaState.CheckParamEnd(5)
	)
	{
		return 0;
	}

	// Read the params:
	AString title, urlPath;
	auto callback = std::make_shared<cWebTabCallback>();
	if (!LuaState.GetStackValues(2, title, urlPath, callback->m_Callback))
	{
		LOGWARNING("cWebAdmin:AddWebTab(): Cannot read required parameters");
		return 0;
	}

	cRoot::Get()->GetWebAdmin()->AddWebTab(title, urlPath, self->GetName(), callback);

	return 0;
}





static int tolua_cWebAdmin_GetAllWebTabs(lua_State * tolua_S)
{
	// Function signature:
	// cWebAdmin:GetAllWebTabs() -> { {"PluginName", "UrlPath", "Title"}, {"PluginName", "UrlPath", "Title"}, ...}

	// Don't care about params at all

	auto webTabs = cRoot::Get()->GetWebAdmin()->GetAllWebTabs();
	lua_createtable(tolua_S, static_cast<int>(webTabs.size()), 0);
	int newTable = lua_gettop(tolua_S);
	int index = 1;
	cLuaState L(tolua_S);
	for (const auto & wt: webTabs)
	{
		lua_createtable(tolua_S, 0, 3);
		L.Push(wt->m_PluginName);
		lua_setfield(tolua_S, -2, "PluginName");
		L.Push(wt->m_UrlPath);
		lua_setfield(tolua_S, -2, "UrlPath");
		L.Push(wt->m_Title);
		lua_setfield(tolua_S, -2, "Title");
		lua_rawseti(tolua_S, newTable, index);
		++index;
	}
	return 1;
}





/** Binding for cWebAdmin::GetPage. */
static int tolua_cWebAdmin_GetPage(lua_State * tolua_S)
{
	/*
	Function signature:
	cWebAdmin:GetPage(a_HTTPRequest) ->
	{
		Content = "",       // Content generated by the plugin
		ContentType = "",   // Content type generated by the plugin (default: "text/html")
		UrlPath = "",       // URL path of the tab
		TabTitle = "",      // Tab's title, as register via cWebAdmin:AddWebTab()
		PluginName = "",    // Plugin's API name
		PluginFolder = "",  // Plugin's folder name (display name)
	}
	*/

	// Check the param types:
	cLuaState S(tolua_S);
	if (
		// Don't care about first param, whether it's cWebAdmin instance or class
		!S.CheckParamUserType(2, "HTTPRequest") ||
		!S.CheckParamEnd(3)
	)
	{
		return 0;
	}

	// Get the parameters:
	HTTPRequest * request = nullptr;
	if (!S.GetStackValue(2, request))
	{
		LOGWARNING("cWebAdmin:GetPage(): Cannot read the HTTPRequest parameter.");
		return 0;
	}

	// Generate the page and push the results as a dictionary-table:
	auto page = cRoot::Get()->GetWebAdmin()->GetPage(*request);
	lua_createtable(S, 0, 6);
	S.Push(page.Content);
	lua_setfield(S, -2, "Content");
	S.Push(page.ContentType);
	lua_setfield(S, -2, "ContentType");
	S.Push(page.TabUrlPath);
	lua_setfield(S, -2, "UrlPath");
	S.Push(page.TabTitle);
	lua_setfield(S, -2, "TabTitle");
	S.Push(page.PluginName);
	lua_setfield(S, -2, "PluginName");
	S.Push(cPluginManager::Get()->GetPluginFolderName(page.PluginName));
	lua_setfield(S, -2, "PluginFolder");
	return 1;
}





/** Binding for cWebAdmin::GetURLEncodedString. */
static int tolua_cWebAdmin_GetURLEncodedString(lua_State * tolua_S)
{
	// Emit the obsoletion warning:
	cLuaState S(tolua_S);
	LOGWARNING("cWebAdmin:GetURLEncodedString() is obsolete, use cUrlParser:UrlEncode() instead.");
	S.LogStackTrace();

	return tolua_cUrlParser_UrlEncode(tolua_S);
}





static int tolua_cClientHandle_SendPluginMessage(lua_State * L)
{
	cLuaState S(L);
	if (
		!S.CheckParamSelf("cClientHandle") ||
		!S.CheckParamString(2, 3) ||
		!S.CheckParamEnd(4)
	)
	{
		return 0;
	}
	cClientHandle * Client = static_cast<cClientHandle *>(tolua_tousertype(L, 1, nullptr));
	if (Client == nullptr)
	{
		LOGWARNING("ClientHandle is nil in cClientHandle:SendPluginMessage()");
		S.LogStackTrace();
		return 0;
	}
	AString Channel, Message;
	Channel.assign(lua_tostring(L, 2), lua_strlen(L, 2));
	Message.assign(lua_tostring(L, 3), lua_strlen(L, 3));
	Client->SendPluginMessage(Channel, Message);
	return 0;
}





static int tolua_cClientHandle_GetForgeMods(lua_State * L)
{
	cLuaState S(L);
	if (
		!S.CheckParamSelf("cClientHandle") ||
		!S.CheckParamEnd(2)
	)
	{
		return 0;
	}
	cClientHandle * Client;
	S.GetStackValue(1, Client);

	S.Push(Client->GetForgeMods());
	return 1;
}





static int tolua_cClientHandle_GetUUID(lua_State * tolua_S)
{
	// Check the params:
	cLuaState L(tolua_S);
	if (
		!L.CheckParamSelf("cClientHandle") ||
		!L.CheckParamEnd(2)
	)
	{
		return 0;
	}

	// Get the params:
	cClientHandle * Self;
	L.GetStackValue(1, Self);

	// Return the UUID as a string:
	L.Push(Self->GetUUID().ToShortString());
	return 1;
}





static int tolua_cClientHandle_GenerateOfflineUUID(lua_State * tolua_S)
{
	// Check the params:
	cLuaState L(tolua_S);
	if (
		!L.CheckParamStaticSelf("cClientHandle") ||
		!L.CheckParamString(2) ||
		!L.CheckParamEnd(3)
	)
	{
		return 0;
	}

	// Get the params:
	AString Username;
	L.GetStackValue(2, Username);

	// Return the UUID as a string:
	L.Push(cClientHandle::GenerateOfflineUUID(Username).ToShortString());
	return 1;
}





static int tolua_cClientHandle_IsUUIDOnline(lua_State * tolua_S)
{
	// Check the params:
	cLuaState L(tolua_S);
	if (
		!L.CheckParamStaticSelf("cClientHandle") ||
		!L.CheckParamUUID(2) ||
		!L.CheckParamEnd(3)
	)
	{
		return 0;
	}

	// Get the params:
	cUUID UUID;
	L.GetStackValue(2, UUID);

	// Return the result:
	L.Push(cClientHandle::IsUUIDOnline(UUID));
	return 1;
}





static int tolua_cMobHeadEntity_SetOwner(lua_State * tolua_S)
{
	// Check params:
	cLuaState L(tolua_S);
	if (
		!L.CheckParamSelf("cMobHeadEntity") ||
		!L.CheckParamUUID(2) ||
		!L.CheckParamString(3, 5) ||
		!L.CheckParamEnd(6)
	)
	{
		return 0;
	}


	// Get the params:
	cMobHeadEntity * Self;
	cUUID OwnerUUID;
	AString OwnerName, OwnerTexture, OwnerTextureSignature;
	L.GetStackValues(1, Self, OwnerUUID, OwnerName, OwnerTexture, OwnerTextureSignature);

	// Set the owner:
	Self->SetOwner(OwnerUUID, OwnerName, OwnerTexture, OwnerTextureSignature);
	return 0;
}





static int tolua_cMobHeadEntity_GetOwnerUUID(lua_State * tolua_S)
{
	// Check params:
	cLuaState L(tolua_S);
	if (
		!L.CheckParamSelf("cMobHeadEntity") ||
		!L.CheckParamEnd(2)
	)
	{
		return 0;
	}

	// Get the params:
	cMobHeadEntity * Self;
	L.GetStackValue(1, Self);

	// Return the UUID as a string:
	cUUID Owner = Self->GetOwnerUUID();
	L.Push(Owner.IsNil() ? AString{} : Owner.ToShortString());
	return 1;
}





static int tolua_cMojangAPI_AddPlayerNameToUUIDMapping(lua_State * L)
{
	cLuaState S(L);
	if (
		!S.CheckParamStaticSelf("cMojangAPI") ||
		!S.CheckParamString(2) ||
		!S.CheckParamUUID(3) ||
		!S.CheckParamEnd(4)
	)
	{
		return 0;
	}

	// Retrieve the parameters:
	AString PlayerName;
	cUUID UUID;
	S.GetStackValues(2, PlayerName, UUID);

	// Store in the cache:
	cRoot::Get()->GetMojangAPI().AddPlayerNameToUUIDMapping(PlayerName, UUID);
	return 0;
}





static int tolua_cMojangAPI_GetPlayerNameFromUUID(lua_State * L)
{
	cLuaState S(L);
	if (
		!S.CheckParamStaticSelf("cMojangAPI") ||
		!S.CheckParamUUID(2) ||
		!S.CheckParamEnd(4)
	)
	{
		return 0;
	}

	cUUID UUID;
	S.GetStackValue(2, UUID);

	// If the UseOnlyCached param was given, read it; default to false
	bool ShouldUseCacheOnly = false;
	if (lua_gettop(L) == 3)
	{
		ShouldUseCacheOnly = (lua_toboolean(L, 3) != 0);
		lua_pop(L, 1);
	}

	// Return the PlayerName:
	AString PlayerName = cRoot::Get()->GetMojangAPI().GetPlayerNameFromUUID(UUID, ShouldUseCacheOnly);
	S.Push(PlayerName);
	return 1;
}





static int tolua_cMojangAPI_GetUUIDFromPlayerName(lua_State * L)
{
	cLuaState S(L);
	if (
		!S.CheckParamStaticSelf("cMojangAPI") ||
		!S.CheckParamString(2) ||
		!S.CheckParamEnd(4)
	)
	{
		return 0;
	}

	AString PlayerName;
	S.GetStackValue(2, PlayerName);

	// If the UseOnlyCached param was given, read it; default to false
	bool ShouldUseCacheOnly = false;
	if (lua_gettop(L) == 3)
	{
		ShouldUseCacheOnly = (lua_toboolean(L, 3) != 0);
		lua_pop(L, 1);
	}

	// Return the UUID as a string:
	cUUID UUID = cRoot::Get()->GetMojangAPI().GetUUIDFromPlayerName(PlayerName, ShouldUseCacheOnly);
	S.Push(UUID.IsNil() ? AString{} : UUID.ToShortString());
	return 1;
}





static int tolua_cMojangAPI_GetUUIDsFromPlayerNames(lua_State * L)
{
	cLuaState S(L);
	if (
		!S.CheckParamStaticSelf("cMojangAPI") ||
		!S.CheckParamTable(2) ||
		!S.CheckParamEnd(4)
	)
	{
		return 0;
	}

	// Convert the input table into AStringVector:
	AStringVector PlayerNames;
	int NumNames = luaL_getn(L, 2);
	PlayerNames.reserve(static_cast<size_t>(NumNames));
	for (int i = 1; i <= NumNames; i++)
	{
		lua_rawgeti(L, 2, i);
		AString Name;
		S.GetStackValue(-1, Name);
		if (!Name.empty())
		{
			PlayerNames.push_back(Name);
		}
		lua_pop(L, 1);
	}

	// If the UseOnlyCached param was given, read it; default to false
	bool ShouldUseCacheOnly = false;
	if (lua_gettop(L) == 3)
	{
		ShouldUseCacheOnly = (lua_toboolean(L, 3) != 0);
		lua_pop(L, 1);
	}

	// Push the output table onto the stack:
	lua_newtable(L);

	// Get the UUIDs:
	auto UUIDs = cRoot::Get()->GetMojangAPI().GetUUIDsFromPlayerNames(PlayerNames, ShouldUseCacheOnly);
	if (UUIDs.size() != PlayerNames.size())
	{
		// A hard error has occured while processing the request, no UUIDs were returned. Return an empty table:
		return 1;
	}

	// Convert to output table, PlayerName -> UUID string:
	size_t len = UUIDs.size();
	for (size_t i = 0; i < len; i++)
	{
		if (UUIDs[i].IsNil())
		{
			// No UUID was provided for PlayerName[i], skip it in the resulting table
			continue;
		}
		S.Push(UUIDs[i].ToShortString());
		lua_setfield(L, 3, PlayerNames[i].c_str());
	}
	return 1;
}





static int tolua_cMojangAPI_MakeUUIDDashed(lua_State * L)
{
	// Function now non-existant but kept for API compatibility

	// Check params:
	cLuaState S(L);
	if (
		!S.CheckParamStaticSelf("cMojangAPI") ||
		!S.CheckParamUUID(2) ||
		!S.CheckParamEnd(3)
	)
	{
		return 0;
	}

	// Get the params:
	cUUID UUID;
	S.GetStackValue(2, UUID);

	// Push the result:
	S.Push(UUID.ToLongString());
	return 1;
}





static int tolua_cMojangAPI_MakeUUIDShort(lua_State * L)
{
	// Function now non-existant but kept for API compatibility

	// Check params:
	cLuaState S(L);
	if (
		!S.CheckParamUserTable(1, "cMojangAPI") ||
		!S.CheckParamUUID(2) ||
		!S.CheckParamEnd(3)
	)
	{
		return 0;
	}

	// Get the params:
	cUUID UUID;
	S.GetStackValue(2, UUID);

	// Push the result:
	S.Push(UUID.ToShortString());
	return 1;
}





static int tolua_get_cItem_m_LoreTable(lua_State * tolua_S)
{
	// Check params:
	cLuaState L(tolua_S);
	if (!L.CheckParamSelf("const cItem"))
	{
		return 0;
	}

	// Get the params:
	const cItem * Self = nullptr;
	L.GetStackValue(1, Self);

	// Push the result:
	L.Push(Self->m_LoreTable);
	return 1;
}





static int tolua_cItem_EnchantByXPLevels(lua_State * tolua_S)
{
	// Check params:
	cLuaState L(tolua_S);
	if (
		!L.CheckParamSelf("cItem") ||
		!L.CheckParamNumber(2)
	)
	{
		return 0;
	}

	// Get the params:
	cItem * Self;
	int NumXPLevels;
	L.GetStackValue(1, Self);
	L.GetStackValue(2, NumXPLevels);

	// Call:
	L.Push(Self->EnchantByXPLevels(NumXPLevels, GetRandomProvider()));
	return 1;
}





static int tolua_set_cItem_m_LoreTable(lua_State * tolua_S)
{
	// Check params:
	cLuaState L(tolua_S);
	if (
		!L.CheckParamSelf("cItem") ||
		!L.CheckParamTable(2)
	)
	{
		return 0;
	}

	// Get the params:
	cItem * Self = nullptr;
	L.GetStackValue(1, Self);

	// Set the value:
	Self->m_LoreTable.clear();
	if (!L.GetStackValue(2, Self->m_LoreTable))
	{
		return L.ApiParamError("cItem.m_LoreTable: Could not read value as an array of strings");
	}
	return 0;
}





static int Lua_ItemGrid_GetSlotCoords(lua_State * L)
{
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(L, 1, "const cItemGrid", 0, &tolua_err) ||
		!tolua_isnumber  (L, 2, 0, &tolua_err) ||
		!tolua_isnoobj   (L, 3, &tolua_err)
	)
	{
		goto tolua_lerror;
	}

	{
		const cItemGrid * self = static_cast<const cItemGrid *>(tolua_tousertype(L, 1, nullptr));
		int SlotNum = static_cast<int>(tolua_tonumber(L, 2, 0));
		if (self == nullptr)
		{
			tolua_error(L, "invalid 'self' in function 'cItemGrid:GetSlotCoords'", nullptr);
			return 0;
		}
		int X, Y;
		self->GetSlotCoords(SlotNum, X, Y);
		tolua_pushnumber(L, static_cast<lua_Number>(X));
		tolua_pushnumber(L, static_cast<lua_Number>(Y));
		return 2;
	}

tolua_lerror:
	tolua_error(L, "#ferror in function 'cItemGrid:GetSlotCoords'.", &tolua_err);
	return 0;
}





/** Provides interface between a Lua table of callbacks and the cBlockTracer::cCallbacks */
class cLuaBlockTracerCallbacks :
	public cBlockTracer::cCallbacks
{
public:
	cLuaBlockTracerCallbacks(cLuaState::cTableRefPtr && a_Callbacks):
		m_Callbacks(std::move(a_Callbacks))
	{
	}

	virtual bool OnNextBlock(Vector3i a_BlockPos, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, eBlockFace a_EntryFace) override
	{
		bool res = false;
		if (m_Callbacks->CallTableFn(
			"OnNextBlock",
			a_BlockPos,
			a_BlockType,
			a_BlockMeta,
			a_EntryFace,
			cLuaState::Return, res)
		)
		{
			return res;
		}
		// No such function in the table, skip the callback
		return false;
	}

	virtual bool OnNextBlockNoData(Vector3i a_BlockPos, char a_EntryFace) override
	{
		bool res = false;
		if (m_Callbacks->CallTableFn(
			"OnNextBlockNoData",
			a_BlockPos,
			a_EntryFace,
			cLuaState::Return, res)
		)
		{
			return res;
		}
		// No such function in the table, skip the callback
		return false;
	}

	virtual bool OnOutOfWorld(Vector3d a_BlockPos) override
	{
		bool res = false;
		if (m_Callbacks->CallTableFn(
			"OnOutOfWorld",
			a_BlockPos,
			cLuaState::Return, res)
		)
		{
			return res;
		}
		// No such function in the table, skip the callback
		return false;
	}

	virtual bool OnIntoWorld(Vector3d a_BlockPos) override
	{
		bool res = false;
		if (m_Callbacks->CallTableFn("OnIntoWorld",
			a_BlockPos,
			cLuaState::Return, res)
		)
		{
			return res;
		}
		// No such function in the table, skip the callback
		return false;
	}

	virtual void OnNoMoreHits(void) override
	{
		m_Callbacks->CallTableFn("OnNoMoreHits");
	}

	virtual void OnNoChunk(void) override
	{
		m_Callbacks->CallTableFn("OnNoChunk");
	}

protected:
	cLuaState::cTableRefPtr m_Callbacks;
};





/** Provides interface between a Lua table of callbacks and the cBlockTracer::cCallbacks
This is the deprecated version of cLuaBlockTracerCallback, used when the plugin calls
the Trace function with number-based coords. */
class cLuaBlockTracerCallbacksOld :
	public cLuaBlockTracerCallbacks
{
public:
	cLuaBlockTracerCallbacksOld(cLuaState::cTableRefPtr && a_Callbacks):
		cLuaBlockTracerCallbacks(std::move(a_Callbacks))
	{
	}

	virtual bool OnNextBlock(Vector3i a_BlockPos, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, eBlockFace a_EntryFace) override
	{
		bool res = false;
		if (m_Callbacks->CallTableFn(
			"OnNextBlock",
			a_BlockPos.x, a_BlockPos.y, a_BlockPos.z,
			a_BlockType,
			a_BlockMeta,
			a_EntryFace,
			cLuaState::Return, res)
		)
		{
			return res;
		}
		// No such function in the table, skip the callback
		return false;
	}

	virtual bool OnNextBlockNoData(Vector3i a_BlockPos, char a_EntryFace) override
	{
		bool res = false;
		if (m_Callbacks->CallTableFn(
			"OnNextBlockNoData",
			a_BlockPos.x, a_BlockPos.y, a_BlockPos.z,
			a_EntryFace,
			cLuaState::Return, res)
		)
		{
			return res;
		}
		// No such function in the table, skip the callback
		return false;
	}

	virtual bool OnOutOfWorld(Vector3d a_BlockPos) override
	{
		bool res = false;
		if (m_Callbacks->CallTableFn(
			"OnOutOfWorld",
			a_BlockPos.x, a_BlockPos.y, a_BlockPos.z,
			cLuaState::Return, res)
		)
		{
			return res;
		}
		// No such function in the table, skip the callback
		return false;
	}

	virtual bool OnIntoWorld(Vector3d a_BlockPos) override
	{
		bool res = false;
		if (m_Callbacks->CallTableFn(
			"OnIntoWorld",
			a_BlockPos.x, a_BlockPos.y, a_BlockPos.z,
			cLuaState::Return, res)
		)
		{
			return res;
		}
		// No such function in the table, skip the callback
		return false;
	}
};





static int tolua_cLineBlockTracer_FirstSolidHitTrace(lua_State * tolua_S)
{
	/* Supported function signatures:
	cLineBlockTracer:FirstSolidHitTrace(World, StartX, StartY, StartZ, EndX, EndY, EndZ) -> bool, [Vector3d, Vector3i, eBlockFace]  // Canonical
	cLineBlockTracer:FirstSolidHitTrace(World, Start, End) -> bool, [Vector3d, Vector3i, eBlockFace]                                // Canonical
	cLineBlockTracer.FirstSolidHitTrace(World, StartX, StartY, StartZ, EndX, EndY, EndZ) -> bool, [Vector3d, Vector3i, eBlockFace]
	cLineBlockTracer.FirstSolidHitTrace(World, Start, End) -> bool, [Vector3d, Vector3i, eBlockFace]
	*/

	// If the first param is the cLineBlockTracer class, shift param index by one:
	int idx = 1;
	tolua_Error err;
	if (tolua_isusertable(tolua_S, 1, "cLineBlockTracer", 0, &err))
	{
		idx = 2;
	}

	// Check params:
	cLuaState L(tolua_S);
	if (
		!L.CheckParamUserType(idx, "cWorld")
	)
	{
		return 0;
	}

	if (L.IsParamNumber(idx + 1))
	{
		// This is the number variant of the call:
		if (
			!L.CheckParamNumber(idx + 1, idx + 6) ||
			!L.CheckParamEnd(idx + 7)
		)
		{
			return 0;
		}
		// Get the params:
		cWorld * world;
		double startX, startY, startZ;
		double endX, endY, endZ;
		if (!L.GetStackValues(idx, world, startX, startY, startZ, endX, endY, endZ))
		{
			LOGWARNING("cLineBlockTracer:FirstSolidHitTrace(): Cannot read parameters, aborting the trace.");
			L.LogStackTrace();
			L.LogStackValues("Values on the stack");
			return 0;
		}
		Vector3d hitCoords;
		Vector3i hitBlockCoords;
		eBlockFace hitBlockFace;
		auto isHit = cLineBlockTracer::FirstSolidHitTrace(*world, Vector3d(startX, startY, startZ), Vector3d(endX, endY, endZ), hitCoords, hitBlockCoords, hitBlockFace);
		L.Push(isHit);
		if (!isHit)
		{
			return 1;
		}
		L.Push(hitCoords);
		L.Push(hitBlockCoords);
		L.Push(hitBlockFace);
		return 4;
	}

	if (L.IsParamVector3(idx + 1))
	{
		// This is the Vector3-based variant of the call:
		if (
			!L.CheckParamVector3(idx + 1, idx + 2) ||
			!L.CheckParamEnd(idx + 3)
		)
		{
			return 0;
		}
		// Get the params:
		cWorld * world;
		Vector3d start;
		Vector3d end;
		if (!L.GetStackValues(idx, world, start, end))
		{
			LOGWARNING("cLineBlockTracer:FirstSolidHitTrace(): Cannot read parameters, aborting the trace.");
			L.LogStackTrace();
			L.LogStackValues("Values on the stack");
			return 0;
		}
		Vector3d hitCoords;
		Vector3i hitBlockCoords;
		eBlockFace hitBlockFace;
		auto isHit = cLineBlockTracer::FirstSolidHitTrace(*world, start, end, hitCoords, hitBlockCoords, hitBlockFace);
		L.Push(isHit);
		if (!isHit)
		{
			return 1;
		}
		L.Push(hitCoords);
		L.Push(hitBlockCoords);
		L.Push(hitBlockFace);
		return 4;
	}

	tolua_error(L, "cLineBlockTracer:FirstSolidHitTrace(): Invalid parameters, expected either a set of coords, or two Vector3's", nullptr);
	return 0;
}





static int tolua_cLineBlockTracer_LineOfSightTrace(lua_State * tolua_S)
{
	/* Supported function signatures:
	cLineBlockTracer:LineOfSightTrace(World, StartX, StartY, StartZ, EndX, EndY, EndZ, Sight) -> bool  // Canonical
	cLineBlockTracer:LineOfSightTrace(World, Start, End, Sight) -> bool                                // Canonical
	cLineBlockTracer.LineOfSightTrace(World, StartX, StartY, StartZ, EndX, EndY, EndZ, Sight) -> bool
	cLineBlockTracer.LineOfSightTrace(World, Start, End, Sight) -> bool
	*/

	// If the first param is the cLineBlockTracer class, shift param index by one:
	int idx = 1;
	tolua_Error err;
	if (tolua_isusertable(tolua_S, 1, "cLineBlockTracer", 0, &err))
	{
		idx = 2;
	}

	// Check params:
	cLuaState L(tolua_S);
	if (
		!L.CheckParamUserType(idx, "cWorld")
	)
	{
		return 0;
	}

	if (L.IsParamNumber(idx + 1))
	{
		// This is the number variant of the call:
		if (
			!L.CheckParamNumber(idx + 1, idx + 6) ||
			// Optional param lineOfSight is not checked
			!L.CheckParamEnd(idx + 8)
		)
		{
			return 0;
		}
		// Get the params:
		cWorld * world;
		double startX, startY, startZ;
		double endX, endY, endZ;
		if (!L.GetStackValues(idx, world, startX, startY, startZ, endX, endY, endZ))
		{
			LOGWARNING("cLineBlockTracer:LineOfSightTrace(): Cannot read parameters, aborting the trace.");
			L.LogStackTrace();
			L.LogStackValues("Values on the stack");
			return 0;
		}
		int lineOfSight = cLineBlockTracer::losAir | cLineBlockTracer::losWater;
		L.GetStackValue(idx + 7, lineOfSight);
		L.Push(cLineBlockTracer::LineOfSightTrace(*world, Vector3d(startX, startY, startZ), Vector3d(endX, endY, endZ), lineOfSight));
		return 1;
	}

	if (L.IsParamVector3(idx + 1))
	{
		// This is the Vector3-based variant of the call:
		if (
			!L.CheckParamVector3(idx + 1, idx + 2) ||
			// Optional param lineOfSight is not checked
			!L.CheckParamEnd(idx + 4)
		)
		{
			return 0;
		}
		// Get the params:
		cWorld * world;
		Vector3d start;
		Vector3d end;
		if (!L.GetStackValues(idx, world, start, end))
		{
			LOGWARNING("cLineBlockTracer:LineOfSightTrace(): Cannot read parameters, aborting the trace.");
			L.LogStackTrace();
			L.LogStackValues("Values on the stack");
			return 0;
		}
		int lineOfSight = cLineBlockTracer::losAirWater;
		L.GetStackValue(idx + 7, lineOfSight);
		L.Push(cLineBlockTracer::LineOfSightTrace(*world, start, end, lineOfSight));
		return 1;
	}

	tolua_error(L, "cLineBlockTracer:LineOfSightTrace(): Invalid parameters, expected either a set of coords, or two Vector3's", nullptr);
	return 0;
}





static int tolua_cLineBlockTracer_Trace(lua_State * tolua_S)
{
	/* Supported function signatures:
	cLineBlockTracer:Trace(World, Callbacks, StartX, StartY, StartZ, EndX, EndY, EndZ)  // Canonical  // DEPRECATED
	cLineBlockTracer.Trace(World, Callbacks, StartX, StartY, StartZ, EndX, EndY, EndZ)  // DEPRECATED
	cLineBlockTracer:Trace(World, Callbacks, Start, End)  // Canonical
	cLineBlockTracer.Trace(World, Callbacks, Start, End)
	*/

	// If the first param is the cLineBlockTracer class, shift param index by one:
	int idx = 1;
	tolua_Error err;
	if (tolua_isusertable(tolua_S, 1, "cLineBlockTracer", 0, &err))
	{
		idx = 2;
	}

	// Check params:
	cLuaState L(tolua_S);
	if (
		!L.CheckParamUserType(idx, "cWorld") ||
		!L.CheckParamTable   (idx + 1)
	)
	{
		return 0;
	}

	// Get the params:
	cWorld * world;
	Vector3d start;
	Vector3d end;
	cLuaState::cTableRefPtr callbacks;
	if (
		L.IsParamNumber  (idx + 2) &&
		L.IsParamNumber  (idx + 3) &&
		L.IsParamNumber  (idx + 4) &&
		L.IsParamNumber  (idx + 5) &&
		L.IsParamNumber  (idx + 6) &&
		L.IsParamNumber  (idx + 7) &&
		L.CheckParamEnd  (idx + 8)
	)
	{
		if (!L.GetStackValues(idx, world, callbacks, start.x, start.y, start.z, end.x, end.y, end.z))
		{
			LOGWARNING("cLineBlockTracer:Trace(): Cannot read parameters (starting at idx %d), aborting the trace.", idx);
			L.LogStackTrace();
			L.LogStackValues("Values on the stack");
			return 0;
		}
		LOGWARNING("cLineBlockTracer:Trace(): Using plain numbers is deprecated, use Vector3 coords instead.");
		L.LogStackTrace();
		// Trace:
		cLuaBlockTracerCallbacksOld tracerCallbacks(std::move(callbacks));
		bool res = cLineBlockTracer::Trace(*world, tracerCallbacks, start, end);
		tolua_pushboolean(L, res ? 1 : 0);
		return 1;
	}
	else if (
		L.IsParamVector3(idx + 2) &&
		L.IsParamVector3(idx + 3) &&
		L.CheckParamEnd (idx + 4)
	)
	{
		if (!L.GetStackValues(idx, world, callbacks, start, end))
		{
			LOGWARNING("cLineBlockTracer:Trace(): Cannot read parameters (starting at idx %d), aborting the trace.", idx);
			L.LogStackTrace();
			L.LogStackValues("Values on the stack");
			return 0;
		}
		// Trace:
		cLuaBlockTracerCallbacks tracerCallbacks(std::move(callbacks));
		bool res = cLineBlockTracer::Trace(*world, tracerCallbacks, start, end);
		tolua_pushboolean(L, res ? 1 : 0);
		return 1;
	}
	return L.ApiParamError("Invalid overload of cLineBlockTracer:Trace()");
}





static int tolua_cLuaWindow_new(lua_State * tolua_S)
{
	// Function signature:
	// cLuaWindow:new(type, slotsX, slotsY, title)

	// Check params:
	cLuaState L(tolua_S);
	if (
		!L.CheckParamUserTable(1, "cLuaWindow") ||
		!L.CheckParamNumber(2, 4) ||
		!L.CheckParamString(5) ||
		!L.CheckParamEnd(6)
	)
	{
		return 0;
	}

	// Read params:
	int windowType, slotsX, slotsY;
	AString title;
	if (!L.GetStackValues(2, windowType, slotsX, slotsY, title))
	{
		LOGWARNING("%s: Cannot read Lua parameters", __FUNCTION__);
		L.LogStackValues();
		L.LogStackTrace();
	}

	// Create the window and return it:
	L.Push(new cLuaWindow(L, static_cast<cLuaWindow::WindowType>(windowType), slotsX, slotsY, title));
	return 1;
}





static int tolua_cLuaWindow_new_local(lua_State * tolua_S)
{
	// Function signature:
	// cLuaWindow:new(type, slotsX, slotsY, title)

	// Check params:
	cLuaState L(tolua_S);
	if (
		!L.CheckParamUserTable(1, "cLuaWindow") ||
		!L.CheckParamNumber(2, 4) ||
		!L.CheckParamString(5) ||
		!L.CheckParamEnd(6)
	)
	{
		return 0;
	}

	// Read params:
	int windowType, slotsX, slotsY;
	AString title;
	if (!L.GetStackValues(2, windowType, slotsX, slotsY, title))
	{
		LOGWARNING("%s: Cannot read Lua parameters", __FUNCTION__);
		L.LogStackValues();
		L.LogStackTrace();
	}

	// Create the window, register it for GC and return it:
	L.Push(new cLuaWindow(L, static_cast<cLuaWindow::WindowType>(windowType), slotsX, slotsY, title));
	tolua_register_gc(tolua_S, lua_gettop(tolua_S));
	return 1;
}





static int tolua_cRoot_DoWithPlayerByUUID(lua_State * tolua_S)
{
	// Check params:
	cLuaState L(tolua_S);
	if (
		!L.CheckParamSelf("cRoot") ||
		!L.CheckParamUUID(2) ||
		!L.CheckParamFunction(3) ||
		!L.CheckParamEnd(4)
	)
	{
		return 0;
	}

	// Get parameters:
	cRoot * Self;
	cUUID PlayerUUID;
	cLuaState::cRef FnRef;
	L.GetStackValues(1, Self, PlayerUUID, FnRef);

	if (PlayerUUID.IsNil())
	{
		return L.ApiParamError("Expected a non-nil UUID for parameter #1");
	}
	if (!FnRef.IsValid())
	{
		return L.ApiParamError("Expected a valid callback function for parameter #2");
	}

	// Call the function:
	bool res = Self->DoWithPlayerByUUID(PlayerUUID, [&](cPlayer & a_Player)
		{
			bool ret = false;
			L.Call(FnRef, &a_Player, cLuaState::Return, ret);
			return ret;
		}
	);

	// Push the result as the return value:
	L.Push(res);
	return 1;
}





static int tolua_cRoot_GetBuildCommitID(lua_State * tolua_S)
{
	cLuaState L(tolua_S);
	L.Push(BUILD_COMMIT_ID);
	return 1;
}





static int tolua_cRoot_GetBuildDateTime(lua_State * tolua_S)
{
	cLuaState L(tolua_S);
	L.Push(BUILD_DATETIME);
	return 1;
}





static int tolua_cRoot_GetBuildID(lua_State * tolua_S)
{
	cLuaState L(tolua_S);
	L.Push(BUILD_ID);
	return 1;
}





static int tolua_cRoot_GetBuildSeriesName(lua_State * tolua_S)
{
	cLuaState L(tolua_S);
	L.Push(BUILD_SERIES_NAME);
	return 1;
}





static int tolua_cRoot_GetBrewingRecipe(lua_State * tolua_S)
{
	cLuaState L(tolua_S);
	if (
		!L.CheckParamUserTable(1, "cRoot") ||
		!L.CheckParamUserType (2, "const cItem") ||
		!L.CheckParamUserType (3, "const cItem") ||
		!L.CheckParamEnd      (4)
	)
	{
		return 0;
	}

	// Check the bottle param:
	cItem * Bottle = nullptr;
	L.GetStackValue(2, Bottle);
	if (Bottle == nullptr)
	{
		LOGWARNING("cRoot:GetBrewingRecipe: the Bottle parameter is nil or missing.");
		return 0;
	}

	cItem * Ingredient = nullptr;
	L.GetStackValue(3, Ingredient);
	if (Ingredient == nullptr)
	{
		LOGWARNING("cRoot:GetBrewingRecipe: the Ingredient parameter is nil or missing.");
		return 0;
	}

	// Get the recipe for the input
	cBrewingRecipes * BR = cRoot::Get()->GetBrewingRecipes();
	const cBrewingRecipes::cRecipe * Recipe = BR->GetRecipeFrom(*Bottle, *Ingredient);
	if (Recipe == nullptr)
	{
		// There is no such brewing recipe for this bottle and ingredient, return no value
		return 0;
	}

	// Push the output item
	cItem & OutItem = const_cast<cItem &>(Recipe->Output);
	L.Push(&OutItem);
	return 1;
}





static int tolua_cRoot_GetFurnaceRecipe(lua_State * tolua_S)
{
	cLuaState L(tolua_S);
	if (
		!L.CheckParamUserTable(1, "cRoot") ||
		!L.CheckParamUserType (2, "const cItem") ||
		!L.CheckParamEnd      (3)
	)
	{
		return 0;
	}

	// Check the input param:
	cItem * Input = nullptr;
	L.GetStackValue(2, Input);
	if (Input == nullptr)
	{
		LOGWARNING("cRoot:GetFurnaceRecipe: the Input parameter is nil or missing.");
		return 0;
	}

	// Get the recipe for the input
	cFurnaceRecipe * FR = cRoot::Get()->GetFurnaceRecipe();
	const cFurnaceRecipe::cRecipe * Recipe = FR->GetRecipeFrom(*Input);
	if (Recipe == nullptr)
	{
		// There is no such furnace recipe for this input, return no value
		return 0;
	}

	// Push the output, number of ticks and input as the three return values:
	L.Push(Recipe->Out, Recipe->CookTime, Recipe->In);
	return 3;
}





static int tolua_cServer_RegisterForgeMod(lua_State * a_LuaState)
{
	cLuaState L(a_LuaState);
	if (
		!L.CheckParamSelf("cServer") ||
		!L.CheckParamString(2, 3) ||
		!L.CheckParamNumber(4) ||
		!L.CheckParamEnd(5)
	)
	{
		return 0;
	}

	cServer * Server;
	AString Name, Version;
	UInt32 Protocol;
	L.GetStackValues(1, Server, Name, Version, Protocol);

	if (!Server->RegisterForgeMod(Name, Version, Protocol))
	{
		tolua_error(L, "duplicate Forge mod name registration", nullptr);
		return 0;
	}

	return 0;
}





static int tolua_cScoreboard_GetTeamNames(lua_State * L)
{
	cLuaState S(L);
	if (
		!S.CheckParamUserType(1, "cScoreboard") ||
		!S.CheckParamEnd(2)
	)
	{
		return 0;
	}

	// Get the groups:
	cScoreboard * Scoreboard = static_cast<cScoreboard *>(tolua_tousertype(L, 1, nullptr));
	AStringVector Teams = Scoreboard->GetTeamNames();

	// Push the results:
	S.Push(Teams);
	return 1;
}





static int tolua_cHopperEntity_GetOutputBlockPos(lua_State * tolua_S)
{
	// function cHopperEntity::GetOutputBlockPos()
	// Exported manually because tolua would require meaningless params

	cLuaState L(tolua_S);
	if (
		!L.CheckParamUserType(1, "cHopperEntity") ||
		!L.CheckParamNumber  (2) ||
		!L.CheckParamEnd     (3)
	)
	{
		return 0;
	}
	cHopperEntity * self = static_cast<cHopperEntity *>(tolua_tousertype(tolua_S, 1, nullptr));
	if (self == nullptr)
	{
		tolua_error(tolua_S, "invalid 'self' in function 'cHopperEntity::GetOutputBlockPos()'", nullptr);
		return 0;
	}

	NIBBLETYPE a_BlockMeta = static_cast<NIBBLETYPE>(tolua_tonumber(tolua_S, 2, 0));
	auto res = self->GetOutputBlockPos(a_BlockMeta);
	tolua_pushboolean(tolua_S, res.first);
	if (res.first)
	{
		tolua_pushnumber(tolua_S, static_cast<lua_Number>(res.second.x));
		tolua_pushnumber(tolua_S, static_cast<lua_Number>(res.second.y));
		tolua_pushnumber(tolua_S, static_cast<lua_Number>(res.second.z));
		return 4;
	}
	return 1;
}





static int tolua_cBoundingBox_CalcLineIntersection(lua_State * a_LuaState)
{
	/* Function signatures:
	bbox:CalcLineIntersection(pt1, pt2) -> bool, [number, blockface]
	cBoundingBox:CalcLineIntersection(min, max, pt1, pt2) -> bool, [number, blockface] (static)
	*/
	cLuaState L(a_LuaState);
	Vector3d min;
	Vector3d max;
	Vector3d pt1;
	Vector3d pt2;
	double lineCoeff;
	eBlockFace blockFace;
	bool res;
	if (L.GetStackValues(2, min, max, pt1, pt2))  // Try the static signature first
	{
		res = cBoundingBox::CalcLineIntersection(min, max, pt1, pt2, lineCoeff, blockFace);
	}
	else
	{
		const cBoundingBox * bbox;
		if (!L.GetStackValues(1, bbox, pt1, pt2))  // Try the regular signature
		{
			L.LogStackValues();
			tolua_error(a_LuaState, "Invalid function params. Expected either bbox:CalcLineIntersection(pt1, pt2) or cBoundingBox:CalcLineIntersection(min, max, pt1, pt2).", nullptr);
			return 0;
		}
		res = bbox->CalcLineIntersection(pt1, pt2, lineCoeff, blockFace);
	}
	L.Push(res);
	if (res)
	{
		L.Push(lineCoeff, blockFace);
		return 3;
	}
	return 1;
}





static int tolua_cBoundingBox_Intersect(lua_State * a_LuaState)
{
	/* Function signature:
	bbox:Intersect(a_OtherBbox) -> bool, cBoundingBox
	*/
	cLuaState L(a_LuaState);
	const cBoundingBox * self;
	const cBoundingBox * other;
	if (!L.GetStackValues(1, self, other))
	{
		L.LogStackValues();
		tolua_error(a_LuaState, "Invalid function params. Expected bbox:Intersect(otherBbox).", nullptr);
		return 0;
	}
	auto intersection = new cBoundingBox(*self);
	auto res = self->Intersect(*other, *intersection);
	L.Push(res);
	if (!res)
	{
		delete intersection;
		return 1;
	}
	L.Push(intersection);
	tolua_register_gc(L, lua_gettop(L));  // Make Lua own the "intersection" object
	return 2;
}





static int tolua_cChunkDesc_GetBlockTypeMeta(lua_State * a_LuaState)
{
	/* Function signature:
	ChunkDesc:GetBlockTypeMeta(RelX, RelY, RelZ) -> BlockType, BlockMeta
	*/

	cLuaState L(a_LuaState);
	const cChunkDesc * self;
	int relX, relY, relZ;
	if (!L.GetStackValues(1, self, relX, relY, relZ))
	{
		L.LogStackValues();
		tolua_error(a_LuaState, "Invalid function params. Expected chunkDesc:GetBlockTypeMeta(relX, relY, relZ)", nullptr);
		return 0;
	}
	BLOCKTYPE blockType;
	NIBBLETYPE blockMeta;
	self->GetBlockTypeMeta(relX, relY, relZ, blockType, blockMeta);
	L.Push(blockType, blockMeta);
	return 2;
}





static int tolua_cColor_GetColor(lua_State * tolua_S)
{
	cLuaState L(tolua_S);

	cColor * self;
	if (!L.CheckParamSelf("cColor") || !L.GetStackValue(1, self))
	{
		return 0;
	}

	L.Push(self->GetRed(), self->GetGreen(), self->GetBlue());
	return 3;
}





static int tolua_cCompositeChat_new(lua_State * a_LuaState)
{
	/* Function signatures:
	cCompositeChat()
	cCompositeChat(a_ParseText, a_MessageType)
	*/

	// Check if it's the no-param version:
	cLuaState L(a_LuaState);
	if (lua_isnone(a_LuaState, 2))
	{
		auto * res = static_cast<cCompositeChat *>(Mtolua_new(cCompositeChat()));
		L.Push(res);
		return 1;
	}

	// Check the second signature:
	AString parseText;
	if (!L.GetStackValue(2, parseText))
	{
		tolua_Error err;
		tolua_error(a_LuaState, "Invalid ParseText parameter (1) in cCompositeChat constructor.", &err);
		return 0;
	}
	int messageTypeInt = mtCustom;
	if (!lua_isnone(a_LuaState, 3))
	{
		if (!L.GetStackValue(3, messageTypeInt))
		{
			tolua_Error err;
			tolua_error(a_LuaState, "Invalid type of the MessageType parameter (2) in cCompositeChat constructor.", &err);
			return 0;
		}
		if ((messageTypeInt < 0) || (messageTypeInt >= mtMaxPlusOne))
		{
			tolua_Error err;
			tolua_error(a_LuaState, "Invalid MessageType parameter (2) value in cCompositeChat constructor.", &err);
			return 0;
		}
	}
	L.Push(static_cast<cCompositeChat *>(Mtolua_new(cCompositeChat(parseText, static_cast<eMessageType>(messageTypeInt)))));
	return 1;
}





static int tolua_cCompositeChat_new_local(lua_State * a_LuaState)
{
	// Use the same constructor as global, just register it for GC:
	auto res = tolua_cCompositeChat_new(a_LuaState);
	if (res == 1)
	{
		tolua_register_gc(a_LuaState, lua_gettop(a_LuaState));
	}
	return res;
}





static int tolua_cCompositeChat_AddRunCommandPart(lua_State * tolua_S)
{
	// function cCompositeChat:AddRunCommandPart(Message, Command, [Style])
	// Exported manually to support call-chaining (return *this)

	// Check params:
	cLuaState L(tolua_S);
	if (
		!L.CheckParamUserType(1, "cCompositeChat") ||
		!L.CheckParamString(2, 3)
	)
	{
		return 0;
	}
	cCompositeChat * self = static_cast<cCompositeChat *>(tolua_tousertype(tolua_S, 1, nullptr));
	if (self == nullptr)
	{
		tolua_error(tolua_S, "invalid 'self' in function 'cCompositeChat:AddRunCommandPart'", nullptr);
		return 0;
	}

	// Add the part:
	AString Text, Command, Style = "u@a";
	L.GetStackValue(2, Text);
	L.GetStackValue(3, Command);
	L.GetStackValue(4, Style);
	self->AddRunCommandPart(Text, Command, Style);

	// Cut away everything from the stack except for the cCompositeChat instance; return that:
	lua_settop(L, 1);
	return 1;
}





static int tolua_cCompositeChat_AddSuggestCommandPart(lua_State * tolua_S)
{
	// function cCompositeChat:AddSuggestCommandPart(Message, Command, [Style])
	// Exported manually to support call-chaining (return *this)

	// Check params:
	cLuaState L(tolua_S);
	if (
		!L.CheckParamUserType(1, "cCompositeChat") ||
		!L.CheckParamString(2, 3)
	)
	{
		return 0;
	}
	cCompositeChat * self = static_cast<cCompositeChat *>(tolua_tousertype(tolua_S, 1, nullptr));
	if (self == nullptr)
	{
		tolua_error(tolua_S, "invalid 'self' in function 'cCompositeChat:AddSuggestCommandPart'", nullptr);
		return 0;
	}

	// Add the part:
	AString Text, Command, Style;
	L.GetStackValue(2, Text);
	L.GetStackValue(3, Command);
	L.GetStackValue(4, Style);
	self->AddSuggestCommandPart(Text, Command, Style);

	// Cut away everything from the stack except for the cCompositeChat instance; return that:
	lua_settop(L, 1);
	return 1;
}





static int tolua_cCompositeChat_AddTextPart(lua_State * tolua_S)
{
	// function cCompositeChat:AddTextPart(Message, [Style])
	// Exported manually to support call-chaining (return *this)

	// Check params:
	cLuaState L(tolua_S);
	if (
		!L.CheckParamUserType(1, "cCompositeChat") ||
		!L.CheckParamString(2)
	)
	{
		return 0;
	}
	cCompositeChat * self = static_cast<cCompositeChat *>(tolua_tousertype(tolua_S, 1, nullptr));
	if (self == nullptr)
	{
		tolua_error(tolua_S, "invalid 'self' in function 'cCompositeChat:AddTextPart'", nullptr);
		return 0;
	}

	// Add the part:
	AString Text, Style;
	L.GetStackValue(2, Text);
	L.GetStackValue(3, Style);
	self->AddTextPart(Text, Style);

	// Cut away everything from the stack except for the cCompositeChat instance; return that:
	lua_settop(L, 1);
	return 1;
}





static int tolua_cCompositeChat_AddUrlPart(lua_State * tolua_S)
{
	// function cCompositeChat:AddTextPart(Message, Url, [Style])
	// Exported manually to support call-chaining (return *this)

	// Check params:
	cLuaState L(tolua_S);
	if (
		!L.CheckParamUserType(1, "cCompositeChat") ||
		!L.CheckParamString(2, 3)
	)
	{
		return 0;
	}
	cCompositeChat * self = static_cast<cCompositeChat *>(tolua_tousertype(tolua_S, 1, nullptr));
	if (self == nullptr)
	{
		tolua_error(tolua_S, "invalid 'self' in function 'cCompositeChat:AddUrlPart'", nullptr);
		return 0;
	}

	// Add the part:
	AString Text, Url, Style;
	L.GetStackValue(2, Text);
	L.GetStackValue(3, Url);
	L.GetStackValue(4, Style);
	self->AddUrlPart(Text, Url, Style);

	// Cut away everything from the stack except for the cCompositeChat instance; return that:
	lua_settop(L, 1);
	return 1;
}





static int tolua_cCompositeChat_Clear(lua_State * tolua_S)
{
	// function cCompositeChat:Clear()
	// Exported manually to support call-chaining (return *this)

	// Check params:
	cLuaState L(tolua_S);
	if (
		!L.CheckParamUserType(1, "cCompositeChat") ||
		!L.CheckParamEnd(2)
	)
	{
		return 0;
	}
	cCompositeChat * self = static_cast<cCompositeChat *>(tolua_tousertype(tolua_S, 1, nullptr));
	if (self == nullptr)
	{
		tolua_error(tolua_S, "invalid 'self' in function 'cCompositeChat:ParseText'", nullptr);
		return 0;
	}

	// Clear all the parts:
	self->Clear();

	// Cut away everything from the stack except for the cCompositeChat instance; return that:
	lua_settop(L, 1);
	return 1;
}





static int tolua_cCompositeChat_ParseText(lua_State * tolua_S)
{
	// function cCompositeChat:ParseText(TextMessage)
	// Exported manually to support call-chaining (return *this)

	// Check params:
	cLuaState L(tolua_S);
	if (
		!L.CheckParamUserType(1, "cCompositeChat") ||
		!L.CheckParamString(2)
	)
	{
		return 0;
	}
	cCompositeChat * self = static_cast<cCompositeChat *>(tolua_tousertype(tolua_S, 1, nullptr));
	if (self == nullptr)
	{
		tolua_error(tolua_S, "invalid 'self' in function 'cCompositeChat:ParseText'", nullptr);
		return 0;
	}

	// Parse the text:
	AString Text;
	L.GetStackValue(2, Text);
	self->ParseText(Text);

	// Cut away everything from the stack except for the cCompositeChat instance; return that:
	lua_settop(L, 1);
	return 1;
}





static int tolua_cCompositeChat_SetMessageType(lua_State * tolua_S)
{
	// function cCompositeChat:SetMessageType(MessageType)
	// Exported manually to support call-chaining (return *this)

	// Check params:
	cLuaState L(tolua_S);
	if (
		!L.CheckParamUserType(1, "cCompositeChat") ||
		!L.CheckParamNumber(2)
	)
	{
		return 0;
	}
	cCompositeChat * self = static_cast<cCompositeChat *>(tolua_tousertype(tolua_S, 1, nullptr));
	if (self == nullptr)
	{
		tolua_error(tolua_S, "invalid 'self' in function 'cCompositeChat:SetMessageType'", nullptr);
		return 0;
	}

	// Set the type:
	int MessageType = mtCustom;
	L.GetStackValue(2, MessageType);
	self->SetMessageType(static_cast<eMessageType>(MessageType));

	// Cut away everything from the stack except for the cCompositeChat instance; return that:
	lua_settop(L, 1);
	return 1;
}





static int tolua_cCompositeChat_UnderlineUrls(lua_State * tolua_S)
{
	// function cCompositeChat:UnderlineUrls()
	// Exported manually to support call-chaining (return self)

	// Check params:
	cLuaState L(tolua_S);
	if (!L.CheckParamUserType(1, "cCompositeChat"))
	{
		return 0;
	}
	cCompositeChat * self = static_cast<cCompositeChat *>(tolua_tousertype(tolua_S, 1, nullptr));
	if (self == nullptr)
	{
		tolua_error(tolua_S, "invalid 'self' in function 'cCompositeChat:UnderlineUrls'", nullptr);
		return 0;
	}

	// Call the processing
	self->UnderlineUrls();

	// Cut away everything from the stack except for the cCompositeChat instance; return that:
	lua_settop(L, 1);
	return 1;
}





static int tolua_cCuboid_Assign(lua_State * tolua_S)
{
	cLuaState L(tolua_S);

	if (!L.CheckParamSelf("cCuboid"))
	{
		return 0;
	}

	cCuboid * self = nullptr;
	L.GetStackValue(1, self);

	// Check the old coord-based signature:
	int x1, y1, z1, x2, y2, z2;
	if (L.GetStackValues(2, x1, y1, z1, x2, y2, z2))
	{
		LOGWARNING("cCuboid:Assign(x1, y1, z1, x2, y2, z2) is deprecated, use cCuboid:Assign(Vector3i, Vector3i) instead.");
		L.LogStackTrace();
		self->Assign({x1, y1, z1}, {x2, y2, z2});
		return 0;
	}

	// Try the (cCuboid) param version:
	cCuboid * other = nullptr;
	if (L.GetStackValue(2, other) && (other != nullptr))
	{
		self->Assign(*other);
		return 0;
	}

	// Try the (Vector3i, Vector3i) param version:
	Vector3i pt1;
	Vector3i pt2;
	if (L.GetStackValues(2, pt1, pt2))
	{
		self->Assign(pt1, pt2);
		return 0;
	}
	return L.ApiParamError("Invalid parameter, expected either a cCuboid or two Vector3i-s.");
}





static int tolua_cCuboid_IsInside(lua_State * tolua_S)
{
	cLuaState L(tolua_S);

	if (!L.CheckParamSelf("cCuboid"))
	{
		return 0;
	}

	cCuboid * self = nullptr;
	L.GetStackValue(1, self);

	// Check the old coord-based signature:
	int x, y, z;
	if (L.GetStackValues(2, x, y, z))
	{
		LOGWARNING("cCuboid:IsInside(x, y, z) is deprecated, use cCuboid:IsInside(Vector3) instead.");
		L.LogStackTrace();
		L.Push(self->IsInside(Vector3i{x, y, z}));
		return 1;
	}

	// Try the Vector3 param version:
	Vector3d pt;
	if (L.GetStackValue(2, pt))
	{
		L.Push(self->IsInside(pt));
		return 1;
	}
	return L.ApiParamError("Invalid parameter #2, expected a Vector3.");
}





static int tolua_cCuboid_Move(lua_State * tolua_S)
{
	cLuaState L(tolua_S);

	if (!L.CheckParamSelf("cCuboid"))
	{
		return 0;
	}

	cCuboid * self = nullptr;
	L.GetStackValue(1, self);

	// Check the old coord-based signature:
	int x, y, z;
	if (L.GetStackValues(2, x, y, z))
	{
		LOGWARNING("cCuboid:Move(x, y, z) is deprecated, use cCuboid:Move(Vector3i) instead.");
		L.LogStackTrace();
		self->Move({x, y, z});
		return 0;
	}

	Vector3i offset;
	if (!L.GetStackValue(2, offset))
	{
		return L.ApiParamError("Invalid parameter #2, expected a Vector3.");
	}
	self->Move(offset);
	return 0;
}





static int tolua_cEntity_Destroy(lua_State * tolua_S)
{
	// Check the params:
	cLuaState L(tolua_S);
	if (!L.CheckParamSelf("cEntity"))
	{
		return 0;
	}

	// Get the params:
	cEntity * self = nullptr;
	L.GetStackValue(1, self);

	if (lua_gettop(L) == 2)
	{
		LOGWARNING("cEntity:Destroy(bool) is deprecated, use cEntity:Destroy() instead.");
	}

	if (self->IsPlayer())
	{
		return L.ApiParamError("Cannot call cEntity:Destroy() on a cPlayer, use cClientHandle:Kick() instead.");
	}

	self->Destroy();
	return 0;
}





static int tolua_cEntity_IsSubmerged(lua_State * tolua_S)
{
	// Check the params:
	cLuaState L(tolua_S);
	if (!L.CheckParamSelf("cEntity"))
	{
		return 0;
	}

	// Get the params:
	cEntity * self = nullptr;
	L.GetStackValue(1, self);

	// API function deprecated:
	LOGWARNING("cEntity:IsSubmerged() is deprecated. Use cEntity:IsHeadInWater() instead.");
	cLuaState::LogStackTrace(tolua_S);

	L.Push(self->IsHeadInWater());
	return 1;
}





static int tolua_cEntity_IsSwimming(lua_State * tolua_S)
{
	// Check the params:
	cLuaState L(tolua_S);
	if (!L.CheckParamSelf("cEntity"))
	{
		return 0;
	}

	// Get the params:
	cEntity * self = nullptr;
	L.GetStackValue(1, self);

	// API function deprecated
	LOGWARNING("cEntity:IsSwimming() is deprecated. Use cEntity:IsInWater() instead.");
	cLuaState::LogStackTrace(tolua_S);

	L.Push(self->IsInWater());
	return 1;
}





static int tolua_cEntity_GetPosition(lua_State * tolua_S)
{
	cLuaState L(tolua_S);

	// Get the params:
	cEntity * self = static_cast<cEntity *>(tolua_tousertype(tolua_S, 1, nullptr));
	if (self == nullptr)
	{
		LOGWARNING("%s: invalid self (%p)", __FUNCTION__, static_cast<void *>(self));
		return 0;
	}

	L.Push(Mtolua_new((Vector3d)(self->GetPosition())));

	tolua_register_gc(L, lua_gettop(L));  // Make Lua own the object
	return 1;
}





static int tolua_cEntity_GetSpeed(lua_State * tolua_S)
{
	cLuaState L(tolua_S);

	// Get the params:
	cEntity * self = static_cast<cEntity *>(tolua_tousertype(tolua_S, 1, nullptr));
	if (self == nullptr)
	{
		LOGWARNING("%s: invalid self (%p)", __FUNCTION__, static_cast<void *>(self));
		return 0;
	}

	L.Push(Mtolua_new((Vector3d)(self->GetSpeed())));

	tolua_register_gc(L, lua_gettop(L));  // Make Lua own the object
	return 1;
}





void cManualBindings::Bind(lua_State * tolua_S)
{
	tolua_beginmodule(tolua_S, nullptr);

		// Create the new classes:
		tolua_usertype(tolua_S, "cCryptoHash");
		tolua_usertype(tolua_S, "cLineBlockTracer");
		tolua_usertype(tolua_S, "cStringCompression");
		tolua_usertype(tolua_S, "cUrlParser");
		tolua_cclass(tolua_S, "cCryptoHash",        "cCryptoHash",        "", nullptr);
		tolua_cclass(tolua_S, "cLineBlockTracer",   "cLineBlockTracer",   "", nullptr);
		tolua_cclass(tolua_S, "cStringCompression", "cStringCompression", "", nullptr);
		tolua_cclass(tolua_S, "cUrlParser",         "cUrlParser",         "", nullptr);

		// Globals:
		tolua_function(tolua_S, "Clamp",                 tolua_Clamp);
		tolua_function(tolua_S, "StringSplit",           tolua_StringSplit);
		tolua_function(tolua_S, "StringSplitWithQuotes", tolua_StringSplitWithQuotes);
		tolua_function(tolua_S, "StringSplitAndTrim",    tolua_StringSplitAndTrim);
		tolua_function(tolua_S, "LOG",                   tolua_LOG);
		tolua_function(tolua_S, "LOGINFO",               tolua_LOGINFO);
		tolua_function(tolua_S, "LOGWARN",               tolua_LOGWARN);
		tolua_function(tolua_S, "LOGWARNING",            tolua_LOGWARN);
		tolua_function(tolua_S, "LOGERROR",              tolua_LOGERROR);
		tolua_function(tolua_S, "Base64Encode",          tolua_Base64Encode);
		tolua_function(tolua_S, "Base64Decode",          tolua_Base64Decode);
		tolua_function(tolua_S, "md5",                   tolua_md5_obsolete);  // OBSOLETE, use cCryptoHash.md5() instead

		tolua_beginmodule(tolua_S, "cBoundingBox");
			tolua_function(tolua_S, "CalcLineIntersection", tolua_cBoundingBox_CalcLineIntersection);
			tolua_function(tolua_S, "Intersect",            tolua_cBoundingBox_Intersect);
		tolua_endmodule(tolua_S);

		tolua_beginmodule(tolua_S, "cChunkDesc");
			tolua_function(tolua_S, "GetBlockTypeMeta", tolua_cChunkDesc_GetBlockTypeMeta);
		tolua_endmodule(tolua_S);

		tolua_beginmodule(tolua_S, "cClientHandle");
			tolua_constant(tolua_S, "MAX_VIEW_DISTANCE",   cClientHandle::MAX_VIEW_DISTANCE);
			tolua_constant(tolua_S, "MIN_VIEW_DISTANCE",   cClientHandle::MIN_VIEW_DISTANCE);

			tolua_function(tolua_S, "GetForgeMods", tolua_cClientHandle_GetForgeMods);
			tolua_function(tolua_S, "SendPluginMessage",   tolua_cClientHandle_SendPluginMessage);
			tolua_function(tolua_S, "GetUUID",             tolua_cClientHandle_GetUUID);
			tolua_function(tolua_S, "GenerateOfflineUUID", tolua_cClientHandle_GenerateOfflineUUID);
			tolua_function(tolua_S, "IsUUIDOnline",        tolua_cClientHandle_IsUUIDOnline);
		tolua_endmodule(tolua_S);

		tolua_beginmodule(tolua_S, "cColor");
			tolua_function(tolua_S, "GetColor", tolua_cColor_GetColor);
		tolua_endmodule(tolua_S);

		tolua_beginmodule(tolua_S, "cCompositeChat");
			tolua_function(tolua_S, "new",                   tolua_cCompositeChat_new);
			tolua_function(tolua_S, "new_local",             tolua_cCompositeChat_new_local);
			tolua_function(tolua_S, ".call",                 tolua_cCompositeChat_new_local);
			tolua_function(tolua_S, "AddRunCommandPart",     tolua_cCompositeChat_AddRunCommandPart);
			tolua_function(tolua_S, "AddSuggestCommandPart", tolua_cCompositeChat_AddSuggestCommandPart);
			tolua_function(tolua_S, "AddTextPart",           tolua_cCompositeChat_AddTextPart);
			tolua_function(tolua_S, "AddUrlPart",            tolua_cCompositeChat_AddUrlPart);
			tolua_function(tolua_S, "Clear",                 tolua_cCompositeChat_Clear);
			tolua_function(tolua_S, "ParseText",             tolua_cCompositeChat_ParseText);
			tolua_function(tolua_S, "SetMessageType",        tolua_cCompositeChat_SetMessageType);
			tolua_function(tolua_S, "UnderlineUrls",         tolua_cCompositeChat_UnderlineUrls);
		tolua_endmodule(tolua_S);

		tolua_beginmodule(tolua_S, "cCryptoHash");
			tolua_function(tolua_S, "md5", tolua_md5);
			tolua_function(tolua_S, "md5HexString", tolua_md5HexString);
			tolua_function(tolua_S, "sha1", tolua_sha1);
			tolua_function(tolua_S, "sha1HexString", tolua_sha1HexString);
		tolua_endmodule(tolua_S);

		tolua_beginmodule(tolua_S, "cCuboid");
			tolua_function(tolua_S, "Assign",   tolua_cCuboid_Assign);
			tolua_function(tolua_S, "IsInside", tolua_cCuboid_IsInside);
			tolua_function(tolua_S, "Move",     tolua_cCuboid_Move);
		tolua_endmodule(tolua_S);

		tolua_beginmodule(tolua_S, "cEntity");
			tolua_constant(tolua_S, "INVALID_ID", cEntity::INVALID_ID);
			tolua_function(tolua_S, "Destroy", tolua_cEntity_Destroy);
			tolua_function(tolua_S, "IsSubmerged", tolua_cEntity_IsSubmerged);
			tolua_function(tolua_S, "IsSwimming", tolua_cEntity_IsSwimming);
			tolua_function(tolua_S, "GetPosition", tolua_cEntity_GetPosition);
			tolua_function(tolua_S, "GetSpeed", tolua_cEntity_GetSpeed);
		tolua_endmodule(tolua_S);

		tolua_beginmodule(tolua_S, "cFile");
			tolua_function(tolua_S, "ChangeFileExt",           tolua_cFile_ChangeFileExt);
			tolua_function(tolua_S, "Copy",                    tolua_cFile_Copy);
			tolua_function(tolua_S, "CreateFolder",            tolua_cFile_CreateFolder);
			tolua_function(tolua_S, "CreateFolderRecursive",   tolua_cFile_CreateFolderRecursive);
			tolua_function(tolua_S, "Delete",                  tolua_cFile_Delete);
			tolua_function(tolua_S, "DeleteFile",              tolua_cFile_DeleteFile);
			tolua_function(tolua_S, "DeleteFolder",            tolua_cFile_DeleteFolder);
			tolua_function(tolua_S, "DeleteFolderContents",    tolua_cFile_DeleteFolderContents);
			tolua_function(tolua_S, "Exists",                  tolua_cFile_Exists);
			tolua_function(tolua_S, "GetFolderContents",       tolua_cFile_GetFolderContents);
			tolua_function(tolua_S, "GetLastModificationTime", tolua_cFile_GetLastModificationTime);
			tolua_function(tolua_S, "GetSize",                 tolua_cFile_GetSize);
			tolua_function(tolua_S, "IsFile",                  tolua_cFile_IsFile);
			tolua_function(tolua_S, "IsFolder",                tolua_cFile_IsFolder);
			tolua_function(tolua_S, "ReadWholeFile",           tolua_cFile_ReadWholeFile);
			tolua_function(tolua_S, "Rename",                  tolua_cFile_Rename);
		tolua_endmodule(tolua_S);

		tolua_beginmodule(tolua_S, "cHopperEntity");
			tolua_function(tolua_S, "GetOutputBlockPos", tolua_cHopperEntity_GetOutputBlockPos);
		tolua_endmodule(tolua_S);

		tolua_beginmodule(tolua_S, "cItem");
			tolua_function(tolua_S, "EnchantByXPLevels", tolua_cItem_EnchantByXPLevels);
			tolua_variable(tolua_S, "m_LoreTable",       tolua_get_cItem_m_LoreTable, tolua_set_cItem_m_LoreTable);
		tolua_endmodule(tolua_S);

		tolua_beginmodule(tolua_S, "cItemGrid");
			tolua_function(tolua_S, "GetSlotCoords", Lua_ItemGrid_GetSlotCoords);
		tolua_endmodule(tolua_S);

		tolua_beginmodule(tolua_S, "cLineBlockTracer");
			tolua_function(tolua_S, "FirstSolidHitTrace", tolua_cLineBlockTracer_FirstSolidHitTrace);
			tolua_function(tolua_S, "LineOfSightTrace",   tolua_cLineBlockTracer_LineOfSightTrace);
			tolua_function(tolua_S, "Trace",              tolua_cLineBlockTracer_Trace);

			tolua_constant(tolua_S, "losAir",   cLineBlockTracer::losAir);
			tolua_constant(tolua_S, "losWater", cLineBlockTracer::losWater);
			tolua_constant(tolua_S, "losLava",  cLineBlockTracer::losLava);
		tolua_endmodule(tolua_S);

		tolua_beginmodule(tolua_S, "cLuaWindow");
			tolua_function(tolua_S, "new",              tolua_cLuaWindow_new);
			tolua_function(tolua_S, "new_local",        tolua_cLuaWindow_new_local);
			tolua_function(tolua_S, ".call",            tolua_cLuaWindow_new_local);
			tolua_function(tolua_S, "SetOnClicked",     tolua_SetObjectCallback<cLuaWindow, &cLuaWindow::SetOnClicked>);
			tolua_function(tolua_S, "SetOnClosing",     tolua_SetObjectCallback<cLuaWindow, &cLuaWindow::SetOnClosing>);
			tolua_function(tolua_S, "SetOnSlotChanged", tolua_SetObjectCallback<cLuaWindow, &cLuaWindow::SetOnSlotChanged>);
		tolua_endmodule(tolua_S);

		tolua_beginmodule(tolua_S, "cMapManager");
			tolua_function(tolua_S, "DoWithMap", DoWithID<cMapManager, cMap, &cMapManager::DoWithMap>);
		tolua_endmodule(tolua_S);

		tolua_beginmodule(tolua_S, "cMobHeadEntity");
			tolua_function(tolua_S, "SetOwner",     tolua_cMobHeadEntity_SetOwner);
			tolua_function(tolua_S, "GetOwnerUUID", tolua_cMobHeadEntity_GetOwnerUUID);
		tolua_endmodule(tolua_S);

		tolua_beginmodule(tolua_S, "cMojangAPI");
			tolua_function(tolua_S, "AddPlayerNameToUUIDMapping", tolua_cMojangAPI_AddPlayerNameToUUIDMapping);
			tolua_function(tolua_S, "GetPlayerNameFromUUID",      tolua_cMojangAPI_GetPlayerNameFromUUID);
			tolua_function(tolua_S, "GetUUIDFromPlayerName",      tolua_cMojangAPI_GetUUIDFromPlayerName);
			tolua_function(tolua_S, "GetUUIDsFromPlayerNames",    tolua_cMojangAPI_GetUUIDsFromPlayerNames);
			tolua_function(tolua_S, "MakeUUIDDashed",             tolua_cMojangAPI_MakeUUIDDashed);
			tolua_function(tolua_S, "MakeUUIDShort",              tolua_cMojangAPI_MakeUUIDShort);
		tolua_endmodule(tolua_S);

		tolua_beginmodule(tolua_S, "cPlayer");
			tolua_function(tolua_S, "GetPermissions",    tolua_cPlayer_GetPermissions);
			tolua_function(tolua_S, "GetRestrictions",   tolua_cPlayer_GetRestrictions);
			tolua_function(tolua_S, "PermissionMatches", tolua_cPlayer_PermissionMatches);
			tolua_function(tolua_S, "GetUUID",           tolua_cPlayer_GetUUID);
		tolua_endmodule(tolua_S);

		tolua_beginmodule(tolua_S, "cPlugin");
			tolua_function(tolua_S, "GetDirectory",      tolua_cPlugin_GetDirectory);
			tolua_function(tolua_S, "GetLocalDirectory", tolua_cPlugin_GetLocalDirectory);
		tolua_endmodule(tolua_S);

		tolua_beginmodule(tolua_S, "cPluginLua");
			tolua_function(tolua_S, "AddWebTab", tolua_cPluginLua_AddWebTab);
		tolua_endmodule(tolua_S);

		tolua_beginmodule(tolua_S, "cPluginManager");
			tolua_function(tolua_S, "AddHook",               tolua_cPluginManager_AddHook);
			tolua_function(tolua_S, "BindCommand",           tolua_cPluginManager_BindCommand);
			tolua_function(tolua_S, "BindConsoleCommand",    tolua_cPluginManager_BindConsoleCommand);
			tolua_function(tolua_S, "CallPlugin",            tolua_cPluginManager_CallPlugin);
			tolua_function(tolua_S, "DoWithPlugin",          StaticDoWith<cPluginManager, cPlugin, &cPluginManager::DoWithPlugin>);
			tolua_function(tolua_S, "ExecuteConsoleCommand", tolua_cPluginManager_ExecuteConsoleCommand);
			tolua_function(tolua_S, "FindPlugins",           tolua_cPluginManager_FindPlugins);
			tolua_function(tolua_S, "ForEachCommand",        tolua_cPluginManager_ForEachCommand);
			tolua_function(tolua_S, "ForEachConsoleCommand", tolua_cPluginManager_ForEachConsoleCommand);
			tolua_function(tolua_S, "ForEachPlugin",         StaticForEach<cPluginManager, cPlugin, &cPluginManager::ForEachPlugin>);
			tolua_function(tolua_S, "GetAllPlugins",         tolua_cPluginManager_GetAllPlugins);
			tolua_function(tolua_S, "GetCurrentPlugin",      tolua_cPluginManager_GetCurrentPlugin);
			tolua_function(tolua_S, "GetPlugin",             tolua_cPluginManager_GetPlugin);
			tolua_function(tolua_S, "LogStackTrace",         tolua_cPluginManager_LogStackTrace);
		tolua_endmodule(tolua_S);

		tolua_beginmodule(tolua_S, "cRoot");
			tolua_function(tolua_S, "DoWithPlayerByUUID",  tolua_cRoot_DoWithPlayerByUUID);
			tolua_function(tolua_S, "FindAndDoWithPlayer", DoWith <cRoot, cPlayer, &cRoot::FindAndDoWithPlayer>);
			tolua_function(tolua_S, "ForEachPlayer",       ForEach<cRoot, cPlayer, &cRoot::ForEachPlayer>);
			tolua_function(tolua_S, "ForEachWorld",        ForEach<cRoot, cWorld,  &cRoot::ForEachWorld>);
			tolua_function(tolua_S, "GetBrewingRecipe",    tolua_cRoot_GetBrewingRecipe);
			tolua_function(tolua_S, "GetBuildCommitID",    tolua_cRoot_GetBuildCommitID);
			tolua_function(tolua_S, "GetBuildDateTime",    tolua_cRoot_GetBuildDateTime);
			tolua_function(tolua_S, "GetBuildID",          tolua_cRoot_GetBuildID);
			tolua_function(tolua_S, "GetBuildSeriesName",  tolua_cRoot_GetBuildSeriesName);
			tolua_function(tolua_S, "GetFurnaceRecipe",    tolua_cRoot_GetFurnaceRecipe);
		tolua_endmodule(tolua_S);

		tolua_beginmodule(tolua_S, "cScoreboard");
			tolua_function(tolua_S, "ForEachObjective", ForEach<cScoreboard, cObjective, &cScoreboard::ForEachObjective>);
			tolua_function(tolua_S, "ForEachTeam",      ForEach<cScoreboard, cTeam,      &cScoreboard::ForEachTeam>);
			tolua_function(tolua_S, "GetTeamNames",     tolua_cScoreboard_GetTeamNames);
		tolua_endmodule(tolua_S);

		tolua_beginmodule(tolua_S, "cServer");
			tolua_function(tolua_S, "RegisterForgeMod",            tolua_cServer_RegisterForgeMod);
		tolua_endmodule(tolua_S);

		tolua_beginmodule(tolua_S, "cStringCompression");
			tolua_function(tolua_S, "CompressStringZLIB",     tolua_CompressStringZLIB);
			tolua_function(tolua_S, "UncompressStringZLIB",   tolua_UncompressStringZLIB);
			tolua_function(tolua_S, "CompressStringGZIP",     tolua_CompressStringGZIP);
			tolua_function(tolua_S, "UncompressStringGZIP",   tolua_UncompressStringGZIP);
			tolua_function(tolua_S, "InflateString",          tolua_InflateString);
		tolua_endmodule(tolua_S);

		tolua_beginmodule(tolua_S, "cUrlParser");
			tolua_function(tolua_S, "GetDefaultPort",     tolua_cUrlParser_GetDefaultPort);
			tolua_function(tolua_S, "IsKnownScheme",      tolua_cUrlParser_IsKnownScheme);
			tolua_function(tolua_S, "Parse",              tolua_cUrlParser_Parse);
			tolua_function(tolua_S, "ParseAuthorityPart", tolua_cUrlParser_ParseAuthorityPart);
			tolua_function(tolua_S, "UrlDecode",          tolua_cUrlParser_UrlDecode);
			tolua_function(tolua_S, "UrlEncode",          tolua_cUrlParser_UrlEncode);
		tolua_endmodule(tolua_S);

		tolua_beginmodule(tolua_S, "cWebAdmin");
			tolua_function(tolua_S, "AddWebTab",                 tolua_cWebAdmin_AddWebTab);
			tolua_function(tolua_S, "GetAllWebTabs",             tolua_cWebAdmin_GetAllWebTabs);
			tolua_function(tolua_S, "GetPage",                   tolua_cWebAdmin_GetPage);
			tolua_function(tolua_S, "GetURLEncodedString",       tolua_cWebAdmin_GetURLEncodedString);
		tolua_endmodule(tolua_S);

		tolua_beginmodule(tolua_S, "HTTPRequest");
			tolua_variable(tolua_S, "FormData",   tolua_get_HTTPRequest_FormData,   nullptr);
			tolua_variable(tolua_S, "Params",     tolua_get_HTTPRequest_Params,     nullptr);
			tolua_variable(tolua_S, "PostParams", tolua_get_HTTPRequest_PostParams, nullptr);
		tolua_endmodule(tolua_S);

		BindNetwork(tolua_S);
		BindRankManager(tolua_S);
		BindWorld(tolua_S);
		BindBlockArea(tolua_S);

	tolua_endmodule(tolua_S);
}
