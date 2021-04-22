// IniFile.cpp:  Implementation of the CIniFile class.
// Written by:   Adam Clauss
// Email: cabadam@houston.rr.com
// You may use this class / code as you wish in your programs.  Feel free to distribute it, and
// email suggested changes to me.
//
// Rewritten by: Shane Hill
// Date:         2001-08-21
// Email:        Shane.Hill@dsto.defence.gov.au
// Reason:       Remove dependancy on MFC. Code should compile on any
//               platform.
////////////////////////////////////////////////////////////////////////////////

/*
!! MODIFIED BY FAKETRUTH and xoft !!
*/

#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

// C++ Includes
#include <fstream>

// C Includes
#include <ctype.h>

// Local Includes
#include "IniFile.h"

#if defined(WIN32)
	#define iniEOL endl
#else
	#define iniEOL '\r' << endl
#endif

using namespace std;





cIniFile::cIniFile(void) :
	m_IsCaseInsensitive(true)
{
}





bool cIniFile::ReadFile(const AString & a_FileName, bool a_AllowExampleRedirect)
{

	m_Filename = a_FileName;

	// Normally you would use ifstream, but the SGI CC compiler has
	// a few bugs with ifstream. So ... fstream used.
	fstream f;
	AString   line;
	AString   keyname, valuename, value;
	AString::size_type pLeft, pRight;
	bool IsFromExampleRedirect = false;


	f.open((a_FileName).c_str(), ios::in);
	if (f.fail())
	{
		f.clear();
		if (a_AllowExampleRedirect)
		{
			// Retry with the .example.ini file instead of .ini:
			AString ExPath(a_FileName.substr(0, a_FileName.length() - 4));
			ExPath.append(".example.ini");
			f.open((ExPath).c_str(), ios::in);
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

	bool IsFirstLine = true;

	while (getline(f, line))
	{
		// To be compatible with Win32, check for existence of '\r'.
		// Win32 files have the '\r' and Unix files don't at the end of a line.
		// Note that the '\r' will be written to INI files from
		// Unix so that the created INI file can be read under Win32
		// without change.

		// Removes UTF-8 Byte Order Markers (BOM) if, present.
		if (IsFirstLine)
		{
			RemoveBom(line);
			IsFirstLine = false;
		}

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
				value = TrimString(line.substr(pLeft + 1));
				AddValue(keyname, valuename, value);
				break;
			}

			case ';':
			case '#':
			{
				if (m_Names.empty())
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
	if (m_Keys.empty() && m_Names.empty() && m_Comments.empty())
	{
		// File be empty or unreadable, equivalent to nonexistant
		return false;
	}

	if (IsFromExampleRedirect)
	{
		WriteFile(a_FileName);
	}

	return true;
}





bool cIniFile::WriteFile(const AString & a_FileName) const
{
	// Normally you would use ofstream, but the SGI CC compiler has
	// a few bugs with ofstream. So ... fstream used.
	fstream f;

	f.open((a_FileName).c_str(), ios::out);
	if (f.fail())
	{
		return false;
	}

	// Write header comments.
	size_t NumComments = m_Comments.size();
	for (size_t commentID = 0; commentID < NumComments; ++commentID)
	{
		f << ';' << m_Comments[commentID] << iniEOL;
	}
	if (NumComments > 0)
	{
		f << iniEOL;
	}

	// Write keys and values.
	for (size_t keyID = 0; keyID < m_Keys.size(); ++keyID)
	{
		f << '[' << m_Names[keyID] << ']' << iniEOL;

		// Comments.
		for (size_t commentID = 0; commentID < m_Keys[keyID].m_Comments.size(); ++commentID)
		{
			f << ';' << m_Keys[keyID].m_Comments[commentID] << iniEOL;
		}

		// Values.
		for (size_t valueID = 0; valueID < m_Keys[keyID].m_Names.size(); ++valueID)
		{
			f << m_Keys[keyID].m_Names[valueID] << '=' << m_Keys[keyID].m_Values[valueID] << iniEOL;
		}
		f << iniEOL;
	}
	f.close();

	return true;
}





int cIniFile::FindKey(const AString & a_KeyName) const
{
	AString CaseKeyName = CheckCase(a_KeyName);
	for (size_t keyID = 0; keyID < m_Names.size(); ++keyID)
	{
		if (CheckCase(m_Names[keyID]) == CaseKeyName)
		{
			return static_cast<int>(keyID);
		}
	}
	return noID;
}





int cIniFile::FindValue(const int keyID, const AString & a_ValueName) const
{
	if (!m_Keys.size() || (keyID >= static_cast<int>(m_Keys.size())))
	{
		return noID;
	}

	AString CaseValueName = CheckCase(a_ValueName);
	for (size_t valueID = 0; valueID < m_Keys[static_cast<size_t>(keyID)].m_Names.size(); ++valueID)
	{
		if (CheckCase(m_Keys[static_cast<size_t>(keyID)].m_Names[valueID]) == CaseValueName)
		{
			return int(valueID);
		}
	}
	return noID;
}





int cIniFile::AddKeyName(const AString & keyname)
{
	m_Names.resize(m_Names.size() + 1, keyname);
	m_Keys.resize(m_Keys.size() + 1);
	return static_cast<int>(m_Names.size()) - 1;
}





AString cIniFile::GetKeyName(const int keyID) const
{
	if (keyID < static_cast<int>(m_Names.size()))
	{
		return m_Names[static_cast<size_t>(keyID)];
	}
	else
	{
		return "";
	}
}





int cIniFile::GetNumValues(const int keyID) const
{
	if (keyID < static_cast<int>(m_Keys.size()))
	{
		return static_cast<int>(m_Keys[static_cast<size_t>(keyID)].m_Names.size());
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
	return static_cast<int>(m_Keys[static_cast<size_t>(keyID)].m_Names.size());
}





AString cIniFile::GetValueName(const int keyID, const int valueID) const
{
	if ((keyID < static_cast<int>(m_Keys.size())) && (valueID < static_cast<int>(m_Keys[static_cast<size_t>(keyID)].m_Names.size())))
	{
		return m_Keys[static_cast<size_t>(keyID)].m_Names[static_cast<size_t>(valueID)];
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

	m_Keys[static_cast<size_t>(keyID)].m_Names.push_back(a_ValueName);
	m_Keys[static_cast<size_t>(keyID)].m_Values.push_back(a_Value);
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
	if ((static_cast<size_t>(keyID) >= m_Keys.size()) || (static_cast<size_t>(valueID) >= m_Keys[static_cast<size_t>(keyID)].m_Names.size()))
	{
		return false;
	}
	m_Keys[static_cast<size_t>(keyID)].m_Values[static_cast<size_t>(valueID)] = value;
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
		m_Keys[static_cast<size_t>(keyID)].m_Names.push_back(a_ValueName);
		m_Keys[static_cast<size_t>(keyID)].m_Values.push_back(a_Value);
	}
	else
	{
		m_Keys[static_cast<size_t>(keyID)].m_Values[static_cast<size_t>(valueID)] = a_Value;
	}

	return true;
}





bool cIniFile::SetValueI(const AString & a_KeyName, const AString & a_ValueName, const int a_Value, const bool a_CreateIfNotExists)
{
	return SetValue(a_KeyName, a_ValueName, Printf("%d", a_Value), a_CreateIfNotExists);
}





bool cIniFile::SetValueI(const AString & a_Keyname, const AString & a_ValueName, const Int64 a_Value, const bool a_CreateIfNotExists)
{
	return SetValue(a_Keyname, a_ValueName, Printf("%lld", a_Value), a_CreateIfNotExists);
}





bool cIniFile::SetValueF(const AString & a_KeyName, const AString & a_ValueName, double const a_Value, const bool a_CreateIfNotExists)
{
	return SetValue(a_KeyName, a_ValueName, Printf("%f", a_Value), a_CreateIfNotExists);
}





AString cIniFile::GetValue(const int keyID, const int valueID, const AString & defValue) const
{
	if ((keyID < static_cast<int>(m_Keys.size())) && (valueID < static_cast<int>(m_Keys[static_cast<size_t>(keyID)].m_Names.size())))
	{
		return m_Keys[static_cast<size_t>(keyID)].m_Values[static_cast<size_t>(valueID)];
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

	return m_Keys[static_cast<size_t>(keyID)].m_Values[static_cast<size_t>(valueID)];
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

	return m_Keys[static_cast<size_t>(keyID)].m_Values[static_cast<size_t>(valueID)];
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





Int64 cIniFile::GetValueSetI(const AString & keyname, const AString & valuename, const Int64 defValue)
{
	AString Data;
	Printf(Data, "%lld", defValue);
	AString resultstring = GetValueSet(keyname, valuename, Data);
	Int64 result = defValue;
#ifdef _WIN32
	sscanf_s(resultstring.c_str(), "%lld", &result);
#else
	sscanf(resultstring.c_str(), "%lld", &result);
#endif
	return result;
}





bool cIniFile::DeleteValueByID(const int keyID, const int valueID)
{
	if ((keyID < static_cast<int>(m_Keys.size())) && (valueID < static_cast<int>(m_Keys[static_cast<size_t>(keyID)].m_Names.size())))
	{
		// This looks strange, but is neccessary.
		vector<AString>::iterator npos = m_Keys[static_cast<size_t>(keyID)].m_Names.begin() + valueID;
		vector<AString>::iterator vpos = m_Keys[static_cast<size_t>(keyID)].m_Values.begin() + valueID;
		m_Keys[static_cast<size_t>(keyID)].m_Names.erase(npos, npos + 1);
		m_Keys[static_cast<size_t>(keyID)].m_Values.erase(vpos, vpos + 1);
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

	vector<AString>::iterator npos = m_Names.begin() + keyID;
	vector<key>::iterator    kpos = m_Keys.begin() + keyID;
	m_Names.erase(npos, npos + 1);
	m_Keys.erase(kpos, kpos + 1);

	return true;
}





void cIniFile::Clear(void)
{
	m_Names.clear();
	m_Keys.clear();
	m_Comments.clear();
}





bool cIniFile::HasValue(const AString & a_KeyName, const AString & a_ValueName) const
{
	// Find the key:
	int keyID = FindKey(a_KeyName);
	if (keyID == noID)
	{
		return false;
	}

	// Find the value:
	int valueID = FindValue(keyID, a_ValueName);
	return (valueID != noID);
}





void cIniFile::AddHeaderComment(const AString & comment)
{
	m_Comments.push_back(comment);
	// comments.resize(comments.size() + 1, comment);
}





AString cIniFile::GetHeaderComment(const int commentID) const
{
	if (commentID < static_cast<int>(m_Comments.size()))
	{
		return m_Comments[static_cast<size_t>(commentID)];
	}
	return "";
}





bool cIniFile::DeleteHeaderComment(int commentID)
{
	if (commentID < static_cast<int>(m_Comments.size()))
	{
		vector<AString>::iterator cpos = m_Comments.begin() + commentID;
		m_Comments.erase(cpos, cpos + 1);
		return true;
	}
	return false;
}





int cIniFile::GetNumKeyComments(const int keyID) const
{
	if (keyID < static_cast<int>(m_Keys.size()))
	{
		return static_cast<int>(m_Keys[static_cast<size_t>(keyID)].m_Comments.size());
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
	return static_cast<int>(m_Keys[static_cast<size_t>(keyID)].m_Comments.size());
}





bool cIniFile::AddKeyComment(const int keyID, const AString & comment)
{
	if (keyID < static_cast<int>(m_Keys.size()))
	{
		m_Keys[static_cast<size_t>(keyID)].m_Comments.resize(m_Keys[static_cast<size_t>(keyID)].m_Comments.size() + 1, comment);
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
	if ((keyID < static_cast<int>(m_Keys.size())) && (commentID < static_cast<int>(m_Keys[static_cast<size_t>(keyID)].m_Comments.size())))
	{
		return m_Keys[static_cast<size_t>(keyID)].m_Comments[static_cast<size_t>(commentID)];
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
	if ((keyID < static_cast<int>(m_Keys.size())) && (commentID < static_cast<int>(m_Keys[static_cast<size_t>(keyID)].m_Comments.size())))
	{
		vector<AString>::iterator cpos = m_Keys[static_cast<size_t>(keyID)].m_Comments.begin() + commentID;
		m_Keys[static_cast<size_t>(keyID)].m_Comments.erase(cpos, cpos + 1);
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
	if (keyID < static_cast<int>(m_Keys.size()))
	{
		m_Keys[static_cast<size_t>(keyID)].m_Comments.clear();
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
	return DeleteKeyComments(static_cast<int>(keyID));
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
		res[i] = static_cast<char>(tolower(res[i]));
	}
	return res;
}





void cIniFile::RemoveBom(AString & a_line) const
{
	// The BOM sequence for UTF-8 is 0xEF, 0xBB, 0xBF
	static unsigned const char BOM[] = { 0xEF, 0xBB, 0xBF };

	// The BOM sequence, if present, is always th e first three characters of the input.
	const AString ref = a_line.substr(0, 3);

	// If any of the first three chars do not match, return and do nothing.
	for (size_t i = 0; i < 3; ++i)
	{
		if (static_cast<unsigned char>(ref[i]) != BOM[i])
		{
			return;
		}
	}

	// First three characters match; erase them.
	a_line.erase(0, 3);
}





bool cIniFile::KeyExists(AString a_keyname) const
{
	return FindKey(a_keyname) != noID;
}





std::vector<std::pair<AString, AString>> cIniFile::GetValues(AString a_keyName)
{
	std::vector<std::pair<AString, AString>> ret;
	int keyID = FindKey(a_keyName);
	if (keyID == noID)
	{
		return ret;
	}
	for (size_t valueID = 0; valueID < m_Keys[static_cast<size_t>(keyID)].m_Names.size(); ++valueID)
	{
		ret.emplace_back(m_Keys[static_cast<size_t>(keyID)].m_Names[valueID], m_Keys[static_cast<size_t>(keyID)].m_Values[valueID]);
	}
	return ret;
}





AStringVector ReadUpgradeIniPorts(
	cSettingsRepositoryInterface & a_Settings,
	const AString & a_KeyName,
	const AString & a_PortsValueName,
	const AString & a_OldIPv4ValueName,
	const AString & a_OldIPv6ValueName,
	const AString & a_DefaultValue
)
{
	// Read the regular value, but don't use the default (in order to detect missing value for upgrade):

	AStringVector Ports;

	for (const auto & pair : a_Settings.GetValues(a_KeyName))
	{
		if (pair.first != a_PortsValueName)
		{
			continue;
		}
		AStringVector temp = StringSplitAndTrim(pair.second, ";,");
		Ports.insert(Ports.end(), temp.begin(), temp.end());
	}

	if (Ports.empty())
	{
		// Historically there were two separate entries for IPv4 and IPv6, merge them and migrate:
		AString Ports4 = a_Settings.GetValue(a_KeyName, a_OldIPv4ValueName, a_DefaultValue);
		AString Ports6 = a_Settings.GetValue(a_KeyName, a_OldIPv6ValueName);
		Ports = MergeStringVectors(StringSplitAndTrim(Ports4, ";,"), StringSplitAndTrim(Ports6, ";,"));
		a_Settings.DeleteValue(a_KeyName, a_OldIPv4ValueName);
		a_Settings.DeleteValue(a_KeyName, a_OldIPv6ValueName);

		// If those weren't present or were empty, use the default:"
		if (Ports.empty())
		{
			Ports = StringSplitAndTrim(a_DefaultValue, ";,");
		}
		a_Settings.SetValue(a_KeyName, a_PortsValueName, StringsConcat(Ports, ','));
	}

	return Ports;
}



