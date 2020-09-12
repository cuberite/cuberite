
// Stubs.cpp

// Implements stubs of various Cuberite methods that are needed for linking but not for runtime
// This is required so that we don't bring in the entire Cuberite via dependencies

#include "Globals.h"
#include "BlockInfo.h"
#include "Blocks/BlockHandler.h"
#include "BlockEntities/BlockEntity.h"





cBlockInfo::cBlockInfo()
{
}





cBlockInfo::cBlockInfoArray::cBlockInfoArray()
{
}





cBlockHandler * cBlockInfo::GetHandler(BLOCKTYPE a_Type)
{
	return nullptr;
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
	const Vector3i a_BlockPos,
	eBlockFace a_BlockFace,
	const Vector3i a_CursorPos,
	BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta
)
{
	return true;
}





void cBlockHandler::OnUpdate(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, cBlockPluginInterface & a_PluginInterface, cChunk & a_Chunk, const Vector3i a_RelPos)
{
}





void cBlockHandler::OnNeighborChanged(cChunkInterface & a_ChunkInterface, Vector3i a_BlockPos, eBlockFace a_WhichNeighbor)
{
}





void cBlockHandler::NeighborChanged(cChunkInterface & a_ChunkInterface, Vector3i a_BlockPos, eBlockFace a_WhichNeighbor)
{
}





cItems cBlockHandler::ConvertToPickups(NIBBLETYPE a_BlockMeta, cBlockEntity * a_BlockEntity, const cEntity * a_Digger, const cItem * a_Tool)
{
	return cItems();
}





bool cBlockHandler::CanBeAt(cChunkInterface & a_ChunkInterface, const Vector3i a_RelPos, const cChunk & a_Chunk)
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





void cBlockHandler::Check(cChunkInterface & a_ChunkInterface, cBlockPluginInterface & a_PluginInterface, Vector3i a_RelPos, cChunk & a_Chunk)
{
}





ColourID cBlockHandler::GetMapBaseColourID(NIBBLETYPE a_Meta)
{
	return 0;
}





bool cBlockHandler::IsInsideBlock(Vector3d a_Position, const NIBBLETYPE a_BlockMeta)
{
	return true;
}





bool cBlockEntity::IsBlockEntityBlockType(BLOCKTYPE a_BlockType)
{
	return false;
}





void cBlockEntity::SetPos(Vector3i a_NewPos)
{
}





OwnedBlockEntity cBlockEntity::Clone(Vector3i a_Pos)
{
	return nullptr;
}





OwnedBlockEntity cBlockEntity::CreateByBlockType(BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, Vector3i a_Pos, cWorld * a_World)
{
	return nullptr;
}
