// IniFile.cpp:  Implementation of the CIniFile class.
// Written by:   Adam Clauss
// Email: cabadam@tamu.edu
// You may use this class / code as you wish in your programs.  Feel free to distribute it, and
// email suggested changes to me.
//
// Rewritten by: Shane Hill
// Date:         2001-08-21
// Email:        Shane.Hill@dsto.defence.gov.au
// Reason:       Remove dependancy on MFC. Code should compile on any
//               platform. Tested on Windows / Linux / Irix
////////////////////////////////////////////////////////////////////////////////

/*
!! MODIFIED BY FAKETRUTH and madmaxoft!!
*/

#pragma once


#include "SettingsRepositoryInterface.h"

#define MAX_KEYNAME    128
#define MAX_VALUENAME  128
#define MAX_VALUEDATA 2048





// tolua_begin

class cIniFile:
	public cSettingsRepositoryInterface
{
private:

	// tolua_end

	using Super = cSettingsRepositoryInterface;


	bool m_IsCaseInsensitive;

	AString m_Filename;

	struct key
	{
		std::vector<AString> m_Names;
		std::vector<AString> m_Values;
		std::vector<AString> m_Comments;
	} ;

	std::vector<key>     m_Keys;
	std::vector<AString> m_Names;
	std::vector<AString> m_Comments;

	/** If the object is case-insensitive, returns s as lowercase; otherwise returns s as-is */
	AString CheckCase(const AString & s) const;

	/** Removes the UTF-8 BOMs (Byte order makers), if present. */
	void RemoveBom(AString & a_line) const;

	// tolua_begin

public:

	// NOTE: This has to be present for ToLua++'s parser to output the noID constant into the API
	// We don't want to export the entire base class, so the constant needs to get pulled into this descendant
	enum
	{
		noID = Super::noID,
	};


	/** Creates a new instance with no data */
	cIniFile(void);

	// tolua_end
	virtual ~cIniFile() override = default;

	virtual std::vector<std::pair<AString, AString>> GetValues(AString a_keyName) override;

	virtual bool KeyExists(const AString a_keyName) const override;

	// tolua_begin

	// Sets whether or not keynames and valuenames should be case sensitive.
	// The default is case insensitive.
	void CaseSensitive  (void) { m_IsCaseInsensitive = false; }
	void CaseInsensitive(void) { m_IsCaseInsensitive = true; }

	/** Reads the contents of the specified ini file
	If the file doesn't exist and a_AllowExampleRedirect is true, tries to read <basename>.example.ini, and
	writes its contents as <basename>.ini, if successful.
	Returns true if successful, false otherwise. */
	bool ReadFile(const AString & a_FileName, bool a_AllowExampleRedirect = true);

	/** Writes data stored in class to the specified ini file.
	Returns true on success, false on failure. */
	bool WriteFile(const AString & a_FileName) const;

	virtual bool Flush() override { return WriteFile(m_Filename); }

	/** Deletes all stored ini data (but doesn't touch the file) */
	void Clear(void);

	/** Returns true iff the specified value exists. */
	bool HasValue(const AString & a_KeyName, const AString & a_ValueName) const override;

	/** Returns index of specified key, or noID if not found */
	int FindKey(const AString & keyname) const;

	/** Returns index of specified value, in the specified key, or noID if not found */
	int FindValue(const int keyID, const AString & valuename) const;

	/** Returns number of keys currently in the ini */
	int GetNumKeys(void) const { return static_cast<int>(m_Keys.size()); }

	/** Add a key name */
	int AddKeyName(const AString & keyname) override;

	// Returns key names by index.
	AString GetKeyName(const int keyID) const;

	// Returns number of values stored for specified key.
	int GetNumValues(const AString & keyname) const;
	int GetNumValues(const int keyID) const;

	// Returns value name by index for a given keyname or keyID.
	AString GetValueName(const AString & keyname, const int valueID) const;
	AString GetValueName(const int keyID, const int valueID) const;

	// Gets value of [keyname] valuename =.
	// Overloaded to return string, int, and double.
	// Returns defValue if key / value not found.
	AString GetValue (const AString & keyname, const AString & valuename, const AString & defValue = "")    const override;
	AString GetValue (const int keyID,    const int valueID,    const AString & defValue = "")    const;
	double  GetValueF(const AString & keyname, const AString & valuename, const double    defValue = 0)     const;
	int     GetValueI(const AString & keyname, const AString & valuename, const int       defValue = 0)     const;
	bool    GetValueB(const AString & keyname, const AString & valuename, const bool      defValue = false) const
	{
		return (GetValueI(keyname, valuename, defValue ? 1 : 0) != 0);
	}

	// Gets the value; if not found, write the default to the INI file
	AString GetValueSet (const AString & keyname, const AString & valuename, const AString & defValue = "") override;
	double  GetValueSetF(const AString & keyname, const AString & valuename, const double    defValue = 0.0);
	int     GetValueSetI(const AString & keyname, const AString & valuename, const int       defValue = 0) override;
	Int64   GetValueSetI(const AString & keyname, const AString & valuename, const Int64     defValue = 0) override;
	bool    GetValueSetB(const AString & keyname, const AString & valuename, const bool      defValue = false) override
	{
		return (GetValueSetI(keyname, valuename, defValue ? 1 : 0) != 0);
	}

	// Adds a new value to the specified key.
	// If a value of the same name already exists, creates another one (non-standard INI file)
	void AddValue (const AString & a_KeyName, const AString & a_ValueName, const AString & a_Value) override;
	void AddValueI(const AString & a_KeyName, const AString & a_ValueName, const int a_Value);
	void AddValueB(const AString & a_KeyName, const AString & a_ValueName, const bool a_Value)
	{
		return AddValueI(a_KeyName, a_ValueName, a_Value ? 1 : 0);
	}
	void AddValueF(const AString & a_KeyName, const AString & a_ValueName, const double a_Value);

	// Overwrites the value of [keyname].valuename
	// Specify the optional parameter as false (0) if you do not want the value created if it doesn't exist.
	// Returns true if value set, false otherwise.
	// Overloaded to accept string, int, and double.
	bool SetValue (const int keyID, const int valueID, const AString & value);
	bool SetValue (const AString & a_KeyName, const AString & a_ValueName, const AString & a_Value, const bool a_CreateIfNotExists = true) override;
	bool SetValueI(const AString & a_KeyName, const AString & a_ValueName, const int a_Value, const bool a_CreateIfNotExists = true) override;
	bool SetValueI(const AString & a_Keyname, const AString & a_ValueName, const Int64 a_Value, const bool a_CreateIfNotExists = true);
	bool SetValueB(const AString & a_KeyName, const AString & a_ValueName, const bool a_Value, const bool a_CreateIfNotExists = true)
	{
		return SetValueI(a_KeyName, a_ValueName, int(a_Value), a_CreateIfNotExists);
	}
	bool SetValueF(const AString & a_KeyName, const AString & a_ValueName, const double a_Value, const bool a_CreateIfNotExists = true);

	// Deletes specified value.
	// Returns true if value existed and deleted, false otherwise.
	bool DeleteValueByID(const int keyID, const int valueID);
	bool DeleteValue(const AString & keyname, const AString & valuename) override;

	// Deletes specified key and all values contained within.
	// Returns true if key existed and deleted, false otherwise.
	bool DeleteKey(const AString & keyname);

	// Header comment functions.
	// Header comments are those comments before the first key.

	/** Returns the number of header comments */
	int GetNumHeaderComments(void) {return static_cast<int>(m_Comments.size());}

	/** Adds a header comment */
	void AddHeaderComment(const AString & comment);

	/** Returns a header comment, or empty string if out of range */
	AString GetHeaderComment(const int commentID) const;

	/** Deletes a header comment. Returns true if successful */
	bool DeleteHeaderComment(int commentID);

	/** Deletes all header comments */
	void DeleteHeaderComments(void) {m_Comments.clear();}


	// Key comment functions.
	// Key comments are those comments within a key. Any comments
	// defined within value names will be added to this list. Therefore,
	// these comments will be moved to the top of the key definition when
	// the CIniFile::WriteFile() is called.

	/** Get number of key comments */
	int GetNumKeyComments(const int keyID) const;

	/** Get number of key comments */
	int GetNumKeyComments(const AString & keyname) const;

	/** Add a key comment */
	bool AddKeyComment(const int keyID, const AString & comment);

	/** Add a key comment */
	bool AddKeyComment(const AString & keyname, const AString & comment) override;

	/** Return a key comment */
	AString GetKeyComment(const int keyID, const int commentID) const;
	AString GetKeyComment(const AString & keyname, const int commentID) const override;

	// Delete a key comment.
	bool DeleteKeyComment(const int keyID, const int commentID);
	bool DeleteKeyComment(const AString & keyname, const int commentID) override;

	// Delete all comments for a key.
	bool DeleteKeyComments(const int keyID);
	bool DeleteKeyComments(const AString & keyname);
};

// tolua_end





/** Reads the list of ports from the INI file, possibly upgrading from IPv4 / IPv6-specific values into new version-agnostic value.
Reads the list of ports from a_PortsValueName. If that value doesn't exist or is empty, the list is combined from values
in a_OldIPv4ValueName and a_OldIPv6ValueName; in this case the old values are removed from the INI file.
If there is none of the three values or they are all empty, the default is used and stored in the Ports value. */
AStringVector ReadUpgradeIniPorts(
	cSettingsRepositoryInterface & a_Settings,
	const AString & a_KeyName,
	const AString & a_PortsValueName,
	const AString & a_OldIPv4ValueName,
	const AString & a_OldIPv6ValueName,
	const AString & a_DefaultValue
);



