#include "Globals.h"
#include "BlockState.h"





BlockState::BlockState():
	mChecksum(initializeChecksum())
{
	// Nothing needed yet
}





BlockState::BlockState(const AString & aKey, const AString & aValue):
	mState({{aKey, aValue}}),
	mChecksum(initializeChecksum())
{
}





BlockState::BlockState(std::initializer_list<std::pair<const AString, AString>> aKeysAndValues):
	mState(aKeysAndValues),
	mChecksum(initializeChecksum())
{
}





BlockState::BlockState(const std::map<AString, AString> & aKeysAndValues):
	mState(aKeysAndValues),
	mChecksum(initializeChecksum())
{
}





BlockState::BlockState(std::map<AString, AString> && aKeysAndValues):
	mState(std::move(aKeysAndValues)),
	mChecksum(initializeChecksum())
{
}





BlockState::BlockState(const BlockState & aCopyFrom, std::initializer_list<std::pair<const AString, AString>> aAdditionalKeysAndValues):
	mState(aCopyFrom.mState)
{
	for (const auto & kav: aAdditionalKeysAndValues)
	{
		mState[kav.first] = kav.second;
	}
	mChecksum = initializeChecksum();
}





BlockState::BlockState(const BlockState & aCopyFrom, const std::map<AString, AString> & aAdditionalKeysAndValues):
	mState(aCopyFrom.mState)
{
	for (const auto & kav: aAdditionalKeysAndValues)
	{
		mState[kav.first] = kav.second;
	}
	mChecksum = initializeChecksum();
}




// This iteratively compares "right-aligned". So it works roughly like this:
//
//   BlockState({{"9", "9"}})                |    "     9 9"
//   BlockState({{"12", "34"}})              |    "    1234"
//   BlockState({{"1", "2"}, {"3", "4"}})    |    " 1 2 3 4"
//
// but without actually assembling the strings ofc.
// Equality is the worst-case
// This will fail with an unordered collection, but std::map is ordered.
bool BlockState::operator <(const BlockState & aOther) const
{
	// Can fast-fail this due to how comparison works
	if (mState.size() < aOther.mState.size())
	{
		return true;
	}
	if (mState.size() > aOther.mState.size())
	{
		return false;
	}
	auto it_a = mState.begin();
	auto it_o = aOther.mState.begin();

	// don't need to check it_o, size checks above ensure size(A) == size(O)
	while (it_a != mState.end())
	{
		{
			auto cmp_k = it_a->first.compare(it_o->first);
			if (cmp_k < 0)
			{
				return true;
			}
			if (cmp_k > 0)
			{
				return false;
			}
		}
		{
			auto cmp_v = it_a->second.compare(it_o->second);
			if (cmp_v < 0)
			{
				return true;
			}
			if (cmp_v > 0)
			{
				return false;
			}
		}

		it_a++;
		it_o++;
	}

	return false;
}





bool BlockState::operator ==(const BlockState & aOther) const
{
	// Fast-fail if the checksums differ or differrent counts:
	if ((mChecksum != aOther.mChecksum) || (mState.size() != aOther.mState.size()))
	{
		return false;
	}

	// Slow-check everything if the checksums match:
	return std::equal(mState.begin(), mState.end(), aOther.mState.begin());
}





const AString & BlockState::value(const AString & aKey) const
{
	auto itr = mState.find(aKey);
	if (itr == mState.end())
	{
		static AString empty;
		return empty;
	}
	return itr->second;
}





UInt32 BlockState::initializeChecksum()
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





void BlockState::removeEmptyKeys()
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





UInt32 BlockState::partialChecksum(const AString & aString)
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
