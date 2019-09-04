
#pragma once

#include "Entity.h"





// tolua_begin
class cArmorStand :
	public cEntity
{
	typedef cEntity super;
	// tolua_end

public:
	CLASS_PROTODEF(cArmorStand)

	cArmorStand(Vector3d a_Pos, double a_Yaw);


	// tolua_begin

	/** Returns true if the armor stand is invisible. */
	virtual bool IsInvisible() const override;

	/** Set whether the armor stand is visible */
	void SetVisible(bool a_IsVisible);

	/** Returns true if the armor stand has a custom name. */
	bool HasCustomName(void) const { return !m_CustomName.empty(); }

	/** Gets the custom name of the armor stand. If no custom name is set, the function returns an empty string. */
	const AString & GetCustomName(void) const { return m_CustomName; }

	/** Sets the custom name of the armor stand. You see the name over the armor stand.
	If you want to disable the custom name, simply set an empty string. */
	void SetCustomName(const AString & a_CustomName);

	/** Is the custom name of this armor stand always visible? If not, you only see the name when you sight the mob. */
	bool IsCustomNameAlwaysVisible(void) const { return m_CustomNameAlwaysVisible; }

	/** Sets the custom name visiblity of this armor stand.
	If it's false, you only see the name when you sight the mob. If it's true, you always see the custom name. */
	void SetCustomNameAlwaysVisible(bool a_CustomNameAlwaysVisible);

	/** Returns true if the armor stand has gravity. */
	bool HasGravity(void) const { return (std::abs(m_Gravity) > std::numeric_limits<float>::epsilon()); }

	/** Returns true if the armor stand is a normal sized one. */
	bool IsSizeNormal(void) const { return !m_IsSmall; }

	/** Returns true if the armor stand is a small one. */
	bool IsSizeSmall(void) const { return m_IsSmall; }

	/** Returns true if the armor stand is a marker (does not interact). */
	bool IsMarker(void) const { return m_IsMarker; }

	/** Sets if the armor stand is a normal sized one. */
	void SetSizeNormal(void);

	/** Sets if the armor stand is a small one. */
	void SetSizeSmall(void);

	/** Sets whether the armor stand is a marker (does not interact). */
	void SetIsMarker(bool a_IsMarker);

	/** Returns true if the armor stand has amrs. */
	bool HasArms(void) const { return m_HasArms; }
	void SetHasArms(bool a_HasArms);

	/** Returns true if the armor stand has a base platform. */
	bool HasBasePlate(void) const { return m_HasBasePlate; }
	void SetHasBasePlate(bool a_HasBasePlate);

	Vector3f GetHeadRotation(void) const { return m_HeadRotation; }
	Vector3f GetBodyRotation(void) const { return m_BodyRotation; }
	Vector3f GetLeftArmRotation(void) const { return m_LeftArmRotation; }
	Vector3f GetRightArmRotation(void) const { return m_RightArmRotation; }
	Vector3f GetLeftLegRotation(void) const { return m_LeftLegRotation; }
	Vector3f GetRightLegRotation(void) const { return m_RightLegRotation; }

	void SetHeadRotation(Vector3f a_HeadRotation);
	void SetBodyRotation(Vector3f a_BodyRotation);
	void SetLeftArmRotation(Vector3f a_LeftArmRotation);
	void SetRightArmRotation(Vector3f a_RightArmRotation);
	void SetLeftLegRotation(Vector3f a_LeftLegRotation);
	void SetRightLegRotation(Vector3f a_RightLegRotation);

	virtual cItem GetEquippedWeapon(void) const override { return m_RightHand; }
	virtual cItem GetOffHandEquipedItem(void) const override { return m_LeftHand; }
	virtual cItem GetEquippedHelmet(void) const override { return m_Helmet; }
	virtual cItem GetEquippedChestplate(void) const override { return m_ChestPlate; }
	virtual cItem GetEquippedLeggings(void) const override { return m_Leggings; }
	virtual cItem GetEquippedBoots(void) const override { return m_Boots; }

	void SetEquippedWeapon(const cItem & a_RightHand);
	void SetOffHandEquipedItem(const cItem & a_LeftHand);
	void SetEquippedHelmet(const cItem & a_Helmet);
	void SetEquippedChestplate(const cItem & a_ChestPlate);
	void SetEquippedLeggings(const cItem & a_Leggings);
	void SetEquippedBoots(const cItem & a_Boots);

	// tolua_end

protected:

	// cEntity overrides:
	virtual void OnRightClicked(cPlayer & a_Player) override;
	virtual void OnClickedAt(cPlayer & a_Player, Vector3f a_TargetPos, eHand a_Hand) override;
	virtual void Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk) override;
	virtual bool DoTakeDamage(TakeDamageInfo & TDI) override;
	virtual void SpawnOn(cClientHandle & a_ClientHandle) override;


	bool m_IsVisible;
	AString m_CustomName;
	bool m_CustomNameAlwaysVisible;
	int m_TicksSinceLastDamaged;  // How many ticks ago we were last damaged by a player?

	bool m_IsSmall;
	bool m_IsMarker;
	bool m_HasArms;
	bool m_HasBasePlate;
	Vector3f m_HeadRotation;
	Vector3f m_BodyRotation;
	Vector3f m_LeftArmRotation;
	Vector3f m_RightArmRotation;
	Vector3f m_LeftLegRotation;
	Vector3f m_RightLegRotation;

	cItem m_LeftHand;
	cItem m_RightHand;
	cItem m_Helmet;
	cItem m_ChestPlate;
	cItem m_Leggings;
	cItem m_Boots;

};  // tolua_export
