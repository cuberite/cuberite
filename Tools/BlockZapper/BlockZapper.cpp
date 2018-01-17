
// BlockZapper.cpp

// Implements the main app entrypoint

#include "Globals.h"

#include <fstream>

#include "Regions.h"
#include "Zapper.h"





#ifdef _MSC_VER
	// Under MSVC, link to WinSock2 (needed by FastNBT's byteswapping)
	#pragma comment(lib, "ws2_32.lib")
#endif





void ShowHelp(const char * a_ProgramFullName)
{
	AString ProgramName(a_ProgramFullName);
	size_t idx = ProgramName.rfind(cFile::PathSeparator);
	if (idx != AString::npos)
	{
		ProgramName.erase(0, idx + 1);
	}
	printf("Tool written by _Xoft(o), code is public domain.\n");
	printf("Usage:\n");
	printf("%s [-w <MCAFolder>]\n", ProgramName.c_str());
	printf("Zaps blocks and / or entities in specified regions.\n");
	printf("Regions are read from stdin, the format is:\n");
	printf("  x1 x2 y1 y2 z1 z2 [B|E|BE]\n");
	printf("B or no specifier zaps blocks only\n");
	printf("E zaps entities only\n");
	printf("BE zaps blocks and entities\n");
	printf("MCA files are searched in the <MCAFolder>; if not specified, in the current folder.\n");
}





int main(int argc, char * argv[])
{
	new cMCLogger;  // Create a new logger, it will assign itself as the main logger instance

	AString MCAFolder = ".";
	for (int i = 1; i < argc; i++)
	{
		if (strcmp(argv[i], "-w") == 0)
		{
			if (i < argc - 1)
			{
				MCAFolder = argv[i + 1];
			}
			i++;
		}
		else if (
			(strcmp(argv[i], "help") == 0) ||
			(strcmp(argv[i], "-?") == 0) ||
			(strcmp(argv[i], "/?") == 0) ||
			(strcmp(argv[i], "-h") == 0) ||
			(strcmp(argv[i], "--help") == 0)
		)
		{
			ShowHelp(argv[0]);
			return 0;
		}
	}

	cRegions Regions;

	/*
	// DEBUG: Read input from a file instead of stdin:
	std::fstream fs("test_in.txt");
	Regions.Read(fs);
	//*/

	Regions.Read(std::cin);

	cZapper Zapper(MCAFolder);
	Zapper.ZapRegions(Regions.GetAll());

	LOGINFO("Done");
	return 0;
} ;
