#include "Globals.h"
#include "BlockTypePalette.h"




BlockTypePalette::BlockTypePalette():
	mMaxIndex(0)
{
}





UInt32 BlockTypePalette::index(const AString & aBlockTypeName, const BlockState & aBlockState)
{
	auto idx = maybeIndex(aBlockTypeName, aBlockState);
	if (idx.second)
	{
		return idx.first;
	}

	// Not found, append:
	auto index = mMaxIndex++;
	mBlockToNumber[aBlockTypeName][aBlockState] = index;
	mNumberToBlock[index] = {aBlockTypeName, aBlockState};
	return index;
}





std::pair<UInt32, bool> BlockTypePalette::maybeIndex(const AString & aBlockTypeName, const BlockState & aBlockState) const
{
	auto itr1 = mBlockToNumber.find(aBlockTypeName);
	if (itr1 == mBlockToNumber.end())
	{
		return {0, false};
	}
	auto itr2 = itr1->second.find(aBlockState);
	if (itr2 == itr1->second.end())
	{
		return {0, false};
	}
	return {itr2->second, true};
}





UInt32 BlockTypePalette::count() const
{
	return static_cast<UInt32>(mNumberToBlock.size());
}





const std::pair<AString, BlockState> & BlockTypePalette::entry(UInt32 aIndex) const
{
	auto itr = mNumberToBlock.find(aIndex);
	if (itr == mNumberToBlock.end())
	{
		throw NoSuchIndexException(aIndex);
	}
	return itr->second;
}





std::map<UInt32, UInt32> BlockTypePalette::createTransformMapAddMissing(const BlockTypePalette & aFrom)
{
	std::map<UInt32, UInt32> res;
	for (const auto & fromEntry: aFrom.mNumberToBlock)
	{
		auto fromIndex = fromEntry.first;
		const auto & blockTypeName = fromEntry.second.first;
		const auto & blockState = fromEntry.second.second;
		res[fromIndex] = index(blockTypeName, blockState);
	}
	return res;
}





std::map<UInt32, UInt32> BlockTypePalette::createTransformMapWithFallback(const BlockTypePalette & aFrom, UInt32 aFallbackIndex) const
{
	std::map<UInt32, UInt32> res;
	for (const auto & fromEntry: aFrom.mNumberToBlock)
	{
		auto fromIndex = fromEntry.first;
		const auto & blockTypeName = fromEntry.second.first;
		const auto & blockState = fromEntry.second.second;
		auto thisIndex = maybeIndex(blockTypeName, blockState);
		if (thisIndex.second)
		{
			// The entry was found in this
			res[fromIndex] = thisIndex.first;
		}
		else
		{
			// The entry was NOT found in this, replace with fallback:
			res[fromIndex] = aFallbackIndex;
		}
	}
	return res;
}
