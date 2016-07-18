
// ByteStream.h

// Interfaces to the cByteBuffer class representing a ringbuffer of bytes





#pragma once





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
// tolua_begin
class cByteBuffer
{
// tolua_end
public:
	cByteBuffer(size_t a_BufferSize);
	cByteBuffer(const cByteBuffer & a_ByteBuffer);
	~cByteBuffer();

	/** Writes the bytes specified to the ringbuffer. Returns true if successful, false if not */
	bool Write(const void * a_Bytes, size_t a_Count);

	/** Returns the number of bytes that can be successfully written to the ringbuffer */
	size_t GetFreeSpace(void) const;  // tolua_export

	/** Returns the number of bytes that are currently in the ringbuffer. Note GetReadableBytes() */
	size_t GetUsedSpace(void) const;  // tolua_export

	/** Returns the number of bytes that are currently available for reading (may be less than UsedSpace due to some data having been read already) */
	size_t GetReadableSpace(void) const;  // tolua_export

	/** Returns the current data start index. For debugging purposes. */
	size_t  GetDataStart(void) const { return m_DataStart; }

	/** Returns true if the specified amount of bytes are available for reading */
	bool CanReadBytes(size_t a_Count) const;  // tolua_export

	/** Returns true if the specified amount of bytes are available for writing */
	bool CanWriteBytes(size_t a_Count) const;  // tolua_export

	// Read the specified datatype and advance the read pointer; return true if successfully read:
	bool ReadBEInt8         (Int8 & a_Value);  // exported in manual bindings
	bool ReadBEInt16        (Int16 & a_Value);  // exported in manual bindings
	bool ReadBEInt32        (Int32 & a_Value);  // exported in manual bindings
	bool ReadBEInt64        (Int64 & a_Value);  // exported in manual bindings
	bool ReadBEUInt8        (UInt8 & a_Value);  // exported in manual bindings
	bool ReadBEUInt16       (UInt16 & a_Value);  // exported in manual bindings
	bool ReadBEUInt32       (UInt32 & a_Value);  // exported in manual bindings
	bool ReadBEUInt64       (UInt64 & a_Value);  // exported in manual bindings
	bool ReadBEFloat        (float & a_Value);  // exported in manual bindings
	bool ReadBEDouble       (double & a_Value);  // exported in manual bindings
	bool ReadBool           (bool & a_Value);  // exported in manual bindings
	bool ReadVarInt32       (UInt32 & a_Value);  // exported in manual bindings
	bool ReadVarInt64       (UInt64 & a_Value);  // exported in manual bindings
	// string length as VarInt, then string as UTF-8.
	bool ReadVarUTF8String  (AString & a_Value);  // exported in manual bindings
	bool ReadLEInt          (int & a_Value);  // exported in manual bindings
	bool ReadPosition64     (int & a_BlockX, int & a_BlockY, int & a_BlockZ);  // exported in manual bindings

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
	bool WriteBEInt8         (Int8   a_Value);  // tolua_export
	bool WriteBEInt16        (Int16  a_Value);  // tolua_export
	bool WriteBEInt32        (Int32  a_Value);  // tolua_export
	bool WriteBEInt64        (Int64  a_Value);  // tolua_export
	bool WriteBEUInt8        (UInt8  a_Value);  // tolua_export
	bool WriteBEUInt16       (UInt16 a_Value);  // tolua_export
	bool WriteBEUInt32       (UInt32 a_Value);  // tolua_export
	bool WriteBEUInt64       (UInt64 a_Value);  // tolua_export
	bool WriteBEFloat        (float  a_Value);  // tolua_export
	bool WriteBEDouble       (double a_Value);  // tolua_export
	bool WriteBool           (bool   a_Value);  // tolua_export
	bool WriteVarInt32       (UInt32 a_Value);  // tolua_export
	bool WriteVarInt64       (UInt64 a_Value);  // tolua_export
	// string length as VarInt, then string as UTF-8
	bool WriteVarUTF8String  (const AString & a_Value);  // tolua_export
	bool WriteLEInt32        (Int32 a_Value);  // tolua_export
	bool WritePosition64     (Int32 a_BlockX, Int32 a_BlockY, Int32 a_BlockZ);  // tolua_export

	/** Reads a_Count bytes into a_Buffer; returns true if successful */
	bool ReadBuf(void * a_Buffer, size_t a_Count);

	/** Writes a_Count bytes into a_Buffer; returns true if successful */
	bool WriteBuf(const void * a_Buffer, size_t a_Count);

	/** Reads a_Count bytes into a_String; returns true if successful */
	bool ReadString(AString & a_String, size_t a_Count);  // exported im manual bindings

	/** Skips reading by a_Count bytes; returns false if not enough bytes in the ringbuffer */
	bool SkipRead(size_t a_Count);  // tolua_export

	/** Reads all available data into a_Data */
	void ReadAll(AString & a_Data);  // exported in manual bindings

	/** Reads the specified number of bytes and writes it into the destinatio bytebuffer. Returns true on success. */
	bool ReadToByteBuffer(cByteBuffer & a_Dst, size_t a_NumBytes);

	/** Removes the bytes that have been read from the ringbuffer */
	void CommitRead(void);  // tolua_export

	/** Restarts next reading operation at the start of the ringbuffer */
	void ResetRead(void);  // tolua_export

	/** Re-reads the data that has been read since the last commit to the current readpos. Used by ProtoProxy to duplicate communication */
	void ReadAgain(AString & a_Out);  // exported in manual bindings

	/** Checks if the internal state is valid (read and write positions in the correct bounds) using ASSERTs */
	void CheckValid(void) const;  // tolua_export

	/** Gets the number of bytes that are needed to represent the given VarInt */
	static size_t GetVarIntSize(UInt32 a_Value);  // tolua_export

	static cByteBuffer * Create(size_t a_BufferSize);  // tolua_export

	static void Destroy(cByteBuffer * a_ByteBuffer);  // tolua_export

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
};  // tolua_export




