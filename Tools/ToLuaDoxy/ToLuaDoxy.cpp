
// ToLuaDoxy.cpp

// Implements the main app entrypoint

#include "Globals.h"
#include <fstream>
#include <iostream>





typedef std::vector<AString> AStrings;





class cProcessor
{
public:
	cProcessor(const AString & a_FileOut) :
		m_Out(a_FileOut.c_str(), std::ios::out),
		m_IsInToLua(false),
		m_IsInComment(false)
	{
	}


	bool IsGood(void) const
	{
		return !m_Out.fail();
	}


	void ProcessFile(const AString & a_FileIn)
	{
		std::ifstream In(a_FileIn.c_str());
		if (In.fail())
		{
			std::cerr << "Cannot open input file " << a_FileIn << "." << std::endl;
			return;
		}
		while (!In.eof())
		{
			AString Line;
			std::getline(In, Line);
			PushLine(Line);
		}
	}

protected:
	std::ofstream m_Out;
	bool          m_IsInToLua;    ///< Set to true if inside a tolua_begin .. tolua_end block
	bool          m_IsInComment;  ///< Set to true if previous line has started a multiline comment; only outside tolua blocks
	AString       m_LastComment;  ///< Accumulator for a multiline comment preceding a tolua block


	void PushLine(const AString & a_Line)
	{
		if (m_IsInToLua)
		{
			// Inside a tolua block
			if (TrimString(a_Line) == "// tolua_end")
			{
				// End of a tolua block
				m_IsInToLua = false;
				return;
			}
			m_Out << a_Line << std::endl;
			return;
		}

		if (m_IsInComment)
		{
			// Inside a multiline comment block, outside of a tolua block; accumulate m_LastComment
			m_LastComment += a_Line + "\n";
			m_IsInComment = (a_Line.find("*/") == AString::npos);
			return;
		}

		AString Trimmed(TrimString(a_Line));

		if (Trimmed == "// tolua_begin")
		{
			// Beginning of a tolua block
			m_IsInToLua = true;
			if (!m_LastComment.empty())
			{
				m_Out << m_LastComment << std::endl;
				m_LastComment.clear();
			}
			return;
		}

		size_t CommentBegin = a_Line.find("/*");
		if (CommentBegin != AString::npos)
		{
			m_IsInComment = (a_Line.find("*/", CommentBegin) == AString::npos);
			m_LastComment = a_Line;
		}

		size_t ExportIdx = a_Line.find("// tolua_export");
		if (ExportIdx != AString::npos)
		{
			// Single-line tolua block

			// Strip the export comment and right-trim the line:
			AString Stripped(a_Line.substr(0, ExportIdx));
			int End = Stripped.length() - 1;
			while ((End > 0) && (Stripped[End] <= 32))
			{
				End--;
			}
			Stripped.erase(End + 1);

			if (!m_LastComment.empty())
			{
				m_Out << m_LastComment << std::endl;
				m_LastComment.clear();
			}
			m_Out << Stripped << std::endl;
			return;
		}

		if (!m_IsInComment)
		{
			m_LastComment.clear();
		}
	}
} ;





/** Parses the specified package file into a list of $cfile-included files and all the other contents
Returns true if successful.
Returns false and prints error if unsuccessful
*/
bool ParsePackageFile(const AString & a_FileName, AStrings & a_CFiles, AStrings & a_DirectContentsLines)
{
	std::ifstream PkgFile(a_FileName.c_str());
	if (PkgFile.fail())
	{
		std::cerr << "Cannot open the package file " << a_FileName << "." << std::endl;
		return false;
	}

	while (!PkgFile.eof())
	{
		AString Line;
		std::getline(PkgFile, Line);
		Line = TrimString(Line);
		if (strncmp(Line.c_str(), "$cfile \"", 8) == 0)
		{
			a_CFiles.push_back(Line.substr(8, Line.length() - 9));
		}
		else
		{
			a_DirectContentsLines.push_back(Line);
		}
	}
	return true;
}





/** Processes the specified input header file into the output file */
void ProcessCFile(const AString & a_CFileIn, const AString & a_CFileOut)
{
	cProcessor p(a_CFileOut);
	if (!p.IsGood())
	{
		std::cerr << "Cannot open output file " << a_CFileOut << "." << std::endl;
		return;
	}
	p.ProcessFile(a_CFileIn);
}





int main(int argc, char * argv[])
{
	AString BaseDir = (argc > 1) ? argv[1] : ".";
	AString OutDir  = (argc > 2) ? argv[2] : "Out";

	// Create the output directory:
	#ifdef _WIN32
	CreateDirectory(OutDir.c_str(), NULL);
	#else
	mkdir(OutDir.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);
	#endif

	// Parse the package file
	AStrings CFiles;
	AStrings DirectLines;
	if (!ParsePackageFile(Printf("%s/AllToLua.pkg", BaseDir.c_str()), CFiles, DirectLines))
	{
		return 1;
	}

	// Process header files:
	for (AStrings::const_iterator itr = CFiles.begin(), end = CFiles.end(); itr != end; ++itr)
	{
		static int cnt = 0;
		AString In = Printf("%s/%s", BaseDir.c_str(), itr->c_str());
		AString Out = Printf("%s/%04x.h", OutDir.c_str(), cnt++);
		ProcessCFile(In, Out);
	}  // for itr - CFiles[]

	return 0;
}
