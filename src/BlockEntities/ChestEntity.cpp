
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "ChestEntity.h"
#include "../Item.h"
#include "../Entities/Player.h"
#include "../UI/ChestWindow.h"





cChestEntity::cChestEntity(int a_BlockX, int a_BlockY, int a_BlockZ, cWorld * a_World, BLOCKTYPE a_Type) :
	super(a_Type, a_BlockX, a_BlockY, a_BlockZ, ContentsWidth, ContentsHeight, a_World),
	m_NumActivePlayers(0)
{
}





cChestEntity::~cChestEntity()
{
	cWindow * Window = GetWindow();
	if (Window != nullptr)
	{
		Window->OwnerDestroyed();
	}
}





void cChestEntity::SendTo(cClientHandle & a_Client)
{
	// The chest entity doesn't need anything sent to the client when it's created / gets in the viewdistance
	// All the actual handling is in the cWindow UI code that gets called when the chest is rclked

	UNUSED(a_Client);
}





void cChestEntity::UsedBy(cPlayer * a_Player)
{
	// If the window is not created, open it anew:
	cWindow * Window = GetWindow();
	if (Window == nullptr)
	{
		OpenNewWindow();
		Window = GetWindow();
	}

	// Open the window for the player:
	if (Window != nullptr)
	{
		if (a_Player->GetWindow() != Window)
		{
			a_Player->OpenWindow(Window);
		}
	}

	// This is rather a hack
	// Instead of marking the chunk as dirty upon chest contents change, we mark it dirty now
	// We cannot properly detect contents change, but such a change doesn't happen without a player opening the chest first.
	// The few false positives aren't much to worry about
	int ChunkX, ChunkZ;
	cChunkDef::BlockToChunk(m_PosX, m_PosZ, ChunkX, ChunkZ);
	m_World->MarkChunkDirty(ChunkX, ChunkZ, true);
}





void cChestEntity::OpenNewWindow(void)
{
	// TODO: cats are an obstruction
	if ((GetPosY() < cChunkDef::Height - 1) && !cBlockInfo::IsTransparent(GetWorld()->GetBlock(GetPosX(), GetPosY() + 1, GetPosZ())))
	{
		// Obstruction, don't open
		return;
	}

	// Callback for opening together with neighbor chest:
	class cOpenDouble :
		public cChestCallback
	{
		cChestEntity * m_ThisChest;
	public:
		cOpenDouble(cChestEntity * a_ThisChest) :
			m_ThisChest(a_ThisChest)
		{
		}

		virtual bool Item(cChestEntity * a_Chest) override
		{
			if ((a_Chest->GetPosY() < cChunkDef::Height - 1) && !cBlockInfo::IsTransparent(a_Chest->GetWorld()->GetBlock(a_Chest->GetPosX(), a_Chest->GetPosY() + 1, a_Chest->GetPosZ())))
			{
				// Obstruction, don't open
				return false;
			}

			// The primary chest should eb the one with lesser X or Z coord:
			cChestEntity * Primary = a_Chest;
			cChestEntity * Secondary = m_ThisChest;
			if (
				(Primary->GetPosX() > Secondary->GetPosX()) ||
				(Primary->GetPosZ() > Secondary->GetPosZ())
			)
			{
				std::swap(Primary, Secondary);
			}
			m_ThisChest->OpenWindow(new cChestWindow(Primary, Secondary));
			return false;
		}
	} ;

	// Scan neighbors for adjacent chests:
	cOpenDouble OpenDbl(this);
	if (
		m_World->DoWithChestAt(m_PosX - 1, m_PosY, m_PosZ,     OpenDbl) ||
		m_World->DoWithChestAt(m_PosX + 1, m_PosY, m_PosZ,     OpenDbl) ||
		m_World->DoWithChestAt(m_PosX,     m_PosY, m_PosZ - 1, OpenDbl) ||
		m_World->DoWithChestAt(m_PosX,     m_PosY, m_PosZ + 1, OpenDbl)
	)
	{
		// The double-chest window has been opened in the callback
		return;
	}

	// There is no chest neighbor, open a single-chest window:
	OpenWindow(new cChestWindow(this));
}
