
// ByteBuffer.cpp

// Implements the cByteBuffer class representing a ringbuffer of bytes

#include "Globals.h"

#include "ByteBuffer.h"
#include "Endianness.h"
#include "UUID.h"
#include "OSSupport/IsThread.h"





/** When defined, each access to a cByteBuffer object is checked whether it's done in the same thread.
cByteBuffer assumes that it is not used by multiple threads at once, this macro adds a runtime check for that.
Unfortunately it is very slow, so it is disabled even for regular DEBUG builds. */
// #define DEBUG_SINGLE_THREAD_ACCESS





// If a string sent over the protocol is larger than this, a warning is emitted to the console
#define MAX_STRING_SIZE (512 KiB)

#define NEEDBYTES(Num) if (!CanReadBytes(Num))  return false  // Check if at least Num bytes can be read from  the buffer, return false if not
#define PUTBYTES(Num)  if (!CanWriteBytes(Num)) return false  // Check if at least Num bytes can be written to the buffer, return false if not





#ifdef DEBUG_SINGLE_THREAD_ACCESS

	/** Simple RAII class that is used for checking that no two threads are using an object simultanously.
	It requires the monitored object to provide the storage for a thread ID.
	It uses that storage to check if the thread ID of consecutive calls is the same all the time. */
	class cSingleThreadAccessChecker
	{
	public:
		cSingleThreadAccessChecker(std::thread::id * a_ThreadID) :
			m_ThreadID(a_ThreadID)
		{
			ASSERT(
				(*a_ThreadID == std::this_thread::get_id()) ||  // Either the object is used by current thread...
				(*a_ThreadID == m_EmptyThreadID)                // ... or by no thread at all
			);

			// Mark as being used by this thread:
			*m_ThreadID = std::this_thread::get_id();
		}

		~cSingleThreadAccessChecker()
		{
			// Mark as not being used by any thread:
			*m_ThreadID = std::thread::id();
		}

	protected:
		/** Points to the storage used for ID of the thread using the object. */
		std::thread::id * m_ThreadID;

		/** The value of an unassigned thread ID, used to speed up checking. */
		static std::thread::id m_EmptyThreadID;
	};

	std::thread::id cSingleThreadAccessChecker::m_EmptyThreadID;

	#define CHECK_THREAD cSingleThreadAccessChecker Checker(&m_ThreadID);

#else
	#define CHECK_THREAD
#endif





////////////////////////////////////////////////////////////////////////////////
// cByteBuffer:

cByteBuffer::cByteBuffer(size_t a_BufferSize) :
	m_Buffer(new char[a_BufferSize + 1]),
	m_BufferSize(a_BufferSize + 1),
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
	m_Buffer = nullptr;
}





bool cByteBuffer::Write(const void * a_Bytes, size_t a_Count)
{
	CHECK_THREAD
	CheckValid();

	// Store the current free space for a check after writing:
	size_t CurFreeSpace = GetFreeSpace();
	#ifdef _DEBUG
		size_t CurReadableSpace = GetReadableSpace();
	#endif
	size_t WrittenBytes = 0;

	if (CurFreeSpace < a_Count)
	{
		return false;
	}
	ASSERT(m_BufferSize >= m_WritePos);
	size_t TillEnd = m_BufferSize - m_WritePos;
	const char * Bytes = static_cast<const char *>(a_Bytes);
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
	CHECK_THREAD
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





size_t cByteBuffer::GetUsedSpace(void) const
{
	CHECK_THREAD
	CheckValid();
	ASSERT(m_BufferSize >= GetFreeSpace());
	ASSERT((m_BufferSize - GetFreeSpace()) >= 1);
	return m_BufferSize - GetFreeSpace() - 1;
}





size_t cByteBuffer::GetReadableSpace(void) const
{
	CHECK_THREAD
	CheckValid();
	if (m_ReadPos > m_WritePos)
	{
		// Wrap around the buffer end:
		ASSERT(m_BufferSize >= m_ReadPos);
		return m_BufferSize - m_ReadPos + m_WritePos;
	}
	// Single readable space partition:
	ASSERT(m_WritePos >= m_ReadPos);
	return m_WritePos - m_ReadPos;
}





bool cByteBuffer::CanReadBytes(size_t a_Count) const
{
	CHECK_THREAD
	CheckValid();
	return (a_Count <= GetReadableSpace());
}





bool cByteBuffer::CanWriteBytes(size_t a_Count) const
{
	CHECK_THREAD
	CheckValid();
	return (a_Count <= GetFreeSpace());
}





bool cByteBuffer::ReadBEInt8(Int8 & a_Value)
{
	CHECK_THREAD
	CheckValid();
	NEEDBYTES(1);
	ReadBuf(&a_Value, 1);
	return true;
}





bool cByteBuffer::ReadBEUInt8(UInt8 & a_Value)
{
	CHECK_THREAD
	CheckValid();
	NEEDBYTES(1);
	ReadBuf(&a_Value, 1);
	return true;
}





bool cByteBuffer::ReadBEInt16(Int16 & a_Value)
{
	CHECK_THREAD
	CheckValid();
	NEEDBYTES(2);
	UInt16 val;
	ReadBuf(&val, 2);
	val = ntohs(val);
	memcpy(&a_Value, &val, 2);
	return true;
}





bool cByteBuffer::ReadBEUInt16(UInt16 & a_Value)
{
	CHECK_THREAD
	CheckValid();
	NEEDBYTES(2);
	ReadBuf(&a_Value, 2);
	a_Value = ntohs(a_Value);
	return true;
}





bool cByteBuffer::ReadBEInt32(Int32 & a_Value)
{
	CHECK_THREAD
	CheckValid();
	NEEDBYTES(4);
	UInt32 val;
	ReadBuf(&val, 4);
	val = ntohl(val);
	memcpy(&a_Value, &val, 4);
	return true;
}





bool cByteBuffer::ReadBEUInt32(UInt32 & a_Value)
{
	CHECK_THREAD
	CheckValid();
	NEEDBYTES(4);
	ReadBuf(&a_Value, 4);
	a_Value = ntohl(a_Value);
	return true;
}





bool cByteBuffer::ReadBEInt64(Int64 & a_Value)
{
	CHECK_THREAD
	CheckValid();
	NEEDBYTES(8);
	ReadBuf(&a_Value, 8);
	a_Value = NetworkToHostLong8(&a_Value);
	return true;
}





bool cByteBuffer::ReadBEUInt64(UInt64 & a_Value)
{
	CHECK_THREAD
	CheckValid();
	NEEDBYTES(8);
	ReadBuf(&a_Value, 8);
	a_Value = NetworkToHostULong8(&a_Value);
	return true;
}





bool cByteBuffer::ReadBEFloat(float & a_Value)
{
	CHECK_THREAD
	CheckValid();
	NEEDBYTES(4);
	ReadBuf(&a_Value, 4);
	a_Value = NetworkToHostFloat4(&a_Value);
	return true;
}





bool cByteBuffer::ReadBEDouble(double & a_Value)
{
	CHECK_THREAD
	CheckValid();
	NEEDBYTES(8);
	ReadBuf(&a_Value, 8);
	a_Value = NetworkToHostDouble8(&a_Value);
	return true;
}





bool cByteBuffer::ReadBool(bool & a_Value)
{
	CHECK_THREAD
	CheckValid();
	NEEDBYTES(1);
	UInt8 Value = 0;
	ReadBuf(&Value, 1);
	a_Value = (Value != 0);
	return true;
}





bool cByteBuffer::ReadVarInt32(UInt32 & a_Value)
{
	CHECK_THREAD
	CheckValid();
	UInt32 Value = 0;
	int Shift = 0;
	unsigned char b = 0;
	do
	{
		NEEDBYTES(1);
		ReadBuf(&b, 1);
		Value = Value | ((static_cast<UInt32>(b & 0x7f)) << Shift);
		Shift += 7;
	} while ((b & 0x80) != 0);
	a_Value = Value;
	return true;
}





bool cByteBuffer::ReadVarInt64(UInt64 & a_Value)
{
	CHECK_THREAD
	CheckValid();
	UInt64 Value = 0;
	int Shift = 0;
	unsigned char b = 0;
	do
	{
		NEEDBYTES(1);
		ReadBuf(&b, 1);
		Value = Value | ((static_cast<UInt64>(b & 0x7f)) << Shift);
		Shift += 7;
	} while ((b & 0x80) != 0);
	a_Value = Value;
	return true;
}





bool cByteBuffer::ReadVarUTF8String(AString & a_Value)
{
	CHECK_THREAD
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
	return ReadString(a_Value, static_cast<size_t>(Size));
}





bool cByteBuffer::ReadLEInt(int & a_Value)
{
	CHECK_THREAD
	CheckValid();
	NEEDBYTES(4);
	ReadBuf(&a_Value, 4);

	#ifdef IS_BIG_ENDIAN
		// Convert:
		a_Value = ((a_Value >> 24) & 0xff) | ((a_Value >> 16) & 0xff00) | ((a_Value >> 8) & 0xff0000) | (a_Value & 0xff000000);
	#endif

	return true;
}





bool cByteBuffer::ReadXYZPosition64(int & a_BlockX, int & a_BlockY, int & a_BlockZ)
{
	CHECK_THREAD
	Int64 Value;
	if (!ReadBEInt64(Value))
	{
		return false;
	}

	// Convert the 64 received bits into 3 coords:
	UInt32 BlockXRaw = (Value >> 38) & 0x03ffffff;  // Top 26 bits
	UInt32 BlockYRaw = (Value >> 26) & 0x0fff;      // Middle 12 bits
	UInt32 BlockZRaw = (Value & 0x03ffffff);        // Bottom 26 bits

	// If the highest bit in the number's range is set, convert the number into negative:
	a_BlockX = ((BlockXRaw & 0x02000000) == 0) ? static_cast<int>(BlockXRaw) : -(0x04000000 - static_cast<int>(BlockXRaw));
	a_BlockY = ((BlockYRaw & 0x0800) == 0)     ? static_cast<int>(BlockYRaw) : -(0x01000    - static_cast<int>(BlockYRaw));
	a_BlockZ = ((BlockZRaw & 0x02000000) == 0) ? static_cast<int>(BlockZRaw) : -(0x04000000 - static_cast<int>(BlockZRaw));
	return true;
}





bool cByteBuffer::ReadXZYPosition64(int & a_BlockX, int & a_BlockY, int & a_BlockZ)
{
	CHECK_THREAD
	Int64 Value;
	if (!ReadBEInt64(Value))
	{
		return false;
	}

	// Convert the 64 received bits into 3 coords:
	UInt32 BlockXRaw = (Value >> 38) & 0x03ffffff;  // Top 26 bits
	UInt32 BlockZRaw = (Value >> 12) & 0x03ffffff;  // Middle 26 bits
	UInt32 BlockYRaw = (Value & 0x0fff);            // Bottom 12 bits

	// If the highest bit in the number's range is set, convert the number into negative:
	a_BlockX = ((BlockXRaw & 0x02000000) == 0) ? static_cast<int>(BlockXRaw) : (static_cast<int>(BlockXRaw) - 0x04000000);
	a_BlockY = ((BlockYRaw & 0x0800)     == 0) ? static_cast<int>(BlockYRaw) : (static_cast<int>(BlockYRaw) - 0x01000);
	a_BlockZ = ((BlockZRaw & 0x02000000) == 0) ? static_cast<int>(BlockZRaw) : (static_cast<int>(BlockZRaw) - 0x04000000);
	return true;
}





bool cByteBuffer::ReadUUID(cUUID & a_Value)
{
	CHECK_THREAD

	std::array<Byte, 16> UUIDBuf;
	if (!ReadBuf(UUIDBuf.data(), UUIDBuf.size()))
	{
		return false;
	}

	a_Value.FromRaw(UUIDBuf);
	return true;
}





bool cByteBuffer::WriteBEInt8(Int8 a_Value)
{
	CHECK_THREAD
	CheckValid();
	PUTBYTES(1);
	return WriteBuf(&a_Value, 1);
}





bool cByteBuffer::WriteBEUInt8(UInt8 a_Value)
{
	CHECK_THREAD
	CheckValid();
	PUTBYTES(1);
	return WriteBuf(&a_Value, 1);
}





bool cByteBuffer::WriteBEInt16(Int16 a_Value)
{
	CHECK_THREAD
	CheckValid();
	PUTBYTES(2);
	UInt16 val;
	memcpy(&val, &a_Value, 2);
	val = htons(val);
	return WriteBuf(&val, 2);
}





bool cByteBuffer::WriteBEUInt16(UInt16 a_Value)
{
	CHECK_THREAD
	CheckValid();
	PUTBYTES(2);
	a_Value = htons(a_Value);
	return WriteBuf(&a_Value, 2);
}





bool cByteBuffer::WriteBEInt32(Int32 a_Value)
{
	CHECK_THREAD
	CheckValid();
	PUTBYTES(4);
	UInt32 Converted = HostToNetwork4(&a_Value);
	return WriteBuf(&Converted, 4);
}





bool cByteBuffer::WriteBEUInt32(UInt32 a_Value)
{
	CHECK_THREAD
	CheckValid();
	PUTBYTES(4);
	UInt32 Converted = HostToNetwork4(&a_Value);
	return WriteBuf(&Converted, 4);
}





bool cByteBuffer::WriteBEInt64(Int64 a_Value)
{
	CHECK_THREAD
	CheckValid();
	PUTBYTES(8);
	UInt64 Converted = HostToNetwork8(&a_Value);
	return WriteBuf(&Converted, 8);
}





bool cByteBuffer::WriteBEUInt64(UInt64 a_Value)
{
	CHECK_THREAD
	CheckValid();
	PUTBYTES(8);
	UInt64 Converted = HostToNetwork8(&a_Value);
	return WriteBuf(&Converted, 8);
}





bool cByteBuffer::WriteBEFloat(float a_Value)
{
	CHECK_THREAD
	CheckValid();
	PUTBYTES(4);
	UInt32 Converted = HostToNetwork4(&a_Value);
	return WriteBuf(&Converted, 4);
}





bool cByteBuffer::WriteBEDouble(double a_Value)
{
	CHECK_THREAD
	CheckValid();
	PUTBYTES(8);
	UInt64 Converted = HostToNetwork8(&a_Value);
	return WriteBuf(&Converted, 8);
}





bool cByteBuffer::WriteBool(bool a_Value)
{
	CHECK_THREAD
	CheckValid();
	UInt8 val = a_Value ? 1 : 0;
	return Write(&val, 1);
}





bool cByteBuffer::WriteVarInt32(UInt32 a_Value)
{
	CHECK_THREAD
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





bool cByteBuffer::WriteVarInt64(UInt64 a_Value)
{
	CHECK_THREAD
	CheckValid();

	// A 64-bit integer can be encoded by at most 10 bytes:
	unsigned char b[10];
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
	CHECK_THREAD
	CheckValid();
	PUTBYTES(a_Value.size() + 1);  // This is a lower-bound on the bytes that will be actually written. Fail early.
	bool res = WriteVarInt32(static_cast<UInt32>(a_Value.size()));
	if (!res)
	{
		return false;
	}
	return WriteBuf(a_Value.data(), a_Value.size());
}





bool cByteBuffer::WriteXYZPosition64(Int32 a_BlockX, Int32 a_BlockY, Int32 a_BlockZ)
{
	CHECK_THREAD
	CheckValid();
	return WriteBEInt64(
		(static_cast<Int64>(a_BlockX & 0x3FFFFFF) << 38) |
		(static_cast<Int64>(a_BlockY & 0xFFF) << 26) |
		(static_cast<Int64>(a_BlockZ & 0x3FFFFFF))
	);
}





bool cByteBuffer::WriteXZYPosition64(Int32 a_BlockX, Int32 a_BlockY, Int32 a_BlockZ)
{
	CHECK_THREAD
	CheckValid();
	return WriteBEInt64(
		(static_cast<Int64>(a_BlockX & 0x3FFFFFF) << 38) |
		(static_cast<Int64>(a_BlockZ & 0x3FFFFFF) << 26) |
		(static_cast<Int64>(a_BlockY & 0xFFF))
	);
}





bool cByteBuffer::ReadBuf(void * a_Buffer, size_t a_Count)
{
	CHECK_THREAD
	CheckValid();
	NEEDBYTES(a_Count);
	char * Dst = static_cast<char *>(a_Buffer);  // So that we can do byte math
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
	CHECK_THREAD
	CheckValid();
	PUTBYTES(a_Count);
	const char * Src = static_cast<const char *>(a_Buffer);  // So that we can do byte math
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
	CHECK_THREAD
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





bool cByteBuffer::SkipRead(size_t a_Count)
{
	CHECK_THREAD
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
	CHECK_THREAD
	CheckValid();
	ReadString(a_Data, GetReadableSpace());
}





bool cByteBuffer::ReadToByteBuffer(cByteBuffer & a_Dst, size_t a_NumBytes)
{
	CHECK_THREAD
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
	CHECK_THREAD
	CheckValid();
	m_DataStart = m_ReadPos;
}





void cByteBuffer::ResetRead(void)
{
	CHECK_THREAD
	CheckValid();
	m_ReadPos = m_DataStart;
}





void cByteBuffer::ReadAgain(AString & a_Out)
{
	// Return the data between m_DataStart and m_ReadPos (the data that has been read but not committed)
	// Used by ProtoProxy to repeat communication twice, once for parsing and the other time for the remote party
	CHECK_THREAD
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
	CHECK_THREAD
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





size_t cByteBuffer::GetVarIntSize(UInt32 a_Value)
{
	size_t Count = 0;

	do
	{
		// If the value cannot be expressed in 7 bits, it needs to take up another byte
		Count++;
		a_Value >>= 7;
	} while (a_Value != 0);

	return Count;
}





