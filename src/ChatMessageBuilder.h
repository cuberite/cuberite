
// ChatMessageBuilder.h

// Declares the cChatMessageBuilder class used to build a chat message

#include "Defines.h"
#include "json/json.h"
#include "Logger.h"
#include "ChatColor.h"

// tolua_begin
class cChatMessageBuilder
{
public:
	// tolua_end

	/** This class is used to store the text, the click event, hover event and insertion text. */
	class cChatMessagePart
	{
	public:
		cChatMessagePart()
		{
		}

		cChatMessagePart(const AString & a_Text):
			m_Text(a_Text)
		{
		}


		~cChatMessagePart() {}

		AString m_Text;
		AString m_InsertionText;
		AString m_HoverText;
		AString m_ClickText;
		AString m_HoverAction;
		AString m_ClickAction;
	} ;

	typedef std::unique_ptr<cChatMessagePart> cChatMessagePartPtr;
	typedef std::vector<cChatMessagePartPtr> cParts;

	// tolua_begin

	/** Adds a cChatMessagePart with (empty) text. */
	cChatMessageBuilder(const AString & a_Text = "") :
		m_MessageType(mtCustom)
	{
		m_Parts.push_back(std::move(cChatMessagePartPtr(cpp14::make_unique<cChatMessagePart>(a_Text))));
	}

	~cChatMessageBuilder();

	void Clear(void);

	AString CreateJsonString(bool a_ShouldUseChatPrefixes = true) const;

	/** Returns the message type set previously by SetMessageType(). */
	eMessageType GetMessageType(void) const { return m_MessageType; }

	/** Returns additional data pertaining to message type, for example, the name of a mtPrivateMsg sender */
	AString GetAdditionalMessageTypeData(void) const { return m_AdditionalMessageTypeData; }

	// tolua_end

	/** Adds a new cChatMessagePart and sets the text. */
	void AppendPart(const AString & a_Text);

	/** Sets a action on the last part in m_Parts, that will be run, if the text has been clicked.
	Actions:
	- open_url
	- run_command
	- suggest_command
	*/
	void SetClickEvent(const AString & a_Action, const AString & a_Text);

	/** Sets a action on the last part in m_Parts that will be run, if the mouse is over the text
	Actions:
	- show_text
	- show_achievement
	- show_item
	*/
	void SetHoverEvent(const AString & a_Action, const AString & a_Text);

	/** Inserts the specified text into the chat on Shift Click (>= 1.8) */
	void SetInsertionText(const AString & a_Text);

	/** Sets the message type, which is indicated by prefixes added to the message when serializing
	Takes optional AdditionalMessageTypeData to set m_AdditionalMessageTypeData. See said variable for more documentation. */
	void SetMessageType(eMessageType a_MessageType, const AString & a_AdditionalMessageTypeData = "");
protected:
	/** This will never be empy, it will always contains one part. */
	cParts m_Parts;

	/** The message type, as indicated by prefixes. */
	eMessageType m_MessageType;

	/** Additional data pertaining to message type, for example, the name of a mtPrivateMsg sender */
	AString m_AdditionalMessageTypeData;
} ;  // tolua_export
