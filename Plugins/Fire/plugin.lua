local FireBlocks = {} -- List of blocks that are currently burning!

function Initialize( Plugin )
	
	Plugin:SetName( "Fire" )
	Plugin:SetVersion( 2 )
	
	PluginManager = cRoot:Get():GetPluginManager()
	PluginManager:AddHook( Plugin, cPluginManager.E_PLUGIN_TICK )
	PluginManager:AddHook( Plugin, cPluginManager.E_PLUGIN_BLOCK_PLACE )
	PluginManager:AddHook( Plugin, cPluginManager.E_PLUGIN_BLOCK_DIG )

	Log( "Initialized " .. Plugin:GetName() .. " v." .. Plugin:GetVersion() )
	
	return true
end

function IsForeverBurnable( BlockID )
	if( BlockID == E_BLOCK_BLOODSTONE ) then
		return true
	end
	return false
end

function IsBurnable( BlockID )
	if( BlockID == E_BLOCK_LEAVES or BlockID == E_BLOCK_LOG ) then
		return true
	end

	return false
end

function FindBurnableAround( X, Y, Z )
	World = cRoot:Get():GetWorld()

	ListBurnables = {}
	if( IsBurnable( World:GetBlock(X-1, Y, Z) ) ) then
		table.insert( ListBurnables, { ["x"] = X-1, ["y"] = Y, ["z"] = Z } )
	end
	if( IsBurnable( World:GetBlock(X+1, Y, Z) ) ) then
		table.insert( ListBurnables, { ["x"] = X+1, ["y"] = Y, ["z"] = Z } )
	end
	if( IsBurnable( World:GetBlock(X, Y-1, Z) ) ) then
		table.insert( ListBurnables, { ["x"] = X, ["y"] = Y-1, ["z"] = Z } )
	end
	if( IsBurnable( World:GetBlock(X, Y+1, Z) ) ) then
		table.insert( ListBurnables, { ["x"] = X, ["y"] = Y+1, ["z"] = Z } )
	end
	if( IsBurnable( World:GetBlock(X, Y, Z-1) ) ) then
		table.insert( ListBurnables, { ["x"] = X, ["y"] = Y, ["z"] = Z-1 } )
	end
	if( IsBurnable( World:GetBlock(X, Y, Z+1) ) ) then
		table.insert( ListBurnables, { ["x"] = X, ["y"] = Y, ["z"] = Z+1 } )
	end

	return ListBurnables
end


function OnBlockPlace( PacketData, Player )

	if( PacketData.m_ItemType == E_BLOCK_FIRE or PacketData.m_ItemType == E_ITEM_FLINT_AND_STEEL ) then
		if( PacketData.m_Direction > -1 ) then
			local X = PacketData.m_PosX
			local Y = PacketData.m_PosY
			local Z = PacketData.m_PosZ

			X, Y, Z = AddDirection( X, Y, Z, PacketData.m_Direction )
			local World = cRoot:Get():GetWorld()
			
			--Since flint and steel doesn't do anything on the server side yet
			if( PacketData.m_ItemType == E_ITEM_FLINT_AND_STEEL ) then
				World:SetBlock( X, Y, Z, E_BLOCK_FIRE, 0 )
			end

			if( not IsForeverBurnable( World:GetBlock( X, Y-1, Z ) ) ) then
				table.insert( FireBlocks, { ["x"] = X, ["y"] = Y, ["z"] = Z } )
			end
		end
	end

	return false -- dont forbid placing the fire
end

-- To put out fires! :D
function OnBlockDig( PacketData, Player )
	if( PacketData.m_Direction < 0 ) then
		return false
	end

	local X = PacketData.m_PosX
	local Y = PacketData.m_PosY
	local Z = PacketData.m_PosZ

	X, Y, Z = AddDirection( X, Y, Z, PacketData.m_Direction )

	local World = cRoot:Get():GetWorld()
	if( World:GetBlock( X, Y, Z ) == E_BLOCK_FIRE ) then
		World:SetBlock( X, Y, Z, E_BLOCK_AIR, 0 )
	end

	return false
end

local NumTicks = 0
function Tick( DeltaTime )
	if( NumTicks < 10 ) then	-- Only spread every 10 ticks, to make sure it doesnt happen too fast
		NumTicks = NumTicks + 1
		return
	end
	NumTicks = 0

	local World = cRoot:Get():GetWorld()

	local NewTable = {}
	for key,val in pairs(FireBlocks) do
		X = val["x"]
		Y = val["y"]
		Z = val["z"]
		local Burnables = FindBurnableAround(X, Y, Z)
		if( math.random(10) > 5 ) then
			table.insert( NewTable, val )
		elseif( #Burnables > 0 ) then
			local ToBurn = Burnables[ math.random( #Burnables ) ]
			World:SetBlock( ToBurn["x"], ToBurn["y"], ToBurn["z"], E_BLOCK_FIRE, 0 )
			table.insert( NewTable, ToBurn )
			table.insert( NewTable, val )
		else
			World:SetBlock( X, Y, Z, 0, 0 )
		end
	end
	FireBlocks = NewTable
end
