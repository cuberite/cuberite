
// TransferEncodingParser.h

// Declares the cTransferEncodingParser class representing the parser for the various transfer encodings (chunked etc.)

#pragma once





// fwd:
class cTransferEncodingParser;
typedef std::shared_ptr<cTransferEncodingParser> cTransferEncodingParserPtr;





/** Used as both the interface that all the parsers share and the (static) factory creating such parsers. */
class cTransferEncodingParser
{
public:
	class cCallbacks
	{
	public:
		// Force a virtual destructor in descendants
		virtual ~cCallbacks() {}

		/** Called when an error has occured while parsing. */
		virtual void OnError(const AString & a_ErrorDescription) = 0;

		/** Called for each chunk of the incoming body data. */
		virtual void OnBodyData(const void * a_Data, size_t a_Size) = 0;

		/** Called when the entire body has been reported by OnBodyData(). */
		virtual void OnBodyFinished(void) = 0;
	};


	// Force a virtual destructor in all descendants
	virtual ~cTransferEncodingParser() {}

	/** Parses the incoming data and calls the appropriate callbacks.
	Returns the number of bytes from the end of a_Data that is already not part of this message (if the parser can detect it).
	Returns AString::npos on an error. */
	virtual size_t Parse(const char * a_Data, size_t a_Size) = 0;

	/** To be called when the stream is terminated from the source (connection closed).
	Flushes any buffers and calls appropriate callbacks. */
	virtual void Finish(void) = 0;

	/** Creates a new parser for the specified encoding.
	If the encoding is not known, returns a nullptr.
	a_ContentLength is the length of the content, received in a Content-Length header.
	It is used for the Identity encoding, it is ignored for the Chunked encoding. */
	static cTransferEncodingParserPtr Create(
		cCallbacks & a_Callbacks,
		const AString & a_TransferEncoding,
		size_t a_ContentLength
	);

protected:
	/** The callbacks used to report progress. */
	cCallbacks & m_Callbacks;


	cTransferEncodingParser(cCallbacks & a_Callbacks):
		m_Callbacks(a_Callbacks)
	{
	}
};




