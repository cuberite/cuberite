// IniFile.cpp:  Implementation of the CIniFile class.
// Written by:   Adam Clauss
// Email: cabadam@houston.rr.com
// You may use this class/code as you wish in your programs.  Feel free to distribute it, and
// email suggested changes to me.
//
// Rewritten by: Shane Hill
// Date:         21/08/2001
// Email:        Shane.Hill@dsto.defence.gov.au
// Reason:       Remove dependancy on MFC. Code should compile on any
//               platform.
//////////////////////////////////////////////////////////////////////

/*
	!! MODIFIED BY FAKETRUTH !!
*/

#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

// C++ Includes
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

// C Includes
#include <stdarg.h>
#include <ctype.h>

// Local Includes
#include "iniFile.h"

#if defined(WIN32)
#define iniEOL endl
#else
#define iniEOL '\r' << endl
#endif

#ifndef _WIN32
#define	sscanf_s(buffer, stringbuffer, ...) (sscanf(buffer, stringbuffer, __VA_ARGS__))
#endif





cIniFile::cIniFile( const string iniPath)
{
  Path( iniPath);
  caseInsensitive = true;
}

bool cIniFile::ReadFile()
{
  // Normally you would use ifstream, but the SGI CC compiler has
  // a few bugs with ifstream. So ... fstream used.
  fstream f;
  string   line;
  string   keyname, valuename, value;
  string::size_type pLeft, pRight;

  f.open( path.c_str(), ios::in);
  if ( f.fail())
    return false;

  while( getline( f, line)) {
    // To be compatible with Win32, check for existence of '\r'.
    // Win32 files have the '\r' and Unix files don't at the end of a line.
    // Note that the '\r' will be written to INI files from
    // Unix so that the created INI file can be read under Win32
    // without change.
	unsigned int lineLength = line.length();
	if(lineLength == 0) continue;
    if ( line[lineLength - 1] == '\r')
      line = line.substr( 0, lineLength - 1);

    if ( line.length()) {
      // Check that the user hasn't opened a binary file by checking the first
      // character of each line!
      if ( !isprint( line[0])) {
	printf( "Failing on char %d\n", line[0]);
	f.close();
	return false;
      }
      if (( pLeft = line.find_first_of(";#[=")) != string::npos) {
	switch ( line[pLeft]) {
	case '[':
	  if ((pRight = line.find_last_of("]")) != string::npos &&
	      pRight > pLeft) {
	    keyname = line.substr( pLeft + 1, pRight - pLeft - 1);
	    AddKeyName( keyname);
	  }
	  break;

	case '=':
	  valuename = line.substr( 0, pLeft);
	  value = line.substr( pLeft + 1);
	  SetValue( keyname, valuename, value);
	  break;

	case ';':
	case '#':
	  if ( !names.size())
	    HeaderComment( line.substr( pLeft + 1));
	  else
	    KeyComment( keyname, line.substr( pLeft + 1));
	  break;
	}
      }
    }
  }

  f.close();
  if ( names.size())
    return true;
  return false;
}





bool cIniFile::WriteFile()
{
  unsigned commentID, keyID, valueID;
  // Normally you would use ofstream, but the SGI CC compiler has
  // a few bugs with ofstream. So ... fstream used.
  fstream f;

  f.open( path.c_str(), ios::out);
  if ( f.fail())
    return false;

  // Write header comments.
  for ( commentID = 0; commentID < comments.size(); ++commentID)
    f << ';' << comments[commentID] << iniEOL;
  if ( comments.size())
    f << iniEOL;

  // Write keys and values.
  for ( keyID = 0; keyID < keys.size(); ++keyID) {
    f << '[' << names[keyID] << ']' << iniEOL;
    // Comments.
    for ( commentID = 0; commentID < keys[keyID].comments.size(); ++commentID)
      f << ';' << keys[keyID].comments[commentID] << iniEOL;
    // Values.
    for ( valueID = 0; valueID < keys[keyID].names.size(); ++valueID)
      f << keys[keyID].names[valueID] << '=' << keys[keyID].values[valueID] << iniEOL;
    f << iniEOL;
  }
  f.close();

  return true;
}





long cIniFile::FindKey( const string & keyname) const
{
  for ( unsigned keyID = 0; keyID < names.size(); ++keyID)
    if ( CheckCase( names[keyID]) == CheckCase( keyname))
      return long(keyID);
  return noID;
}





long cIniFile::FindValue( unsigned const keyID, const string & valuename) const
{
  if ( !keys.size() || keyID >= keys.size())
    return noID;

  for ( unsigned valueID = 0; valueID < keys[keyID].names.size(); ++valueID)
    if ( CheckCase( keys[keyID].names[valueID]) == CheckCase( valuename))
      return long(valueID);
  return noID;
}





unsigned cIniFile::AddKeyName( const string & keyname)
{
  names.resize( names.size() + 1, keyname);
  keys.resize( keys.size() + 1);
  return names.size() - 1;
}





string cIniFile::KeyName( unsigned const keyID) const
{
  if ( keyID < names.size())
    return names[keyID];
  else
    return "";
}





unsigned cIniFile::NumValues( unsigned const keyID)
{
  if ( keyID < keys.size())
    return keys[keyID].names.size();
  return 0;
}





unsigned cIniFile::NumValues( const string & keyname)
{
  long keyID = FindKey( keyname);
  if ( keyID == noID)
    return 0;
  return keys[keyID].names.size();
}





string cIniFile::ValueName( unsigned const keyID, unsigned const valueID) const
{
  if ( keyID < keys.size() && valueID < keys[keyID].names.size())
    return keys[keyID].names[valueID];
  return "";
}





string cIniFile::ValueName( const string & keyname, unsigned const valueID) const
{
  long keyID = FindKey( keyname);
  if ( keyID == noID)
    return "";
  return ValueName( keyID, valueID);
}





bool cIniFile::SetValue( unsigned const keyID, unsigned const valueID, const string & value)
{
  if ( keyID < keys.size() && valueID < keys[keyID].names.size())
    keys[keyID].values[valueID] = value;

  return false;
}





bool cIniFile::SetValue( const string & keyname, const string & valuename, const string & value, bool const create)
{
  long keyID = FindKey( keyname);
  if ( keyID == noID) {
    if ( create)
      keyID = long( AddKeyName( keyname));
    else
      return false;
  }

  long valueID = FindValue( unsigned(keyID), valuename);
  if ( valueID == noID) {
    if ( !create)
      return false;
    keys[keyID].names.resize( keys[keyID].names.size() + 1, valuename);
    keys[keyID].values.resize( keys[keyID].values.size() + 1, value);
  } else
  {
	  if(!create)
		keys[keyID].values[valueID] = value;
	  else
	  {
		  keys[keyID].names.resize( keys[keyID].names.size() + 1, valuename);
		  keys[keyID].values.resize( keys[keyID].values.size() + 1, value);
	  }
  }

  return true;
}





bool cIniFile::SetValueI( const string & keyname, const string & valuename, int const value, bool const create)
{
  AString Data;
  Printf(Data, "%d", value);
  return SetValue( keyname, valuename, Data, create);
}





bool cIniFile::SetValueF( const string & keyname, const string & valuename, double const value, bool const create)
{
  AString Data;
  Printf(Data, "%f", value);
  return SetValue( keyname, valuename, Data, create);
}





bool cIniFile::SetValueV( const string & keyname, const string & valuename, char *format, ...)
{
  va_list args;

  va_start( args, format);
  
  AString Data;
  AppendVPrintf(Data, format, args);
  va_end( args);
  return SetValue( keyname, valuename, Data);
}





string cIniFile::GetValue( unsigned const keyID, unsigned const valueID, const string & defValue) const
{
  if ( keyID < keys.size() && valueID < keys[keyID].names.size())
    return keys[keyID].values[valueID];
  return defValue;
}





string cIniFile::GetValue( const string & keyname, const string & valuename, const string & defValue) const
{
  long keyID = FindKey( keyname);
  if ( keyID == noID)
    return defValue;

  long valueID = FindValue( unsigned(keyID), valuename);
  if ( valueID == noID)
    return defValue;

  return keys[keyID].values[valueID];
}





int cIniFile::GetValueI(const string & keyname, const string & valuename, int const defValue) const
{
	AString Data;
	Printf(Data, "%d", defValue);
  return atoi( GetValue( keyname, valuename, Data).c_str());
}





double cIniFile::GetValueF(const string & keyname, const string & valuename, double const defValue) const
{
  AString Data;
  Printf(Data, "%f", defValue);
  return atof( GetValue( keyname, valuename, Data).c_str());
}





AString cIniFile::GetValueSet(const AString & keyname, const AString & valuename, const AString & defValue)
{
	long keyID = FindKey( keyname);
	if ( keyID == noID)
	{
		SetValue(keyname, valuename, defValue);
		return defValue;
	}

	long valueID = FindValue( unsigned(keyID), valuename);
	if ( valueID == noID)
	{
		SetValue(keyname, valuename, defValue);
		return defValue;
	}

	return keys[keyID].values[valueID];
}





double cIniFile::GetValueSetF(const AString & keyname, const AString & valuename, const double defValue)
{
  AString Data;
  Printf(Data, "%f", defValue);
  return atof(GetValueSet(keyname, valuename, Data).c_str());
}





int cIniFile::GetValueSetI(const AString & keyname, const AString & valuename, const int defValue)
{
	AString Data;
	Printf(Data, "%d", defValue);
  return atoi(GetValueSet(keyname, valuename, Data).c_str());
}





bool cIniFile::DeleteValueByID( const unsigned keyID, const unsigned valueID )
{
	if ( keyID < keys.size() && valueID < keys[keyID].names.size())
	{
		// This looks strange, but is neccessary.
		vector<string>::iterator npos = keys[keyID].names.begin() + valueID;
		vector<string>::iterator vpos = keys[keyID].values.begin() + valueID;
		keys[keyID].names.erase( npos, npos + 1);
		keys[keyID].values.erase( vpos, vpos + 1);
		return true;
	}
	return false;
}

bool cIniFile::DeleteValue( const string & keyname, const string & valuename)
{
  long keyID = FindKey( keyname);
  if ( keyID == noID)
    return false;

  long valueID = FindValue( unsigned(keyID), valuename);
  if ( valueID == noID)
    return false;

  return DeleteValueByID( keyID, valueID );
}

bool cIniFile::DeleteKey( const string & keyname)
{
  long keyID = FindKey( keyname);
  if ( keyID == noID)
    return false;

  // Now hopefully this destroys the vector lists within keys.
  // Looking at <vector> source, this should be the case using the destructor.
  // If not, I may have to do it explicitly. Memory leak check should tell.
  // memleak_test.cpp shows that the following not required.
  //keys[keyID].names.clear();
  //keys[keyID].values.clear();

  vector<string>::iterator npos = names.begin() + keyID;
  vector<key>::iterator    kpos = keys.begin() + keyID;
  names.erase( npos, npos + 1);
  keys.erase( kpos, kpos + 1);

  return true;
}

void cIniFile::Erase()
{
  // This loop not needed. The vector<> destructor seems to do
  // all the work itself. memleak_test.cpp shows this.
  //for ( unsigned i = 0; i < keys.size(); ++i) {
  //  keys[i].names.clear();
  //  keys[i].values.clear();
  //}
  names.clear();
  keys.clear();
  comments.clear();
}

void cIniFile::HeaderComment( const string & comment)
{
  comments.resize( comments.size() + 1, comment);
}

string cIniFile::HeaderComment( unsigned const commentID) const
{
  if ( commentID < comments.size())
    return comments[commentID];
  return "";
}

bool cIniFile::DeleteHeaderComment( unsigned commentID)
{
  if ( commentID < comments.size()) {
    vector<string>::iterator cpos = comments.begin() + commentID;
    comments.erase( cpos, cpos + 1);
    return true;
  }
  return false;
}

unsigned cIniFile::NumKeyComments( unsigned const keyID) const
{
  if ( keyID < keys.size())
    return keys[keyID].comments.size();
  return 0;
}

unsigned cIniFile::NumKeyComments( const string & keyname) const
{
  long keyID = FindKey( keyname);
  if ( keyID == noID)
    return 0;
  return keys[keyID].comments.size();
}

bool cIniFile::KeyComment( unsigned const keyID, const string & comment)
{
  if ( keyID < keys.size()) {
    keys[keyID].comments.resize( keys[keyID].comments.size() + 1, comment);
    return true;
  }
  return false;
}

bool cIniFile::KeyComment( const string & keyname, const string & comment)
{
  long keyID = FindKey( keyname);
  if ( keyID == noID)
    return false;
  return KeyComment( unsigned(keyID), comment);
}

string cIniFile::KeyComment( unsigned const keyID, unsigned const commentID) const
{
  if ( keyID < keys.size() && commentID < keys[keyID].comments.size())
    return keys[keyID].comments[commentID];
  return "";
}

string cIniFile::KeyComment( const string & keyname, unsigned const commentID) const
{
  long keyID = FindKey( keyname);
  if ( keyID == noID)
    return "";
  return KeyComment( unsigned(keyID), commentID);
}

bool cIniFile::DeleteKeyComment( unsigned const keyID, unsigned const commentID)
{
  if ( keyID < keys.size() && commentID < keys[keyID].comments.size()) {
    vector<string>::iterator cpos = keys[keyID].comments.begin() + commentID;
    keys[keyID].comments.erase( cpos, cpos + 1);
    return true;
  }
  return false;
}

bool cIniFile::DeleteKeyComment( const string & keyname, unsigned const commentID)
{
  long keyID = FindKey( keyname);
  if ( keyID == noID)
    return false;
  return DeleteKeyComment( unsigned(keyID), commentID);
}

bool cIniFile::DeleteKeyComments( unsigned const keyID)
{
  if ( keyID < keys.size()) {
    keys[keyID].comments.clear();
    return true;
  }
  return false;
}

bool cIniFile::DeleteKeyComments( const string & keyname)
{
  long keyID = FindKey( keyname);
  if ( keyID == noID)
    return false;
  return DeleteKeyComments( unsigned(keyID));
}

string cIniFile::CheckCase( string s) const
{
  if ( caseInsensitive)
    for ( string::size_type i = 0; i < s.length(); ++i)
      s[i] = (char)tolower(s[i]);
  return s;
}
