
#pragma once

#include "SettingsRepositoryInterface.h"

class cOverridesSettingsRepository : public cSettingsRepositoryInterface
{

public:
	cOverridesSettingsRepository(std::unique_ptr<cSettingsRepositoryInterface> a_Main, cSettingsRepositoryInterface & a_Overrides);

	virtual ~cOverridesSettingsRepository() override = default;

	virtual bool KeyExists(const AString keyname) const override;

	virtual bool HasValue(const AString & a_KeyName, const AString & a_ValueName) const override;

	virtual int AddKeyName(const AString & keyname) override;

	virtual bool AddKeyComment(const AString & keyname, const AString & comment) override;

	virtual AString GetKeyComment(const AString & keyname, const int commentID) const override;

	virtual bool DeleteKeyComment(const AString & keyname, const int commentID) override;

	virtual void AddValue (const AString & a_KeyName, const AString & a_ValueName, const AString & a_Value) override;

	virtual std::vector<std::pair<AString, AString>> GetValues(AString a_keyName) override;

	virtual AString GetValue (const AString & keyname, const AString & valuename, const AString & defValue = "")    const override;

	virtual AString GetValueSet (const AString & keyname, const AString & valuename, const AString & defValue = "") override;
	virtual int     GetValueSetI(const AString & keyname, const AString & valuename, const int       defValue = 0) override;
	virtual Int64   GetValueSetI(const AString & keyname, const AString & valuename, const Int64     defValue = 0) override;
	virtual bool    GetValueSetB(const AString & keyname, const AString & valuename, const bool      defValue = false) override;

	virtual bool SetValue (const AString & a_KeyName, const AString & a_ValueName, const AString & a_Value, const bool a_CreateIfNotExists = true) override;
	virtual bool SetValueI(const AString & a_KeyName, const AString & a_ValueName, const int a_Value, const bool a_CreateIfNotExists = true) override;

	virtual bool DeleteValue(const AString & keyname, const AString & valuename) override;

	virtual bool Flush() override;

private:

	std::unique_ptr<cSettingsRepositoryInterface> m_Main;
	cSettingsRepositoryInterface * m_Overrides;

};

