#pragma once

class cPawn;

// tolua_begin
class cEntityEffect {
public:
	
	/** All types of entity effects (numbers correspond to IDs) */
	enum eType
	{
		effNoEffect       = 0,
		effSpeed          = 1,
		effSlowness       = 2,
		effHaste          = 3,
		effMiningFatigue  = 4,
		effStrength       = 5,
		effInstantHealth  = 6,
		effInstantDamage  = 7,
		effJumpBoost      = 8,
		effNausea         = 9,
		effRegeneration   = 10,
		effResistance     = 11,
		effFireResistance = 12,
		effWaterBreathing = 13,
		effInvisibility   = 14,
		effBlindness      = 15,
		effNightVision    = 16,
		effHunger         = 17,
		effWeakness       = 18,
		effPoison         = 19,
		effWither         = 20,
		effHealthBoost    = 21,
		effAbsorption     = 22,
		effSaturation     = 23,
	} ;
	
	/** The duration of the effect */
	int m_Ticks;
	
	/** Returns how strong the effect will be applied */
	short GetIntensity() { return m_Intensity; }
	
	/** Returns the pawn that used this entity effect */
	cPawn *GetUser() { return m_User; }
	
	/** Returns the distance modifier for affecting potency */
	double GetDistanceModifier() { return m_DistanceModifier; }
	
	void SetIntensity(short a_Intensity)                { m_Intensity        = a_Intensity;        }
	void SetUser(cPawn *a_User)                         { m_User             = a_User;             }
	void SetDistanceModifier(double a_DistanceModifier) { m_DistanceModifier = a_DistanceModifier; }
	
	/**
	 *  An empty entity effect
	 */
	cEntityEffect();
	
	/**
	 *  An entity effect
	 *  @param a_Ticks            The duration of the effect
	 *  @param a_Intensity        How strong the effect will be applied
	 *  @param a_User             The pawn that used this entity effect
	 *  @param a_DistanceModifier The distance modifier for affecting potency, defaults to 1
	 */
	cEntityEffect(int a_Ticks, short a_Intensity, cPawn *a_User, double a_DistanceModifier = 1);
	
private:
	/** How strong the effect will be applied */
	short m_Intensity;
	
	/** The pawn that used this entity effect */
	cPawn *m_User;
	
	/** The distance modifier for affecting potency */
	double m_DistanceModifier;
};
// tolua_end
