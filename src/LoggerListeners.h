
#include "Logger.h"

std::unique_ptr<cLogger::cListener> MakeConsoleListener(bool a_IsService);
std::pair<bool, std::unique_ptr<cLogger::cListener>> MakeFileListener();




