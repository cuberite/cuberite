
cmake_minimum_required (VERSION 2.6)
project (MCServer)

include_directories ("${PROJECT_SOURCE_DIR}/../")
set(SOURCE CriticalSection Timer Thread Sleep IsThread Event SocketThreads ListenThread)
set(SOURCE ${SOURCE} BlockingTCPLink Socket File)

add_library(OSSupport ${SOURCE})
