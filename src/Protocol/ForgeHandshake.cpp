
// ForgeHandshake.cpp

// Implements Forge protocol handshaking

#include "ForgeHandshake.h"

#include "json/json.h"

void cForgeHandshake::augmentServerListPing(Json::Value & ResponseValue)
{
	// modinfo:
	// TODO: only send if mods enabled
	Json::Value Modinfo;
	Modinfo["type"] = "FML";

	Json::Value ModList(Json::arrayValue);
	// TODO: customizable modList
	Modinfo["modList"] = ModList;

	// Augment the response:
	ResponseValue["modinfo"] = Modinfo;
}
