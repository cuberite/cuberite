
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "EnchantingTableEntity.h"
#include "../ClientHandle.h"





cEnchantingTableEntity::cEnchantingTableEntity(BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, Vector3i a_Pos, cWorld * a_World, AString a_CustomName) :
	Super(a_BlockType, a_BlockMeta, a_Pos, a_World),
	m_CustomName(std::move(a_CustomName))
{
	ASSERT(a_BlockType == E_BLOCK_ENCHANTMENT_TABLE);
}





cItems cEnchantingTableEntity::ConvertToPickups() const
{
	cItem Item(E_BLOCK_ENCHANTMENT_TABLE);
	Item.m_CustomName = m_CustomName;
	return Item;
}





void cEnchantingTableEntity::CopyFrom(const cBlockEntity & a_Src)
{
	Super::CopyFrom(a_Src);

	auto & Src = static_cast<const cEnchantingTableEntity &>(a_Src);
	m_CustomName = Src.m_CustomName;
}





bool cEnchantingTableEntity::UsedBy(cPlayer * a_Player)
{
	UNUSED(a_Player);
	return true;
}





void cEnchantingTableEntity::SendTo(cClientHandle & a_Client)
{
	a_Client.SendUpdateBlockEntity(*this);
}
