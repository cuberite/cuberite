
// Stubs.cpp

// Implements stubs of various Cuberite methods that are needed for linking but not for runtime
// This is required so that we don't bring in the entire Cuberite via dependencies

#include "Globals.h"
#include "BlockInfo.h"
#include "Blocks/BlockHandler.h"
#include "BlockEntities/BlockEntity.h"





cBoundingBox::cBoundingBox(double, double, double, double, double, double)
{
}





cBoundingBox cBlockHandler::GetPlacementCollisionBox(BlockState a_XM, BlockState a_XP, BlockState a_YM, BlockState a_YP, BlockState a_ZM, BlockState a_ZP) const
{
	return cBoundingBox(0, 0, 0, 0, 0, 0);
}





void cBlockHandler::OnUpdate(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, cBlockPluginInterface & a_PluginInterface, cChunk & a_Chunk, const Vector3i a_RelPos) const
{
}





void cBlockHandler::OnNeighborChanged(cChunkInterface & a_ChunkInterface, Vector3i a_BlockPos, eBlockFace a_WhichNeighbor) const
{
}





void cBlockHandler::NeighborChanged(cChunkInterface & a_ChunkInterface, Vector3i a_BlockPos, eBlockFace a_WhichNeighbor)
{
}





cItems cBlockHandler::ConvertToPickups(BlockState a_Block, const cItem * a_Tool) const
{
	return cItems();
}





bool cBlockHandler::CanBeAt(const cChunk & a_Chunk, const Vector3i a_Position, const BlockState a_Self) const
{
	return true;
}





bool cBlockHandler::IsUseable() const
{
	return false;
}





bool cBlockHandler::DoesIgnoreBuildCollision(const cWorld & a_World, const cItem & a_HeldItem, Vector3i a_Position, BlockState a_Self, eBlockFace a_ClickedBlockFace, bool a_ClickedDirectly) const
{
	return m_BlockType == BlockType::Air;
}





void cBlockHandler::Check(cChunkInterface & a_ChunkInterface, cBlockPluginInterface & a_PluginInterface, Vector3i a_RelPos, cChunk & a_Chunk) const
{
}





ColourID cBlockHandler::GetMapBaseColourID() const
{
	return 0;
}





bool cBlockHandler::IsInsideBlock(Vector3d a_Position, const BlockState a_Self) const
{
	return true;
}





const cBlockHandler & cBlockHandler::For(BlockType a_BlockType)
{
	// Dummy handler.
	static cBlockHandler Handler(BlockType::Air);
	return Handler;
}





bool cBlockEntity::IsBlockEntityBlockType(BlockState a_Block)
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





OwnedBlockEntity cBlockEntity::CreateByBlockType(BlockState a_Block, Vector3i a_Pos, cWorld * a_World)
{
	return nullptr;
}
