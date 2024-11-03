
// Packetizer.h

// Declares the cPacketizer class representing a wrapper for sending a single packet over a protocol.
// The class provides auto-locking, serialization and send-on-instance-destroy semantics





#pragma once

#include "Protocol.h"



class cByteBuffer;


// fwd:
class cUUID;





/** Macros used to read packets more easily */
#define HANDLE_READ(ByteBuf, Proc, Type, Var) \
	Type Var; \
	do { \
		if (!ByteBuf.Proc(Var))\
		{\
			return;\
		} \
	} while (false)





#define HANDLE_PACKET_READ(ByteBuf, Proc, Type, Var) \
	Type Var; \
	do { \
		{ \
			if (!ByteBuf.Proc(Var)) \
			{ \
				ByteBuf.CheckValid(); \
				return false; \
			} \
			ByteBuf.CheckValid(); \
		} \
	} while (false)





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
		auto id = m_Protocol.GetPacketID(a_PacketType);
		// LOG("%s - 0x%.2X", PacketTypeToStr(a_PacketType), id);
		m_Out.WriteVarInt32(id);
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


	inline void WriteVarInt64(UInt64 a_Value)
	{
		VERIFY(m_Out.WriteVarInt64(a_Value));
	}


	inline void WriteString(const AString & a_Value)
	{
		VERIFY(m_Out.WriteVarUTF8String(a_Value));
	}


	inline void WriteBuf(const ContiguousByteBufferView a_Data)
	{
		VERIFY(m_Out.Write(a_Data.data(), a_Data.size()));
	}

	// TODO: implement a variant with max length
	inline void WriteLengthPrefixedBuf(const ContiguousByteBufferView a_Data)
	{
		VERIFY(m_Out.WriteVarInt32(static_cast<UInt32>(a_Data.size())));
		VERIFY(m_Out.Write(a_Data.data(), a_Data.size()));
	}

	/** Writes the specified block position as a single encoded 64-bit BigEndian integer.
	The three coordinates are written in XYZ order. */
	inline void WriteXYZPosition64(int a_BlockX, int a_BlockY, int a_BlockZ)
	{
		VERIFY(m_Out.WriteXYZPosition64(a_BlockX, a_BlockY, a_BlockZ));
	}

	/** Writes the specified block position as a single encoded 64-bit BigEndian integer.
	The three coordinates are written in XYZ order. */
	inline void WriteXYZPosition64(const Vector3i a_Position)
	{
		VERIFY(m_Out.WriteXYZPosition64(a_Position.x, a_Position.y, a_Position.z));
	}

	/** Writes the specified block position as a single encoded 64-bit BigEndian integer.
	The three coordinates are written in XZY order, in 1.14+. */
	inline void WriteXZYPosition64(int a_BlockX, int a_BlockY, int a_BlockZ)
	{
		VERIFY(m_Out.WriteXZYPosition64(a_BlockX, a_BlockY, a_BlockZ));
	}

	/** Writes the specified block position as a single encoded 64-bit BigEndian integer.
	The three coordinates are written in XZY order, in 1.14+. */
	inline void WriteXZYPosition64(const Vector3i a_Position)
	{
		VERIFY(m_Out.WriteXZYPosition64(a_Position.x, a_Position.y, a_Position.z));
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




