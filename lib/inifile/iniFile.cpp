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
!! MODIFIED BY FAKETRUTH and xoft !!
*/

#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

// C++ Includes
#include <fstream>

// C Includes
#include <ctype.h>

// Local Includes
#include "iniFile.h"

#if defined(WIN32)
	#define iniEOL endl
#else
	#define iniEOL '\r' << endl
#endif

#ifndef _WIN32
	#define sscanf_s(buffer, stringbuffer, ...) (sscanf(buffer, stringbuffer, __VA_ARGS__))
#endif

using namespace std;





cIniFile::cIniFile(void) :
	m_IsCaseInsensitive(true)
{
}





bool cIniFile::ReadFile(const AString & a_FileName, bool a_AllowExampleRedirect)
{
	// Normally you would use ifstream, but the SGI CC compiler has
	// a few bugs with ifstream. So ... fstream used.
	fstream f;
	AString   line;
	AString   keyname, valuename, value;
	AString::size_type pLeft, pRight;
	bool IsFromExampleRedirect = false;
	
	f.open((FILE_IO_PREFIX + a_FileName).c_str(), ios::in);
	if (f.fail())
	{
		f.clear();
		if (a_AllowExampleRedirect)
		{
			// Retry with the .example.ini file instead of .ini:
			AString ExPath(a_FileName.substr(0, a_FileName.length() - 4));
			ExPath.append(".example.ini");
			f.open((FILE_IO_PREFIX + ExPath).c_str(), ios::in);
			if (f.fail())
			{
				return false;
			}
			IsFromExampleRedirect = true;
		}
		else
		{
			return false;
		}
	}

	while (getline(f, line))
	{
		// To be compatible with Win32, check for existence of '\r'.
		// Win32 files have the '\r' and Unix files don't at the end of a line.
		// Note that the '\r' will be written to INI files from
		// Unix so that the created INI file can be read under Win32
		// without change.
		size_t lineLength = line.length();
		if (lineLength == 0)
		{
			continue;
		}
		if (line[lineLength - 1] == '\r')
		{
			line = line.substr(0, lineLength - 1);
		}

		if (line.length() == 0)
		{
			continue;
		}

		// Check that the user hasn't opened a binary file by checking the first
		// character of each line!
		if (!isprint(line[0]))
		{
			printf("%s: Binary-check failed on char %d\n", __FUNCTION__, line[0]);
			f.close();
			return false;
		}
		if ((pLeft = line.find_first_of(";#[=")) == AString::npos)
		{
			continue;
		}

		switch (line[pLeft])
		{
			case '[':
			{
				if (
					((pRight = line.find_last_of("]")) != AString::npos) &&
					(pRight > pLeft)
					)
				{
					keyname = line.substr(pLeft + 1, pRight - pLeft - 1);
					AddKeyName(keyname);
				}
				break;
			}

			case '=':
			{
				valuename = line.substr(0, pLeft);
				value = line.substr(pLeft + 1);
				AddValue(keyname, valuename, value);
				break;
			}

			case ';':
			case '#':
			{
				if (names.size() == 0)
				{
					AddHeaderComment(line.substr(pLeft + 1));
				}
				else
				{
					AddKeyComment(keyname, line.substr(pLeft + 1));
				}
				break;
			}
		}  // switch (line[pLeft])
	}  // while (getline())

	f.close();
	if (names.size() == 0)
	{
		return false;
	}
	
	if (IsFromExampleRedirect)
	{
		WriteFile(FILE_IO_PREFIX + a_FileName);
	}
	return true;
}





bool cIniFile::WriteFile(const AString & a_FileName) const
{
	// Normally you would use ofstream, but the SGI CC compiler has
	// a few bugs with ofstream. So ... fstream used.
	fstream f;

	f.open((FILE_IO_PREFIX + a_FileName).c_str(), ios::out);
	if (f.fail())
	{
		return false;
	}

	// Write header comments.
	size_t NumComments = comments.size();
	for (size_t commentID = 0; commentID < NumComments; ++commentID)
	{
		f << ';' << comments[commentID] << iniEOL;
	}
	if (NumComments > 0)
	{
		f << iniEOL;
	}

	// Write keys and values.
	for (size_t keyID = 0; keyID < keys.size(); ++keyID)
	{
		f << '[' << names[keyID] << ']' << iniEOL;
		
		// Comments.
		for (size_t commentID = 0; commentID < keys[keyID].comments.size(); ++commentID)
		{
			f << ';' << keys[keyID].comments[commentID] << iniEOL;
		}
		
		// Values.
		for (size_t valueID = 0; valueID < keys[keyID].names.size(); ++valueID)
		{
			f << keys[keyID].names[valueID] << '=' << keys[keyID].values[valueID] << iniEOL;
		}
		f << iniEOL;
	}
	f.close();

	return true;
}





int cIniFile::FindKey(const AString & a_KeyName) const
{
	AString CaseKeyName = CheckCase(a_KeyName);
	for (size_t keyID = 0; keyID < names.size(); ++keyID)
	{
		if (CheckCase(names[keyID]) == CaseKeyName)
		{
			return keyID;
		}
	}
	return noID;
}





int cIniFile::FindValue(const int keyID, const AString & a_ValueName) const
{
	if (!keys.size() || (keyID >= (int)keys.size()))
	{
		return noID;
	}

	AString CaseValueName = CheckCase(a_ValueName);
	for (size_t valueID = 0; valueID < keys[keyID].names.size(); ++valueID)
	{
		if (CheckCase(keys[keyID].names[valueID]) == CaseValueName)
		{
			return int(valueID);
		}
	}
	return noID;
}





int cIniFile::AddKeyName(const AString & keyname)
{
	names.resize(names.size() + 1, keyname);
	keys.resize(keys.size() + 1);
	return names.size() - 1;
}





AString cIniFile::GetKeyName(const int keyID) const
{
	if (keyID < (int)names.size())
	{
		return names[keyID];
	}
	else
	{
		return "";
	}
}





int cIniFile::GetNumValues(const int keyID) const
{
	if (keyID < (int)keys.size())
	{
		return (int)keys[keyID].names.size();
	}
	return 0;
}





int cIniFile::GetNumValues(const AString & keyname) const
{
	int keyID = FindKey(keyname);
	if (keyID == noID)
	{
		return 0;
	}
	return (int)keys[keyID].names.size();
}





AString cIniFile::GetValueName(const int keyID, const int valueID) const
{
	if ((keyID < (int)keys.size()) && (valueID < (int)keys[keyID].names.size()))
	{
		return keys[keyID].names[valueID];
	}
	return "";
}





AString cIniFile::GetValueName(const AString & keyname, const int valueID) const
{
	int keyID = FindKey(keyname);
	if (keyID == noID)
	{
		return "";
	}
	return GetValueName(keyID, valueID);
}





void cIniFile::AddValue(const AString & a_KeyName, const AString & a_ValueName, const AString & a_Value)
{
	int keyID = FindKey(a_KeyName);
	if (keyID == noID)
	{
		keyID = int(AddKeyName(a_KeyName));
	}

	keys[keyID].names.push_back(a_ValueName);
	keys[keyID].values.push_back(a_Value);
}





void cIniFile::AddValueI(const AString & a_KeyName, const AString & a_ValueName, const int a_Value)
{
	AddValue(a_KeyName, a_ValueName, Printf("%d", a_Value));
}





void cIniFile::AddValueF(const AString & a_KeyName, const AString & a_ValueName, const double a_Value)
{
	AddValue(a_KeyName, a_ValueName, Printf("%f", a_Value));
}





bool cIniFile::SetValue(const int keyID, const int valueID, const AString & value)
{
	if (((size_t)keyID >= keys.size()) || ((size_t)valueID >= keys[keyID].names.size()))
	{
		return false;
	}
	keys[keyID].values[valueID] = value;
	return true;
}





bool cIniFile::SetValue(const AString & a_KeyName, const AString & a_ValueName, const AString & a_Value, const bool a_CreateIfNotExists)
{
	int keyID = FindKey(a_KeyName);
	if (keyID == noID)
	{
		if (!a_CreateIfNotExists)
		{
			return false;
		}
		keyID = AddKeyName(a_KeyName);
	}

	int valueID = FindValue(keyID, a_ValueName);
	if (valueID == noID)
	{
		if (!a_CreateIfNotExists)
		{
			return false;
		}
		keys[keyID].names.push_back(a_ValueName);
		keys[keyID].values.push_back(a_Value);
	}
	else
	{
		keys[keyID].values[valueID] = a_Value;
	}

	return true;
}





bool cIniFile::SetValueI(const AString & a_KeyName, const AString & a_ValueName, const int a_Value, const bool a_CreateIfNotExists)
{
	return SetValue(a_KeyName, a_ValueName, Printf("%d", a_Value), a_CreateIfNotExists);
}





bool cIniFile::SetValueF(const AString & a_KeyName, const AString & a_ValueName, double const a_Value, const bool a_CreateIfNotExists)
{
	return SetValue(a_KeyName, a_ValueName, Printf("%f", a_Value), a_CreateIfNotExists);
}





bool cIniFile::SetValueV(const AString & a_KeyName, const AString & a_ValueName, const char * a_Format, ...)
{
	va_list args;
	va_start(args, a_Format);
	AString Data;
	AppendVPrintf(Data, a_Format, args);
	va_end(args);
	return SetValue(a_KeyName, a_ValueName, Data);
}





AString cIniFile::GetValue(const int keyID, const int valueID, const AString & defValue) const
{
	if ((keyID < (int)keys.size()) && (valueID < (int)keys[keyID].names.size()))
	{
		return keys[keyID].values[valueID];
	}
	return defValue;
}





AString cIniFile::GetValue(const AString & keyname, const AString & valuename, const AString & defValue) const
{
	int keyID = FindKey(keyname);
	if (keyID == noID)
	{
		return defValue;
	}

	int valueID = FindValue(int(keyID), valuename);
	if (valueID == noID)
	{
		return defValue;
	}

	return keys[keyID].values[valueID];
}





int cIniFile::GetValueI(const AString & keyname, const AString & valuename, const int defValue) const
{
	AString Data;
	Printf(Data, "%d", defValue);
	return atoi(GetValue(keyname, valuename, Data).c_str());
}





double cIniFile::GetValueF(const AString & keyname, const AString & valuename, double const defValue) const
{
	AString Data;
	Printf(Data, "%f", defValue);
	return atof(GetValue(keyname, valuename, Data).c_str());
}





AString cIniFile::GetValueSet(const AString & keyname, const AString & valuename, const AString & defValue)
{
	int keyID = FindKey(keyname);
	if (keyID == noID)
	{
		SetValue(keyname, valuename, defValue);
		return defValue;
	}

	int valueID = FindValue(int(keyID), valuename);
	if (valueID == noID)
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





bool cIniFile::DeleteValueByID(const int keyID, const int valueID)
{
	if ((keyID < (int)keys.size()) && (valueID < (int)keys[keyID].names.size()))
	{
		// This looks strange, but is neccessary.
		vector<AString>::iterator npos = keys[keyID].names.begin() + valueID;
		vector<AString>::iterator vpos = keys[keyID].values.begin() + valueID;
		keys[keyID].names.erase(npos, npos + 1);
		keys[keyID].values.erase(vpos, vpos + 1);
		return true;
	}
	return false;
}





bool cIniFile::DeleteValue(const AString & keyname, const AString & valuename)
{
	int keyID = FindKey(keyname);
	if (keyID == noID)
	{
		return false;
	}

	int valueID = FindValue(int(keyID), valuename);
	if (valueID == noID)
	{
		return false;
	}

	return DeleteValueByID(keyID, valueID);
}





bool cIniFile::DeleteKey(const AString & keyname)
{
	int keyID = FindKey(keyname);
	if (keyID == noID)
	{
		return false;
	}

	vector<AString>::iterator npos = names.begin() + keyID;
	vector<key>::iterator    kpos = keys.begin() + keyID;
	names.erase(npos, npos + 1);
	keys.erase(kpos, kpos + 1);

	return true;
}





void cIniFile::Clear(void)
{
	names.clear();
	keys.clear();
	comments.clear();
}





void cIniFile::AddHeaderComment(const AString & comment)
{
	comments.push_back(comment);
	// comments.resize(comments.size() + 1, comment);
}





AString cIniFile::GetHeaderComment(const int commentID) const
{
	if (commentID < (int)comments.size())
	{
		return comments[commentID];
	}
	return "";
}





bool cIniFile::DeleteHeaderComment(int commentID)
{
	if (commentID < (int)comments.size())
	{
		vector<AString>::iterator cpos = comments.begin() + commentID;
		comments.erase(cpos, cpos + 1);
		return true;
	}
	return false;
}





int cIniFile::GetNumKeyComments(const int keyID) const
{
	if (keyID < (int)keys.size())
	{
		return keys[keyID].comments.size();
	}
	return 0;
}





int cIniFile::GetNumKeyComments(const AString & keyname) const
{
	int keyID = FindKey(keyname);
	if (keyID == noID)
	{
		return 0;
	}
	return (int)keys[keyID].comments.size();
}





bool cIniFile::AddKeyComment(const int keyID, const AString & comment)
{
	if (keyID < (int)keys.size())
	{
		keys[keyID].comments.resize(keys[keyID].comments.size() + 1, comment);
		return true;
	}
	return false;
}





bool cIniFile::AddKeyComment(const AString & keyname, const AString & comment)
{
	int keyID = FindKey(keyname);
	if (keyID == noID)
	{
		return false;
	}
	return AddKeyComment(keyID, comment);
}





AString cIniFile::GetKeyComment(const int keyID, const int commentID) const
{
	if ((keyID < (int)keys.size()) && (commentID < (int)keys[keyID].comments.size()))
	{
		return keys[keyID].comments[commentID];
	}
	return "";
}





AString cIniFile::GetKeyComment(const AString & keyname, const int commentID) const
{
	int keyID = FindKey(keyname);
	if (keyID == noID)
	{
		return "";
	}
	return GetKeyComment(int(keyID), commentID);
}





bool cIniFile::DeleteKeyComment(const int keyID, const int commentID)
{
	if ((keyID < (int)keys.size()) && (commentID < (int)keys[keyID].comments.size()))
	{
		vector<AString>::iterator cpos = keys[keyID].comments.begin() + commentID;
		keys[keyID].comments.erase(cpos, cpos + 1);
		return true;
	}
	return false;
}





bool cIniFile::DeleteKeyComment(const AString & keyname, const int commentID)
{
	int keyID = FindKey(keyname);
	if (keyID == noID)
	{
		return false;
	}
	return DeleteKeyComment(int(keyID), commentID);
}





bool cIniFile::DeleteKeyComments(const int keyID)
{
	if (keyID < (int)keys.size())
	{
		keys[keyID].comments.clear();
		return true;
	}
	return false;
}





bool cIniFile::DeleteKeyComments(const AString & keyname)
{
	int keyID = FindKey(keyname);
	if (keyID == noID)
	{
		return false;
	}
	return DeleteKeyComments(int(keyID));
}





AString cIniFile::CheckCase(const AString & s) const
{
	if (!m_IsCaseInsensitive)
	{
		return s;
	}
	AString res(s);
	size_t len = res.length();
	for (size_t i = 0; i < len; i++)
	{
		res[i] = tolower(res[i]);
	}
	return res;
}




