
// ByteBuffer.cpp

// Implements the cByteBuffer class representing a ringbuffer of bytes

#include "Globals.h"

#include "ByteBuffer.h"
#include "Endianness.h"
#include "OSSupport/IsThread.h"





// If a string sent over the protocol is larger than this, a warning is emitted to the console
#define MAX_STRING_SIZE (512 KiB)

#define NEEDBYTES(Num) if (!CanReadBytes(Num))  return false;  // Check if at least Num bytes can be read from  the buffer, return false if not
#define PUTBYTES(Num)  if (!CanWriteBytes(Num)) return false;  // Check if at least Num bytes can be written to the buffer, return false if not





#if 0

/// Self-test of the VarInt-reading and writing code
class cByteBufferSelfTest
{
public:
	cByteBufferSelfTest(void)
	{
		TestRead();
		TestWrite();
	}
	
	void TestRead(void)
	{
		cByteBuffer buf(50);
		buf.Write("\x05\xac\x02\x00", 4);
		UInt32 v1;
		ASSERT(buf.ReadVarInt(v1) && (v1 == 5));
		UInt32 v2;
		ASSERT(buf.ReadVarInt(v2) && (v2 == 300));
		UInt32 v3;
		ASSERT(buf.ReadVarInt(v3) && (v3 == 0));
	}
	
	void TestWrite(void)
	{
		cByteBuffer buf(50);
		buf.WriteVarInt(5);
		buf.WriteVarInt(300);
		buf.WriteVarInt(0);
		AString All;
		buf.ReadAll(All);
		ASSERT(All.size() == 4);
		ASSERT(memcmp(All.data(), "\x05\xac\x02\x00", All.size()) == 0);
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





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cByteBuffer:

cByteBuffer::cByteBuffer(int a_BufferSize) :
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
}





bool cByteBuffer::Write(const char * a_Bytes, int a_Count)
{
	CHECK_THREAD;
	CheckValid();

	// Store the current free space for a check after writing:
	int CurFreeSpace = GetFreeSpace();
	int CurReadableSpace = GetReadableSpace();
	int WrittenBytes = 0;
	
	if (GetFreeSpace() < a_Count)
	{
		return false;
	}
	int TillEnd = m_BufferSize - m_WritePos;
	if (TillEnd <= a_Count)
	{
		// Need to wrap around the ringbuffer end
		if (TillEnd > 0)
		{
			memcpy(m_Buffer + m_WritePos, a_Bytes, TillEnd);
			a_Bytes += TillEnd;
			a_Count -= TillEnd;
			WrittenBytes = TillEnd;
		}
		m_WritePos = 0;
	}
	
	// We're guaranteed that we'll fit in a single write op
	if (a_Count > 0)
	{
		memcpy(m_Buffer + m_WritePos, a_Bytes, a_Count);
		m_WritePos += a_Count;
		WrittenBytes += a_Count;
	}
	
	ASSERT(GetFreeSpace() == CurFreeSpace - WrittenBytes);
	ASSERT(GetReadableSpace() == CurReadableSpace + WrittenBytes);
	return true;
}





int cByteBuffer::GetFreeSpace(void) const
{
	CHECK_THREAD;
	CheckValid();
	if (m_WritePos >= m_DataStart)
	{
		// Wrap around the buffer end:
		return m_BufferSize - m_WritePos + m_DataStart - 1;
	}
	// Single free space partition:
	return m_DataStart - m_WritePos - 1;
}





/// Returns the number of bytes that are currently in the ringbuffer. Note GetReadableBytes()
int cByteBuffer::GetUsedSpace(void) const
{
	CHECK_THREAD;
	CheckValid();
	return m_BufferSize - GetFreeSpace();
}





/// Returns the number of bytes that are currently available for reading (may be less than UsedSpace due to some data having been read already)
int cByteBuffer::GetReadableSpace(void) const
{
	CHECK_THREAD;
	CheckValid();
	if (m_ReadPos > m_WritePos)
	{
		// Wrap around the buffer end:
		return m_BufferSize - m_ReadPos + m_WritePos;
	}
	// Single readable space partition:
	return m_WritePos - m_ReadPos ;
}





bool cByteBuffer::CanReadBytes(int a_Count) const
{
	CHECK_THREAD;
	CheckValid();
	return (a_Count <= GetReadableSpace());
}





bool cByteBuffer::CanWriteBytes(int a_Count) const
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
	a_Value = ntohs(a_Value);
	return true;
}





bool cByteBuffer::ReadBEInt(int & a_Value)
{
	CHECK_THREAD;
	CheckValid();
	NEEDBYTES(4);
	ReadBuf(&a_Value, 4);
	a_Value = ntohl(a_Value);
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
	return ReadUTF16String(a_Value, Length);
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
		Value = Value | (((Int64)(b & 0x7f)) << Shift);
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
		LOGWARNING("%s: String too large: %llu (%llu KiB)", __FUNCTION__, Size, Size / 1024);
	}
	return ReadString(a_Value, (int)Size);
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
	short Converted = htons(a_Value);
	return WriteBuf(&Converted, 2);
}





bool cByteBuffer::WriteBEInt(int a_Value)
{
	CHECK_THREAD;
	CheckValid();
	PUTBYTES(4);
	int Converted = HostToNetwork4(&a_Value);
	return WriteBuf(&Converted, 4);
}





bool cByteBuffer::WriteBEInt64(Int64 a_Value)
{
	CHECK_THREAD;
	CheckValid();
	PUTBYTES(8);
	Int64 Converted = HostToNetwork8(&a_Value);
	return WriteBuf(&Converted, 8);
}





bool cByteBuffer::WriteBEFloat(float a_Value)
{
	CHECK_THREAD;
	CheckValid();
	PUTBYTES(4);
	int Converted = HostToNetwork4(&a_Value);
	return WriteBuf(&Converted, 4);
}





bool cByteBuffer::WriteBEDouble(double a_Value)
{
	CHECK_THREAD;
	CheckValid();
	PUTBYTES(8);
	Int64 Converted = HostToNetwork8(&a_Value);
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
	int idx = 0;
	do
	{
		b[idx] = (a_Value & 0x7f) | ((a_Value > 0x7f) ? 0x80 : 0x00);
		a_Value = a_Value >> 7;
		idx++;
	} while (a_Value > 0);

	return WriteBuf(b, idx);
}




bool cByteBuffer::WriteVarUTF8String(AString & a_Value)
{
	CHECK_THREAD;
	CheckValid();
	PUTBYTES(a_Value.size() + 1);  // This is a lower-bound on the bytes that will be actually written. Fail early.
	bool res = WriteVarInt(a_Value.size());
	if (!res)
	{
		return false;
	}
	return WriteBuf(a_Value.data(), a_Value.size());
}





bool cByteBuffer::ReadBuf(void * a_Buffer, int a_Count)
{
	CHECK_THREAD;
	CheckValid();
	ASSERT(a_Count >= 0);
	NEEDBYTES(a_Count);
	char * Dst = (char *)a_Buffer;  // So that we can do byte math
	int BytesToEndOfBuffer = m_BufferSize - m_ReadPos;
	ASSERT(BytesToEndOfBuffer >= 0);  // Sanity check
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





bool cByteBuffer::WriteBuf(const void * a_Buffer, int a_Count)
{
	CHECK_THREAD;
	CheckValid();
	ASSERT(a_Count >= 0);
	PUTBYTES(a_Count);
	char * Src = (char *)a_Buffer;  // So that we can do byte math
	int BytesToEndOfBuffer = m_BufferSize - m_WritePos;
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





bool cByteBuffer::ReadString(AString & a_String, int a_Count)
{
	CHECK_THREAD;
	CheckValid();
	ASSERT(a_Count >= 0);
	NEEDBYTES(a_Count);
	a_String.clear();
	a_String.reserve(a_Count);
	int BytesToEndOfBuffer = m_BufferSize - m_ReadPos;
	ASSERT(BytesToEndOfBuffer >= 0);  // Sanity check
	if (BytesToEndOfBuffer <= a_Count)
	{
		// Reading across the ringbuffer end, read the first part and adjust parameters:
		if (BytesToEndOfBuffer > 0)
		{
			a_String.assign(m_Buffer + m_ReadPos, BytesToEndOfBuffer);
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





bool cByteBuffer::ReadUTF16String(AString & a_String, int a_NumChars)
{
	// Reads 2 * a_NumChars bytes and interprets it as a UTF16 string, converting it into UTF8 string a_String
	CHECK_THREAD;
	CheckValid();
	ASSERT(a_NumChars >= 0);
	AString RawData;
	if (!ReadString(RawData, a_NumChars * 2))
	{
		return false;
	}
	RawBEToUTF8((short *)(RawData.data()), a_NumChars, a_String);
	return true;
}





bool cByteBuffer::SkipRead(int a_Count)
{
	CHECK_THREAD;
	CheckValid();
	ASSERT(a_Count >= 0);
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
	int DataStart = m_DataStart;
	if (m_ReadPos < m_DataStart)
	{
		// Across the ringbuffer end, read the first part and adjust next part's start:
		a_Out.append(m_Buffer + m_DataStart, m_BufferSize - m_DataStart);
		DataStart = 0;
	}
	a_Out.append(m_Buffer + DataStart, m_ReadPos - DataStart);
}





void cByteBuffer::AdvanceReadPos(int a_Count)
{
	CHECK_THREAD;
	CheckValid();
	m_ReadPos += a_Count;
	if (m_ReadPos > m_BufferSize)
	{
		m_ReadPos -= m_BufferSize;
	}
}





void cByteBuffer::CheckValid(void) const
{
	ASSERT(m_ReadPos >= 0);
	ASSERT(m_ReadPos < m_BufferSize);
	ASSERT(m_WritePos >= 0);
	ASSERT(m_WritePos < m_BufferSize);
}




