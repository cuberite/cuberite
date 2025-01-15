
#pragma once

#include "Entity.h"
#include "EntityEffect.h"

// fwd cMonster
class cMonster;





// tolua_begin
class cPawn:
	public cEntity
{
	// tolua_end
	using Super = cEntity;

public:

	CLASS_PROTODEF(cPawn)

	cPawn(eEntityType a_EntityType, float a_Width, float a_Height);

	virtual void Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk) override;
	virtual void KilledBy(TakeDamageInfo & a_TDI) override;

	virtual bool IsFireproof(void) const override;
	virtual bool IsInvisible() const override;
	virtual void HandleAir(void) override;
	virtual void HandleFalling(void);
	virtual void OnRemoveFromWorld(cWorld & a_World) override;

	/** Handles farmland trampling when hitting the ground.
	Algorithm:
	fall height <= 0.6875 blocks: no trampling
	fall height > 0.6875 and <= 1.0625: 25% chance of trampling
	fall height > 1.0625 and <= 1.5625: 66% chance of trampling
	fall height > 1.5625: always trample
	The values may differ from vanilla, they were determined experimentally.
	Additionaly, mobs smaller than 0.512 cubic blocks won't trample.
	*/
	void HandleFarmlandTrampling(double a_FallHeight, BlockState a_BlockAtFoot, BlockState a_BlockBelow);

	/** Tells all pawns which are targeting us to stop targeting us. */
	void StopEveryoneFromTargetingMe();



	// tolua_begin

	/** Applies an entity effect.
	Checks with plugins if they allow the addition.
	a_EffectIntensity is the level of the effect (0 = Potion I, 1 = Potion II, etc).
	a_DistanceModifier is the scalar multiplied to the potion duration (only applies to splash potions).
	*/
	void AddEntityEffect(cEntityEffect::eType a_EffectType, int a_EffectDurationTicks, short a_EffectIntensity, double a_DistanceModifier = 1);

	/** Removes a currently applied entity effect. */
	void RemoveEntityEffect(cEntityEffect::eType a_EffectType);

	/** Returns true, if the entity effect is currently applied. */
	bool HasEntityEffect(cEntityEffect::eType a_EffectType) const;

	/** Removes all currently applied entity effects (used when drinking milk) */
	void ClearEntityEffects(void);

	// tolua_end

	/** Remove the monster from the list of monsters targeting this pawn. */
	void NoLongerTargetingMe(cMonster * a_Monster);

	/** Add the monster to the list of monsters targeting this pawn. (Does not check if already in list!) */
	void TargetingMe(cMonster * a_Monster);

	/** Returns all entity effects */
	std::map<cEntityEffect::eType, cEntityEffect *> GetEntityEffects() const;

	/** Returns the entity effect, if it is currently applied or nullptr if not. */
	cEntityEffect * GetEntityEffect(cEntityEffect::eType a_EffectType) const;

	// tolua_begin

	static bool FindTeleportDestination(cWorld & a_World, const int a_HeightRequired, const unsigned int a_NumTries, Vector3d & a_Destination, const Vector3i a_MinBoxCorner, const Vector3i a_MaxBoxCorner);

	static bool FindTeleportDestination(cWorld & a_World, const int a_HeightRequired, const unsigned int a_NumTries, Vector3d & a_Destination, const cBoundingBox a_BoundingBox);

	/** Used by enderman and chorus fruit.
	Checks for valid destinations in a cube of length 2 * a_HalfCubeWidth centred at a_Centre.
	Returns true and places destination in a_Destination if successful.
	Returns false if destination could be found after a_NumTries attempts.
	Details at: https://minecraft.wiki/w/Enderman#Teleportation. */
	static bool FindTeleportDestination(cWorld & a_World, const int a_HeightRequired, const unsigned int a_NumTries, Vector3d & a_Destination, Vector3i a_Centre, const int a_HalfCubeWidth);

	// tolua_end

protected:

	typedef std::map<cEntityEffect::eType, std::unique_ptr<cEntityEffect>> tEffectMap;
	tEffectMap m_EntityEffects;

	double m_LastGroundHeight;
	bool m_bTouchGround;

	virtual void ResetPosition(Vector3d a_NewPosition) override;

private:

	/** A list of all monsters that are targeting this pawn. */
	std::vector<cMonster*> m_TargetingMe;

	/** Attempt to activate a Totem of Undying.
	If activation for the given type of damage was successful, consumes the totem and returns true. */
	bool DeductTotem(eDamageType a_DamageType);
} ;  // tolua_export
