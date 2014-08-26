
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules
#include "AIPassiveComponent.h"





cAIPassiveComponent::cAIPassiveComponent(cMonster * a_Monster) : cAIComponent(a_Monster){}





void cAIPassiveComponent::Tick(float a_Dt, cChunk & a_Chunk)
{
	super::Tick(a_Dt, a_Chunk);
}