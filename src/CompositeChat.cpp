
// CompositeChat.cpp

// Implements the cCompositeChat class used to wrap a chat message with multiple parts (text, url, cmd)

#include "Globals.h"
#include "CompositeChat.h"
#include "ClientHandle.h"
#include "JsonUtils.h"





////////////////////////////////////////////////////////////////////////////////
// cCompositeChat:

cCompositeChat cCompositeChat::FromText(const AString & a_Text)
{
	cCompositeChat component;
	component.AddTextPart(a_Text);
	return component;
}





cCompositeChat::cCompositeChat(void) :
	m_MessageType(mtCustom)
{
}





cCompositeChat::cCompositeChat(const AString & a_ParseText, eMessageType a_MessageType) :
	m_MessageType(a_MessageType)
{
	ParseText(a_ParseText);
}





void cCompositeChat::Clear(void)
{
	m_Parts.clear();
}





void cCompositeChat::AddTextPart(const AString & a_Message, const AString & a_Style)
{
	m_Parts.push_back(TextPart{{ a_Message, a_Style, {} } });
}





void cCompositeChat::AddClientTranslatedPart(const AString & a_TranslationID, const AStringVector & a_Parameters, const AString & a_Style)
{
	m_Parts.push_back(ClientTranslatedPart{{ a_TranslationID, a_Style, {} }, a_Parameters });
}





void cCompositeChat::AddUrlPart(const AString & a_Text, const AString & a_Url, const AString & a_Style)
{
	m_Parts.push_back(UrlPart{{ a_Text, a_Style, {} }, a_Url });
}





void cCompositeChat::AddRunCommandPart(const AString & a_Text, const AString & a_Command, const AString & a_Style)
{
	m_Parts.push_back(RunCommandPart{{{ a_Text, a_Style, {} }, a_Command } });
}





void cCompositeChat::AddSuggestCommandPart(const AString & a_Text, const AString & a_SuggestedCommand, const AString & a_Style)
{
	m_Parts.push_back(SuggestCommandPart{{{ a_Text, a_Style, {} }, a_SuggestedCommand } });
}





void cCompositeChat::AddShowAchievementPart(const AString & a_PlayerName, const AString & a_Achievement, const AString & a_Style)
{
	m_Parts.push_back(ShowAchievementPart{{ a_Achievement, a_Style, {} }, a_PlayerName });
}




/**
* Parse the input message to add colors or link then add it to the object.
*
* It detects every & of the message and the next character for it to colorize.
* It detect : in the text to detect link structures.
*
* @param a_ParseText The input text to parse
*/
void cCompositeChat::ParseText(const AString & a_ParseText)
{
	size_t len = a_ParseText.length();
	size_t cursor = 0;
	AString CurrentStyle;
	AString CurrentText;

	for (size_t i = 0; i < len; i++)
	{
		switch (a_ParseText[i])
		{
			case '&':  //< Color code
			{
				if ((i != 0) && (a_ParseText[i-1] == '\\'))
				{
					CurrentText.append(a_ParseText, cursor, i - cursor - 1).append("&");
					AddTextPart(CurrentText, CurrentStyle);
					CurrentText.clear();
					cursor = ++i;
					continue;
				}

				if (cursor < i)
				{
					CurrentText.append(a_ParseText, cursor, i - cursor);
					AddTextPart(CurrentText, CurrentStyle);
					CurrentText.clear();
				}
				i++;
				cursor = i + 1;

				if (a_ParseText[i] == 'r')
				{
					CurrentStyle = "";
				}
				else
				{
					CurrentStyle.push_back(a_ParseText[i]);
				}
				break;
			}

			case ':':
			{
				static const constexpr std::array<std::string_view, 2> LinkPrefixes =
				{
					{
						"http",
						"https"
					}
				};
				for (const auto & Prefix : LinkPrefixes)
				{
					size_t PrefixLen = Prefix.size();
					if (
						(i >= cursor + PrefixLen) &&  // There is enough space in front of the colon for the prefix
						(std::string_view(a_ParseText).substr(i - PrefixLen, PrefixLen) == Prefix)  // the prefix matches
					)
					{
						// Add everything before this as a text part:
						if (i > cursor+ PrefixLen)
						{
							CurrentText.append(a_ParseText.c_str() + cursor, i - cursor - PrefixLen);
							cursor= i - PrefixLen;
						}
						if (!CurrentText.empty())
						{
							AddTextPart(CurrentText, CurrentStyle);
							CurrentText.clear();
						}

						// Go till the last non-whitespace char in the text:
						for (; i < len; i++)
						{
							if (isspace(a_ParseText[i]))
							{
								break;
							}
						}
						AddUrlPart(a_ParseText.substr(cursor, i - cursor), a_ParseText.substr(cursor, i - cursor), CurrentStyle);
						cursor = i;
						break;
					}
				}  // for Prefix - LinkPrefix[]
				break;
			}  // case ':'
		}  // switch (a_ParseText[i])
	}  // for i - a_ParseText[]
	if (cursor < len)
	{
		CurrentText.clear();
		CurrentText.append(a_ParseText, cursor, len - cursor);
		AddTextPart(CurrentText, CurrentStyle);
	}
}





void cCompositeChat::SetMessageType(eMessageType a_MessageType, const AString & a_AdditionalMessageTypeData)
{
	m_MessageType = a_MessageType;
	m_AdditionalMessageTypeData = a_AdditionalMessageTypeData;
}





void cCompositeChat::UnderlineUrls(void)
{
	for (auto & Part : m_Parts)
	{
		std::visit(OverloadedVariantAccess
		{
			[](TextPart &             a_Part) {  },
			[](ClientTranslatedPart & a_Part) {  },
			[](UrlPart &              a_Part) { a_Part.Style += 'n'; },
			[](RunCommandPart &       a_Part) {  },
			[](SuggestCommandPart &   a_Part) {  },
			[](ShowAchievementPart &  a_Part) {  },
		}, Part);
	}
}





AString cCompositeChat::ExtractText(void) const
{
	AString Msg;
	for (const auto & Part : m_Parts)
	{
		std::visit(OverloadedVariantAccess
		{
			[&Msg](const TextPart &             a_Part) { Msg.append(a_Part.Text); },
			[&Msg](const ClientTranslatedPart & a_Part) { Msg.append(a_Part.Text); },
			[&Msg](const UrlPart &              a_Part) { Msg.append(a_Part.Url); },
			[&Msg](const RunCommandPart &       a_Part) { Msg.append(a_Part.Text); },
			[&Msg](const SuggestCommandPart &   a_Part) { Msg.append(a_Part.Text); },
			[    ](const ShowAchievementPart &  a_Part) {  },
		}, Part);
	}
	return Msg;
}





eLogLevel cCompositeChat::MessageTypeToLogLevel(eMessageType a_MessageType)
{
	switch (a_MessageType)
	{
		case mtCustom:         return eLogLevel::Regular;
		case mtFailure:        return eLogLevel::Warning;
		case mtInformation:    return eLogLevel::Info;
		case mtSuccess:        return eLogLevel::Regular;
		case mtWarning:        return eLogLevel::Warning;
		case mtFatal:          return eLogLevel::Error;
		case mtDeath:          return eLogLevel::Regular;
		case mtPrivateMessage: return eLogLevel::Regular;
		case mtJoin:           return eLogLevel::Regular;
		case mtLeave:          return eLogLevel::Regular;
		case mtMaxPlusOne: break;
	}
	ASSERT(!"Unhandled MessageType");
	return eLogLevel::Error;
}





AString cCompositeChat::CreateJsonString(bool a_ShouldUseChatPrefixes) const
{
	Json::Value Message;
	Message["text"] = cClientHandle::FormatMessageType(a_ShouldUseChatPrefixes, GetMessageType(), GetAdditionalMessageTypeData());  // The client crashes without this field being present
	for (const auto & Part : m_Parts)
	{
		Json::Value JsonPart;
		std::visit(OverloadedVariantAccess
		{
			[this, &JsonPart](const TextPart & a_Part)
			{
				JsonPart["text"] = a_Part.Text;
				AddChatPartStyle(JsonPart, a_Part.Style);
			},
			[this, &JsonPart](const ClientTranslatedPart & a_Part)
			{
				JsonPart["translate"] = a_Part.Text;
				Json::Value With;
				for (const auto & Parameter : a_Part.Parameters)
				{
					With.append(Parameter);
				}
				if (!a_Part.Parameters.empty())
				{
					JsonPart["with"] = With;
				}
				AddChatPartStyle(JsonPart, a_Part.Style);
			},
			[this, &JsonPart](const UrlPart & a_Part)
			{
				JsonPart["text"] = a_Part.Text;
				Json::Value Url;
				Url["action"] = "open_url";
				Url["value"] = a_Part.Url;
				JsonPart["clickEvent"] = Url;
				AddChatPartStyle(JsonPart, a_Part.Style);
			},
			[this, &JsonPart](const RunCommandPart & a_Part)
			{
				JsonPart["text"] = a_Part.Text;
				Json::Value Cmd;
				Cmd["action"] = "run_command";
				Cmd["value"] = a_Part.Command;
				JsonPart["clickEvent"] = Cmd;
				AddChatPartStyle(JsonPart, a_Part.Style);
			},
			[this, &JsonPart](const SuggestCommandPart & a_Part)
			{
				JsonPart["text"] = a_Part.Text;
				Json::Value Cmd;
				Cmd["action"] = "suggest_command";
				Cmd["value"] = a_Part.Command;
				JsonPart["clickEvent"] = Cmd;
				AddChatPartStyle(JsonPart, a_Part.Style);
			},
			[this, &JsonPart](const ShowAchievementPart & a_Part)
			{
				JsonPart["translate"] = "chat.type.achievement";

				Json::Value Ach;
				Ach["action"] = "show_achievement";
				Ach["value"] = a_Part.Text;

				Json::Value AchColourAndName;
				AchColourAndName["color"] = "green";
				AchColourAndName["translate"] = a_Part.Text;
				AchColourAndName["hoverEvent"] = Ach;

				Json::Value Extra;
				Extra.append(AchColourAndName);

				Json::Value Name;
				Name["text"] = a_Part.PlayerName;

				Json::Value With;
				With.append(Name);
				With.append(Extra);

				JsonPart["with"] = With;
				AddChatPartStyle(JsonPart, a_Part.Style);
			},
		}, Part);
		Message["extra"].append(JsonPart);
	}  // for itr - Parts[]

	#if 1
		// Serialize as machine-readable string (no whitespace):
		return JsonUtils::WriteFastString(Message);
	#else
		// Serialize as human-readable string (pretty-printed):
		return JsonUtils::WriteStyledString(Message);
	#endif
}





void cCompositeChat::AddChatPartStyle(Json::Value & a_Value, const AString & a_PartStyle) const
{
	size_t len = a_PartStyle.length();
	for (size_t i = 0; i < len; i++)
	{
		switch (a_PartStyle[i])
		{
			case 'k': a_Value["obfuscated"] = Json::Value(true); break;
			case 'l': a_Value["bold"] = Json::Value(true); break;
			case 's':  // Deprecated
				LOGERROR("Value s in AddChatPartStyle() is deprecated");
			case 'm': a_Value["strikethrough"] = Json::Value(true); break;
			case 'u':  // Deprecated
				LOGERROR("Value u in AddChatPartStyle() is deprecated");
			case 'n': a_Value["underlined"] = Json::Value(true); break;
			case 'i':  // Deprecated
				LOGERROR("Value i in AddChatPartStyle() is deprecated");
			case 'o': a_Value["italic"] = Json::Value(true); break;
			case '0': a_Value["color"] = Json::Value("black"); break;
			case '1': a_Value["color"] = Json::Value("dark_blue"); break;
			case '2': a_Value["color"] = Json::Value("dark_green"); break;
			case '3': a_Value["color"] = Json::Value("dark_aqua"); break;
			case '4': a_Value["color"] = Json::Value("dark_red"); break;
			case '5': a_Value["color"] = Json::Value("dark_purple"); break;
			case '6': a_Value["color"] = Json::Value("gold"); break;
			case '7': a_Value["color"] = Json::Value("gray"); break;
			case '8': a_Value["color"] = Json::Value("dark_gray"); break;
			case '9': a_Value["color"] = Json::Value("blue"); break;
			case 'a': a_Value["color"] = Json::Value("green"); break;
			case 'b': a_Value["color"] = Json::Value("aqua"); break;
			case 'c': a_Value["color"] = Json::Value("red"); break;
			case 'd': a_Value["color"] = Json::Value("light_purple"); break;
			case 'e': a_Value["color"] = Json::Value("yellow"); break;
			case 'f': a_Value["color"] = Json::Value("white"); break;

		}  // switch (Style[i])
	}  // for i - a_PartStyle[]
}
