
#pragma once

#include "cPacket.h"





class Vector3i;





class cPacket_SpawnMob : public cPacket
{
public:
	cPacket_SpawnMob();
	cPacket_SpawnMob( const cPacket_SpawnMob & a_Clone );
	virtual cPacket* Clone() const { return new cPacket_SpawnMob( *this ); }
	~cPacket_SpawnMob();

	virtual void Serialize(AString & a_Data) const override;

	int       m_UniqueID;
	char      m_Type;
	Vector3i* m_Pos;
	char      m_Yaw;
	char      m_Pitch;

	static const unsigned int c_Size = 1 + 4 + 1 + 4 + 4 + 4 + 1 + 1; // + metadata

	unsigned int m_MetaDataSize;
	char * m_MetaData;
};




