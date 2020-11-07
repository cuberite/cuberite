
// CompositeChat.cpp

// Implements the cCompositeChat class used to wrap a chat message with multiple parts (text, url, cmd)

#include "Globals.h"
#include "CompositeChat.h"
#include "ClientHandle.h"
#include "JsonUtils.h"





////////////////////////////////////////////////////////////////////////////////
// cCompositeChat:

cCompositeChat::cCompositeChat(void) :
	m_MessageType(mtCustom)
{
}





cCompositeChat::cCompositeChat(const AString & a_ParseText, eMessageType a_MessageType) :
	m_MessageType(a_MessageType)
{
	ParseText(a_ParseText);
}





cCompositeChat::~cCompositeChat()
{
	Clear();
}





void cCompositeChat::Clear(void)
{
	m_Parts.clear();
}





void cCompositeChat::AddTextPart(const AString & a_Message, const AString & a_Style)
{
	m_Parts.emplace_back(new cTextPart(a_Message, a_Style));
}





void cCompositeChat::AddClientTranslatedPart(const AString & a_TranslationID, const AStringVector & a_Parameters, const AString & a_Style)
{
	m_Parts.emplace_back(new cClientTranslatedPart(a_TranslationID, a_Parameters, a_Style));
}





void cCompositeChat::AddUrlPart(const AString & a_Text, const AString & a_Url, const AString & a_Style)
{
	m_Parts.emplace_back(new cUrlPart(a_Text, a_Url, a_Style));
}





void cCompositeChat::AddRunCommandPart(const AString & a_Text, const AString & a_Command, const AString & a_Style)
{
	m_Parts.emplace_back(new cRunCommandPart(a_Text, a_Command, a_Style));
}





void cCompositeChat::AddSuggestCommandPart(const AString & a_Text, const AString & a_SuggestedCommand, const AString & a_Style)
{
	m_Parts.emplace_back(new cSuggestCommandPart(a_Text, a_SuggestedCommand, a_Style));
}





void cCompositeChat::AddShowAchievementPart(const AString & a_PlayerName, const AString & a_Achievement, const AString & a_Style)
{
	m_Parts.emplace_back(new cShowAchievementPart(a_PlayerName, a_Achievement, a_Style));
}





void cCompositeChat::ParseText(const AString & a_ParseText)
{
	size_t len = a_ParseText.length();
	size_t first = 0;  // First character of the currently parsed block
	AString CurrentStyle;
	AString CurrentText;
	for (size_t i = 0; i < len; i++)
	{
		switch (a_ParseText[i])
		{
			case '@':
			{
				// Color code
				i++;
				if (i >= len)
				{
					// Not enough following text
					break;
				}
				if (a_ParseText[i] == '@')
				{
					// "@@" escape, just put a "@" into the current text and keep parsing as text
					if (i > first + 1)
					{
						CurrentText.append(a_ParseText.c_str() + first, i - first - 1);
					}
					first = i + 1;
					continue;
				}
				else
				{
					// True color code. Create a part for the CurrentText and start parsing anew:
					if (i >= first)
					{
						CurrentText.append(a_ParseText.c_str() + first, i - first - 1);
						first = i + 1;
					}
					if (!CurrentText.empty())
					{
						m_Parts.emplace_back(new cTextPart(CurrentText, CurrentStyle));
						CurrentText.clear();
					}
					AddStyle(CurrentStyle, a_ParseText.substr(i - 1, 2));
				}
				break;
			}

			case ':':
			{
				static const std::array<AString, 2> LinkPrefixes =
				{
					{
						AString("http"),
						AString("https")
					}
				};
				for (const auto & Prefix : LinkPrefixes)
				{
					size_t PrefixLen = Prefix.size();
					if (
						(i >= first + PrefixLen) &&  // There is enough space in front of the colon for the prefix
						(strncmp(a_ParseText.c_str() + i - PrefixLen, Prefix.c_str(), PrefixLen) == 0)  // the prefix matches
					)
					{
						// Add everything before this as a text part:
						if (i > first + PrefixLen)
						{
							CurrentText.append(a_ParseText.c_str() + first, i - first - PrefixLen);
							first = i - PrefixLen;
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
						AddUrlPart(a_ParseText.substr(first, i - first), a_ParseText.substr(first, i - first), CurrentStyle);
						first = i;
						break;
					}
				}  // for Prefix - LinkPrefix[]
				break;
			}  // case ':'
		}  // switch (a_ParseText[i])
	}  // for i - a_ParseText[]
	if (first < len)
	{
		AddTextPart(a_ParseText.substr(first, len - first), CurrentStyle);
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
		if (Part->m_PartType == ptUrl)
		{
			Part->m_Style.append("u");
		}
	}
}





AString cCompositeChat::ExtractText(void) const
{
	AString Msg;
	for (const auto & Part : m_Parts)
	{
		switch (Part->m_PartType)
		{
			case ptText:
			case ptClientTranslated:
			case ptRunCommand:
			case ptSuggestCommand:
			{
				Msg.append(Part->m_Text);
				break;
			}
			case ptUrl:
			{
				Msg.append(static_cast<const cUrlPart *>(Part)->m_Url);
				break;
			}
			case ptShowAchievement:
			{
				break;
			}
		}  // switch (PartType)
	}  // for itr - m_Parts[]
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





void cCompositeChat::AddStyle(AString & a_Style, const AString & a_AddStyle)
{
	if (a_AddStyle.empty())
	{
		return;
	}
	if (a_AddStyle[0] == '@')
	{
		size_t idx = a_Style.find('@');
		if ((idx != AString::npos) && (idx != a_Style.length()))
		{
			a_Style.erase(idx, 2);
		}
		a_Style.append(a_AddStyle);
		return;
	}
	a_Style.append(a_AddStyle);
}





AString cCompositeChat::CreateJsonString(bool a_ShouldUseChatPrefixes) const
{
	Json::Value Message;
	Message["text"] = cClientHandle::FormatMessageType(a_ShouldUseChatPrefixes, GetMessageType(), GetAdditionalMessageTypeData());  // The client crashes without this field being present
	for (const auto & Part : m_Parts)
	{
		Json::Value JsonPart;
		switch (Part->m_PartType)
		{
			case cCompositeChat::ptText:
			{
				JsonPart["text"] = Part->m_Text;
				AddChatPartStyle(JsonPart, Part->m_Style);
				break;
			}

			case cCompositeChat::ptClientTranslated:
			{
				const auto TranslatedPart = static_cast<const cCompositeChat::cClientTranslatedPart *>(Part);
				JsonPart["translate"] = TranslatedPart->m_Text;
				Json::Value With;
				for (const auto & Parameter : TranslatedPart->m_Parameters)
				{
					With.append(Parameter);
				}
				if (!TranslatedPart->m_Parameters.empty())
				{
					JsonPart["with"] = With;
				}
				AddChatPartStyle(JsonPart, TranslatedPart->m_Style);
				break;
			}

			case cCompositeChat::ptUrl:
			{
				const auto UrlPart = static_cast<const cCompositeChat::cUrlPart *>(Part);
				JsonPart["text"] = UrlPart->m_Text;
				Json::Value Url;
				Url["action"] = "open_url";
				LOG(UrlPart->m_Url);
				Url["value"] = UrlPart->m_Url;
				JsonPart["clickEvent"] = Url;
				AddChatPartStyle(JsonPart, UrlPart->m_Style);
				break;
			}

			case cCompositeChat::ptSuggestCommand:
			case cCompositeChat::ptRunCommand:
			{
				const auto CommandPart = static_cast<const cCompositeChat::cCommandPart *>(Part);
				JsonPart["text"] = CommandPart->m_Text;
				Json::Value Cmd;
				Cmd["action"] = (CommandPart->m_PartType == cCompositeChat::ptRunCommand) ? "run_command" : "suggest_command";
				Cmd["value"] = CommandPart->m_Command;
				JsonPart["clickEvent"] = Cmd;
				AddChatPartStyle(JsonPart, CommandPart->m_Style);
				break;
			}

			case cCompositeChat::ptShowAchievement:
			{
				const auto AchievementPart = static_cast<const cCompositeChat::cShowAchievementPart *>(Part);
				JsonPart["translate"] = "chat.type.achievement";

				Json::Value Ach;
				Ach["action"] = "show_achievement";
				Ach["value"] = AchievementPart->m_Text;

				Json::Value AchColourAndName;
				AchColourAndName["color"] = "green";
				AchColourAndName["translate"] = AchievementPart->m_Text;
				AchColourAndName["hoverEvent"] = Ach;

				Json::Value Extra;
				Extra.append(AchColourAndName);

				Json::Value Name;
				Name["text"] = AchievementPart->m_PlayerName;

				Json::Value With;
				With.append(Name);
				With.append(Extra);

				JsonPart["with"] = With;
				AddChatPartStyle(JsonPart, AchievementPart->m_Style);
				break;
			}
		}
		Message["extra"].append(JsonPart);
	}  // for itr - Parts[]

	#if 1
		// Serialize as machine-readable string (no whitespace):
		return JsonUtils::WriteFastString(Message);
	#else
		// Serialize as human-readable string (pretty-printed):
		return JsonUtils::WriteStyledString(msg);
	#endif
}





void cCompositeChat::AddChatPartStyle(Json::Value & a_Value, const AString & a_PartStyle) const
{
	size_t len = a_PartStyle.length();
	for (size_t i = 0; i < len; i++)
	{
		switch (a_PartStyle[i])
		{
			case 'b':
			{
				// bold
				a_Value["bold"] = Json::Value(true);
				break;
			}

			case 'i':
			{
				// italic
				a_Value["italic"] = Json::Value(true);
				break;
			}

			case 'u':
			{
				// Underlined
				a_Value["underlined"] = Json::Value(true);
				break;
			}

			case 's':
			{
				// strikethrough
				a_Value["strikethrough"] = Json::Value(true);
				break;
			}

			case 'o':
			{
				// obfuscated
				a_Value["obfuscated"] = Json::Value(true);
				break;
			}

			case '@':
			{
				// Color, specified by the next char:
				i++;
				if (i >= len)
				{
					// String too short, didn't contain a color
					break;
				}
				switch (a_PartStyle[i])
				{
					case '0': a_Value["color"] = Json::Value("black");        break;
					case '1': a_Value["color"] = Json::Value("dark_blue");    break;
					case '2': a_Value["color"] = Json::Value("dark_green");   break;
					case '3': a_Value["color"] = Json::Value("dark_aqua");    break;
					case '4': a_Value["color"] = Json::Value("dark_red");     break;
					case '5': a_Value["color"] = Json::Value("dark_purple");  break;
					case '6': a_Value["color"] = Json::Value("gold");         break;
					case '7': a_Value["color"] = Json::Value("gray");         break;
					case '8': a_Value["color"] = Json::Value("dark_gray");    break;
					case '9': a_Value["color"] = Json::Value("blue");         break;
					case 'a': a_Value["color"] = Json::Value("green");        break;
					case 'b': a_Value["color"] = Json::Value("aqua");         break;
					case 'c': a_Value["color"] = Json::Value("red");          break;
					case 'd': a_Value["color"] = Json::Value("light_purple"); break;
					case 'e': a_Value["color"] = Json::Value("yellow");       break;
					case 'f': a_Value["color"] = Json::Value("white");        break;
				}  // switch (color)
			}  // case '@'
		}  // switch (Style[i])
	}  // for i - a_PartStyle[]
}





////////////////////////////////////////////////////////////////////////////////
// cCompositeChat::cBasePart:

cCompositeChat::cBasePart::cBasePart(cCompositeChat::ePartType a_PartType, const AString & a_Text, const AString & a_Style) :
	m_PartType(a_PartType),
	m_Text(a_Text),
	m_Style(a_Style)
{
}





////////////////////////////////////////////////////////////////////////////////
// cCompositeChat::cTextPart:

cCompositeChat::cTextPart::cTextPart(const AString & a_Text, const AString &a_Style) :
	Super(ptText, a_Text, a_Style)
{
}





////////////////////////////////////////////////////////////////////////////////
// cCompositeChat::cClientTranslatedPart:

cCompositeChat::cClientTranslatedPart::cClientTranslatedPart(const AString & a_TranslationID, const AStringVector & a_Parameters, const AString & a_Style) :
	Super(ptClientTranslated, a_TranslationID, a_Style),
	m_Parameters(a_Parameters)
{
}





////////////////////////////////////////////////////////////////////////////////
// cCompositeChat::cUrlPart:

cCompositeChat::cUrlPart::cUrlPart(const AString & a_Text, const AString & a_Url, const AString & a_Style) :
	Super(ptUrl, a_Text, a_Style),
	m_Url(a_Url)
{
}





////////////////////////////////////////////////////////////////////////////////
// cCompositeChat::cCommandPart:

cCompositeChat::cCommandPart::cCommandPart(ePartType a_PartType, const AString & a_Text, const AString & a_Command, const AString & a_Style) :
	Super(a_PartType, a_Text, a_Style),
	m_Command(a_Command)
{
}





////////////////////////////////////////////////////////////////////////////////
// cCompositeChat::cRunCommandPart:

cCompositeChat::cRunCommandPart::cRunCommandPart(const AString & a_Text, const AString & a_Command, const AString & a_Style) :
	Super(ptRunCommand, a_Text, a_Command, a_Style)
{
}




////////////////////////////////////////////////////////////////////////////////
// cCompositeChat::cSuggestCommandPart:

cCompositeChat::cSuggestCommandPart::cSuggestCommandPart(const AString & a_Text, const AString & a_Command, const AString & a_Style) :
	Super(ptSuggestCommand, a_Text, a_Command, a_Style)
{
}





////////////////////////////////////////////////////////////////////////////////
// cCompositeChat::cShowAchievementPart:

cCompositeChat::cShowAchievementPart::cShowAchievementPart(const AString & a_PlayerName, const AString & a_Achievement, const AString & a_Style) :
	Super(ptShowAchievement, a_Achievement, a_Style),
	m_PlayerName(a_PlayerName)
{
}




