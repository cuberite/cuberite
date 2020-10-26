
// CompositeChat.h

// Declares the cCompositeChat class used to wrap a chat message with multiple parts (text, url, cmd)

#pragma once

#include "Defines.h"
#include "json/json.h"





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
		ptShowAchievement,
	} ;



	class cBasePart
	{
	public:
		ePartType m_PartType;
		AString m_Text;
		AString m_Style;
		AString m_AdditionalStyleData;

		cBasePart(ePartType a_PartType, const AString & a_Text, const AString & a_Style = "");

		// Force a virtual destructor in descendants
		virtual ~cBasePart() {}
	} ;



	class cTextPart:
		public cBasePart
	{
		using Super = cBasePart;

	public:

		cTextPart(const AString & a_Text, const AString & a_Style = "");
	} ;



	class cClientTranslatedPart:
		public cBasePart
	{
		using Super = cBasePart;

	public:

		AStringVector m_Parameters;

		cClientTranslatedPart(const AString & a_TranslationID, const AStringVector & a_Parameters, const AString & a_Style = "");
	} ;



	class cUrlPart:
		public cBasePart
	{
		using Super = cBasePart;

	public:

		AString m_Url;

		cUrlPart(const AString & a_Text, const AString & a_Url, const AString & a_Style = "");
	} ;



	class cCommandPart:
		public cBasePart
	{
		using Super = cBasePart;

	public:

		AString m_Command;

		cCommandPart(ePartType a_PartType, const AString & a_Text, const AString & a_Command, const AString & a_Style = "");
	} ;



	class cRunCommandPart:
		public cCommandPart
	{
		using Super = cCommandPart;

	public:

		cRunCommandPart(const AString & a_Text, const AString & a_Command, const AString & a_Style = "");
	} ;



	class cSuggestCommandPart:
		public cCommandPart
	{
		using Super = cCommandPart;

	public:

		cSuggestCommandPart(const AString & a_Text, const AString & a_Command, const AString & a_Style = "");
	} ;



	class cShowAchievementPart:
		public cBasePart
	{
		using Super = cBasePart;

	public:

		AString m_PlayerName;
		cShowAchievementPart(const AString & a_PlayerName, const AString & a_Achievement, const AString & a_Style = "");
	} ;



	using cParts = std::vector<cBasePart *>;


	/** Creates a new empty chat message.
	Exported manually due to the other overload needing a manual export. */
	cCompositeChat(void);

	/** Creates a new chat message and parses the text into parts.
	Recognizes "http:" and "https:" links and @color-codes.
	Uses ParseText() for the actual parsing.
	Exported manually due to ToLua++ generating extra output parameter. */
	cCompositeChat(const AString & a_ParseText, eMessageType a_MessageType = mtCustom);

	~cCompositeChat();  // tolua_export

	// The following are exported in ManualBindings in order to support chaining - they return "self" in Lua (#755)

	/** Removes all parts from the object. */
	void Clear(void);

	/** Adds a plain text part, with optional style.
	The default style is plain white text. */
	void AddTextPart(const AString & a_Message, const AString & a_Style = "");

	/** Adds a part that is translated client-side, with the formatting parameters and optional style. */
	void AddClientTranslatedPart(const AString & a_TranslationID, const AStringVector & a_Parameters, const AString & a_Style = "");

	/** Adds a part that opens an URL when clicked.
	The default style is underlined light blue text. */
	void AddUrlPart(const AString & a_Text, const AString & a_Url, const AString & a_Style = "u@c");

	/** Adds a part that runs a command when clicked.
	The default style is underlined light green text. */
	void AddRunCommandPart(const AString & a_Text, const AString & a_Command, const AString & a_Style = "u@a");

	/** Adds a part that suggests a command (enters it into the chat message area, but doesn't send) when clicked.
	The default style is underlined yellow text. */
	void AddSuggestCommandPart(const AString & a_Text, const AString & a_SuggestedCommand, const AString & a_Style = "u@b");

	/** Adds a part that fully formats a specified achievement using client translatable strings
	Takes achievement name and player awarded to. Displays as {player} has earned the achievement {achievement_name}.
	*/
	void AddShowAchievementPart(const AString & a_PlayerName, const AString & a_Achievement, const AString & a_Style = "");

	/** Parses text into various parts, adds those.
	Recognizes "http:" and "https:" URLs and @color-codes. */
	void ParseText(const AString & a_ParseText);

	/** Adds the "underline" style to each part that is an URL. */
	void UnderlineUrls(void);

	/** Sets the message type, which is indicated by prefixes added to the message when serializing
	Takes optional AdditionalMessageTypeData to set m_AdditionalMessageTypeData. See said variable for more documentation.
	Exported manually, because ToLua++ would generate extra return values. */
	void SetMessageType(eMessageType a_MessageType, const AString & a_AdditionalMessageTypeData = "");

	// tolua_begin

	/** Returns the message type set previously by SetMessageType(). */
	eMessageType GetMessageType(void) const { return m_MessageType; }

	/** Returns additional data pertaining to message type, for example, the name of a mtPrivateMsg sender */
	AString GetAdditionalMessageTypeData(void) const { return m_AdditionalMessageTypeData; }

	/** Returns the text from the parts that comprises the human-readable data.
	Used for older protocols that don't support composite chat
	and for console-logging. */
	AString ExtractText(void) const;

	AString CreateJsonString(bool a_ShouldUseChatPrefixes = true) const;

	// tolua_end

	const cParts & GetParts(void) const { return m_Parts; }

	/** Converts the MessageType to a LogLevel value.
	Used by the logging bindings when logging a cCompositeChat object. */
	static eLogLevel MessageTypeToLogLevel(eMessageType a_MessageType);

	/** Adds the chat part's style (represented by the part's stylestring) into the Json object. */
	void AddChatPartStyle(Json::Value & a_Value, const AString & a_PartStyle) const;

protected:
	/** All the parts that */
	cParts m_Parts;

	/** The message type, as indicated by prefixes. */
	eMessageType m_MessageType;

	/** Additional data pertaining to message type, for example, the name of a mtPrivateMsg sender */
	AString m_AdditionalMessageTypeData;


	/** Adds a_AddStyle to a_Style; overwrites the existing style if appropriate.
	If the style already contains something that a_AddStyle overrides, it is erased first. */
	void AddStyle(AString & a_Style, const AString & a_AddStyle);
} ;  // tolua_export




