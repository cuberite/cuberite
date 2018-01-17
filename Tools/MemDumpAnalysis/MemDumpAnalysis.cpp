
// MemDumpAnalysis.cpp

// Defines the entry point for the console application.

#include "Globals.h"

#ifdef _WIN32
	#pragma comment(lib, "ws2_32.lib")  // Needed for StringUtils' RawBEToUtf8() et al.
#endif  // _WIN32





typedef std::set<AString> AStringSet;





class cFunction
{
public:
	int        m_Size;   ///< Sum of memory block sizes allocated by this function or its children
	int        m_Count;  ///< Total number of memory blocks allocated by this function or its children
	AStringSet m_ChildrenNames;

	cFunction(void) :
		m_Size(0),
		m_Count(0)
	{
	}
} ;

typedef std::map<AString, cFunction> FunctionMap;





int g_CurrentID = 0;
int g_CurrentSize = 0;
FunctionMap g_FnMap;
AString g_PrevFunctionName;





bool IsFnBlackListed(const char * a_FnName)
{
	static const char * BlackList[] =
	{
		"MyAllocHook",
		"_heap_alloc_dbg_impl",
		"_nh_malloc_dbg_impl",
		"_nh_malloc_dbg",
		"malloc",
		"operator new",
		"_malloc_dbg",
		"realloc_help",
		"_realloc_dbg",
		"realloc",
		"l_alloc",
		"luaM_realloc_",
		"",
	} ;

	for (int i = 0; i < ARRAYCOUNT(BlackList); i++)
	{
		if (strcmp(BlackList[i], a_FnName) == 0)
		{
			return true;
		}
	}
	return false;
}





const char * FindAttr(const char ** a_Attrs, const char * a_AttrName)
{
	for (const char ** Attr = a_Attrs; *Attr != NULL; Attr += 2)
	{
		if (strcmp(*Attr, a_AttrName) == 0)
		{
			return *(Attr + 1);
		}
	}  // for Attr - a_Attrs[]
	return NULL;
}





void OnStartElement(void * a_Data, const char * a_Element, const char ** a_Attrs)
{
	if (strcmp(a_Element, "LEAK") == 0)
	{
		const char * attrID   = FindAttr(a_Attrs, "requestID");
		const char * attrSize = FindAttr(a_Attrs, "size");
		g_CurrentID   = atoi((attrID   == NULL) ? "-1" : attrID);
		g_CurrentSize = atoi((attrSize == NULL) ? "-1" : attrSize);
		g_PrevFunctionName.clear();
		return;
	}
	if (strcmp(a_Element, "STACKENTRY") == 0)
	{
		const char * fnName = FindAttr(a_Attrs, "decl");
		if (fnName == NULL)
		{
			g_CurrentID = -1;
			g_CurrentSize = -1;
			return;
		}
		if (g_CurrentSize < 0)
		{
			return;
		}
		if (IsFnBlackListed(fnName))
		{
			return;
		}
		AString FunctionName = fnName;
		cFunction & Function = g_FnMap[FunctionName];
		Function.m_Size += g_CurrentSize;
		Function.m_Count += 1;
		if (!g_PrevFunctionName.empty())
		{
			Function.m_ChildrenNames.insert(g_PrevFunctionName);
		}
		std::swap(g_PrevFunctionName, FunctionName);  // We only care about moving FunctionName into g_PrevFunctionName
		return;
	}
}





void OnEndElement(void * a_Data, const char * a_Element)
{
	if (strcmp(a_Element, "LEAK") == 0)
	{
		g_CurrentID = -1;
		g_CurrentSize = -1;
		return;
	}
}





bool CompareFnInt(const std::pair<AString, int> & a_First, const std::pair<AString, int> & a_Second)
{
	return (a_First.second < a_Second.second);
}





void WriteSizeStatistics(void)
{
	typedef std::vector<std::pair<AString, int> > StringIntPairs;
	StringIntPairs FnSizes;

	cFile f("memdump_totals.txt", cFile::fmWrite);
	if (!f.IsOpen())
	{
		LOGERROR("Cannot open memdump_totals.txt");
		return;
	}

	for (FunctionMap::iterator itr = g_FnMap.begin(), end = g_FnMap.end(); itr != end; ++itr)
	{
		FnSizes.push_back(std::pair<AString, int>(itr->first, itr->second.m_Size));
	}  // for itr - g_FnSizes[]
	std::sort(FnSizes.begin(), FnSizes.end(), CompareFnInt);

	for (StringIntPairs::const_iterator itr = FnSizes.begin(), end = FnSizes.end(); itr != end; ++itr)
	{
		f.Printf("%d\t%s\n", itr->second, itr->first.c_str());
	}  // for itr - FnSizes[]
}





void WriteCountStatistics(void)
{
	typedef std::vector<std::pair<AString, int> > StringIntPairs;
	StringIntPairs FnCounts;

	cFile f("memdump_counts.txt", cFile::fmWrite);
	if (!f.IsOpen())
	{
		LOGERROR("Cannot open memdump_counts.txt");
		return;
	}

	for (FunctionMap::iterator itr = g_FnMap.begin(), end = g_FnMap.end(); itr != end; ++itr)
	{
		FnCounts.push_back(std::pair<AString, int>(itr->first, itr->second.m_Count));
	}  // for itr - g_FnSizes[]
	std::sort(FnCounts.begin(), FnCounts.end(), CompareFnInt);

	for (StringIntPairs::const_iterator itr = FnCounts.begin(), end = FnCounts.end(); itr != end; ++itr)
	{
		f.Printf("%d\t%s\n", itr->second, itr->first.c_str());
	}  // for itr - FnSizes[]
}





AString HTMLEscape(const AString & a_Text)
{
	AString res;
	res.reserve(a_Text.size());
	size_t len = a_Text.length();
	for (size_t i = 0; i < len; i++)
	{
		switch (a_Text[i])
		{
			case '<': res.append("&lt;<BR/>"); break;
			case '>': res.append("<BR/>&gt;"); break;
			case '&': res.append("&amp;"); break;
			default:
			{
				res.push_back(a_Text[i]);
			}
		}
	}  // for i - a_Text[]
	return res;
}





void WriteDotGraph(void)
{
	cFile f("memdump.dot", cFile::fmWrite);
	if (!f.IsOpen())
	{
		LOGERROR("Cannot open memdump.dot");
		return;
	}

	f.Printf("digraph {\n\tnode [shape=plaintext]\n\n");
	for (FunctionMap::const_iterator itrF = g_FnMap.begin(), endF = g_FnMap.end(); itrF != endF; ++itrF)
	{
		f.Printf("\t\"%s\" [label=<%s<BR/>%d bytes (%d KiB)<BR/>%d blocks>]\n",
			itrF->first.c_str(),
			HTMLEscape(itrF->first).c_str(),
			itrF->second.m_Size,
			(itrF->second.m_Size + 1023) / 1024,
			itrF->second.m_Count
		);
		const AStringSet & Children = itrF->second.m_ChildrenNames;
		for (AStringSet::const_iterator itrN = Children.begin(), endN = Children.end(); itrN != endN; ++itrN)
		{
			f.Printf("\t\t\"%s\" -> \"%s\"\n", itrF->first.c_str(), itrN->c_str());
		}
		f.Printf("\n");
	}  // for itr
	f.Printf("}\n");
}





int main(int argc, char * argv[])
{
	// Open the dump file:
	cFile f("memdump.xml", cFile::fmRead);
	if (!f.IsOpen())
	{
		printf("Cannot open memdump.xml\n");
		return 1;
	}

	// Create the XML parser:
	XML_Parser Parser = XML_ParserCreate(NULL);
	XML_SetElementHandler(Parser, OnStartElement, OnEndElement);

	// Feed the file through XML parser:
	char Buffer[512 KiB];
	while (true)
	{
		int NumBytes = f.Read(Buffer, sizeof(Buffer));
		if (NumBytes <= 0)
		{
			break;
		}
		XML_Parse(Parser, Buffer, NumBytes, false);
		putc('.', stdout);
	}
	XML_Parse(Parser, "", 0, true);
	f.Close();

	// Output the statistics
	WriteSizeStatistics();
	WriteCountStatistics();
	WriteDotGraph();

	return 0;
}
