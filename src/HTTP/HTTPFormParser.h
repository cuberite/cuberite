
// HTTPFormParser.h

// Declares the cHTTPFormParser class representing a parser for forms sent over HTTP




#pragma once

#include "MultipartParser.h"





// fwd:
class cHTTPIncomingRequest;





class cHTTPFormParser :
	public std::map<AString, AString>,
	public cMultipartParser::cCallbacks
{
public:
	enum eKind
	{
		fpkURL,             ///< The form has been transmitted as parameters to a GET request
		fpkFormUrlEncoded,  ///< The form has been POSTed or PUT, with Content-Type of "application/x-www-form-urlencoded"
		fpkMultipart,       ///< The form has been POSTed or PUT, with Content-Type of "multipart/form-data"
	} ;

	class cCallbacks
	{
	public:
		// Force a virtual destructor in descendants:
		virtual ~cCallbacks() {}

		/** Called when a new file part is encountered in the form data */
		virtual void OnFileStart(cHTTPFormParser & a_Parser, const AString & a_FileName) = 0;

		/** Called when more file data has come for the current file in the form data */
		virtual void OnFileData(cHTTPFormParser & a_Parser, const char * a_Data, size_t a_Size) = 0;

		/** Called when the current file part has ended in the form data */
		virtual void OnFileEnd(cHTTPFormParser & a_Parser) = 0;
	} ;


	/** Creates a parser that is tied to a request and notifies of various events using a callback mechanism */
	cHTTPFormParser(const cHTTPIncomingRequest & a_Request, cCallbacks & a_Callbacks);

	/** Creates a parser with the specified content type that reads data from a string */
	cHTTPFormParser(eKind a_Kind, const char * a_Data, size_t a_Size, cCallbacks & a_Callbacks);

	/** Adds more data into the parser, as the request body is received */
	void Parse(const char * a_Data, size_t a_Size);

	/** Notifies that there's no more data incoming and the parser should finish its parsing.
	Returns true if parsing successful. */
	bool Finish(void);

	/** Returns true if the headers suggest the request has form data parseable by this class */
	static bool HasFormData(const cHTTPIncomingRequest & a_Request);

protected:

	/** The callbacks to call for incoming file data */
	cCallbacks & m_Callbacks;

	/** The kind of the parser (decided in the constructor, used in Parse() */
	eKind m_Kind;

	/** Buffer for the incoming data until it's parsed */
	AString m_IncomingData;

	/** True if the information received so far is a valid form; set to false on first problem. Further parsing is skipped when false. */
	bool m_IsValid;

	/** The parser for the multipart data, if used */
	std::unique_ptr<cMultipartParser> m_MultipartParser;

	/** Name of the currently parsed part in multipart data */
	AString m_CurrentPartName;

	/** True if the currently parsed part in multipart data is a file */
	bool m_IsCurrentPartFile;

	/** Filename of the current parsed part in multipart data (for file uploads) */
	AString m_CurrentPartFileName;

	/** Set to true after m_Callbacks.OnFileStart() has been called, reset to false on PartEnd */
	bool m_FileHasBeenAnnounced;


	/** Sets up the object for parsing a fpkMultipart request */
	void BeginMultipart(const cHTTPIncomingRequest & a_Request);

	/** Parses m_IncomingData as form-urlencoded data (fpkURL or fpkFormUrlEncoded kinds) */
	void ParseFormUrlEncoded(void);

	// cMultipartParser::cCallbacks overrides:
	virtual void OnPartStart (void) override;
	virtual void OnPartHeader(const AString & a_Key, const AString & a_Value) override;
	virtual void OnPartData  (const char * a_Data, size_t a_Size) override;
	virtual void OnPartEnd   (void) override;
} ;




