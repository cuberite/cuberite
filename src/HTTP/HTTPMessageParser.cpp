
// HTTPMessageParser.cpp

// Implements the cHTTPMessageParser class that parses HTTP messages (request or response) being pushed into the parser,
// and reports the individual parts via callbacks

#include "Globals.h"
#include "HTTPMessageParser.h"





cHTTPMessageParser::cHTTPMessageParser(cHTTPMessageParser::cCallbacks & a_Callbacks):
	m_Callbacks(a_Callbacks),
	m_EnvelopeParser(*this)
{
	Reset();
}





size_t cHTTPMessageParser::Parse(const char * a_Data, size_t a_Size)
{
	// If parsing already finished or errorred, let the caller keep all the data:
	if (m_IsFinished || m_HasHadError)
	{
		return 0;
	}

	// If still waiting for the status line, add to buffer and try parsing it:
	auto inBufferSoFar = m_Buffer.size();
	if (m_FirstLine.empty())
	{
		m_Buffer.append(a_Data, a_Size);
		auto bytesConsumedFirstLine = ParseFirstLine();
		ASSERT(bytesConsumedFirstLine <= inBufferSoFar + a_Size);  // Haven't consumed more data than there is in the buffer
		ASSERT(bytesConsumedFirstLine > inBufferSoFar);  // Have consumed at least the previous buffer contents
		if (m_FirstLine.empty())
		{
			// All data used, but not a complete status line yet.
			return a_Size;
		}
		if (m_HasHadError)
		{
			return AString::npos;
		}
		// Status line completed, feed the rest of the buffer into the envelope parser:
		auto bytesConsumedEnvelope = m_EnvelopeParser.Parse(m_Buffer.data(), m_Buffer.size());
		if (bytesConsumedEnvelope == AString::npos)
		{
			m_HasHadError = true;
			m_Callbacks.OnError("Failed to parse the envelope");
			return AString::npos;
		}
		ASSERT(bytesConsumedEnvelope <= bytesConsumedFirstLine + a_Size);  // Haven't consumed more data than there was in the buffer
		m_Buffer.erase(0, bytesConsumedEnvelope);
		if (!m_EnvelopeParser.IsInHeaders())
		{
			HeadersFinished();
			// Process any data still left in the buffer as message body:
			auto bytesConsumedBody = ParseBody(m_Buffer.data(), m_Buffer.size());
			if (bytesConsumedBody == AString::npos)
			{
				// Error has already been reported by ParseBody, just bail out:
				return AString::npos;
			}
			return bytesConsumedBody + bytesConsumedEnvelope + bytesConsumedFirstLine - inBufferSoFar;
		}
		return a_Size;
	}  // if (m_FirstLine.empty())

	// If still parsing headers, send them to the envelope parser:
	if (m_EnvelopeParser.IsInHeaders())
	{
		auto bytesConsumed = m_EnvelopeParser.Parse(a_Data, a_Size);
		if (bytesConsumed == AString::npos)
		{
			m_HasHadError = true;
			m_Callbacks.OnError("Failed to parse the envelope");
			return AString::npos;
		}
		if (!m_EnvelopeParser.IsInHeaders())
		{
			HeadersFinished();
			// Process any data still left as message body:
			auto bytesConsumedBody = ParseBody(a_Data + bytesConsumed, a_Size - bytesConsumed);
			if (bytesConsumedBody == AString::npos)
			{
				// Error has already been reported by ParseBody, just bail out:
				return AString::npos;
			}
		}
		return a_Size;
	}

	// Already parsing the body
	return ParseBody(a_Data, a_Size);
}





void cHTTPMessageParser::Reset(void)
{
	m_HasHadError = false;
	m_IsFinished = false;
	m_FirstLine.clear();
	m_Buffer.clear();
	m_EnvelopeParser.Reset();
	m_TransferEncodingParser.reset();
	m_TransferEncoding.clear();
	m_ContentLength = 0;
}




size_t cHTTPMessageParser::ParseFirstLine(void)
{
	auto idxLineEnd = m_Buffer.find("\r\n");
	if (idxLineEnd == AString::npos)
	{
		// Not a complete line yet
		return m_Buffer.size();
	}
	m_FirstLine = m_Buffer.substr(0, idxLineEnd);
	m_Buffer.erase(0, idxLineEnd + 2);
	m_Callbacks.OnFirstLine(m_FirstLine);
	return idxLineEnd + 2;
}




size_t cHTTPMessageParser::ParseBody(const char * a_Data, size_t a_Size)
{
	if (m_TransferEncodingParser == nullptr)
	{
		// We have no Transfer-encoding parser assigned. This should have happened when finishing the envelope
		OnError("No transfer encoding parser");
		return AString::npos;
	}

	// Parse the body using the transfer encoding parser:
	// (Note that TE parser returns the number of bytes left, while we return the number of bytes consumed)
	return a_Size - m_TransferEncodingParser->Parse(a_Data, a_Size);
}





void cHTTPMessageParser::HeadersFinished(void)
{
	m_Callbacks.OnHeadersFinished();
	m_TransferEncodingParser = cTransferEncodingParser::Create(*this, m_TransferEncoding, m_ContentLength);
	if (m_TransferEncodingParser == nullptr)
	{
		OnError(Printf("Unknown transfer encoding: %s", m_TransferEncoding.c_str()));
		return;
	}
}





void cHTTPMessageParser::OnHeaderLine(const AString & a_Key, const AString & a_Value)
{
	m_Callbacks.OnHeaderLine(a_Key, a_Value);
	auto Key = StrToLower(a_Key);
	if (Key == "content-length")
	{
		if (!StringToInteger(a_Value, m_ContentLength))
		{
			OnError(Printf("Invalid content length header value: \"%s\"", a_Value.c_str()));
		}
		return;
	}
	if (Key == "transfer-encoding")
	{
		m_TransferEncoding = a_Value;
		return;
	}
}





void cHTTPMessageParser::OnError(const AString & a_ErrorDescription)
{
	m_HasHadError = true;
	m_Callbacks.OnError(a_ErrorDescription);
}





void cHTTPMessageParser::OnBodyData(const void * a_Data, size_t a_Size)
{
	m_Callbacks.OnBodyData(a_Data, a_Size);
}





void cHTTPMessageParser::OnBodyFinished(void)
{
	m_IsFinished = true;
	m_Callbacks.OnBodyFinished();
}




