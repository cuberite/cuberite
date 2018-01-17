
// HTTPMessageParser.h

// Declares the cHTTPMessageParser class that parses HTTP messages (request or response) being pushed into the parser,
// and reports the individual parts via callbacks





#pragma once

#include "EnvelopeParser.h"
#include "TransferEncodingParser.h"





class cHTTPMessageParser:
	protected cEnvelopeParser::cCallbacks,
	protected cTransferEncodingParser::cCallbacks
{
public:
	class cCallbacks
	{
	public:
		// Force a virtual destructor in descendants:
		virtual ~cCallbacks() {}

		/** Called when an error has occured while parsing. */
		virtual void OnError(const AString & a_ErrorDescription) = 0;

		/** Called when the first line of the request or response is fully parsed.
		Doesn't check the validity of the line, only extracts the first complete line. */
		virtual void OnFirstLine(const AString & a_FirstLine) = 0;

		/** Called when a single header line is parsed. */
		virtual void OnHeaderLine(const AString & a_Key, const AString & a_Value) = 0;

		/** Called when all the headers have been parsed. */
		virtual void OnHeadersFinished(void) = 0;

		/** Called for each chunk of the incoming body data. */
		virtual void OnBodyData(const void * a_Data, size_t a_Size) = 0;

		/** Called when the entire body has been reported by OnBodyData(). */
		virtual void OnBodyFinished(void) = 0;
	};

	/** Creates a new parser instance that will use the specified callbacks for reporting. */
	cHTTPMessageParser(cCallbacks & a_Callbacks);

	/** Parses the incoming data and calls the appropriate callbacks.
	Returns the number of bytes consumed or AString::npos number for error. */
	size_t Parse(const char * a_Data, size_t a_Size);

	/** Called when the server indicates no more data will be sent (HTTP 1.0 socket closed).
	Finishes all parsing and calls apropriate callbacks (error if incomplete response). */
	void Finish(void);

	/** Returns true if the entire response has been already parsed. */
	bool IsFinished(void) const { return m_IsFinished; }

	/** Resets the parser to the initial state, so that a new request can be parsed. */
	void Reset(void);


protected:

	/** The callbacks used for reporting. */
	cCallbacks & m_Callbacks;

	/** Set to true if an error has been encountered by the parser. */
	bool m_HasHadError;

	/** True if the response has been fully parsed. */
	bool m_IsFinished;

	/** The complete first line of the response. Empty if not parsed yet. */
	AString m_FirstLine;

	/** Buffer for the incoming data until the status line is parsed. */
	AString m_Buffer;

	/** Parser for the envelope data (headers) */
	cEnvelopeParser m_EnvelopeParser;

	/** The specific parser for the transfer encoding used by this response. */
	cTransferEncodingParserPtr m_TransferEncodingParser;

	/** The transfer encoding to be used by the parser.
	Filled while parsing headers, used when headers are finished. */
	AString m_TransferEncoding;

	/** The content length, parsed from the headers, if available.
	Unused for chunked encoding.
	Filled while parsing headers, used when headers are finished. */
	size_t m_ContentLength;


	/** Parses the first line out of m_Buffer.
	Removes the first line from m_Buffer, if appropriate.
	Returns the number of bytes consumed out of m_Buffer, or AString::npos number for error. */
	size_t ParseFirstLine(void);

	/** Parses the message body.
	Processes transfer encoding and calls the callbacks for body data.
	Returns the number of bytes consumed or AString::npos number for error. */
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




