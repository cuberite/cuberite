
// Packetizer.h

// Declares the cPacketizer class representing a wrapper for sending a single packet over a protocol.
// The class provides auto-locking, serialization and send-on-instance-destroy semantics





#pragma once

#include "Protocol.h"



class cByteBuffer;


// fwd:
class cUUID;





/** Composes an individual packet in the protocol's m_OutPacketBuffer; sends it just before being destructed. */
class cPacketizer
{
public:
	/** Starts serializing a new packet into the protocol's m_OutPacketBuffer.
	Locks the protocol's m_CSPacket to avoid multithreading issues. */
	cPacketizer(cProtocol & a_Protocol, cProtocol::ePacketType a_PacketType) :
		m_Protocol(a_Protocol),
		m_Out(a_Protocol.m_OutPacketBuffer),
		m_Lock(a_Protocol.m_CSPacket),
		m_PacketType(a_PacketType)  // Used for logging purposes
	{
		m_Out.WriteVarInt32(m_Protocol.GetPacketID(a_PacketType));
	}

	/** Sends the packet via the contained protocol's SendPacket() function. */
	~cPacketizer();

	inline void WriteBool(bool a_Value)
	{
		VERIFY(m_Out.WriteBool(a_Value));
	}

	inline void WriteBEUInt8(UInt8 a_Value)
	{
		VERIFY(m_Out.WriteBEUInt8(a_Value));
	}


	inline void WriteBEInt8(Int8 a_Value)
	{
		VERIFY(m_Out.WriteBEInt8(a_Value));
	}


	inline void WriteBEInt16(Int16 a_Value)
	{
		VERIFY(m_Out.WriteBEInt16(a_Value));
	}


	inline void WriteBEUInt16(UInt16 a_Value)
	{
		VERIFY(m_Out.WriteBEUInt16(a_Value));
	}


	inline void WriteBEInt32(Int32 a_Value)
	{
		VERIFY(m_Out.WriteBEInt32(a_Value));
	}


	inline void WriteBEUInt32(UInt32 a_Value)
	{
		VERIFY(m_Out.WriteBEUInt32(a_Value));
	}


	inline void WriteBEInt64(Int64 a_Value)
	{
		VERIFY(m_Out.WriteBEInt64(a_Value));
	}


	inline void WriteBEUInt64(UInt64 a_Value)
	{
		VERIFY(m_Out.WriteBEUInt64(a_Value));
	}


	inline void WriteBEFloat(float a_Value)
	{
		VERIFY(m_Out.WriteBEFloat(a_Value));
	}


	inline void WriteBEDouble(double a_Value)
	{
		VERIFY(m_Out.WriteBEDouble(a_Value));
	}


	inline void WriteVarInt32(UInt32 a_Value)
	{
		VERIFY(m_Out.WriteVarInt32(a_Value));
	}


	inline void WriteString(const AString & a_Value)
	{
		VERIFY(m_Out.WriteVarUTF8String(a_Value));
	}


	inline void WriteBuf(const char * a_Data, size_t a_Size)
	{
		VERIFY(m_Out.Write(a_Data, a_Size));
	}


	/** Writes the specified block position as a single encoded 64-bit BigEndian integer.
	The three coordinates are written in XYZ order. */
	inline void WriteXYZPosition64(int a_BlockX, int a_BlockY, int a_BlockZ)
	{
		VERIFY(m_Out.WriteXYZPosition64(a_BlockX, a_BlockY, a_BlockZ));
	}

	/** Writes the specified block position as a single encoded 64-bit BigEndian integer.
	The three coordinates are written in XYZ order. */
	inline void WriteXYZPosition64(const Vector3i & a_Pos)
	{
		VERIFY(m_Out.WriteXYZPosition64(a_Pos.x, a_Pos.y, a_Pos.z));
	}

	/** Writes the specified block position as a single encoded 64-bit BigEndian integer.
	The three coordinates are written in XZY order, in 1.14+. */
	inline void WriteXZYPosition64(int a_BlockX, int a_BlockY, int a_BlockZ)
	{
		VERIFY(m_Out.WriteXZYPosition64(a_BlockX, a_BlockY, a_BlockZ));
	}

	/** Writes the specified angle using a single byte. */
	void WriteByteAngle(double a_Angle);

	/** Writes the double value as a 27:5 fixed-point integer. */
	void WriteFPInt(double a_Value);

	/** Writes the specified UUID as a 128-bit BigEndian integer. */
	void WriteUUID(const cUUID & a_UUID);

	cProtocol::ePacketType GetPacketType() const { return m_PacketType; }

	/** Returns the human-readable representation of the packet type.
	Used for logging the packets. */
	static AString PacketTypeToStr(cProtocol::ePacketType a_PacketType);

protected:
	/** The protocol instance in which the packet is being constructed. */
	cProtocol & m_Protocol;

	/** The protocol's buffer for the constructed packet data. */
	cByteBuffer & m_Out;

	/** The RAII lock preventing multithreaded access to the protocol buffer while constructing the packet. */
	cCSLock m_Lock;

	/** Type of the contained packet.
	Used for logging purposes, the packet type is encoded into m_Out immediately in constructor. */
	cProtocol::ePacketType m_PacketType;
} ;




