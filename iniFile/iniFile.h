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
	bool        m_IsCaseInsensitive;
	std::string m_Path;
	
	struct key
	{
		std::vector<std::string> names;
		std::vector<std::string> values; 
		std::vector<std::string> comments;
	} ;
	
	std::vector<key>         keys;
	std::vector<std::string> names;
	std::vector<std::string> comments;
	
	/// If the object is case-insensitive, returns s as lowercase; otherwise returns s as-is
	std::string CheckCase(const std::string & s) const;

public:
	enum errors
	{
		noID = -1,
	};
	
	/// Creates a new instance; sets m_Path to a_Path, but doesn't read the file
	cIniFile(const std::string & a_Path = "");

	// Sets whether or not keynames and valuenames should be case sensitive.
	// The default is case insensitive.
	void CaseSensitive  (void) { m_IsCaseInsensitive = false; }
	void CaseInsensitive(void) { m_IsCaseInsensitive = true; }

	// Sets path of ini file to read and write from.
	void Path(const std::string & newPath)    {m_Path = newPath;}
	const std::string & Path(void) const      {return m_Path;}
	void SetPath(const std::string & newPath) {Path(newPath);}

	/** Reads the ini file specified in m_Path
	If the file doesn't exist and a_AllowExampleRedirect is true, tries to read <basename>.example.ini, and
	writes its contents as <basename>.ini, if successful.
	Returns true if successful, false otherwise.
	*/
	bool ReadFile(bool a_AllowExampleRedirect = true);

	/// Writes data stored in class to ini file specified in m_Path
	bool WriteFile(void) const;

	/// Deletes all stored ini data (but doesn't touch the file)
	void Clear(void);
	void Reset(void) { Clear(); }
	void Erase(void) { Clear(); }  // OBSOLETE, this name is misguiding and will be removed from the interface

	/// Returns index of specified key, or noID if not found
	long FindKey(const std::string & keyname) const;

	/// Returns index of specified value, in the specified key, or noID if not found
	long FindValue(const unsigned keyID, const std::string & valuename) const;

	/// Returns number of keys currently in the ini
	unsigned NumKeys   (void) const {return names.size();}
	unsigned GetNumKeys(void) const {return NumKeys();}

	/// Add a key name
	unsigned AddKeyName(const std::string & keyname);

	// Returns key names by index.
	std::string KeyName(const unsigned keyID) const;
	std::string GetKeyName(const unsigned keyID) const {return KeyName(keyID);}

	// Returns number of values stored for specified key.
	unsigned NumValues   (const std::string & keyname);
	unsigned GetNumValues(const std::string & keyname) {return NumValues(keyname);}
	unsigned NumValues   (const unsigned keyID);
	unsigned GetNumValues(const unsigned keyID) {return NumValues(keyID);}

	// Returns value name by index for a given keyname or keyID.
	std::string ValueName( const std::string & keyname, const unsigned valueID) const;
	std::string GetValueName( const std::string & keyname, const unsigned valueID) const
	{
		return ValueName(keyname, valueID);
	}
	std::string ValueName   (const unsigned keyID, const unsigned valueID) const;
	std::string GetValueName(const unsigned keyID, const unsigned valueID) const
	{
		return ValueName(keyID, valueID);
	}

	// Gets value of [keyname] valuename =.
	// Overloaded to return string, int, and double.
	// Returns defValue if key/value not found.
	AString GetValue (const AString & keyname, const AString & valuename, const AString & defValue = "")    const;
	AString GetValue (const unsigned keyID,    const unsigned valueID,    const AString & defValue = "")    const;
	double  GetValueF(const AString & keyname, const AString & valuename, const double    defValue = 0)     const;
	int     GetValueI(const AString & keyname, const AString & valuename, const int       defValue = 0)     const;
	bool    GetValueB(const AString & keyname, const AString & valuename, const bool      defValue = false) const
	{
		return (GetValueI(keyname, valuename, int(defValue)) > 0);
	}
	
	// Gets the value; if not found, write the default to the INI file
	AString GetValueSet (const AString & keyname, const AString & valuename, const AString & defValue = "");
	double  GetValueSetF(const AString & keyname, const AString & valuename, const double    defValue = 0.0);
	int     GetValueSetI(const AString & keyname, const AString & valuename, const int       defValue = 0);
	bool    GetValueSetB(const AString & keyname, const AString & valuename, const bool      defValue = false)
	{
		return (GetValueSetI(keyname, valuename, defValue ? 1 : 0) > 0);
	}

	// Sets value of [keyname] valuename =.
	// Specify the optional paramter as false (0) if you do not want it to create
	// the key if it doesn't exist. Returns true if data entered, false otherwise.
	// Overloaded to accept string, int, and double.
	bool SetValue( const unsigned keyID, const unsigned valueID, const std::string & value);
	bool SetValue( const std::string & keyname, const std::string & valuename, const std::string & value, const bool create = true);
	bool SetValueI( const std::string & keyname, const std::string & valuename, const int value, const bool create = true);
	bool SetValueB( const std::string & keyname, const std::string & valuename, const bool value, const bool create = true)
	{
		return SetValueI( keyname, valuename, int(value), create);
	}
	bool SetValueF( const std::string & keyname, const std::string & valuename, const double value, const bool create = true);
	
	// tolua_end
	
	bool SetValueV( const std::string & keyname, const std::string & valuename, char *format, ...);
	
	// tolua_begin

	// Deletes specified value.
	// Returns true if value existed and deleted, false otherwise.
	bool DeleteValueByID( const unsigned keyID, const unsigned valueID );
	bool DeleteValue( const std::string & keyname, const std::string & valuename);

	// Deletes specified key and all values contained within.
	// Returns true if key existed and deleted, false otherwise.
	bool DeleteKey(const std::string & keyname);

	// Header comment functions.
	// Header comments are those comments before the first key.
	//
	// Number of header comments.
	unsigned NumHeaderComments(void) {return comments.size();}
	// Add a header comment.
	void     HeaderComment(const std::string & comment);
	// Return a header comment.
	std::string   HeaderComment(const unsigned commentID) const;
	// Delete a header comment.
	bool     DeleteHeaderComment(unsigned commentID);
	// Delete all header comments.
	void     DeleteHeaderComments(void) {comments.clear();}


	// Key comment functions.
	// Key comments are those comments within a key. Any comments
	// defined within value names will be added to this list. Therefore,
	// these comments will be moved to the top of the key definition when
	// the CIniFile::WriteFile() is called.
	//
	// Number of key comments.
	unsigned NumKeyComments( const unsigned keyID) const;
	unsigned NumKeyComments( const std::string & keyname) const;
	
	// Add a key comment.
	bool     KeyComment(const unsigned keyID, const std::string & comment);
	bool     KeyComment(const std::string & keyname, const std::string & comment);
	
	// Return a key comment.
	std::string   KeyComment(const unsigned keyID, const unsigned commentID) const;
	std::string   KeyComment(const std::string & keyname, const unsigned commentID) const;
	
	// Delete a key comment.
	bool     DeleteKeyComment(const unsigned keyID, const unsigned commentID);
	bool     DeleteKeyComment(const std::string & keyname, const unsigned commentID);
	
	// Delete all comments for a key.
	bool     DeleteKeyComments(const unsigned keyID);
	bool     DeleteKeyComments(const std::string & keyname);
};

// tolua_end

#endif
