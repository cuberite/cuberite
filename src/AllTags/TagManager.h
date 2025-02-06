#pragma once
#include "Globals.h"
#include "AbstractTag.h"
#include "Tags.h"

class TagManager
{
public:
	void SetupTags()
	{
		m_ItemTags.LoadFromFiles("Protocol/1.21.4/tags/item/", "item");
		m_BlockTags.LoadFromFiles("Protocol/1.21.4/tags/block/", "block");
		m_FluidTags.LoadFromFiles("Protocol/1.21.4/tags/fluid/", "fluid");
	}

	const TagRegistry::cTagRegistry<Item, &NamespaceSerializer::ToItem, ItemTags, &TagTranslation::GetItem> & GetItemTags() const { return m_ItemTags; }
	const TagRegistry::cTagRegistry<BlockType, &NamespaceSerializer::ToBlockType, BlockTags, &TagTranslation::GetBlock> & GetBlockTags() const { return m_BlockTags; }
	const TagRegistry::cTagRegistry<FluidType, &NamespaceSerializer::ToFluidType, FluidTags, &TagTranslation::GetFluid> & GetFluidTags() const { return m_FluidTags; }
private:
	TagRegistry::cTagRegistry<Item, &NamespaceSerializer::ToItem, ItemTags, &TagTranslation::GetItem> m_ItemTags;
	TagRegistry::cTagRegistry<BlockType, &NamespaceSerializer::ToBlockType, BlockTags, &TagTranslation::GetBlock> m_BlockTags;
	TagRegistry::cTagRegistry<FluidType, &NamespaceSerializer::ToFluidType, FluidTags, &TagTranslation::GetFluid> m_FluidTags;
};
