
// CompositeChat.cpp

// Implements the cCompositeChat class used to wrap a chat message with multiple parts (text, url, cmd)

#include "Globals.h"
#include "CompositeChat.h"





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cCompositeChat:

cCompositeChat::cCompositeChat(void) :
	m_MessageType(mtCustom)
{
}





cCompositeChat::cCompositeChat(const AString & a_ParseText) :
	m_MessageType(mtCustom)
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





void cCompositeChat::ParseText(const AString & a_ParseText)
{
	// TODO
}





void cCompositeChat::SetMessageType(eMessageType a_MessageType)
{
	m_MessageType = a_MessageType;
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cCompositeChat::cBasePart:

cCompositeChat::cBasePart::cBasePart(cCompositeChat::ePartType a_PartType, const AString & a_Text, const AString & a_Style) :
	m_PartType(a_PartType),
	m_Text(a_Text),
	m_Style(a_Style)
{
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cCompositeChat::cTextPart:

cCompositeChat::cTextPart::cTextPart(const AString & a_Text, const AString &a_Style) :
	super(ptText, a_Text, a_Style)
{
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cCompositeChat::cClientTranslatedPart:

cCompositeChat::cClientTranslatedPart::cClientTranslatedPart(const AString & a_TranslationID, const AStringVector & a_Parameters, const AString & a_Style) :
	super(ptClientTranslated, a_TranslationID, a_Style),
	m_Parameters(a_Parameters)
{
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cCompositeChat::cUrlPart:

cCompositeChat::cUrlPart::cUrlPart(const AString & a_Text, const AString & a_Url, const AString & a_Style) :
	super(ptUrl, a_Text, a_Style),
	m_Url(a_Url)
{
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cCompositeChat::cCommandPart:

cCompositeChat::cCommandPart::cCommandPart(ePartType a_PartType, const AString & a_Text, const AString & a_Command, const AString & a_Style) :
	super(a_PartType, a_Text, a_Style),
	m_Command(a_Command)
{
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cCompositeChat::cRunCommandPart:

cCompositeChat::cRunCommandPart::cRunCommandPart(const AString & a_Text, const AString & a_Command, const AString & a_Style) :
	super(ptRunCommand, a_Text, a_Command, a_Style)
{
}




///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cCompositeChat::cSuggestCommandPart:

cCompositeChat::cSuggestCommandPart::cSuggestCommandPart(const AString & a_Text, const AString & a_Command, const AString & a_Style) :
	super(ptSuggestCommand, a_Text, a_Command, a_Style)
{
}




