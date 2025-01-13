
// CompositeChat.h

// Declares the cCompositeChat class used to wrap a chat message with multiple parts (text, url, cmd)

#pragma once

#include "Defines.h"
#include "json/json.h"
#include "WorldStorage/FastNBT.h"




// tolua_begin
/** Container for a single chat message composed of multiple functional parts.
Each part corresponds roughly to the behavior supported by the client messaging:
	- plain text, optionaly colorized / styled
	- clickable URLs
	- clickable commands (run)
	- clickable commands (suggest)
Each part has a text assigned to it that can be styled. The style is specified using a string,
each character / character combination in the string specifies the style to use:
	- (char from 0 - 9 or a - f) = color X
	- k = obfuscated
	- l = bold
	- m = strikethrough
	- n = underlined
	- o = italic
	- r = reset
If the protocol version doesn't support all the features, it degrades gracefully.
*/
class cCompositeChat
{
public:
	// tolua_end


	struct BasePart
	{
		AString Text;
		AString Style;
		AString AdditionalStyleData;
	} ;



	struct TextPart:
		public BasePart
	{
	} ;



	struct ClientTranslatedPart:
		public BasePart
	{
		AStringVector Parameters;
	} ;



	struct UrlPart:
		public BasePart
	{
		AString Url;
	} ;



	struct CommandPart:
		public BasePart
	{
		AString Command;
	} ;



	struct RunCommandPart:
		public CommandPart
	{
	} ;



	struct SuggestCommandPart:
		public CommandPart
	{
	} ;



	struct ShowAchievementPart:
		public BasePart
	{
		AString PlayerName;
	} ;



	/** Creates a new empty chat message.
	Exported manually due to the other overload needing a manual export. */
	cCompositeChat(void);

	/** Creates a new chat message and parses the text into parts.
	Recognizes "http:" and "https:" links and &format-character.
	Uses ParseText() for the actual parsing.
	Exported manually due to ToLua++ generating extra output parameter. */
	cCompositeChat(const AString & a_ParseText, eMessageType a_MessageType = mtCustom);

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
	void AddUrlPart(const AString & a_Text, const AString & a_Url, const AString & a_Style = "nc");

	/** Adds a part that runs a command when clicked.
	The default style is underlined light green text. */
	void AddRunCommandPart(const AString & a_Text, const AString & a_Command, const AString & a_Style = "na");

	/** Adds a part that suggests a command (enters it into the chat message area, but doesn't send) when clicked.
	The default style is underlined yellow text. */
	void AddSuggestCommandPart(const AString & a_Text, const AString & a_SuggestedCommand, const AString & a_Style = "nb");

	/** Adds a part that fully formats a specified achievement using client translatable strings
	Takes achievement name and player awarded to. Displays as {player} has earned the achievement {achievement_name}.
	*/
	void AddShowAchievementPart(const AString & a_PlayerName, const AString & a_Achievement, const AString & a_Style = "");

	/** Parses text into various parts, adds those.
	Recognizes "http:" and "https:" URLs and &color-codes. */
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

	const auto & GetParts(void) const { return m_Parts; }

	/** Converts the MessageType to a LogLevel value.
	Used by the logging bindings when logging a cCompositeChat object. */
	static eLogLevel MessageTypeToLogLevel(eMessageType a_MessageType);

	void WriteAsNBT(cFastNBTWriter & a_Writer, bool a_ShouldUseChatPrefixes) const;

	void AddChatPartStyle(cFastNBTWriter & a_Writer, const AString & a_PartStyle) const;

	/** Adds the chat part's style (represented by the part's stylestring) into the Json object. */
	void AddChatPartStyle(Json::Value & a_Value, const AString & a_PartStyle) const;

protected:

	/** All the parts that */
	std::vector<std::variant<TextPart, ClientTranslatedPart, UrlPart, RunCommandPart, SuggestCommandPart, ShowAchievementPart>> m_Parts;

	/** The message type, as indicated by prefixes. */
	eMessageType m_MessageType;

	/** Additional data pertaining to message type, for example, the name of a mtPrivateMsg sender */
	AString m_AdditionalMessageTypeData;

} ;  // tolua_export
