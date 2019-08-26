
// CompositeChatTest.cpp

// Implements the main app entrypoint for the cCompositeChat class test

#include "Globals.h"
#include "../TestHelpers.h"
#include "CompositeChat.h"





static void TestParser1(void)
{
	cCompositeChat Msg;
	Msg.ParseText("Testing @2color codes and http://links parser");
	const cCompositeChat::cParts & Parts = Msg.GetParts();
	TEST_EQUAL(Parts.size(), 4);
	TEST_EQUAL(Parts[0]->m_PartType, cCompositeChat::ptText);
	TEST_EQUAL(Parts[1]->m_PartType, cCompositeChat::ptText);
	TEST_EQUAL(Parts[2]->m_PartType, cCompositeChat::ptUrl);
	TEST_EQUAL(Parts[3]->m_PartType, cCompositeChat::ptText);
	TEST_EQUAL(Parts[0]->m_Style, "");
	TEST_EQUAL(Parts[1]->m_Style, "@2");
	TEST_EQUAL(Parts[2]->m_Style, "@2");
	TEST_EQUAL(Parts[3]->m_Style, "@2");
}





static void TestParser2(void)
{
	cCompositeChat Msg;
	Msg.ParseText("@3Advanced stuff: @5overriding color codes and http://links.with/@4color-in-them handling");
	const cCompositeChat::cParts & Parts = Msg.GetParts();
	TEST_EQUAL(Parts.size(), 4);
	TEST_EQUAL(Parts[0]->m_PartType, cCompositeChat::ptText);
	TEST_EQUAL(Parts[1]->m_PartType, cCompositeChat::ptText);
	TEST_EQUAL(Parts[2]->m_PartType, cCompositeChat::ptUrl);
	TEST_EQUAL(Parts[3]->m_PartType, cCompositeChat::ptText);
	TEST_EQUAL(Parts[0]->m_Style, "@3");
	TEST_EQUAL(Parts[1]->m_Style, "@5");
	TEST_EQUAL(Parts[2]->m_Style, "@5");
	TEST_EQUAL(Parts[3]->m_Style, "@5");
}





static void TestParser3(void)
{
	cCompositeChat Msg;
	Msg.ParseText("http://links.starting the text");
	const cCompositeChat::cParts & Parts = Msg.GetParts();
	TEST_EQUAL(Parts.size(), 2);
	TEST_EQUAL(Parts[0]->m_PartType, cCompositeChat::ptUrl);
	TEST_EQUAL(Parts[1]->m_PartType, cCompositeChat::ptText);
	TEST_EQUAL(Parts[0]->m_Style, "");
	TEST_EQUAL(Parts[1]->m_Style, "");
}





static void TestParser4(void)
{
	cCompositeChat Msg;
	Msg.ParseText("links finishing the text: http://some.server");
	const cCompositeChat::cParts & Parts = Msg.GetParts();
	TEST_EQUAL(Parts.size(), 2);
	TEST_EQUAL(Parts[0]->m_PartType, cCompositeChat::ptText);
	TEST_EQUAL(Parts[1]->m_PartType, cCompositeChat::ptUrl);
	TEST_EQUAL(Parts[0]->m_Style, "");
	TEST_EQUAL(Parts[1]->m_Style, "");
}





static void TestParser5(void)
{
	cCompositeChat Msg;
	Msg.ParseText("http://only.links");
	const cCompositeChat::cParts & Parts = Msg.GetParts();
	TEST_EQUAL(Parts.size(), 1);
	TEST_EQUAL(Parts[0]->m_PartType, cCompositeChat::ptUrl);
	TEST_EQUAL(Parts[0]->m_Style, "");
}





IMPLEMENT_TEST_MAIN("CompositeChat",
	TestParser1();
	TestParser2();
	TestParser3();
	TestParser4();
	TestParser5();
)
