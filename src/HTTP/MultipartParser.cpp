
// MultipartParser.cpp

// Implements the cMultipartParser class that parses messages in "multipart/*" encoding into the separate parts

#include "Globals.h"
#include "MultipartParser.h"
#include "NameValueParser.h"





// Disable MSVC warnings:
#if defined(_MSC_VER)
	#pragma warning(push)
	#pragma warning(disable:4355)  // 'this' : used in base member initializer list
#endif





////////////////////////////////////////////////////////////////////////////////
// self-test:

#if 0

class cMultipartParserTest :
	public cMultipartParser::cCallbacks
{
public:
	cMultipartParserTest(void)
	{
		cMultipartParser Parser("multipart/mixed; boundary=\"MyBoundaryString\"; foo=bar", *this);
		const char Data[] =
"ThisIsIgnoredPrologue\r\n\
--MyBoundaryString\r\n\
\r\n\
Body with confusing strings\r\n\
--NotABoundary\r\n\
--MyBoundaryStringWithPostfix\r\n\
--\r\n\
--MyBoundaryString\r\n\
content-disposition: inline\r\n\
\r\n\
This is body\r\n\
--MyBoundaryString\r\n\
\r\n\
Headerless body with trailing CRLF\r\n\
\r\n\
--MyBoundaryString--\r\n\
ThisIsIgnoredEpilogue";
		printf("Multipart parsing test commencing.\n");
		Parser.Parse(Data, sizeof(Data) - 1);
		// DEBUG: Check if the onscreen output corresponds with the data above
		printf("Multipart parsing test finished\n");
	}

	virtual void OnPartStart(void) override
	{
		printf("Starting a new part\n");
	}


	virtual void OnPartHeader(const AString & a_Key, const AString & a_Value) override
	{
		printf("  Hdr: \"%s\"=\"%s\"\n", a_Key.c_str(), a_Value.c_str());
	}


	virtual void OnPartData(const char * a_Data, int a_Size) override
	{
		printf("  Data: %d bytes, \"%.*s\"\n", a_Size, a_Size, a_Data);
	}


	virtual void OnPartEnd(void) override
	{
		printf("Part end\n");
	}
} g_Test;

#endif





////////////////////////////////////////////////////////////////////////////////
// cMultipartParser:


cMultipartParser::cMultipartParser(const AString & a_ContentType, cCallbacks & a_Callbacks) :
	m_Callbacks(a_Callbacks),
	m_IsValid(true),
	m_EnvelopeParser(*this),
	m_HasHadData(false)
{
	// Check that the content type is multipart:
	AString ContentType(a_ContentType);
	if (strncmp(ContentType.c_str(), "multipart/", 10) != 0)
	{
		m_IsValid = false;
		return;
	}
	size_t idxSC = ContentType.find(';', 10);
	if (idxSC == AString::npos)
	{
		m_IsValid = false;
		return;
	}

	// Find the multipart boundary:
	ContentType.erase(0, idxSC + 1);
	cNameValueParser CTParser(ContentType.c_str(), ContentType.size());
	CTParser.Finish();
	if (!CTParser.IsValid())
	{
		m_IsValid = false;
		return;
	}
	m_Boundary = CTParser["boundary"];
	m_IsValid = !m_Boundary.empty();
	if (!m_IsValid)
	{
		return;
	}

	// Set the envelope parser for parsing the body, so that our Parse() function parses the ignored prefix data as a body
	m_EnvelopeParser.SetIsInHeaders(false);

	// Append an initial CRLF to the incoming data, so that a body starting with the boundary line will get caught
	m_IncomingData.assign("\r\n");

	/*
	m_Boundary = AString("\r\n--") + m_Boundary
	m_BoundaryEnd = m_Boundary + "--\r\n";
	m_Boundary = m_Boundary + "\r\n";
	*/
}





void cMultipartParser::Parse(const char * a_Data, size_t a_Size)
{
	// Skip parsing if invalid
	if (!m_IsValid)
	{
		return;
	}

	// Append to buffer, then parse it:
	m_IncomingData.append(a_Data, a_Size);
	for (;;)
	{
		if (m_EnvelopeParser.IsInHeaders())
		{
			size_t BytesConsumed = m_EnvelopeParser.Parse(m_IncomingData.data(), m_IncomingData.size());
			if (BytesConsumed == AString::npos)
			{
				m_IsValid = false;
				return;
			}
			if ((BytesConsumed == a_Size) && m_EnvelopeParser.IsInHeaders())
			{
				// All the incoming data has been consumed and still waiting for more
				return;
			}
			m_IncomingData.erase(0, BytesConsumed);
		}

		// Search for boundary / boundary end:
		size_t idxBoundary = m_IncomingData.find("\r\n--");
		if (idxBoundary == AString::npos)
		{
			// Boundary string start not present, present as much data to the part callback as possible
			if (m_IncomingData.size() > m_Boundary.size() + 8)
			{
				size_t BytesToReport = m_IncomingData.size() - m_Boundary.size() - 8;
				m_Callbacks.OnPartData(m_IncomingData.data(), BytesToReport);
				m_IncomingData.erase(0, BytesToReport);
			}
			return;
		}
		if (idxBoundary > 0)
		{
			m_Callbacks.OnPartData(m_IncomingData.data(), idxBoundary);
			m_IncomingData.erase(0, idxBoundary);
		}
		idxBoundary = 4;
		size_t LineEnd = m_IncomingData.find("\r\n", idxBoundary);
		if (LineEnd == AString::npos)
		{
			// Not a complete line yet, present as much data to the part callback as possible
			if (m_IncomingData.size() > m_Boundary.size() + 8)
			{
				size_t BytesToReport = m_IncomingData.size() - m_Boundary.size() - 8;
				m_Callbacks.OnPartData(m_IncomingData.data(), BytesToReport);
				m_IncomingData.erase(0, BytesToReport);
			}
			return;
		}
		if (
			(LineEnd - idxBoundary != m_Boundary.size()) &&  // Line length not equal to boundary
			(LineEnd - idxBoundary != m_Boundary.size() + 2)  // Line length not equal to boundary end
		)
		{
			// Got a line, but it's not a boundary, report it as data:
			m_Callbacks.OnPartData(m_IncomingData.data(), LineEnd);
			m_IncomingData.erase(0, LineEnd);
			continue;
		}

		if (strncmp(m_IncomingData.c_str() + idxBoundary, m_Boundary.c_str(), m_Boundary.size()) == 0)
		{
			// Boundary or BoundaryEnd found:
			m_Callbacks.OnPartEnd();
			size_t idxSlash = idxBoundary + m_Boundary.size();
			if ((m_IncomingData[idxSlash] == '-') && (m_IncomingData[idxSlash + 1] == '-'))
			{
				// This was the last part
				m_Callbacks.OnPartData(m_IncomingData.data() + idxSlash + 4, m_IncomingData.size() - idxSlash - 4);
				m_IncomingData.clear();
				return;
			}
			m_Callbacks.OnPartStart();
			m_IncomingData.erase(0, LineEnd + 2);

			// Keep parsing for the headers that may have come with this data:
			m_EnvelopeParser.Reset();
			continue;
		}

		// It's a line, but not a boundary. It can be fully sent to the data receiver, since a boundary cannot cross lines
		m_Callbacks.OnPartData(m_IncomingData.c_str(), LineEnd);
		m_IncomingData.erase(0, LineEnd);
	}  // while (true)
}





void cMultipartParser::OnHeaderLine(const AString & a_Key, const AString & a_Value)
{
	m_Callbacks.OnPartHeader(a_Key, a_Value);
}




