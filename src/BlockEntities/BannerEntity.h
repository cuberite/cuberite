
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

	cBannerEntity(BlockState a_Block, Vector3i a_Pos, cWorld * a_World);
	cBannerEntity(BlockState a_Block, Vector3i a_Pos, cWorld * a_World, unsigned char a_BaseColor);

	unsigned char GetBaseColor() const;
	void SetBaseColor(unsigned char a_Color);

private:

	unsigned char m_BaseColor;

	// cBlockEntity overrides:
	virtual cItems ConvertToPickups() const override;
	virtual void CopyFrom(const cBlockEntity & a_Src) override;
	virtual void SendTo(cClientHandle & a_Client) override;
	virtual bool UsedBy(cPlayer * a_Player) override;
} ;  // tolua_export
