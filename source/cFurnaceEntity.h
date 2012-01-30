#pragma once

#include "cBlockEntity.h"
#include "cWindowOwner.h"
#include "FileDefine.h"

namespace Json
{
	class Value;
}

class cChunk;
class cClientHandle;
class cServer;
class cItem;
class cNBTData;
class cFurnaceEntity : public cBlockEntity, public cWindowOwner
{
public:
	cFurnaceEntity(int a_X, int a_Y, int a_Z, cChunk* a_Chunk);
	virtual ~cFurnaceEntity();
	virtual void Destroy();

	bool LoadFromFile(cFile & a_File);  // deprecated format

	bool LoadFromJson(const Json::Value& a_Value );
	void SaveToJson  (Json::Value& a_Value );

	bool Tick( float a_Dt );
	virtual void UsedBy( cPlayer & a_Player );

	bool StartCooking();

	void ResetCookTimer();
private:
	cItem* m_Items;
	cItem* m_CookingItem;
	float m_CookTime;
	float m_TimeCooked;

	float m_BurnTime;
	float m_TimeBurned;
};