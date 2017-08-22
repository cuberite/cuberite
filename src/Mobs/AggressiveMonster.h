#pragma once

#include "Monster.h"
#include "Behaviors/BehaviorAggressive.h"


typedef std::string AString;

class cAggressiveMonster :
    public cMonster
{
    typedef cMonster super;

public:

    cAggressiveMonster(const AString & a_ConfigName, eMonsterType a_MobType, const AString & a_SoundHurt, const AString & a_SoundDeath, double a_Width, double a_Height, int a_AggressionLightLevel);

    virtual void Tick          (std::chrono::milliseconds a_Dt, cChunk & a_Chunk) override;

private:
    cBehaviorAggressive m_BehaviorAggressive;
} ;
