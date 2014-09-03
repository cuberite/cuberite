// BeaconEntity.h

// Declares the cBeaconEntity class representing a single beacon in the world





#pragma once

#include "BlockEntityWithItems.h"





namespace Json
{
	class Value;
}





// tolua_begin
class cBeaconEntity :
	public cBlockEntityWithItems
{
	typedef cBlockEntityWithItems super;

public:
	// tolua_end

	cBeaconEntity(int a_BlockX, int a_BlockY, int a_BlockZ, cWorld * a_World);

	bool LoadFromJson(const Json::Value & a_Value);
	// cBlockEntity overrides:
	virtual void SaveToJson(Json::Value& a_Value) override;
	virtual void SendTo(cClientHandle & a_Client) override;
	virtual bool Tick(float a_Dt, cChunk & a_Chunk) override;
	virtual void UsedBy(cPlayer * a_Player) override;

	/** Modify the beacon level. (It is needed to load the beacon corectly) */
	void SetBeaconLevel(char a_Level) { m_BeaconLevel = a_Level; }

	// tolua_begin

	/** Is the beacon active? */
	bool IsActive(void) const { return m_IsActive; }

	/** Returns the beacon level. (0 - 4) */
	char GetBeaconLevel(void) const { return m_BeaconLevel; }

	cEntityEffect::eType GetPrimaryEffect(void) const { return m_PrimaryEffect; }
	cEntityEffect::eType GetSecondaryEffect(void) const { return m_SecondaryEffect; }

	/** Sets the primary effect. Returns false when the effect is invalid. */
	bool SetPrimaryEffect(cEntityEffect::eType a_Effect);

	/** Sets the secondary effect. Returns false when the effect is invalid. */
	bool SetSecondaryEffect(cEntityEffect::eType a_Effect);

	/** Calculate the amount of layers the pyramid below the beacon has. */
	char CalculatePyramidLevel(void);

	/** Is the beacon blocked by non-transparent blocks that are higher than the beacon? */
	bool IsBeaconBlocked(void);

	/** Update the beacon. */
	void UpdateBeacon(void);

	/** Give the near-players the effects. */
	void GiveEffects(void);

	/** Returns true if the block is a diamond block, a golden block, an iron block or an emerald block. */
	static bool IsMineralBlock(BLOCKTYPE a_BlockType);

	/** Returns true if the effect can be used. */
	static bool IsValidEffect(cEntityEffect::eType a_Effect, char a_BeaconLevel);

	// tolua_end

protected:
	bool m_IsActive;
	char m_BeaconLevel;

	cEntityEffect::eType m_PrimaryEffect, m_SecondaryEffect;
} ;  // tolua_export




