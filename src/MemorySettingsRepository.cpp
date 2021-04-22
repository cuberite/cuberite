
#include "Globals.h"

#include "MemorySettingsRepository.h"




bool cMemorySettingsRepository::KeyExists(const AString keyname) const
{
	return m_Map.count(keyname) != 0;
}





bool cMemorySettingsRepository::HasValue(const AString & a_KeyName, const AString & a_ValueName) const
{
	auto outerIter = m_Map.find(a_KeyName);
	if (outerIter == m_Map.end())
	{
		return false;
	}
	auto iter = outerIter->second.find(a_ValueName);
	return (iter != outerIter->second.end());
}





int cMemorySettingsRepository::AddKeyName(const AString & a_keyname)
{
	m_Map.emplace(a_keyname, std::unordered_multimap<AString, sValue>{});
	return 0;
}





bool cMemorySettingsRepository::AddKeyComment(const AString & keyname, const AString & comment)
{
	return false;
}





AString cMemorySettingsRepository::GetKeyComment(const AString & keyname, const int commentID) const
{
	return "";
}





bool cMemorySettingsRepository::DeleteKeyComment(const AString & keyname, const int commentID)
{
	return false;
}





void cMemorySettingsRepository::AddValue (const AString & a_KeyName, const AString & a_ValueName, const AString & a_Value)
{
	if (m_Writable)
	{
		m_Map[a_KeyName].emplace(a_ValueName, sValue(a_Value));
	}
}





void cMemorySettingsRepository::AddValue (const AString & a_KeyName, const AString & a_ValueName, Int64 a_Value)
{
	if (m_Writable)
	{
		m_Map[a_KeyName].emplace(a_ValueName, sValue(a_Value));
	}
}





void cMemorySettingsRepository::AddValue (const AString & a_KeyName, const AString & a_ValueName, bool a_Value)
{
	if (m_Writable)
	{
		m_Map[a_KeyName].emplace(a_ValueName, sValue(a_Value));
	}
}





std::vector<std::pair<AString, AString>> cMemorySettingsRepository::GetValues(AString a_keyName)
{
	std::vector<std::pair<AString, AString>> ret;
	for (const auto & pair : m_Map[a_keyName])
	{
		ret.emplace_back(pair.first, pair.second.getStringValue());
	}
	return ret;
}





AString cMemorySettingsRepository::GetValue (const AString & a_KeyName, const AString & a_ValueName, const AString & defValue)    const
{
	auto outerIter = m_Map.find(a_KeyName);
	if (outerIter == m_Map.end())
	{
		return defValue;
	}
	auto iter = outerIter->second.find(a_ValueName);
	if (iter == outerIter->second.end())
	{
		return defValue;
	}
	return iter->second.getStringValue();
}





AString cMemorySettingsRepository::GetValueSet (const AString & a_KeyName, const AString & a_ValueName, const AString & defValue)
{
	auto outerIter = m_Map.find(a_KeyName);
	if (outerIter == m_Map.end())
	{
		AddValue(a_KeyName, a_ValueName, defValue);
		return defValue;
	}
	auto iter = outerIter->second.find(a_ValueName);
	if (iter == outerIter->second.end())
	{
		AddValue(a_KeyName, a_ValueName, defValue);
		return defValue;
	}
	return iter->second.getStringValue();
}





int cMemorySettingsRepository::GetValueSetI(const AString & a_KeyName, const AString & a_ValueName, const int defValue)
{
	auto outerIter = m_Map.find(a_KeyName);
	if (outerIter == m_Map.end())
	{
		AddValue(a_KeyName, a_ValueName, static_cast<Int64>(defValue));
		return defValue;
	}
	auto iter = outerIter->second.find(a_ValueName);
	if (iter == outerIter->second.end())
	{
		AddValue(a_KeyName, a_ValueName, static_cast<Int64>(defValue));
		return defValue;
	}
	return static_cast<int>(iter->second.getIntValue());
}





Int64 cMemorySettingsRepository::GetValueSetI(const AString & a_KeyName, const AString & a_ValueName, const Int64 defValue)
{
	auto outerIter = m_Map.find(a_KeyName);
	if (outerIter == m_Map.end())
	{
		AddValue(a_KeyName, a_ValueName, defValue);
		return defValue;
	}
	auto iter = outerIter->second.find(a_ValueName);
	if (iter == outerIter->second.end())
	{
		AddValue(a_KeyName, a_ValueName, defValue);
		return defValue;
	}
	return iter->second.getIntValue();
}





bool cMemorySettingsRepository::GetValueSetB(const AString & a_KeyName, const AString & a_ValueName, const bool defValue)
{
	auto outerIter = m_Map.find(a_KeyName);
	if (outerIter == m_Map.end())
	{
		AddValue(a_KeyName, a_ValueName, defValue);
		return defValue;
	}
	auto iter = outerIter->second.find(a_ValueName);
	if (iter == outerIter->second.end())
	{
		AddValue(a_KeyName, a_ValueName, defValue);
		return defValue;
	}
	return iter->second.getBoolValue();
}





bool cMemorySettingsRepository::SetValue (const AString & a_KeyName, const AString & a_ValueName, const AString & a_Value, const bool a_CreateIfNotExists)
{
	if (!m_Writable)
	{
		return false;
	}
	auto outerIter = m_Map.find(a_KeyName);
	if (outerIter == m_Map.end())
	{
		if (a_CreateIfNotExists)
		{
			AddValue(a_KeyName, a_ValueName, a_Value);
		}
		return a_CreateIfNotExists;
	}
	auto iter = outerIter->second.find(a_ValueName);
	if (iter == outerIter->second.end())
	{
		if (a_CreateIfNotExists)
		{
			AddValue(a_KeyName, a_ValueName, a_Value);
		}
		return a_CreateIfNotExists;
	}
	iter->second = sValue(a_Value);
	return true;
}





bool cMemorySettingsRepository::SetValueI(const AString & a_KeyName, const AString & a_ValueName, const int a_Value, const bool a_CreateIfNotExists)
{
	if (!m_Writable)
	{
		return false;
	}
	auto outerIter = m_Map.find(a_KeyName);
	if (outerIter == m_Map.end())
	{
		if (a_CreateIfNotExists)
		{
			AddValue(a_KeyName, a_ValueName, static_cast<Int64>(a_Value));
		}
		return a_CreateIfNotExists;
	}
	auto iter = outerIter->second.find(a_ValueName);
	if (iter == outerIter->second.end())
	{
		if (a_CreateIfNotExists)
		{
			AddValue(a_KeyName, a_ValueName, static_cast<Int64>(a_Value));
		}
		return a_CreateIfNotExists;
	}
	iter->second = sValue(static_cast<Int64>(a_Value));
	return true;
}





bool cMemorySettingsRepository::DeleteValue(const AString & a_KeyName, const AString & a_ValueName)
{
	if (!m_Writable)
	{
		return false;
	}
	auto outerIter = m_Map.find(a_KeyName);
	if (outerIter == m_Map.end())
	{
		return false;
	}
	auto iter = outerIter->second.find(a_ValueName);
	if (iter == outerIter->second.end())
	{
		return false;
	}
	outerIter->second.erase(iter);
	return true;
}





bool cMemorySettingsRepository::Flush()
{
	return true;
}

