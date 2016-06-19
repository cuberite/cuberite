
// CompositeChatTest.cpp

// Implements the main app entrypoint for the cCompositeChat class test

#include "Globals.h"
#include "CompositeChat.h"





static void TestParser1(void)
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





static void TestParser2(void)
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





static void TestParser3(void)
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





static void TestParser4(void)
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





static void TestParser5(void)
{
	cCompositeChat Msg;
	Msg.ParseText("http://only.links");
	const cCompositeChat::cParts & Parts = Msg.GetParts();
	assert_test(Parts.size() == 1);
	assert_test(Parts[0]->m_PartType == cCompositeChat::ptUrl);
	assert_test(Parts[0]->m_Style == "");
}





int main(int argc, char * argv[])
{
	LOGD("Test started.");

	LOGD("Running tests: 1");
	TestParser1();

	LOGD("Running tests: 2");
	TestParser2();

	LOGD("Running tests: 3");
	TestParser3();

	LOGD("Running tests: 4");
	TestParser4();

	LOGD("Running tests: 5");
	TestParser5();

	LOG("CompositeChat test finished.");
}





