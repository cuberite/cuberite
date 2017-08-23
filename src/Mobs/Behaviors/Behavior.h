#pragma once

struct TakeDamageInfo;
class cBehavior
{
public:
	virtual bool IsControlDesired();
	virtual bool ControlStarting();
	virtual bool ControlEnding();
	virtual void Tick();
	virtual void PostTick();
	virtual void PreTick();


	virtual void onRightClicked();
	virtual void Destroyed();
	virtual void DoTakeDamage(TakeDamageInfo & a_TDI);
	virtual ~cBehavior() {}
};
