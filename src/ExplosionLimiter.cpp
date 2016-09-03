#include "ExplosionLimiter.h"
cExplosionLimiter::cExplosionLimiter()
	: m_ExplosionsAllowedPerSecond()
{

}

void cExplosionLimiter::SetExplosionsAllowedPerSecond(unsigned int a_ExplosionsAllowedPerSecond)
{
	m_ExplosionsAllowedPerSecond = a_ExplosionsAllowedPerSecond;
}





void cExplosionLimiter::ResetExplosionLimit()
{
	m_ExplosionsAllowedThisSecond = m_ExplosionsAllowedPerSecond;
}





bool cExplosionLimiter::RequestExplosion()
{
	if (m_ExplosionsAllowedThisSecond > 0)
	{
		m_ExplosionsAllowedThisSecond -= 1;
		return true;
	}
	return false;
}
