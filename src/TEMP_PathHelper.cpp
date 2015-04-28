//Overkill includes out of desperation, it worked.

#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Server.h"
#include "ClientHandle.h"
#include "Mobs/Monster.h"
#include "Root.h"
#include "World.h"
#include "ChunkDef.h"
#include "Bindings/PluginManager.h"
#include "ChatColor.h"
#include "Entities/Player.h"
#include "Inventory.h"
#include "Item.h"
#include "FurnaceRecipe.h"
#include "WebAdmin.h"
#include "Protocol/ProtocolRecognizer.h"
#include "CommandOutput.h"

#include "IniFile.h"
#include "Vector3.h"

#include "TEMP_PathHelper.h"
#include "Mobs/Path.h"
void cPathHelper::consoleCommand()
{
	cPath::consoleCommand();
}
