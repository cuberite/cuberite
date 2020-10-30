
#pragma once

#include "Entity.h"





// tolua_begin
class cEnderCrystal :
	public cEntity
{
	// tolua_end
	using Super = cEntity;

public:

	CLASS_PROTODEF(cEnderCrystal)

	cEnderCrystal(Vector3d a_Pos, bool a_ShowBottom);

	// tolua_begin
	// Getters and Setters
	bool ShowsBottom() const { return m_ShowBottom; }
	void SetShowBottom(bool a_ShowBottom);

	Vector3i GetBeamTarget() const { return m_BeamTarget; }
	void SetBeamTarget(Vector3i a_BeamTarget);

	/** If the EnderCrystal should send it's beam to the client. */
	bool DisplaysBeam() const { return m_DisplayBeam; }
	void SetDisplayBeam(bool a_DisplayBeam);

	// tolua_end
private:

	// If the bedrock base should be displayed
	bool m_ShowBottom;

	Vector3i m_BeamTarget;
	bool m_DisplayBeam;

	// cEntity overrides:
	virtual void SpawnOn(cClientHandle & a_ClientHandle) override;
	virtual void Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk) override;
	virtual void KilledBy(TakeDamageInfo & a_TDI) override;

};  // tolua_export




