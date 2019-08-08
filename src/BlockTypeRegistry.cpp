
#include "Globals.h"
#include "BlockTypeRegistry.h"




////////////////////////////////////////////////////////////////////////////////
// BlockInfo:

BlockInfo::BlockInfo(
	const AString & aPluginName,
	const AString & aBlockTypeName,
	std::shared_ptr<cBlockHandler> aHandler,
	const std::map<AString, AString> & aHints,
	const std::map<AString, BlockInfo::HintCallback> & aHintCallbacks
):
	mPluginName(aPluginName),
	mBlockTypeName(aBlockTypeName),
	mHandler(aHandler),
	mHints(aHints),
	mHintCallbacks(aHintCallbacks)
{
}





AString BlockInfo::hintValue(
	const AString & aHintName,
	const BlockState & aBlockState
)
{
	// Search the hint callbacks first:
	auto itrC = mHintCallbacks.find(aHintName);
	if (itrC != mHintCallbacks.end())
	{
		// Hint callback found, use it:
		return itrC->second(mBlockTypeName, aBlockState);
	}

	// Search the static hints:
	auto itr = mHints.find(aHintName);
	if (itr != mHints.end())
	{
		// Hint found, use it:
		return itr->second;
	}

	// Nothing found, return empty string:
	return AString();
}





void BlockInfo::setHint(const AString & aHintKey, const AString & aHintValue)
{
	mHints[aHintKey] = aHintValue;

	// Warn if the hint is already provided by a callback (aHintValue will be ignored when evaluating the hint):
	auto itrC = mHintCallbacks.find(aHintKey);
	if (itrC != mHintCallbacks.end())
	{
		LOGINFO("Setting a static hint %s for block type %s, but there's already a callback for that hint. The static hint will be ignored.",
			aHintKey.c_str(), mBlockTypeName.c_str()
		);
	}
}





void BlockInfo::removeHint(const AString & aHintKey)
{
	mHints.erase(aHintKey);
}





////////////////////////////////////////////////////////////////////////////////
// BlockTypeRegistry:

void BlockTypeRegistry::registerBlockType(
	const AString & aPluginName,
	const AString & aBlockTypeName,
	std::shared_ptr<cBlockHandler> aHandler,
	const std::map<AString, AString> & aHints,
	const std::map<AString, BlockInfo::HintCallback> & aHintCallbacks
)
{
	auto blockInfo = std::make_shared<BlockInfo>(aPluginName, aBlockTypeName, aHandler, aHints, aHintCallbacks);

	// Check previous registrations:
	cCSLock lock(mCSRegistry);
	auto itr = mRegistry.find(aBlockTypeName);
	if (itr != mRegistry.end())
	{
		if (itr->second->pluginName() != aPluginName)
		{
			throw AlreadyRegisteredException(itr->second, blockInfo);
		}
	}

	// Store the registration:
	mRegistry[aBlockTypeName] = blockInfo;
}





std::shared_ptr<BlockInfo> BlockTypeRegistry::blockInfo(const AString & aBlockTypeName)
{
	cCSLock lock(mCSRegistry);
	auto itr = mRegistry.find(aBlockTypeName);
	if (itr == mRegistry.end())
	{
		return nullptr;
	}
	return itr->second;
}





void BlockTypeRegistry::removeAllByPlugin(const AString & aPluginName)
{
	cCSLock lock(mCSRegistry);
	for (auto itr = mRegistry.begin(); itr != mRegistry.end();)
	{
		if (itr->second->pluginName() == aPluginName)
		{
			itr = mRegistry.erase(itr);
		}
		else
		{
			++itr;
		}
	}
}





void BlockTypeRegistry::setBlockTypeHint(
	const AString & aBlockTypeName,
	const AString & aHintKey,
	const AString & aHintValue
)
{
	cCSLock lock(mCSRegistry);
	auto blockInfo = mRegistry.find(aBlockTypeName);
	if (blockInfo == mRegistry.end())
	{
		throw NotRegisteredException(aBlockTypeName, aHintKey, aHintValue);
	}
	blockInfo->second->setHint(aHintKey, aHintValue);
}





void BlockTypeRegistry::removeBlockTypeHint(
	const AString & aBlockTypeName,
	const AString & aHintKey
)
{
	cCSLock lock(mCSRegistry);
	auto blockInfo = mRegistry.find(aBlockTypeName);
	if (blockInfo == mRegistry.end())
	{
		return;
	}
	blockInfo->second->removeHint(aHintKey);
}





////////////////////////////////////////////////////////////////////////////////
// BlockTypeRegistry::AlreadyRegisteredException:

BlockTypeRegistry::AlreadyRegisteredException::AlreadyRegisteredException(
	std::shared_ptr<BlockInfo> aPreviousRegistration,
	std::shared_ptr<BlockInfo> aNewRegistration
) :
	Super(message(aPreviousRegistration, aNewRegistration)),
	mPreviousRegistration(aPreviousRegistration),
	mNewRegistration(aNewRegistration)
{
}





AString BlockTypeRegistry::AlreadyRegisteredException::message(
	std::shared_ptr<BlockInfo> aPreviousRegistration,
	std::shared_ptr<BlockInfo> aNewRegistration
)
{
	return Printf("Attempting to register BlockTypeName %s from plugin %s, while it is already registered in plugin %s",
		aNewRegistration->blockTypeName().c_str(),
		aNewRegistration->pluginName().c_str(),
		aPreviousRegistration->pluginName().c_str()
	);
}





////////////////////////////////////////////////////////////////////////////////
// BlockTypeRegistry::NotRegisteredException:

BlockTypeRegistry::NotRegisteredException::NotRegisteredException(
	const AString & aBlockTypeName,
	const AString & aHintKey,
	const AString & aHintValue
):
	Super(Printf(
		"Attempting to set a hint of nonexistent BlockTypeName.\n\tBlockTypeName = %s\n\tHintKey = %s\n\tHintValue = %s",
		aBlockTypeName.c_str(),
		aHintKey.c_str(),
		aHintValue.c_str()
	))
{
}
