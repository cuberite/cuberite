#pragma once

#include "../FastRandom.h"

class Ageable {
public:
    Ageable() { cFastRandom Random; SetAge(Random.NextInt(10) == 0 ? -1: 1); }
    
    bool    IsBaby        (void) const { return m_Age < 0; }
    Byte    GetAge        (void) const { return m_Age; }
    
    void SetAge(Byte a_Age) {
        m_Age = a_Age;
    }
    
protected:
    Byte    m_Age;
} ;