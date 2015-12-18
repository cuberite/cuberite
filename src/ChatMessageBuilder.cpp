
// ChatMessageBuilder.cpp

// Implements the ChatMessage class used to wrap a chat message with multiple parts (text, url, run / suggest cmd, hover text)

#include "Globals.h"
#include "ChatMessageBuilder.h"
#include "ClientHandle.h"
// #include "SelfTests.h"


void cChatMessageBuilder::AppendPart(const AString & a_Text)
{
	m_Parts.push_back(std::move(cChatMessagePartPtr(cpp14::make_unique<cChatMessagePart>(a_Text))));
}





void cChatMessageBuilder::SetClickEvent(const AString & a_Action, const AString & a_Text)
{
	auto & Part = m_Parts.back();
	Part->m_ClickAction = a_Action;
	Part->m_ClickText = a_Text;
}





void cChatMessageBuilder::SetHoverEvent(const AString & a_Action, const AString & a_Text)
{
	auto & Part = m_Parts.back();
	Part->m_HoverAction = a_Action;
	Part->m_HoverText = a_Text;
}





void cChatMessageBuilder::SetInsertionText(const AString & a_Text)
{
	auto & Part = m_Parts.back();
	Part->m_InsertionText = a_Text;
}





AString cChatMessageBuilder::CreateJsonString(bool a_ShouldUseChatPrefixes) const
{
	Json::Value msg;
	msg["text"] = cClientHandle::FormatMessageType(a_ShouldUseChatPrefixes, GetMessageType(), GetAdditionalMessageTypeData());  // The client crashes without this field being present
	for (auto & ChatMessagePart : m_Parts)
	{
		Json::Value Part;
		Part["text"] = ChatMessagePart->m_Text;

		if (!ChatMessagePart->m_ClickAction.empty())
		{
			Json::Value Click;
			Click["action"] = ChatMessagePart->m_ClickAction;
			Click["value"] = ChatMessagePart->m_ClickText;
			Part["clickEvent"] = Click;
		}
		if (!ChatMessagePart->m_HoverAction.empty())
		{
			Json::Value Hover;
			Hover["action"] = ChatMessagePart->m_HoverAction;
			Hover["value"] = ChatMessagePart->m_HoverText;
			Part["hoverEvent"] = Hover;
		}
		if (!ChatMessagePart->m_InsertionText.empty())
		{
			Part["insertion"] = ChatMessagePart->m_InsertionText;
		}
		msg["extra"].append(std::move(Part));
	}

	return msg.toStyledString();
}





void cChatMessageBuilder::SetMessageType(eMessageType a_MessageType, const AString & a_AdditionalMessageTypeData)
{
	m_MessageType = a_MessageType;
	m_AdditionalMessageTypeData = a_AdditionalMessageTypeData;
}





cChatMessageBuilder::~cChatMessageBuilder()
{
	Clear();
}





void cChatMessageBuilder::Clear(void)
{
	m_Parts.clear();

	// Add a empty part
	m_Parts.push_back(std::move(cChatMessagePartPtr(cpp14::make_unique<cChatMessagePart>(""))));
}
