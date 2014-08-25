#pragma once

class cMonster;
class cEntity;
class cChunk;

class cAttackComponent
{
protected:
	cMonster * m_Self;
	float m_AttackRate;
	int m_AttackDamage;
	int m_AttackRange;
	float m_AttackInterval;
public:
	cAttackComponent(cMonster * a_Entity);
	virtual ~cAttackComponent(){}
	
	virtual void Tick(float a_Dt, cChunk & a_Chunk){}

	// Get Functions
	int GetAttackRate() { return (int)m_AttackRate; }
	int GetAttackRange() { return m_AttackRange; }
	int GetAttackDamage() { return m_AttackDamage; }
	float GetAttackInterval() { return m_AttackInterval; }

	// Set Functions
	void SetAttackRate(float a_AttackRate) { m_AttackRate = a_AttackRate; }
	void SetAttackRange(int a_AttackRange) { m_AttackRange = a_AttackRange; }
	void SetAttackDamage(int a_AttackDamage) { m_AttackDamage = a_AttackDamage; }
	void SetAttackInterval(float a_AttackInterval) { m_AttackInterval = a_AttackInterval; }
};
