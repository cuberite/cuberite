#pragma once

struct TakeDamageInfo;
class cChunk;
class cPlayer;
class cMonster;
class cPawn;
class cWorld;
class cItems;
class cEntity;
struct TakeDamageInfo;
#include <chrono>

class cBehavior
{
public:
	// Tick-related
	virtual bool IsControlDesired(std::chrono::milliseconds a_Dt, cChunk & a_Chunk);
	virtual bool ControlStarting(std::chrono::milliseconds a_Dt, cChunk & a_Chunk);
	virtual bool ControlEnding(std::chrono::milliseconds a_Dt, cChunk & a_Chunk);
	virtual void Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk);
	virtual void PostTick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk);
	virtual void PreTick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk);

	// Other
	virtual void OnRightClicked(cPlayer & a_Player);
	virtual void Destroyed();
	virtual void DoTakeDamage(TakeDamageInfo & a_TDI);
	virtual ~cBehavior() {}
};
