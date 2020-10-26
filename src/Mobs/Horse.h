
#pragma once

#include "PassiveMonster.h"
#include "../UI/WindowOwner.h"





class cHorse:
	public cPassiveMonster,
	public cEntityWindowOwner
{
	using Super = cPassiveMonster;

public:

	cHorse(int Type, int Color, int Style, int TameTimes);
	virtual ~cHorse() override;

	CLASS_PROTODEF(cHorse)

	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = nullptr) override;
	virtual void InStateIdle(std::chrono::milliseconds a_Dt, cChunk & a_Chunk) override;
	virtual void HandleSpeedFromAttachee(float a_Forward, float a_Sideways) override;
	virtual void Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk) override;
	virtual void OnRightClicked(cPlayer & a_Player) override;

	bool IsSaddled      (void) const  {return !m_Saddle.IsEmpty(); }
	bool IsChested      (void) const  {return m_bHasChest; }
	bool IsEating       (void) const  {return m_bIsEating; }
	bool IsRearing      (void) const  {return m_bIsRearing; }
	bool IsMthOpen      (void) const  {return m_bIsMouthOpen; }
	bool IsTame         (void) const override {return m_bIsTame; }
	int  GetHorseType   (void) const  {return m_Type; }
	int  GetHorseColor  (void) const  {return m_Color; }
	int  GetHorseStyle  (void) const  {return m_Style; }
	int  GetHorseArmour (void) const;

	/** Set the horse's saddle to the given item.
	@param a_SaddleItem should be either a saddle or empty. */
	void SetHorseSaddle(cItem a_SaddleItem);

	/** Set the horse's armor slot to the given item.
	@param a_SaddleItem should be either a type of horse armor or empty. */
	void SetHorseArmor(cItem a_ArmorItem);

	const cItem & GetHorseSaddle()    const { return m_Saddle; }
	const cItem & GetHorseArmorItem() const { return m_Armor;  }

	virtual void GetBreedingItems(cItems & a_Items) override
	{
		a_Items.Add(E_ITEM_GOLDEN_CARROT);
		a_Items.Add(E_ITEM_GOLDEN_APPLE);
	}

	void PlayerOpenWindow(cPlayer & a_Player);

private:

	bool m_bHasChest, m_bIsEating, m_bIsRearing, m_bIsMouthOpen, m_bIsTame;
	int m_Type, m_Color, m_Style, m_TimesToTame, m_TameAttemptTimes, m_RearTickCount;
	float m_MaxSpeed;
	cItem m_Saddle;
	cItem m_Armor;

} ;




