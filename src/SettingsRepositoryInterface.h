
#pragma once

class cSettingsRepositoryInterface
{
public:

	enum errors
	{
		noID = -1,
	};

	virtual ~cSettingsRepositoryInterface() = default;

	/** Returns true iff the specified key exists */
	virtual bool KeyExists(const AString a_keyname) const = 0;

	/** Returns true iff the specified value exists. */
	virtual bool HasValue(const AString & a_KeyName, const AString & a_ValueName) const = 0;

	/** Add a key name. Return value is not required to mean anything */
	virtual int AddKeyName(const AString & a_keyname) = 0;

	/** Add a key comment, will always fail if the repository does not support comments */
	virtual bool AddKeyComment(const AString & a_keyname, const AString & a_comment) = 0;

	/** Return a key comment, returns "" for repositories that do not return comments */
	virtual AString GetKeyComment(const AString & a_keyname, const int a_commentID) const = 0;

	/** Delete a key comment, will always fail if the repository does not support comments */
	virtual bool DeleteKeyComment(const AString & a_keyname, const int a_commentID) = 0;

	/** Adds a new value to the specified key.
	If a value of the same name already exists, creates another one */
	virtual void AddValue (const AString & a_KeyName, const AString & a_ValueName, const AString & a_Value) = 0;

	/** returns a vector containing a name, value pair for each value under the key */
	virtual std::vector<std::pair<AString, AString>> GetValues(AString a_keyName) = 0;

	/** Get the value at the specified key and value, returns defValue on failure */
	virtual AString GetValue (const AString & a_keyname, const AString & a_valuename, const AString & a_defValue = "")    const = 0;

	/** Gets the value; if not found, write the default to the repository */
	virtual AString GetValueSet (const AString & a_keyname, const AString & a_valuename, const AString & a_defValue = "") = 0;
	virtual int     GetValueSetI(const AString & a_keyname, const AString & a_valuename, const int       a_defValue = 0) = 0;
	virtual Int64   GetValueSetI(const AString & a_keyname, const AString & a_valuename, const Int64     a_defValue = 0) = 0;
	virtual bool    GetValueSetB(const AString & a_keyname, const AString & a_valuename, const bool      a_defValue = false) = 0;

	/** Overwrites the value of the key, value pair
	Specify the optional parameter as false if you do not want the value created if it doesn't exist.
	Returns true if value set, false otherwise. */
	virtual bool SetValue (const AString & a_KeyName, const AString & a_ValueName, const AString & a_Value, const bool a_CreateIfNotExists = true) = 0;
	virtual bool SetValueI(const AString & a_KeyName, const AString & a_ValueName, const int a_Value, const bool a_CreateIfNotExists = true) = 0;

	/** Deletes the specified key, value pair */
	virtual bool DeleteValue(const AString & a_keyname, const AString & a_valuename) = 0;


	/** Writes the changes to the backing store, if the repository has one */
	virtual bool Flush() = 0;
};
