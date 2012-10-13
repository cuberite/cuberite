#include "Globals.h"
#include "Plugin_Squirrel.h"
#include "squirrelbindings/SquirrelFunctions.h"
#include "squirrelbindings/SquirrelBindings.h"
#include "squirrelbindings/SquirrelBaseClass.h"


cPlugin_Squirrel::cPlugin_Squirrel( const char* a_PluginName )
	: cPlugin( a_PluginName )
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
	
	if(!m_Plugin->HasFunction("OnDisable")) return;
	
	m_Plugin->GetFunction("OnDisable").Execute();
}





void cPlugin_Squirrel::Tick(float a_Dt)
{
	cCSLock Lock( m_CriticalSection );

	if(!m_Plugin->HasFunction("OnTick")) return;
	
	m_Plugin->GetFunction("OnTick").Execute(a_Dt);
}





bool cPlugin_Squirrel::OnCollectPickup(cPlayer * a_Player, cPickup * a_Pickup)
{
	cCSLock Lock(m_CriticalSection);

	if (!m_Plugin->HasFunction("OnCollectPickup"))
	{
		return false;
	}
	
	return m_Plugin->GetFunction("OnCollectPickup").Evaluate<bool>(a_Player, a_Pickup);
}





bool cPlugin_Squirrel::OnDisconnect(cPlayer* a_Player, const AString & a_Reason)
{
	cCSLock Lock( m_CriticalSection );
	
	if (!m_Plugin->HasFunction("OnDisconnect")) return false;
	
	return m_Plugin->GetFunction("OnDisconnect").Evaluate<bool>(a_Player, a_Reason);
}





bool cPlugin_Squirrel::OnBlockPlace(cPlayer * a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, const cItem & a_HeldItem)
{
	cCSLock Lock(m_CriticalSection);

	if (!m_Plugin->HasFunction("OnBlockPlace")) return false;
	
	return m_Plugin->GetFunction("OnBlockPlace").Evaluate<bool>(a_Player, a_BlockX, a_BlockY, a_BlockZ, a_BlockFace, a_HeldItem);
}





bool cPlugin_Squirrel::OnBlockDig(cPlayer * a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, char a_Status, BLOCKTYPE a_OldBlock, NIBBLETYPE a_OldMeta)
{
	cCSLock Lock(m_CriticalSection);

	if (!m_Plugin->HasFunction("OnBlockDig")) return false;

	return m_Plugin->GetFunction("OnBlockDig").Evaluate<bool>(a_Player, a_BlockX, a_BlockY, a_BlockZ, a_BlockFace, a_Status, a_OldBlock, a_OldMeta);
}





bool cPlugin_Squirrel::OnChat(cPlayer * a_Player, const AString & a_Message)
{
	cCSLock Lock(m_CriticalSection);

	if (!m_Plugin->HasFunction("OnChat")) return false;
	
	return m_Plugin->GetFunction("OnChat").Evaluate<bool>(a_Player, a_Message);

}





bool cPlugin_Squirrel::OnLogin(cClientHandle * a_Client, int a_ProtocolVersion, const AString & a_Username)
{
	cCSLock Lock( m_CriticalSection );

	if (!m_Plugin->HasFunction("OnLogin"))
	{
		return false;
	}
	
	return m_Plugin->GetFunction("OnLogin").Evaluate<bool>(a_Client, a_ProtocolVersion, a_Username);
}





void cPlugin_Squirrel::OnPlayerSpawn( cPlayer* a_Player )
{
	cCSLock Lock( m_CriticalSection );

	if(!m_Plugin->HasFunction("OnPlayerSpawn")) return;
	
	return m_Plugin->GetFunction("OnPlayerSpawn").Execute(a_Player);

}





bool cPlugin_Squirrel::OnPlayerJoin( cPlayer* a_Player )
{
	cCSLock Lock( m_CriticalSection );

	if(!m_Plugin->HasFunction("OnPlayerJoin")) return false;
	
	return m_Plugin->GetFunction("OnPlayerJoin").Evaluate<bool>(a_Player);
}





void cPlugin_Squirrel::OnPlayerMove( cPlayer* a_Player )
{
	cCSLock Lock( m_CriticalSection );

	if(!m_Plugin->HasFunction("OnPlayerMove")) return;
	
	return m_Plugin->GetFunction("OnPlayerMove").Execute(a_Player);

}





void cPlugin_Squirrel::OnTakeDamage( cPawn* a_Pawn, TakeDamageInfo* a_TakeDamageInfo )
{
	cCSLock Lock( m_CriticalSection );

	if(!m_Plugin->HasFunction("OnTakeDamage")) return;
	
	return m_Plugin->GetFunction("OnTakeDamage")(a_Pawn, a_TakeDamageInfo);
}





bool cPlugin_Squirrel::OnKilled( cPawn* a_Killed, cEntity* a_Killer )
{
	cCSLock Lock( m_CriticalSection );
	if(!m_Plugin->HasFunction("OnKilled")) return false;
	return m_Plugin->GetFunction("OnKilled").Evaluate<bool>(a_Killed, a_Killer);
}





void cPlugin_Squirrel::OnChunkGenerated(cWorld * a_World, int a_ChunkX, int a_ChunkZ)
{
	cCSLock Lock(m_CriticalSection);
	if(!m_Plugin->HasFunction("OnChunkGenerated")) return;
	return m_Plugin->GetFunction("OnChunkGenerated")(a_World, a_ChunkX, a_ChunkZ);
}





bool cPlugin_Squirrel::OnChunkGenerating(cWorld * a_World, int a_ChunkX, int a_ChunkZ, cLuaChunk * a_pLuaChunk)
{
	cCSLock Lock(m_CriticalSection);
	if(!m_Plugin->HasFunction("OnChunkGenerating")) return false;
	return m_Plugin->GetFunction("OnChunkGenerating").Evaluate<bool>(a_World, a_ChunkX, a_ChunkZ, a_pLuaChunk);
}





bool cPlugin_Squirrel::OnPreCrafting(const cPlayer * a_Player, const cCraftingGrid * a_Grid, cCraftingRecipe * a_Recipe)
{
	cCSLock Lock(m_CriticalSection);
	if(!m_Plugin->HasFunction("OnPreCrafting")) return false;
	return m_Plugin->GetFunction("OnPreCrafting").Evaluate<bool>((cPlayer *) a_Player, (cCraftingGrid *) a_Grid, a_Recipe);

}





bool cPlugin_Squirrel::OnCraftingNoRecipe(const cPlayer * a_Player, const cCraftingGrid * a_Grid, cCraftingRecipe * a_Recipe)
{
	cCSLock Lock(m_CriticalSection);
	if(!m_Plugin->HasFunction("OnCraftingNoRecipe")) return false;
	return m_Plugin->GetFunction("OnCraftingNoRecipe").Evaluate<bool>((cPlayer *) a_Player, (cCraftingGrid *) a_Grid, a_Recipe);
}





bool cPlugin_Squirrel::OnPostCrafting(const cPlayer * a_Player, const cCraftingGrid * a_Grid, cCraftingRecipe * a_Recipe)
{
	cCSLock Lock(m_CriticalSection);
	
	if(!m_Plugin->HasFunction("OnPostCrafting")) return false;
	return m_Plugin->GetFunction("OnPostCrafting").Evaluate<bool>((cPlayer *) a_Player, (cCraftingGrid *) a_Grid, a_Recipe);
}





bool cPlugin_Squirrel::OnBlockToPickup(
	BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, 
	const cPlayer * a_Player, const cItem & a_EquippedItem, cItems & a_Pickups
)
{
	cCSLock Lock(m_CriticalSection);

	if(!m_Plugin->HasFunction("OnBlockToPickup")) return false;
	return m_Plugin->GetFunction("OnBlockToPickup").Evaluate<bool>(a_BlockType, a_BlockMeta, (cPlayer *) a_Player, a_EquippedItem, a_Pickups);

}





bool cPlugin_Squirrel::OnWeatherChanged(cWorld * a_World)
{
	cCSLock Lock(m_CriticalSection);
	
	if(!m_Plugin->HasFunction("OnWeatherChanged")) return false;
	return m_Plugin->GetFunction("OnWeatherChanged").Evaluate<bool>(a_World);
}





bool cPlugin_Squirrel::OnUpdatingSign(
	cWorld * a_World, 
	int a_BlockX, int a_BlockY, int a_BlockZ, 
	AString & a_Line1, AString & a_Line2, AString & a_Line3, AString & a_Line4,
	cPlayer * a_Player
)
{
	cCSLock Lock(m_CriticalSection);
	
	if(!m_Plugin->HasFunction("OnUpdatingSign")) return false;
	return m_Plugin->GetFunction("OnUpdatingSign")
		.Evaluate<bool>(
			a_World,
			a_BlockX,
			a_BlockY,
			a_BlockZ,
			a_Line1,
			a_Line2,
			a_Line3,
			a_Line4,
			a_Player
		);
}





bool cPlugin_Squirrel::OnUpdatedSign(
	cWorld * a_World, 
	int a_BlockX, int a_BlockY, int a_BlockZ, 
	const AString & a_Line1, const AString & a_Line2, const AString & a_Line3, const AString & a_Line4,
	cPlayer * a_Player
)
{
	cCSLock Lock(m_CriticalSection);

	if(!m_Plugin->HasFunction("OnUpdatedSign")) return false;
	return m_Plugin->GetFunction("OnUpdatedSign")
		.Evaluate<bool>(
			a_World,
			a_BlockX,
			a_BlockY,
			a_BlockZ,
			a_Line1,
			a_Line2,
			a_Line3,
			a_Line4,
			a_Player
		);
}
