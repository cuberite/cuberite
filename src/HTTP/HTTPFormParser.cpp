
// HTTPFormParser.cpp

// Implements the cHTTPFormParser class representing a parser for forms sent over HTTP

#include "Globals.h"
#include "HTTPFormParser.h"
#include "HTTPMessage.h"
#include "NameValueParser.h"





cHTTPFormParser::cHTTPFormParser(const cHTTPIncomingRequest & a_Request, cCallbacks & a_Callbacks) :
	m_Callbacks(a_Callbacks),
	m_IsValid(true),
	m_IsCurrentPartFile(false),
	m_FileHasBeenAnnounced(false)
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
		if (strncmp(a_Request.GetContentType().c_str(), "application/x-www-form-urlencoded", 33) == 0)
		{
			m_Kind = fpkFormUrlEncoded;
			return;
		}
		if (strncmp(a_Request.GetContentType().c_str(), "multipart/form-data", 19) == 0)
		{
			m_Kind = fpkMultipart;
			BeginMultipart(a_Request);
			return;
		}
	}
	// Invalid method / content type combination, this is not a HTTP form
	m_IsValid = false;
}





cHTTPFormParser::cHTTPFormParser(eKind a_Kind, const char * a_Data, size_t a_Size, cCallbacks & a_Callbacks) :
	m_Callbacks(a_Callbacks),
	m_Kind(a_Kind),
	m_IsValid(true),
	m_IsCurrentPartFile(false),
	m_FileHasBeenAnnounced(false)
{
	Parse(a_Data, a_Size);
}





void cHTTPFormParser::Parse(const char * a_Data, size_t a_Size)
{
	if (!m_IsValid)
	{
		return;
	}

	switch (m_Kind)
	{
		case fpkURL:
		case fpkFormUrlEncoded:
		{
			// This format is used for smaller forms (not file uploads), so we can delay parsing it until Finish()
			m_IncomingData.append(a_Data, a_Size);
			break;
		}
		case fpkMultipart:
		{
			ASSERT(m_MultipartParser.get() != nullptr);
			m_MultipartParser->Parse(a_Data, a_Size);
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
		case fpkMultipart:
		{
			// Nothing needed for other formats
			break;
		}
	}
	return (m_IsValid && m_IncomingData.empty());
}





bool cHTTPFormParser::HasFormData(const cHTTPIncomingRequest & a_Request)
{
	const AString & ContentType = a_Request.GetContentType();
	return (
		(ContentType == "application/x-www-form-urlencoded") ||
		(strncmp(ContentType.c_str(), "multipart/form-data", 19) == 0) ||
		(
			(a_Request.GetMethod() == "GET") &&
			(a_Request.GetURL().find('?') != AString::npos)
		)
	);
}





void cHTTPFormParser::BeginMultipart(const cHTTPIncomingRequest & a_Request)
{
	ASSERT(m_MultipartParser.get() == nullptr);
	m_MultipartParser.reset(new cMultipartParser(a_Request.GetContentType(), *this));
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
				auto name = URLDecode(ReplaceAllCharOccurrences(Components[0], '+', ' '));
				if (name.first)
				{
					(*this)[name.second] = "";
				}
				break;
			}
			case 2:
			{
				// name=value format:
				auto name = URLDecode(Components[0]);
				auto value = URLDecode(Components[1]);
				if (name.first && value.first)
				{
					(*this)[name.second] = value.second;
				}
				break;
			}
		}
	}  // for itr - Lines[]
	m_IncomingData.clear();
}





void cHTTPFormParser::OnPartStart(void)
{
	m_CurrentPartFileName.clear();
	m_CurrentPartName.clear();
	m_IsCurrentPartFile = false;
	m_FileHasBeenAnnounced = false;
}





void cHTTPFormParser::OnPartHeader(const AString & a_Key, const AString & a_Value)
{
	if (NoCaseCompare(a_Key, "Content-Disposition") == 0)
	{
		size_t len = a_Value.size();
		size_t ParamsStart = AString::npos;
		for (size_t i = 0; i < len; ++i)
		{
			if (a_Value[i] > ' ')
			{
				if (strncmp(a_Value.c_str() + i, "form-data", 9) != 0)
				{
					// Content disposition is not "form-data", mark the whole form invalid
					m_IsValid = false;
					return;
				}
				ParamsStart = a_Value.find(';', i + 9);
				break;
			}
		}
		if (ParamsStart == AString::npos)
		{
			// There is data missing in the Content-Disposition field, mark the whole form invalid:
			m_IsValid = false;
			return;
		}

		// Parse the field name and optional filename from this header:
		cNameValueParser Parser(a_Value.data() + ParamsStart, a_Value.size() - ParamsStart);
		Parser.Finish();
		m_CurrentPartName = Parser["name"];
		if (!Parser.IsValid() || m_CurrentPartName.empty())
		{
			// The required parameter "name" is missing, mark the whole form invalid:
			m_IsValid = false;
			return;
		}
		m_CurrentPartFileName = Parser["filename"];
	}
}





void cHTTPFormParser::OnPartData(const char * a_Data, size_t a_Size)
{
	if (m_CurrentPartName.empty())
	{
		// Prologue, epilogue or invalid part
		return;
	}
	if (m_CurrentPartFileName.empty())
	{
		// This is a variable, store it in the map
		iterator itr = find(m_CurrentPartName);
		if (itr == end())
		{
			(*this)[m_CurrentPartName] = AString(a_Data, a_Size);
		}
		else
		{
			itr->second.append(a_Data, a_Size);
		}
	}
	else
	{
		// This is a file, pass it on through the callbacks
		if (!m_FileHasBeenAnnounced)
		{
			m_Callbacks.OnFileStart(*this, m_CurrentPartFileName);
			m_FileHasBeenAnnounced = true;
		}
		m_Callbacks.OnFileData(*this, a_Data, a_Size);
	}
}





void cHTTPFormParser::OnPartEnd(void)
{
	if (m_FileHasBeenAnnounced)
	{
		m_Callbacks.OnFileEnd(*this);
	}
	m_CurrentPartName.clear();
	m_CurrentPartFileName.clear();
}




