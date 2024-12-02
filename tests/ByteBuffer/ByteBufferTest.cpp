
// ByteBufferTest.cpp

// Implements the main app entrypoint for the cByteBuffer class test

#include "Globals.h"
#include <gtest/gtest.h>
#include "ByteBuffer.h"


TEST(ByteBufferTest, ConstructorDestructorTest)
{
	// This test case just validates that the ByteBuffer is constructed/destructed without throwing an exception
	EXPECT_NO_THROW(cByteBuffer buffer(100));

	// We can add destructor validation by creating a buffer and deleting it
	auto * pBuffer = new cByteBuffer(100);
	EXPECT_NO_THROW(delete pBuffer);
}


TEST(ByteBufferTest, GetSpacesAndDataStartTest)
{
	cByteBuffer buffer(50);
	EXPECT_EQ(buffer.GetFreeSpace(), 50);
	EXPECT_EQ(buffer.GetUsedSpace(), 0);
	EXPECT_EQ(buffer.GetReadableSpace(), 0);
	EXPECT_EQ(buffer.GetDataStart(), 0);

	AString val("Hello");
	buffer.Write(val.c_str(), val.size());

	EXPECT_EQ(buffer.GetFreeSpace(), 50 - val.size());
	EXPECT_EQ(buffer.GetUsedSpace(), val.size());
	EXPECT_EQ(buffer.GetReadableSpace(), val.size());
	EXPECT_EQ(buffer.GetDataStart(), 0);
}


TEST(ByteBufferTest, CanBEIntXRepresentTest)
{
	// Try values that can and cannot be represented
	EXPECT_TRUE(cByteBuffer::CanBEInt8Represent(127));
	EXPECT_FALSE(cByteBuffer::CanBEInt8Represent(128));
	EXPECT_TRUE(cByteBuffer::CanBEInt16Represent(32767));
	EXPECT_FALSE(cByteBuffer::CanBEInt16Represent(32768));
}


TEST(ByteBufferTest, TemplateReadVarIntTest)
{
	cByteBuffer buffer(50);
	UInt32 val = 300;

	EXPECT_TRUE(buffer.WriteVarInt32(val));

	UInt32 readVal;
	EXPECT_TRUE(buffer.ReadVarInt(readVal));

	// Validate read value:
	EXPECT_EQ(readVal, val);
}

TEST(ByteBufferTest, WriteBufTest)
{
	cByteBuffer buffer(20);
	std::array<unsigned char, 4> val = {0x01, 0x02, 0x03, 0x04};

	EXPECT_TRUE(buffer.WriteBuf(val.data(), val.size()));

	std::array<unsigned char, 4> readVal;
	buffer.ReadBuf(readVal.data(), readVal.size());

	// Validate read buffer data with written buffer data:
	EXPECT_TRUE(std::equal(val.begin(), val.end(), readVal.begin()));
}


TEST(ByteBufferTest, ReadFromSkipTest)
{
	cByteBuffer buffer(20);
	Int8 val1 = 100;
	Int8 val2 = 50;

	buffer.WriteBEInt8(val1);
	buffer.WriteBEInt8(val2);
	buffer.SkipRead(1); // Skip the first value

	Int8 readVal;
	EXPECT_TRUE(buffer.ReadBEInt8(readVal));

	// Validate read value with second written value:
	EXPECT_EQ(readVal, val2);
}


TEST(ByteBufferTest, ReadAllTest)
{
	cByteBuffer buffer(50);

	std::string str = "Testing ReadAll function";
	buffer.Write(str.c_str(), str.size());

	ContiguousByteBuffer readVal;
	buffer.ReadAll(readVal);

	// Since ReadAll doesn't return a value we can check the size of the buffer
	EXPECT_EQ(readVal.size(), str.size());
}


TEST(ByteBufferTest, WriteAndRetrieveBEInt8Test) {
	cByteBuffer buffer(20);
	Int8 testVal = 100;

	EXPECT_TRUE(buffer.WriteBEInt8(testVal));
	Int8 retrievedVal;
	EXPECT_TRUE(buffer.ReadBEInt8(retrievedVal));
	EXPECT_EQ(retrievedVal, testVal);
}


TEST(ByteBufferTest, WriteAndRetrieveBEInt16Test) {
	cByteBuffer buffer(20);
	Int16 testVal = 500;

	EXPECT_TRUE(buffer.WriteBEInt16(testVal));
	Int16 retrievedVal;
	EXPECT_TRUE(buffer.ReadBEInt16(retrievedVal));
	EXPECT_EQ(retrievedVal, testVal);
}


TEST(ByteBufferTest, WriteAndRetrieveBEInt32Test) {
	cByteBuffer buffer(20);
	Int32 testVal = 1500;

	EXPECT_TRUE(buffer.WriteBEInt32(testVal));
	Int32 retrievedVal;
	EXPECT_TRUE(buffer.ReadBEInt32(retrievedVal));
	EXPECT_EQ(retrievedVal, testVal);
}


TEST(ByteBufferTest, WriteAndRetrieveBEInt64Test) {
	cByteBuffer buffer(20);
	Int64 testVal = 5000000000;

	EXPECT_TRUE(buffer.WriteBEInt64(testVal));
	Int64 retrievedVal;
	EXPECT_TRUE(buffer.ReadBEInt64(retrievedVal));
	EXPECT_EQ(retrievedVal, testVal);
}


TEST(ByteBufferTest, WriteAndRetrieveBEUInt32Test) {
	cByteBuffer buffer(20);
	UInt32 testVal = 4294967295;

	EXPECT_TRUE(buffer.WriteBEUInt32(testVal));
	UInt32 retrievedVal;
	EXPECT_TRUE(buffer.ReadBEUInt32(retrievedVal));
	EXPECT_EQ(retrievedVal, testVal);
}


TEST(ByteBufferTest, WriteAndRetrieveBEUInt64Test) {
	cByteBuffer buffer(20);
	UInt64 testVal = 18446744073709551615u; // Max value for UInt64

	EXPECT_TRUE(buffer.WriteBEUInt64(testVal));
	UInt64 retrievedVal;
	EXPECT_TRUE(buffer.ReadBEUInt64(retrievedVal));
	EXPECT_EQ(retrievedVal, testVal);
}


TEST(ByteBufferTest, WriteAndRetrieveBool) {
	cByteBuffer buffer(20);
	bool testVal = true;

	EXPECT_TRUE(buffer.WriteBool(testVal));
	bool retrievedVal;
	EXPECT_TRUE(buffer.ReadBool(retrievedVal));
	EXPECT_EQ(retrievedVal, testVal);
}


TEST(ByteBufferTest, VarIntReadWriteTest) {
	cByteBuffer buffer(50);
	UInt32 testVal = 300;

	EXPECT_TRUE(buffer.WriteVarInt32(testVal));
	UInt32 retrievedVal;
	EXPECT_TRUE(buffer.ReadVarInt32(retrievedVal));
	EXPECT_EQ(retrievedVal, testVal);
}


TEST(ByteBufferTest, WriteAndRetrieveVarUTF8String) {
	cByteBuffer buffer(50);
	AString testVal = "Hello, world!";

	EXPECT_TRUE(buffer.WriteVarUTF8String(testVal));
	AString retrievedVal;
	EXPECT_TRUE(buffer.ReadVarUTF8String(retrievedVal));
	EXPECT_EQ(retrievedVal, testVal);
}


TEST(ByteBufferTest, WriteAndRetrieveBEFloat) {
	cByteBuffer buffer(20);
	float testVal = 123.45f;

	EXPECT_TRUE(buffer.WriteBEFloat(testVal));
	float retrievedVal;
	EXPECT_TRUE(buffer.ReadBEFloat(retrievedVal));
	EXPECT_FLOAT_EQ(retrievedVal, testVal);
}


TEST(ByteBufferTest, WriteAndReadBufTest)
{
	cByteBuffer buffer(50);
	std::string testVal = "This is a test string.";

	bool writeStatus = buffer.WriteBuf(testVal.data(), testVal.length());
	EXPECT_TRUE(writeStatus);

	std::string readVal;
	EXPECT_TRUE(buffer.ReadBuf(readVal.data(), testVal.length()));

	EXPECT_STREQ(readVal.c_str(), testVal.c_str());
}


TEST(ByteBufferTest, WriteBufByteValTest)
{
	cByteBuffer buffer(50);
	unsigned char fillChar = 'A';

	bool writeStatus = buffer.WriteBuf(10, fillChar);
	EXPECT_TRUE(writeStatus);

	std::string retrievedVal;
	EXPECT_TRUE(buffer.ReadBuf(retrievedVal.data(), 10));

	for (const auto & val : retrievedVal)
	{
		EXPECT_EQ(val, fillChar);
	}
}


TEST(ByteBufferTest, ReadSomeTest)
{
	cByteBuffer buffer(50);
	std::string testVal = "Hello, world!";
	EXPECT_TRUE(buffer.Write(testVal.c_str(), testVal.size()));

	ContiguousByteBuffer readVal;
	EXPECT_TRUE(buffer.ReadSome(readVal, 5));

	std::string readValString;
	std::transform(readVal.begin(), readVal.end(), std::back_inserter(readValString),
				   [](std::byte b) { return static_cast<char>(b); });
	EXPECT_EQ(readValString, testVal.substr(0, 5));
}

TEST(ByteBufferTest, SkipReadTest)
{
	cByteBuffer buffer(50);
	UInt16 testData = 100;
	buffer.WriteBEUInt16(testData);

	bool skipStatus = buffer.SkipRead(2);
	EXPECT_TRUE(skipStatus);
}

TEST(ByteBufferTest, GetVarIntSizeTest)
{
	EXPECT_EQ(cByteBuffer::GetVarIntSize(0), 1); // Single byte with all bits set to 0
	EXPECT_EQ(cByteBuffer::GetVarIntSize(1), 1); // Single byte with smallest positive value
	EXPECT_EQ(cByteBuffer::GetVarIntSize(127), 1); // Max single byte value (7 bits all 1s)
	EXPECT_EQ(cByteBuffer::GetVarIntSize(128), 2); // Smallest two-byte value (10000000 00000001)
	EXPECT_EQ(cByteBuffer::GetVarIntSize(16383), 2); // Max two-byte value, (01111111 11111111)
	EXPECT_EQ(cByteBuffer::GetVarIntSize(16384), 3); // Smallest three-byte value (10000000 10000000 00000001)
	EXPECT_EQ(cByteBuffer::GetVarIntSize(2097151), 3); // Max three-byte value (01111111 11111111 11111111)
	EXPECT_EQ(cByteBuffer::GetVarIntSize(2097152), 4); // Smallest four-byte value
	EXPECT_EQ(cByteBuffer::GetVarIntSize(268435455), 4); // Max four-byte value
	EXPECT_EQ(cByteBuffer::GetVarIntSize(268435456), 5); // Smallest five-byte value
}


TEST(ByteBufferTest, WritePastEndTest) {
	cByteBuffer buffer(30);
	std::string val = "This string is longer than 30 characters";
	bool isSuccess = buffer.Write(val.c_str(), val.size());
	EXPECT_FALSE(isSuccess);
}


TEST(ByteBufferTest, ResetAndReadTest) {
	cByteBuffer buffer(50);
	Int32 testVal = 300;
	Int32 testVal2 = 400;
	buffer.WriteBEInt32(testVal);
	buffer.WriteBEInt32(testVal2);
	buffer.ResetRead();
	Int32 retrievedVal;
	EXPECT_TRUE(buffer.ReadBEInt32(retrievedVal));
	EXPECT_EQ(retrievedVal, testVal);
}


TEST(ByteBufferTest, ReadFromEmptyBufferTest) {
	cByteBuffer buffer(20);
	UInt16 retrievedVal;
	EXPECT_FALSE(buffer.ReadBEUInt16(retrievedVal));
}


TEST(ByteBufferTest, WriteToFullBufferTest) {
	cByteBuffer buffer(20);
	AString val("This string is too long for the buffer.");
	EXPECT_FALSE(buffer.Write(val.c_str(), val.size()));
}


TEST(ByteBufferTest, ReadPastEndTest) {
	cByteBuffer buffer(50);
	Int32 testVal = 300;
	buffer.WriteBEInt32(testVal);
	Int32 retrievedVal1, retrievedVal2;
	buffer.ReadBEInt32(retrievedVal1);
	bool isReadSuccess = buffer.ReadBEInt32(retrievedVal2);
	EXPECT_FALSE(isReadSuccess);
}


TEST(ByteBuffer, Wrap)
{
	cByteBuffer buf(3);
	for (int i = 0; i < 1000; i++)
	{
		size_t FreeSpace = buf.GetFreeSpace();
		EXPECT_EQ(buf.GetReadableSpace(), 0);
		EXPECT_GE(FreeSpace, 1);
		EXPECT_TRUE(buf.Write("a", 1));
		EXPECT_TRUE(buf.CanReadBytes(1));
		EXPECT_EQ(buf.GetReadableSpace(), 1);
		UInt8 v = 0;
		EXPECT_TRUE(buf.ReadBEUInt8(v));
		EXPECT_EQ(v, 'a');
		EXPECT_EQ(buf.GetReadableSpace(), 0);
		buf.CommitRead();
		EXPECT_EQ(buf.GetFreeSpace(), FreeSpace);  // We're back to normal
	}
}


TEST(ByteBuffer, XYZPositionRoundtrip)
{
	cByteBuffer buf(50);
	buf.WriteXYZPosition64(-33554432, -2048, -33554432); // Testing the minimun values
	int x, y, z;
	EXPECT_TRUE(buf.ReadXYZPosition64(x, y, z));
	EXPECT_EQ(x, -33554432);
	EXPECT_EQ(y, -2048);
	EXPECT_EQ(z, -33554432);
}


TEST(ByteBuffer, XZYPositionRoundtrip)
{
	cByteBuffer buf(50);
	buf.WriteXZYPosition64(-33554432, -2048, -33554432); // Testing the minimun values
	int x, y, z;
	EXPECT_TRUE(buf.ReadXZYPosition64(x, y, z));
	EXPECT_EQ(x, -33554432);
	EXPECT_EQ(y, -2048);
	EXPECT_EQ(z, -33554432);
}
