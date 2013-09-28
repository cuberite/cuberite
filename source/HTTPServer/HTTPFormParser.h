
// HTTPFormParser.h

// Declares the cHTTPFormParser class representing a parser for forms sent over HTTP




#pragma once





// fwd:
class cHTTPRequest;





class cHTTPFormParser :
	public std::map<AString, AString>
{
public:
	cHTTPFormParser(cHTTPRequest & a_Request);
	
	/// Adds more data into the parser, as the request body is received
	void Parse(const char * a_Data, int a_Size);
	
	/** Notifies that there's no more data incoming and the parser should finish its parsing.
	Returns true if parsing successful
	*/
	bool Finish(void);
	
	/// Returns true if the headers suggest the request has form data parseable by this class
	static bool HasFormData(const cHTTPRequest & a_Request);
	
protected:
	enum eKind
	{
		fpkURL,             ///< The form has been transmitted as parameters to a GET request
		fpkFormUrlEncoded,  ///< The form has been POSTed or PUT, with Content-Type of "application/x-www-form-urlencoded"
		fpkMultipart,       ///< The form has been POSTed or PUT, with Content-Type of "multipart/*". Currently unsupported
	};

	/// The kind of the parser (decided in the constructor, used in Parse()
	eKind m_Kind;
	
	AString m_IncomingData;
	
	bool m_IsValid;
		
	
	/// Parses m_IncomingData as form-urlencoded data (fpkURL or fpkFormUrlEncoded kinds)
	void ParseFormUrlEncoded(void);
	
	/// Parses m_IncomingData as multipart data (fpkMultipart kind)
	void ParseMultipart(void);
} ;




