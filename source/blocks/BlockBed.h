#pragma once
#include "BlockHandler.h"
#include "../World.h"
#include "../Sign.h"
#include "../Player.h"

class cBlockBedHandler : public cBlockHandler
{
public:
	cBlockBedHandler(BLOCKTYPE a_BlockID)
		: cBlockHandler(a_BlockID)
	{

	}





	virtual void PlaceBlock(cWorld *a_World, cPlayer *a_Player, NIBBLETYPE a_BlockMeta, int a_X, int a_Y, int a_Z, char a_Dir) override
	{
		if( a_Dir != 1 ) // Can only be placed on the floor
			return;

		NIBBLETYPE Meta = RotationToMetaData( a_Player->GetRotation() );
		Vector3i Direction = MetaDataToDirection( Meta );

		if (a_World->GetBlock(a_X+Direction.x, a_Y, a_Z+Direction.z) != E_BLOCK_AIR)
		{
			return;
		}

		a_World->SetBlock(a_X,               a_Y, a_Z,               E_BLOCK_BED, Meta);
		a_World->SetBlock(a_X + Direction.x, a_Y, a_Z + Direction.z, E_BLOCK_BED, Meta | 0x8);

		OnPlacedByPlayer(a_World, a_Player, a_X, a_Y, a_Z, a_Dir);
	}





	virtual void OnDestroyed(cWorld *a_World, int a_X, int a_Y, int a_Z) override
	{
		char OldMeta = a_World->GetBlockMeta(a_X, a_Y, a_Z);

		Vector3i ThisPos( a_X, a_Y, a_Z );
		Vector3i Direction = MetaDataToDirection( OldMeta & 0x7 );
		if (OldMeta & 0x8)
		{
			// Was pillow
			if (a_World->GetBlock(ThisPos - Direction) == E_BLOCK_BED)
			{
				a_World->FastSetBlock(ThisPos - Direction, E_BLOCK_AIR, 0);
			}
		}
		else
		{
			// Was foot end
			if (a_World->GetBlock(ThisPos + Direction) == E_BLOCK_BED)
			{
				a_World->FastSetBlock(ThisPos + Direction, E_BLOCK_AIR, 0);
			}
		}
	}





	virtual int GetDropID() override
	{
		return E_ITEM_BED;
	}

	virtual NIBBLETYPE GetDropMeta(NIBBLETYPE a_BlockMeta) override
	{
		return 0;
	}





	virtual bool AllowBlockOnTop() override
	{
		return false;
	}





	static NIBBLETYPE RotationToMetaData( float a_Rotation )
	{
		a_Rotation += 180 + (180/4); // So its not aligned with axis
		if( a_Rotation > 360.f ) a_Rotation -= 360.f;

		a_Rotation = (a_Rotation/360) * 4;

		return ((char)a_Rotation+2) % 4;
	}





	static Vector3i MetaDataToDirection( NIBBLETYPE a_MetaData )
	{
		switch( a_MetaData )
		{
		case 0: // south  +z
			return Vector3i(0, 0, 1);
		case 1: // west   -x
			return Vector3i(-1, 0, 0);
		case 2: // north  -z
			return Vector3i(0, 0, -1);
		case 3: // east   +x
			return Vector3i(1, 0, 0);
		};
		return Vector3i();
	}
};
