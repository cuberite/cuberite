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
