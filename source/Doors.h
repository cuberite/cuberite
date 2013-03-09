
#pragma once





// tolua_begin
class cDoors
{
public:
	static char RotationToMetaData(double a_Rotation)
	{
		a_Rotation += 90 + 45; // So its not aligned with axis
		if (a_Rotation > 360)
		{
			a_Rotation -= 360;
		}
		if (a_Rotation >= 0.f && a_Rotation < 90)
		{
			return 0x0;
		}
		else if ((a_Rotation >= 180) && (a_Rotation < 270))
		{
			return 0x2;
		}
		else if ((a_Rotation >= 90) && (a_Rotation < 180))
		{
			return 0x1;
		}
		else
		{
			return 0x3;
		}
	}


	static NIBBLETYPE ChangeStateMetaData(NIBBLETYPE a_MetaData)
	{

		a_MetaData ^= 4;		//XOR bit 2 aka 3. bit (Door open state)

		return a_MetaData;
	}
	

	static void ChangeDoor(cWorld * a_World, int a_X, int a_Y, int a_Z)
	{
		NIBBLETYPE OldMetaData = a_World->GetBlockMeta(a_X, a_Y, a_Z);
		
		a_World->SetBlockMeta(a_X, a_Y, a_Z, ChangeStateMetaData(OldMetaData));
		
		if (OldMetaData & 8)
		{
			// Current block is top of the door
			BLOCKTYPE  BottomBlock = a_World->GetBlock(a_X, a_Y - 1, a_Z);
			NIBBLETYPE BottomMeta  = a_World->GetBlockMeta(a_X, a_Y - 1, a_Z);

			if (IsDoor(BottomBlock) && !(BottomMeta & 8))
			{
				a_World->SetBlockMeta(a_X, a_Y - 1, a_Z, ChangeStateMetaData(BottomMeta));
			}
		}
		else
		{
			// Current block is bottom of the door
			BLOCKTYPE  TopBlock = a_World->GetBlock(a_X, a_Y + 1, a_Z);
			NIBBLETYPE TopMeta = a_World->GetBlockMeta(a_X, a_Y + 1, a_Z);

			if (IsDoor(TopBlock) && (TopMeta & 8))
			{
				a_World->SetBlockMeta(a_X, a_Y + 1, a_Z, ChangeStateMetaData(TopMeta));
			}
		}
	}
	
	
	inline static bool IsDoor(BLOCKTYPE a_Block)
	{
		return (a_Block == E_BLOCK_WOODEN_DOOR) || (a_Block == E_BLOCK_IRON_DOOR);
	}
} ;
// tolua_end




