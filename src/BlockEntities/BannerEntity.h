
// BannerEntity.h

// Declares the cBannerEntity class representing a single banner in the world





#pragma once

#include "BlockEntity.h"





// tolua_begin
class cBannerEntity :
	public cBlockEntity
{
	// tolua_end

	using Super = cBlockEntity;

public:

	cBannerEntity(BLOCKTYPE a_BlockType, BLOCKMETATYPE a_BlockMeta, Vector3i a_Pos, cWorld * a_World, unsigned char a_BaseColor = 1, AString a_CustomName = "");

	unsigned char GetBaseColor() const { return (m_BlockMeta >> 4) & 0x0f; }
	void SetBaseColor(unsigned char a_Color) { m_BlockMeta = (m_BlockMeta & (~0xf0)) | static_cast<BLOCKMETATYPE>(a_Color << 4); }

	const AString & GetCustomName() const { return m_CustomName; }
	void SetCustomName(const AString & a_CustomName) { m_CustomName = a_CustomName; }

private:

	AString m_CustomName;

	// cBlockEntity overrides:
	virtual cItems ConvertToPickups() const override;
	virtual void CopyFrom(const cBlockEntity & a_Src) override;
	virtual void SendTo(cClientHandle & a_Client) override;
	virtual bool UsedBy(cPlayer * a_Player) override;
} ;  // tolua_export
