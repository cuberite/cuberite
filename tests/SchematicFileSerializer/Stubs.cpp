
// Stubs.cpp

// Implements stubs of various Cuberite methods that are needed for linking but not for runtime
// This is required so that we don't bring in the entire Cuberite via dependencies

#include "Globals.h"
#include "BlockInfo.h"
#include "Blocks/BlockHandler.h"
#include "BlockEntities/BlockEntity.h"





void cBlockInfo::sHandlerDeleter::operator () (cBlockHandler * a_Handler)
{
	delete a_Handler;
}





cBlockInfo::cBlockInfoArray::cBlockInfoArray()
{
	cBlockInfoArray & BlockInfos = *this;
	// The piece-loading code uses the handlers for rotations, so we need valid handlers
	// Insert dummy handlers:
	for (size_t i = 0; i < BlockInfos.size(); i++)
	{
		BlockInfos[i].m_Handler.reset(new cBlockHandler(static_cast<BLOCKTYPE>(i)));
	}
}


cBoundingBox::cBoundingBox(double, double, double, double, double, double)
{
}





cBlockHandler::cBlockHandler(BLOCKTYPE a_BlockType)
{
}





cBoundingBox cBlockHandler::GetPlacementCollisionBox(BLOCKTYPE a_XM, BLOCKTYPE a_XP, BLOCKTYPE a_YM, BLOCKTYPE a_YP, BLOCKTYPE a_ZM, BLOCKTYPE a_ZP)
{
	return cBoundingBox(0, 0, 0, 0, 0, 0);
}





bool cBlockHandler::GetPlacementBlockTypeMeta(
	cChunkInterface & a_ChunkInterface, cPlayer & a_Player,
	int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace,
	int a_CursorX, int a_CursorY, int a_CursorZ,
	BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta
)
{
	return true;
}





void cBlockHandler::OnUpdate(cChunkInterface & cChunkInterface, cWorldInterface & a_WorldInterface, cBlockPluginInterface & a_PluginInterface, cChunk & a_Chunk, int a_BlockX, int a_BlockY, int a_BlockZ)
{
}





void cBlockHandler::OnPlacedByPlayer(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, cPlayer & a_Player, const sSetBlock & a_BlockChange)
{
}





void cBlockHandler::OnDestroyedByPlayer(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ)
{
}





void cBlockHandler::OnPlaced(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta)
{
}





void cBlockHandler::OnDestroyed(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, int a_BlockX, int a_BlockY, int a_BlockZ)
{
}





void cBlockHandler::NeighborChanged(cChunkInterface & a_ChunkInterface, int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_WhichNeighbor)
{
}





void cBlockHandler::ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta)
{
}





void cBlockHandler::DropBlock(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, cBlockPluginInterface & a_BlockPluginInterface, cEntity * a_Digger, int a_BlockX, int a_BlockY, int a_BlockZ, bool a_CanDrop)
{
}





bool cBlockHandler::CanBeAt(cChunkInterface & a_ChunkInterface, int a_BlockX, int a_BlockY, int a_BlockZ, const cChunk & a_Chunk)
{
	return true;
}





bool cBlockHandler::IsUseable()
{
	return false;
}





bool cBlockHandler::IsClickedThrough(void)
{
	return false;
}





bool cBlockHandler::DoesIgnoreBuildCollision(cChunkInterface & a_ChunkInterface, Vector3i a_Pos, cPlayer & a_Player, NIBBLETYPE a_Meta)
{
	return (m_BlockType == E_BLOCK_AIR);
}





bool cBlockHandler::DoesDropOnUnsuitable(void)
{
	return true;
}





void cBlockHandler::Check(cChunkInterface & a_ChunkInterface, cBlockPluginInterface & a_PluginInterface, int a_RelX, int a_RelY, int a_RelZ, cChunk & a_Chunk)
{
}





ColourID cBlockHandler::GetMapBaseColourID(NIBBLETYPE a_Meta)
{
	return 0;
}





bool cBlockHandler::IsInsideBlock(Vector3d a_Position, const BLOCKTYPE a_BlockType, const NIBBLETYPE a_BlockMeta)
{
	return true;
}





bool cBlockEntity::IsBlockEntityBlockType(BLOCKTYPE a_BlockType)
{
	return false;
}





void cBlockEntity::SetPos(int a_BlockX, int a_BlockY, int a_BlockZ)
{
}





cBlockEntity * cBlockEntity::Clone(int a_BlockX, int a_BlockY, int a_BlockZ)
{
	return nullptr;
}





cBlockEntity * cBlockEntity::CreateByBlockType(BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, int a_BlockX, int a_BlockY, int a_BlockZ, cWorld * a_World)
{
	return nullptr;
}




