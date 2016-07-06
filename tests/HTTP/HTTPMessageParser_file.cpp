
// HTTPMessageParser_file.cpp

// Implements a test that feeds file contents into a cHTTPMessageParser instance and prints all callbacks

#include "Globals.h"
#include "HTTP/HTTPMessageParser.h"





/** Maximum size of the input buffer, through which the file is read */
static const size_t MAX_BUF = 4096;





class cCallbacks:
	public cHTTPMessageParser::cCallbacks
{
	typedef cHTTPMessageParser::cCallbacks Super;
public:
	cCallbacks(void)
	{
		printf("cCallbacks created\n");
	}

	// cHTTPResponseParser::cCallbacks overrides:
	virtual void OnError(const AString & a_ErrorDescription) override
	{
		printf("Error: \"%s\"\n", a_ErrorDescription.c_str());
	}

	/** Called when the first line (request / status) is fully parsed. */
	virtual void OnFirstLine(const AString & a_FirstLine) override
	{
		printf("First line: \"%s\"\n", a_FirstLine.c_str());
	}

	/** Called when a single header line is parsed. */
	virtual void OnHeaderLine(const AString & a_Key, const AString & a_Value) override
	{
		printf("Header line: \"%s\": \"%s\"\n", a_Key.c_str(), a_Value.c_str());
	}

	/** Called when all the headers have been parsed. */
	virtual void OnHeadersFinished(void) override
	{
		printf("Headers finished\n");
	}

	/** Called for each chunk of the incoming body data. */
	virtual void OnBodyData(const void * a_Data, size_t a_Size) override
	{
		AString hexDump;
		CreateHexDump(hexDump, a_Data, a_Size, 16);
		printf("Body data: %u bytes\n%s", static_cast<unsigned>(a_Size), hexDump.c_str());
	}

	virtual void OnBodyFinished(void) override
	{
		printf("Body finished\n");
	}
};





int main(int argc, char * argv[])
{
	LOGD("Test started");

	// Open the input file:
	if (argc <= 1)
	{
		printf("Usage: %s <filename> [<buffersize>]\n", argv[0]);
		return 1;
	}
	FILE * f;
	if (strcmp(argv[1], "-") == 0)
	{
		f = stdin;
	}
	else
	{
		f = fopen(argv[1], "rb");
		if (f == nullptr)
		{
			printf("Cannot open file \"%s\". Aborting.\n", argv[1]);
			return 2;
		}
	}

	// If a third param is present, use it as the buffer size
	size_t bufSize = MAX_BUF;
	if (argc >= 3)
	{
		if (!StringToInteger(argv[2], bufSize) || (bufSize == 0))
		{
			bufSize = MAX_BUF;
			printf("\"%s\" is not a valid buffer size, using the default of %u instead.\n", argv[2], static_cast<unsigned>(bufSize));
		}
		if (bufSize > MAX_BUF)
		{
			bufSize = MAX_BUF;
			printf("\"%s\" is too large, maximum buffer size is %u. Using the size %u instead.\n", argv[2], static_cast<unsigned>(bufSize), static_cast<unsigned>(bufSize));
		}
	}

	// Feed the file contents into the parser:
	cCallbacks callbacks;
	cHTTPMessageParser parser(callbacks);
	while (true)
	{
		char buf[MAX_BUF];
		auto numBytes = fread(buf, 1, bufSize, f);
		if (numBytes == 0)
		{
			printf("Read 0 bytes from file (EOF?), terminating\n");
			break;
		}
		auto numConsumed = parser.Parse(buf, numBytes);
		if (numConsumed == AString::npos)
		{
			printf("Parser indicates there was an error, terminating parsing.\n");
			break;
		}
		ASSERT(numConsumed <= numBytes);
		if (numConsumed < numBytes)
		{
			printf("Parser indicates stream end, but there's more data (at least %u bytes) in the file.\n", static_cast<unsigned>(numBytes - numConsumed));
		}
	}
	if (!parser.IsFinished())
	{
		printf("Parser indicates an incomplete stream.\n");
	}

	// Close the input file:
	if (f != stdin)
	{
		fclose(f);
	}

	return 0;
}




