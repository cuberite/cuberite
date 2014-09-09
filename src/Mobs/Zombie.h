#pragma once

#include "Monster.h"
#include "Components/BurningComponent.h"
#include "Chunk.h"



class cZombie :
	public cMonster
{
	typedef cMonster super;
	
public:
	cZombie(bool a_IsVillagerZombie);

	CLASS_PROTODEF(cZombie)
	
	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = NULL) override;
	virtual void Tick(float a_Dt, cChunk & a_Chunk)
	{
		super::Tick(a_Dt, a_Chunk);
		m_BurningComponent.Tick(a_Dt, a_Chunk);
	}
	
	bool IsVillagerZombie(void) const { return m_IsVillagerZombie; }
	bool IsConverting    (void) const { return m_IsConverting; }

private:

	bool m_IsVillagerZombie;
	bool m_IsConverting;
	
	cBurningComponent<cEntity, cChunk> m_BurningComponent;

} ;




