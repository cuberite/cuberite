
// MultipartParser.h

// Declares the cMultipartParser class that parses messages in "multipart/*" encoding into the separate parts





#pragma once

#include "EnvelopeParser.h"





class cMultipartParser :
	protected cEnvelopeParser::cCallbacks
{
public:
	class cCallbacks
	{
	public:
		// Force a virtual destructor in descendants:
		virtual ~cCallbacks() {}

		/** Called when a new part starts */
		virtual void OnPartStart(void) = 0;

		/** Called when a complete header line is received for a part */
		virtual void OnPartHeader(const AString & a_Key, const AString & a_Value) = 0;

		/** Called when body for a part is received */
		virtual void OnPartData(const char * a_Data, size_t a_Size) = 0;

		/** Called when the current part ends */
		virtual void OnPartEnd(void) = 0;
	} ;

	/** Creates the parser, expects to find the boundary in a_ContentType */
	cMultipartParser(const AString & a_ContentType, cCallbacks & a_Callbacks);

	/** Parses more incoming data */
	void Parse(const char * a_Data, size_t a_Size);

protected:
	/** The callbacks to call for various parsing events */
	cCallbacks & m_Callbacks;

	/** True if the data parsed so far is valid; if false, further parsing is skipped */
	bool m_IsValid;

	/** Parser for each part's envelope */
	cEnvelopeParser m_EnvelopeParser;

	/** Buffer for the incoming data until it is parsed */
	AString m_IncomingData;

	/** The boundary, excluding both the initial "--" and the terminating CRLF */
	AString m_Boundary;

	/** Set to true if some data for the current part has already been signalized to m_Callbacks. Used for proper CRLF inserting. */
	bool m_HasHadData;


	/** Parse one line of incoming data. The CRLF has already been stripped from a_Data / a_Size */
	void ParseLine(const char * a_Data, size_t a_Size);

	/** Parse one line of incoming data in the headers section of a part. The CRLF has already been stripped from a_Data / a_Size */
	void ParseHeaderLine(const char * a_Data, size_t a_Size);

	// cEnvelopeParser overrides:
	virtual void OnHeaderLine(const AString & a_Key, const AString & a_Value) override;
} ;




