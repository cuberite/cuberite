
// cMetadataWriter.h

// Handles entity metadata writing





#pragma once

#include "ByteBuffer.h"
#include "Defines.h"
#include "Item.h"
#include "Protocol/ProtocolRecognizer.h"

class cMetadataWriter
{
public:
	cMetadataWriter(int a_ProtocolVersion) :
		m_ProtocolVersion(a_ProtocolVersion)
	{
	}

	virtual ~cMetadataWriter() {}

	virtual void WriteByte(Int8 a_Value) = 0;
	virtual void WriteInt(Int32 a_Value) = 0;
	virtual void WriteFloat(float a_Value) = 0;
	virtual void WriteString(const AString & a_Value) = 0;
	virtual void WriteItem(const cItem & a_Value) = 0;
	virtual void WritePosition(int a_X, int a_Y, int a_Z) = 0;
	virtual void WriteBool(bool a_Value) = 0;
	virtual void WriteBlockType(BLOCKTYPE a_ID, NIBBLETYPE a_Data) = 0;
	virtual void SkipMeta(void) = 0;

	const int m_ProtocolVersion;
};
