
// HTTPFormParser.cpp

// Implements the cHTTPFormParser class representing a parser for forms sent over HTTP

#include "Globals.h"
#include "HTTPFormParser.h"
#include "HTTPMessage.h"





cHTTPFormParser::cHTTPFormParser(cHTTPRequest & a_Request) :
	m_IsValid(true)
{
	if (a_Request.GetMethod() == "GET")
	{
		m_Kind = fpkURL;
		
		// Directly parse the URL in the request:
		const AString & URL = a_Request.GetURL();
		size_t idxQM = URL.find('?');
		if (idxQM != AString::npos)
		{
			Parse(URL.c_str() + idxQM + 1, URL.size() - idxQM - 1);
		}
		return;
	}
	if ((a_Request.GetMethod() == "POST") || (a_Request.GetMethod() == "PUT"))
	{
		if (a_Request.GetContentType() == "application/x-www-form-urlencoded")
		{
			m_Kind = fpkFormUrlEncoded;
			return;
		}
		if (strncmp(a_Request.GetContentType().c_str(), "multipart/form-data", 19) == 0)
		{
			m_Kind = fpkMultipart;
			return;
		}
	}
	ASSERT(!"Unhandled request method");
}





void cHTTPFormParser::Parse(const char * a_Data, int a_Size)
{
	m_IncomingData.append(a_Data, a_Size);
	switch (m_Kind)
	{
		case fpkURL:
		case fpkFormUrlEncoded:
		{
			// This format is used for smaller forms (not file uploads), so we can delay parsing it until Finish()
			break;
		}
		case fpkMultipart:
		{
			ParseMultipart();
			break;
		}
		default:
		{
			ASSERT(!"Unhandled form kind");
			break;
		}
	}
}





bool cHTTPFormParser::Finish(void)
{
	switch (m_Kind)
	{
		case fpkURL:
		case fpkFormUrlEncoded:
		{
			// m_IncomingData has all the form data, parse it now:
			ParseFormUrlEncoded();
			break;
		}
	}
	return (m_IsValid && m_IncomingData.empty());
}





bool cHTTPFormParser::HasFormData(const cHTTPRequest & a_Request)
{
	return (
		(a_Request.GetContentType() == "application/x-www-form-urlencoded") ||
		(strncmp(a_Request.GetContentType().c_str(), "multipart/form-data", 19) == 0) ||
		(
			(a_Request.GetMethod() == "GET") &&
			(a_Request.GetURL().find('?') != AString::npos)
		)
	);
	return false;
}





void cHTTPFormParser::ParseFormUrlEncoded(void)
{
	// Parse m_IncomingData for all the variables; no more data is incoming, since this is called from Finish()
	// This may not be the most performant version, but we don't care, the form data is small enough and we're not a full-fledged web server anyway
	AStringVector Lines = StringSplit(m_IncomingData, "&");
	for (AStringVector::iterator itr = Lines.begin(), end = Lines.end(); itr != end; ++itr)
	{
		AStringVector Components = StringSplit(*itr, "=");
		switch (Components.size())
		{
			default:
			{
				// Neither name nor value, or too many "="s, mark this as invalid form:
				m_IsValid = false;
				return;
			}
			case 1:
			{
				// Only name present
				(*this)[URLDecode(ReplaceAllCharOccurrences(Components[0], '+', ' '))] = "";
				break;
			}
			case 2:
			{
				// name=value format:
				(*this)[URLDecode(ReplaceAllCharOccurrences(Components[0], '+', ' '))] = URLDecode(ReplaceAllCharOccurrences(Components[1], '+', ' '));
				break;
			}
		}
	}  // for itr - Lines[]
	m_IncomingData.clear();
	
	/*
	size_t len = m_IncomingData.size();
	if (len == 0)
	{
		// No values in the form, consider this valid, too.
		return;
	}
	size_t len1 = len - 1;

	for (size_t i = 0; i < len; )
	{
		char ch = m_IncomingData[i];
		AString Name;
		AString Value;
		while ((i < len1) && (ch != '=') && (ch != '&'))
		{
			if (ch == '+')
			{
				ch = ' ';
			}
			Name.push_back(ch);
			ch = m_IncomingData[++i];
		}
		if (i == len1)
		{
			Value.push_back(ch);
		}
		
		if (ch == '=')
		{
			ch = m_IncomingData[++i];
			while ((i < len1) && (ch != '&'))
			{
				if (ch == '+')
				{
					ch = ' ';
				}
				Value.push_back(ch);
				ch = m_IncomingData[++i];
			}
			if (i == len1)
			{
				Value.push_back(ch);
			}
		}
		(*this)[URLDecode(Name)] = URLDecode(Value);
		if (ch == '&')
		{
			++i;
		}
	}  // for i - m_IncomingData[]
	*/
}





void cHTTPFormParser::ParseMultipart(void)
{
	// TODO
}




