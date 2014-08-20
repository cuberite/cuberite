
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules
#include "AIAggressiveComponent.h"
#include <iostream>

#include "../NewMonster.h"


cAIAggressiveComponent::cAIAggressiveComponent(cNewMonster * a_Monster) : cAIComponent(a_Monster), m_Target(NULL){}

void cAIAggressiveComponent::Tick(float a_Dt, cChunk & a_Chunk)
{
	super::Tick(a_Dt, a_Chunk);
}


void cAIAggressiveComponent::Attack(float a_Dt)
{
}


