#pragma once
#include "cPacket.h"
#include "PacketID.h"

class cPacket_Metadata : public cPacket 
{
public:
	cPacket_Metadata(int s, int id);
	cPacket_Metadata();
	~cPacket_Metadata();

	bool Send(cSocket & a_Socket);
	void FormPacket();
	virtual cPacket* Clone() const { return new cPacket_Metadata( *this ); }
	
	enum MetaState{NORMAL,BURNING,CROUCHED,RIDING} EMetaState;
	
	static const unsigned int c_Size = 4 + 1;
	int m_UniqueID;
	char m_Type;
	unsigned int m_MetaDataSize;
	char* m_MetaData;
};
