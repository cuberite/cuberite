
#pragma once

#include "PassiveMonster.h"





class cHorse :
    public cPassiveMonster
{
    typedef cPassiveMonster super;

public:
    cHorse(int Type, int Color, int Style, int TameTimes);

    CLASS_PROTODEF(cHorse)

    virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = nullptr) override;
    virtual void HandleSpeedFromAttachee(float a_Forward, float a_Sideways) override;
    virtual void Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk) override;
    virtual void OnRightClicked(cPlayer & a_Player) override;

    bool IsSaddled      (void) const  {return m_bIsSaddled; }
    bool IsChested      (void) const  {return m_bHasChest; }
    bool IsEating       (void) const  {return m_bIsEating; }
    bool IsRearing      (void) const  {return m_bIsRearing; }
    bool IsMthOpen      (void) const  {return m_bIsMouthOpen; }
    bool IsTame         (void) const override {return m_bIsTame; }
    int  GetHorseType   (void) const  {return m_Type; }
    int  GetHorseColor  (void) const  {return m_Color; }
    int  GetHorseStyle  (void) const  {return m_Style; }
    int  GetHorseArmour (void) const  {return m_Armour;}

    virtual void GetBreedingItems(cItems & a_Items) override
    {
        a_Items.Add(E_ITEM_GOLDEN_CARROT);
        a_Items.Add(E_ITEM_GOLDEN_APPLE);
    }

private:

    bool m_bHasChest, m_bIsEating, m_bIsRearing, m_bIsMouthOpen, m_bIsTame, m_bIsSaddled;
    int m_Type, m_Color, m_Style, m_Armour, m_TimesToTame, m_TameAttemptTimes, m_RearTickCount;
    float m_MaxSpeed;

} ;




