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

	// tolua_begin

	/** Is the beacon active? */
	bool IsActive(void) const { return m_IsActive; }

	/** Returns the beacon level. (0 - 4) */
	char GetBeaconLevel(void) const { return m_BeaconLevel; }

	cEntityEffect::eType GetPrimaryPotion(void) const { return m_PrimaryPotion; }
	cEntityEffect::eType GetSecondaryPotion(void) const { return m_SecondaryPotion; }

	/** Select the primary potion. Returns false when the potion is invalid.*/
	bool SelectPrimaryPotion(cEntityEffect::eType a_Potion);

	/** Select the secondary potion. Returns false when the potion is invalid. */
	bool SelectSecondaryPotion(cEntityEffect::eType a_Potion);

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

	/** Returns true if the potion can be used. */
	static bool IsValidPotion(cEntityEffect::eType a_Potion, char a_BeaconLevel);

	// tolua_end

protected:
	bool m_IsActive;
	char m_BeaconLevel;

	cEntityEffect::eType m_PrimaryPotion, m_SecondaryPotion;
} ;  // tolua_export




