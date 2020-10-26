
#pragma once

#include "HangingEntity.h"


class cWorldInterface;




// tolua_begin
class cLeashKnot :
	public cHangingEntity
{
	// tolua_end
	using Super = cHangingEntity;

public:  // tolua_export

	CLASS_PROTODEF(cLeashKnot)

	cLeashKnot(eBlockFace a_BlockFace, Vector3d a_Pos);

	/** Looks for mobs leashed to a player and ties them to this knot */
	void TiePlayersLeashedMobs(cPlayer & a_Player, bool a_ShouldBroadCast);

	void SetShouldSelfDestroy() { m_ShouldSelfDestroy = true; }

	/** Returns the leash knot entity representing the knot at the specified position. Returns nullptr if there's no knot. */
	static cLeashKnot * FindKnotAtPos(cWorldInterface & a_WorldInterface, Vector3i a_BlockPos);

private:

	/** When a fence is destroyed, the knot on it gets destroyed after a while. This flag turns on the countdown to self destroy. */
	bool m_ShouldSelfDestroy;
	int m_TicksToSelfDestroy;

	virtual void OnRightClicked(cPlayer & a_Player) override;
	virtual void KilledBy(TakeDamageInfo & a_TDI) override;
	virtual void GetDrops(cItems & a_Items, cEntity * a_Killer) override;
	virtual void SpawnOn(cClientHandle & a_ClientHandle) override;
	virtual void Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk) override;

};  // tolua_export




