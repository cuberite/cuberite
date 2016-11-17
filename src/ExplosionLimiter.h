class cExplosionLimiter
{
public:
	cExplosionLimiter();

	/** Set the number of allowed TNT explosions caused by other TNTS per second. */
	void SetExplosionsAllowedPerSecond(unsigned int a_ExplosionsAllowedPerSecond);

	/** Reset the explosion limit. Called once a second from cWorld. */
	void ResetExplosionLimit();

	/** Check whether a TNT is allowed to explode other TNTs.
	Returns false if explosion is denied and true if allowed. */
	bool RequestExplosion();

private:
	unsigned int m_ExplosionsAllowedThisSecond;
	unsigned int m_ExplosionsAllowedPerSecond;
};
