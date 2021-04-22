
// CompositeChatTest.cpp

// Implements the main app entrypoint for the cCompositeChat class test

#include "Globals.h"
#include "../TestHelpers.h"
#include "CompositeChat.h"





static void TestParser1(void)
{
	cCompositeChat Msg;
	Msg.ParseText("Testing @2color codes and http://links parser");
	const auto & Parts = Msg.GetParts();
	TEST_EQUAL(Parts.size(), 4);

	TEST_TRUE(std::holds_alternative<cCompositeChat::TextPart>(Parts[0]));
	TEST_EQUAL(std::get<cCompositeChat::TextPart>(Parts[0]).Style, "");

	TEST_TRUE(std::holds_alternative<cCompositeChat::TextPart>(Parts[1]));
	TEST_EQUAL(std::get<cCompositeChat::TextPart>(Parts[1]).Style, "@2");

	TEST_TRUE(std::holds_alternative<cCompositeChat::UrlPart>(Parts[2]));
	TEST_EQUAL(std::get<cCompositeChat::UrlPart>(Parts[2]).Style, "@2");

	TEST_TRUE(std::holds_alternative<cCompositeChat::TextPart>(Parts[3]));
	TEST_EQUAL(std::get<cCompositeChat::TextPart>(Parts[3]).Style, "@2");
}





static void TestParser2(void)
{
	cCompositeChat Msg;
	Msg.ParseText("@3Advanced stuff: @5overriding color codes and http://links.with/@4color-in-them handling");
	const auto & Parts = Msg.GetParts();
	TEST_EQUAL(Parts.size(), 4);

	TEST_TRUE(std::holds_alternative<cCompositeChat::TextPart>(Parts[0]));
	TEST_EQUAL(std::get<cCompositeChat::TextPart>(Parts[0]).Style, "@3");

	TEST_TRUE(std::holds_alternative<cCompositeChat::TextPart>(Parts[1]));
	TEST_EQUAL(std::get<cCompositeChat::TextPart>(Parts[1]).Style, "@5");

	TEST_TRUE(std::holds_alternative<cCompositeChat::UrlPart>(Parts[2]));
	TEST_EQUAL(std::get<cCompositeChat::UrlPart>(Parts[2]).Style, "@5");

	TEST_TRUE(std::holds_alternative<cCompositeChat::TextPart>(Parts[3]));
	TEST_EQUAL(std::get<cCompositeChat::TextPart>(Parts[3]).Style, "@5");
}





static void TestParser3(void)
{
	cCompositeChat Msg;
	Msg.ParseText("http://links.starting the text");
	const auto & Parts = Msg.GetParts();
	TEST_EQUAL(Parts.size(), 2);

	TEST_TRUE(std::holds_alternative<cCompositeChat::UrlPart>(Parts[0]));
	TEST_EQUAL(std::get<cCompositeChat::UrlPart>(Parts[0]).Style, "");

	TEST_TRUE(std::holds_alternative<cCompositeChat::TextPart>(Parts[1]));
	TEST_EQUAL(std::get<cCompositeChat::TextPart>(Parts[1]).Style, "");
}





static void TestParser4(void)
{
	cCompositeChat Msg;
	Msg.ParseText("links finishing the text: http://some.server");
	const auto & Parts = Msg.GetParts();
	TEST_EQUAL(Parts.size(), 2);

	TEST_TRUE(std::holds_alternative<cCompositeChat::TextPart>(Parts[0]));
	TEST_EQUAL(std::get<cCompositeChat::TextPart>(Parts[0]).Style, "");

	TEST_TRUE(std::holds_alternative<cCompositeChat::UrlPart>(Parts[1]));
	TEST_EQUAL(std::get<cCompositeChat::UrlPart>(Parts[1]).Style, "");
}





static void TestParser5(void)
{
	cCompositeChat Msg;
	Msg.ParseText("http://only.links");
	const auto & Parts = Msg.GetParts();
	TEST_EQUAL(Parts.size(), 1);

	TEST_TRUE(std::holds_alternative<cCompositeChat::UrlPart>(Parts[0]));
	TEST_EQUAL(std::get<cCompositeChat::UrlPart>(Parts[0]).Style, "");
}




static void TestParser6(void)
{
	cCompositeChat Msg;
	Msg.ParseText("Hello World");
	const auto & Parts = Msg.GetParts();
	TEST_EQUAL(Parts.size(), 1);

	TEST_TRUE(std::holds_alternative<cCompositeChat::TextPart>(Parts[0]));
	TEST_EQUAL(std::get<cCompositeChat::TextPart>(Parts[0]).Style, "");
}





IMPLEMENT_TEST_MAIN("CompositeChat",
	TestParser1();
	TestParser2();
	TestParser3();
	TestParser4();
	TestParser5();
	TestParser6();
)
