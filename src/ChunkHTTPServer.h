
// ChunkHTTPServer.h

#pragma once

#include "HTTP/HTTPServer.h"
#include "Generating/ChunkGenerator.h"
#include "Generating/ChunkDesc.h"

class ChunkHTTPServer :
	public cHTTPServer::cCallbacks
{
public:

	ChunkHTTPServer(void);
	virtual ~ChunkHTTPServer() override;

	/** Initializes the object. Returns true if successfully initialized and ready to start */
	bool Init(void);

	/** Starts the HTTP server taking care of the Chunk generation. Returns true if successful */
	bool Start(void);

	/** Stops the HTTP server, if it was started. */
	void Stop(void);

protected:

	/** Set to true if Init() succeeds and the webadmin isn't to be disabled */
	bool m_IsInitialized;

	/** Set to true if Start() succeeds in starting the server, reset back to false in Stop(). */
	bool m_IsRunning;

	/** The HTTP server which provides the underlying HTTP parsing, serialization and events */
	cHTTPServer m_HTTPServer;

	std::map<AString, std::unique_ptr<cChunkGenerator>> m_GeneratorMap;

	// cHTTPServer::cCallbacks overrides:
	virtual void OnRequestBegun   (cHTTPServerConnection & a_Connection, cHTTPIncomingRequest & a_Request) override;
	virtual void OnRequestBody    (cHTTPServerConnection & a_Connection, cHTTPIncomingRequest & a_Request, const char * a_Data, size_t a_Size) override;
	virtual void OnRequestFinished(cHTTPServerConnection & a_Connection, cHTTPIncomingRequest & a_Request) override;
} ;





// Revert MSVC warnings back to orignal state:
#if defined(_MSC_VER)
	#pragma warning(pop)
#endif




