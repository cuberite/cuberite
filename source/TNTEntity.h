
#pragma once

#include "Entity.h"
#include "Defines.h"





class cTNTEntity :
	public cEntity
{
typedef cEntity super;
	
public:
	CLASS_PROTODEF(cTNTEntity);

	cTNTEntity(int a_X,int a_Y,int a_Z,float a_MaxFuseTime);
	cTNTEntity(const Vector3i a_Pos,float a_MaxFuseTime);
	
	// cEntity overrides:
	virtual void Initialize(cWorld * a_World) override;
	virtual void SpawnOn(cClientHandle & a_ClientHandle) override;
	virtual void Tick(float a_Dt, cChunk & a_Chunk) override;
private:
	float m_Counter;     //In seconds too
	float m_MaxFuseTime; //in seconds
};




