
#pragma once

class cSettingsRepositoryInterface
{
public:

	enum errors
	{
		noID = -1,
	};

	virtual ~cSettingsRepositoryInterface() = default;

	virtual bool KeyExists(const AString keyname) const = 0;

	virtual bool HasValue(const AString & a_KeyName, const AString & a_ValueName) const = 0;

	virtual int AddKeyName(const AString & keyname) = 0;

	virtual bool AddKeyComment(const AString & keyname, const AString & comment) = 0;
	
	virtual AString GetKeyComment(const AString & keyname, const int commentID) const = 0;

	virtual bool DeleteKeyComment(const AString & keyname, const int commentID) = 0;

	virtual void AddValue (const AString & a_KeyName, const AString & a_ValueName, const AString & a_Value) = 0;

	virtual std::vector<std::pair<AString, AString>> GetValues(AString a_keyName) = 0;

	virtual AString GetValue (const AString & keyname, const AString & valuename, const AString & defValue = "")    const = 0;

	virtual AString GetValueSet (const AString & keyname, const AString & valuename, const AString & defValue = "") = 0;
	virtual int     GetValueSetI(const AString & keyname, const AString & valuename, const int       defValue = 0) = 0;
	virtual Int64   GetValueSetI(const AString & keyname, const AString & valuename, const Int64     defValue = 0) = 0;
	virtual bool    GetValueSetB(const AString & keyname, const AString & valuename, const bool      defValue = false) = 0;

	virtual bool SetValue (const AString & a_KeyName, const AString & a_ValueName, const AString & a_Value, const bool a_CreateIfNotExists = true) = 0;
	virtual bool SetValueI(const AString & a_KeyName, const AString & a_ValueName, const int a_Value, const bool a_CreateIfNotExists = true) = 0;


	virtual bool DeleteValue(const AString & keyname, const AString & valuename) = 0;


	virtual bool Flush() = 0;
};
