#include "Globals.h"
#include "WorldStorage/FastNBT.h"

int main() {
	std::string test = "";
	cParsedNBT reader{test.c_str(), test.size()};
	cFastNBTWriter writer{};
}
