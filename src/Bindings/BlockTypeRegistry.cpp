
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
	m_PluginName(aPluginName),
	m_BlockTypeName(aBlockTypeName),
	m_Handler(std::move(aHandler)),
	m_Hints(aHints),
	m_HintCallbacks(aHintCallbacks)
{
}





AString BlockInfo::hintValue(
	const AString & aHintName,
	const BlockState & aBlockState
)
{
	// Search the hint callbacks first:
	auto itrC = m_HintCallbacks.find(aHintName);
	if (itrC != m_HintCallbacks.end())
	{
		// Hint callback found, use it:
		return itrC->second(m_BlockTypeName, aBlockState);
	}

	// Search the static hints:
	auto itr = m_Hints.find(aHintName);
	if (itr != m_Hints.end())
	{
		// Hint found, use it:
		return itr->second;
	}

	// Nothing found, return empty string:
	return AString();
}





void BlockInfo::setHint(const AString & aHintKey, const AString & aHintValue)
{
	m_Hints[aHintKey] = aHintValue;

	// Warn if the hint is already provided by a callback (aHintValue will be ignored when evaluating the hint):
	auto itrC = m_HintCallbacks.find(aHintKey);
	if (itrC != m_HintCallbacks.end())
	{
		LOGINFO("Setting a static hint %s for block type %s, but there's already a callback for that hint. The static hint will be ignored.",
			aHintKey.c_str(), m_BlockTypeName.c_str()
		);
	}
}





void BlockInfo::removeHint(const AString & aHintKey)
{
	m_Hints.erase(aHintKey);
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
	auto blockInfo = std::make_shared<BlockInfo>(
		aPluginName, aBlockTypeName, std::move(aHandler), aHints, aHintCallbacks
	);

	// Check previous registrations:
	cCSLock lock(m_CSRegistry);
	auto itr = m_Registry.find(aBlockTypeName);
	if (itr != m_Registry.end())
	{
		if (itr->second->pluginName() != aPluginName)
		{
			throw AlreadyRegisteredException(itr->second, blockInfo);
		}
	}

	// Store the registration:
	m_Registry[aBlockTypeName] = blockInfo;
}





std::shared_ptr<BlockInfo> BlockTypeRegistry::blockInfo(const AString & aBlockTypeName)
{
	cCSLock lock(m_CSRegistry);
	auto itr = m_Registry.find(aBlockTypeName);
	if (itr == m_Registry.end())
	{
		return nullptr;
	}
	return itr->second;
}





void BlockTypeRegistry::removeAllByPlugin(const AString & aPluginName)
{
	cCSLock lock(m_CSRegistry);
	for (auto itr = m_Registry.begin(); itr != m_Registry.end();)
	{
		if (itr->second->pluginName() == aPluginName)
		{
			itr = m_Registry.erase(itr);
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
	cCSLock lock(m_CSRegistry);
	auto blockInfo = m_Registry.find(aBlockTypeName);
	if (blockInfo == m_Registry.end())
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
	cCSLock lock(m_CSRegistry);
	auto blockInfo = m_Registry.find(aBlockTypeName);
	if (blockInfo == m_Registry.end())
	{
		return;
	}
	blockInfo->second->removeHint(aHintKey);
}





////////////////////////////////////////////////////////////////////////////////
// BlockTypeRegistry::AlreadyRegisteredException:

BlockTypeRegistry::AlreadyRegisteredException::AlreadyRegisteredException(
	const std::shared_ptr<BlockInfo> & aPreviousRegistration,
	const std::shared_ptr<BlockInfo> & aNewRegistration
) :
	Super(message(aPreviousRegistration, aNewRegistration)),
	m_PreviousRegistration(aPreviousRegistration),
	m_NewRegistration(aNewRegistration)
{
}





AString BlockTypeRegistry::AlreadyRegisteredException::message(
	const std::shared_ptr<BlockInfo> & aPreviousRegistration,
	const std::shared_ptr<BlockInfo> & aNewRegistration
)
{
	return fmt::format(
		FMT_STRING("Attempting to register BlockTypeName {} from plugin {}, while it is already registered in plugin {}"),
		aNewRegistration->blockTypeName(),
		aNewRegistration->pluginName(),
		aPreviousRegistration->pluginName()
	);
}





////////////////////////////////////////////////////////////////////////////////
// BlockTypeRegistry::NotRegisteredException:

BlockTypeRegistry::NotRegisteredException::NotRegisteredException(
	const AString & aBlockTypeName,
	const AString & aHintKey,
	const AString & aHintValue
):
	Super(fmt::format(
		FMT_STRING("Attempting to set a hint of nonexistent BlockTypeName.\n\tBlockTypeName = {}\n\tHintKey = {}\n\tHintValue = {}"),
		aBlockTypeName,
		aHintKey,
		aHintValue
	))
{
}
