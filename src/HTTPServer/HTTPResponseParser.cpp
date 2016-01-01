
// HTTPResponseParser.cpp

// Implements the cHTTPResponseParser class representing the parser for incoming HTTP responses

#include "Globals.h"
#include "HTTPResponseParser.h"





cHTTPResponseParser::cHTTPResponseParser(cHTTPResponseParser::cCallbacks & a_Callbacks):
	Super(mkResponse),
	m_Callbacks(a_Callbacks),
	m_IsInHeaders(true),
	m_IsFinished(false),
	m_EnvelopeParser(*this)
{
}





size_t cHTTPResponseParser::Parse(const char * a_Data, size_t a_Size)
{
	// If parsing already finished or errorred, let the caller keep all the data:
	if (m_IsFinished || m_HasHadError)
	{
		return a_Size;
	}

	// If still waiting for the status line, add to buffer and try parsing it:
	if (m_StatusLine.empty())
	{
		m_Buffer.append(a_Data, a_Size);
		if (!ParseStatusLine())
		{
			// All data used, but not a complete status line yet.
			return 0;
		}
		if (m_HasHadError)
		{
			return AString::npos;
		}
		// Status line completed, feed the rest of the buffer into the envelope parser:
		auto bytesConsumed = m_EnvelopeParser.Parse(m_Buffer.data(), m_Buffer.size());
		if (bytesConsumed == AString::npos)
		{
			m_HasHadError = true;
			m_Callbacks.OnError("Failed to parse the envelope");
			return AString::npos;
		}
		m_Buffer.erase(0, bytesConsumed);
		if (!m_Buffer.empty())
		{
			// Headers finished and there's still data left in the buffer, process it as message body:
			m_IsInHeaders = false;
			return ParseBody(m_Buffer.data(), m_Buffer.size());
		}
		return 0;
	}  // if (m_StatusLine.empty())

	// If still parsing headers, send them to the envelope parser:
	if (m_IsInHeaders)
	{
		auto bytesConsumed = m_EnvelopeParser.Parse(a_Data, a_Size);
		if (bytesConsumed == AString::npos)
		{
			m_HasHadError = true;
			m_Callbacks.OnError("Failed to parse the envelope");
			return AString::npos;
		}
		if (bytesConsumed < a_Size)
		{
			// Headers finished and there's still data left in the buffer, process it as message body:
			HeadersFinished();
			return ParseBody(a_Data + bytesConsumed, a_Size - bytesConsumed);
		}
		return 0;
	}

	// Already parsing the body
	return ParseBody(a_Data, a_Size);
}





bool cHTTPResponseParser::ParseStatusLine(void)
{
	auto idxLineEnd = m_Buffer.find("\r\n");
	if (idxLineEnd == AString::npos)
	{
		// Not a complete line yet
		return false;
	}
	m_StatusLine = m_Buffer.substr(0, idxLineEnd);
	m_Buffer.erase(0, idxLineEnd + 2);
	m_Callbacks.OnStatusLine(m_StatusLine);
	return true;
}




size_t cHTTPResponseParser::ParseBody(const char * a_Data, size_t a_Size)
{
	if (m_TransferEncodingParser == nullptr)
	{
		// We have no Transfer-encoding parser assigned. This should have happened when finishing the envelope
		return AString::npos;
	}

	// Parse the body using the transfer encoding parser:
	return m_TransferEncodingParser->Parse(a_Data, a_Size);
}





void cHTTPResponseParser::HeadersFinished(void)
{
	m_IsInHeaders = false;
	m_Callbacks.OnHeadersFinished();

	auto transferEncoding = m_Headers.find("transfer-encoding");
	if (transferEncoding == m_Headers.end())
	{
		m_TransferEncodingParser = cTransferEncodingParser::Create(*this, "identity", m_ContentLength);
	}
}





void cHTTPResponseParser::OnHeaderLine(const AString & a_Key, const AString & a_Value)
{
	AddHeader(a_Key, a_Value);
	m_Callbacks.OnHeaderLine(a_Key, a_Value);
}





void cHTTPResponseParser::OnError(const AString & a_ErrorDescription)
{
	m_HasHadError = true;
	m_Callbacks.OnError(a_ErrorDescription);
}





void cHTTPResponseParser::OnBodyData(const void * a_Data, size_t a_Size)
{
	m_Callbacks.OnBodyData(a_Data, a_Size);
}





void cHTTPResponseParser::OnBodyFinished(void)
{
	m_Callbacks.OnBodyFinished();
}




