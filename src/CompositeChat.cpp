
// CompositeChat.cpp

// Implements the cCompositeChat class used to wrap a chat message with multiple parts (text, url, cmd)

#include "Globals.h"
#include "CompositeChat.h"





#ifdef SELF_TEST

/** A simple self-test that verifies that the composite chat parser is working properly. */
class SelfTest_CompositeChat
{
public:
	SelfTest_CompositeChat(void)
	{
		fprintf(stderr, "cCompositeChat self test...\n");
		TestParser1();
		TestParser2();
		TestParser3();
		TestParser4();
		TestParser5();
		fprintf(stderr, "cCompositeChat self test finished.\n");
	}
	
	void TestParser1(void)
	{
		cCompositeChat Msg;
		Msg.ParseText("Testing @2color codes and http://links parser");
		const cCompositeChat::cParts & Parts = Msg.GetParts();
		assert_test(Parts.size() == 4);
		assert_test(Parts[0]->m_PartType == cCompositeChat::ptText);
		assert_test(Parts[1]->m_PartType == cCompositeChat::ptText);
		assert_test(Parts[2]->m_PartType == cCompositeChat::ptUrl);
		assert_test(Parts[3]->m_PartType == cCompositeChat::ptText);
		assert_test(Parts[0]->m_Style == "");
		assert_test(Parts[1]->m_Style == "@2");
		assert_test(Parts[2]->m_Style == "@2");
		assert_test(Parts[3]->m_Style == "@2");
	}
	
	void TestParser2(void)
	{
		cCompositeChat Msg;
		Msg.ParseText("@3Advanced stuff: @5overriding color codes and http://links.with/@4color-in-them handling");
		const cCompositeChat::cParts & Parts = Msg.GetParts();
		assert_test(Parts.size() == 4);
		assert_test(Parts[0]->m_PartType == cCompositeChat::ptText);
		assert_test(Parts[1]->m_PartType == cCompositeChat::ptText);
		assert_test(Parts[2]->m_PartType == cCompositeChat::ptUrl);
		assert_test(Parts[3]->m_PartType == cCompositeChat::ptText);
		assert_test(Parts[0]->m_Style == "@3");
		assert_test(Parts[1]->m_Style == "@5");
		assert_test(Parts[2]->m_Style == "@5");
		assert_test(Parts[3]->m_Style == "@5");
	}
	
	void TestParser3(void)
	{
		cCompositeChat Msg;
		Msg.ParseText("http://links.starting the text");
		const cCompositeChat::cParts & Parts = Msg.GetParts();
		assert_test(Parts.size() == 2);
		assert_test(Parts[0]->m_PartType == cCompositeChat::ptUrl);
		assert_test(Parts[1]->m_PartType == cCompositeChat::ptText);
		assert_test(Parts[0]->m_Style == "");
		assert_test(Parts[1]->m_Style == "");
	}
	
	void TestParser4(void)
	{
		cCompositeChat Msg;
		Msg.ParseText("links finishing the text: http://some.server");
		const cCompositeChat::cParts & Parts = Msg.GetParts();
		assert_test(Parts.size() == 2);
		assert_test(Parts[0]->m_PartType == cCompositeChat::ptText);
		assert_test(Parts[1]->m_PartType == cCompositeChat::ptUrl);
		assert_test(Parts[0]->m_Style == "");
		assert_test(Parts[1]->m_Style == "");
	}
	
	void TestParser5(void)
	{
		cCompositeChat Msg;
		Msg.ParseText("http://only.links");
		const cCompositeChat::cParts & Parts = Msg.GetParts();
		assert_test(Parts.size() == 1);
		assert_test(Parts[0]->m_PartType == cCompositeChat::ptUrl);
		assert_test(Parts[0]->m_Style == "");
	}
	
} gTest;
#endif  // SELF_TEST





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
				Msg.append(((cUrlPart *)(*itr))->m_Url);
				break;
			}
		}  // switch (PartType)
	}  // for itr - m_Parts[]
	return Msg;
}





cMCLogger::eLogLevel cCompositeChat::MessageTypeToLogLevel(eMessageType a_MessageType)
{
	switch (a_MessageType)
	{
		case mtCustom:         return cMCLogger::llRegular;
		case mtFailure:        return cMCLogger::llWarning;
		case mtInformation:    return cMCLogger::llInfo;
		case mtSuccess:        return cMCLogger::llRegular;
		case mtWarning:        return cMCLogger::llWarning;
		case mtFatal:          return cMCLogger::llError;
		case mtDeath:          return cMCLogger::llRegular;
		case mtPrivateMessage: return cMCLogger::llRegular;
		case mtJoin:           return cMCLogger::llRegular;
		case mtLeave:          return cMCLogger::llRegular;
	}
	ASSERT(!"Unhandled MessageType");
	return cMCLogger::llError;
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
	super(ptText, a_Text, a_Style)
{
}





////////////////////////////////////////////////////////////////////////////////
// cCompositeChat::cClientTranslatedPart:

cCompositeChat::cClientTranslatedPart::cClientTranslatedPart(const AString & a_TranslationID, const AStringVector & a_Parameters, const AString & a_Style) :
	super(ptClientTranslated, a_TranslationID, a_Style),
	m_Parameters(a_Parameters)
{
}





////////////////////////////////////////////////////////////////////////////////
// cCompositeChat::cUrlPart:

cCompositeChat::cUrlPart::cUrlPart(const AString & a_Text, const AString & a_Url, const AString & a_Style) :
	super(ptUrl, a_Text, a_Style),
	m_Url(a_Url)
{
}





////////////////////////////////////////////////////////////////////////////////
// cCompositeChat::cCommandPart:

cCompositeChat::cCommandPart::cCommandPart(ePartType a_PartType, const AString & a_Text, const AString & a_Command, const AString & a_Style) :
	super(a_PartType, a_Text, a_Style),
	m_Command(a_Command)
{
}





////////////////////////////////////////////////////////////////////////////////
// cCompositeChat::cRunCommandPart:

cCompositeChat::cRunCommandPart::cRunCommandPart(const AString & a_Text, const AString & a_Command, const AString & a_Style) :
	super(ptRunCommand, a_Text, a_Command, a_Style)
{
}




////////////////////////////////////////////////////////////////////////////////
// cCompositeChat::cSuggestCommandPart:

cCompositeChat::cSuggestCommandPart::cSuggestCommandPart(const AString & a_Text, const AString & a_Command, const AString & a_Style) :
	super(ptSuggestCommand, a_Text, a_Command, a_Style)
{
}





////////////////////////////////////////////////////////////////////////////////
// cCompositeChat::cShowAchievementPart:

cCompositeChat::cShowAchievementPart::cShowAchievementPart(const AString & a_PlayerName, const AString & a_Achievement, const AString & a_Style) :
	super(ptShowAchievement, a_Achievement, a_Style),
	m_PlayerName(a_PlayerName)
{
}




