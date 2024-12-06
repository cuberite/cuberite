#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "ShulkerBoxEntity.h"
#include "../BlockInfo.h"
#include "../Item.h"
#include "../Entities/Player.h"
#include "../UI/ShulkerBoxWindow.h"
#include "../ClientHandle.h"
#include "../Mobs/Ocelot.h"

class cItemGrid;





cShulkerBoxEntity::cShulkerBoxEntity(BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, Vector3i a_Pos, cWorld * a_World):
	Super(a_BlockType, a_BlockMeta, a_Pos, ContentsWidth, ContentsHeight, a_World),
	m_NumActivePlayers(0)
{
}





void cShulkerBoxEntity::CopyFrom(const cBlockEntity & a_Src)
{
	Super::CopyFrom(a_Src);
	auto & src = static_cast<const cShulkerBoxEntity &>(a_Src);
	m_Contents.CopyFrom(src.m_Contents);

	m_NumActivePlayers = 0;
}





void cShulkerBoxEntity::OnRemoveFromWorld()
{
	const auto Window = GetWindow();
	if (Window != nullptr)
	{
		Window->OwnerDestroyed();
	}
}





void cShulkerBoxEntity::SendTo(cClientHandle & a_Client)
{
	a_Client.SendBlockAction(m_Pos.x, m_Pos.y, m_Pos.z, 1, 0, m_BlockType);
}





bool cShulkerBoxEntity::UsedBy(cPlayer * a_Player)
{
	if (IsBlocked())
	{
		// Obstruction, don't open
		return false;
	}

	cShulkerBoxEntity * ShulkerBox = this;

	a_Player->GetStatManager().AddValue(Statistic::OpenShulkerBox);


	// If the window is not created, open it anew:
	cWindow * Window = ShulkerBox->GetWindow();
	if (Window == nullptr)
	{
		ShulkerBox->OpenNewWindow();
		Window = ShulkerBox->GetWindow();
	}

	// Open the window for the player:
	if (Window != nullptr)
	{
		if (a_Player->GetWindow() != Window)
		{
			a_Player->OpenWindow(*Window);
		}
	}

	// This is rather a hack
	// Instead of marking the chunk as dirty upon chest contents change, we mark it dirty now
	// We cannot properly detect contents change, but such a change doesn't happen without a player opening the chest first.
	// The few false positives aren't much to worry about
	auto chunkCoords = cChunkDef::BlockToChunk(m_Pos);
	m_World->MarkChunkDirty(chunkCoords.m_ChunkX, chunkCoords.m_ChunkZ);
	return true;
}





void cShulkerBoxEntity::OpenNewWindow(void)
{
	OpenWindow(new cShulkerBoxWindow(this, m_CustomName));
}





bool cShulkerBoxEntity::IsShulkerBox(short a_ItemType)
{
	switch (a_ItemType)
	{
		case E_BLOCK_WHITE_SHULKER_BOX:
		case E_BLOCK_ORANGE_SHULKER_BOX:
		case E_BLOCK_MAGENTA_SHULKER_BOX:
		case E_BLOCK_LIGHT_BLUE_SHULKER_BOX:
		case E_BLOCK_YELLOW_SHULKER_BOX:
		case E_BLOCK_LIME_SHULKER_BOX:
		case E_BLOCK_PINK_SHULKER_BOX:
		case E_BLOCK_GRAY_SHULKER_BOX:
		case E_BLOCK_LIGHT_GRAY_SHULKER_BOX:
		case E_BLOCK_CYAN_SHULKER_BOX:
		case E_BLOCK_PURPLE_SHULKER_BOX:
		case E_BLOCK_BLUE_SHULKER_BOX:
		case E_BLOCK_BROWN_SHULKER_BOX:
		case E_BLOCK_GREEN_SHULKER_BOX:
		case E_BLOCK_RED_SHULKER_BOX:
		case E_BLOCK_BLACK_SHULKER_BOX:
			return true;
	}

	return false;
}





Vector3i cShulkerBoxEntity::BlockMetaToObstructionCheckDirection(NIBBLETYPE a_BlockMeta)
{
	switch (a_BlockMeta)
	{
		case BLOCK_FACE_XP: return Vector3i(1, 0, 0);
		case BLOCK_FACE_XM: return Vector3i(-1, 0, 0);
		case BLOCK_FACE_YP: return Vector3i(0, 1, 0);
		case BLOCK_FACE_YM: return Vector3i(0, -1, 0);
		case BLOCK_FACE_ZP: return Vector3i(0, 0, 1);
		case BLOCK_FACE_ZM: return Vector3i(0, 0, -1);
	}
	return Vector3i();
}





bool cShulkerBoxEntity::IsBlocked()
{
	Vector3i CheckPos = GetPos() + cShulkerBoxEntity::BlockMetaToObstructionCheckDirection(m_BlockMeta);

	return (
		(GetPosY() < cChunkDef::Height - 1) &&
		(
			!cBlockInfo::IsTransparent(GetWorld()->GetBlock(CheckPos)) ||
			!cOcelot::IsCatSittingOnBlock(GetWorld(), Vector3d(CheckPos))
		)
	);
}





void cShulkerBoxEntity::OnSlotChanged(cItemGrid * a_Grid, int a_SlotNum)
{
	ASSERT(a_Grid == &m_Contents);

	if (m_World == nullptr)
	{
		return;
	}

	// Have cBlockEntityWithItems update redstone and try to broadcast our window:
	Super::OnSlotChanged(a_Grid, a_SlotNum);

	cWindow * Window = GetWindow();

	if (Window != nullptr)
	{
		Window->BroadcastWholeWindow();
	}

	m_World->MarkChunkDirty(GetChunkX(), GetChunkZ());
}





cItems cShulkerBoxEntity::ConvertToPickups() const
{
	cItem ShulkerBoxDrop = cItem(GetBlockType(), 1, 0, "", m_CustomName);

	Json::Value a_TagJson(Json::objectValue);
	BlockEntityTagSerializer::WriteToJson(a_TagJson, ShulkerBoxDrop, GetContents());
	ShulkerBoxDrop.m_BlockEntityTag = a_TagJson;

	return ShulkerBoxDrop;
}





void cShulkerBoxEntity::Load(const cItem & a_Item)
{
	auto a_ItemGrid = cItemGrid(3, 9);
	BlockEntityTagSerializer::ParseFromJson(a_Item.m_BlockEntityTag, a_ItemGrid);
	if (!a_Item.IsCustomNameEmpty())
	{
		m_CustomName = a_Item.m_CustomName;
	}
	m_Contents.CopyFrom(a_ItemGrid);
}
