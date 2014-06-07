#pragma once

class cPawn;

// tolua_begin
class cEntityEffect {
public:
	
	/** All types of entity effects (numbers correspond to IDs) */
	enum eType
	{
		efSpeed          = 1,
		efSlowness       = 2,
		efHaste          = 3,
		efMiningFatigue  = 4,
		efStrength       = 5,
		efInstantHealth  = 6,
		efInstantDamage  = 7,
		efJumpBoost      = 8,
		efNausia         = 9,
		efRegeneration   = 10,
		efResistance     = 11,
		efFireResistance = 12,
		efWaterBreathing = 13,
		efInvisibility   = 14,
		efBlindness      = 15,
		efNightVision    = 16,
		efHunger         = 17,
		efWeakness       = 18,
		efPoison         = 19,
		efWither         = 20,
		efHealthBoost    = 21,
		efAbsorption     = 22,
		efSaturation     = 23,
	} ;
	
	/** The duration of the effect */
	int m_Ticks;
	
	/** Returns how strong the effect will be applied */
	short GetIntensity() { return m_Intensity; }
	
	/** Returns the pawn that used this entity effect */
	cPawn *GetUser() { return m_User; }
	
	/** Returns the distance modifier for affecting potency */
	double GetDistanceModifier() { return m_DistanceModifier; }
	
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
