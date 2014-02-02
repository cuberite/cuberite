
#include "Globals.h"

#include "ChunkInterface.h"
#include "BlockHandler.h"

bool cChunkInterface::DigBlock(cWorldInterface & a_WorldInterface, int a_X, int a_Y, int a_Z)
{
	cBlockHandler *Handler = cBlockHandler::GetBlockHandler(GetBlock(a_X, a_Y, a_Z));
	Handler->OnDestroyed(*this, a_WorldInterface, a_X, a_Y, a_Z);
	return m_ChunkMap->DigBlock(a_X, a_Y, a_Z);
}
