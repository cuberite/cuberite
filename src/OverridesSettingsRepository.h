
#pragma once

#include "SettingsRepositoryInterface.h"

#include <unordered_map>

class cOverridesSettingsRepository : public cSettingsRepositoryInterface
{

public:
	cOverridesSettingsRepository(std::unique_ptr<cSettingsRepositoryInterface> a_Main, std::unique_ptr<cSettingsRepositoryInterface> a_Overrides);

	virtual ~cOverridesSettingsRepository() override = default;

	virtual bool KeyExists(const AString a_keyname) const override;

	virtual bool HasValue(const AString & a_KeyName, const AString & a_ValueName) const override;

	virtual int AddKeyName(const AString & a_keyname) override;

	virtual bool AddKeyComment(const AString & a_keyname, const AString & a_comment) override;

	virtual AString GetKeyComment(const AString & a_keyname, const int a_commentID) const override;

	virtual bool DeleteKeyComment(const AString & a_keyname, const int a_commentID) override;

	virtual void AddValue (const AString & a_KeyName, const AString & a_ValueName, const AString & a_Value) override;

	virtual std::vector<std::pair<AString, AString>> GetValues(AString a_keyName) override;

	virtual AString GetValue (const AString & a_keyname, const AString & a_valuename, const AString & a_defValue = "")    const override;

	virtual AString GetValueSet (const AString & a_keyname, const AString & a_valuename, const AString & a_defValue = "") override;
	virtual int     GetValueSetI(const AString & a_keyname, const AString & a_valuename, const int       a_defValue = 0) override;
	virtual Int64   GetValueSetI(const AString & a_keyname, const AString & a_valuename, const Int64     a_defValue = 0) override;
	virtual bool    GetValueSetB(const AString & a_keyname, const AString & a_valuename, const bool      a_defValue = false) override;

	virtual bool SetValue (const AString & a_KeyName, const AString & a_ValueName, const AString & a_Value, const bool a_CreateIfNotExists = true) override;
	virtual bool SetValueI(const AString & a_KeyName, const AString & a_ValueName, const int a_Value, const bool a_CreateIfNotExists = true) override;

	virtual bool DeleteValue(const AString & a_keyname, const AString & a_valuename) override;

	virtual bool Flush() override;

private:

	std::unique_ptr<cSettingsRepositoryInterface> m_Main;
	std::unique_ptr<cSettingsRepositoryInterface> m_Overrides;

};

