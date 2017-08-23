#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Behavior.h"





bool cBehavior::IsControlDesired()
{
	return false;
}





bool cBehavior::ControlStarting()
{
	return true;
}





bool cBehavior::ControlEnding()
{
	return true;
}





void cBehavior::Tick()
{
	LOGD("ERROR: Called a TICK on a behavior that doesn't have one.");
	ASSERT(1 == 0);
}





void cBehavior::PostTick()
{
	LOGD("ERROR: Called a PostTick on a behavior that doesn't have one.");
	ASSERT(1 == 0);
}





void cBehavior::PreTick()
{
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
