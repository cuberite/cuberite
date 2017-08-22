
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "PassiveMonster.h"
#include "../World.h"
#include "../Entities/Player.h"
#include "BoundingBox.h"




cPassiveMonster::cPassiveMonster(const AString & a_ConfigName, eMonsterType a_MobType, const AString & a_SoundHurt, const AString & a_SoundDeath, double a_Width, double a_Height) :
    super(a_ConfigName, a_MobType, a_SoundHurt, a_SoundDeath, a_Width, a_Height),
    m_BehaviorBreeder(this), m_BehaviorItemFollower(this), m_BehaviorCoward(this), m_BehaviorWanderer(this)
{
    m_EMPersonality = PASSIVE;
}





cPassiveMonster::~cPassiveMonster()
{

}





bool cPassiveMonster::DoTakeDamage(TakeDamageInfo & a_TDI)
{
    if (!super::DoTakeDamage(a_TDI))
#pragma once

#include "Monster.h"
#include "Behaviors/BehaviorBreeder.h"
#include "Behaviors/BehaviorItemFollower.h"
#include "Behaviors/BehaviorCoward.h"


typedef std::string AString;
class cPassiveMonster : public cMonster
{
    typedef cMonster super;

public:
    cPassiveMonster(const AString & a_ConfigName, eMonsterType a_MobType, const AString & a_SoundHurt, const AString & a_SoundDeath, double a_Width, double a_Height, cItems & a_BreedingItems, cItems & a_FollowedItems);
    virtual ~cPassiveMonster();
    virtual void Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk) override;
    virtual void OnRightClicked(cPlayer & a_Player) override;

    /** When hit by someone, run away */
    virtual bool DoTakeDamage(TakeDamageInfo & a_TDI) override;

    virtual void Destroyed(void) override;

private:
    cBehaviorBreeder m_BehaviorBreeder;
    cBehaviorItemFollower m_BehaviorItemFollower;
    cBehaviorCoward m_BehaviorCoward;
};

    {
        return false;
    }
    m_BehaviorCoward.DoTakeDamage(a_TDI);
    return true;
}





void cPassiveMonster::Destroyed()
{
    m_BehaviorBreeder.Destroyed();
    super::Destroyed();
}





cBehaviorBreeder * cPassiveMonster::GetBehaviorBreeder()
{
    return &m_BehaviorBreeder;
}





const cBehaviorBreeder * cPassiveMonster::GetBehaviorBreeder() const
{
    return static_cast<const cBehaviorBreeder *>(&m_BehaviorBreeder);
}


void cPassiveMonster::Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
    super::Tick(a_Dt, a_Chunk);

    for (;;)
    {
        if (m_BehaviorCoward.ActiveTick())
        {
            break;
        }
        if (m_BehaviorBreeder.ActiveTick())
        {
            break;
        }
        if (m_BehaviorItemFollower.ActiveTick())
        {
            break;
        }
        if (m_BehaviorWanderer.ActiveTick(a_Dt, a_Chunk))
        {
            break;
        }

        ASSERT(!"Not a single Behavior took control, this is not normal. ");
        break;
    }

    m_BehaviorBreeder.Tick();
}





void cPassiveMonster::OnRightClicked(cPlayer & a_Player)
{
    super::OnRightClicked(a_Player);
    m_BehaviorBreeder.OnRightClicked(a_Player);
}
