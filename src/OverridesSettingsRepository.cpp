
#include "Globals.h"
#include "OverridesSettingsRepository.h"

cOverridesSettingsRepository::cOverridesSettingsRepository(std::unique_ptr<cSettingsRepositoryInterface> a_Main, cSettingsRepositoryInterface & a_Overrides) :
	m_Main(std::move(a_Main)),
	m_Overrides(&a_Overrides)
{
}





bool cOverridesSettingsRepository::KeyExists(const AString a_keyName) const
{
	return m_Overrides->KeyExists(a_keyName) || m_Main->KeyExists(a_keyName);
}





bool cOverridesSettingsRepository::HasValue(const AString & a_KeyName, const AString & a_ValueName) const
{
	return m_Overrides->HasValue(a_KeyName, a_ValueName) || m_Main->HasValue(a_KeyName, a_ValueName);
}





int cOverridesSettingsRepository::AddKeyName(const AString & a_keyname)
{

	if (m_Overrides->KeyExists(a_keyname))
	{
		m_Overrides->AddKeyName(a_keyname);
		return 0;
	}

	return m_Main->AddKeyName(a_keyname);
}





bool cOverridesSettingsRepository::AddKeyComment(const AString & a_keyname, const AString & a_comment)
{
	if (m_Overrides->KeyExists(a_keyname))
	{
		return m_Overrides->AddKeyComment(a_keyname, a_comment);
	}

	return m_Main->AddKeyComment(a_keyname, a_comment);
}





AString cOverridesSettingsRepository::GetKeyComment(const AString & a_keyname, const int a_commentID) const
{

	if (m_Overrides->KeyExists(a_keyname))
	{
		return m_Overrides->GetKeyComment(a_keyname, a_commentID);
	}

	return m_Main->GetKeyComment(a_keyname, a_commentID);
}





bool cOverridesSettingsRepository::DeleteKeyComment(const AString & a_keyname, const int a_commentID)
{
	if (m_Overrides->KeyExists(a_keyname))
	{
		return m_Overrides->DeleteKeyComment(a_keyname, a_commentID);
	}

	return m_Main->DeleteKeyComment(a_keyname, a_commentID);
}





void cOverridesSettingsRepository::AddValue (const AString & a_KeyName, const AString & a_ValueName, const AString & a_Value)
{
	if (m_Overrides->HasValue(a_KeyName, a_ValueName))
	{
		m_Overrides->AddValue(a_KeyName, a_ValueName, a_Value);
	}
	else
	{
		m_Main->AddValue(a_KeyName, a_ValueName, a_Value);
	}
}





std::vector<std::pair<AString, AString>> cOverridesSettingsRepository::GetValues(AString a_keyName)
{
	auto overrides = m_Overrides->GetValues(a_keyName);
	auto main = m_Main->GetValues(a_keyName);

	auto ret = overrides;

	for (const auto & mainpair : main)
	{
		bool found = false;
		for (const auto & overridepair : overrides)
		{
			if (overridepair.first == mainpair.first)
			{
				found = true;
				break;
			}
		}
		if (!found)
		{
			ret.push_back(mainpair);
		}
	}
	return ret;
}





AString cOverridesSettingsRepository::GetValue(const AString & a_KeyName, const AString & a_ValueName, const AString & defValue) const
{
	if (m_Overrides->HasValue(a_KeyName, a_ValueName))
	{
		return m_Overrides->GetValue(a_KeyName, a_ValueName, defValue);
	}
	else
	{
		return m_Main->GetValue(a_KeyName, a_ValueName, defValue);
	}
}





AString cOverridesSettingsRepository::GetValueSet (const AString & a_KeyName, const AString & a_ValueName, const AString & defValue)
{
	if (m_Overrides->HasValue(a_KeyName, a_ValueName))
	{
		return m_Overrides->GetValueSet(a_KeyName, a_ValueName, defValue);
	}
	else
	{
		return m_Main->GetValueSet(a_KeyName, a_ValueName, defValue);
	}
}





int cOverridesSettingsRepository::GetValueSetI(const AString & a_KeyName, const AString & a_ValueName, const int defValue)
{
	if (m_Overrides->HasValue(a_KeyName, a_ValueName))
	{
		return m_Overrides->GetValueSetI(a_KeyName, a_ValueName, defValue);
	}
	else
	{
		return m_Main->GetValueSetI(a_KeyName, a_ValueName, defValue);
	}
}





Int64 cOverridesSettingsRepository::GetValueSetI(const AString & a_KeyName, const AString & a_ValueName, const Int64 defValue)
{
	if (m_Overrides->HasValue(a_KeyName, a_ValueName))
	{
		return m_Overrides->GetValueSetI(a_KeyName, a_ValueName, defValue);
	}
	else
	{
		return m_Main->GetValueSetI(a_KeyName, a_ValueName, defValue);
	}
}





bool cOverridesSettingsRepository::GetValueSetB(const AString & a_KeyName, const AString & a_ValueName, const bool defValue)
{
	if (m_Overrides->HasValue(a_KeyName, a_ValueName))
	{
		return m_Overrides->GetValueSetB(a_KeyName, a_ValueName, defValue);
	}
	else
	{
		return m_Main->GetValueSetB(a_KeyName, a_ValueName, defValue);
	}
}





bool cOverridesSettingsRepository::SetValue (const AString & a_KeyName, const AString & a_ValueName, const AString & a_Value, const bool a_CreateIfNotExists)
{
	if (m_Overrides->HasValue(a_KeyName, a_ValueName))
	{
		return m_Overrides->SetValue(a_KeyName, a_ValueName, a_Value, a_CreateIfNotExists);
	}
	else
	{
		return m_Main->SetValue(a_KeyName, a_ValueName, a_Value, a_CreateIfNotExists);
	}
}





bool cOverridesSettingsRepository::SetValueI(const AString & a_KeyName, const AString & a_ValueName, const int a_Value, const bool a_CreateIfNotExists)
{
	if (m_Overrides->HasValue(a_KeyName, a_ValueName))
	{
		return m_Overrides->SetValueI(a_KeyName, a_ValueName, a_Value, a_CreateIfNotExists);
	}
	else
	{
		return m_Main->SetValueI(a_KeyName, a_ValueName, a_Value, a_CreateIfNotExists);
	}
}





bool cOverridesSettingsRepository::DeleteValue(const AString & a_KeyName, const AString & a_ValueName)
{
	if (m_Overrides->HasValue(a_KeyName, a_ValueName))
	{
		return m_Overrides->DeleteValue(a_KeyName, a_ValueName);
	}
	else
	{
		return m_Main->DeleteValue(a_KeyName, a_ValueName);
	}
}





bool cOverridesSettingsRepository::Flush()
{
	return m_Overrides->Flush() && m_Main->Flush();
}

