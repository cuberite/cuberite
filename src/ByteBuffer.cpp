
// ByteBuffer.cpp

// Implements the cByteBuffer class representing a ringbuffer of bytes

#include "Globals.h"

#include "ByteBuffer.h"
#include "Endianness.h"
#include "OSSupport/IsThread.h"





// Try to determine endianness:
#if ( \
	defined(__i386__) || defined(__alpha__) || \
	defined(__ia64) || defined(__ia64__) || \
	defined(_M_IX86) || defined(_M_IA64) || \
	defined(_M_ALPHA) || defined(__amd64) || \
	defined(__amd64__) || defined(_M_AMD64) || \
	defined(__x86_64) || defined(__x86_64__) || \
	defined(_M_X64) || defined(__bfin__) || \
	defined(__ARMEL__) || \
	(defined(_WIN32) && defined(__ARM__) && defined(_MSC_VER)) \
)
	#define IS_LITTLE_ENDIAN
#elif ( \
	defined (__ARMEB__) || defined(__sparc) \
)
	#define IS_BIG_ENDIAN
#else
	#error Cannot determine endianness of this platform
#endif

// If a string sent over the protocol is larger than this, a warning is emitted to the console
#define MAX_STRING_SIZE (512 KiB)

#define NEEDBYTES(Num) if (!CanReadBytes(Num))  return false;  // Check if at least Num bytes can be read from  the buffer, return false if not
#define PUTBYTES(Num)  if (!CanWriteBytes(Num)) return false;  // Check if at least Num bytes can be written to the buffer, return false if not





#ifdef SELF_TEST

/// Self-test of the VarInt-reading and writing code
static class cByteBufferSelfTest
{
public:
	cByteBufferSelfTest(void)
	{
		TestRead();
		TestWrite();
		TestWrap();
	}
	
	void TestRead(void)
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
	
	void TestWrite(void)
	{
		cByteBuffer buf(50);
		buf.WriteVarInt(5);
		buf.WriteVarInt(300);
		buf.WriteVarInt(0);
		AString All;
		buf.ReadAll(All);
		assert_test(All.size() == 4);
		assert_test(memcmp(All.data(), "\x05\xac\x02\x00", All.size()) == 0);
	}
	
	void TestWrap(void)
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
			unsigned char v = 0;
			assert_test(buf.ReadByte(v));
			assert_test(v == 'a');
			assert_test(buf.GetReadableSpace() == 0);
			buf.CommitRead();
			assert_test(buf.GetFreeSpace() == FreeSpace);  // We're back to normal
		}
	}
	
} g_ByteBufferTest;

#endif





#ifdef _DEBUG

/// Simple RAII class that uses one internal unsigned long for checking if two threads are using an object simultanously
class cSingleThreadAccessChecker
{
public:
	cSingleThreadAccessChecker(unsigned long * a_ThreadID) :
		m_ThreadID(a_ThreadID)
	{
		ASSERT((*a_ThreadID == 0) || (*a_ThreadID == cIsThread::GetCurrentID()));
	}
	
	~cSingleThreadAccessChecker()
	{
		*m_ThreadID = 0;
	}
	
protected:
	unsigned long * m_ThreadID;
} ;

#define CHECK_THREAD cSingleThreadAccessChecker Checker(const_cast<unsigned long *>(&m_ThreadID))

#else
	#define CHECK_THREAD
#endif





////////////////////////////////////////////////////////////////////////////////
// cByteBuffer:

cByteBuffer::cByteBuffer(size_t a_BufferSize) :
	m_Buffer(new char[a_BufferSize + 1]),
	m_BufferSize(a_BufferSize + 1),
	#ifdef _DEBUG
	m_ThreadID(0),
	#endif  // _DEBUG
	m_DataStart(0),
	m_WritePos(0),
	m_ReadPos(0)
{
	// Allocating one byte more than the buffer size requested, so that we can distinguish between
	// completely-full and completely-empty states
}





cByteBuffer::~cByteBuffer()
{
	CheckValid();
	delete[] m_Buffer;
	m_Buffer = NULL;
}





bool cByteBuffer::Write(const void * a_Bytes, size_t a_Count)
{
	CHECK_THREAD;
	CheckValid();

	// Store the current free space for a check after writing:
	size_t CurFreeSpace = GetFreeSpace();
	size_t CurReadableSpace = GetReadableSpace();
	size_t WrittenBytes = 0;
	
	if (CurFreeSpace < a_Count)
	{
		return false;
	}
	ASSERT(m_BufferSize >= m_WritePos);
	size_t TillEnd = m_BufferSize - m_WritePos;
	const char * Bytes = (const char *)a_Bytes;
	if (TillEnd <= a_Count)
	{
		// Need to wrap around the ringbuffer end
		if (TillEnd > 0)
		{
			memcpy(m_Buffer + m_WritePos, Bytes, TillEnd);
			Bytes += TillEnd;
			a_Count -= TillEnd;
			WrittenBytes = TillEnd;
		}
		m_WritePos = 0;
	}
	
	// We're guaranteed that we'll fit in a single write op
	if (a_Count > 0)
	{
		memcpy(m_Buffer + m_WritePos, Bytes, a_Count);
		m_WritePos += a_Count;
		WrittenBytes += a_Count;
	}
	
	ASSERT(GetFreeSpace() == CurFreeSpace - WrittenBytes);
	ASSERT(GetReadableSpace() == CurReadableSpace + WrittenBytes);
	return true;
}





size_t cByteBuffer::GetFreeSpace(void) const
{
	CHECK_THREAD;
	CheckValid();
	if (m_WritePos >= m_DataStart)
	{
		// Wrap around the buffer end:
		ASSERT(m_BufferSize >= m_WritePos);
		ASSERT((m_BufferSize - m_WritePos + m_DataStart) >= 1);
		return m_BufferSize - m_WritePos + m_DataStart - 1;
	}
	// Single free space partition:
	ASSERT(m_BufferSize >= m_WritePos);
	ASSERT(m_BufferSize - m_WritePos >= 1);
	return m_DataStart - m_WritePos - 1;
}





/// Returns the number of bytes that are currently in the ringbuffer. Note GetReadableBytes()
size_t cByteBuffer::GetUsedSpace(void) const
{
	CHECK_THREAD;
	CheckValid();
	ASSERT(m_BufferSize >= GetFreeSpace());
	ASSERT((m_BufferSize - GetFreeSpace()) >= 1);
	return m_BufferSize - GetFreeSpace() - 1;
}





/// Returns the number of bytes that are currently available for reading (may be less than UsedSpace due to some data having been read already)
size_t cByteBuffer::GetReadableSpace(void) const
{
	CHECK_THREAD;
	CheckValid();
	if (m_ReadPos > m_WritePos)
	{
		// Wrap around the buffer end:
		ASSERT(m_BufferSize >= m_ReadPos);
		return m_BufferSize - m_ReadPos + m_WritePos;
	}
	// Single readable space partition:
	ASSERT(m_WritePos >= m_ReadPos);
	return m_WritePos - m_ReadPos ;
}





bool cByteBuffer::CanReadBytes(size_t a_Count) const
{
	CHECK_THREAD;
	CheckValid();
	return (a_Count <= GetReadableSpace());
}





bool cByteBuffer::CanWriteBytes(size_t a_Count) const
{
	CHECK_THREAD;
	CheckValid();
	return (a_Count <= GetFreeSpace());
}





bool cByteBuffer::ReadChar(char & a_Value)
{
	CHECK_THREAD;
	CheckValid();
	NEEDBYTES(1);
	ReadBuf(&a_Value, 1);
	return true;
}





bool cByteBuffer::ReadByte(unsigned char & a_Value)
{
	CHECK_THREAD;
	CheckValid();
	NEEDBYTES(1);
	ReadBuf(&a_Value, 1);
	return true;
}





bool cByteBuffer::ReadBEShort(short & a_Value)
{
	CHECK_THREAD;
	CheckValid();
	NEEDBYTES(2);
	ReadBuf(&a_Value, 2);
	a_Value = (short)ntohs((u_short)a_Value);
	return true;
}





bool cByteBuffer::ReadBEInt(int & a_Value)
{
	CHECK_THREAD;
	CheckValid();
	NEEDBYTES(4);
	ReadBuf(&a_Value, 4);
	a_Value = (int)ntohl((u_long)a_Value);
	return true;
}





bool cByteBuffer::ReadBEInt64(Int64 & a_Value)
{
	CHECK_THREAD;
	CheckValid();
	NEEDBYTES(8);
	ReadBuf(&a_Value, 8);
	a_Value = NetworkToHostLong8(&a_Value);
	return true;
}





bool cByteBuffer::ReadBEFloat(float & a_Value)
{
	CHECK_THREAD;
	CheckValid();
	NEEDBYTES(4);
	ReadBuf(&a_Value, 4);
	a_Value = NetworkToHostFloat4(&a_Value);
	return true;
}





bool cByteBuffer::ReadBEDouble(double & a_Value)
{
	CHECK_THREAD;
	CheckValid();
	NEEDBYTES(8);
	ReadBuf(&a_Value, 8);
	a_Value = NetworkToHostDouble8(&a_Value);
	return true;
}





bool cByteBuffer::ReadBool(bool & a_Value)
{
	CHECK_THREAD;
	CheckValid();
	NEEDBYTES(1);
	char Value = 0;
	ReadBuf(&Value, 1);
	a_Value = (Value != 0);
	return true;
}





bool cByteBuffer::ReadBEUTF16String16(AString & a_Value)
{
	CHECK_THREAD;
	CheckValid();
	short Length;
	if (!ReadBEShort(Length))
	{
		return false;
	}
	if (Length < 0)
	{
		ASSERT(!"Negative string length? Are you sure?");
		return true;
	}
	return ReadUTF16String(a_Value, (size_t)Length);
}





bool cByteBuffer::ReadVarInt(UInt32 & a_Value)
{
	CHECK_THREAD;
	CheckValid();
	UInt32 Value = 0;
	int Shift = 0;
	unsigned char b = 0;
	do
	{
		NEEDBYTES(1);
		ReadBuf(&b, 1);
		Value = Value | (((UInt32)(b & 0x7f)) << Shift);
		Shift += 7;
	} while ((b & 0x80) != 0);
	a_Value = Value;
	return true;
}





bool cByteBuffer::ReadVarUTF8String(AString & a_Value)
{
	CHECK_THREAD;
	CheckValid();
	UInt32 Size = 0;
	if (!ReadVarInt(Size))
	{
		return false;
	}
	if (Size > MAX_STRING_SIZE)
	{
		LOGWARNING("%s: String too large: %u (%u KiB)", __FUNCTION__, Size, Size / 1024);
	}
	return ReadString(a_Value, (size_t)Size);
}





bool cByteBuffer::ReadLEInt(int & a_Value)
{
	CHECK_THREAD;
	CheckValid();
	NEEDBYTES(4);
	ReadBuf(&a_Value, 4);
	
	#ifdef IS_BIG_ENDIAN
		// Convert:
		a_Value = ((a_Value >> 24) & 0xff) | ((a_Value >> 16) & 0xff00) | ((a_Value >> 8) & 0xff0000) | (a_Value & 0xff000000);
	#endif
	
	return true;
}





bool cByteBuffer::WriteChar(char a_Value)
{
	CHECK_THREAD;
	CheckValid();
	PUTBYTES(1);
	return WriteBuf(&a_Value, 1);
}





bool cByteBuffer::WriteByte(unsigned char a_Value)
{
	CHECK_THREAD;
	CheckValid();
	PUTBYTES(1);
	return WriteBuf(&a_Value, 1);
}





bool cByteBuffer::WriteBEShort(short a_Value)
{
	CHECK_THREAD;
	CheckValid();
	PUTBYTES(2);
	u_short Converted = htons((u_short)a_Value);
	return WriteBuf(&Converted, 2);
}





bool cByteBuffer::WriteBEInt(int a_Value)
{
	CHECK_THREAD;
	CheckValid();
	PUTBYTES(4);
	UInt32 Converted = HostToNetwork4(&a_Value);
	return WriteBuf(&Converted, 4);
}





bool cByteBuffer::WriteBEInt64(Int64 a_Value)
{
	CHECK_THREAD;
	CheckValid();
	PUTBYTES(8);
	UInt64 Converted = HostToNetwork8(&a_Value);
	return WriteBuf(&Converted, 8);
}





bool cByteBuffer::WriteBEFloat(float a_Value)
{
	CHECK_THREAD;
	CheckValid();
	PUTBYTES(4);
	UInt32 Converted = HostToNetwork4(&a_Value);
	return WriteBuf(&Converted, 4);
}





bool cByteBuffer::WriteBEDouble(double a_Value)
{
	CHECK_THREAD;
	CheckValid();
	PUTBYTES(8);
	UInt64 Converted = HostToNetwork8(&a_Value);
	return WriteBuf(&Converted, 8);
}






bool cByteBuffer::WriteBool(bool a_Value)
{
	CHECK_THREAD;
	CheckValid();
	return WriteChar(a_Value ? 1 : 0);
}





bool cByteBuffer::WriteBEUTF16String16(const AString & a_Value)
{
	CHECK_THREAD;
	CheckValid();
	PUTBYTES(2);
	AString UTF16BE;
	UTF8ToRawBEUTF16(a_Value.data(), a_Value.size(), UTF16BE);
	WriteBEShort((short)(UTF16BE.size() / 2));
	PUTBYTES(UTF16BE.size());
	WriteBuf(UTF16BE.data(), UTF16BE.size());
	return true;
}





bool cByteBuffer::WriteVarInt(UInt32 a_Value)
{
	CHECK_THREAD;
	CheckValid();
	
	// A 32-bit integer can be encoded by at most 5 bytes:
	unsigned char b[5];
	size_t idx = 0;
	do
	{
		b[idx] = (a_Value & 0x7f) | ((a_Value > 0x7f) ? 0x80 : 0x00);
		a_Value = a_Value >> 7;
		idx++;
	} while (a_Value > 0);

	return WriteBuf(b, idx);
}




bool cByteBuffer::WriteVarUTF8String(const AString & a_Value)
{
	CHECK_THREAD;
	CheckValid();
	PUTBYTES(a_Value.size() + 1);  // This is a lower-bound on the bytes that will be actually written. Fail early.
	bool res = WriteVarInt((UInt32)(a_Value.size()));
	if (!res)
	{
		return false;
	}
	return WriteBuf(a_Value.data(), a_Value.size());
}





bool cByteBuffer::WriteLEInt(int a_Value)
{
	CHECK_THREAD;
	CheckValid();
	#ifdef IS_LITTLE_ENDIAN
		return WriteBuf((const char *)&a_Value, 4);
	#else
		int Value = ((a_Value >> 24) & 0xff) | ((a_Value >> 16) & 0xff00) | ((a_Value >> 8) & 0xff0000) | (a_Value & 0xff000000);
		return WriteBuf((const char *)&Value, 4);
	#endif
}





bool cByteBuffer::ReadBuf(void * a_Buffer, size_t a_Count)
{
	CHECK_THREAD;
	CheckValid();
	NEEDBYTES(a_Count);
	char * Dst = (char *)a_Buffer;  // So that we can do byte math
	ASSERT(m_BufferSize >= m_ReadPos);
	size_t BytesToEndOfBuffer = m_BufferSize - m_ReadPos;
	if (BytesToEndOfBuffer <= a_Count)
	{
		// Reading across the ringbuffer end, read the first part and adjust parameters:
		if (BytesToEndOfBuffer > 0)
		{
			memcpy(Dst, m_Buffer + m_ReadPos, BytesToEndOfBuffer);
			Dst += BytesToEndOfBuffer;
			a_Count -= BytesToEndOfBuffer;
		}
		m_ReadPos = 0;
	}
	
	// Read the rest of the bytes in a single read (guaranteed to fit):
	if (a_Count > 0)
	{
		memcpy(Dst, m_Buffer + m_ReadPos, a_Count);
		m_ReadPos += a_Count;
	}
	return true;
}





bool cByteBuffer::WriteBuf(const void * a_Buffer, size_t a_Count)
{
	CHECK_THREAD;
	CheckValid();
	PUTBYTES(a_Count);
	char * Src = (char *)a_Buffer;  // So that we can do byte math
	ASSERT(m_BufferSize >= m_ReadPos);
	size_t BytesToEndOfBuffer = m_BufferSize - m_WritePos;
	if (BytesToEndOfBuffer <= a_Count)
	{
		// Reading across the ringbuffer end, read the first part and adjust parameters:
		memcpy(m_Buffer + m_WritePos, Src, BytesToEndOfBuffer);
		Src += BytesToEndOfBuffer;
		a_Count -= BytesToEndOfBuffer;
		m_WritePos = 0;
	}
	
	// Read the rest of the bytes in a single read (guaranteed to fit):
	if (a_Count > 0)
	{
		memcpy(m_Buffer + m_WritePos, Src, a_Count);
		m_WritePos += a_Count;
	}
	return true;
}





bool cByteBuffer::ReadString(AString & a_String, size_t a_Count)
{
	CHECK_THREAD;
	CheckValid();
	NEEDBYTES(a_Count);
	a_String.clear();
	a_String.reserve(a_Count);
	ASSERT(m_BufferSize >= m_ReadPos);
	size_t BytesToEndOfBuffer = m_BufferSize - m_ReadPos;
	if (BytesToEndOfBuffer <= a_Count)
	{
		// Reading across the ringbuffer end, read the first part and adjust parameters:
		if (BytesToEndOfBuffer > 0)
		{
			a_String.assign(m_Buffer + m_ReadPos, BytesToEndOfBuffer);
			ASSERT(a_Count >= BytesToEndOfBuffer);
			a_Count -= BytesToEndOfBuffer;
		}
		m_ReadPos = 0;
	}

	// Read the rest of the bytes in a single read (guaranteed to fit):
	if (a_Count > 0)
	{
		a_String.append(m_Buffer + m_ReadPos, a_Count);
		m_ReadPos += a_Count;
	}
	return true;
}





bool cByteBuffer::ReadUTF16String(AString & a_String, size_t a_NumChars)
{
	// Reads 2 * a_NumChars bytes and interprets it as a UTF16 string, converting it into UTF8 string a_String
	CHECK_THREAD;
	CheckValid();
	AString RawData;
	if (!ReadString(RawData, a_NumChars * 2))
	{
		return false;
	}
	RawBEToUTF8(RawData.data(), a_NumChars, a_String);
	return true;
}





bool cByteBuffer::SkipRead(size_t a_Count)
{
	CHECK_THREAD;
	CheckValid();
	if (!CanReadBytes(a_Count))
	{
		return false;
	}
	AdvanceReadPos(a_Count);
	return true;
}





void cByteBuffer::ReadAll(AString & a_Data)
{
	CHECK_THREAD;
	CheckValid();
	ReadString(a_Data, GetReadableSpace());
}





bool cByteBuffer::ReadToByteBuffer(cByteBuffer & a_Dst, size_t a_NumBytes)
{
	if (!a_Dst.CanWriteBytes(a_NumBytes) || !CanReadBytes(a_NumBytes))
	{
		// There's not enough source bytes or space in the dest BB
		return false;
	}
	char buf[1024];
	// > 0 without generating warnings about unsigned comparisons where size_t is unsigned
	while (a_NumBytes != 0)
	{
		size_t num = (a_NumBytes > sizeof(buf)) ? sizeof(buf) : a_NumBytes;
		VERIFY(ReadBuf(buf, num));
		VERIFY(a_Dst.Write(buf, num));
		ASSERT(a_NumBytes >= num);
		a_NumBytes -= num;
	}
	return true;
}





void cByteBuffer::CommitRead(void)
{
	CHECK_THREAD;
	CheckValid();
	m_DataStart = m_ReadPos;
}





void cByteBuffer::ResetRead(void)
{
	CHECK_THREAD;
	CheckValid();
	m_ReadPos = m_DataStart;
}





void cByteBuffer::ReadAgain(AString & a_Out)
{
	// Return the data between m_DataStart and m_ReadPos (the data that has been read but not committed)
	// Used by ProtoProxy to repeat communication twice, once for parsing and the other time for the remote party
	CHECK_THREAD;
	CheckValid();
	size_t DataStart = m_DataStart;
	if (m_ReadPos < m_DataStart)
	{
		// Across the ringbuffer end, read the first part and adjust next part's start:
		ASSERT(m_BufferSize >= m_DataStart);
		a_Out.append(m_Buffer + m_DataStart, m_BufferSize - m_DataStart);
		DataStart = 0;
	}
	ASSERT(m_ReadPos >= DataStart);
	a_Out.append(m_Buffer + DataStart, m_ReadPos - DataStart);
}





void cByteBuffer::AdvanceReadPos(size_t a_Count)
{
	CHECK_THREAD;
	CheckValid();
	m_ReadPos += a_Count;
	if (m_ReadPos >= m_BufferSize)
	{
		m_ReadPos -= m_BufferSize;
	}
}





void cByteBuffer::CheckValid(void) const
{
	ASSERT(m_ReadPos < m_BufferSize);
	ASSERT(m_WritePos < m_BufferSize);
}





