#pragma once

class cPawn;

// tolua_begin
class cEntityEffect
{
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
	
	/** Creates an empty entity effect */
	cEntityEffect(void);
	
	/** Creates an entity effect of the specified type
	@param a_Duration         How long this effect will last, in ticks
	@param a_Intensity        How strong the effect will be applied
	@param a_DistanceModifier The distance modifier for affecting potency, defaults to 1 */
	cEntityEffect(int a_Duration, short a_Intensity, double a_DistanceModifier = 1);
	
	/** Creates an entity effect by copying another
	@param a_OtherEffect      The other effect to copy */
	cEntityEffect(const cEntityEffect & a_OtherEffect);
	
	/** Creates an entity effect by copying another
	@param a_OtherEffect      The other effect to copy */
	cEntityEffect & operator=(cEntityEffect a_OtherEffect);
	
	virtual ~cEntityEffect(void) {};
	
	/** Creates a pointer to the proper entity effect from the effect type
	@warning This function creates raw pointers that must be manually managed.
	@param a_EffectType       The effect type to create the effect from
	@param a_Duration         How long this effect will last, in ticks
	@param a_Intensity        How strong the effect will be applied
	@param a_DistanceModifier The distance modifier for affecting potency, defaults to 1 */
	static cEntityEffect * CreateEntityEffect(cEntityEffect::eType a_EffectType, int a_Duration, short a_Intensity, double a_DistanceModifier);
	
	/** Returns how many ticks this effect has been active for */
	int GetTicks() { return m_Ticks; }
	/** Returns the duration of the effect */
	int GetDuration() { return m_Duration; }
	/** Returns how strong the effect will be applied */
	short GetIntensity() { return m_Intensity; }
	/** Returns the distance modifier for affecting potency */
	double GetDistanceModifier() { return m_DistanceModifier; }
	
	void SetTicks(int a_Ticks)                          { m_Ticks            = a_Ticks;            }
	void SetDuration(int a_Duration)                    { m_Duration         = a_Duration;         }
	void SetIntensity(short a_Intensity)                { m_Intensity        = a_Intensity;        }
	void SetDistanceModifier(double a_DistanceModifier) { m_DistanceModifier = a_DistanceModifier; }
	
	virtual void OnTick(cPawn & a_Target);
	virtual void OnActivate(cPawn & a_Target) { }
	virtual void OnDeactivate(cPawn & a_Target) { }
	
protected:
	/** How many ticks this effect has been active for */
	int m_Ticks;
	
	/** How long this effect will last, in ticks */
	int m_Duration;
	
	/** How strong the effect will be applied */
	short m_Intensity;
	
	/** The distance modifier for affecting potency */
	double m_DistanceModifier;
};

/////////////////////////////////////////////////////////////////////////
// Speed
/////////////////////////////////////////////////////////////////////////
class cEntityEffectSpeed:
	public cEntityEffect
{
	typedef cEntityEffect super;
public:
	cEntityEffectSpeed(int a_Duration, short a_Intensity, double a_DistanceModifier = 1):
		super(a_Duration, a_Intensity, a_DistanceModifier)
	{
	}
};

/////////////////////////////////////////////////////////////////////////
// Slowness
/////////////////////////////////////////////////////////////////////////
class cEntityEffectSlowness:
	public cEntityEffect
{
	typedef cEntityEffect super;
public:
	cEntityEffectSlowness(int a_Duration, short a_Intensity, double a_DistanceModifier = 1):
		super(a_Duration, a_Intensity, a_DistanceModifier)
	{
	}
};

/////////////////////////////////////////////////////////////////////////
// Haste
/////////////////////////////////////////////////////////////////////////
class cEntityEffectHaste:
	public cEntityEffect
{
	typedef cEntityEffect super;
public:
	cEntityEffectHaste(int a_Duration, short a_Intensity, double a_DistanceModifier = 1):
		super(a_Duration, a_Intensity, a_DistanceModifier)
	{
	}
};

/////////////////////////////////////////////////////////////////////////
// Mining Fatigue
/////////////////////////////////////////////////////////////////////////
class cEntityEffectMiningFatigue:
	public cEntityEffect
{
	typedef cEntityEffect super;
public:
	cEntityEffectMiningFatigue(int a_Duration, short a_Intensity, double a_DistanceModifier = 1):
		super(a_Duration, a_Intensity, a_DistanceModifier)
	{
	}
};

/////////////////////////////////////////////////////////////////////////
// Strength
/////////////////////////////////////////////////////////////////////////
class cEntityEffectStrength:
	public cEntityEffect
{
	typedef cEntityEffect super;
public:
	cEntityEffectStrength(int a_Duration, short a_Intensity, double a_DistanceModifier = 1):
		super(a_Duration, a_Intensity, a_DistanceModifier)
	{
	}
};

/////////////////////////////////////////////////////////////////////////
// Instant Health
/////////////////////////////////////////////////////////////////////////
class cEntityEffectInstantHealth:
	public cEntityEffect
{
	typedef cEntityEffect super;
public:
	cEntityEffectInstantHealth(int a_Duration, short a_Intensity, double a_DistanceModifier = 1):
		super(a_Duration, a_Intensity, a_DistanceModifier)
	{
	}
	
	virtual void OnActivate(cPawn & a_Target) override;
};

/////////////////////////////////////////////////////////////////////////
// Instant Damage
/////////////////////////////////////////////////////////////////////////
class cEntityEffectInstantDamage:
	public cEntityEffect
{
	typedef cEntityEffect super;
public:
	cEntityEffectInstantDamage(int a_Duration, short a_Intensity, double a_DistanceModifier = 1):
		super(a_Duration, a_Intensity, a_DistanceModifier)
	{
	}
	
	virtual void OnActivate(cPawn & a_Target) override;
};

/////////////////////////////////////////////////////////////////////////
// Jump Boost
/////////////////////////////////////////////////////////////////////////
class cEntityEffectJumpBoost:
	public cEntityEffect
{
	typedef cEntityEffect super;
public:
	cEntityEffectJumpBoost(int a_Duration, short a_Intensity, double a_DistanceModifier = 1):
		super(a_Duration, a_Intensity, a_DistanceModifier)
	{
	}
};

/////////////////////////////////////////////////////////////////////////
// Nausea
/////////////////////////////////////////////////////////////////////////
class cEntityEffectNausea:
	public cEntityEffect
{
	typedef cEntityEffect super;
public:
	cEntityEffectNausea(int a_Duration, short a_Intensity, double a_DistanceModifier = 1):
	super(a_Duration, a_Intensity, a_DistanceModifier)
	{
	}
};

/////////////////////////////////////////////////////////////////////////
// Regeneration
/////////////////////////////////////////////////////////////////////////
class cEntityEffectRegeneration:
	public cEntityEffect
{
	typedef cEntityEffect super;
public:
	cEntityEffectRegeneration(int a_Duration, short a_Intensity, double a_DistanceModifier = 1):
		super(a_Duration, a_Intensity, a_DistanceModifier)
	{
	}
	
	virtual void OnTick(cPawn & a_Target) override;
};

/////////////////////////////////////////////////////////////////////////
// Resistance
/////////////////////////////////////////////////////////////////////////
class cEntityEffectResistance:
	public cEntityEffect
{
	typedef cEntityEffect super;
public:
	cEntityEffectResistance(int a_Duration, short a_Intensity, double a_DistanceModifier = 1):
		super(a_Duration, a_Intensity, a_DistanceModifier)
	{
	}
};

/////////////////////////////////////////////////////////////////////////
// Fire Resistance
/////////////////////////////////////////////////////////////////////////
class cEntityEffectFireResistance:
	public cEntityEffect
{
	typedef cEntityEffect super;
public:
	cEntityEffectFireResistance(int a_Duration, short a_Intensity, double a_DistanceModifier = 1):
		super(a_Duration, a_Intensity, a_DistanceModifier)
	{
	}
};

/////////////////////////////////////////////////////////////////////////
// Water Breathing
/////////////////////////////////////////////////////////////////////////
class cEntityEffectWaterBreathing:
	public cEntityEffect
{
	typedef cEntityEffect super;
public:
	cEntityEffectWaterBreathing(int a_Duration, short a_Intensity, double a_DistanceModifier = 1):
		super(a_Duration, a_Intensity, a_DistanceModifier)
	{
	}
};

/////////////////////////////////////////////////////////////////////////
// Invisibility
/////////////////////////////////////////////////////////////////////////
class cEntityEffectInvisibility:
	public cEntityEffect
{
	typedef cEntityEffect super;
public:
	cEntityEffectInvisibility(int a_Duration, short a_Intensity, double a_DistanceModifier = 1):
		super(a_Duration, a_Intensity, a_DistanceModifier)
	{
	}
};

/////////////////////////////////////////////////////////////////////////
// Blindness
/////////////////////////////////////////////////////////////////////////
class cEntityEffectBlindness:
	public cEntityEffect
{
	typedef cEntityEffect super;
public:
	cEntityEffectBlindness(int a_Duration, short a_Intensity, double a_DistanceModifier = 1):
		super(a_Duration, a_Intensity, a_DistanceModifier)
	{
	}
};

/////////////////////////////////////////////////////////////////////////
// Night Vision
/////////////////////////////////////////////////////////////////////////
class cEntityEffectNightVision:
	public cEntityEffect
{
	typedef cEntityEffect super;
public:
	cEntityEffectNightVision(int a_Duration, short a_Intensity, double a_DistanceModifier = 1):
		super(a_Duration, a_Intensity, a_DistanceModifier)
	{
	}
};

/////////////////////////////////////////////////////////////////////////
// Hunger
/////////////////////////////////////////////////////////////////////////
class cEntityEffectHunger:
	public cEntityEffect
{
	typedef cEntityEffect super;
public:
	cEntityEffectHunger(int a_Duration, short a_Intensity, double a_DistanceModifier = 1):
		super(a_Duration, a_Intensity, a_DistanceModifier)
	{
	}
	
	virtual void OnTick(cPawn & a_Target) override;
};

/////////////////////////////////////////////////////////////////////////
// Weakness
/////////////////////////////////////////////////////////////////////////
class cEntityEffectWeakness:
	public cEntityEffect
{
	typedef cEntityEffect super;
public:
	cEntityEffectWeakness(int a_Duration, short a_Intensity, double a_DistanceModifier = 1):
		super(a_Duration, a_Intensity, a_DistanceModifier)
	{
	}
	
	virtual void OnTick(cPawn & a_Target) override;
};

/////////////////////////////////////////////////////////////////////////
// Poison
/////////////////////////////////////////////////////////////////////////
class cEntityEffectPoison:
	public cEntityEffect
{
	typedef cEntityEffect super;
public:
	cEntityEffectPoison(int a_Duration, short a_Intensity, double a_DistanceModifier = 1):
		super(a_Duration, a_Intensity, a_DistanceModifier)
	{
	}
	
	virtual void OnTick(cPawn & a_Target) override;
};

/////////////////////////////////////////////////////////////////////////
// Wither
/////////////////////////////////////////////////////////////////////////
class cEntityEffectWither:
	public cEntityEffect
{
	typedef cEntityEffect super;
public:
	cEntityEffectWither(int a_Duration, short a_Intensity, double a_DistanceModifier = 1):
		super(a_Duration, a_Intensity, a_DistanceModifier)
	{
	}
	
	virtual void OnTick(cPawn & a_Target) override;
};

/////////////////////////////////////////////////////////////////////////
// Health Boost
/////////////////////////////////////////////////////////////////////////
class cEntityEffectHealthBoost:
	public cEntityEffect
{
	typedef cEntityEffect super;
public:
	cEntityEffectHealthBoost(int a_Duration, short a_Intensity, double a_DistanceModifier = 1):
		super(a_Duration, a_Intensity, a_DistanceModifier)
	{
	}
};

/////////////////////////////////////////////////////////////////////////
// Absorption
/////////////////////////////////////////////////////////////////////////
class cEntityEffectAbsorption:
	public cEntityEffect
{
	typedef cEntityEffect super;
public:
	cEntityEffectAbsorption(int a_Duration, short a_Intensity, double a_DistanceModifier = 1):
		super(a_Duration, a_Intensity, a_DistanceModifier)
	{
	}
};

/////////////////////////////////////////////////////////////////////////
// Saturation
/////////////////////////////////////////////////////////////////////////
class cEntityEffectSaturation:
	public cEntityEffect
{
	typedef cEntityEffect super;
public:
	cEntityEffectSaturation(int a_Duration, short a_Intensity, double a_DistanceModifier = 1):
		super(a_Duration, a_Intensity, a_DistanceModifier)
	{
	}
	
	virtual void OnTick(cPawn & a_Target) override;
};



// tolua_end
