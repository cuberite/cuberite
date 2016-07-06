
// LoadablePieces.cpp

// Implements the LoadablePieces test main entrypoint

#include "Globals.h"
#ifdef _WIN32
	#include <direct.h>
	#define GetCurrentFolder _getcwd
#else
	#include <unistd.h>
	#define GetCurrentFolder getcwd
#endif
#include "Generating/PrefabPiecePool.h"





static int DoTest(void)
{
	cPrefabPiecePool test;
	auto res = test.LoadFromFile("Test.cubeset", true);
	if (!res)
	{
		LOGWARNING("Loading from file \"Test.cubeset\" failed.");
		return 1;
	}
	LOG("Loaded %u regular pieces and %u starting pieces", static_cast<unsigned>(test.GetAllPiecesCount()), static_cast<unsigned>(test.GetStartingPiecesCount()));

	// Check that we loaded all the pieces:
	testassert(test.GetAllPiecesCount() == 1);
	testassert(test.GetStartingPiecesCount() == 1);

	return 0;
}





int main(int argc, char * argv[])
{
	LOGD("Test started");

	// Print the current directory for reference:
	char folder[FILENAME_MAX];
	GetCurrentFolder(folder, sizeof(folder));
	LOG("Running cPrefabPiecePool test from folder \"%s\".", folder);

	// Run the test:
	int res = DoTest();
	LOG("cPrefabPiecePool loading test done: %s", (res == 0) ? "success" : "failure");
	return res;
}




