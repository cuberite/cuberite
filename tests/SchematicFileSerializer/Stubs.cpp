
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





cBoundingBox cBlockHandler::GetPlacementCollisionBox(BLOCKTYPE a_XM, BLOCKTYPE a_XP, BLOCKTYPE a_YM, BLOCKTYPE a_YP, BLOCKTYPE a_ZM, BLOCKTYPE a_ZP) const
{
	return cBoundingBox(0, 0, 0, 0, 0, 0);
}





bool cBlockHandler::GetPlacementBlockTypeMeta(
	cChunkInterface & a_ChunkInterface, cPlayer & a_Player,
	const Vector3i a_BlockPos,
	eBlockFace a_BlockFace,
	const Vector3i a_CursorPos,
	BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta
) const
{
	return true;
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





cItems cBlockHandler::ConvertToPickups(NIBBLETYPE a_BlockMeta, const cEntity * a_Digger, const cItem * a_Tool) const
{
	return cItems();
}





bool cBlockHandler::CanBeAt(cChunkInterface & a_ChunkInterface, const Vector3i a_RelPos, const cChunk & a_Chunk) const
{
	return true;
}





bool cBlockHandler::IsUseable() const
{
	return false;
}





bool cBlockHandler::IsClickedThrough(void) const
{
	return false;
}





bool cBlockHandler::DoesIgnoreBuildCollision(cChunkInterface & a_ChunkInterface, Vector3i a_Pos, cPlayer & a_Player, NIBBLETYPE a_Meta) const
{
	return (m_BlockType == E_BLOCK_AIR);
}





bool cBlockHandler::DoesDropOnUnsuitable(void) const
{
	return true;
}





void cBlockHandler::Check(cChunkInterface & a_ChunkInterface, cBlockPluginInterface & a_PluginInterface, Vector3i a_RelPos, cChunk & a_Chunk) const
{
}





ColourID cBlockHandler::GetMapBaseColourID(NIBBLETYPE a_Meta) const
{
	return 0;
}





bool cBlockHandler::IsInsideBlock(Vector3d a_Position, const NIBBLETYPE a_BlockMeta) const
{
	return true;
}





const cBlockHandler & cBlockHandler::For(BLOCKTYPE a_BlockType)
{
	// Dummy handler.
	static cBlockHandler Handler(E_BLOCK_AIR);
	return Handler;
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
