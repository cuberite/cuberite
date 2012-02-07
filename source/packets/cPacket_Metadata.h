
#pragma once
#include "cPacket.h"

#include "../cPawn.h"





class cPacket_Metadata : public cPacket 
{
public:
	cPacket_Metadata(int s, int id);
	cPacket_Metadata();
	~cPacket_Metadata();

	virtual void Serialize(AString & a_Data) const override;
	
	void FormPacket();
	virtual cPacket* Clone() const { return new cPacket_Metadata( *this ); }
	
	cPawn::MetaData m_EMetaData;
	
	static const unsigned int c_Size = 4 + 1;
	int m_UniqueID;
	char m_Type;
	unsigned int m_MetaDataSize;
	char* m_MetaData;
};




