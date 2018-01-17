
// EnvelopeParser.cpp

// Implements the cEnvelopeParser class representing a parser for RFC-822 envelope headers, used both in HTTP and in MIME

#include "Globals.h"
#include "EnvelopeParser.h"





cEnvelopeParser::cEnvelopeParser(cCallbacks & a_Callbacks) :
	m_Callbacks(a_Callbacks),
	m_IsInHeaders(true)
{
}





size_t cEnvelopeParser::Parse(const char * a_Data, size_t a_Size)
{
	if (!m_IsInHeaders)
	{
		return 0;
	}

	// Start searching 1 char from the end of the already received data, if available:
	auto searchStart = m_IncomingData.size();
	searchStart = (searchStart > 1) ? searchStart - 1 : 0;

	m_IncomingData.append(a_Data, a_Size);

	size_t idxCRLF = m_IncomingData.find("\r\n", searchStart);
	if (idxCRLF == AString::npos)
	{
		// Not a complete line yet, all input consumed:
		return a_Size;
	}

	// Parse as many lines as found:
	size_t Last = 0;
	do
	{
		if (idxCRLF == Last)
		{
			// This was the last line of the data. Finish whatever value has been cached and return:
			NotifyLast();
			m_IsInHeaders = false;
			return a_Size - (m_IncomingData.size() - idxCRLF) + 2;
		}
		if (!ParseLine(m_IncomingData.c_str() + Last, idxCRLF - Last))
		{
			// An error has occurred
			m_IsInHeaders = false;
			return AString::npos;
		}
		Last = idxCRLF + 2;
		idxCRLF = m_IncomingData.find("\r\n", idxCRLF + 2);
	} while (idxCRLF != AString::npos);
	m_IncomingData.erase(0, Last);

	// Parsed all lines and still expecting more
	return a_Size;
}





void cEnvelopeParser::Reset(void)
{
	m_IsInHeaders = true;
	m_IncomingData.clear();
	m_LastKey.clear();
	m_LastValue.clear();
}





void cEnvelopeParser::NotifyLast(void)
{
	if (!m_LastKey.empty())
	{
		m_Callbacks.OnHeaderLine(m_LastKey, m_LastValue);
		m_LastKey.clear();
	}
	m_LastValue.clear();
}





bool cEnvelopeParser::ParseLine(const char * a_Data, size_t a_Size)
{
	ASSERT(a_Size > 0);
	if (a_Data[0] <= ' ')
	{
		// This line is a continuation for the previous line
		if (m_LastKey.empty())
		{
			return false;
		}
		// Append, including the whitespace in a_Data[0]
		m_LastValue.append(a_Data, a_Size);
		return true;
	}

	// This is a line with a new key:
	NotifyLast();
	for (size_t i = 0; i < a_Size; i++)
	{
		if (a_Data[i] == ':')
		{
			m_LastKey.assign(a_Data, i);
			if (a_Size > i + 1)
			{
				m_LastValue.assign(a_Data + i + 2, a_Size - i - 2);
			}
			else
			{
				m_LastValue.clear();
			}
			return true;
		}
	}  // for i - a_Data[]

	// No colon was found, key-less header??
	return false;
}




