
#include "Globals.h"
#include "BlockTypeRegistry.h"
#include "../TestHelpers.h"




/** Dummy BlockState implementation */
class BlockState
{
public:
	BlockState() = default;
};




/** Dummy cBlockHandler implementation that allows simple checking for equality through mIdent. */
class cBlockHandler
{
public:
	cBlockHandler(UInt32 aIdent):
		mIdent(aIdent)
	{
	}

	UInt32 mIdent;
};





/** Tests simple block type name registration.
Registers a block type, checks that the type is then registered. */
static void testSimpleReg()
{
	LOGD("Testing simple registration...");

	// Register the block type:
	BlockTypeRegistry reg;
	AString blockTypeName("test:block1");
	AString pluginName("testPlugin");
	AString hint1("testHint1");
	AString hint1Value("value1");
	std::shared_ptr<cBlockHandler> handler(new cBlockHandler(0x12345678));
	std::map<AString, AString> hints = {{hint1, hint1Value}, {"testHint2", "value2"}};
	std::map<AString, BlockInfo::HintCallback> hintCallbacks;
	reg.registerBlockType(pluginName, blockTypeName, handler, hints, hintCallbacks);

	// Query the registration:
	auto blockInfo = reg.blockInfo(blockTypeName);
	TEST_NOTEQUAL(blockInfo, nullptr);
	TEST_EQUAL(blockInfo->blockTypeName(), blockTypeName);
	TEST_EQUAL(blockInfo->pluginName(), pluginName);
	TEST_EQUAL(blockInfo->handler(), handler);
	TEST_EQUAL(blockInfo->hintValue(hint1, BlockState()), hint1Value);
	TEST_EQUAL(blockInfo->hintValue("nonexistent", BlockState()), "");
}





/** Tests setting and removing a BlockType hint. */
static void testHintSetRemove()
{
	LOGD("Testing hint addition and removal...");

	// Register the block type:
	BlockTypeRegistry reg;
	AString blockTypeName("test:block1");
	AString pluginName("testPlugin");
	AString hint1("testHint1");
	AString hint1Value("value1");
	AString hint2("testHint2");
	AString hint2Value("value2");
	std::shared_ptr<cBlockHandler> handler(new cBlockHandler(0x12345678));
	std::map<AString, AString> hints = {{hint1, hint1Value}};
	std::map<AString, BlockInfo::HintCallback> hintCallbacks;
	reg.registerBlockType(pluginName, blockTypeName, handler, hints, hintCallbacks);

	// Modify the hints:
	auto blockInfo = reg.blockInfo(blockTypeName);
	reg.setBlockTypeHint(blockTypeName, hint2, hint2Value);
	TEST_EQUAL(blockInfo->hintValue(hint2, BlockState()), hint2Value);  // Was created successfully
	reg.setBlockTypeHint(blockTypeName, hint1, "testValue2");
	TEST_EQUAL(blockInfo->hintValue(hint1, BlockState()), "testValue2");  // Was updated successfully
	reg.removeBlockTypeHint(blockTypeName, hint2);
	TEST_EQUAL(blockInfo->hintValue(hint2, BlockState()), "");  // Was removed successfully

	// Test the error reporting:
	TEST_THROWS(reg.setBlockTypeHint("nonexistent", "hint", "value"), BlockTypeRegistry::NotRegisteredException);
	reg.removeBlockTypeHint(blockTypeName, "nonexistent");  // Should fail silently
}





/** Tests that the plugin-based information is used correctly for registration.
Registers two different block types with two different plugins, then tries to re-register them from a different plugin.
Finally removes the registration through removeAllByPlugin() and checks its success. */
static void testPlugins()
{
	LOGD("Testing plugin-based checks / removal...");

	// Register the block types:
	BlockTypeRegistry reg;
	AString blockTypeName1("test:block1");
	AString pluginName1("testPlugin1");
	AString hint1("testHint1");
	AString hint1Value("value1");
	std::shared_ptr<cBlockHandler> handler1(new cBlockHandler(1));
	std::map<AString, AString> hints = {{hint1, hint1Value}, {"testHint2", "value2"}};
	std::map<AString, BlockInfo::HintCallback> hintCallbacks;
	reg.registerBlockType(pluginName1, blockTypeName1, handler1, hints, hintCallbacks);
	AString blockTypeName2("test:block2");
	AString pluginName2("testPlugin2");
	std::shared_ptr<cBlockHandler> handler2(new cBlockHandler(2));
	reg.registerBlockType(pluginName2, blockTypeName2, handler2, hints, hintCallbacks);

	// Test the refusal to register under a different plugin:
	TEST_THROWS(reg.registerBlockType(pluginName2, blockTypeName1, handler2, hints, hintCallbacks), BlockTypeRegistry::AlreadyRegisteredException);
	TEST_EQUAL(reg.blockInfo(blockTypeName1)->handler()->mIdent, 1);  // Did we overwrite the old registration?
	reg.registerBlockType(pluginName1, blockTypeName1, handler1, hints, hintCallbacks);  // Re-registering must succeed

	// Unregister by plugin, then re-register from a different plugin:
	reg.removeAllByPlugin(pluginName1);
	TEST_EQUAL(reg.blockInfo(blockTypeName1), nullptr);  // Unregistered successfully
	TEST_NOTEQUAL(reg.blockInfo(blockTypeName2), nullptr);  // Didn't unregister from the other plugin
	std::shared_ptr<cBlockHandler> handler3(new cBlockHandler(3));
	reg.registerBlockType(pluginName2, blockTypeName1, handler3, hints, hintCallbacks);
	TEST_NOTEQUAL(reg.blockInfo(blockTypeName1), nullptr);  // Registered successfully
	TEST_EQUAL(reg.blockInfo(blockTypeName1)->pluginName(), pluginName2);
	TEST_EQUAL(reg.blockInfo(blockTypeName1)->handler()->mIdent, 3);
	TEST_EQUAL(reg.blockInfo(blockTypeName2)->handler()->mIdent, 2);
	reg.removeAllByPlugin(pluginName2);
	TEST_EQUAL(reg.blockInfo(blockTypeName1), nullptr);  // Unregistered successfully
	TEST_EQUAL(reg.blockInfo(blockTypeName2), nullptr);  // Unregistered successfully
}




/** Tests that the callback-based hints work properly. */
static void testHintCallbacks()
{
	LOGD("Testing hint callbacks...");

	// Register the block type:
	BlockTypeRegistry reg;
	AString blockTypeName("test:block1");
	AString pluginName("testPlugin");
	AString hint1("testHint1");
	AString hint1Value("value1");
	AString hc1("hintCallback1");
	int callbackCount = 0;
	auto callback1 = [&callbackCount](const AString & aBlockType, const BlockState & aBlockState)
	{
		callbackCount = callbackCount + 1;
		return aBlockType + "_hint";
	};
	std::shared_ptr<cBlockHandler> handler(new cBlockHandler(0x12345678));
	std::map<AString, AString> hints = {{hint1, hint1Value}, {"testHint2", "value2"}};
	std::map<AString, BlockInfo::HintCallback> hintCallbacks = {{hc1, callback1}};
	reg.registerBlockType(pluginName, blockTypeName, handler, hints, hintCallbacks);

	// Check that querying the hint using a callback works:
	TEST_EQUAL(reg.blockInfo(blockTypeName)->hintValue(hc1, BlockState()), blockTypeName + "_hint");
	TEST_EQUAL(callbackCount, 1);  // Called exactly once
}





/** Tests whether thread-locking works properly by running two threads,
one constantly (re-)registering and the other one constantly querying the same block type. */
static void testThreadLocking()
{
	LOGD("Testing thread locking...");

	// Register the block type:
	BlockTypeRegistry reg;
	AString blockTypeName("test:block1");
	AString pluginName("testPlugin");
	AString hint1("testHint1");
	AString hint1Value("value1");
	std::shared_ptr<cBlockHandler> handler(new cBlockHandler(0x12345678));
	std::map<AString, AString> hints = {{hint1, hint1Value}, {"testHint2", "value2"}};
	std::map<AString, BlockInfo::HintCallback> hintCallbacks;
	reg.registerBlockType(pluginName, blockTypeName, handler, hints, hintCallbacks);

	// Run the two threads for at least a second:
	auto endTime = time(nullptr) + 2;
	auto keepRegistering = [&]()
	{
		while (time(nullptr) < endTime)
		{
			reg.registerBlockType(pluginName, blockTypeName, handler, hints, hintCallbacks);
		}
	};
	auto keepQuerying = [&]()
	{
		unsigned numQueries = 0;
		while (time(nullptr) < endTime)
		{
			TEST_NOTEQUAL(reg.blockInfo(blockTypeName), nullptr);
			numQueries += 1;
		}
		LOGD("%u queries have been executed", numQueries);
	};
	std::thread thr1(keepRegistering);
	std::thread thr2(keepQuerying);
	thr1.join();
	thr2.join();
}





static void testBlockTypeRegistry()
{
	testSimpleReg();
	testHintSetRemove();
	testPlugins();
	testHintCallbacks();
	testThreadLocking();
}





IMPLEMENT_TEST_MAIN("BlockTypeRegistryTest",
	testBlockTypeRegistry();
)
