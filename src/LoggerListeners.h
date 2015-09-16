
#include "Logger.h"
#include "OSSupport/File.h"

std::unique_ptr<cLogger::cListener> MakeConsoleListener(bool a_IsService);
std::pair<bool, std::unique_ptr<cLogger::cListener>> MakeFileListener();




