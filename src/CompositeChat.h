
// CompositeChat.h

// Declares the cCompositeChat class used to wrap a chat message with multiple parts (text, url, cmd)

#include "Defines.h"





// tolua_begin
/** Container for a single chat message composed of multiple functional parts.
Each part corresponds roughly to the behavior supported by the client messaging:
	- plain text, optionaly colorized / styled
	- clickable URLs
	- clickable commands (run)
	- clickable commands (suggest)
Each part has a text assigned to it that can be styled. The style is specified using a string,
each character / character combination in the string specifies the style to use:
	- b = bold
	- i = italic
	- u = underlined
	- s = strikethrough
	- o = obfuscated
	- @X = color X (X is 0 - 9 or a - f, same as dye meta
If the protocol version doesn't support all the features, it degrades gracefully.
*/
class cCompositeChat
{
public:
	// tolua_end
	
	enum ePartType
	{
		ptText,
		ptClientTranslated,
		ptUrl,
		ptRunCommand,
		ptSuggestCommand,
	} ;
	
	class cBasePart
	{
	public:
		ePartType m_PartType;
		AString m_Text;
		AString m_Style;
		
		cBasePart(ePartType a_PartType, const AString & a_Text, const AString & a_Style = "");
		
		// Force a virtual destructor in descendants
		virtual ~cBasePart() {}
	} ;
	
	class cTextPart :
		public cBasePart
	{
		typedef cBasePart super;
	public:
		cTextPart(const AString & a_Text, const AString & a_Style = "");
	} ;
	
	class cClientTranslatedPart :
		public cBasePart
	{
		typedef cBasePart super;
	public:
		AStringVector m_Parameters;
		
		cClientTranslatedPart(const AString & a_TranslationID, const AStringVector & a_Parameters, const AString & a_Style = "");
	} ;
	
	class cUrlPart :
		public cBasePart
	{
		typedef cBasePart super;
	public:
		AString m_Url;
		
		cUrlPart(const AString & a_Text, const AString & a_Url, const AString & a_Style = "");
	} ;
	
	class cCommandPart :
		public cBasePart
	{
		typedef cBasePart super;
	public:
		AString m_Command;
		
		cCommandPart(ePartType a_PartType, const AString & a_Text, const AString & a_Command, const AString & a_Style = "");
	} ;
	
	class cRunCommandPart :
		public cCommandPart
	{
		typedef cCommandPart super;
	public:
		cRunCommandPart(const AString & a_Text, const AString & a_Command, const AString & a_Style = "");
	} ;
	
	class cSuggestCommandPart :
		public cCommandPart
	{
		typedef cCommandPart super;
	public:
		cSuggestCommandPart(const AString & a_Text, const AString & a_Command, const AString & a_Style = "");
	} ;
	
	typedef std::vector<cBasePart *> cParts;
	
	// tolua_begin
	
	/** Creates a new empty chat message */
	cCompositeChat(void);
	
	/** Creates a new chat message and parses the text into parts.
	Recognizes "http:" and "https:" links and @color-codes.
	Uses ParseText() for the actual parsing. */
	cCompositeChat(const AString & a_ParseText);
	
	~cCompositeChat();
	
	/** Removes all parts from the object. */
	void Clear(void);
	
	/** Adds a plain text part, with optional style.
	The default style is plain white text. */
	void AddTextPart(const AString & a_Message, const AString & a_Style = "");
	
	// tolua_end
	
	/** Adds a part that is translated client-side, with the formatting parameters and optional style.
	Exported in ManualBindings due to AStringVector usage - Lua uses an array-table of strings. */
	void AddClientTranslatedPart(const AString & a_TranslationID, const AStringVector & a_Parameters, const AString & a_Style = "");
	
	// tolua_begin
	
	/** Adds a part that opens an URL when clicked.
	The default style is underlined light blue text. */
	void AddUrlPart(const AString & a_Text, const AString & a_Url, const AString & a_Style = "u@c");
	
	/** Adds a part that runs a command when clicked.
	The default style is underlined light green text. */
	void AddRunCommandPart(const AString & a_Text, const AString & a_Command, const AString & a_Style = "u@a");
	
	/** Adds a part that suggests a command (enters it into the chat message area, but doesn't send) when clicked.
	The default style is underlined yellow text. */
	void AddSuggestCommandPart(const AString & a_Text, const AString & a_SuggestedCommand, const AString & a_Style = "u@b");
	
	/** Parses text into various parts, adds those.
	Recognizes "http:" and "https:" URLs and @color-codes. */
	void ParseText(const AString & a_ParseText);
	
	/** Sets the message type, which is indicated by prefixes added to the message when serializing. */
	void SetMessageType(eMessageType a_MessageType);
	
	/** Returns the message type set previously by SetMessageType(). */
	eMessageType GetMessageType(void) const { return m_MessageType; }
	
	/** Adds the "underline" style to each part that is an URL. */
	void UnderlineUrls(void);
	
	// tolua_end
	
	const cParts & GetParts(void) const { return m_Parts; }
	
protected:
	/** All the parts that */
	cParts m_Parts;
	
	/** The message type, as indicated by prefixes. */
	eMessageType m_MessageType;
	
	
	/** Adds a_AddStyle to a_Style; overwrites the existing style if appropriate.
	If the style already contains something that a_AddStyle overrides, it is erased first. */
	void AddStyle(AString & a_Style, const AString & a_AddStyle);
} ;  // tolua_export




