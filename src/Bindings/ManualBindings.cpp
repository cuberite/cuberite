
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "ManualBindings.h"
#undef TOLUA_TEMPLATE_BIND
#include <sstream>
#include <iomanip>
#include <array>
#include "tolua++/include/tolua++.h"
#include "polarssl/md5.h"
#include "polarssl/sha1.h"
#include "PluginLua.h"
#include "PluginManager.h"
#include "LuaWindow.h"
#include "LuaChunkStay.h"
#include "../Root.h"
#include "../World.h"
#include "../Entities/Player.h"
#include "../WebAdmin.h"
#include "../ClientHandle.h"
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
#include "../LineBlockTracer.h"
#include "../WorldStorage/SchematicFileSerializer.h"
#include "../CompositeChat.h"
#include "../StringCompression.h"
#include "../CommandOutput.h"
#include "../BuildInfo.h"
#include "../HTTP/UrlParser.h"





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





int cManualBindings::lua_do_error(lua_State * L, const char * a_pFormat, ...)
{
	// Retrieve current function name
	lua_Debug entry;
	VERIFY(lua_getstack(L, 0, &entry));
	VERIFY(lua_getinfo(L, "n", &entry));

	// Insert function name into error msg
	AString msg(a_pFormat);
	ReplaceString(msg, "#funcname#", (entry.name != nullptr) ? entry.name : "?");

	// Copied from luaL_error and modified
	va_list argp;
	va_start(argp, a_pFormat);
	luaL_where(L, 1);
	lua_pushvfstring(L, msg.c_str(), argp);
	va_end(argp);
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
static AString GetLogMessage(lua_State * tolua_S)
{
	tolua_Error err;
	if (tolua_isusertype(tolua_S, 1, "cCompositeChat", false, &err))
	{
		return reinterpret_cast<cCompositeChat *>(tolua_tousertype(tolua_S, 1, nullptr))->ExtractText();
	}
	else
	{
		size_t len = 0;
		const char * str = lua_tolstring(tolua_S, 1, &len);
		if (str != nullptr)
		{
			return AString(str, len);
		}
	}
	return "";
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
	cLogger::eLogLevel LogLevel = cLogger::llRegular;
	tolua_Error err;
	if (tolua_isusertype(tolua_S, 1, "cCompositeChat", false, &err))
	{
		LogLevel = cCompositeChat::MessageTypeToLogLevel(reinterpret_cast<cCompositeChat *>(tolua_tousertype(tolua_S, 1, nullptr))->GetMessageType());
	}

	// Log the message:
	cLogger::GetInstance().LogSimple(GetLogMessage(tolua_S).c_str(), LogLevel);
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

	cLogger::GetInstance().LogSimple(GetLogMessage(tolua_S).c_str(), cLogger::llInfo);
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

	cLogger::GetInstance().LogSimple(GetLogMessage(tolua_S).c_str(), cLogger::llWarning);
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

	cLogger::GetInstance().LogSimple(GetLogMessage(tolua_S).c_str(), cLogger::llError);
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
	cPluginLua * Plugin = reinterpret_cast<cPluginLua *>(const_cast<void*>(lua_topointer(L, -1)));
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
	ASSERT(L.GetStackValues(2, FileName, NewExt));
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
	ASSERT(L.GetStackValues(2, SrcFile, DstFile));
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
	ASSERT(L.GetStackValues(2, FolderPath));
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
	ASSERT(L.GetStackValues(2, FolderPath));
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
	ASSERT(L.GetStackValues(2, Path));
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
	ASSERT(L.GetStackValues(2, Path));
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
	ASSERT(L.GetStackValues(2, Path));
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
	ASSERT(L.GetStackValues(2, Path));
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
	ASSERT(L.GetStackValues(2, Path));
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
	ASSERT(LuaState.GetStackValues(2, Folder));

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
	ASSERT(L.GetStackValues(2, Path));
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
	ASSERT(L.GetStackValues(2, Path));
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
	ASSERT(L.GetStackValues(2, Path));
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
	ASSERT(L.GetStackValues(2, Path));
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
	ASSERT(LuaState.GetStackValues(2, FileName));

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
	ASSERT(L.GetStackValues(2, SrcPath, DstPath));
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





static int tolua_cPluginManager_AddHook_FnRef(cPluginManager & a_PluginManager, cLuaState & S, int a_ParamIdx)
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
	int HookType = static_cast<int>(tolua_tonumber(S, a_ParamIdx, -1));
	if (!a_PluginManager.IsValidHookType(HookType))
	{
		LOGWARNING("cPluginManager.AddHook(): Invalid HOOK_TYPE parameter: %d", HookType);
		S.LogStackTrace();
		return 0;
	}

	// Add the hook to the plugin
	if (!Plugin->AddHookRef(HookType, a_ParamIdx + 1))
	{
		LOGWARNING("cPluginManager.AddHook(): Cannot add hook %d, unknown error.", HookType);
		S.LogStackTrace();
		return 0;
	}
	a_PluginManager.AddHook(Plugin, HookType);

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
	auto & PlgMgr = cPluginManager::Get();

	// If the first param is a cPluginManager instance, use it instead of the global one:
	int ParamIdx = 1;
	tolua_Error err;
	if (tolua_isusertable(S, 1, "cPluginManager", 0, &err))
	{
		// Style 1, use the global PlgMgr, but increment ParamIdx
		ParamIdx++;
	}

	if (lua_isnumber(S, ParamIdx) && lua_isfunction(S, ParamIdx + 1))
	{
		// The next params are a number and a function, assume style 1 or 2
		return tolua_cPluginManager_AddHook_FnRef(PlgMgr, S, ParamIdx);
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
	L.Push(cPluginManager::Get().ForEachCommand(Callback));
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
	L.Push(cPluginManager::Get().ForEachConsoleCommand(Callback));
	return 1;
}





static int tolua_cPluginManager_BindCommand(lua_State * L)
{
	/* Function signatures:
		cPluginManager:BindCommand(Command, Permission, Function, HelpString)
		cPluginManager.BindCommand(Command, Permission, Function, HelpString)  -- without the "self" param
	*/
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
	auto & self = cPluginManager::Get();
	AString Command   (tolua_tocppstring(L, idx,     ""));
	AString Permission(tolua_tocppstring(L, idx + 1, ""));
	AString HelpString(tolua_tocppstring(L, idx + 3, ""));

	// Store the function reference:
	lua_pop(L, 1);  // Pop the help string off the stack
	int FnRef = luaL_ref(L, LUA_REGISTRYINDEX);  // Store function reference
	if (FnRef == LUA_REFNIL)
	{
		LOGERROR("\"BindCommand\": Cannot create a function reference. Command \"%s\" not bound.", Command.c_str());
		return 0;
	}

	if (!self.BindCommand(Command, Plugin, Permission, HelpString))
	{
		// Refused. Possibly already bound. Error message has been given, display the callstack:
		cLuaState LS(L);
		LS.LogStackTrace();
		return 0;
	}

	Plugin->BindCommand(Command, FnRef);
	lua_pushboolean(L, true);
	return 1;
}





static int tolua_cPluginManager_BindConsoleCommand(lua_State * L)
{
	/* Function signatures:
		cPluginManager:BindConsoleCommand(Command, Function, HelpString)
		cPluginManager.BindConsoleCommand(Command, Function, HelpString)  -- without the "self" param
	*/

	// Get the plugin identification out of LuaState:
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
	auto & self = cPluginManager::Get();
	AString Command   (tolua_tocppstring(L, idx,     ""));
	AString HelpString(tolua_tocppstring(L, idx + 2, ""));

	// Store the function reference:
	lua_pop(L, 1);  // Pop the help string off the stack
	int FnRef = luaL_ref(L, LUA_REGISTRYINDEX);  // Store function reference
	if (FnRef == LUA_REFNIL)
	{
		LOGERROR("\"BindConsoleCommand\": Cannot create a function reference. Console Command \"%s\" not bound.", Command.c_str());
		return 0;
	}

	if (!self.BindConsoleCommand(Command, Plugin, HelpString))
	{
		// Refused. Possibly already bound. Error message has been given, display the callstack:
		cLuaState LS(L);
		LS.LogStackTrace();
		return 0;
	}

	Plugin->BindConsoleCommand(Command, FnRef);
	lua_pushboolean(L, true);
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
	class cCallback :
		public cPluginManager::cPluginCallback
	{
	public:
		int m_NumReturns;

		cCallback(const AString & a_FunctionName, cLuaState & a_SrcLuaState) :
			m_NumReturns(0),
			m_FunctionName(a_FunctionName),
			m_SrcLuaState(a_SrcLuaState)
		{
		}
	protected:
		const AString & m_FunctionName;
		cLuaState & m_SrcLuaState;

		virtual bool Item(cPlugin * a_Plugin) override
		{
			if (!a_Plugin->IsLoaded())
			{
				return false;
			}
			m_NumReturns = static_cast<cPluginLua *>(a_Plugin)->CallFunctionFromForeignState(
				m_FunctionName, m_SrcLuaState, 4, lua_gettop(m_SrcLuaState)
			);
			return true;
		}
	} Callback(FunctionName, L);
	if (!cPluginManager::Get().DoWithPlugin(PluginName, Callback))
	{
		return 0;
	}
	if (Callback.m_NumReturns < 0)
	{
		// The call has failed, there are zero return values. Do NOT return negative number (Lua considers that a "yield")
		return 0;
	}
	return Callback.m_NumReturns;
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
	L.Push(cPluginManager::Get().ExecuteConsoleCommand(Split, CommandOutput, Command));
	L.Push(CommandOutput.GetAccum());
	return 2;
}





static int tolua_cPluginManager_FindPlugins(lua_State * tolua_S)
{
	// API function no longer exists:
	LOGWARNING("cPluginManager:FindPlugins() is obsolete, use cPluginManager:RefreshPluginList() instead!");
	cLuaState::LogStackTrace(tolua_S);

	// Still, do the actual work performed by the API function when it existed:
	cPluginManager::Get().RefreshPluginList();
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
	cPlayer * self = reinterpret_cast<cPlayer *>(tolua_tousertype(tolua_S, 1, nullptr));
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
	cPlayer * self = reinterpret_cast<cPlayer *>(tolua_tousertype(tolua_S, 1, nullptr));
	if (self == nullptr)
	{
		LOGWARNING("%s: invalid self (%p)", __FUNCTION__, static_cast<void *>(self));
		return 0;
	}

	// Push the permissions:
	L.Push(self->GetRestrictions());
	return 1;
}





static int tolua_cPlayer_OpenWindow(lua_State * tolua_S)
{
	// Function signature: cPlayer:OpenWindow(Window)

	// Retrieve the plugin instance from the Lua state
	cPluginLua * Plugin = cManualBindings::GetLuaPlugin(tolua_S);
	if (Plugin == nullptr)
	{
		return 0;
	}

	// Get the parameters:
	cPlayer * self = reinterpret_cast<cPlayer *>(tolua_tousertype(tolua_S, 1, nullptr));
	cWindow * wnd  = reinterpret_cast<cWindow *>(tolua_tousertype(tolua_S, 2, nullptr));
	if ((self == nullptr) || (wnd == nullptr))
	{
		LOGWARNING("%s: invalid self (%p) or wnd (%p)", __FUNCTION__, static_cast<void *>(self), static_cast<void *>(wnd));
		return 0;
	}

	// If cLuaWindow, add a reference, so that Lua won't delete the cLuaWindow object mid-processing
	tolua_Error err;
	if (tolua_isusertype(tolua_S, 2, "cLuaWindow", 0, &err))
	{
		cLuaWindow * LuaWnd = reinterpret_cast<cLuaWindow *>(wnd);
		// Only if not already referenced
		if (!LuaWnd->IsLuaReferenced())
		{
			int LuaRef = luaL_ref(tolua_S, LUA_REGISTRYINDEX);
			if (LuaRef == LUA_REFNIL)
			{
				LOGWARNING("%s: Cannot create a window reference. Cannot open window \"%s\".",
					__FUNCTION__, wnd->GetWindowTitle().c_str()
				);
				return 0;
			}
			LuaWnd->SetLuaRef(Plugin, LuaRef);
		}
	}

	// Open the window
	self->OpenWindow(wnd);
	return 0;
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





template <
	class OBJTYPE,
	void (OBJTYPE::*SetCallback)(cPluginLua * a_Plugin, int a_FnRef)
>
static int tolua_SetObjectCallback(lua_State * tolua_S)
{
	// Function signature: OBJTYPE:SetWhateverCallback(CallbackFunction)

	// Retrieve the plugin instance from the Lua state
	cPluginLua * Plugin = cManualBindings::GetLuaPlugin(tolua_S);
	if (Plugin == nullptr)
	{
		// Warning message has already been printed by GetLuaPlugin(), bail out silently
		return 0;
	}

	// Get the parameters - self and the function reference:
	OBJTYPE * self = reinterpret_cast<OBJTYPE *>(tolua_tousertype(tolua_S, 1, nullptr));
	if (self == nullptr)
	{
		LOGWARNING("%s: invalid self (%p)", __FUNCTION__, static_cast<void *>(self));
		return 0;
	}
	int FnRef = luaL_ref(tolua_S, LUA_REGISTRYINDEX);  // Store function reference for later retrieval
	if (FnRef == LUA_REFNIL)
	{
		LOGERROR("%s: Cannot create a function reference. Callback not set.", __FUNCTION__);
		return 0;
	}

	// Set the callback
	(self->*SetCallback)(Plugin, FnRef);
	return 0;
}





static int tolua_cPluginLua_AddWebTab(lua_State * tolua_S)
{
	cLuaState LuaState(tolua_S);
	cPluginLua * self = nullptr;

	if (!LuaState.GetStackValue(1, self))
	{
		LOGWARNING("cPluginLua:AddWebTab: invalid self as first argument");
		return 0;
	}

	tolua_Error tolua_err;
	tolua_err.array = 0;
	tolua_err.index = 3;
	tolua_err.type = "function";

	std::string Title;
	int Reference = LUA_REFNIL;

	if (LuaState.CheckParamString(2) && LuaState.CheckParamFunction(3))
	{
		Reference = luaL_ref(tolua_S, LUA_REGISTRYINDEX);
		LuaState.GetStackValue(2, Title);
	}
	else
	{
		return cManualBindings::tolua_do_error(tolua_S, "#ferror calling function '#funcname#'", &tolua_err);
	}

	if (Reference != LUA_REFNIL)
	{
		if (!self->AddWebTab(Title.c_str(), tolua_S, Reference))
		{
			luaL_unref(tolua_S, LUA_REGISTRYINDEX, Reference);
		}
	}
	else
	{
		LOGWARNING("cPluginLua:AddWebTab: invalid function reference in 2nd argument (Title: \"%s\")", Title.c_str());
	}

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
	md5(SourceString, len, Output);
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
	md5(SourceString, len, md5Output);

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
	sha1(SourceString, len, Output);
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
	sha1(SourceString, len, sha1Output);

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





static int tolua_push_StringStringMap(lua_State* tolua_S, std::map< std::string, std::string >& a_StringStringMap)
{
	lua_newtable(tolua_S);
	int top = lua_gettop(tolua_S);

	for (std::map<std::string, std::string>::iterator it = a_StringStringMap.begin(); it != a_StringStringMap.end(); ++it)
	{
		const char * key = it->first.c_str();
		const char * value = it->second.c_str();
		lua_pushstring(tolua_S, key);
		lua_pushstring(tolua_S, value);
		lua_settable(tolua_S, top);
	}

	return 1;
}





static int tolua_get_HTTPRequest_Params(lua_State* tolua_S)
{
	HTTPRequest * self = reinterpret_cast<HTTPRequest *>(tolua_tousertype(tolua_S, 1, nullptr));
	return tolua_push_StringStringMap(tolua_S, self->Params);
}





static int tolua_get_HTTPRequest_PostParams(lua_State* tolua_S)
{
	HTTPRequest * self = reinterpret_cast<HTTPRequest *>(tolua_tousertype(tolua_S, 1, nullptr));
	return tolua_push_StringStringMap(tolua_S, self->PostParams);
}





static int tolua_get_HTTPRequest_FormData(lua_State* tolua_S)
{
	HTTPRequest * self = reinterpret_cast<HTTPRequest *>(tolua_tousertype(tolua_S, 1, nullptr));
	std::map<std::string, HTTPFormData> & FormData = self->FormData;

	lua_newtable(tolua_S);
	int top = lua_gettop(tolua_S);

	for (std::map<std::string, HTTPFormData>::iterator it = FormData.begin(); it != FormData.end(); ++it)
	{
		lua_pushstring(tolua_S, it->first.c_str());
		tolua_pushusertype(tolua_S, &(it->second), "HTTPFormData");
		// lua_pushlstring(tolua_S, it->second.Value.c_str(), it->second.Value.size());  // Might contain binary data
		lua_settable(tolua_S, top);
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
		L.PushNil();
		L.Push(res.second);
		return 2;
	}
	L.Push(scheme);
	L.Push(username);
	L.Push(password);
	L.Push(host);
	L.Push(port);
	L.Push(path);
	L.Push(query);
	L.Push(fragment);
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
		L.PushNil();
		L.Push(res.second);
		return 2;
	}
	L.Push(username);
	L.Push(password);
	L.Push(host);
	L.Push(port);
	return 4;
}





static int tolua_cWebAdmin_GetPlugins(lua_State * tolua_S)
{
	cWebAdmin * self = reinterpret_cast<cWebAdmin *>(tolua_tousertype(tolua_S, 1, nullptr));

	const cWebAdmin::PluginList & AllPlugins = self->GetPlugins();

	lua_createtable(tolua_S, static_cast<int>(AllPlugins.size()), 0);
	int newTable = lua_gettop(tolua_S);
	int index = 1;
	cWebAdmin::PluginList::const_iterator iter = AllPlugins.begin();
	while (iter != AllPlugins.end())
	{
		const cWebPlugin * Plugin = *iter;
		tolua_pushusertype(tolua_S, reinterpret_cast<void *>(const_cast<cWebPlugin*>(Plugin)), "const cWebPlugin");
		lua_rawseti(tolua_S, newTable, index);
		++iter;
		++index;
	}
	return 1;
}





/** Binding for cWebAdmin::GetHTMLEscapedString.
Manual code required because ToLua generates an extra return value */
static int tolua_AllToLua_cWebAdmin_GetHTMLEscapedString(lua_State * tolua_S)
{
	// Check the param types:
	cLuaState S(tolua_S);
	if (
		!S.CheckParamUserTable(1, "cWebAdmin") ||
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
	S.Push(cWebAdmin::GetHTMLEscapedString(Input));
	return 1;
}





/** Binding for cWebAdmin::GetURLEncodedString.
Manual code required because ToLua generates an extra return value */
static int tolua_AllToLua_cWebAdmin_GetURLEncodedString(lua_State * tolua_S)
{
	// Check the param types:
	cLuaState S(tolua_S);
	if (
		!S.CheckParamUserTable(1, "cWebAdmin") ||
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
	S.Push(cWebAdmin::GetURLEncodedString(Input));
	return 1;
}





static int tolua_cWebPlugin_GetTabNames(lua_State * tolua_S)
{
	// Returns a map of (SafeTitle -> Title) for the plugin's web tabs.
	auto self = reinterpret_cast<cWebPlugin *>(tolua_tousertype(tolua_S, 1, nullptr));
	auto TabNames = self->GetTabNames();
	lua_newtable(tolua_S);
	int index = 1;
	for (auto itr = TabNames.cbegin(), end = TabNames.cend(); itr != end; ++itr)
	{
		tolua_pushstring(tolua_S, itr->second.c_str());  // Because the SafeTitle is supposed to be unique, use it as key
		tolua_pushstring(tolua_S, itr->first.c_str());
		lua_rawset(tolua_S, -3);
		++index;
	}
	return 1;
}





static int tolua_cClientHandle_SendPluginMessage(lua_State * L)
{
	cLuaState S(L);
	if (
		!S.CheckParamUserType(1, "cClientHandle") ||
		!S.CheckParamString(2, 3) ||
		!S.CheckParamEnd(4)
	)
	{
		return 0;
	}
	cClientHandle * Client = reinterpret_cast<cClientHandle *>(tolua_tousertype(L, 1, nullptr));
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





static int tolua_cMojangAPI_AddPlayerNameToUUIDMapping(lua_State * L)
{
	cLuaState S(L);
	if (
		!S.CheckParamUserTable(1, "cMojangAPI") ||
		!S.CheckParamString(2) ||
		!S.CheckParamString(3) ||
		!S.CheckParamEnd(4)
	)
	{
		return 0;
	}

	// Retrieve the parameters:
	AString UUID, PlayerName;
	S.GetStackValue(2, PlayerName);
	S.GetStackValue(3, UUID);

	// Store in the cache:
	cRoot::Get()->GetMojangAPI().AddPlayerNameToUUIDMapping(PlayerName, UUID);
	return 0;
}





static int tolua_cMojangAPI_GetPlayerNameFromUUID(lua_State * L)
{
	cLuaState S(L);
	if (
		!S.CheckParamUserTable(1, "cMojangAPI") ||
		!S.CheckParamString(2) ||
		!S.CheckParamEnd(4)
	)
	{
		return 0;
	}

	AString UUID;
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
		!S.CheckParamUserTable(1, "cMojangAPI") ||
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

	// Return the UUID:
	AString UUID = cRoot::Get()->GetMojangAPI().GetUUIDFromPlayerName(PlayerName, ShouldUseCacheOnly);
	S.Push(UUID);
	return 1;
}





static int tolua_cMojangAPI_GetUUIDsFromPlayerNames(lua_State * L)
{
	cLuaState S(L);
	if (
		!S.CheckParamUserTable(1, "cMojangAPI") ||
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
	AStringVector UUIDs = cRoot::Get()->GetMojangAPI().GetUUIDsFromPlayerNames(PlayerNames, ShouldUseCacheOnly);
	if (UUIDs.size() != PlayerNames.size())
	{
		// A hard error has occured while processing the request, no UUIDs were returned. Return an empty table:
		return 1;
	}

	// Convert to output table, PlayerName -> UUID:
	size_t len = UUIDs.size();
	for (size_t i = 0; i < len; i++)
	{
		if (UUIDs[i].empty())
		{
			// No UUID was provided for PlayerName[i], skip it in the resulting table
			continue;
		}
		lua_pushlstring(L, UUIDs[i].c_str(), UUIDs[i].length());
		lua_setfield(L, 3, PlayerNames[i].c_str());
	}
	return 1;
}





static int tolua_cMojangAPI_MakeUUIDDashed(lua_State * L)
{
	// Function signature: cMojangAPI:MakeUUIDDashed(UUID) -> string

	// Check params:
	cLuaState S(L);
	if (
		!S.CheckParamUserTable(1, "cMojangAPI") ||
		!S.CheckParamString(2) ||
		!S.CheckParamEnd(3)
	)
	{
		return 0;
	}

	// Get the params:
	AString UUID;
	S.GetStackValue(2, UUID);

	// Push the result:
	S.Push(cRoot::Get()->GetMojangAPI().MakeUUIDDashed(UUID));
	return 1;
}





static int tolua_cMojangAPI_MakeUUIDShort(lua_State * L)
{
	// Function signature: cMojangAPI:MakeUUIDShort(UUID) -> string

	// Check params:
	cLuaState S(L);
	if (
		!S.CheckParamUserTable(1, "cMojangAPI") ||
		!S.CheckParamString(2) ||
		!S.CheckParamEnd(3)
	)
	{
		return 0;
	}

	// Get the params:
	AString UUID;
	S.GetStackValue(2, UUID);

	// Push the result:
	S.Push(cRoot::Get()->GetMojangAPI().MakeUUIDShort(UUID));
	return 1;
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
		const cItemGrid * self = reinterpret_cast<const cItemGrid *>(tolua_tousertype(L, 1, nullptr));
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
	cLuaBlockTracerCallbacks(cLuaState & a_LuaState, int a_ParamNum) :
		m_LuaState(a_LuaState),
		m_TableRef(a_LuaState, a_ParamNum)
	{
	}

	virtual bool OnNextBlock(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, char a_EntryFace) override
	{
		bool res = false;
		if (!m_LuaState.Call(
			cLuaState::cTableRef(m_TableRef, "OnNextBlock"),
			a_BlockX, a_BlockY, a_BlockZ, a_BlockType, a_BlockMeta, a_EntryFace,
			cLuaState::Return, res
		))
		{
			// No such function in the table, skip the callback
			return false;
		}
		return res;
	}

	virtual bool OnNextBlockNoData(int a_BlockX, int a_BlockY, int a_BlockZ, char a_EntryFace) override
	{
		bool res = false;
		if (!m_LuaState.Call(
			cLuaState::cTableRef(m_TableRef, "OnNextBlockNoData"),
			a_BlockX, a_BlockY, a_BlockZ, a_EntryFace,
			cLuaState::Return, res
		))
		{
			// No such function in the table, skip the callback
			return false;
		}
		return res;
	}

	virtual bool OnOutOfWorld(double a_BlockX, double a_BlockY, double a_BlockZ) override
	{
		bool res = false;
		if (!m_LuaState.Call(
			cLuaState::cTableRef(m_TableRef, "OnOutOfWorld"),
			a_BlockX, a_BlockY, a_BlockZ,
			cLuaState::Return, res
		))
		{
			// No such function in the table, skip the callback
			return false;
		}
		return res;
	}

	virtual bool OnIntoWorld(double a_BlockX, double a_BlockY, double a_BlockZ) override
	{
		bool res = false;
		if (!m_LuaState.Call(
			cLuaState::cTableRef(m_TableRef, "OnIntoWorld"),
			a_BlockX, a_BlockY, a_BlockZ,
			cLuaState::Return, res
		))
		{
			// No such function in the table, skip the callback
			return false;
		}
		return res;
	}

	virtual void OnNoMoreHits(void) override
	{
		m_LuaState.Call(cLuaState::cTableRef(m_TableRef, "OnNoMoreHits"));
	}

	virtual void OnNoChunk(void) override
	{
		m_LuaState.Call(cLuaState::cTableRef(m_TableRef, "OnNoChunk"));
	}

protected:
	cLuaState & m_LuaState;
	cLuaState::cRef m_TableRef;
} ;





static int tolua_cLineBlockTracer_Trace(lua_State * tolua_S)
{
	/* Supported function signatures:
	cLineBlockTracer:Trace(World, Callbacks, StartX, StartY, StartZ, EndX, EndY, EndZ)  // Canonical
	cLineBlockTracer.Trace(World, Callbacks, StartX, StartY, StartZ, EndX, EndY, EndZ)
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
		!L.CheckParamTable   (idx + 1) ||
		!L.CheckParamNumber  (idx + 2, idx + 7) ||
		!L.CheckParamEnd     (idx + 8)
	)
	{
		return 0;
	}

	// Trace:
	cWorld * World = reinterpret_cast<cWorld *>(tolua_tousertype(L, idx, nullptr));
	cLuaBlockTracerCallbacks Callbacks(L, idx + 1);
	double StartX = tolua_tonumber(L, idx + 2, 0);
	double StartY = tolua_tonumber(L, idx + 3, 0);
	double StartZ = tolua_tonumber(L, idx + 4, 0);
	double EndX   = tolua_tonumber(L, idx + 5, 0);
	double EndY   = tolua_tonumber(L, idx + 6, 0);
	double EndZ   = tolua_tonumber(L, idx + 7, 0);
	bool res = cLineBlockTracer::Trace(*World, Callbacks, StartX, StartY, StartZ, EndX, EndY, EndZ);
	tolua_pushboolean(L, res ? 1 : 0);
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
	auto & BR = cRoot::Get()->GetBrewingRecipes();
	const cBrewingRecipes::cRecipe * Recipe = BR.GetRecipeFrom(*Bottle, *Ingredient);
	if (Recipe == nullptr)
	{
		// There is no such brewing recipe for this bottle and ingredient, return no value
		return 0;
	}

	// Push the output item
	L.Push(Recipe->Output.get());
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
	auto & FR = cRoot::Get()->GetFurnaceRecipe();
	const auto Recipe = FR.GetRecipeFrom(*Input);
	if (Recipe == nullptr)
	{
		// There is no such furnace recipe for this input, return no value
		return 0;
	}

	// Push the output, number of ticks and input as the three return values:
	L.Push(Recipe->Out);
	L.Push(Recipe->CookTime);
	L.Push(Recipe->In);
	return 3;
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
	cHopperEntity * self = reinterpret_cast<cHopperEntity *>(tolua_tousertype(tolua_S, 1, nullptr));
	if (self == nullptr)
	{
		tolua_error(tolua_S, "invalid 'self' in function 'cHopperEntity::GetOutputBlockPos()'", nullptr);
		return 0;
	}

	NIBBLETYPE a_BlockMeta = static_cast<NIBBLETYPE>(tolua_tonumber(tolua_S, 2, 0));
	int a_OutputX, a_OutputY, a_OutputZ;
	bool res = self->GetOutputBlockPos(a_BlockMeta, a_OutputX, a_OutputY, a_OutputZ);
	tolua_pushboolean(tolua_S, res);
	if (res)
	{
		tolua_pushnumber(tolua_S, static_cast<lua_Number>(a_OutputX));
		tolua_pushnumber(tolua_S, static_cast<lua_Number>(a_OutputY));
		tolua_pushnumber(tolua_S, static_cast<lua_Number>(a_OutputZ));
		return 4;
	}
	return 1;
}






static int tolua_cBlockArea_GetBlockTypeMeta(lua_State * tolua_S)
{
	// function cBlockArea::GetBlockTypeMeta()
	// Exported manually because tolua generates extra input params for the outputs

	cLuaState L(tolua_S);
	if (
		!L.CheckParamUserType(1, "cBlockArea") ||
		!L.CheckParamNumber  (2, 4)
	)
	{
		return 0;
	}

	cBlockArea * self = reinterpret_cast<cBlockArea *>(tolua_tousertype(tolua_S, 1, nullptr));
	if (self == nullptr)
	{
		tolua_error(tolua_S, "invalid 'self' in function 'cBlockArea:GetRelBlockTypeMeta'", nullptr);
		return 0;
	}
	int BlockX = static_cast<int>(tolua_tonumber(tolua_S, 2, 0));
	int BlockY = static_cast<int>(tolua_tonumber(tolua_S, 3, 0));
	int BlockZ = static_cast<int>(tolua_tonumber(tolua_S, 4, 0));
	BLOCKTYPE BlockType;
	NIBBLETYPE BlockMeta;
	self->GetBlockTypeMeta(BlockX, BlockY, BlockZ, BlockType, BlockMeta);
	tolua_pushnumber(tolua_S, BlockType);
	tolua_pushnumber(tolua_S, BlockMeta);
	return 2;
}





static int tolua_cBlockArea_GetOrigin(lua_State * tolua_S)
{
	// function cBlockArea::GetOrigin()
	// Returns all three coords of the origin point
	// Exported manually because there's no direct C++ equivalent,
	// plus tolua would generate extra input params for the outputs

	cLuaState L(tolua_S);
	if (!L.CheckParamUserType(1, "cBlockArea"))
	{
		return 0;
	}

	cBlockArea * self = reinterpret_cast<cBlockArea *>(tolua_tousertype(tolua_S, 1, nullptr));
	if (self == nullptr)
	{
		tolua_error(tolua_S, "invalid 'self' in function 'cBlockArea:GetOrigin'", nullptr);
		return 0;
	}

	// Push the three origin coords:
	lua_pushnumber(tolua_S, self->GetOriginX());
	lua_pushnumber(tolua_S, self->GetOriginY());
	lua_pushnumber(tolua_S, self->GetOriginZ());
	return 3;
}





static int tolua_cBlockArea_GetNonAirCropRelCoords(lua_State * tolua_S)
{
	// function cBlockArea::GetNonAirCropRelCoords()
	// Exported manually because tolua would generate extra input params for the outputs

	cLuaState L(tolua_S);
	if (!L.CheckParamUserType(1, "cBlockArea"))
	{
		return 0;
	}

	cBlockArea * self = nullptr;
	BLOCKTYPE IgnoreBlockType = E_BLOCK_AIR;
	L.GetStackValues(1, self, IgnoreBlockType);
	if (self == nullptr)
	{
		tolua_error(tolua_S, "invalid 'self' in function 'cBlockArea:GetNonAirCropRelCoords'", nullptr);
		return 0;
	}

	// Calculate the crop coords:
	int MinRelX, MinRelY, MinRelZ, MaxRelX, MaxRelY, MaxRelZ;
	self->GetNonAirCropRelCoords(MinRelX, MinRelY, MinRelZ, MaxRelX, MaxRelY, MaxRelZ, IgnoreBlockType);

	// Push the six crop coords:
	L.Push(MinRelX);
	L.Push(MinRelY);
	L.Push(MinRelZ);
	L.Push(MaxRelX);
	L.Push(MaxRelY);
	L.Push(MaxRelZ);
	return 6;
}





static int tolua_cBlockArea_GetRelBlockTypeMeta(lua_State * tolua_S)
{
	// function cBlockArea::GetRelBlockTypeMeta()
	// Exported manually because tolua generates extra input params for the outputs

	cLuaState L(tolua_S);
	if (
		!L.CheckParamUserType(1, "cBlockArea") ||
		!L.CheckParamNumber  (2, 4)
	)
	{
		return 0;
	}

	cBlockArea * self = reinterpret_cast<cBlockArea *>(tolua_tousertype(tolua_S, 1, nullptr));
	if (self == nullptr)
	{
		tolua_error(tolua_S, "invalid 'self' in function 'cBlockArea:GetRelBlockTypeMeta'", nullptr);
		return 0;
	}
	int BlockX = static_cast<int>(tolua_tonumber(tolua_S, 2, 0));
	int BlockY = static_cast<int>(tolua_tonumber(tolua_S, 3, 0));
	int BlockZ = static_cast<int>(tolua_tonumber(tolua_S, 4, 0));
	BLOCKTYPE BlockType;
	NIBBLETYPE BlockMeta;
	self->GetRelBlockTypeMeta(BlockX, BlockY, BlockZ, BlockType, BlockMeta);
	tolua_pushnumber(tolua_S, BlockType);
	tolua_pushnumber(tolua_S, BlockMeta);
	return 2;
}





static int tolua_cBlockArea_GetSize(lua_State * tolua_S)
{
	// function cBlockArea::GetSize()
	// Returns all three sizes of the area
	// Exported manually because there's no direct C++ equivalent,
	// plus tolua would generate extra input params for the outputs

	cLuaState L(tolua_S);
	if (!L.CheckParamUserType(1, "cBlockArea"))
	{
		return 0;
	}

	cBlockArea * self = reinterpret_cast<cBlockArea *>(tolua_tousertype(tolua_S, 1, nullptr));
	if (self == nullptr)
	{
		tolua_error(tolua_S, "invalid 'self' in function 'cBlockArea:GetSize'", nullptr);
		return 0;
	}

	// Push the three origin coords:
	lua_pushnumber(tolua_S, self->GetSizeX());
	lua_pushnumber(tolua_S, self->GetSizeY());
	lua_pushnumber(tolua_S, self->GetSizeZ());
	return 3;
}





static int tolua_cBlockArea_GetCoordRange(lua_State * tolua_S)
{
	// function cBlockArea::GetCoordRange()
	// Returns all three sizes of the area, miuns one, so that they represent the maximum coord value
	// Exported manually because there's no direct C++ equivalent,
	// plus tolua would generate extra input params for the outputs

	cLuaState L(tolua_S);
	if (!L.CheckParamUserType(1, "cBlockArea"))
	{
		return 0;
	}

	cBlockArea * self = reinterpret_cast<cBlockArea *>(tolua_tousertype(tolua_S, 1, nullptr));
	if (self == nullptr)
	{
		tolua_error(tolua_S, "invalid 'self' in function 'cBlockArea:GetSize'", nullptr);
		return 0;
	}

	// Push the three origin coords:
	lua_pushnumber(tolua_S, self->GetSizeX() - 1);
	lua_pushnumber(tolua_S, self->GetSizeY() - 1);
	lua_pushnumber(tolua_S, self->GetSizeZ() - 1);
	return 3;
}





static int tolua_cBlockArea_LoadFromSchematicFile(lua_State * tolua_S)
{
	// function cBlockArea::LoadFromSchematicFile
	// Exported manually because function has been moved to SchematicFileSerializer.cpp
	cLuaState L(tolua_S);
	if (
		!L.CheckParamUserType(1, "cBlockArea") ||
		!L.CheckParamString  (2) ||
		!L.CheckParamEnd     (3)
	)
	{
		return 0;
	}
	cBlockArea * self = reinterpret_cast<cBlockArea *>(tolua_tousertype(tolua_S, 1, nullptr));
	if (self == nullptr)
	{
		tolua_error(tolua_S, "invalid 'self' in function 'cBlockArea::LoadFromSchematicFile'", nullptr);
		return 0;
	}

	AString Filename = tolua_tostring(tolua_S, 2, 0);
	bool res = cSchematicFileSerializer::LoadFromSchematicFile(*self, Filename);
	tolua_pushboolean(tolua_S, res);
	return 1;
}





static int tolua_cBlockArea_LoadFromSchematicString(lua_State * tolua_S)
{
	// function cBlockArea::LoadFromSchematicString
	// Exported manually because function has been moved to SchematicFileSerializer.cpp
	cLuaState L(tolua_S);
	if (
		!L.CheckParamUserType(1, "cBlockArea") ||
		!L.CheckParamString  (2) ||
		!L.CheckParamEnd     (3)
	)
	{
		return 0;
	}
	cBlockArea * self = reinterpret_cast<cBlockArea *>(tolua_tousertype(tolua_S, 1, nullptr));
	if (self == nullptr)
	{
		tolua_error(tolua_S, "invalid 'self' in function 'cBlockArea::LoadFromSchematicFile'", nullptr);
		return 0;
	}

	AString Data;
	L.GetStackValue(2, Data);
	bool res = cSchematicFileSerializer::LoadFromSchematicString(*self, Data);
	tolua_pushboolean(tolua_S, res);
	return 1;
}





static int tolua_cBlockArea_SaveToSchematicFile(lua_State * tolua_S)
{
	// function cBlockArea::SaveToSchematicFile
	// Exported manually because function has been moved to SchematicFileSerializer.cpp
	cLuaState L(tolua_S);
	if (
		!L.CheckParamUserType(1, "cBlockArea") ||
		!L.CheckParamString  (2) ||
		!L.CheckParamEnd     (3)
	)
	{
		return 0;
	}
	cBlockArea * self = reinterpret_cast<cBlockArea *>(tolua_tousertype(tolua_S, 1, nullptr));
	if (self == nullptr)
	{
		tolua_error(tolua_S, "invalid 'self' in function 'cBlockArea::SaveToSchematicFile'", nullptr);
		return 0;
	}
	AString Filename = tolua_tostring(tolua_S, 2, 0);
	bool res = cSchematicFileSerializer::SaveToSchematicFile(*self, Filename);
	tolua_pushboolean(tolua_S, res);
	return 1;
}





static int tolua_cBlockArea_SaveToSchematicString(lua_State * tolua_S)
{
	// function cBlockArea::SaveToSchematicString
	// Exported manually because function has been moved to SchematicFileSerializer.cpp
	cLuaState L(tolua_S);
	if (
		!L.CheckParamUserType(1, "cBlockArea") ||
		!L.CheckParamEnd     (2)
	)
	{
		return 0;
	}
	cBlockArea * self = reinterpret_cast<cBlockArea *>(tolua_tousertype(tolua_S, 1, nullptr));
	if (self == nullptr)
	{
		tolua_error(tolua_S, "invalid 'self' in function 'cBlockArea::SaveToSchematicFile'", nullptr);
		return 0;
	}

	AString Data;
	if (cSchematicFileSerializer::SaveToSchematicString(*self, Data))
	{
		L.Push(Data);
		return 1;
	}
	return 0;
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
	cCompositeChat * self = reinterpret_cast<cCompositeChat *>(tolua_tousertype(tolua_S, 1, nullptr));
	if (self == nullptr)
	{
		tolua_error(tolua_S, "invalid 'self' in function 'cCompositeChat:AddRunCommandPart'", nullptr);
		return 0;
	}

	// Add the part:
	AString Text, Command, Style;
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
	cCompositeChat * self = reinterpret_cast<cCompositeChat *>(tolua_tousertype(tolua_S, 1, nullptr));
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
	cCompositeChat * self = reinterpret_cast<cCompositeChat *>(tolua_tousertype(tolua_S, 1, nullptr));
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
	cCompositeChat * self = reinterpret_cast<cCompositeChat *>(tolua_tousertype(tolua_S, 1, nullptr));
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
	cCompositeChat * self = reinterpret_cast<cCompositeChat *>(tolua_tousertype(tolua_S, 1, nullptr));
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
	cCompositeChat * self = reinterpret_cast<cCompositeChat *>(tolua_tousertype(tolua_S, 1, nullptr));
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
	// Exported manually to support call-chaining (return *this)

	// Check params:
	cLuaState L(tolua_S);
	if (!L.CheckParamUserType(1, "cCompositeChat"))
	{
		return 0;
	}
	cCompositeChat * self = reinterpret_cast<cCompositeChat *>(tolua_tousertype(tolua_S, 1, nullptr));
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

		tolua_beginmodule(tolua_S, "cBlockArea");
			tolua_function(tolua_S, "GetBlockTypeMeta",        tolua_cBlockArea_GetBlockTypeMeta);
			tolua_function(tolua_S, "GetCoordRange",           tolua_cBlockArea_GetCoordRange);
			tolua_function(tolua_S, "GetOrigin",               tolua_cBlockArea_GetOrigin);
			tolua_function(tolua_S, "GetNonAirCropRelCoords",  tolua_cBlockArea_GetNonAirCropRelCoords);
			tolua_function(tolua_S, "GetRelBlockTypeMeta",     tolua_cBlockArea_GetRelBlockTypeMeta);
			tolua_function(tolua_S, "GetSize",                 tolua_cBlockArea_GetSize);
			tolua_function(tolua_S, "LoadFromSchematicFile",   tolua_cBlockArea_LoadFromSchematicFile);
			tolua_function(tolua_S, "LoadFromSchematicString", tolua_cBlockArea_LoadFromSchematicString);
			tolua_function(tolua_S, "SaveToSchematicFile",     tolua_cBlockArea_SaveToSchematicFile);
			tolua_function(tolua_S, "SaveToSchematicString",   tolua_cBlockArea_SaveToSchematicString);
		tolua_endmodule(tolua_S);

		tolua_beginmodule(tolua_S, "cClientHandle");
			tolua_constant(tolua_S, "MAX_VIEW_DISTANCE", cClientHandle::MAX_VIEW_DISTANCE);
			tolua_constant(tolua_S, "MIN_VIEW_DISTANCE", cClientHandle::MIN_VIEW_DISTANCE);
			tolua_function(tolua_S, "SendPluginMessage", tolua_cClientHandle_SendPluginMessage);
		tolua_endmodule(tolua_S);

		tolua_beginmodule(tolua_S, "cCompositeChat");
			tolua_function(tolua_S, "AddRunCommandPart",     tolua_cCompositeChat_AddRunCommandPart);
			tolua_function(tolua_S, "AddSuggestCommandPart", tolua_cCompositeChat_AddSuggestCommandPart);
			tolua_function(tolua_S, "AddTextPart",           tolua_cCompositeChat_AddTextPart);
			tolua_function(tolua_S, "AddUrlPart",            tolua_cCompositeChat_AddUrlPart);
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

		tolua_beginmodule(tolua_S, "cEntity");
			tolua_constant(tolua_S, "INVALID_ID", cEntity::INVALID_ID);
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

		tolua_beginmodule(tolua_S, "cItemGrid");
			tolua_function(tolua_S, "GetSlotCoords", Lua_ItemGrid_GetSlotCoords);
		tolua_endmodule(tolua_S);

		tolua_beginmodule(tolua_S, "cLineBlockTracer");
			tolua_function(tolua_S, "Trace", tolua_cLineBlockTracer_Trace);
		tolua_endmodule(tolua_S);

		tolua_beginmodule(tolua_S, "cLuaWindow");
			tolua_function(tolua_S, "SetOnClosing",     tolua_SetObjectCallback<cLuaWindow, &cLuaWindow::SetOnClosing>);
			tolua_function(tolua_S, "SetOnSlotChanged", tolua_SetObjectCallback<cLuaWindow, &cLuaWindow::SetOnSlotChanged>);
		tolua_endmodule(tolua_S);

		tolua_beginmodule(tolua_S, "cMapManager");
			tolua_function(tolua_S, "DoWithMap", DoWithID<cMapManager, cMap, &cMapManager::DoWithMap>);
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
			tolua_function(tolua_S, "OpenWindow",        tolua_cPlayer_OpenWindow);
			tolua_function(tolua_S, "PermissionMatches", tolua_cPlayer_PermissionMatches);
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
			tolua_function(tolua_S, "FindAndDoWithPlayer", DoWith <cRoot, cPlayer, &cRoot::FindAndDoWithPlayer>);
			tolua_function(tolua_S, "DoWithPlayerByUUID",  DoWith <cRoot, cPlayer, &cRoot::DoWithPlayerByUUID>);
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
		tolua_endmodule(tolua_S);

		tolua_beginmodule(tolua_S, "cWebAdmin");
			tolua_function(tolua_S, "GetHTMLEscapedString", tolua_AllToLua_cWebAdmin_GetHTMLEscapedString);
			tolua_function(tolua_S, "GetPlugins",           tolua_cWebAdmin_GetPlugins);
			tolua_function(tolua_S, "GetURLEncodedString",  tolua_AllToLua_cWebAdmin_GetURLEncodedString);
		tolua_endmodule(tolua_S);

		tolua_beginmodule(tolua_S, "cWebPlugin");
			tolua_function(tolua_S, "GetTabNames", tolua_cWebPlugin_GetTabNames);
		tolua_endmodule(tolua_S);

		tolua_beginmodule(tolua_S, "HTTPRequest");
			tolua_variable(tolua_S, "FormData",   tolua_get_HTTPRequest_FormData,   nullptr);
			tolua_variable(tolua_S, "Params",     tolua_get_HTTPRequest_Params,     nullptr);
			tolua_variable(tolua_S, "PostParams", tolua_get_HTTPRequest_PostParams, nullptr);
		tolua_endmodule(tolua_S);

		BindNetwork(tolua_S);
		BindRankManager(tolua_S);
		BindWorld(tolua_S);

	tolua_endmodule(tolua_S);
}




