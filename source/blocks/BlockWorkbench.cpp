#include "Globals.h"
#include "BlockWorkbench.h"
#include "../cItem.h"
#include "../cPlayer.h"
#include "../cCraftingWindow.h"

cBlockWorkbenchHandler::cBlockWorkbenchHandler(BLOCKTYPE a_BlockID)
	: cBlockHandler(a_BlockID)
{
}

void cBlockWorkbenchHandler::OnPlaced(cWorld *a_World, int a_X, int a_Y, int a_Z, int a_Dir)
{
}

void cBlockWorkbenchHandler::OnDestroyed(cWorld *a_World, int a_X, int a_Y, int a_Z)
{
}

void cBlockWorkbenchHandler::OnClick(cWorld *a_World, cPlayer *a_Player, int a_X, int a_Y, int a_Z)
{
	cWindow* Window = new cCraftingWindow(0, true);
	a_Player->OpenWindow(Window);
}
