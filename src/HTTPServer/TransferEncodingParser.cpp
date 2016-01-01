
// TransferEncodingParser.cpp

// Implements the cTransferEncodingParser class and its descendants representing the parser for the various transfer encodings (chunked etc.)

#include "Globals.h"
#include "TransferEncodingParser.h"





////////////////////////////////////////////////////////////////////////////////
// cChunkedTEParser:

class cChunkedTEParser:
	public cTransferEncodingParser
{
	typedef cTransferEncodingParser Super;

public:
	cChunkedTEParser(cCallbacks & a_Callbacks):
		Super(a_Callbacks),
		m_IsFinished(false)
	{
	}


protected:

	/** True if the datastream has finished (zero-length chunk received). */
	bool m_IsFinished;


	// cTransferEncodingParser overrides:
	virtual size_t Parse(const char * a_Data, size_t a_Size) override
	{
		// TODO
		m_Callbacks.OnError("cChunkedTEParser not implemented yet");
		return AString::npos;
	}

	virtual void Finish(void) override
	{
		if (!m_IsFinished)
		{
			m_Callbacks.OnError("ChunkedTransferEncoding: Finish signal received before the data stream ended");
		}
		m_IsFinished = true;
	}
};





////////////////////////////////////////////////////////////////////////////////
// cIdentityTEParser:

class cIdentityTEParser:
	public cTransferEncodingParser
{
	typedef cTransferEncodingParser Super;

public:
	cIdentityTEParser(cCallbacks & a_Callbacks, size_t a_ContentLength):
		Super(a_Callbacks),
		m_BytesLeft(a_ContentLength)
	{
	}


protected:
	/** How many bytes of content are left before the message ends. */
	size_t m_BytesLeft;

	// cTransferEncodingParser overrides:
	virtual size_t Parse(const char * a_Data, size_t a_Size) override
	{
		auto size = std::min(a_Size, m_BytesLeft);
		if (size > 0)
		{
			m_Callbacks.OnBodyData(a_Data, size);
		}
		m_BytesLeft -= size;
		if (m_BytesLeft == 0)
		{
			m_Callbacks.OnBodyFinished();
		}
		return a_Size - size;
	}

	virtual void Finish(void) override
	{
		if (m_BytesLeft > 0)
		{
			m_Callbacks.OnError("IdentityTransferEncoding: body was truncated");
		}
		else
		{
			// BodyFinished has already been called, just bail out
		}
	}
};





////////////////////////////////////////////////////////////////////////////////
// cTransferEncodingParser:

cTransferEncodingParserPtr cTransferEncodingParser::Create(
	cCallbacks & a_Callbacks,
	const AString & a_TransferEncoding,
	size_t a_ContentLength
)
{
	if (a_TransferEncoding == "chunked")
	{
		return std::make_shared<cChunkedTEParser>(a_Callbacks);
	}
	if (a_TransferEncoding.empty())
	{
		return std::make_shared<cIdentityTEParser>(a_Callbacks, a_ContentLength);
	}
	return nullptr;
}




