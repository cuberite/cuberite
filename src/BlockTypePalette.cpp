#include "Globals.h"
#include "BlockTypePalette.h"




BlockTypePalette::BlockTypePalette()
{
	// Nothing needed yet
}





UInt32 BlockTypePalette::index(const AString & aBlockTypeName, const BlockState & aBlockState)
{
	auto idx = maybeIndex(aBlockTypeName, aBlockState);
	if (idx.second)
	{
		return idx.first;
	}

	// Not found, append:
	mPalette.push_back(std::make_pair(aBlockTypeName, aBlockState));
	return static_cast<UInt32>(mPalette.size() - 1);
}





std::pair<UInt32, bool> BlockTypePalette::maybeIndex(const AString & aBlockTypeName, const BlockState & aBlockState) const
{
	auto count = mPalette.size();
	for (size_t idx = 0; idx < count; ++idx)
	{
		const auto & entry = mPalette[idx];
		if ((entry.first == aBlockTypeName) && (entry.second == aBlockState))
		{
			return std::make_pair(static_cast<UInt32>(idx), true);
		}
	}
	return std::make_pair(0, false);
}





UInt32 BlockTypePalette::count() const
{
	return static_cast<UInt32>(mPalette.size());
}





const std::pair<AString, BlockState> & BlockTypePalette::entry(UInt32 aIndex) const
{
	ASSERT(aIndex < mPalette.size());
	return mPalette[aIndex];
}





std::map<UInt32, UInt32> BlockTypePalette::createTransformMap(const BlockTypePalette & aOther)
{
	std::map<UInt32, UInt32> res;
	auto numIndices = aOther.count();
	for (UInt32 idx = 0; idx < numIndices; ++idx)
	{
		const auto & e = aOther.mPalette[idx];
		res[idx] = index(e.first, e.second);
	}
	return res;
}
