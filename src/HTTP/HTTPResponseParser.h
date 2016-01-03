
// HTTPResponseParser.h

// Declares the cHTTPResponseParser class representing the parser for incoming HTTP responses




#pragma once

#include "HTTPMessage.h"
#include "TransferEncodingParser.h"





class cHTTPResponseParser:
	public cHTTPMessage,
	protected cEnvelopeParser::cCallbacks,
	protected cTransferEncodingParser::cCallbacks
{
	typedef cHTTPMessage Super;

public:
	class cCallbacks
	{
	public:
		// Force a virtual destructor in descendants:
		virtual ~cCallbacks() {}

		/** Called when an error has occured while parsing. */
		virtual void OnError(const AString & a_ErrorDescription) = 0;

		/** Called when the status line is fully parsed. */
		virtual void OnStatusLine(const AString & a_StatusLine) = 0;

		/** Called when a single header line is parsed. */
		virtual void OnHeaderLine(const AString & a_Key, const AString & a_Value) = 0;

		/** Called when all the headers have been parsed. */
		virtual void OnHeadersFinished(void) = 0;

		/** Called for each chunk of the incoming body data. */
		virtual void OnBodyData(const void * a_Data, size_t a_Size) = 0;

		/** Called when the entire body has been reported by OnBodyData(). */
		virtual void OnBodyFinished(void) = 0;
	};

	cHTTPResponseParser(cCallbacks & a_Callbacks);

	/** Parses the incoming data and calls the appropriate callbacks.
	Returns the number of bytes from the end of a_Data that is already not part of this response.
	Returns AString::npos on an error. */
	size_t Parse(const char * a_Data, size_t a_Size);

	/** Called when the server indicates no more data will be sent (HTTP 1.0 socket closed).
	Finishes all parsing and calls apropriate callbacks (error if incomplete response). */
	void Finish(void);

	/** Returns true if the entire response has been already parsed. */
	bool IsFinished(void) const { return m_IsFinished; }


protected:

	/** The callbacks used for reporting. */
	cCallbacks & m_Callbacks;

	/** Set to true if an error has been encountered by the parser. */
	bool m_HasHadError;

	/** True if the parser is still parsing the status or headers. */
	bool m_IsInHeaders;

	/** True if the response has been fully parsed. */
	bool m_IsFinished;

	/** The complete status line of the response. Empty if not parsed yet. */
	AString m_StatusLine;

	/** Buffer for the incoming data until the status line is parsed. */
	AString m_Buffer;

	/** Parser for the envelope data (headers) */
	cEnvelopeParser m_EnvelopeParser;

	/** The specific parser for the transfer encoding used by this response. */
	cTransferEncodingParserPtr m_TransferEncodingParser;


	/** Parses the status line out of the m_Buffer.
	Removes the status line from m_Buffer, if appropriate.
	Returns true if the entire status line has been parsed. */
	bool ParseStatusLine(void);

	/** Parses the message body.
	Processes transfer encoding and calls the callbacks for body data.
	Returns the number of bytes from the end of a_Data that is already not part of this response.
	Returns AString::npos on error. */
	size_t ParseBody(const char * a_Data, size_t a_Size);

	/** Called internally when the headers-parsing has just finished. */
	void HeadersFinished(void);

	// cEnvelopeParser::cCallbacks overrides:
	virtual void OnHeaderLine(const AString & a_Key, const AString & a_Value) override;

	// cTransferEncodingParser::cCallbacks overrides:
	virtual void OnError(const AString & a_ErrorDescription) override;
	virtual void OnBodyData(const void * a_Data, size_t a_Size) override;
	virtual void OnBodyFinished(void) override;
};




