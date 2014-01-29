// IniFile.cpp:  Implementation of the CIniFile class.
// Written by:   Adam Clauss
// Email: cabadam@tamu.edu
// You may use this class/code as you wish in your programs.  Feel free to distribute it, and
// email suggested changes to me.
//
// Rewritten by: Shane Hill
// Date:         21/08/2001
// Email:        Shane.Hill@dsto.defence.gov.au
// Reason:       Remove dependancy on MFC. Code should compile on any
//               platform. Tested on Windows/Linux/Irix
//////////////////////////////////////////////////////////////////////

/*
!! MODIFIED BY FAKETRUTH and madmaxoft!!
*/

#ifndef CIniFile_H
#define CIniFile_H





#define MAX_KEYNAME    128
#define MAX_VALUENAME  128
#define MAX_VALUEDATA 2048





// tolua_begin

class cIniFile
{
private:
	bool m_IsCaseInsensitive;
	
	struct key
	{
		std::vector<AString> names;
		std::vector<AString> values;
		std::vector<AString> comments;
	} ;
	
	std::vector<key>     keys;
	std::vector<AString> names;
	std::vector<AString> comments;
	
	/// If the object is case-insensitive, returns s as lowercase; otherwise returns s as-is
	AString CheckCase(const AString & s) const;

public:
	enum errors
	{
		noID = -1,
	};
	
	/// Creates a new instance with no data
	cIniFile(void);

	// Sets whether or not keynames and valuenames should be case sensitive.
	// The default is case insensitive.
	void CaseSensitive  (void) { m_IsCaseInsensitive = false; }
	void CaseInsensitive(void) { m_IsCaseInsensitive = true; }

	/** Reads the contents of the specified ini file
	If the file doesn't exist and a_AllowExampleRedirect is true, tries to read <basename>.example.ini, and
	writes its contents as <basename>.ini, if successful.
	Returns true if successful, false otherwise.
	*/
	bool ReadFile(const AString & a_FileName, bool a_AllowExampleRedirect = true);

	/// Writes data stored in class to the specified ini file
	bool WriteFile(const AString & a_FileName) const;

	/// Deletes all stored ini data (but doesn't touch the file)
	void Clear(void);

	/// Returns index of specified key, or noID if not found
	int FindKey(const AString & keyname) const;

	/// Returns index of specified value, in the specified key, or noID if not found
	int FindValue(const int keyID, const AString & valuename) const;

	/// Returns number of keys currently in the ini
	int GetNumKeys(void) const { return (int)keys.size(); }

	/// Add a key name
	int AddKeyName(const AString & keyname);

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
	// Returns defValue if key/value not found.
	AString GetValue (const AString & keyname, const AString & valuename, const AString & defValue = "")    const;
	AString GetValue (const int keyID,    const int valueID,    const AString & defValue = "")    const;
	double  GetValueF(const AString & keyname, const AString & valuename, const double    defValue = 0)     const;
	int     GetValueI(const AString & keyname, const AString & valuename, const int       defValue = 0)     const;
	bool    GetValueB(const AString & keyname, const AString & valuename, const bool      defValue = false) const
	{
		return (GetValueI(keyname, valuename, defValue ? 1 : 0) != 0);
	}
	
	// Gets the value; if not found, write the default to the INI file
	AString GetValueSet (const AString & keyname, const AString & valuename, const AString & defValue = "");
	double  GetValueSetF(const AString & keyname, const AString & valuename, const double    defValue = 0.0);
	int     GetValueSetI(const AString & keyname, const AString & valuename, const int       defValue = 0);
	bool    GetValueSetB(const AString & keyname, const AString & valuename, const bool      defValue = false)
	{
		return (GetValueSetI(keyname, valuename, defValue ? 1 : 0) != 0);
	}

	// Adds a new value to the specified key.
	// If a value of the same name already exists, creates another one (non-standard INI file)
	void AddValue (const AString & a_KeyName, const AString & a_ValueName, const AString & a_Value);
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
	bool SetValue (const AString & a_KeyName, const AString & a_ValueName, const AString & a_Value, const bool a_CreateIfNotExists = true);
	bool SetValueI(const AString & a_KeyName, const AString & a_ValueName, const int a_Value, const bool a_CreateIfNotExists = true);
	bool SetValueB(const AString & a_KeyName, const AString & a_ValueName, const bool a_Value, const bool a_CreateIfNotExists = true)
	{
		return SetValueI(a_KeyName, a_ValueName, int(a_Value), a_CreateIfNotExists);
	}
	bool SetValueF(const AString & a_KeyName, const AString & a_ValueName, const double a_Value, const bool a_CreateIfNotExists = true);
	
	// tolua_end
	
	bool SetValueV( const AString & a_KeyName, const AString & a_ValueName, const char * a_Format, ...);
	
	// tolua_begin

	// Deletes specified value.
	// Returns true if value existed and deleted, false otherwise.
	bool DeleteValueByID(const int keyID, const int valueID);
	bool DeleteValue(const AString & keyname, const AString & valuename);

	// Deletes specified key and all values contained within.
	// Returns true if key existed and deleted, false otherwise.
	bool DeleteKey(const AString & keyname);

	// Header comment functions.
	// Header comments are those comments before the first key.

	/// Returns the number of header comments
	int GetNumHeaderComments(void) {return (int)comments.size();}
	
	/// Adds a header comment
	void AddHeaderComment(const AString & comment);
	
	/// Returns a header comment, or empty string if out of range
	AString GetHeaderComment(const int commentID) const;
	
	/// Deletes a header comment. Returns true if successful
	bool DeleteHeaderComment(int commentID);
	
	/// Deletes all header comments
	void DeleteHeaderComments(void) {comments.clear();}


	// Key comment functions.
	// Key comments are those comments within a key. Any comments
	// defined within value names will be added to this list. Therefore,
	// these comments will be moved to the top of the key definition when
	// the CIniFile::WriteFile() is called.

	/// Get number of key comments
	int GetNumKeyComments(const int keyID) const;

	/// Get number of key comments
	int GetNumKeyComments(const AString & keyname) const;
	
	/// Add a key comment
	bool AddKeyComment(const int keyID, const AString & comment);

	/// Add a key comment
	bool AddKeyComment(const AString & keyname, const AString & comment);
	
	/// Return a key comment
	AString GetKeyComment(const int keyID, const int commentID) const;
	AString GetKeyComment(const AString & keyname, const int commentID) const;
	
	// Delete a key comment.
	bool DeleteKeyComment(const int keyID, const int commentID);
	bool DeleteKeyComment(const AString & keyname, const int commentID);
	
	// Delete all comments for a key.
	bool DeleteKeyComments(const int keyID);
	bool DeleteKeyComments(const AString & keyname);
};

// tolua_end

#endif
