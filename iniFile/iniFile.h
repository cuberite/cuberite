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





class cIniFile							//tolua_export
{										//tolua_export
private:
	bool   caseInsensitive;
	std::string path;
	struct key {
		std::vector<std::string> names;
		std::vector<std::string> values; 
		std::vector<std::string> comments;
	};
	std::vector<key>    keys; 
	std::vector<std::string> names; 
	std::vector<std::string> comments;
	std::string CheckCase( std::string s) const;

public:
	enum errors{ noID = -1};									//tolua_export
	cIniFile( const std::string iniPath = "");				//tolua_export
	virtual ~cIniFile()                            {}

	// Sets whether or not keynames and valuenames should be case sensitive.
	// The default is case insensitive.
	void CaseSensitive()                           {caseInsensitive = false;}	//tolua_export
	void CaseInsensitive()                         {caseInsensitive = true;}	//tolua_export

	// Sets path of ini file to read and write from.
	void Path(const std::string & newPath)                {path = newPath;}	//tolua_export
	std::string Path() const                            {return path;}	//tolua_export
	void SetPath(const std::string & newPath)             {Path( newPath);}	//tolua_export

	// Reads ini file specified using path.
	// Returns true if successful, false otherwise.
	bool ReadFile();														//tolua_export

	// Writes data stored in class to ini file.
	bool WriteFile();														//tolua_export

	// Deletes all stored ini data.
	void Erase();															//tolua_export
	void Clear()                                   {Erase();}				//tolua_export
	void Reset()                                   {Erase();}				//tolua_export

	// Returns index of specified key, or noID if not found.
	long FindKey( const std::string & keyname) const;						//tolua_export

	// Returns index of specified value, in the specified key, or noID if not found.
	long FindValue( const unsigned keyID, const std::string & valuename) const;		//tolua_export

	// Returns number of keys currently in the ini.
	unsigned NumKeys() const                       {return names.size();}			//tolua_export
	unsigned GetNumKeys() const                    {return NumKeys();}			//tolua_export

	// Add a key name.
	unsigned AddKeyName( const std::string & keyname);								//tolua_export

	// Returns key names by index.
	std::string KeyName( const unsigned keyID) const;								//tolua_export
	std::string GetKeyName( const unsigned keyID) const {return KeyName(keyID);}	//tolua_export

	// Returns number of values stored for specified key.
	unsigned NumValues( const std::string & keyname);									//tolua_export
	unsigned GetNumValues( const std::string & keyname)   {return NumValues( keyname);}	//tolua_export
	unsigned NumValues( const unsigned keyID);										//tolua_export
	unsigned GetNumValues( const unsigned keyID)   {return NumValues( keyID);}		//tolua_export

	// Returns value name by index for a given keyname or keyID.
	std::string ValueName( const std::string & keyname, const unsigned valueID) const;	//tolua_export
	std::string GetValueName( const std::string & keyname, const unsigned valueID) const {	//tolua_export
		return ValueName( keyname, valueID);
	}																						//tolua_export
	std::string ValueName( const unsigned keyID, const unsigned valueID) const;		//tolua_export
	std::string GetValueName( const unsigned keyID, const unsigned valueID) const {	//tolua_export
		return ValueName( keyID, valueID);
	}																					//tolua_export

	// Gets value of [keyname] valuename =.
	// Overloaded to return string, int, and double.
	// Returns defValue if key/value not found.
	AString GetValue (const AString & keyname, const AString & valuename, const AString & defValue = "")    const;   // tolua_export
	AString GetValue (const unsigned keyID,    const unsigned valueID,    const AString & defValue = "")    const;   // tolua_export
	double  GetValueF(const AString & keyname, const AString & valuename, const double    defValue = 0)     const;   // tolua_export
	int     GetValueI(const AString & keyname, const AString & valuename, const int       defValue = 0)     const;   // tolua_export
	bool    GetValueB(const AString & keyname, const AString & valuename, const bool      defValue = false) const {  // tolua_export
		return ( GetValueI( keyname, valuename, int( defValue)) > 0);
	}  // tolua_export
	
	// Gets the value; if not found, write the default to the INI file
	AString GetValueSet (const AString & keyname, const AString & valuename, const AString & defValue = "");      // tolua_export
	double  GetValueSetF(const AString & keyname, const AString & valuename, const double    defValue = 0.0);     // tolua_export
	int     GetValueSetI(const AString & keyname, const AString & valuename, const int       defValue = 0);       // tolua_export
	bool    GetValueSetB(const AString & keyname, const AString & valuename, const bool      defValue = false) {  // tolua_export
		return (GetValueSetI(keyname, valuename, defValue ? 1 : 0) > 0);
	}  // tolua_export

	// Sets value of [keyname] valuename =.
	// Specify the optional paramter as false (0) if you do not want it to create
	// the key if it doesn't exist. Returns true if data entered, false otherwise.
	// Overloaded to accept string, int, and double.
	bool SetValue( const unsigned keyID, const unsigned valueID, const std::string & value);											//tolua_export
	bool SetValue( const std::string & keyname, const std::string & valuename, const std::string & value, const bool create = true);		//tolua_export
	bool SetValueI( const std::string & keyname, const std::string & valuename, const int value, const bool create = true);				//tolua_export
	bool SetValueB( const std::string & keyname, const std::string & valuename, const bool value, const bool create = true) {				//tolua_export
		return SetValueI( keyname, valuename, int(value), create);
	}																																	//tolua_export
	bool SetValueF( const std::string & keyname, const std::string & valuename, const double value, const bool create = true);			//tolua_export
	bool SetValueV( const std::string & keyname, const std::string & valuename, char *format, ...);

	// Deletes specified value.
	// Returns true if value existed and deleted, false otherwise.
	bool DeleteValueByID( const unsigned keyID, const unsigned valueID );																	//tolua_export
	bool DeleteValue( const std::string & keyname, const std::string & valuename);														//tolua_export

	// Deletes specified key and all values contained within.
	// Returns true if key existed and deleted, false otherwise.
	bool DeleteKey(const std::string & keyname);																								//tolua_export

	// Header comment functions.
	// Header comments are those comments before the first key.
	//
	// Number of header comments.
	unsigned NumHeaderComments()                  {return comments.size();}															//tolua_export
	// Add a header comment.
	void     HeaderComment( const std::string & comment);																				//tolua_export
	// Return a header comment.
	std::string   HeaderComment( const unsigned commentID) const;																		//tolua_export
	// Delete a header comment.
	bool     DeleteHeaderComment( unsigned commentID);																				//tolua_export
	// Delete all header comments.
	void     DeleteHeaderComments()               {comments.clear();}																	//tolua_export

	// Key comment functions.
	// Key comments are those comments within a key. Any comments
	// defined within value names will be added to this list. Therefore,
	// these comments will be moved to the top of the key definition when
	// the CIniFile::WriteFile() is called.
	//
	// Number of key comments.
	unsigned NumKeyComments( const unsigned keyID) const;																				//tolua_export
	unsigned NumKeyComments( const std::string & keyname) const;																		//tolua_export
	// Add a key comment.
	bool     KeyComment( const unsigned keyID, const std::string & comment);															//tolua_export
	bool     KeyComment( const std::string & keyname, const std::string & comment);														//tolua_export
	// Return a key comment.
	std::string   KeyComment( const unsigned keyID, const unsigned commentID) const;													//tolua_export
	std::string   KeyComment( const std::string & keyname, const unsigned commentID) const;												//tolua_export
	// Delete a key comment.
	bool     DeleteKeyComment( const unsigned keyID, const unsigned commentID);														//tolua_export
	bool     DeleteKeyComment( const std::string & keyname, const unsigned commentID);													//tolua_export
	// Delete all comments for a key.
	bool     DeleteKeyComments( const unsigned keyID);																				//tolua_export
	bool     DeleteKeyComments( const std::string & keyname);																			//tolua_export
};	//tolua_export

#endif
