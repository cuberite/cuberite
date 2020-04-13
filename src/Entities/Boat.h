
// Boat.h

// Declares the cBoat class representing a boat in the world





#pragma once

#include "Entity.h"



// tolua_begin

class cBoat:
	public cEntity
{
	// tolua_end

	using Super = cEntity;

	// tolua_begin

public:
	enum eMaterial
	{
		bmOak,
		bmSpruce,
		bmBirch,
		bmJungle,
		bmAcacia,
		bmDarkOak
	};

	// tolua_end

	CLASS_PROTODEF(cBoat)

	cBoat(Vector3d a_Pos, eMaterial a_Material);

	// cEntity overrides:
	virtual void SpawnOn(cClientHandle & a_ClientHandle) override;
	virtual void BroadcastMovementUpdate(const cClientHandle * a_Exclude = nullptr) override;
	virtual void OnRightClicked(cPlayer & a_Player) override;
	virtual bool DoTakeDamage(TakeDamageInfo & TDI) override;
	virtual void Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk) override;
	virtual void HandleSpeedFromAttachee(float a_Forward, float a_Sideways) override;

	int GetLastDamage(void) const { return m_LastDamage; }
	int GetForwardDirection(void) const { return m_ForwardDirection; }

	float GetDamageTaken(void) const { return m_DamageTaken; }

	// tolua_begin

	/** Returns the eMaterial of the boat */
	eMaterial GetMaterial(void) const { return m_Material; }

	/** Sets the eMaterial of the boat */
	void SetMaterial(cBoat::eMaterial a_Material) { m_Material = a_Material; }

	/** Returns the eMaterial that should be used for a boat created from the specified item. Returns bmOak if not a boat item */
	static eMaterial ItemToMaterial(const cItem & a_Item);

	/** Returns the boat item of the boat material */
	static cItem MaterialToItem(eMaterial a_Material);

	/** Returns the eMaterial as string */
	static AString MaterialToString(const eMaterial a_Material);

	/** Returns the boat material for the passed string. Returns oak if not valid */
	static eMaterial StringToMaterial(const AString & a_Material);

	// tolua_end

	bool IsRightPaddleUsed(void) const { return m_RightPaddleUsed; }
	bool IsLeftPaddleUsed(void) const { return m_LeftPaddleUsed; }

	void SetLastDamage(int TimeSinceLastHit);

	void UpdatePaddles(bool rightPaddleUsed, bool leftPaddleUsed);
private:
	int m_LastDamage;
	int m_ForwardDirection;

	float m_DamageTaken;

	eMaterial m_Material;

	bool m_RightPaddleUsed;
	bool m_LeftPaddleUsed;
} ;  // tolua_export
