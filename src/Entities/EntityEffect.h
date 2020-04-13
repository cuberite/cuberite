#pragma once

class cPawn;

// tolua_begin
class cEntityEffect
{
public:

	/** All types of entity effects (numbers correspond to protocol / storage types) */
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

	/** Returns the potion color (used by the client for visuals), based on the potion's damage value */
	static int GetPotionColor(short a_ItemDamage);


	/** Translates the potion's damage value into the entity effect that the potion gives */
	static cEntityEffect::eType GetPotionEffectType(short a_ItemDamage);


	/** Retrieves the intensity level from the potion's damage value. Returns 0 for level I potions, 1 for level II potions. */
	static short GetPotionEffectIntensity(short a_ItemDamage);


	/** Returns the effect duration, in ticks, based on the potion's damage value */
	static int GetPotionEffectDuration(short a_ItemDamage);

	/** Returns true if the potion with the given damage is drinkable */
	static bool IsPotionDrinkable(short a_ItemDamage);

	// tolua_end

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
	cEntityEffect & operator =(cEntityEffect a_OtherEffect);

	virtual ~cEntityEffect(void) {}

	/** Creates a pointer to the proper entity effect from the effect type
	@warning This function creates raw pointers that must be manually managed.
	@param a_EffectType       The effect type to create the effect from
	@param a_Duration         How long this effect will last, in ticks
	@param a_Intensity        How strong the effect will be applied
	@param a_DistanceModifier The distance modifier for affecting potency, defaults to 1 */
	static std::unique_ptr<cEntityEffect> CreateEntityEffect(cEntityEffect::eType a_EffectType, int a_Duration, short a_Intensity, double a_DistanceModifier);

	/** Returns how many ticks this effect has been active for */
	int GetTicks(void) const { return m_Ticks; }

	/** Returns the duration of the effect */
	int GetDuration(void) const { return m_Duration; }

	/** Returns how strong the effect will be applied */
	short GetIntensity(void) const { return m_Intensity; }

	/** Returns the distance modifier for affecting potency */
	double GetDistanceModifier(void) const { return m_DistanceModifier; }

	void SetTicks(int a_Ticks)                          { m_Ticks            = a_Ticks;            }
	void SetDuration(int a_Duration)                    { m_Duration         = a_Duration;         }
	void SetIntensity(short a_Intensity)                { m_Intensity        = a_Intensity;        }
	void SetDistanceModifier(double a_DistanceModifier) { m_DistanceModifier = a_DistanceModifier; }

	/** Called on each tick.
	By default increases the m_Ticks, descendants may override to provide additional processing. */
	virtual void OnTick(cPawn & a_Target);

	/** Called when the effect is first added to an entity */
	virtual void OnActivate(cPawn & a_Target) { }

	/** Called when the effect is removed from an entity */
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
};  // tolua_export





class cEntityEffectSpeed:
	public cEntityEffect
{
	using Super = cEntityEffect;

public:

	cEntityEffectSpeed(int a_Duration, short a_Intensity, double a_DistanceModifier = 1):
		Super(a_Duration, a_Intensity, a_DistanceModifier)
	{
	}

	virtual void OnActivate(cPawn & a_Target) override;

	virtual void OnDeactivate(cPawn & a_Target) override;
};





class cEntityEffectSlowness:
	public cEntityEffect
{
	using Super = cEntityEffect;

public:

	cEntityEffectSlowness(int a_Duration, short a_Intensity, double a_DistanceModifier = 1):
		Super(a_Duration, a_Intensity, a_DistanceModifier)
	{
	}

	virtual void OnActivate(cPawn & a_Target) override;

	virtual void OnDeactivate(cPawn & a_Target) override;
};





class cEntityEffectHaste:
	public cEntityEffect
{
	using Super = cEntityEffect;

public:

	cEntityEffectHaste(int a_Duration, short a_Intensity, double a_DistanceModifier = 1):
		Super(a_Duration, a_Intensity, a_DistanceModifier)
	{
	}
};





class cEntityEffectMiningFatigue:
	public cEntityEffect
{
	using Super = cEntityEffect;

public:

	cEntityEffectMiningFatigue(int a_Duration, short a_Intensity, double a_DistanceModifier = 1):
		Super(a_Duration, a_Intensity, a_DistanceModifier)
	{
	}
};





class cEntityEffectStrength:
	public cEntityEffect
{
	using Super = cEntityEffect;

public:

	cEntityEffectStrength(int a_Duration, short a_Intensity, double a_DistanceModifier = 1):
		Super(a_Duration, a_Intensity, a_DistanceModifier)
	{
	}
};





class cEntityEffectInstantHealth:
	public cEntityEffect
{
	using Super = cEntityEffect;

public:

	cEntityEffectInstantHealth(int a_Duration, short a_Intensity, double a_DistanceModifier = 1):
		Super(a_Duration, a_Intensity, a_DistanceModifier)
	{
	}

	virtual void OnActivate(cPawn & a_Target) override;
};





class cEntityEffectInstantDamage:
	public cEntityEffect
{
	using Super = cEntityEffect;

public:

	cEntityEffectInstantDamage(int a_Duration, short a_Intensity, double a_DistanceModifier = 1):
		Super(a_Duration, a_Intensity, a_DistanceModifier)
	{
	}

	virtual void OnActivate(cPawn & a_Target) override;
};





class cEntityEffectJumpBoost:
	public cEntityEffect
{
	using Super = cEntityEffect;

public:

	cEntityEffectJumpBoost(int a_Duration, short a_Intensity, double a_DistanceModifier = 1):
		Super(a_Duration, a_Intensity, a_DistanceModifier)
	{
	}
};





class cEntityEffectNausea:
	public cEntityEffect
{
	using Super = cEntityEffect;

public:

	cEntityEffectNausea(int a_Duration, short a_Intensity, double a_DistanceModifier = 1):
		Super(a_Duration, a_Intensity, a_DistanceModifier)
	{
	}
};





class cEntityEffectRegeneration:
	public cEntityEffect
{
	using Super = cEntityEffect;

public:

	cEntityEffectRegeneration(int a_Duration, short a_Intensity, double a_DistanceModifier = 1):
		Super(a_Duration, a_Intensity, a_DistanceModifier)
	{
	}

	virtual void OnTick(cPawn & a_Target) override;
};





class cEntityEffectResistance:
	public cEntityEffect
{
	using Super = cEntityEffect;

public:

	cEntityEffectResistance(int a_Duration, short a_Intensity, double a_DistanceModifier = 1):
		Super(a_Duration, a_Intensity, a_DistanceModifier)
	{
	}
};





class cEntityEffectFireResistance:
	public cEntityEffect
{
	using Super = cEntityEffect;

public:

	cEntityEffectFireResistance(int a_Duration, short a_Intensity, double a_DistanceModifier = 1):
		Super(a_Duration, a_Intensity, a_DistanceModifier)
	{
	}
};





class cEntityEffectWaterBreathing:
	public cEntityEffect
{
	using Super = cEntityEffect;

public:

	cEntityEffectWaterBreathing(int a_Duration, short a_Intensity, double a_DistanceModifier = 1):
		Super(a_Duration, a_Intensity, a_DistanceModifier)
	{
	}
};





class cEntityEffectInvisibility:
	public cEntityEffect
{
	using Super = cEntityEffect;

public:

	cEntityEffectInvisibility(int a_Duration, short a_Intensity, double a_DistanceModifier = 1):
		Super(a_Duration, a_Intensity, a_DistanceModifier)
	{
	}

	virtual void OnActivate  (cPawn & a_Target) override { BroadcastMetadata(a_Target); }
	virtual void OnDeactivate(cPawn & a_Target) override { BroadcastMetadata(a_Target); }

private:
	static void BroadcastMetadata(cPawn & a_Target);
};





class cEntityEffectBlindness:
	public cEntityEffect
{
	using Super = cEntityEffect;

public:

	cEntityEffectBlindness(int a_Duration, short a_Intensity, double a_DistanceModifier = 1):
		Super(a_Duration, a_Intensity, a_DistanceModifier)
	{
	}
};





class cEntityEffectNightVision:
	public cEntityEffect
{
	using Super = cEntityEffect;

public:

	cEntityEffectNightVision(int a_Duration, short a_Intensity, double a_DistanceModifier = 1):
		Super(a_Duration, a_Intensity, a_DistanceModifier)
	{
	}
};





class cEntityEffectHunger:
	public cEntityEffect
{
	using Super = cEntityEffect;

public:

	cEntityEffectHunger(int a_Duration, short a_Intensity, double a_DistanceModifier = 1):
		Super(a_Duration, a_Intensity, a_DistanceModifier)
	{
	}

	// cEntityEffect overrides:
	virtual void OnTick(cPawn & a_Target) override;
};





class cEntityEffectWeakness:
	public cEntityEffect
{
	using Super = cEntityEffect;

public:

	cEntityEffectWeakness(int a_Duration, short a_Intensity, double a_DistanceModifier = 1):
		Super(a_Duration, a_Intensity, a_DistanceModifier)
	{
	}

	// cEntityEffect overrides:
	virtual void OnTick(cPawn & a_Target) override;
};





class cEntityEffectPoison:
	public cEntityEffect
{
	using Super = cEntityEffect;

public:

	cEntityEffectPoison(int a_Duration, short a_Intensity, double a_DistanceModifier = 1):
		Super(a_Duration, a_Intensity, a_DistanceModifier)
	{
	}

	// cEntityEffect overrides:
	virtual void OnTick(cPawn & a_Target) override;
};





class cEntityEffectWither:
	public cEntityEffect
{
	using Super = cEntityEffect;

public:

	cEntityEffectWither(int a_Duration, short a_Intensity, double a_DistanceModifier = 1):
		Super(a_Duration, a_Intensity, a_DistanceModifier)
	{
	}

	// cEntityEffect overrides:
	virtual void OnTick(cPawn & a_Target) override;
};





class cEntityEffectHealthBoost:
	public cEntityEffect
{
	using Super = cEntityEffect;

public:

	cEntityEffectHealthBoost(int a_Duration, short a_Intensity, double a_DistanceModifier = 1):
		Super(a_Duration, a_Intensity, a_DistanceModifier)
	{
	}
};





class cEntityEffectAbsorption:
	public cEntityEffect
{
	using Super = cEntityEffect;

public:

	cEntityEffectAbsorption(int a_Duration, short a_Intensity, double a_DistanceModifier = 1):
		Super(a_Duration, a_Intensity, a_DistanceModifier)
	{
	}
};





class cEntityEffectSaturation:
	public cEntityEffect
{
	using Super = cEntityEffect;

public:

	cEntityEffectSaturation(int a_Duration, short a_Intensity, double a_DistanceModifier = 1):
		Super(a_Duration, a_Intensity, a_DistanceModifier)
	{
	}

	virtual void OnTick(cPawn & a_Target) override;
};




