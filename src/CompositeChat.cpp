
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
	for (cParts::iterator itr = m_Parts.begin(), end = m_Parts.end(); itr != end; ++itr)
	{
		delete *itr;
	}  // for itr - m_Parts[]
	m_Parts.clear();
}





void cCompositeChat::AddTextPart(const AString & a_Message, const AString & a_Style)
{
	m_Parts.push_back(new cTextPart(a_Message, a_Style));
}





void cCompositeChat::AddClientTranslatedPart(const AString & a_TranslationID, const AStringVector & a_Parameters, const AString & a_Style)
{
	m_Parts.push_back(new cClientTranslatedPart(a_TranslationID, a_Parameters, a_Style));
}





void cCompositeChat::AddUrlPart(const AString & a_Text, const AString & a_Url, const AString & a_Style)
{
	m_Parts.push_back(new cUrlPart(a_Text, a_Url, a_Style));
}





void cCompositeChat::AddRunCommandPart(const AString & a_Text, const AString & a_Command, const AString & a_Style)
{
	m_Parts.push_back(new cRunCommandPart(a_Text, a_Command, a_Style));
}





void cCompositeChat::AddSuggestCommandPart(const AString & a_Text, const AString & a_SuggestedCommand, const AString & a_Style)
{
	m_Parts.push_back(new cSuggestCommandPart(a_Text, a_SuggestedCommand, a_Style));
}





void cCompositeChat::AddShowAchievementPart(const AString & a_PlayerName, const AString & a_Achievement, const AString & a_Style)
{
	m_Parts.push_back(new cShowAchievementPart(a_PlayerName, a_Achievement, a_Style));
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
						m_Parts.push_back(new cTextPart(CurrentText, CurrentStyle));
						CurrentText.clear();
					}
					AddStyle(CurrentStyle, a_ParseText.substr(i - 1, 2));
				}
				break;
			}

			case ':':
			{
				const char * LinkPrefixes[] =
				{
					"http",
					"https"
				};
				for (size_t Prefix = 0; Prefix < ARRAYCOUNT(LinkPrefixes); Prefix++)
				{
					size_t PrefixLen = strlen(LinkPrefixes[Prefix]);
					if (
						(i >= first + PrefixLen) &&  // There is enough space in front of the colon for the prefix
						(strncmp(a_ParseText.c_str() + i - PrefixLen, LinkPrefixes[Prefix], PrefixLen) == 0)  // the prefix matches
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
	for (cParts::iterator itr = m_Parts.begin(), end = m_Parts.end(); itr != end; ++itr)
	{
		if ((*itr)->m_PartType == ptUrl)
		{
			(*itr)->m_Style.append("u");
		}
	}  // for itr - m_Parts[]
}





AString cCompositeChat::ExtractText(void) const
{
	AString Msg;
	for (cParts::const_iterator itr = m_Parts.begin(), end = m_Parts.end(); itr != end; ++itr)
	{
		switch ((*itr)->m_PartType)
		{
			case ptText:
			case ptClientTranslated:
			case ptRunCommand:
			case ptSuggestCommand:
			{
				Msg.append((*itr)->m_Text);
				break;
			}
			case ptUrl:
			{
				Msg.append((static_cast<cUrlPart *>(*itr))->m_Url);
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
	Json::Value msg;
	msg["text"] = cClientHandle::FormatMessageType(a_ShouldUseChatPrefixes, GetMessageType(), GetAdditionalMessageTypeData());  // The client crashes without this field being present
	const cCompositeChat::cParts & Parts = GetParts();
	for (cCompositeChat::cParts::const_iterator itr = Parts.begin(), end = Parts.end(); itr != end; ++itr)
	{
		Json::Value Part;
		switch ((*itr)->m_PartType)
		{
			case cCompositeChat::ptText:
			{
				Part["text"] = (*itr)->m_Text;
				AddChatPartStyle(Part, (*itr)->m_Style);
				break;
			}

			case cCompositeChat::ptClientTranslated:
			{
				const cCompositeChat::cClientTranslatedPart & p = static_cast<const cCompositeChat::cClientTranslatedPart &>(**itr);
				Part["translate"] = p.m_Text;
				Json::Value With;
				for (AStringVector::const_iterator itrW = p.m_Parameters.begin(), endW = p.m_Parameters.end(); itrW != endW; ++itr)
				{
					With.append(*itrW);
				}
				if (!p.m_Parameters.empty())
				{
					Part["with"] = With;
				}
				AddChatPartStyle(Part, p.m_Style);
				break;
			}

			case cCompositeChat::ptUrl:
			{
				const cCompositeChat::cUrlPart & p = static_cast<const cCompositeChat::cUrlPart &>(**itr);
				Part["text"] = p.m_Text;
				Json::Value Url;
				Url["action"] = "open_url";
				Url["value"] = p.m_Url;
				Part["clickEvent"] = Url;
				AddChatPartStyle(Part, p.m_Style);
				break;
			}

			case cCompositeChat::ptSuggestCommand:
			case cCompositeChat::ptRunCommand:
			{
				const cCompositeChat::cCommandPart & p = static_cast<const cCompositeChat::cCommandPart &>(**itr);
				Part["text"] = p.m_Text;
				Json::Value Cmd;
				Cmd["action"] = (p.m_PartType == cCompositeChat::ptRunCommand) ? "run_command" : "suggest_command";
				Cmd["value"] = p.m_Command;
				Part["clickEvent"] = Cmd;
				AddChatPartStyle(Part, p.m_Style);
				break;
			}

			case cCompositeChat::ptShowAchievement:
			{
				const cCompositeChat::cShowAchievementPart & p = static_cast<const cCompositeChat::cShowAchievementPart &>(**itr);
				Part["translate"] = "chat.type.achievement";

				Json::Value Ach;
				Ach["action"] = "show_achievement";
				Ach["value"] = p.m_Text;

				Json::Value AchColourAndName;
				AchColourAndName["color"] = "green";
				AchColourAndName["translate"] = p.m_Text;
				AchColourAndName["hoverEvent"] = Ach;

				Json::Value Extra;
				Extra.append(AchColourAndName);

				Json::Value Name;
				Name["text"] = p.m_PlayerName;

				Json::Value With;
				With.append(Name);
				With.append(Extra);

				Part["with"] = With;
				AddChatPartStyle(Part, p.m_Style);
				break;
			}
		}
		msg["extra"].append(Part);
	}  // for itr - Parts[]

	#if 1
		// Serialize as machine-readable string (no whitespace):
		return JsonUtils::WriteFastString(msg);
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




