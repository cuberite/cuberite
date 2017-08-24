#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Behavior.h"





bool cBehavior::IsControlDesired(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	UNUSED(a_Dt);
	UNUSED(a_Chunk);
	LOGD("ERROR: Probably forgot to implement  cBehavior::IsControlDesired but implement  cBehavior::Tick");
	return false;
}





bool cBehavior::ControlStarting(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	UNUSED(a_Dt);
	UNUSED(a_Chunk);
	return true;
}





bool cBehavior::ControlEnding(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	UNUSED(a_Dt);
	UNUSED(a_Chunk);
	return true;
}





void cBehavior::Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	UNUSED(a_Dt);
	UNUSED(a_Chunk);
	LOGD("ERROR: Called a TICK on a behavior that doesn't have one.");
	ASSERT(1 == 0);
}





void cBehavior::PostTick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	UNUSED(a_Dt);
	UNUSED(a_Chunk);
	LOGD("ERROR: Called a PostTick on a behavior that doesn't have one.");
	ASSERT(1 == 0);
}





void cBehavior::PreTick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	UNUSED(a_Dt);
	UNUSED(a_Chunk);
	LOGD("ERROR: Called a PreTick on a behavior that doesn't have one.");
	ASSERT(1 == 0);
}





void cBehavior::onRightClicked()
{
	LOGD("ERROR: Called onRightClicked on a behavior that doesn't have one.");
	ASSERT(1 == 0);
}





void cBehavior::Destroyed()
{
	LOGD("ERROR: Called Destroyed on a behavior that doesn't have one.");
	ASSERT(1 == 0);
}




void cBehavior::DoTakeDamage(TakeDamageInfo & a_TDI)
{
	UNUSED(a_TDI);
	LOGD("ERROR: Called DoTakeDamage on a behavior that doesn't have one.");
	ASSERT(1 == 0);
}
