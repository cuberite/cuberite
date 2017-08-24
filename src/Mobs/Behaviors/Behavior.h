#pragma once

struct TakeDamageInfo;
class cChunk;
#include <chrono>

class cBehavior
{
public:
	virtual bool IsControlDesired(std::chrono::milliseconds a_Dt, cChunk & a_Chunk);
	virtual bool ControlStarting(std::chrono::milliseconds a_Dt, cChunk & a_Chunk);
	virtual bool ControlEnding(std::chrono::milliseconds a_Dt, cChunk & a_Chunk);
	virtual void Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk);
	virtual void PostTick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk);
	virtual void PreTick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk);


	virtual void onRightClicked();
	virtual void Destroyed();
	virtual void DoTakeDamage(TakeDamageInfo & a_TDI);
	virtual ~cBehavior() {}
};
