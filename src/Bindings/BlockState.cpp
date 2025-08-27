#include "Globals.h"
#include "Bindings/BlockState.h"





CustomBlockState::CustomBlockState():
	mChecksum(initializeChecksum())
{
	// Nothing needed yet
}





CustomBlockState::CustomBlockState(const AString & aKey, const AString & aValue):
	mState({{aKey, aValue}}),
	mChecksum(initializeChecksum())
{
}





CustomBlockState::CustomBlockState(std::initializer_list<std::pair<const AString, AString>> aKeysAndValues):
	mState(aKeysAndValues),
	mChecksum(initializeChecksum())
{
}





CustomBlockState::CustomBlockState(const std::map<AString, AString> & aKeysAndValues):
	mState(aKeysAndValues),
	mChecksum(initializeChecksum())
{
}





CustomBlockState::CustomBlockState(std::map<AString, AString> && aKeysAndValues):
	mState(std::move(aKeysAndValues)),
	mChecksum(initializeChecksum())
{
}





CustomBlockState::CustomBlockState(const CustomBlockState & aCopyFrom, std::initializer_list<std::pair<const AString, AString>> aAdditionalKeysAndValues):
	mState(aCopyFrom.mState)
{
	for (const auto & kav: aAdditionalKeysAndValues)
	{
		mState[kav.first] = kav.second;
	}
	mChecksum = initializeChecksum();
}





CustomBlockState::CustomBlockState(const CustomBlockState & aCopyFrom, const std::map<AString, AString> & aAdditionalKeysAndValues):
	mState(aCopyFrom.mState)
{
	for (const auto & kav: aAdditionalKeysAndValues)
	{
		mState[kav.first] = kav.second;
	}
	mChecksum = initializeChecksum();
}





bool CustomBlockState::operator <(const CustomBlockState & aOther) const
{
	// Fast-return this using checksum
	if (mChecksum != aOther.mChecksum)
	{
		return (mChecksum < aOther.mChecksum);
	}

	// Can fast-return this due to how comparison works
	if (mState.size() != aOther.mState.size())
	{
		return (mState.size() < aOther.mState.size());
	}

	auto itA = mState.begin();
	auto itOther = aOther.mState.begin();

	// don't need to check itOther, size checks above ensure size(A) == size(O)
	while (itA != mState.end())
	{
		{
			const auto cmp = itA->first.compare(itOther->first);
			if (cmp != 0)
			{
				return (cmp < 0);
			}
		}
		{
			const auto cmp = itA->second.compare(itOther->second);
			if (cmp != 0)
			{
				return (cmp < 0);
			}
		}

		++itA;
		++itOther;
	}

	return false;
}





bool CustomBlockState::operator ==(const CustomBlockState & aOther) const
{
	// Fast-fail if the checksums differ or differrent counts:
	if ((mChecksum != aOther.mChecksum) || (mState.size() != aOther.mState.size()))
	{
		return false;
	}

	// Slow-check everything if the checksums match:
	return std::equal(mState.begin(), mState.end(), aOther.mState.begin());
}





const AString & CustomBlockState::value(const AString & aKey) const
{
	auto itr = mState.find(aKey);
	if (itr == mState.end())
	{
		static AString empty;
		return empty;
	}
	return itr->second;
}





UInt32 CustomBlockState::initializeChecksum()
{
	removeEmptyKeys();

	// Calculate the checksum as a XOR of all mState keys' and values' checksums
	// This way we don't depend on the std::map's ordering
	UInt32 res = 0;
	for (const auto & kv: mState)
	{
		auto partial = partialChecksum(kv.first) ^ partialChecksum(kv.second);
		res = res ^ partial;
	}
	return res;
}





void CustomBlockState::removeEmptyKeys()
{
	for (auto itr = mState.begin(); itr != mState.end();)
	{
		if (itr->second.empty())
		{
			itr = mState.erase(itr);
		}
		else
		{
			++itr;
		}
	}
}





UInt32 CustomBlockState::partialChecksum(const AString & aString)
{
	UInt32 shift = 0;
	UInt32 res = 0;
	for (auto ch: aString)
	{
		UInt32 v = static_cast<UInt8>(ch);
		v = v << shift;
		shift = (shift + 1) % 24;
		res = res ^ v;
	}
	return res;
}
