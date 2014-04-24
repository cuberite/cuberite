
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
class cByteBuffer
{
public:
	cByteBuffer(size_t a_BufferSize);
	~cByteBuffer();
	
	/// Writes the bytes specified to the ringbuffer. Returns true if successful, false if not
	bool Write(const char * a_Bytes, size_t a_Count);
	
	/// Returns the number of bytes that can be successfully written to the ringbuffer
	size_t GetFreeSpace(void) const;
	
	/// Returns the number of bytes that are currently in the ringbuffer. Note GetReadableBytes()
	size_t GetUsedSpace(void) const;
	
	/// Returns the number of bytes that are currently available for reading (may be less than UsedSpace due to some data having been read already)
	size_t GetReadableSpace(void) const;
	
	/// Returns the current data start index. For debugging purposes.
	size_t  GetDataStart(void) const { return m_DataStart; }
	
	/// Returns true if the specified amount of bytes are available for reading
	bool CanReadBytes(size_t a_Count) const;

	/// Returns true if the specified amount of bytes are available for writing
	bool CanWriteBytes(size_t a_Count) const;

	// Read the specified datatype and advance the read pointer; return true if successfully read:
	bool ReadChar           (char & a_Value);
	bool ReadByte           (unsigned char & a_Value);
	bool ReadBEShort        (short & a_Value);
	bool ReadBEInt          (int & a_Value);
	bool ReadBEInt64        (Int64 & a_Value);
	bool ReadBEFloat        (float & a_Value);
	bool ReadBEDouble       (double & a_Value);
	bool ReadBool           (bool & a_Value);
	bool ReadBEUTF16String16(AString & a_Value);  // string length as BE short, then string as UTF-16BE
	bool ReadVarInt         (UInt32 & a_Value);
	bool ReadVarUTF8String  (AString & a_Value);  // string length as VarInt, then string as UTF-8
	bool ReadLEInt          (int & a_Value);

	/// Reads VarInt, assigns it to anything that can be assigned from an UInt32 (unsigned short, char, Byte, double, ...)
	template <typename T> bool ReadVarInt(T & a_Value)
	{
		UInt32 v;
		bool res = ReadVarInt(v);
		if (res)
		{
			a_Value = v;
		}
		return res;
	}

	// Write the specified datatype; return true if successfully written
	bool WriteChar           (char a_Value);
	bool WriteByte           (unsigned char a_Value);
	bool WriteBEShort        (short  a_Value);
	bool WriteBEInt          (int    a_Value);
	bool WriteBEInt64        (Int64  a_Value);
	bool WriteBEFloat        (float  a_Value);
	bool WriteBEDouble       (double a_Value);
	bool WriteBool           (bool   a_Value);
	bool WriteBEUTF16String16(const AString & a_Value);  // string length as BE short, then string as UTF-16BE
	bool WriteVarInt         (UInt32 a_Value);
	bool WriteVarUTF8String  (const AString & a_Value);  // string length as VarInt, then string as UTF-8
	bool WriteLEInt          (int a_Value);
	
	/// Reads a_Count bytes into a_Buffer; returns true if successful
	bool ReadBuf(void * a_Buffer, size_t a_Count);
	
	/// Writes a_Count bytes into a_Buffer; returns true if successful
	bool WriteBuf(const void * a_Buffer, size_t a_Count);
	
	/// Reads a_Count bytes into a_String; returns true if successful
	bool ReadString(AString & a_String, size_t a_Count);
	
	/// Reads 2 * a_NumChars bytes and interprets it as a UTF16-BE string, converting it into UTF8 string a_String
	bool ReadUTF16String(AString & a_String, int a_NumChars);
	
	/// Skips reading by a_Count bytes; returns false if not enough bytes in the ringbuffer
	bool SkipRead(size_t a_Count);
	
	/// Reads all available data into a_Data
	void ReadAll(AString & a_Data);
	
	/// Reads the specified number of bytes and writes it into the destinatio bytebuffer. Returns true on success.
	bool ReadToByteBuffer(cByteBuffer & a_Dst, size_t a_NumBytes);
	
	/// Removes the bytes that have been read from the ringbuffer
	void CommitRead(void);
	
	/// Restarts next reading operation at the start of the ringbuffer
	void ResetRead(void);
	
	/// Re-reads the data that has been read since the last commit to the current readpos. Used by ProtoProxy to duplicate communication
	void ReadAgain(AString & a_Out);
	
	/// Checks if the internal state is valid (read and write positions in the correct bounds) using ASSERTs
	void CheckValid(void) const;

protected:
	char * m_Buffer;
	size_t m_BufferSize;  // Total size of the ringbuffer
	
	#ifdef _DEBUG
	volatile unsigned long m_ThreadID;  // Thread that is currently accessing the object, checked via cSingleThreadAccessChecker
	#endif  // _DEBUG
	
	size_t m_DataStart;  // Where the data starts in the ringbuffer
	size_t m_WritePos;   // Where the data ends in the ringbuffer
	size_t m_ReadPos;    // Where the next read will start in the ringbuffer
	
	/// Advances the m_ReadPos by a_Count bytes
	void AdvanceReadPos(size_t a_Count);
} ;




