target_sources(
	${CMAKE_PROJECT_NAME} PRIVATE

	CriticalSection.cpp
	Event.cpp
	File.cpp
	GZipFile.cpp
	HostnameLookup.cpp
	IPLookup.cpp
	IsThread.cpp
	NetworkInterfaceEnum.cpp
	NetworkLookup.cpp
	NetworkSingleton.cpp
	ServerHandleImpl.cpp
	StackTrace.cpp
	TCPLinkImpl.cpp
	UDPEndpointImpl.cpp
	WinStackWalker.cpp

	AtomicUniquePtr.h
	ConsoleSignalHandler.h
	CriticalSection.h
	Event.h
	File.h
	GetAddressInfoError.h
	GZipFile.h
	HostnameLookup.h
	IPLookup.h
	IsThread.h
	MiniDumpWriter.h
	Network.h
	NetworkLookup.h
	NetworkSingleton.h
	Queue.h
	ServerHandleImpl.h
	SleepResolutionBooster.h
	StackTrace.h
	StartAsService.h
	Stopwatch.h
	TCPLinkImpl.h
	UDPEndpointImpl.h
	WinStackWalker.h
)

