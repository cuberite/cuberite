
class cWorld;

class cBroadcaster
{

public:

	cBroadcaster(cWorld * a_World);

	void BroadcastParticleEffect(const AString & a_ParticleName, const Vector3f a_Src, const Vector3f a_Offset, float a_ParticleData, int a_ParticleAmount, cClientHandle * a_Exclude = nullptr);

	void BroadcastParticleEffect(const AString & a_ParticleName, const Vector3f a_Src, const Vector3f a_Offset, float a_ParticleData, int a_ParticleAmount, std::array<int, 2> a_Data, cClientHandle * a_Exclude = nullptr);

private:
	cWorld * m_World;

};
