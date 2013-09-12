
-- APIDesc.lua

-- Contains the API objects' descriptions




g_APIDesc =
{
	Classes =
	{
		cBlockArea =
		{
			Desc = [[
				This class is used when multiple adjacent blocks are to be manipulated. Because of chunking
				and multithreading, manipulating single blocks using {{api:cWorld|cWorld:SetBlock}}() is a rather
				time-consuming operation (locks for exclusive access need to be obtained, chunk lookup is done
				for each block), so whenever you need to manipulate multiple adjacent blocks, it's better to wrap
				the operation into a cBlockArea access. cBlockArea is capable of reading / writing across chunk
				boundaries, has no chunk lookups for get and set operations and is not subject to multithreading
				locking (because it is not shared among threads).</p>
				<p>
				cBlockArea remembers its origin (MinX, MinY, MinZ coords in the Read() call) and therefore supports
				absolute as well as relative get / set operations. Despite that, the contents of a cBlockArea can
				be written back into the world at any coords.</p>
				<p>
				cBlockArea can hold any combination of the following datatypes:<ul>
					<li>block types</li>
					<li>block metas</li>
					<li>blocklight</li>
					<li>skylight</li>
				</ul>
				Read() and Write() functions have parameters that tell the class which datatypes to read / write.
				Note that a datatype that has not been read cannot be written (FIXME).</p>
				<p>
				Typical usage:<ul>
					<li>Create cBlockArea object</li>
					<li>Read an area from the world</li>
					<li>Modify blocks inside cBlockArea</li>
					<li>Write the area back to a world</li>
				</ul></p>
			]],
			Functions =
			{
				Clear = { Params = "", Return = "", Notes = "Clears the object, resets it to zero size" },
				CopyFrom = { Params = "BlockAreaSrc", Return = "", Notes = "Copies contents from BlockAreaSrc into self" },
				CopyTo = { Params = "BlockAreaDst", Return = "", Notes = "Copies contents from self into BlockAreaDst." },
				Create = { Params = "SizeX, SizeY, SizeZ, [DataTypes]", Return = "", Notes = "Initializes this BlockArea to an empty area of the specified size and origin of {0, 0, 0}. Any previous contents are lost." },
				Crop = { Params = "AddMinX, SubMaxX, AddMinY, SubMaxY, AddMinZ, SubMaxZ", Return = "", Notes = "Crops the specified number of blocks from each border. Modifies the size of this blockarea object." },
				DumpToRawFile = { Params = "FileName", Return = "", Notes = "Dumps the raw data into a file. For debugging purposes only." },
				Expand = { Params = "SubMinX, AddMaxX, SubMinY, AddMaxY, SubMinZ, AddMaxZ", Return = "", Notes = "Expands the specified number of blocks from each border. Modifies the size of this blockarea object. New blocks created with this operation are filled with zeroes." },
				Fill = { Params = "DataTypes, BlockType, [BlockMeta], [BlockLight], [BlockSkyLight]", Return = "", Notes = "Fills the entire block area with the same values, specified. Uses the DataTypes param to determine which content types are modified." },
				FillRelCuboid = { Params = "MinRelX, MaxRelX, MinRelY, MaxRelY, MinRelZ, MaxRelZ, DataTypes, BlockType, [BlockMeta], [BlockLight], [BlockSkyLight]", Return = "", Notes = "Fills the specified cuboid with the same values (like Fill() )." },
				GetBlockLight = { Params = "BlockX, BlockY, BlockZ", Return = "NIBBLETYPE", Notes = "Returns the blocklight at the specified absolute coords" },
				GetBlockMeta = { Params = "BlockX, BlockY, BlockZ", Return = "NIBBLETYPE", Notes = "Returns the block meta at the specified absolute coords" },
				GetBlockSkyLight = { Params = "BlockX, BlockY, BlockZ", Return = "NIBBLETYPE", Notes = "Returns the skylight at the specified absolute coords" },
				GetBlockType = { Params = "BlockX, BlockY, BlockZ", Return = "BLOCKTYPE", Notes = "Returns the block type at the specified absolute coords" },
				GetBlockTypeMeta = { Params = "BlockX, BlockY, BlockZ", Return = "BLOCKTYPE, NIBBLETYPE", Notes = "Returns the block type and meta at the specified absolute coords" },
				GetDataTypes = { Params = "", Return = "number", Notes = "Returns the mask of datatypes that the objectis currently holding" },
				GetOriginX = { Params = "", Return = "number", Notes = "Returns the origin x-coord" },
				GetOriginY = { Params = "", Return = "number", Notes = "Returns the origin y-coord" },
				GetOriginZ = { Params = "", Return = "number", Notes = "Returns the origin z-coord" },
				GetRelBlockLight = { Params = "RelBlockX, RelBlockY, RelBlockZ", Return = "NIBBLETYPE", Notes = "Returns the blocklight at the specified relative coords" },
				GetRelBlockMeta = { Params = "RelBlockX, RelBlockY, RelBlockZ", Return = "NIBBLETYPE", Notes = "Returns the block meta at the specified relative coords" },
				GetRelBlockSkyLight = { Params = "RelBlockX, RelBlockY, RelBlockZ", Return = "NIBBLETYPE", Notes = "Returns the skylight at the specified relative coords" },
				GetRelBlockType = { Params = "RelBlockX, RelBlockY, RelBlockZ", Return = "BLOCKTYPE", Notes = "Returns the block type at the specified relative coords" },
				GetRelBlockTypeMeta = { Params = "RelBlockX, RelBlockY, RelBlockZ", Return = "NIBBLETYPE", Notes = "Returns the block type and meta at the specified relative coords" },
				GetSizeX = { Params = "", Return = "number", Notes = "Returns the size of the held data in the x-axis" },
				GetSizeY = { Params = "", Return = "number", Notes = "Returns the size of the held data in the y-axis" },
				GetSizeZ = { Params = "", Return = "number", Notes = "Returns the size of the held data in the z-axis" },
				HasBlockLights = { Params = "", Return = "bool", Notes = "Returns true if current datatypes include blocklight" },
				HasBlockMetas = { Params = "", Return = "bool", Notes = "Returns true if current datatypes include block metas" },
				HasBlockSkyLights = { Params = "", Return = "bool", Notes = "Returns true if current datatypes include skylight" },
				HasBlockTypes = { Params = "", Return = "bool", Notes = "Returns true if current datatypes include block types" },
				LoadFromSchematicFile = { Params = "FileName", Return = "", Notes = "Clears current content and loads new content from the specified schematic file. Returns true if successful. Returns false and logs error if unsuccessful, old content is preserved in such a case." },
				Merge = { Params = "BlockAreaSrc, RelX, RelY, RelZ, Strategy", Return = "", Notes = "Merges BlockAreaSrc into this object at the specified relative coords, using the specified strategy" },
				MirrorXY = { Params = "", Return = "", Notes = "Mirrors this block area around the XY plane. Modifies blocks' metas (if present) to match (i. e. furnaces facing the opposite direction)." },
				MirrorXYNoMeta = { Params = "", Return = "", Notes = "Mirrors this block area around the XY plane. Doesn't modify blocks' metas." },
				MirrorXZ = { Params = "", Return = "", Notes = "Mirrors this block area around the XZ plane. Modifies blocks' metas (if present)" },
				MirrorXZNoMeta = { Params = "", Return = "", Notes = "Mirrors this block area around the XZ plane. Doesn't modify blocks' metas." },
				MirrorYZ = { Params = "", Return = "", Notes = "Mirrors this block area around the YZ plane. Modifies blocks' metas (if present)" },
				MirrorYZNoMeta = { Params = "", Return = "", Notes = "Mirrors this block area around the YZ plane. Doesn't modify blocks' metas." },
				Read = { Params = "World, MinX, MaxX, MinY, MaxY, MinZ, MaxZ, DataTypes", Return = "bool", Notes = "Reads the area from World, returns true if successful" },
				RelLine = { Params = "RelX1, RelY1, RelZ1, RelX2, RelY2, RelZ2, DataTypes, BlockType, [BlockMeta], [BlockLight], [BlockSkyLight]", Return = "", Notes = "Draws a line between the two specified points. Sets only datatypes specified by DataTypes." },
				RotateCCW = { Params = "", Return = "", Notes = "Rotates the block area around the Y axis, counter-clockwise (east -> north). Modifies blocks' metas (if present) to match." },
				RotateCCWNoMeta = { Params = "", Return = "", Notes = "Rotates the block area around the Y axis, counter-clockwise (east -> north). Doesn't modify blocks' metas." },
				RotateCW = { Params = "", Return = "", Notes = "Rotates the block area around the Y axis, clockwise (north -> east). Modifies blocks' metas (if present) to match." },
				RotateCWNoMeta = { Params = "", Return = "", Notes = "Rotates the block area around the Y axis, clockwise (north -> east). Doesn't modify blocks' metas." },
				SaveToSchematicFile = { Params = "FileName", Return = "", Notes = "Saves the current contents to a schematic file. Returns true if successful." },
				SetBlockLight = { Params = "BlockX, BlockY, BlockZ, BlockLight", Return = "", Notes = "Sets the blocklight at the specified absolute coords" },
				SetBlockMeta = { Params = "BlockX, BlockY, BlockZ, BlockMeta", Return = "", Notes = "Sets the block meta at the specified absolute coords" },
				SetBlockSkyLight = { Params = "BlockX, BlockY, BlockZ, SkyLight", Return = "", Notes = "Sets the skylight at the specified absolute coords" },
				SetBlockType = { Params = "BlockX, BlockY, BlockZ, BlockType", Return = "", Notes = "Sets the block type at the specified absolute coords" },
				SetRelBlockLight = { Params = "RelBlockX, RelBlockY, RelBlockZ, BlockLight", Return = "", Notes = "Sets the blocklight at the specified relative coords" },
				SetRelBlockMeta = { Params = "RelBlockX, RelBlockY, RelBlockZ, BlockMeta", Return = "", Notes = "Sets the block meta at the specified relative coords" },
				SetRelBlockSkyLight = { Params = "RelBlockX, RelBlockY, RelBlockZ, SkyLight", Return = "", Notes = "Sets the skylight at the specified relative coords" },
				SetRelBlockType = { Params = "RelBlockX, RelBlockY, RelBlockZ, BlockType", Return = "", Notes = "Sets the block type at the specified relative coords" },
				Write = { Params = "World, MinX, MinY, MinZ, DataTypes", Return = "bool", Notes = "Writes the area into World at the specified coords, returns true if successful" },
			},
			Constants =
			{
				baTypes = { Notes = "Operation should work on block types" },
				baMetas = { Notes = "Operations should work on block metas" },
				baLight = { Notes = "Operations should work on block (emissive) light" },
				baSkyLight = { Notes = "Operations should work on skylight" },
				msOverwrite = { Notes = "Src overwrites anything in Dst" },
				msFillAir = { Notes = "Dst is overwritten by Src only where Src has air blocks" },
				msImprint = { Notes = "Src overwrites Dst anywhere where Dst has non-air blocks" },
				msLake = { Notes = "Special mode for merging lake images" },
			},
		},

		cBlockEntity =
		{
			Desc = [[
				Block entities are simply blocks in the world that have persistent data, such as the text for a sign
				or contents of a chest. All block entities are also saved in the chunk data of the chunk they reside in.
				The cBlockEntity class acts as a common ancestor for all the individual block entities.
			]],
			
			Functions =
			{
				GetBlockType = { Params = "", Return = "BLOCKTYPE", Notes = "Returns the blocktype which is represented by this blockentity. This is the primary means of type-identification" },
				GetChunkX    = { Params = "", Return = "number", Notes = "Returns the chunk X-coord of the block entity's chunk" },
				GetChunkZ    = { Params = "", Return = "number", Notes = "Returns the chunk Z-coord of the block entity's chunk" },
				GetPosX      = { Params = "", Return = "number", Notes = "Returns the block X-coord of the block entity's block" },
				GetPosY      = { Params = "", Return = "number", Notes = "Returns the block Y-coord of the block entity's block" },
				GetPosZ      = { Params = "", Return = "number", Notes = "Returns the block Z-coord of the block entity's block" },
				GetRelX      = { Params = "", Return = "number", Notes = "Returns the relative X coord of the block entity's block within the chunk" },
				GetRelZ      = { Params = "", Return = "number", Notes = "Returns the relative Z coord of the block entity's block within the chunk" },
				GetWorld     = { Params = "", Return = "{{cWorld|cWorld}}", Notes = "Returns the world to which the block entity belongs" },
			},
			Constants =
			{
			},
		},

		cBlockEntityWithItems =
		{
			Desc = [[
				This class is a common ancestor for all {{cBlockEntity|block entities}} that provide item storage.
				Internally, the object has a {{cItemGrid|cItemGrid}} object for storing the items; this ItemGrid is
				accessible through the API. The storage is a grid of items, items in it can be addressed either by a slot
				number, or by XY coords within the grid. If a UI window is opened for this block entity, the item storage
				is monitored for changes and the changes are immediately sent to clients of the UI window.
			]],
			
			Inherits = "cBlockEntity",
			
			Functions =
			{
				GetContents = { Params = "", Return = "{{cItemGrid|cItemGrid}}", Notes = "Returns the cItemGrid object representing the items stored within this block entity" },
				GetSlot = { Params = "SlotNum", Return = "{{cItem|cItem}}", Notes = "Returns the cItem for the specified slot number. Returns nil for invalid slot numbers" },
				GetSlot = { Params = "X, Y", Return = "{{cItem|cItem}}", Notes = "Returns the cItem for the specified slot coords. Returns nil for invalid slot coords" },
				SetSlot = { Params = "SlotNum, {{cItem|cItem}}", Return = "", Notes = "Sets the cItem for the specified slot number. Ignored if invalid slot number" },
				SetSlot = { Params = "X, Y, {{cItem|cItem}}", Return = "", Notes = "Sets the cItem for the specified slot coords. Ignored if invalid slot coords" },
			},
			Constants =
			{
			},
		},

		cChatColor =
		{
			Desc = [[
				A wrapper class for constants representing colors or effects.
			]],
			
			Functions =
			{
				MakeColor = { Params = "ColorCodeConstant", Return = "string", Notes = "Creates the complete color-code-sequence from the color or effect constant" },
			},
			Constants =
			{
				Color         = { Notes = "The first character of the color-code-sequence, §" },
				Delimiter     = { Notes = "The first character of the color-code-sequence, §" },
				Random        = { Notes = "Random letters and symbols animate instead of the text" },
				Plain         = { Notes = "Resets all formatting to normal" },
			},
		},

		cChestEntity =
		{
			Desc = [[
				A chest entity is a {{cBlockEntityWithItems|cBlockEntityWithItems}} descendant that represents a chest
				in the world. Note that doublechests consist of two separate cChestEntity objects, they do not collaborate
				in any way.
			]],
			
			Inherits = "cBlockEntityWithItems",
			
			Functions =
			{
			},
			Constants =
			{
			},
		},

		cChunkDesc =
		{
			Desc = [[
				The cChunkDesc class is a container for chunk data while the chunk is being generated. As such, it is
				only used as a parameter for the {{OnChunkGenerating|OnChunkGenerating}} and
				{{OnChunkGenerated|OnChunkGenerated}} hooks and cannot be constructed on its own. Plugins can use this
				class in both those hooks to manipulate generated chunks.
			]],
			
			Functions =
			{
				FillBlocks                = { Params = "BlockType, BlockMeta", Return = "", Notes = "Fills the entire chunk with the specified blocks" },
				GetBiome                  = { Params = "RelX, RelZ", Return = "EMCSBiome", Notes = "Returns the biome at the specified relative coords" },
				GetBlockMeta              = { Params = "RelX, RelY, RelZ", Return = "NIBBLETYPE", Notes = "Returns the block meta at the specified relative coords" },
				GetBlockType              = { Params = "RelX, RelY, RelZ", Return = "BLOCKTYPE", Notes = "Returns the block type at the specified relative coords" },
				GetBlockTypeMeta          = { Params = "RelX, RelY, RelZ", Return = "BLOCKTYPE, NIBBLETYPE", Notes = "Returns the block type and meta at the specified relative coords" },
				GetHeight                 = { Params = "RelX, RelZ", Return = "number", Notes = "Returns the height at the specified relative coords" },
				IsUsingDefaultBiomes      = { Params = "", Return = "bool", Notes = "Returns true if the chunk is set to use default biome generator" },
				IsUsingDefaultComposition = { Params = "", Return = "bool", Notes = "Returns true if the chunk is set to use default composition generator" },
				IsUsingDefaultFinish      = { Params = "", Return = "bool", Notes = "Returns true if the chunk is set to use default finishers" },
				IsUsingDefaultHeight      = { Params = "", Return = "bool", Notes = "Returns true if the chunk is set to use default height generator" },
				IsUsingDefaultStructures  = { Params = "", Return = "bool", Notes = "Returns true if the chunk is set to use default structures" },
				ReadBlockArea             = { Params = "BlockArea, MinRelX, MaxRelX, MinRelY, MaxRelY, MinRelZ, MaxRelZ", Return = "", Notes = "Reads data from the chunk into the block area object" },
				SetBiome                  = { Params = "RelX, RelZ, EMCSBiome", Return = "", Notes = "Sets the biome at the specified relative coords" },
				SetBlockMeta              = { Params = "RelX, RelY, RelZ, BlockMeta", Return = "", Notes = "Sets the block meta at the specified relative coords" },
				SetBlockType              = { Params = "RelX, RelY, RelZ, BlockType", Return = "", Notes = "Sets the block type at the specified relative coords" },
				SetBlockTypeMeta          = { Params = "RelX, RelY, RelZ, BlockType, BlockMeta", Return = "", Notes = "Sets the block type and meta at the specified relative coords" },
				SetHeight                 = { Params = "RelX, RelZ, Height", Return = "", Notes = "Sets the height at the specified relative coords" },
				SetUseDefaultBiomes       = { Params = "bool", Return = "", Notes = "Sets the chunk to use default biome generator or not" },
				SetUseDefaultComposition  = { Params = "bool", Return = "", Notes = "Sets the chunk to use default composition generator or not" },
				SetUseDefaultFinish       = { Params = "bool", Return = "", Notes = "Sets the chunk to use default finishers or not" },
				SetUseDefaultHeight       = { Params = "bool", Return = "", Notes = "Sets the chunk to use default height generator or not" },
				SetUseDefaultStructures   = { Params = "bool", Return = "", Notes = "Sets the chunk to use default structures or not" },
				WriteBlockArea            = { Params = "BlockArea, MinRelX, MinRelY, MinRelZ", Return = "", Notes = "Writes data from the block area into the chunk" },
			},
			Constants =
			{
			},
		},

		cClientHandle =
		{
			Desc = [[A cClientHandle represents technical aspect of connected player - it's game client.
]],
			Functions =
			{
				GetPing = { Params = "", Return = "number", Notes = "Returns the ping time, in ms" },
				GetPlayer = { Params = "", Return = "{{cPlayer|cPlayer}}", Notes = "Returns the player object connected to this client" },
				GetUniqueID = { Params = "", Return = "number", Notes = "Returns the UniqueID of the client used to identify the client in the server" },
				GetUsername = { Params = "", Return = "string", Notes = "Returns the username that the client has provided" },
				GetViewDistance = { Params = "", Return = "number", Notes = "Returns the viewdistance (number of chunks loaded for the player in each direction)" },
				Kick = { Params = "Reason", Return = "", Notes = "Kicks the user with the specified reason" },
				SetUsername = { Params = "Name", Return = "", Notes = "Sets the username" },
				SetViewDistance = { Params = "ViewDistance", Return = "", Notes = "Sets the viewdistance (number of chunks loaded for the player in each direction)" },
				SendBlockChange = { Params = "BlockX, BlockY, BlockZ, BlockType, BlockMeta", Return = "", Notes = "Sends a block to the client. This can be used to create fake blocks." },
			},
			Constants =
			{
				MAX = { Notes = "10" },
				MIN = { Notes = "4" },
			},
		},

		cCraftingGrid =
		{
			Desc = [[cCraftingGrid represents the player's crafting grid. It is used only in {{OnCraftingNoRecipe|OnCraftingNoRecipe}}, {{OnPostCrafting|OnPostCrafting}} and {{OnPreCrafting|OnPreCrafting}} hooks. Plugins may use it to inspect the items the player placed on their crafting grid.
]],
			Functions =
			{
				Clear = { Params = "", Return = "", Notes = "Clears the entire grid" },
				ConsumeGrid = { Params = "{{cCraftingGrid|CraftingGrid}}", Return = "", Notes = "Consumes items specified in CraftingGrid from the current contents" },
				Dump = { Params = "", Return = "", Notes = "DEBUG build: Dumps the contents of the grid to the log. RELEASE build: no action" },
				GetHeight = { Params = "", Return = "number", Notes = "Returns the height of the grid" },
				GetItem = { Params = "x, y", Return = "{{cItem|cItem}}", Notes = "Returns the item at the specified coords" },
				GetWidth = { Params = "", Return = "number", Notes = "Returns the width of the grid" },
				SetItem = { Params = "x, y, {{cItem|cItem}}", Return = "", Notes = "Sets the item at the specified coords" },
				SetItem = { Params = "x, y, ItemType, ItemCount, ItemDamage", Return = "", Notes = "Sets the item at the specified coords" },
			},
			Constants =
			{
			},
		},

		cCraftingRecipe =
		{
			Desc = [[This class is used to represent a crafting recipe, either a built-in one, or one created dynamically in a plugin. It is used only as a parameter for {{OnCraftingNoRecipe|OnCraftingNoRecipe}}, {{OnPostCrafting|OnPostCrafting}} and {{OnPreCrafting|OnPreCrafting}} hooks. Plugins may use it to inspect or modify a crafting recipe that a player views in their crafting window, either at a crafting table or the survival inventory screen.
</p>
		<p>Internally, the class contains a {{cItem|cItem}} for the result.
]],
			Functions =
			{
				Clear = { Params = "", Return = "", Notes = "Clears the entire recipe, both ingredients and results" },
				ConsumeIngredients = { Params = "CraftingGrid", Return = "", Notes = "Consumes ingredients specified in the given {{cCraftingGrid|cCraftingGrid}} class" },
				Dump = { Params = "", Return = "", Notes = "DEBUG build: dumps ingredients and result into server log. RELEASE build: no action" },
				GetIngredient = { Params = "x, y", Return = "{{cItem|cItem}}", Notes = "Returns the ingredient stored in the recipe at the specified coords" },
				GetIngredientsHeight = { Params = "", Return = "number", Notes = "Returns the height of the ingredients' grid" },
				GetIngredientsWidth = { Params = "", Return = "number", Notes = "Returns the width of the ingredients' grid" },
				GetResult = { Params = "", Return = "{{cItem|cItem}}", Notes = "Returns the result of the recipe" },
				SetIngredient = { Params = "x, y, {{cItem|cItem}}", Return = "", Notes = "Sets the ingredient at the specified coords" },
				SetIngredient = { Params = "x, y, ItemType, ItemCount, ItemDamage", Return = "", Notes = "Sets the ingredient at the specified coords" },
				SetResult = { Params = "{{cItem|cItem}}", Return = "", Notes = "Sets the result item" },
				SetResult = { Params = "ItemType, ItemCount, ItemDamage", Return = "", Notes = "Sets the result item" },
			},
			Constants =
			{
			},
		},

		cCuboid =
		{
			Desc = [[cCuboid offers some native support for cuboids. A cuboid simply consists of two {{vector3i|vector3i}}'s. It offers some extra functions for sorting and checking if a point is inside the cuboid.
]],
			Functions =
			{
			},
			Constants =
			{
				p = { Notes = "{{Vector3i|Vector3i}}" },
				p = { Notes = "{{Vector3i|Vector3i}}" },
				Sort = { Notes = "void" },
				IsInside = { Notes = "bool" },
				IsInside = { Notes = "bool" },
			},
		},

		cDispenserEntity =
		{
			Desc = [[This class represents a dispenser block entity in the world. Most of this block entity's functionality is implemented in the {{cDropSpenserEntity|cDropSpenserEntity}} class that represents the behavior common with a {{cDropperEntity|dropper}} entity.
</p>
		<p>An object of this class can be created from scratch when generating chunks ({{OnChunkGenerated|OnChunkGenerated}} and {{OnChunkGenerating|OnChunkGenerating}} hooks). 
]],
			Functions =
			{
				constructor = { Params = "BlockX, BlockY, BlockZ", Return = "cDispenserEntity", Notes = "Creates a new cDispenserEntity at the specified coords" },
			},
			Constants =
			{
			},
		},

		cDropperEntity =
		{
			Desc = [[This class represents a dropper block entity in the world. Most of this block entity's functionality is implemented in the {{cDropSpenserEntity|cDropSpenserEntity}} class that represents the behavior common with the {{cDispenserEntity|dispenser}} entity.
</p>
		<p>An object of this class can be created from scratch when generating chunks ({{OnChunkGenerated|OnChunkGenerated}} and {{OnChunkGenerating|OnChunkGenerating}} hooks). 
]],
			Functions =
			{
				constructor = { Params = "BlockX, BlockY, BlockZ", Return = "cDropperEntity", Notes = "Creates a new cDropperEntity at the specified coords" },
			},
			Constants =
			{
			},
		},

		cDropSpenser =
		{
			Desc = [[This is a class that implements behavior common to both {{cDispenserEntity|dispensers}} and {{cDropperEntity|droppers}}.
]],
			Functions =
			{
				Activate = { Params = "", Return = "", Notes = "Sets the block entity to dropspense an item in the next tick" },
				AddDropSpenserDir = { Params = "BlockX, BlockY, BlockZ, BlockMeta", Return = "BlockX, BlockY, BlockZ", Notes = "Adjusts the block coords to where the dropspenser items materialize" },
				SetRedstonePower = { Params = "IsPowered", Return = "", Notes = "Sets the redstone status of the dropspenser. If the redstone power goes from off to on, the dropspenser will be activated" },
			},
			Constants =
			{
				ContentsWidth = { Notes = "Width (X) of the cItemGrid representing the contents" },
				ContentsHeight = { Notes = "Height (Y) of the cItemGrid representing the contents" },
			},
		},

		cEnchantments =
		{
			Desc = [[This class  is the storage for enchantments for a single {{cItem|cItem}} object, through its m_Enchantments member variable. Although it is possible to create a standalone object of this class, it is not yet used in any API directly. 
</p>
		<p>Enchantments can be initialized either programmatically by calling the individual functions (SetLevel()), or by using a string description of the enchantment combination. This string description is in the form "id=lvl;id=lvl;...;id=lvl;", where id is either a numerical ID of the enchantment, or its textual representation from the table below, and lvl is the desired enchantment level. The class can also create its string description from its current contents; however that string description will only have the numerical IDs.
]],
			Functions =
			{
				constructor = { Params = "", Return = "cEnchantments", Notes = "Creates a new empty cEnchantments object" },
				constructor = { Params = "StringSpec", Return = "cEnchantments", Notes = "Creates a new cEnchantments object filled with enchantments based on the string description" },
				AddFromString = { Params = "StringSpec", Return = "", Notes = "Adds the enchantments in the string description into the object. If a specified enchantment already existed, it is overwritten." },
				Clear = { Params = "", Return = "", Notes = "Removes all enchantments" },
				GetLevel = { Params = "EnchantmentNumID", Return = "number", Notes = "Returns the level of the specified enchantment stored in this object; 0 if not stored" },
				IsEmpty = { Params = "", Return = "bool", Notes = "Returns true if the object stores no enchantments" },
				SetLevel = { Params = "EnchantmentNumID, Level", Return = "", Notes = "Sets the level for the specified enchantment, adding it if not stored before or removing it if level < = 0" },
				StringToEnchantmentID = { Params = "EnchantmentTextID", Return = "number", Notes = "(static) Returns the enchantment numerical ID, -1 if not understood. Case insensitive" },
				ToString = { Params = "", Return = "string", Notes = "Returns the string description of all the enchantments stored in this object, in numerical-ID form" },
			},
			Constants =
			{
			},
		},

		cEntity =
		{
			Desc = [[A cEntity object represents an object in the world, it has a position and orientation. cEntity is an abstract class, and can not be instantiated directly, instead, all entities are implemented as subclasses. The cEntity class works as the common interface for the operations that all (most) entities support.
</p>
		<p>All cEntity objects have an Entity Type so it can be determined what kind of entity it is efficiently. Entities also have a class inheritance awareness, they know their class name, their parent class' name and can decide if there is a class within their inheritance chain. Since these functions operate on strings, they are slightly slower than checking the entity type directly, on the other hand, they are more specific (compare etMob vs "cSpider" class name).
</p>
		<p>Note that you should not store a cEntity object between two hooks' calls, because MCServer may remove that entity in between the calls. If you need to refer to an entity later, use its UniqueID and {{cWorld|cWorld}}'s entity manipulation functions to access the entity.
]],
			Functions =
			{
				Destroy = { Params = "", Return = "", Notes = "Schedules the entity to be destroyed" },
				GetChunkX = { Params = "", Return = "number", Notes = "Returns the X-coord of the chunk in which the entity is placed" },
				GetChunkY = { Params = "", Return = "number", Notes = "Returns the Y-coord of the chunk in which the entity is placed" },
				GetChunkZ = { Params = "", Return = "number", Notes = "Returns the Z-coord of the chunk in which the entity is placed" },
				GetClass = { Params = "", Return = "string", Notes = "Returns the classname of the entity, such as \"spider\" or \"pickup\"" },
				GetClassStatic = { Params = "", Return = "string", Notes = "Returns the entity classname that this class implements. Each descendant overrides this function. Is static" },
				GetEntityType = { Params = "", Return = "cEntity.eEntityType", Notes = "Returns the type of the entity, one of the etXXX constants" },
				GetLookVector = { Params = "", Return = "Vector3f", Notes = "Returns the vector that defines the direction in which the entity is looking" },
				GetParentClass = { Params = "", Return = "string", Notes = "Returns the name of the direct parent class for this entity" },
				GetPitch = { Params = "", Return = "number", Notes = "Returns the pitch (nose-down rotation) of the entity" },
				GetPosX = { Params = "", Return = "number", Notes = "Returns the X-coord of the entity's pivot" },
				GetPosY = { Params = "", Return = "number", Notes = "Returns the Y-coord of the entity's pivot" },
				GetPosZ = { Params = "", Return = "number", Notes = "Returns the Z-coord of the entity's pivot" },
				GetPosition = { Params = "", Return = "Vector3d", Notes = "Returns the entity's pivot position as a 3D vector" },
				GetRoll = { Params = "", Return = "number", Notes = "Returns the roll (sideways rotation) of the entity" },
				GetRot = { Params = "", Return = "Vector3f", Notes = "Returns the entire rotation vector (Rotation, Pitch, Roll)" },
				GetRotation = { Params = "", Return = "number", Notes = "Returns the rotation (direction) of the entity" },
				GetSpeed = { Params = "", Return = "Vector3d", Notes = "Returns the complete speed vector of the entity" },
				GetSpeedX = { Params = "", Return = "number", Notes = "Returns the X-part of the speed vector" },
				GetSpeedY = { Params = "", Return = "number", Notes = "Returns the Y-part of the speed vector" },
				GetSpeedZ = { Params = "", Return = "number", Notes = "Returns the Z-part of the speed vector" },
				GetUniqueID = { Params = "", Return = "number", Notes = "Returns the ID that uniquely identifies the entity" },
				GetWorld = { Params = "", Return = "{{cWorld|cWorld}}", Notes = "Returns the world where the entity resides" },
				IsA = { Params = "ClassName", Return = "bool", Notes = "Returns true if the entity class is a descendant of the specified class name, or the specified class itself" },
				IsCrouched = { Params = "", Return = "bool", Notes = "Returns true if the entity is crouched. False for entities that don't support crouching" },
				IsDestroyed = { Params = "", Return = "bool", Notes = "Returns true if the entity has been destroyed and is awaiting removal from the internal structures" },
				IsMinecart = { Params = "", Return = "bool", Notes = "Returns true if the entity represents a minecart" },
				IsMob = { Params = "", Return = "bool", Notes = "Returns true if the entity represents any mob" },
				IsOnFire = { Params = "", Return = "bool", Notes = "Returns true if the entity is on fire" },
				IsPickup = { Params = "", Return = "bool", Notes = "Returns true if the entity represents a pickup" },
				IsPlayer = { Params = "", Return = "bool", Notes = "Returns true if the entity represents a player" },
				IsTNT = { Params = "", Return = "bool", Notes = "Returns true if the entity represents a TNT entity" },
				IsRclking = { Params = "", Return = "bool", Notes = "Currently unimplemented" },
				IsSprinting = { Params = "", Return = "bool", Notes = "Returns true if the entity is sprinting. ENtities that cannot sprint return always false" },
				SetPitch = { Params = "number", Return = "", Notes = "Sets the pitch (nose-down rotation) of the entity" },
				SetPosX = { Params = "number", Return = "", Notes = "Sets the X-coord of the entity's pivot" },
				SetPosY = { Params = "number", Return = "", Notes = "Sets the Y-coord of the entity's pivot" },
				SetPosZ = { Params = "number", Return = "", Notes = "Sets the Z-coord of the entity's pivot" },
				SetPosition = { Params = "X, Y, Z", Return = "", Notes = "Sets all three coords of the entity's pivot" },
				SetPosition = { Params = "{{Vector3d|Vector3d}}", Return = "", Notes = ":::" },
				SetRoll = { Params = "number", Return = "", Notes = "Sets the roll (sideways rotation) of the entity" },
				SetRot = { Params = "{{Vector3f|Vector3f}}", Return = "", Notes = "Sets the entire rotation vector (Rotation, Pitch, Roll)" },
				SetRotation = { Params = "number", Return = "", Notes = "Sets the rotation (direction) of the entity" },
			},
			Constants =
			{
				etEntity = { Notes = "N" },
				etPlayer = { Notes = "{{cPlayer|cPlayer" },
				etPickup = { Notes = "{{cPickup|cPickup" },
				etMob = { Notes = "{{cMonster|cMonster}} and descendan" },
				etFallingBlock = { Notes = "{{cFallingBlock|cFallingBlock" },
				etMinecart = { Notes = "{{cMinecart|cMinecart" },
				etTNT = { Notes = "{{cTNTEntity|cTNTEntity" },
			},
		},

		cFurnaceEntity =
		{
			Desc = [[This class represents a furnace block entity in the world. An object of this class can be created from scratch when generating chunks ({{OnChunkGenerated|OnChunkGenerated}} and {{OnChunkGenerating|OnChunkGenerating}} hooks)
]],
			Functions =
			{
				constructor = { Params = "BlockX, BlockY, BlockZ, BlockType, BlockMeta", Return = "cFurnaceEntity", Notes = "Creates a new cFurnaceEntity at the specified coords and the specified block type / meta" },
				GetCookTimeLeft = { Params = "", Return = "number", Notes = "Returns the time until the current item finishes cooking, in ticks" },
				GetFuelBurnTimeLeft = { Params = "", Return = "number", Notes = "Returns the time until the current fuel is depleted, in ticks" },
				GetFuelSlot = { Params = "", Return = "{{cItem|cItem}}", Notes = "Returns the item in the fuel slot" },
				GetInputSlot = { Params = "", Return = "{{cItem|cItem}}", Notes = "Returns the item in the input slot" },
				GetOutputSlot = { Params = "", Return = "{{cItem|cItem}}", Notes = "Returns the item in the output slot" },
				GetTimeCooked = { Params = "", Return = "number", Notes = "Returns the time that the current item has been cooking, in ticks" },
				HasFuelTimeLeft = { Params = "", Return = "bool", Notes = "Returns true if there's time before the current fuel is depleted" },
				SetFuelSlot = { Params = "{{cItem|cItem}}", Return = "", Notes = "Sets the item in the fuel slot" },
				SetInputSlot = { Params = "{{cItem|cItem}}", Return = "", Notes = "Sets the item in the input slot" },
				SetOutputSlot = { Params = "{{cItem|cItem}}", Return = "", Notes = "Sets the item in the output slot" },
			},
			Constants =
			{
				fsInput = { Notes = "Index of the input slot, when using the GetSlot() / SetSlot() functions" },
				fsFuel = { Notes = "Index of the fuel slot, when using the GetSlot() / SetSlot() functions" },
				fsOutput = { Notes = "Index of the output slot, when using the GetSlot() / SetSlot() functions" },
				ContentsWidth = { Notes = "Width (X) of the {{cItemGrid|cItemGrid}} representing the contents" },
				ContentsHeight = { Notes = "Height (Y) of the {{cItemGrid|cItemGrid}} representing the contents" },
			},
		},

		cGroup =
		{
			Desc = [[cGroup is a group {{cPlayer|cPlayer}}'s can be in. Groups define the permissions players have, and optionally the color of their name in the chat.
]],
			Functions =
			{
			},
			Constants =
			{
				SetName = { Notes = "void" },
				GetName = { Notes = "String" },
				SetColor = { Notes = "void" },
				GetColor = { Notes = "String" },
				AddCommand = { Notes = "void" },
				HasCommand = { Notes = "bool" },
				AddPermission = { Notes = "void" },
				InheritFrom = { Notes = "void" },
			},
		},

		cIniFile =
		{
			Desc = [[The cIniFile is a class that makes it simple to read from and write to INI files. MCServer uses mostly INI files for settings and options.
]],
			Functions =
			{
			},
			Constants =
			{
				cIniFile = { Notes = "{{cIniFile|cIniFile}}" },
				CaseSensitive = { Notes = "void" },
				CaseInsensitive = { Notes = "void" },
				Path = { Notes = "void" },
				Path = { Notes = "String" },
				SetPath = { Notes = "void" },
				ReadFile = { Notes = "bool" },
				WriteFile = { Notes = "bool" },
				Erase = { Notes = "void" },
				Clear = { Notes = "void" },
				Reset = { Notes = "void" },
				FindKey = { Notes = "long i" },
				FindValue = { Notes = "long i" },
				NumKeys = { Notes = "unsigned i" },
				GetNumKeys = { Notes = "unsigned i" },
				AddKeyName = { Notes = "unsigned int" },
				KeyName = { Notes = "Stri" },
				GetKeyName = { Notes = "Stri" },
				NumValues = { Notes = "unsigned int" },
				GetNumValues = { Notes = "unsigned int" },
				NumValues = { Notes = "unsigned int" },
				GetNumValues = { Notes = "unsigned int" },
				ValueName = { Notes = "Stri" },
				GetValueName = { Notes = "Stri" },
				ValueName = { Notes = "Stri" },
				GetValueName = { Notes = "Stri" },
				GetValue = { Notes = "Stri" },
				GetValue = { Notes = "Stri" },
				GetValueI = { Notes = "i" },
				GetValueB = { Notes = "bo" },
				GetValueF = { Notes = "doub" },
				GetValueSet = { Notes = "Stri" },
				GetValueSetI = { Notes = "i" },
				GetValueSetB = { Notes = "bo" },
				GetValueSetF = { Notes = "doub" },
				SetValue = { Notes = "bool" },
				SetValue = { Notes = "bool" },
				SetValueI = { Notes = "bool" },
				SetValueB = { Notes = "bool" },
				SetValueF = { Notes = "bool" },
				DeleteValueByID = { Notes = "bool" },
				DeleteValue = { Notes = "bool" },
				DeleteKey = { Notes = "bool" },
				NumHeaderComments = { Notes = "unsigned int" },
				HeaderComment = { Notes = "void" },
				HeaderComment = { Notes = "Stri" },
				DeleteHeaderComment = { Notes = "bool" },
				DeleteHeaderComments = { Notes = "void" },
				NumKeyComments = { Notes = "unsigned i" },
				NumKeyComments = { Notes = "unsigned i" },
				KeyComment = { Notes = "bool" },
				KeyComment = { Notes = "bool" },
				KeyComment = { Notes = "Stri" },
				KeyComment = { Notes = "Stri" },
				DeleteKeyComment = { Notes = "bool" },
				DeleteKeyComment = { Notes = "bool" },
				DeleteKeyComments = { Notes = "bool" },
				DeleteKeyComments = { Notes = "bool" },
			},
		},

		cInventory =
		{
			Desc = [[This object is used to store the items that a {{cPlayer|cPlayer}} has. It also keeps track of what item the player has currently selected in their hotbar.
Internally, the class uses three {{cItemGrid|cItemGrid}} objects to store the contents:
<li>Armor</li>
<li>Inventory</li>
<li>Hotbar</li>
These ItemGrids are available in the API and can be manipulated by the plugins, too.
]],
			Functions =
			{
				AddItem = { Params = "{{cItem|cItem}}, [AllowNewStacks]", Return = "number", Notes = "Adds an item to the storage; if AllowNewStacks is true (default), will also create new stacks in empty slots. Returns the number of items added" },
				AddItems = { Params = "{{cItems|cItems}}, [AllowNewStacks]", Return = "number", Notes = "Same as AddItem, but for several items at once" },
				ChangeSlotCount = { Params = "SlotNum, AddToCount", Return = "number", Notes = "Adds AddToCount to the count of items in the specified slot. If the slot was empty, ignores the call. Returns the new count in the slot, or -1 if invalid SlotNum" },
				Clear = { Params = "", Return = "", Notes = "Empties all slots" },
				CopyToItems = { Params = "{{cItems|cItems}}", Return = "", Notes = "Copies all non-empty slots into the cItems object provided; original cItems contents are preserved" },
				DamageEquippedItem = { Params = "[DamageAmount]", Return = "bool", Notes = "Adds the specified damage (1 by default) to the currently equipped it" },
				DamageItem = { Params = "SlotNum, [DamageAmount]", Return = "bool", Notes = "Adds the specified damage (1 by default) to the specified item, returns true if the item reached its max damage and should be destroyed" },
				GetArmorGrid = { Params = "", Return = "{{cItemGrid|cItemGrid}}", Notes = "Returns the ItemGrid representing the armor grid (1 x 4 slots)" },
				GetArmorSlot = { Params = "ArmorSlotNum", Return = "{{cItem|cItem}}", Notes = "Returns the specified armor slot contents. Note that the returned item is read-only" },
				GetEquippedBoots = { Params = "", Return = "{{cItem|cItem}}", Notes = "Returns the item in the \"boots\" slot of the armor grid. Note that the returned item is read-only" },
				GetEquippedChestplate = { Params = "", Return = "{{cItem|cItem}}", Notes = "Returns the item in the \"chestplate\" slot of the armor grid. Note that the returned item is read-only" },
				GetEquippedHelmet = { Params = "", Return = "{{cItem|cItem}}", Notes = "Returns the item in the \"helmet\" slot of the armor grid. Note that the returned item is read-only" },
				GetEquippedItem = { Params = "", Return = "{{cItem|cItem}}", Notes = "Returns the currently selected item from the hotbar. Note that the returned item is read-only" },
				GetEquippedLeggings = { Params = "", Return = "{{cItem|cItem}}", Notes = "Returns the item in the \"leggings\" slot of the armor grid. Note that the returned item is read-only" },
				GetEquippedSlotNum = { Params = "", Return = "number", Notes = "Returns the hotbar slot number for the currently selected item" },
				GetHotbarGrid = { Params = "", Return = "{{cItemGrid|cItemGrid}}", Notes = "Returns the ItemGrid representing the hotbar grid (9 x 1 slots)" },
				GetHotbarSlot = { Params = "HotBarSlotNum", Return = "{{cItem|cItem}}", Notes = "Returns the specified hotbar slot contents. Note that the returned item is read-only" },
				GetInventoryGrid = { Params = "", Return = "{{cItemGrid|cItemGrid}}", Notes = "Returns the ItemGrid representing the main inventory (9 x 3 slots)" },
				GetInventorySlot = { Params = "InventorySlotNum", Return = "{{cItem|cItem}}", Notes = "Returns the specified main inventory slot contents. Note that the returned item is read-only" },
				GetOwner = { Params = "", Return = "{{cPlayer|cPlayer}}", Notes = "Returns the player whose inventory this object represents" },
				GetSlot = { Params = "SlotNum", Return = "{{cItem|cItem}}", Notes = "Returns the contents of the specified slot. Note that the returned item is read-only" },
				HasItems = { Params = "{{cItem|cItem}}", Return = "bool", Notes = "Returns true if there are at least as many items of the specified type as in the parameter" },
				HowManyCanFit = { Params = "{{cItem|cItem}}", Return = "number", Notes = "Returns the number of the specified items that can fit in the storage, including empty slots" },
				HowManyItems = { Params = "{{cItem|cItem}}", Return = "number", Notes = "Returns the number of the specified items that are currently stored" },
				RemoveOneEquippedItem = { Params = "", Return = "", Notes = "Removes one item from the hotbar's currently selected slot" },
				SetArmorSlot = { Params = "ArmorSlotNum, {{cItem|cItem}}", Return = "", Notes = "Sets the specified armor slot contents" },
				SetEquippedSlotNum = { Params = "EquippedSlotNum", Return = "", Notes = "Sets the currently selected hotbar slot number" },
				SetHotbarSlot = { Params = "HotbarSlotNum, {{cItem|cItem}}", Return = "", Notes = "Sets the specified hotbar slot contents" },
				SetInventorySlot = { Params = "InventorySlotNum, {{cItem|cItem}}", Return = "", Notes = "Sets the specified main inventory slot contents" },
				SetSlot = { Params = "SlotNum, {{cItem|cItem}}", Return = "", Notes = "Sets the specified slot contents" },
			},
			Constants =
			{
				invArmorCount = { Notes = "4" },
				invArmorOffset = { Notes = "0" },
				invInventoryCount = { Notes = "" },
				invInventoryOffset = { Notes = "4" },
				invHotbarCount = { Notes = "9" },
				invHotbarOffset = { Notes = "" },
				invNumSlots = { Notes = "" },
			},
		},

		cItem =
		{
			Desc = [[cItem is what defines an item or stack of items in the game, it contains the item ID, damage, quantity and enchantments. Each slot in a {{cEnchantments|cEnchantments}} class
]],
			Functions =
			{
				constructor = { Params = "", Return = "cItem", Notes = "Creates a new empty cItem obje" },
				constructor = { Params = "ItemType, Count, Damage, EnchantmentString", Return = "cItem", Notes = "Creates a new cItem object of the specified type, count (1 by default), damage (0 by default) and enchantments (non-enchanted by default)" },
				constructor = { Params = "cItem", Return = "cItem", Notes = "Creates an exact copy of the cItem object in the parameter" },
				Clear = { Params = "", Return = "", Notes = "Resets the instance to an empty item" },
				CopyOne = { Params = "", Return = "cItem", Notes = "Creates a copy of this object, with its count set to 1" },
				DamageItem = { Params = "[Amount]", Return = "bool", Notes = "Adds the specified damage. Returns true when damage reaches max value and the item should be destroyed (but doesn't destroy the item)" },
				Empty = { Params = "", Return = "", Notes = "Resets the instance to an empty item" },
				GetMaxDamage = { Params = "", Return = "number", Notes = "Returns the maximum value for damage that this item can get before breaking; zero if damage is not accounted for for this item type" },
				IsDamageable = { Params = "", Return = "bool", Notes = "Returns true if this item does account for its damage" },
				IsEnchantable = { Params = "ItemType", Return = "bool", Notes = "(static) Returns true if the specified ItemType is an enchantable item, as defined by the 1.2.5 network protocol (deprecated)" },
				IsEqual = { Params = "cItem", Return = "bool", Notes = "Returns true if the item in the parameter is the same as the one stored in the object (type, damage and enchantments)" },
				IsSameType = { Params = "cItem", Return = "bool", Notes = "Returns true if the item in the parameter is of the same ItemType as the one stored in the object" },
				IsStackableWith = { Params = "cItem", Return = "bool", Notes = "Returns true if the item in the parameter is stackable with the one stored in the object" },
			},
			Constants =
			{
			},
		},

		cItemGrid =
		{
			Desc = [[This class represents a 2D array of items. It is used as the underlying storage and API for all cases that use a grid of items:
<li>Chest contents</li>
<li>(TODO) Chest minecart contents</li>
<li>Dispenser contents</li>
<li>Dropper contents</li>
<li>(TODO) Furnace contents (?)</li>
<li>(TODO) Hopper contents</li>
<li>(TODO) Hopper minecart contents</li>
<li>Player Inventory areas</li>
<li>(TODO) Trapped chest contents</li>
</p>
		<p>The items contained in this object are accessed either by a pair of XY coords, or a slot number (x + Width * y). There are functions available for converting between the two formats.
]],
			Functions =
			{
				AddItem = { Params = "{{cItem|cItem}}, [AllowNewStacks]", Return = "number", Notes = "Adds an item to the storage; if AllowNewStacks is true (default), will also create new stacks in empty slots. Returns the number of items added" },
				AddItems = { Params = "{{cItems|cItems}}, [AllowNewStacks]", Return = "number", Notes = "Same as AddItem, but for several items at once" },
				ChangeSlotCount = { Params = "SlotNum, AddToCount", Return = "number", Notes = "Adds AddToCount to the count of items in the specified slot. If the slot was empty, ignores the call. Returns the new count in the slot, or -1 if invalid SlotNum" },
				ChangeSlotCount = { Params = "X, Y, AddToCount", Return = "number", Notes = "Adds AddToCount to the count of items in the specified slot. If the slot was empty, ignores the call. Returns the new count in the slot, or -1 if invalid slot coords" },
				Clear = { Params = "", Return = "", Notes = "Empties all slots" },
				CopyToItems = { Params = "{{cItems|cItems}}", Return = "", Notes = "Copies all non-empty slots into the cItems object provided; original cItems contents are preserved" },
				DamageItem = { Params = "SlotNum, [DamageAmount]", Return = "bool", Notes = "Adds the specified damage (1 by default) to the specified item, returns true if the item reached its max damage and should be destroyed" },
				DamageItem = { Params = "X, Y, [DamageAmount]", Return = "bool", Notes = "Adds the specified damage (1 by default) to the specified item, returns true if the item reached its max damage and should be destroyed" },
				EmptySlot = { Params = "SlotNum", Return = "", Notes = "Destroys the item in the specified slot" },
				EmptySlot = { Params = "X, Y", Return = "", Notes = "Destroys the item in the specified slot" },
				GetFirstEmptySlot = { Params = "", Return = "number", Notes = "Returns the SlotNumber of the first empty slot, -1 if all slots are full" },
				GetHeight = { Params = "", Return = "number", Notes = "Returns the Y dimension of the grid" },
				GetLastEmptySlot = { Params = "", Return = "number", Notes = "Returns the SlotNumber of the last empty slot, -1 if all slots are full" },
				GetNextEmptySlot = { Params = "StartFrom", Return = "number", Notes = "Returns the SlotNumber of the first empty slot following StartFrom, -1 if all the following slots are full" },
				GetNumSlots = { Params = "", Return = "number", Notes = "Returns the total number of slots in the grid (Width * Height)" },
				GetSlot = { Params = "SlotNumber", Return = "{{cItem|cItem}}", Notes = "Returns the item in the specified slot. Note that the item is read-only" },
				GetSlot = { Params = "X, Y", Return = "{{cItem|cItem}}", Notes = "Returns the item in the specified slot. Note that the item is read-only" },
				GetSlotCoords = { Params = "SlotNum", Return = "number, number", Notes = "Returns the X and Y coords for the specified SlotNumber. Returns \"-1, -1\" on invalid SlotNumber" },
				GetSlotNum = { Params = "X, Y", Return = "number", Notes = "Returns the SlotNumber for the specified slot coords. Returns -1 on invalid coords" },
				GetWidth = { Params = "", Return = "number", Notes = "Returns the X dimension of the grid" },
				HasItems = { Params = "{{cItem|cItem}}", Return = "bool", Notes = "Returns true if there are at least as many items of the specified type as in the parameter" },
				HowManyCanFit = { Params = "{{cItem|cItem}}", Return = "number", Notes = "Returns the number of the specified items that can fit in the storage, including empty slots" },
				HowManyItems = { Params = "{{cItem|cItem}}", Return = "number", Notes = "Returns the number of the specified items that are currently stored" },
				IsSlotEmpty = { Params = "SlotNum", Return = "bool", Notes = "Returns true if the specified slot is empty, or an invalid slot is specified" },
				IsSlotEmpty = { Params = "X, Y", Return = "bool", Notes = "Returns true if the specified slot is empty, or an invalid slot is specified" },
				RemoveOneItem = { Params = "SlotNum", Return = "{{cItem|cItem}}", Notes = "Removes one item from the stack in the specified slot and returns it as a single cItem. Empty slots are skipped and an empty item is returned" },
				RemoveOneItem = { Params = "X, Y", Return = "{{cItem|cItem}}", Notes = "Removes one item from the stack in the specified slot and returns it as a single cItem. Empty slots are skipped and an empty item is returned" },
				SetSlot = { Params = "SlotNum, {{cItem|cItem}}", Return = "", Notes = "Sets the specified slot to the specified item" },
				SetSlot = { Params = "X, Y, {{cItem|cItem}}", Return = "", Notes = "Sets the specified slot to the specified item" },
			},
			Constants =
			{
			},
		},

		citems =
		{
			Desc = [[]],
			Functions =
			{
				constructor = { Params = "", Return = "cItems", Notes = "Creates a new cItems object" },
				Add = { Params = "Index, {{cItem|cItem}}", Return = "", Notes = "Adds a new item to the end of the collection" },
				Add = { Params = "Index, ItemType, ItemCount, ItemDamage", Return = "", Notes = "Adds a new item to the end of the collection" },
				Clear = { Params = "", Return = "", Notes = "Removes all items from the collection" },
				Delete = { Params = "Index", Return = "", Notes = "Deletes item at the specified index" },
				Get = { Params = "Index", Return = "{{cItem|cItem}}", Notes = "Returns the item at the specified index" },
				Set = { Params = "Index, {{cItem|cItem}}", Return = "", Notes = "Sets the item at the specified index to the specified item" },
				Set = { Params = "Index, ItemType, ItemCount, ItemDamage", Return = "", Notes = "Sets the item at the specified index to the specified item" },
				Size = { Params = "", Return = "number", Notes = "Returns the number of items in the collection" },
			},
			Constants =
			{
			},
		},

		cLadder =
		{
			Desc = [[cLadder just represents ladders and their specific - rotation.
]],
			Functions =
			{
			},
			Constants =
			{
				DirectionToMetaData = { Notes = "char" },
				MetaDataToDirection = { Notes = "char" },
			},
		},

		cluachunk =
		{
			Desc = [[]],
			Functions =
			{
			},
			Constants =
			{
			},
		},

		Callbacks =
		{
			Desc = [[This class is used by plugins wishing to display a custom window to the player, unrelated to block entities or entities near the player. The window can be of any type and have any contents that the plugin defines. Callbacks for when the player modifies the window contents and when the player closes the window can be set.
</p>
		<p>This class inherits from the {{cWindow|cWindow}} class, so all cWindow's functions and constants can be used, in addition to the cLuaWindow-specific functions listed below.
</p>
		<p>The contents of this window are represented by a {{cWindow|cWindow}}:GetSlot() etc. or {{cPlayer|cPlayer}}:GetInventory() to access the player inventory.
</p>
		<p>When creating a new cLuaWindow object, you need to specify both the window type and the contents' width and height. Note that MCServer accepts any combination of these, but opening a window for a player may crash their client if the contents' dimensions don't match the client's expectations.
</p>
		<p>To open the window for a player, call {{cPlayer|cPlayer}}:OpenWindow(). Multiple players can open window of the same cLuaWindow object. All players see the same items in the window's contents (like chest, unlike crafting table).
The object calls the following functions at the appropriate time:
==== OnClosing Callback ====
This callback, settabel via the SetOnClosing() function, will be called when the player tries to close the window, or the window is closed for any other reason (such as a player disconnecting).
<code lua>
function OnWindowClosing(a_Window, a_Player, a_CanRefuse)
</code>
The a_Window parameter is the cLuaWindow object representing the window, a_Player is the player for whom the window is about to close. a_CanRefuse specifies whether the callback can refuse the closing. If the callback returns true and a_CanRefuse is true, the window is not closed (internally, the server sends a new OpenWindow packet to the client).
==== OnSlotChanged Callback ====
This callback, settable via the SetOnSlotChanged() function, will be called whenever the contents of any slot in the window's contents (i. e. NOT in the player inventory!) changes.
<code lua>
function OnWindowSlotChanged(a_Window, a_SlotNum)
</code>
The a_Window parameter is the cLuaWindow object representing the window, a_SlotNum is the slot number. There is no reference to a {{cWorld|cWorld}}:DoWithPlayer().
</p>
		<p>Any returned values are ignored.
]],
			Functions =
			{
				constructor = { Params = "WindowType, ContentsWidth, ContentsHeight, Title", Return = "", Notes = "Creates a new object of this class" },
				GetContents = { Params = "", Return = "{{cItemGrid|cItemGrid}}", Notes = "Returns the cItemGrid object representing the internal storage in this window" },
				SetOnClosing = { Params = "OnClosingCallback", Return = "", Notes = "Sets the function that the window will call when it is about to be closed by a player" },
				SetOnSlotChanged = { Params = "OnSlotChangedCallback", Return = "", Notes = "Sets the function that the window will call when a slot is changed by a player" },
			},
			Constants =
			{
			},
		},

		cMCLogger =
		{
			Desc = [[cMCLogger gives you a bit more complex way to log things.
]],
			Functions =
			{
			},
			Constants =
			{
				LogSimple = { Notes = "void" },
			},
		},

		cPacket =
		{
			Desc = [[This packet is received by clients when they are digging blocks
]],
			Functions =
			{
			},
			Constants =
			{
				m = { Notes = "char" },
				m = { Notes = "int" },
				m = { Notes = "char" },
				m = { Notes = "int" },
				m = { Notes = "char" },
			},
		},

		cPacket =
		{
			Desc = [[This packet is received by clients when they are placing blocks
]],
			Functions =
			{
			},
			Constants =
			{
				m = { Notes = "int" },
				m = { Notes = "char" },
				m = { Notes = "int" },
				m = { Notes = "char" },
				m = { Notes = "short int" },
				m = { Notes = "char" },
				m = { Notes = "short int" },
			},
		},

		cPacket =
		{
			Desc = [[This packet is received when a client logs in
]],
			Functions =
			{
			},
			Constants =
			{
				m = { Notes = "int" },
				m = { Notes = "String" },
				m = { Notes = "String" },
				m = { Notes = "int" },
				m = { Notes = "int" },
				m = { Notes = "char" },
				m = { Notes = "char" },
				m = { Notes = "char" },
			},
		},

		cPawn =
		{
			Desc = [[cPawn is a controllable pawn object, controlled by either AI or a player. cPawn inherits all functions and members of {{centity|centity}}
]],
			Functions =
			{
			},
			Constants =
			{
				TeleportToEntity = { Notes = "void" },
				TeleportTo = { Notes = "void" },
				Heal = { Notes = "void" },
				TakeDamage = { Notes = "void" },
				KilledBy = { Notes = "void" },
				GetHealth = { Notes = "int" },
			},
		},

		cPickup =
		{
			Desc = [[cPickup is a pickup object representation. It is also commonly known as "drops". With this class you could create your own "drop" or modify automatically created.
]],
			Functions =
			{
			},
			Constants =
			{
				cPickup = { Notes = "[[cPickup}}" },
				GetItem = { Notes = "{{cItem|cItem}}" },
				CollectedBy = { Notes = "bool" },
			},
		},

		cPlayer =
		{
			Desc = [[cPlayer describes a human player in the server. cPlayer inherits all functions and members of {{cPawn|cPawn}}
]],
			Functions =
			{
			},
			Constants =
			{
				GetEyeHeight = { Notes = "double" },
				GetEyePosition = { Notes = "{{Vector3d|Vector3d}}" },
				GetFlying = { Notes = "bool" },
				GetStance = { Notes = "double" },
				GetInventory = { Notes = "{{cInventory|cInventory}}" },
				TeleportTo = { Notes = "void" },
				GetGameMode = { Notes = "{{eGameMode|eGameMode}}" },
				GetIP = { Notes = "String" },
				GetLastBlockActionTime = { Notes = "float" },
				GetLastBlockActionCnt = { Notes = "int" },
				SetLastBlockActionCnt = { Notes = "void" },
				SetLastBlockActionTime = { Notes = "void" },
				SetGameMode = { Notes = "void" },
				MoveTo = { Notes = "void" },
				GetClientHandle = { Notes = "{{cClientHandle|cClientHandle}}" },
				SendMessage = { Notes = "void" },
				GetName = { Notes = "String" },
				SetName = { Notes = "void" },
				AddToGroup = { Notes = "void" },
				CanUseCommand = { Notes = "bool" },
				HasPermission = { Notes = "bool" },
				IsInGroup = { Notes = "bool" },
				GetColor = { Notes = "String" },
				TossItem = { Notes = "void" },
				Heal = { Notes = "void" },
				TakeDamage = { Notes = "void" },
				KilledBy = { Notes = "void" },
				Respawn = { Notes = "void" },
				SetVisible = { Notes = "void" },
				IsVisible = { Notes = "bool" },
				MoveToWorld = { Notes = "bool" },
				LoadPermissionsFromDisk = { Notes = "void" },
				GetGroups = { Notes = "list<{{cGroup|cGroup}}>" },
				GetResolvedPermissions = { Notes = "String" },
			},
		},

		cPlugin =
		{
			Desc = [[cPlugin describes a Lua plugin. This page is dedicated to new-style plugins and contain their functions.
]],
			Functions =
			{
			},
			Constants =
			{
				GetName = { Notes = "String" },
				SetName = { Notes = "void" },
				GetVersion = { Notes = "int" },
				SetVersion = { Notes = "void" },
				GetFileName = { Notes = "String" },
				CreateWebPlugin = { Notes = "{{cWebPlugin|cWebPlugin}}" },
			},
		},

		cPluginManager =
		{
			Desc = [[This class is used for generic plugin-related functionality. The plugin manager has a list of all plugins, can enable or disable plugins, manages hook and in-game console commands.
</p>
		<p>There is one instance of cPluginManager in MCServer, to get it, call either {{GetPluginManager|GetPluginManager}}() or cPluginManager:Get() function.
]],
			Functions =
			{
				AddHook = { Params = "{{cPlugin|Plugin}}, HookType", Return = "", Notes = "Adds processing of the specified hook" },
				BindCommand = { Params = "Command, Permission, Callback, HelpString", Return = "", Notes = "Binds an in-game command with the specified callback function, permission and help string" },
				BindConsoleCommand = { Params = "Command, Callback, HelpString", Return = "", Notes = "Binds a console command with the specified callback function and help string" },
				DisablePlugin = { Params = "PluginName", Return = "", Notes = "Disables a plugin specified by its name" },
				ExecuteCommand = { Params = "Player, Command", Return = "bool", Notes = "Executes the command as if given by the specified Player. Checks permissions. Returns true if executed" },
				ExecuteConsoleCommand = { Params = "Command", Return = "bool", Notes = "Executes the command as if given on the server console. Returns true if executed." },
				FindPlugins = { Params = "", Return = "", Notes = "Refreshes the list of plugins to include all folders inside the Plugins folder (potentially new disabled plugins)" },
				ForceExecuteCommand = { Params = "Player, Command", Return = "bool", Notes = "Same as ExecuteCommand, but doesn't check permissions" },
				ForEachCommand = { Params = "Callback", Return = "", Notes = "Calls the Callback function for each command that has been bound using BindCommand()" },
				ForEachConsoleCommand = { Params = "Callback", Return = "", Notes = "Calls the Callback function for each command that has been bound using BindConsoleCommand()" },
				Get = { Params = "", Return = "cPluginManager", Notes = "Returns the single instance of the plugin manager" },
				GetAllPlugins = { Params = "", Return = "PluginTable", Notes = "Returns a table of all plugins, [name => cPlugin] pairs" },
				GetCommandPermission = { Params = "Command", Return = "Permission", Notes = "Returns the permission needed for executing the specified command" },
				GetNumPlugins = { Params = "", Return = "number", Notes = "Returns the number of plugins, including the disabled ones" },
				GetPlugin = { Params = "PluginName", Return = "{{cPlugin|cPlugin}}", Notes = "Returns a plugin handle of the specified plugin" },
				IsCommandBound = { Params = "Command", Return = "boolean", Notes = "Returns true if in-game Command is already bound (by any plugin)" },
				IsConsoleCommandBound = { Params = "Command", Return = "boolean", Notes = "Returns true if console Command is already bound (by any plugin)" },
				LoadPlugin = { Params = "PluginFolder", Return = "", Notes = "Loads a plugin from the specified folder" },
				ReloadPlugins = { Params = "", Return = "", Notes = "Reloads all active plugins" },
			},
			Constants =
			{
			},
		},

		cplugin_newlua =
		{
			Desc = [[]],
			Functions =
			{
			},
			Constants =
			{
			},
		},

		cRoot =
		{
			Desc = [[There is always only one cRoot object in MCServer. cRoot manages all the important objects such as {{cServer|cServer}}
]],
			Functions =
			{
			},
			Constants =
			{
			},
		},

		cServer =
		{
			Desc = [[cServer is typically only used by plugins to broadcast a chat message to all players in the server. Natively however, cServer accepts connections from clients and adds those clients to the game.
]],
			Functions =
			{
			},
			Constants =
			{
			},
		},

		Data =
		{
			Desc = [[<li>Inherits {{cBlockEntity|cBlockEntity}}</li>
A sign entity represents a sign in the world.
Sign entities are saved and loaded from disk when the chunk they reside in is saved or loaded
</p>
		<p>Here's some raw C++ code showing how sign entities are saved
<code cpp>
void cSignEntity::WriteToFile(FILE* a_File)
{
	fwrite( &m_BlockType, sizeof( ENUM_BLOCK_ID ), 1, a_File );
	fwrite( &m_PosX, sizeof( int ), 1, a_File );
	fwrite( &m_PosY, sizeof( int ), 1, a_File );
	fwrite( &m_PosZ, sizeof( int ), 1, a_File );
</p>
		<p>	for( int i = 0; i < 4; i++ )
	{
		short Size = m_Line[i].size();
		fwrite( &Size, sizeof(short), 1, a_File );
		fwrite( m_Line[i].c_str(), Size * sizeof(char), 1, a_File );
	}
}
</code>
]],
			Functions =
			{
			},
			Constants =
			{
			},
		},

		cStringMap =
		{
			Desc = [[cStringMap is an object that maps strings with strings, it's also known as a dictionary
]],
			Functions =
			{
			},
			Constants =
			{
			},
		},

		cTCPLink =
		{
			Desc = [[OBSOLETE, Do not use!
]],
			Functions =
			{
			},
			Constants =
			{
			},
		},

		cTracer =
		{
			Desc = [[A cTracer object is used to trace lines in the world. One thing you can use the cTracer for, is tracing what block a player is looking at, but you can do more with it if you want.
</p>
		<p>The cTracer is still a work in progress
]],
			Functions =
			{
			},
			Constants =
			{
			},
		},

		cWindow =
		{
			Desc = [[This class is the common ancestor for all window classes used by MCServer. It is inherited by the {{cLuaWindow|cLuaWindow}} class that plugins use for opening custom windows. It is planned to be used for window-related hooks in the future. It implements the basic functionality of any window.
</p>
		<p>Note that one cWindow object can be used for multiple players at the same time, and therefore the slot contents are player-specific (e. g. crafting grid, or player inventory). Thus the GetSlot() and SetSlot() functions need to have the {{cPlayer|cPlayer}} parameter that specifies the player for which the contents are to be queried.
]],
			Functions =
			{
				GetWindowID = { Params = "", Return = "number", Notes = "Returns the ID of the window, as used by the network protocol" },
				GetWindowTitle = { Params = "", Return = "string", Notes = "Returns the window title that will be displayed to the player" },
				GetWindowType = { Params = "", Return = "number", Notes = "Returns the type of the window, one of the constants in the table above" },
				IsSlotInPlayerHotbar = { Params = "number", Return = "bool", Notes = "Returns true if the specified slot number is in the player hotbar" },
				IsSlotInPlayerInventory = { Params = "number", Return = "bool", Notes = "Returns true if the specified slot number is in the player's main inventory or in the hotbar. Note that this returns false for armor slots!" },
				IsSlotInPlayerMainInventory = { Params = "number", Return = "bool", Notes = "Returns true if the specified slot number is in the player's main inventory" },
				SetSlot = { Params = "{{cItem|cItem}}", Return = "", Notes = "Sets the contents of the specified slot for the specified player. Ignored if the slot number is invalid" },
				SetWindowTitle = { Params = "string", Return = "", Notes = "Sets the window title that will be displayed to the player" },
			},
			Constants =
			{
				Inventory = { Notes = "" },
				Chest = { Notes = "0" },
				Workbench = { Notes = "1" },
				Furnace = { Notes = "2" },
				DropSpenser = { Notes = "3" },
				Enchantment = { Notes = "4" },
				Brewery = { Notes = "5" },
				NPCTrade = { Notes = "6" },
				Beacon = { Notes = "7" },
				Anvil = { Notes = "8" },
				Hopper = { Notes = "9" },
			},
		},

		cWorld =
		{
			Desc = [[cWorld is the game world, at the moment there can only be one world. The world manages all {{cChunk | Chunks}}, {{cPlayer | Players}} and the time.
]],
			Functions =
			{
			},
			Constants =
			{
			},
		},

		Coordinates =
		{
			Desc = [[The PAK format is highly compact format that has a slightly better compression ratio and saving / loading times than the {{dataformatanvil | Anvil}} format. 
I use this function to convert block coordinates to indices which I use to access the arrays of blocks
<code cpp>
unsigned int cChunk::MakeIndex(int x, int y, int z )
{
	if( x < 16 && x > -1 && y < 128 && y > -1 && z < 16 && z > -1 )
		return y + (z * 128) + (x * 128 * 16);
	return 0;
}
</code>
]],
			Functions =
			{
			},
			Constants =
			{
			},
		},

		eGameMode =
		{
			Desc = [[</p>
		<p>{{eGameMode|eGameMode}} is an enum that defines what game mode a player is in. It can be one of these two values.
</p>
		<p>^ eGameMode ^
| eGameMode_Survival |
| eGameMode_Creative |
]],
			Functions =
			{
			},
			Constants =
			{
			},
		},

		Initialize =
		{
			Desc = [[The Initialize() function is the main entrypoint to a plugin. Within this function the plugin is expected to register any hook callbacks, commands, read settings, add webadmin tabs etc. MCServer calls this function after all the files in the plugin directory have been read and the global values and commands have been executed by the Lua engine, so all the global variables and functions are accessible.
</p>
		<p>Typically, plugins will also store the Plugin parameter into a global variable so that it is accessible later.
]],
			Functions =
			{
			},
			Constants =
			{
			},
		},

		Documented =
		{
			Desc = [[</p>
		<p>A plugin is a script written in Lua that can modify the game in multiple ways.
A list of items regarding plugins that have been documented
</p>
		<p>{{indexmenu>:api:plugin#1}}
]],
			Functions =
			{
			},
			Constants =
			{
			},
		},

		TakeDamageInfo =
		{
			Desc = [[The TakeDamageInfo is a struct that contains the amount of damage, and the entity that caused the damage. It is used in the {{OnTakeDamage|OnTakeDamage}}() hook and in the {{cEntity|cEntity}}'s TakeDamage() function.
]],
			Functions =
			{
			},
			Constants =
			{
			},
		},

		Vector3 =
		{
			Desc = [[Vector3 is a family of classes that all represent a point in space
<li>{{vector3f|vector3f}}</li>
<li>Uses floating point values</li>
<li>{{vector3d|vector3d}}</li>
<li>Uses double precision floating point values</li>
<li>{{vector3i|vector3i}}</li>
<li>Uses fixed point (integer) values</li>
]],
			Functions =
			{
			},
			Constants =
			{
			},
		},

		Vector3d =
		{
			Desc = [[A Vector3d object uses double precision floating point values to describe a point in space. Vector3d is part of the {{vector3|vector3}} family.
]],
			Functions =
			{
			},
			Constants =
			{
			},
		},

		Vector3f =
		{
			Desc = [[A Vector3f object uses floating point values to describe a point in space. Vector3f is part of the {{vector3|vector3}} family.
]],
			Functions =
			{
			},
			Constants =
			{
			},
		},

		Vector3i =
		{
			Desc = [[A Vector3i object uses integer values to describe a point in space. Vector3i is part of the {{vector3|vector3}} family.
]],
			Functions =
			{
			},
			Constants =
			{
			},
		},

		Documented =
		{
			Desc = [[</p>
		<p>WebPlugins provide an interface for the server through a webpage, to be able to use WebPlugins you need to enable WebAdmin in webadmin.ini{{ :api:webadmin.png?200| A WebPlugin in action}}
<code ini>
[WebAdmin]
Enabled=1
Port=8080
</code>
A list of items regarding WebPlugins that have been documented
</p>
		<p>{{indexmenu>:api:webplugin#1}}
]],
			Functions =
			{
			},
			Constants =
			{
			},
		},

	},
	
	
	IgnoreFunctions =
	{
		"globals.assert",
		"globals.collectgarbage",
		"globals.xpcall",
	},
} ;





