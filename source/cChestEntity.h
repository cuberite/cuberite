#pragma once

#include "cBlockEntity.h"
#include "cWindowOwner.h"
#include "FileDefine.h"

namespace Json
{
	class Value;
};

class cClientHandle;
class cServer;
class cItem;
class cNBTData;
class cChestEntity : public cBlockEntity, public cWindowOwner
{
public:
	cChestEntity(int a_X, int a_Y, int a_Z, cChunk* a_Chunk);
	virtual ~cChestEntity();
	virtual void Destroy();

	void HandleData( cNBTData* a_NBTData );

	cItem *	GetSlot( int a_Slot );
	void	SetSlot( int a_Slot, cItem & a_Item );

	void WriteToFile(FILE* a_File);
	bool LoadFromFile(FILE* a_File);

	bool LoadFromJson( const Json::Value& a_Value );
	void SaveToJson( Json::Value& a_Value );

	void SendTo( cClientHandle* a_Client, cServer* a_Server );

	virtual void UsedBy( cPlayer & a_Player );

	static const int c_ChestWidth = 9;
	static const int c_ChestHeight = 3;
private:
	cItem* m_Content;
};