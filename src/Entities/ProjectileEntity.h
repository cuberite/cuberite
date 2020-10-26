
// ProjectileEntity.h

// Declares the cProjectileEntity class representing the common base class for projectiles





#pragma once

#include "Entity.h"





// tolua_begin

class cProjectileEntity :
	public cEntity
{
	// tolua_end

	using Super = cEntity;

	// tolua_begin


public:

	/** The kind of the projectile. */
	enum eKind
	{
		pkArrow,
		pkSnowball,
		pkEgg,
		pkGhastFireball,
		pkFireCharge,
		pkEnderPearl,
		pkExpBottle,
		pkSplashPotion,
		pkFirework,
		pkWitherSkull,
	} ;

	// tolua_end

	CLASS_PROTODEF(cProjectileEntity)

	cProjectileEntity(eKind a_Kind, cEntity * a_Creator, Vector3d a_Pos, double a_Width, double a_Height);
	cProjectileEntity(eKind a_Kind, cEntity * a_Creator, Vector3d a_Pos, Vector3d a_Speed, double a_Width, double a_Height);

	/** Creates a new instance of the specified projectile entity.
	a_Item is the item from which the projectile originated (such as firework or arrow). */
	static std::unique_ptr<cProjectileEntity> Create(
		eKind a_Kind,
		cEntity * a_Creator,
		Vector3d a_Pos,
		const cItem * a_Item,
		const Vector3d * a_Speed = nullptr
	);

	/** OBSOLETE, use the Vector3d-based overload instead.
	Creates a new instance of the specified projectile entity.
	a_Item is the item from which the projectile originated (such as firework or arrow). */
	static std::unique_ptr<cProjectileEntity> Create(
		eKind a_Kind,
		cEntity * a_Creator,
		double a_PosX, double a_PosY, double a_PosZ,
		const cItem * a_Item,
		const Vector3d * a_Speed = nullptr
	)
	{
		return Create(a_Kind, a_Creator, {a_PosX, a_PosY, a_PosZ}, a_Item, a_Speed);
	}

	/** Called by the physics blocktracer when the entity hits a solid block, the hit position and the face hit (BLOCK_FACE_) is given */
	virtual void OnHitSolidBlock(Vector3d a_HitPos, eBlockFace a_HitFace);

	/** Called by the physics blocktracer when the entity hits another entity */
	virtual void OnHitEntity(cEntity & a_EntityHit, Vector3d a_HitPos);

	/** Called by Chunk when the projectile is eligible for player collection */
	virtual void CollectedBy(cPlayer & a_Dest);

	// tolua_begin

	/** Returns the kind of the projectile (fast class identification) */
	eKind GetProjectileKind(void) const { return m_ProjectileKind; }

	/** Returns the unique ID of the entity who created this projectile
	May return an ID <0
	*/
	UInt32 GetCreatorUniqueID(void) const { return m_CreatorData.m_UniqueID; }

	/** Returns the name of the player that created the projectile
	Will be empty for non-player creators
	*/
	AString GetCreatorName(void) const { return m_CreatorData.m_Name; }

	/** Returns the string that is used as the entity type (class name) in MCA files */
	AString GetMCAClassName(void) const;

	/** Returns true if the projectile has hit the ground and is stuck there */
	bool IsInGround(void) const { return m_IsInGround; }

	// tolua_end

	/** Sets the internal InGround flag. To be used by MCA loader only! */
	void SetIsInGround(bool a_IsInGround) { m_IsInGround = a_IsInGround; }

protected:

	/** A structure that stores the Entity ID and Playername of the projectile's creator
	Used to migitate invalid pointers caused by the creator being destroyed. */
	struct CreatorData
	{
		CreatorData(UInt32 a_UniqueID, const AString & a_Name, const cEnchantments & a_Enchantments) :
			m_UniqueID(a_UniqueID),
			m_Name(a_Name),
			m_Enchantments(a_Enchantments)
		{
		}

		const UInt32 m_UniqueID;
		AString m_Name;
		cEnchantments m_Enchantments;
	};

	/** The type of projectile I am */
	eKind m_ProjectileKind;

	/** The structure for containing the entity ID and name who has created this projectile
	The ID and / or name may be nullptr (e.g. for dispensers / mobs). */
	CreatorData m_CreatorData;

	/** True if the projectile has hit the ground and is stuck there */
	bool m_IsInGround;

	// cEntity overrides:
	virtual void Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk) override;
	virtual void HandlePhysics(std::chrono::milliseconds a_Dt, cChunk & a_Chunk) override;
	virtual void SpawnOn(cClientHandle & a_Client) final override;

} ;  // tolua_export
