#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules
#include "cAIPassiveAggressiveComponent.h"





cAIPassiveAggressiveComponent::cAIPassiveAggressiveComponent(cMonster * a_Monster) : cAIComponent(a_Monster){}





void cAIPassiveAggressiveComponent::Tick(float a_Dt, cChunk & a_Chunk)
{
	super::Tick(a_Dt, a_Chunk);
}
