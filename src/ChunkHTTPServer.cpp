
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "ChunkHTTPServer.h"

#include "HTTP/HTTPMessage.h"
#include "HTTP/HTTPServerConnection.h"

#include "WorldStorage/SchematicFileSerializer.h"
#include "WorldStorage/NBTChunkSerializer.h"
#include "WorldStorage/FastNBT.h"
#include "StringCompression.h"

#include <regex>





static const char DEFAULT_CHUNCKHTTPSERVER_PORTS[] = "5555";





////////////////////////////////////////////////////////////////////////////////
// ChunkHTTPServer:

ChunkHTTPServer::ChunkHTTPServer(void) :
	m_IsInitialized(false),
	m_IsRunning(false)
{
}





ChunkHTTPServer::~ChunkHTTPServer()
{
	ASSERT(!m_IsRunning);  // Was the HTTP server stopped properly?
}





bool ChunkHTTPServer::Init(void)
{
	LOGD("Initialising Chunck HTTP Server...");

	if (!m_HTTPServer.Initialize())
	{
		return false;
	}

	m_IsInitialized = true;

	return true;
}





bool ChunkHTTPServer::Start(void)
{
	if (!m_IsInitialized)
	{
		// Not initialized
		return false;
	}

	LOGD("Starting Chunck HTTP Server...");

	AStringVector Ports =
	{
		DEFAULT_CHUNCKHTTPSERVER_PORTS
	};

	m_IsRunning = m_HTTPServer.Start(*this, Ports);
	return m_IsRunning;
}





void ChunkHTTPServer::Stop(void)
{
	if (!m_IsRunning)
	{
		return;
	}

	LOGD("Stopping Chunck HTTP Server...");
	m_HTTPServer.Stop();
	m_IsRunning = false;
}





class ChunkHTTPServerUserData:
	public cHTTPIncomingRequest::cUserData
{
	public:
	AString m_Data;
};




void ChunkHTTPServer::OnRequestBegun(cHTTPServerConnection & a_Connection, cHTTPIncomingRequest & a_Request)
{
	UNUSED(a_Connection);

	LOGD("OnRequestBegun");

	a_Request.SetUserData(std::make_shared<ChunkHTTPServerUserData>());
}





void ChunkHTTPServer::OnRequestBody(cHTTPServerConnection & a_Connection, cHTTPIncomingRequest & a_Request, const char * a_Data, size_t a_Size)
{
	UNUSED(a_Connection);

	LOGD("OnRequestBody (%lld)", (long long)a_Size);

	std::static_pointer_cast<ChunkHTTPServerUserData>(a_Request.GetUserData())->m_Data.append(a_Data, a_Size);
}





void ChunkHTTPServer::OnRequestFinished(cHTTPServerConnection & a_Connection, cHTTPIncomingRequest & a_Request)
{
	const AString & URL = a_Request.GetURL();
	std::smatch m;
	const std::regex Initialize_regexp    ("/([-_[:alnum:]]+)/Initialize");
	const std::regex GenerateBiomes_regexp("/([-_[:alnum:]]+)/GenerateBiomes/(-?[[:digit:]]+);(-?[[:digit:]]+)");
	const std::regex Generate_regexp      ("/([-_[:alnum:]]+)/Generate/(-?[[:digit:]]+);(-?[[:digit:]]+)");

	cHTTPOutgoingResponse Resp;

	if (std::regex_match(URL, m, Initialize_regexp))
	{
		Resp.SetContentType("text/html");
		a_Connection.Send(Resp);

		cIniFile IniFile;
		IniFile.ReadString(std::static_pointer_cast<ChunkHTTPServerUserData>(a_Request.GetUserData())->m_Data);

		LOGD("Creating %s Generator", m[1].str().c_str());
		m_GeneratorMap[m[1]] = cChunkGenerator::CreateFromIniFile(IniFile);

		a_Connection.Send("OK");
	}
	else if (std::regex_match(URL, m, GenerateBiomes_regexp))
	{
		LOGD("%s GenerateBiomes (%s,%s)", m[1].str().c_str(), m[2].str().c_str(), m[3].str().c_str());

		cChunkCoords Coords(stoi(m[2]), stoi(m[3]));
		cChunkDef::BiomeMap BiomeMap;
		m_GeneratorMap[m[1]]->GenerateBiomes(Coords, BiomeMap);

		AString Data;

		cFastNBTWriter Writer;
		NBTChunkSerializer::serializeBiomeMap(BiomeMap, Writer);
		Writer.Finish();

		AString NBTString = Writer.GetResult();

		int res = CompressStringGZIP(NBTString.data(), NBTString.size(), Data);
		if (res != Z_OK)
		{
			LOG("%s: Cannot Gzip the area data NBT representation: %d", __FUNCTION__, res);
		}

		Resp.SetContentType("application/bin");
		a_Connection.Send(Resp);
		a_Connection.Send(Data);
	}
	else if (std::regex_match(URL, m, Generate_regexp))
	{
		LOGD("%s Generate (%s,%s)", m[1].str().c_str(), m[2].str().c_str(), m[3].str().c_str());

		cChunkCoords Coords(stoi(m[2]), stoi(m[3]));
		cChunkDesc ChunkDesc(Coords);
		m_GeneratorMap[m[1]]->Generate(ChunkDesc);

		AString Data;

		cFastNBTWriter Writer;
		NBTChunkSerializer::serializeChunkDesc(ChunkDesc, Writer);
		Writer.Finish();

		AString NBTString = Writer.GetResult();

		int res = CompressStringGZIP(NBTString.data(), NBTString.size(), Data);
		if (res != Z_OK)
		{
			LOG("%s: Cannot Gzip the area data NBT representation: %d", __FUNCTION__, res);
		}

		Resp.SetContentType("application/bin");
		a_Connection.Send(Resp);
		a_Connection.Send(Data);
	}
	else
	{
		Resp.SetContentType("text/html");
		a_Connection.Send(Resp);
		a_Connection.Send("<h2>404 Not Found</h2>");
		LOG("Unknown URL : %s", URL.c_str());
	}

	a_Connection.FinishResponse();
}
