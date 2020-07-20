
// ByteStream.h

// Interfaces to the cByteBuffer class representing a ringbuffer of bytes





#pragma once



// fwd:
class cUUID;


/** An object that can store incoming bytes and lets its clients read the bytes sequentially
The bytes are stored in a ringbuffer of constant size; if more than that size
is requested, the write operation fails.
The bytes stored can be retrieved using various ReadXXX functions; these assume that the needed
number of bytes are present in the buffer (ASSERT; for performance reasons).
The reading doesn't actually remove the bytes, it only moves the internal read ptr.
To remove the bytes, call CommitRead().
To re-start reading from the beginning, call ResetRead().
This class doesn't implement thread safety, the clients of this class need to provide
their own synchronization.
*/
class cByteBuffer
{
public:
	cByteBuffer(size_t a_BufferSize);
	~cByteBuffer();

	/** Writes the bytes specified to the ringbuffer. Returns true if successful, false if not */
	bool Write(const void * a_Bytes, size_t a_Count);

	/** Returns the number of bytes that can be successfully written to the ringbuffer */
	size_t GetFreeSpace(void) const;

	/** Returns the number of bytes that are currently in the ringbuffer. Note GetReadableBytes() */
	size_t GetUsedSpace(void) const;

	/** Returns the number of bytes that are currently available for reading (may be less than UsedSpace due to some data having been read already) */
	size_t GetReadableSpace(void) const;

	/** Returns the current data start index. For debugging purposes. */
	size_t  GetDataStart(void) const { return m_DataStart; }

	/** Returns true if the specified amount of bytes are available for reading */
	bool CanReadBytes(size_t a_Count) const;

	/** Returns true if the specified amount of bytes are available for writing */
	bool CanWriteBytes(size_t a_Count) const;

	// Read the specified datatype and advance the read pointer; return true if successfully read:
	bool ReadBEInt8         (Int8 & a_Value);
	bool ReadBEInt16        (Int16 & a_Value);
	bool ReadBEInt32        (Int32 & a_Value);
	bool ReadBEInt64        (Int64 & a_Value);
	bool ReadBEUInt8        (UInt8 & a_Value);
	bool ReadBEUInt16       (UInt16 & a_Value);
	bool ReadBEUInt32       (UInt32 & a_Value);
	bool ReadBEUInt64       (UInt64 & a_Value);
	bool ReadBEFloat        (float & a_Value);
	bool ReadBEDouble       (double & a_Value);
	bool ReadBool           (bool & a_Value);
	bool ReadVarInt32       (UInt32 & a_Value);
	bool ReadVarInt64       (UInt64 & a_Value);
	bool ReadVarUTF8String  (AString & a_Value);  // string length as VarInt, then string as UTF-8
	bool ReadLEInt          (int & a_Value);
	bool ReadXYZPosition64  (int & a_BlockX, int & a_BlockY, int & a_BlockZ);
	bool ReadXZYPosition64  (int & a_BlockX, int & a_BlockY, int & a_BlockZ);
	bool ReadUUID           (cUUID & a_Value);

	/** Reads VarInt, assigns it to anything that can be assigned from an UInt64 (unsigned short, char, Byte, double, ...) */
	template <typename T> bool ReadVarInt(T & a_Value)
	{
		UInt64 v;
		bool res = ReadVarInt64(v);
		if (res)
		{
			a_Value = static_cast<T>(v);
		}
		return res;
	}

	// Write the specified datatype; return true if successfully written
	bool WriteBEInt8         (Int8   a_Value);
	bool WriteBEInt16        (Int16  a_Value);
	bool WriteBEInt32        (Int32  a_Value);
	bool WriteBEInt64        (Int64  a_Value);
	bool WriteBEUInt8        (UInt8  a_Value);
	bool WriteBEUInt16       (UInt16 a_Value);
	bool WriteBEUInt32       (UInt32 a_Value);
	bool WriteBEUInt64       (UInt64 a_Value);
	bool WriteBEFloat        (float  a_Value);
	bool WriteBEDouble       (double a_Value);
	bool WriteBool           (bool   a_Value);
	bool WriteVarInt32       (UInt32 a_Value);
	bool WriteVarInt64       (UInt64 a_Value);
	bool WriteVarUTF8String  (const AString & a_Value);  // string length as VarInt, then string as UTF-8
	bool WriteLEInt32        (Int32 a_Value);
	bool WriteXYZPosition64  (Int32 a_BlockX, Int32 a_BlockY, Int32 a_BlockZ);
	bool WriteXZYPosition64  (Int32 a_BlockX, Int32 a_BlockY, Int32 a_BlockZ);

	/** Reads a_Count bytes into a_Buffer; returns true if successful */
	bool ReadBuf(void * a_Buffer, size_t a_Count);

	/** Writes a_Count bytes into a_Buffer; returns true if successful */
	bool WriteBuf(const void * a_Buffer, size_t a_Count);

	/** Reads a_Count bytes into a_String; returns true if successful */
	bool ReadString(AString & a_String, size_t a_Count);

	/** Skips reading by a_Count bytes; returns false if not enough bytes in the ringbuffer */
	bool SkipRead(size_t a_Count);

	/** Reads all available data into a_Data */
	void ReadAll(AString & a_Data);

	/** Reads the specified number of bytes and writes it into the destinatio bytebuffer. Returns true on success. */
	bool ReadToByteBuffer(cByteBuffer & a_Dst, size_t a_NumBytes);

	/** Removes the bytes that have been read from the ringbuffer */
	void CommitRead(void);

	/** Restarts next reading operation at the start of the ringbuffer */
	void ResetRead(void);

	/** Re-reads the data that has been read since the last commit to the current readpos. Used by ProtoProxy to duplicate communication */
	void ReadAgain(AString & a_Out);

	/** Checks if the internal state is valid (read and write positions in the correct bounds) using ASSERTs */
	void CheckValid(void) const;

	/** Gets the number of bytes that are needed to represent the given VarInt */
	static size_t GetVarIntSize(UInt32 a_Value);

protected:
	char * m_Buffer;
	size_t m_BufferSize;  // Total size of the ringbuffer

	size_t m_DataStart;  // Where the data starts in the ringbuffer
	size_t m_WritePos;   // Where the data ends in the ringbuffer
	size_t m_ReadPos;    // Where the next read will start in the ringbuffer

	#ifdef _DEBUG
		/** The ID of the thread currently accessing the object.
		Used for checking that only one thread accesses the object at a time, via cSingleThreadAccessChecker. */
		mutable std::thread::id m_ThreadID;
	#endif

	/** Advances the m_ReadPos by a_Count bytes */
	void AdvanceReadPos(size_t a_Count);
} ;




