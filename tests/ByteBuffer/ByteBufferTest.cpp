
// ByteBufferTest.cpp

// Implements the main app entrypoint for the cByteBuffer class test

#include "Globals.h"
#include "ByteBuffer.h"





static void TestRead(void)
{
	cByteBuffer buf(50);
	buf.Write("\x05\xac\x02\x00", 4);
	UInt32 v1;
	assert_test(buf.ReadVarInt(v1) && (v1 == 5));
	UInt32 v2;
	assert_test(buf.ReadVarInt(v2) && (v2 == 300));
	UInt32 v3;
	assert_test(buf.ReadVarInt(v3) && (v3 == 0));
}





static void TestWrite(void)
{
	cByteBuffer buf(50);
	buf.WriteVarInt32(5);
	buf.WriteVarInt32(300);
	buf.WriteVarInt32(0);
	AString All;
	buf.ReadAll(All);
	assert_test(All.size() == 4);
	assert_test(memcmp(All.data(), "\x05\xac\x02\x00", All.size()) == 0);
}





static void TestWrap(void)
{
	cByteBuffer buf(3);
	for (int i = 0; i < 1000; i++)
	{
		size_t FreeSpace = buf.GetFreeSpace();
		assert_test(buf.GetReadableSpace() == 0);
		assert_test(FreeSpace > 0);
		assert_test(buf.Write("a", 1));
		assert_test(buf.CanReadBytes(1));
		assert_test(buf.GetReadableSpace() == 1);
		UInt8 v = 0;
		assert_test(buf.ReadBEUInt8(v));
		assert_test(v == 'a');
		assert_test(buf.GetReadableSpace() == 0);
		buf.CommitRead();
		assert_test(buf.GetFreeSpace() == FreeSpace);  // We're back to normal
	}
}





int main(int argc, char * argv[])
{
	LOGD("Test started");

	LOGD("Testing reads");
	TestRead();

	LOGD("Testing writes");
	TestWrite();

	LOGD("Testing wraps");
	TestWrap();

	LOG("ByteBuffer test finished.");
}





