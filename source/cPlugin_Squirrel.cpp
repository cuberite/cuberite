#include "Globals.h"
#include "cPlugin_Squirrel.h"
#include "squirrelbindings/SquirrelFunctions.h"
#include "squirrelbindings/SquirrelBindings.h"
#include "squirrelbindings/cSquirrelBaseClass.h"


cPlugin_Squirrel::cPlugin_Squirrel( const char* a_PluginName )
{
	SetLanguage( cPlugin::E_SQUIRREL );
	m_PluginName = a_PluginName;
}

cPlugin_Squirrel::~cPlugin_Squirrel()
{
	delete m_Plugin;
}

bool cPlugin_Squirrel::Initialize()
{
	cCSLock Lock(m_CriticalSection);

	std::string PluginPath = std::string("Plugins/") + m_PluginName + ".nut";

	Sqrat::Script script;
	script.CompileFile(PluginPath);
	if(script.IsNull())
	{
		LOGERROR("Unable to run script \"%s\"", m_PluginName);
	}

	try {
		script.Run();

		Sqrat::Function construct(Sqrat::RootTable(), m_PluginName);
		
		if(construct.IsNull())
		{
			LOGERROR("Constructor for Plugin \"%s\" not found.", m_PluginName);
			return false;
		}

		Sqrat::Object obj = construct.Evaluate<Sqrat::Object>();
		
		((cSquirrelBaseClass *) obj.GetInstanceUP())->setInstance(this);

		m_Plugin = new SquirrelObject(obj);
		

		Sqrat::Object PluginName = obj.GetSlot("name");
		if(!PluginName.IsNull())
		{
			this->SetName(PluginName.Cast<const char*>());
		}

		Sqrat::Function init = m_Plugin->GetFunction("Initialize");
		if(init.IsNull())
		{
			LOGERROR("Can not initialize plugin \"%s\"", m_PluginName);
			return false;
		}

		return init.Evaluate<bool>();
		
	} catch(Sqrat::Exception &e)
	{
		LOGERROR("Initialisation of \"%s\" failed: %s", m_PluginName, e.Message().c_str());
		return false;
	}
}





void cPlugin_Squirrel::OnDisable()
{
	cCSLock Lock(m_CriticalSection);
	
	m_Plugin->GetFunction("OnDisable").Execute();
}





void cPlugin_Squirrel::Tick(float a_Dt)
{
	cCSLock Lock( m_CriticalSection );
	
	m_Plugin->GetFunction("OnTick").Execute(a_Dt);
}





bool cPlugin_Squirrel::OnCollectItem( cPickup* a_Pickup, cPlayer* a_Player )
{
	cCSLock Lock( m_CriticalSection );
	
	return m_Plugin->GetFunction("OnCollectItem").Evaluate<bool>(a_Pickup, a_Player);
}





bool cPlugin_Squirrel::OnDisconnect(const AString & a_Reason, cPlayer* a_Player )
{
	cCSLock Lock( m_CriticalSection );
	
	
	return m_Plugin->GetFunction("OnDisconnect").Evaluate<bool>(a_Reason, a_Player);
}





bool cPlugin_Squirrel::OnBlockPlace( cPacket_BlockPlace* a_PacketData, cPlayer* a_Player )
{
	cCSLock Lock( m_CriticalSection );
	
	return m_Plugin->GetFunction("OnBlockPlace").Evaluate<bool>(a_PacketData, a_Player);
}





bool cPlugin_Squirrel::OnBlockDig( cPacket_BlockDig* a_PacketData, cPlayer* a_Player, cItem* a_PickupItem )
{
	cCSLock Lock( m_CriticalSection );

	return m_Plugin->GetFunction("OnBlockDig").Evaluate<bool>(a_PacketData, a_Player, a_PickupItem);
}





bool cPlugin_Squirrel::OnChat( const char* a_Chat, cPlayer* a_Player )
{
	cCSLock Lock( m_CriticalSection );
	
	return m_Plugin->GetFunction("OnChat").Evaluate<bool>(a_Chat, a_Player);

}





bool cPlugin_Squirrel::OnLogin( cPacket_Login* a_PacketData )
{
	cCSLock Lock( m_CriticalSection );
	
	return m_Plugin->GetFunction("OnLogin").Evaluate<bool>(a_PacketData);
}





void cPlugin_Squirrel::OnPlayerSpawn( cPlayer* a_Player )
{
	cCSLock Lock( m_CriticalSection );
	
	return m_Plugin->GetFunction("OnPlayerSpawn").Execute(a_Player);

}





bool cPlugin_Squirrel::OnPlayerJoin( cPlayer* a_Player )
{
	cCSLock Lock( m_CriticalSection );
	
	return m_Plugin->GetFunction("OnPlayerJoin").Evaluate<bool>(a_Player);
}





void cPlugin_Squirrel::OnPlayerMove( cPlayer* a_Player )
{
	cCSLock Lock( m_CriticalSection );
	
	return m_Plugin->GetFunction("OnPlayerMove").Execute(a_Player);

}





void cPlugin_Squirrel::OnTakeDamage( cPawn* a_Pawn, TakeDamageInfo* a_TakeDamageInfo )
{
	cCSLock Lock( m_CriticalSection );
	
	return m_Plugin->GetFunction("OnTakeDamage")(a_Pawn, a_TakeDamageInfo);
}





bool cPlugin_Squirrel::OnKilled( cPawn* a_Killed, cEntity* a_Killer )
{
	cCSLock Lock( m_CriticalSection );
	
	return m_Plugin->GetFunction("OnKilled").Evaluate<bool>(a_Killed, a_Killer);
}





void cPlugin_Squirrel::OnChunkGenerated(cWorld * a_World, int a_ChunkX, int a_ChunkZ)
{
	cCSLock Lock(m_CriticalSection);
	
	return m_Plugin->GetFunction("OnChunkGenerated")(a_World, a_ChunkX, a_ChunkZ);
}





bool cPlugin_Squirrel::OnChunkGenerating(cWorld * a_World, int a_ChunkX, int a_ChunkZ, cLuaChunk * a_pLuaChunk)
{
	cCSLock Lock(m_CriticalSection);
	
	return m_Plugin->GetFunction("OnChunkGenerating").Evaluate<bool>(a_World, a_ChunkX, a_ChunkZ, a_pLuaChunk);
}





bool cPlugin_Squirrel::OnPreCrafting(const cPlayer * a_Player, const cCraftingGrid * a_Grid, cCraftingRecipe * a_Recipe)
{
	cCSLock Lock(m_CriticalSection);
	
	return m_Plugin->GetFunction("OnPreCrafting").Evaluate<bool>((cPlayer *) a_Player, (cCraftingGrid *) a_Grid, a_Recipe);

}





bool cPlugin_Squirrel::OnCraftingNoRecipe(const cPlayer * a_Player, const cCraftingGrid * a_Grid, cCraftingRecipe * a_Recipe)
{
	cCSLock Lock(m_CriticalSection);
	
	return m_Plugin->GetFunction("OnCraftingNoRecipe").Evaluate<bool>((cPlayer *) a_Player, (cCraftingGrid *) a_Grid, a_Recipe);
}





bool cPlugin_Squirrel::OnPostCrafting(const cPlayer * a_Player, const cCraftingGrid * a_Grid, cCraftingRecipe * a_Recipe)
{
	cCSLock Lock(m_CriticalSection);
	
	
	return m_Plugin->GetFunction("OnPostCrafting").Evaluate<bool>((cPlayer *) a_Player, (cCraftingGrid *) a_Grid, a_Recipe);
}





bool cPlugin_Squirrel::OnBlockToPickup(
	BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, 
	const cPlayer * a_Player, const cItem & a_EquippedItem, cItems & a_Pickups
)
{
	cCSLock Lock(m_CriticalSection);

	
	return m_Plugin->GetFunction("OnBlockToPickup").Evaluate<bool>(a_BlockType, a_BlockMeta, (cPlayer *) a_Player, a_EquippedItem, a_Pickups);

}





bool cPlugin_Squirrel::OnWeatherChanged(cWorld * a_World)
{
	cCSLock Lock(m_CriticalSection);
	
	
	return m_Plugin->GetFunction("OnWeatherChanged").Evaluate<bool>(a_World);
}





bool cPlugin_Squirrel::OnUpdatingSign(
	cWorld * a_World, 
	int a_BlockX, int a_BlockY, int a_BlockZ, 
	AString & a_Line1, AString & a_Line2, AString & a_Line3, AString & a_Line4
)
{
	cCSLock Lock(m_CriticalSection);
	
	
	return m_Plugin->GetFunction("OnUpdatingSign")
		.Evaluate<bool>(
			a_World,
			a_BlockX,
			a_BlockY,
			a_BlockZ,
			a_Line1,
			a_Line2,
			a_Line3,
			a_Line4
		);
}





bool cPlugin_Squirrel::OnUpdatedSign(
	cWorld * a_World, 
	int a_BlockX, int a_BlockY, int a_BlockZ, 
	const AString & a_Line1, const AString & a_Line2, const AString & a_Line3, const AString & a_Line4
)
{
	return m_Plugin->GetFunction("OnUpdatedSign")
		.Evaluate<bool>(
			a_World,
			a_BlockX,
			a_BlockY,
			a_BlockZ,
			a_Line1,
			a_Line2,
			a_Line3,
			a_Line4
		);
}
