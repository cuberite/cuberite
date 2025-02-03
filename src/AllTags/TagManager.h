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
	}

	const TagRegistry::cTagRegistry<Item, &NamespaceSerializer::ToItem, ItemTags, &TagTranslation::GetItem> & GetItemTags() const { return m_ItemTags; }
	const TagRegistry::cTagRegistry<BlockType, &NamespaceSerializer::ToBlockType, BlockTags, &TagTranslation::GetBlock> & GetBlockTags() const { return m_BlockTags; }
private:
	TagRegistry::cTagRegistry<Item, &NamespaceSerializer::ToItem, ItemTags, &TagTranslation::GetItem> m_ItemTags;
	TagRegistry::cTagRegistry<BlockType, &NamespaceSerializer::ToBlockType, BlockTags, &TagTranslation::GetBlock> m_BlockTags;
};
