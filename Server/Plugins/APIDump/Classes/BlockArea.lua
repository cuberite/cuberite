return
{
	cBlockArea =
	{
		Desc = [[
			This class is used when multiple adjacent blocks are to be manipulated. Because of chunking
			and multithreading, manipulating single blocks using {{cWorld|cWorld:SetBlock}}() is a rather
			time-consuming operation (locks for exclusive access need to be obtained, chunk lookup is done
			for each block), so whenever you need to manipulate multiple adjacent blocks, it's better to wrap
			the operation into a cBlockArea access. cBlockArea is capable of reading / writing across chunk
			boundaries, has no chunk lookups for get and set operations and is not subject to multithreading
			locking (because it is not shared among threads).</p>
			<p>
			cBlockArea remembers its origin (MinX, MinY, MinZ coords in the Read() call) and therefore supports
			absolute as well as relative get / set operations. Despite that, the contents of a cBlockArea can
			be written back into the world at any coords. Most functions in this class come in pair, one that
			works with the absolute coords (what the coords would have been in the original world the area was read
			from) and one (usually with "Rel" in their name) that work on the relative coords (those range from
			zero to Size - 1). Also note that most functions will raise an error if an out-of-range coord is
			supplied to them.</p>
			<p>
			cBlockArea can hold any combination of the following datatypes:<ul>
				<li>block types</li>
				<li>block metas</li>
				<li>blocklight</li>
				<li>skylight</li>
				<li>block entities (only together with block types)</li>
			</ul>
			Read() and Write() functions have parameters that tell the class which datatypes to read / write.
			Note that a datatype that has not been read cannot be written.</p>
			<p>Block entities stored inside a cBlockArea object have their position set to the relative position
			within the area.</p>
			<p>
			Typical usage:<ul>
				<li>Create cBlockArea object</li>
				<li>Read an area from the world / load from file / create anew</li>
				<li>Modify blocks inside cBlockArea</li>
				<li>Write the area back to a world / save to file</li>
			</ul></p>
			<p>
			Calls to any setter of this class will not trigger simulator updates (lava, water, redstone).
			</p>
		]],
		Functions =
		{
			Clear =
			{
				Notes = "Clears the object, resets it to zero size",
			},
			constructor =
			{
				Returns =
				{
					{
						Type = "cBlockArea",
					},
				},
				Notes = "Creates a new empty cBlockArea object",
			},
			CopyFrom =
			{
				Params =
				{
					{
						Name = "BlockAreaSrc",
						Type = "cBlockArea",
					},
				},
				Notes = "Copies contents from BlockAreaSrc into self",
			},
			CopyTo =
			{
				Params =
				{
					{
						Name = "BlockAreaDst",
						Type = "cBlockArea",
					},
				},
				Notes = "Copies contents from self into BlockAreaDst.",
			},
			CountNonAirBlocks =
			{
				Returns =
				{
					{
						Type = "number",
					},
				},
				Notes = "Returns the count of blocks that are not air. Returns 0 if blocktypes not available. Block metas are ignored (if present, air with any meta is still considered air).",
			},
			CountSpecificBlocks =
			{
				{
					Params =
					{
						{
							Name = "BlockType",
							Type = "number",
						},
					},
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Counts the number of occurences of the specified blocktype contained in the area.",
				},
				{
					Params =
					{
						{
							Name = "BlockType",
							Type = "number",
						},
						{
							Name = "BlockMeta",
							Type = "number",
						},
					},
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Counts the number of occurrences of the specified blocktype + blockmeta combination contained in the area.",
				},
			},
			Create =
			{
				{
					Params =
					{
						{
							Name = "SizeX",
							Type = "number",
						},
						{
							Name = "SizeY",
							Type = "number",
						},
						{
							Name = "SizeZ",
							Type = "number",
						},
					},
					Notes = "Initializes this BlockArea to an empty area of the specified size and origin of {0, 0, 0}. Datatypes are set to baTypes + baMetas. Any previous contents are lost.",
				},
				{
					Params =
					{
						{
							Name = "SizeX",
							Type = "number",
						},
						{
							Name = "SizeY",
							Type = "number",
						},
						{
							Name = "SizeZ",
							Type = "number",
						},
						{
							Name = "DataTypes",
							Type = "number",
						},
					},
					Notes = "Initializes this BlockArea to an empty area of the specified size and origin of {0, 0, 0}. Any previous contents are lost.",
				},
				{
					Params =
					{
						{
							Name = "Size",
							Type = "Vector3i",
						},
					},
					Notes = "Creates a new area of the specified size. Datatypes are set to baTypes + baMetas. Origin is set to all zeroes. BlockTypes are set to air, block metas to zero, blocklights to zero and skylights to full light.",
				},
				{
					Params =
					{
						{
							Name = "Size",
							Type = "Vector3i",
						},
						{
							Name = "DataTypes",
							Type = "number",
						},
					},
					Notes = "Creates a new area of the specified size and contents. Origin is set to all zeroes. BlockTypes are set to air, block metas to zero, blocklights to zero and skylights to full light.",
				},
			},
			Crop =
			{
				Params =
				{
					{
						Name = "AddMinX",
						Type = "number",
					},
					{
						Name = "SubMaxX",
						Type = "number",
					},
					{
						Name = "AddMinY",
						Type = "number",
					},
					{
						Name = "SubMaxY",
						Type = "number",
					},
					{
						Name = "AddMinZ",
						Type = "number",
					},
					{
						Name = "SubMaxZ",
						Type = "number",
					},
				},
				Notes = "Crops the specified number of blocks from each border. Modifies the size of this blockarea object.",
			},  -- Crop

			DoWithBlockEntityAt =
			{
				{
					Params =
					{
						{
							Name = "BlockX",
							Type = "number",
						},
						{
							Name = "BlockY",
							Type = "number",
						},
						{
							Name = "BlockZ",
							Type = "number",
						},
						{
							Name = "Callback",
							Type = "function",
						},
					},
					Returns =
					{
						Name = "HasCalled",
						Type = "boolean",
					},
					Notes = "Calls the specified callback with the block entity at the specified absolute coords. The CallbackFunction has the following signature: <pre class=\"prettyprint lang-lua\">function Callback({{cBlockEntity|BlockEntity}})</pre> Returns false if there's no block entity at the specified coords. Returns the value that the callback has returned otherwise.",
				},
				{
					Params =
					{
						{
							Name = "Coords",
							Type = "Vector3i",
						},
						{
							Name = "Callback",
							Type = "function",
						},
					},
					Returns =
					{
						Name = "HasCalled",
						Type = "boolean",
					},
					Notes = "Calls the specified callback with the block entity at the specified absolute coords. The CallbackFunction has the following signature: <pre class=\"prettyprint lang-lua\">function Callback({{cBlockEntity|BlockEntity}})</pre> Returns false if there's no block entity at the specified coords. Returns the value that the callback has returned otherwise.",
				},
			},  -- DoWithBlockEntityAt

			DoWithBlockEntityRelAt =
			{
				{
					Params =
					{
						{
							Name = "RelX",
							Type = "number",
						},
						{
							Name = "RelY",
							Type = "number",
						},
						{
							Name = "RelZ",
							Type = "number",
						},
						{
							Name = "Callback",
							Type = "function",
						},
					},
					Returns =
					{
						Name = "HasCalled",
						Type = "boolean",
					},
					Notes = "Calls the specified callback with the block entity at the specified relative coords. The CallbackFunction has the following signature: <pre class=\"prettyprint lang-lua\">function Callback({{cBlockEntity|BlockEntity}})</pre> Returns false if there's no block entity at the specified coords. Returns the value that the callback has returned otherwise.",
				},
				{
					Params =
					{
						{
							Name = "RelCoords",
							Type = "Vector3i",
						},
						{
							Name = "Callback",
							Type = "function",
						},
					},
					Returns =
					{
						Name = "HasCalled",
						Type = "boolean",
					},
					Notes = "Calls the specified callback with the block entity at the specified relative coords. The CallbackFunction has the following signature: <pre class=\"prettyprint lang-lua\">function Callback({{cBlockEntity|BlockEntity}})</pre> Returns false if there's no block entity at the specified coords. Returns the value that the callback has returned otherwise.",
				},
			},  -- DoWithBlockEntityRelAt

			DumpToRawFile =
			{
				Params =
				{
					{
						Name = "FileName",
						Type = "string",
					},
				},
				Notes = "Dumps the raw data into a file. For debugging purposes only.",
			},

			Expand =
			{
				Params =
				{
					{
						Name = "SubMinX",
						Type = "number",
					},
					{
						Name = "AddMaxX",
						Type = "number",
					},
					{
						Name = "SubMinY",
						Type = "number",
					},
					{
						Name = "AddMaxY",
						Type = "number",
					},
					{
						Name = "SubMinZ",
						Type = "number",
					},
					{
						Name = "AddMaxZ",
						Type = "number",
					},
				},
				Notes = "Expands the specified number of blocks from each border. Modifies the size of this blockarea object. New blocks created with this operation are filled with zeroes.",
			},

			Fill =
			{
				Params =
				{
					{
						Name = "DataTypes",
						Type = "number",
					},
					{
						Name = "BlockType",
						Type = "number",
					},
					{
						Name = "BlockMeta",
						Type = "number",
						IsOptional = true,
					},
					{
						Name = "BlockLight",
						Type = "number",
						IsOptional = true,
					},
					{
						Name = "BlockSkyLight",
						Type = "number",
						IsOptional = true,
					},
				},
				Notes = "Fills the entire block area with the same values, specified. Uses the DataTypes param to determine which content types are modified.",
			},

			FillRelCuboid =
			{
				{
					Params =
					{
						{
							Name = "RelCuboid",
							Type = "cCuboid",
						},
						{
							Name = "DataTypes",
							Type = "number",
						},
						{
							Name = "BlockType",
							Type = "number",
						},
						{
							Name = "BlockMeta",
							Type = "number",
							IsOptional = true,
						},
						{
							Name = "BlockLight",
							Type = "number",
							IsOptional = true,
						},
						{
							Name = "BlockSkyLight",
							Type = "number",
							IsOptional = true,
						},
					},
					Notes = "Fills the specified cuboid (in relative coords) with the same values (like Fill() ).",
				},
				{
					Params =
					{
						{
							Name = "MinRelX",
							Type = "number",
						},
						{
							Name = "MaxRelX",
							Type = "number",
						},
						{
							Name = "MinRelY",
							Type = "number",
						},
						{
							Name = "MaxRelY",
							Type = "number",
						},
						{
							Name = "MinRelZ",
							Type = "number",
						},
						{
							Name = "MaxRelZ",
							Type = "number",
						},
						{
							Name = "DataTypes",
							Type = "number",
						},
						{
							Name = "BlockType",
							Type = "number",
						},
						{
							Name = "BlockMeta",
							Type = "number",
							IsOptional = true,
						},
						{
							Name = "BlockLight",
							Type = "number",
							IsOptional = true,
						},
						{
							Name = "BlockSkyLight",
							Type = "number",
							IsOptional = true,
						},
					},
					Notes = "Fills the specified cuboid with the same values (like Fill() ).",
				},
			},

			ForEachBlockEntity =
			{
				Params =
				{
					{
						Name = "Coords",
						Type = "Vector3i",
					},
					{
						Name = "Callback",
						Type = "function",
					},
				},
				Returns =
				{
					Name = "HasProcessedAll",
					Type = "boolean",
				},
				Notes = "Calls the specified callback with the block entity for each block entity contained in the object. Returns true if all block entities have been processed (including when there are zero block entities), or false if the callback has aborted the enumeration by returning true. The CallbackFunction has the following signature: <pre class=\"prettyprint lang-lua\">function Callback({{cBlockEntity|BlockEntity}})</pre> The callback should return false or no value to continue with the next block entity, or true to abort the enumeration.",
			},  -- ForEachBlockEntity

			GetBlockLight =
			{
				Params =
				{
					{
						Name = "BlockX",
						Type = "number",
					},
					{
						Name = "BlockY",
						Type = "number",
					},
					{
						Name = "BlockZ",
						Type = "number",
					},
				},
				Returns =
				{
					{
						Name = "BlockLight",
						Type = "number",
					},
				},
				Notes = "Returns the blocklight (emissive light) at the specified absolute coords",
			},

			GetBlockMeta =
			{
				Params =
				{
					{
						Name = "BlockX",
						Type = "number",
					},
					{
						Name = "BlockY",
						Type = "number",
					},
					{
						Name = "BlockZ",
						Type = "number",
					},
				},
				Returns =
				{
					{
						Name = "BlockMeta",
						Type = "number",
					},
				},
				Notes = "Returns the block meta at the specified absolute coords",
			},

			GetBlockSkyLight =
			{
				Params =
				{
					{
						Name = "BlockX",
						Type = "number",
					},
					{
						Name = "BlockY",
						Type = "number",
					},
					{
						Name = "BlockZ",
						Type = "number",
					},
				},
				Returns =
				{
					{
						Name = "BlockSkyLight",
						Type = "number",
					},
				},
				Notes = "Returns the skylight at the specified absolute coords",
			},

			GetBlockType =
			{
				Params =
				{
					{
						Name = "BlockX",
						Type = "number",
					},
					{
						Name = "BlockY",
						Type = "number",
					},
					{
						Name = "BlockZ",
						Type = "number",
					},
				},
				Returns =
				{
					{
						Name = "BLOCKTYPE",
						Type = "number",
					},
				},
				Notes = "Returns the block type at the specified absolute coords",
			},

			GetBlockTypeMeta =
			{
				Params =
				{
					{
						Name = "BlockX",
						Type = "number",
					},
					{
						Name = "BlockY",
						Type = "number",
					},
					{
						Name = "BlockZ",
						Type = "number",
					},
				},
				Returns =
				{
					{
						Name = "BlockType",
						Type = "number",
					},
					{
						Name = "BlockMeta",
						Type = "number",
					},
				},
				Notes = "Returns the block type and meta at the specified absolute coords",
			},

			GetBounds =
			{
				Params = {},
				Returns =
				{
					{
						Name = "Bounds",
						Type = "cCuboid",
					}
				},
				Notes = "Returns the {{cCuboid|cuboid}} that specifies the original coords of the world from which the area was read. Basically constructs a {{cCuboid}} out of GetOrigin() and GetOrigin() + GetCoordRange().",
			},

			GetCoordRange =
			{
				Returns =
				{
					{
						Name = "MaxX",
						Type = "number",
					},
					{
						Name = "MaxY",
						Type = "number",
					},
					{
						Name = "MaxZ",
						Type = "number",
					},
				},
				Notes = "Returns the maximum relative coords in all 3 axes. See also GetSize().",
			},

			GetDataTypes =
			{
				Returns =
				{
					{
						Type = "number",
					},
				},
				Notes = "Returns the mask of datatypes (ba* constants added together) that the object is currently holding.",
			},

			GetNonAirCropRelCoords =
			{
				Params =
				{
					{
						Name = "IgnoredBlockType",
						Type = "number",
						IsOptional = true,
					},
				},
				Returns =
				{
					{
						Name = "MinRelX",
						Type = "number",
					},
					{
						Name = "MinRelY",
						Type = "number",
					},
					{
						Name = "MinRelZ",
						Type = "number",
					},
					{
						Name = "MaxRelX",
						Type = "number",
					},
					{
						Name = "MaxRelY",
						Type = "number",
					},
					{
						Name = "MaxRelZ",
						Type = "number",
					},
				},
				Notes = "Returns the minimum and maximum coords in each direction for the first block in each direction of type different to IgnoredBlockType (E_BLOCK_AIR by default). If there are no non-ignored blocks within the area, or blocktypes are not present, the returned values are reverse-ranges (MinX <- m_RangeX, MaxX <- 0 etc.). IgnoreBlockType defaults to air.",
			},

			GetOrigin =
			{
				Returns =
				{
					{
						Name = "OriginX",
						Type = "number",
					},
					{
						Name = "OriginY",
						Type = "number",
					},
					{
						Name = "OriginZ",
						Type = "number",
					},
				},
				Notes = "Returns the origin coords of where the area was read from.",
			},

			GetOriginX =
			{
				Returns =
				{
					{
						Type = "number",
					},
				},
				Notes = "Returns the origin x-coord",
			},

			GetOriginY =
			{
				Returns =
				{
					{
						Type = "number",
					},
				},
				Notes = "Returns the origin y-coord",
			},

			GetOriginZ =
			{
				Returns =
				{
					{
						Type = "number",
					},
				},
				Notes = "Returns the origin z-coord",
			},

			GetRelBlockLight =
			{
				Params =
				{
					{
						Name = "RelBlockX",
						Type = "number",
					},
					{
						Name = "RelBlockY",
						Type = "number",
					},
					{
						Name = "RelBlockZ",
						Type = "number",
					},
				},
				Returns =
				{
					{
						Name = "NIBBLETYPE",
						Type = "number",
					},
				},
				Notes = "Returns the blocklight at the specified relative coords",
			},

			GetRelBlockMeta =
			{
				Params =
				{
					{
						Name = "RelBlockX",
						Type = "number",
					},
					{
						Name = "RelBlockY",
						Type = "number",
					},
					{
						Name = "RelBlockZ",
						Type = "number",
					},
				},
				Returns =
				{
					{
						Name = "NIBBLETYPE",
						Type = "number",
					},
				},
				Notes = "Returns the block meta at the specified relative coords",
			},

			GetRelBlockSkyLight =
			{
				Params =
				{
					{
						Name = "RelBlockX",
						Type = "number",
					},
					{
						Name = "RelBlockY",
						Type = "number",
					},
					{
						Name = "RelBlockZ",
						Type = "number",
					},
				},
				Returns =
				{
					{
						Name = "NIBBLETYPE",
						Type = "number",
					},
				},
				Notes = "Returns the skylight at the specified relative coords",
			},


			GetRelBlockType =
			{
				Params =
				{
					{
						Name = "RelBlockX",
						Type = "number",
					},
					{
						Name = "RelBlockY",
						Type = "number",
					},
					{
						Name = "RelBlockZ",
						Type = "number",
					},
				},
				Returns =
				{
					{
						Name = "BLOCKTYPE",
						Type = "number",
					},
				},
				Notes = "Returns the block type at the specified relative coords",
			},

			GetRelBlockTypeMeta =
			{
				Params =
				{
					{
						Name = "RelBlockX",
						Type = "number",
					},
					{
						Name = "RelBlockY",
						Type = "number",
					},
					{
						Name = "RelBlockZ",
						Type = "number",
					},
				},
				Returns =
				{
					{
						Name = "BLOCKTYPE",
						Type = "number",
					},
					{
						Name = "NIBBLETYPE",
						Type = "number",
					},
				},
				Notes = "Returns the block type and meta at the specified relative coords",
			},

			GetSize =
			{
				Returns =
				{
					{
						Name = "SizeX",
						Type = "number",
					},
					{
						Name = "SizeY",
						Type = "number",
					},
					{
						Name = "SizeZ",
						Type = "number",
					},
				},
				Notes = "Returns the size of the area in all 3 axes. See also GetCoordRange().",
			},
			GetSizeX =
			{
				Returns =
				{
					{
						Type = "number",
					},
				},
				Notes = "Returns the size of the held data in the x-axis",
			},
			GetSizeY =
			{
				Returns =
				{
					{
						Type = "number",
					},
				},
				Notes = "Returns the size of the held data in the y-axis",
			},
			GetSizeZ =
			{
				Returns =
				{
					{
						Type = "number",
					},
				},
				Notes = "Returns the size of the held data in the z-axis",
			},
			GetVolume =
			{
				Returns =
				{
					{
						Type = "number",
					},
				},
				Notes = "Returns the volume of the area - the total number of blocks stored within.",
			},
			GetWEOffset =
			{
				Returns =
				{
					{
						Type = "Vector3i",
					},
				},
				Notes = "Returns the WE offset, a data value sometimes stored in the schematic files. Cuberite doesn't use this value, but provides access to it using this method. The default is {0, 0, 0}.",
			},
			HasBlockEntities =
			{
				Returns =
				{
					{
						Type = "boolean",
					},
				},
				Notes = "Returns true if current datatypes include block entities.",
			},
			HasBlockLights =
			{
				Returns =
				{
					{
						Type = "boolean",
					},
				},
				Notes = "Returns true if current datatypes include blocklight",
			},
			HasBlockMetas =
			{
				Returns =
				{
					{
						Type = "boolean",
					},
				},
				Notes = "Returns true if current datatypes include block metas",
			},
			HasBlockSkyLights =
			{
				Returns =
				{
					{
						Type = "boolean",
					},
				},
				Notes = "Returns true if current datatypes include skylight",
			},
			HasBlockTypes =
			{
				Returns =
				{
					{
						Type = "boolean",
					},
				},
				Notes = "Returns true if current datatypes include block types",
			},

			IsValidCoords =
			{
				{
					Params =
					{
						{
							Name = "BlockX",
							Type = "number",
						},
						{
							Name = "BlockY",
							Type = "number",
						},
						{
							Name = "BlockZ",
							Type = "number",
						},
					},
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Returns true if the specified absolute coords are within the area.",
				},
				{
					Params =
					{
						{
							Name = "Coords",
							Type = "Vector3i",
						},
					},
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Returns true if the specified absolute coords are within the area.",
				},
			},  -- IsValidCoords

			IsValidDataTypeCombination =
			{
				IsStatic = true,
				Params =
				{
					{
						Name = "DataTypes",
						Type = "number",
					},
				},
				Returns =
				{
					{
						Type = "boolean",
					},
				},
				Notes = "Returns true if the specified combination of datatypes (ba* constants added together) is valid. Most combinations are valid, but for example baBlockEntities without baTypes is an invalid combination.",
			},  -- IsValidDataTypeCombination

			IsValidRelCoords =
			{
				{
					Params =
					{
						{
							Name = "RelBlockX",
							Type = "number",
						},
						{
							Name = "RelBlockY",
							Type = "number",
						},
						{
							Name = "RelBlockZ",
							Type = "number",
						},
					},
					Returns =
					{
						{
							Type = "boolean",
						}
					},
					Notes = "Returns true if the specified relative coords are within the area.",
				},
				{
					Params =
					{
						{
							Name = "RelCoords",
							Type = "Vector3i",
						},
					},
					Returns =
					{
						{
							Type = "boolean",
						}
					},
					Notes = "Returns true if the specified relative coords are within the area.",
				},
			},  -- IsValidRelCoords

			LoadFromSchematicFile =
			{
				Params =
				{
					{
						Name = "FileName",
						Type = "string",
					},
				},
				Returns =
				{
					{ Type = "boolean" },
				},
				Notes = "Clears current content and loads new content from the specified schematic file. Returns true if successful. Returns false and logs error if unsuccessful, old content is preserved in such a case.",
			},
			LoadFromSchematicString =
			{
				Params =
				{
					{
						Name = "SchematicData",
						Type = "string",
					},
				},
				Returns =
				{
					{ Type = "boolean" },
				},
				Notes = "Clears current content and loads new content from the specified string (assumed to contain .schematic data). Returns true if successful. Returns false and logs error if unsuccessful, old content is preserved in such a case.",
			},
			Merge =
			{
				{
					Params =
					{
						{
							Name = "BlockAreaSrc",
							Type = "cBlockArea",
						},
						{
							Name = "RelMinCoords",
							Type = "number",
						},
						{
							Name = "Strategy",
							Type = "string",
						},
					},
					Notes = "Merges BlockAreaSrc into this object at the specified relative coords, using the specified strategy",
				},
				{
					Params =
					{
						{
							Name = "BlockAreaSrc",
							Type = "cBlockArea",
						},
						{
							Name = "RelX",
							Type = "number",
						},
						{
							Name = "RelY",
							Type = "number",
						},
						{
							Name = "RelZ",
							Type = "number",
						},
						{
							Name = "Strategy",
							Type = "string",
						},
					},
					Notes = "Merges BlockAreaSrc into this object at the specified relative coords, using the specified strategy",
				},
			},
			MirrorXY =
			{
				Notes = "Mirrors this block area around the XY plane. Modifies blocks' metas (if present) to match (i. e. furnaces facing the opposite direction).",
			},
			MirrorXYNoMeta =
			{
				Notes = "Mirrors this block area around the XY plane. Doesn't modify blocks' metas.",
			},
			MirrorXZ =
			{
				Notes = "Mirrors this block area around the XZ plane. Modifies blocks' metas (if present)",
			},
			MirrorXZNoMeta =
			{
				Notes = "Mirrors this block area around the XZ plane. Doesn't modify blocks' metas.",
			},
			MirrorYZ =
			{
				Notes = "Mirrors this block area around the YZ plane. Modifies blocks' metas (if present)",
			},
			MirrorYZNoMeta =
			{
				Notes = "Mirrors this block area around the YZ plane. Doesn't modify blocks' metas.",
			},
			Read =
			{
				{
					Params =
					{
						{
							Name = "World",
							Type = "cWorld",
						},
						{
							Name = "Cuboid",
							Type = "cCuboid",
						},
					},
					Returns =
					{
						{
							Name = "IsSuccess",
							Type = "boolean",
						},
					},
					Notes = "Reads the area from World, returns true if successful. baTypes and baMetas are read.",
				},
				{
					Params =
					{
						{
							Name = "World",
							Type = "cWorld",
						},
						{
							Name = "Cuboid",
							Type = "cCuboid",
						},
						{
							Name = "DataTypes",
							Type = "number",
						},
					},
					Returns =
					{
						{
							Name = "IsSuccess",
							Type = "boolean",
						},
					},
					Notes = "Reads the area from World, returns true if successful. DataTypes is the sum of baXXX datatypes to be read",
				},
				{
					Params =
					{
						{
							Name = "World",
							Type = "cWorld",
						},
						{
							Name = "Point1",
							Type = "Vector3i",
						},
						{
							Name = "Point2",
							Type = "Vector3i",
						},
					},
					Returns =
					{
						{
							Name = "IsSuccess",
							Type = "boolean",
						},
					},
					Notes = "Reads the area from World, returns true if successful. baTypes and baMetas are read.",
				},
				{
					Params =
					{
						{
							Name = "World",
							Type = "cWorld",
						},
						{
							Name = "Point1",
							Type = "Vector3i",
						},
						{
							Name = "Point2",
							Type = "Vector3i",
						},
						{
							Name = "DataTypes",
							Type = "number",
						},
					},
					Returns =
					{
						{
							Name = "IsSuccess",
							Type = "boolean",
						},
					},
					Notes = "Reads the area from World, returns true if successful. DataTypes is a sum of baXXX datatypes to be read.",
				},
				{
					Params =
					{
						{
							Name = "World",
							Type = "cWorld",
						},
						{
							Name = "MinX",
							Type = "number",
						},
						{
							Name = "MaxX",
							Type = "number",
						},
						{
							Name = "MinY",
							Type = "number",
						},
						{
							Name = "MaxY",
							Type = "number",
						},
						{
							Name = "MinZ",
							Type = "number",
						},
						{
							Name = "MaxZ",
							Type = "number",
						},
					},
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Reads the area from World, returns true if successful. baTypes and baMetas are read.",
				},
				{
					Params =
					{
						{
							Name = "World",
							Type = "cWorld",
						},
						{
							Name = "MinX",
							Type = "number",
						},
						{
							Name = "MaxX",
							Type = "number",
						},
						{
							Name = "MinY",
							Type = "number",
						},
						{
							Name = "MaxY",
							Type = "number",
						},
						{
							Name = "MinZ",
							Type = "number",
						},
						{
							Name = "MaxZ",
							Type = "number",
						},
						{
							Name = "DataTypes",
							Type = "number",
						},
					},
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Reads the area from World, returns true if successful. DataTypes is a sum of baXXX datatypes to read.",
				},
			},
			RelLine =
			{
				{
					Params =
					{
						{
							Name = "RelPoint1",
							Type = "Vector3i",
						},
						{
							Name = "RelPoint2",
							Type = "Vector3i",
						},
						{
							Name = "DataTypes",
							Type = "number",
						},
						{
							Name = "BlockType",
							Type = "number",
						},
						{
							Name = "BlockMeta",
							Type = "number",
							IsOptional = true,
						},
						{
							Name = "BlockLight",
							Type = "number",
							IsOptional = true,
						},
						{
							Name = "BlockSkyLight",
							Type = "number",
							IsOptional = true,
						},
					},
					Notes = "Draws a line between the two specified points. Sets only datatypes specified by DataTypes (baXXX constants).",
				},
				{
					Params =
					{
						{
							Name = "RelX1",
							Type = "number",
						},
						{
							Name = "RelY1",
							Type = "number",
						},
						{
							Name = "RelZ1",
							Type = "number",
						},
						{
							Name = "RelX2",
							Type = "number",
						},
						{
							Name = "RelY2",
							Type = "number",
						},
						{
							Name = "RelZ2",
							Type = "number",
						},
						{
							Name = "DataTypes",
							Type = "number",
						},
						{
							Name = "BlockType",
							Type = "number",
						},
						{
							Name = "BlockMeta",
							Type = "number",
							IsOptional = true,
						},
						{
							Name = "BlockLight",
							Type = "number",
							IsOptional = true,
						},
						{
							Name = "BlockSkyLight",
							Type = "number",
							IsOptional = true,
						},
					},
					Notes = "Draws a line between the two specified points. Sets only datatypes specified by DataTypes (baXXX constants).",
				},
			},
			RotateCCW =
			{
				Notes = "Rotates the block area around the Y axis, counter-clockwise (east -> north). Modifies blocks' metas (if present) to match.",
			},
			RotateCCWNoMeta =
			{
				Notes = "Rotates the block area around the Y axis, counter-clockwise (east -> north). Doesn't modify blocks' metas.",
			},
			RotateCW =
			{
				Notes = "Rotates the block area around the Y axis, clockwise (north -> east). Modifies blocks' metas (if present) to match.",
			},
			RotateCWNoMeta =
			{
				Notes = "Rotates the block area around the Y axis, clockwise (north -> east). Doesn't modify blocks' metas.",
			},
			SaveToSchematicFile =
			{
				Params =
				{
					{
						Name = "FileName",
						Type = "string",
					},
				},
				Returns =
				{
					{
						Type = "boolean",
					},
				},
				Notes = "Saves the current contents to a schematic file. Returns true if successful.",
			},
			SaveToSchematicString =
			{
				Returns =
				{
					{
						Type = "string",
					},
				},
				Notes = "Saves the current contents to a string (in a .schematic file format). Returns the data if successful, nil if failed.",
			},
			SetBlockLight =
			{
				Params =
				{
					{
						Name = "BlockX",
						Type = "number",
					},
					{
						Name = "BlockY",
						Type = "number",
					},
					{
						Name = "BlockZ",
						Type = "number",
					},
					{
						Name = "BlockLight",
						Type = "number",
					},
				},
				Notes = "Sets the blocklight at the specified absolute coords",
			},
			SetBlockMeta =
			{
				Params =
				{
					{
						Name = "BlockX",
						Type = "number",
					},
					{
						Name = "BlockY",
						Type = "number",
					},
					{
						Name = "BlockZ",
						Type = "number",
					},
					{
						Name = "BlockMeta",
						Type = "number",
					},
				},
				Notes = "Sets the block meta at the specified absolute coords.",
			},
			SetBlockSkyLight =
			{
				Params =
				{
					{
						Name = "BlockX",
						Type = "number",
					},
					{
						Name = "BlockY",
						Type = "number",
					},
					{
						Name = "BlockZ",
						Type = "number",
					},
					{
						Name = "BlockSkyLight",
						Type = "number",
					},
				},
				Notes = "Sets the skylight at the specified absolute coords",
			},
			SetBlockType =
			{
				Params =
				{
					{
						Name = "BlockX",
						Type = "number",
					},
					{
						Name = "BlockY",
						Type = "number",
					},
					{
						Name = "BlockZ",
						Type = "number",
					},
					{
						Name = "BlockType",
						Type = "number",
					},
				},
				Notes = "Sets the block type at the specified absolute coords",
			},
			SetBlockTypeMeta =
			{
				Params =
				{
					{
						Name = "BlockX",
						Type = "number",
					},
					{
						Name = "BlockY",
						Type = "number",
					},
					{
						Name = "BlockZ",
						Type = "number",
					},
					{
						Name = "BlockType",
						Type = "number",
					},
					{
						Name = "BlockMeta",
						Type = "number",
					},
				},
				Notes = "Sets the block type and meta at the specified absolute coords",
			},
			SetOrigin =
			{
				{
					Params =
					{
						{
							Name = "Origin",
							Type = "Vector3i",
						},
					},
					Notes = "Resets the origin for the absolute coords. Only affects how absolute coords are translated into relative coords.",
				},
				{
					Params =
					{
						{
							Name = "OriginX",
							Type = "number",
						},
						{
							Name = "OriginY",
							Type = "number",
						},
						{
							Name = "OriginZ",
							Type = "number",
						},
					},
					Notes = "Resets the origin for the absolute coords. Only affects how absolute coords are translated into relative coords.",
				},
			},
			SetRelBlockLight =
			{
				Params =
				{
					{
						Name = "RelBlockX",
						Type = "number",
					},
					{
						Name = "RelBlockY",
						Type = "number",
					},
					{
						Name = "RelBlockZ",
						Type = "number",
					},
					{
						Name = "BlockLight",
						Type = "number",
					},
				},
				Notes = "Sets the blocklight at the specified relative coords",
			},
			SetRelBlockMeta =
			{
				Params =
				{
					{
						Name = "RelBlockX",
						Type = "number",
					},
					{
						Name = "RelBlockY",
						Type = "number",
					},
					{
						Name = "RelBlockZ",
						Type = "number",
					},
					{
						Name = "BlockMeta",
						Type = "number",
					},
				},
				Notes = "Sets the block meta at the specified relative coords",
			},
			SetRelBlockSkyLight =
			{
				Params =
				{
					{
						Name = "RelBlockX",
						Type = "number",
					},
					{
						Name = "RelBlockY",
						Type = "number",
					},
					{
						Name = "RelBlockZ",
						Type = "number",
					},
					{
						Name = "BlockSkyLight",
						Type = "number",
					},
				},
				Notes = "Sets the skylight at the specified relative coords",
			},
			SetRelBlockType =
			{
				Params =
				{
					{
						Name = "RelBlockX",
						Type = "number",
					},
					{
						Name = "RelBlockY",
						Type = "number",
					},
					{
						Name = "RelBlockZ",
						Type = "number",
					},
					{
						Name = "BlockType",
						Type = "number",
					},
				},
				Notes = "Sets the block type at the specified relative coords",
			},
			SetRelBlockTypeMeta =
			{
				Params =
				{
					{
						Name = "RelBlockX",
						Type = "number",
					},
					{
						Name = "RelBlockY",
						Type = "number",
					},
					{
						Name = "RelBlockZ",
						Type = "number",
					},
					{
						Name = "BlockType",
						Type = "number",
					},
					{
						Name = "BlockMeta",
						Type = "number",
					},
				},
				Notes = "Sets the block type and meta at the specified relative coords",
			},
			SetWEOffset =
			{
				{
					Params =
					{
						{
							Name = "Offset",
							Type = "Vector3i",
						},
					},
					Notes = "Sets the WE offset, a data value sometimes stored in the schematic files. Mostly used for WorldEdit. Cuberite doesn't use this value, but provides access to it using this method.",
				},
				{
					Params =
					{
						{
							Name = "OffsetX",
							Type = "number",
						},
						{
							Name = "OffsetY",
							Type = "number",
						},
						{
							Name = "OffsetZ",
							Type = "number",
						},
					},
					Notes = "Sets the WE offset, a data value sometimes stored in the schematic files. Mostly used for WorldEdit. Cuberite doesn't use this value, but provides access to it using this method.",
				},
			},
			Write =
			{
				{
					Params =
					{
						{
							Name = "World",
							Type = "cWorld",
						},
						{
							Name = "MinPoint",
							Type = "Vector3i",
						},
					},
					Returns =
					{
						{
							Name = "IsSuccess",
							Type = "boolean",
						},
					},
					Notes = "Writes the area into World at the specified coords, returns true if successful. All present data types are written.",
				},
				{
					Params =
					{
						{
							Name = "World",
							Type = "cWorld",
						},
						{
							Name = "MinPoint",
							Type = "Vector3i",
						},
						{
							Name = "DataTypes",
							Type = "number",
						},
					},
					Returns =
					{
						{
							Name = "IsSuccess",
							Type = "boolean",
						},
					},
					Notes = "Writes the area into World at the specified coords, returns true if successful. DataTypes is the sum of baXXX datatypes to write.",
				},
				{
					Params =
					{
						{
							Name = "World",
							Type = "cWorld",
						},
						{
							Name = "MinX",
							Type = "number",
						},
						{
							Name = "MinY",
							Type = "number",
						},
						{
							Name = "MinZ",
							Type = "number",
						},
					},
					Returns =
					{
						{
							Name = "IsSuccess",
							Type = "boolean",
						},
					},
					Notes = "Writes the area into World at the specified coords, returns true if successful. All present data types are written.",
				},
				{
					Params =
					{
						{
							Name = "World",
							Type = "cWorld",
						},
						{
							Name = "MinX",
							Type = "number",
						},
						{
							Name = "MinY",
							Type = "number",
						},
						{
							Name = "MinZ",
							Type = "number",
						},
						{
							Name = "DataTypes",
							Type = "number",
						},
					},
					Returns =
					{
						{
							Name = "IsSuccess",
							Type = "boolean",
						},
					},
					Notes = "Writes the area into World at the specified coords, returns true if successful. DataTypes is the sum of baXXX datatypes to write.",
				},
			},
		},
		Constants =
		{
			baBlockEntities =
			{
				Notes = "Operations should work on block entities. Note that this flag is invalid without baTypes.",
			},
			baLight =
			{
				Notes = "Operations should work on block (emissive) light",
			},
			baMetas =
			{
				Notes = "Operations should work on block metas",
			},
			baSkyLight =
			{
				Notes = "Operations should work on skylight",
			},
			baTypes =
			{
				Notes = "Operation should work on block types",
			},
			msDifference =
			{
				Notes = "Block becomes air if 'self' and src are the same. Otherwise it becomes the src block.",
			},
			msFillAir =
			{
				Notes = "'self' is overwritten by Src only where 'self' has air blocks",
			},
			msImprint =
			{
				Notes = "Src overwrites 'self' anywhere where 'self' has non-air blocks",
			},
			msLake =
			{
				Notes = "Special mode for merging lake images",
			},
			msMask =
			{
				Notes = "The blocks that are exactly the same are kept in 'self', all differing blocks are replaced by air",
			},
			msOverwrite =
			{
				Notes = "Src overwrites anything in 'self'",
			},
			msSimpleCompare =
			{
				Notes = "The blocks that are exactly the same are replaced with air, all differing blocks are replaced by stone",
			},
			msSpongePrint =
			{
				Notes = "Similar to msImprint, sponge block doesn't overwrite anything, all other blocks overwrite everything",
			},
		},
		ConstantGroups =
		{
			BATypes =
			{
				Include = "ba.*",
				TextBefore = [[
					The following constants are used to signalize the datatype to read or write:
				]],
			},
			eMergeStrategy =
			{
				Include = "ms.*",
				TextAfter = "See below for a detailed explanation of the individual merge strategies.",
				TextBefore = [[
					The Merge() function can use different strategies to combine the source and destination blocks.
					The following constants are used:
				]],
			},
		},
		AdditionalInfo =
		{
			{
				Header = "Merge strategies",
				Contents = [[
					<p>The strategy parameter specifies how individual blocks are combined together, using the table below.
					</p>
					<table class="inline">
					<tbody><tr>
					<th colspan="2">area block</th><th colspan="3">result</th>
					</tr>
					<tr>
					<th> this </th><th> Src </th><th> msOverwrite </th><th> msFillAir </th><th> msImprint </th>
					</tr>
					<tr>
					<td> air </td><td> air </td><td> air </td><td> air </td><td> air </td>
					</tr>
					<tr>
					<td> A </td><td> air </td><td> air </td><td> A </td><td> A </td>
					</tr>
					<tr>
					<td> air </td><td> B </td><td> B </td><td> B </td><td> B </td>
					</tr>
					<tr>
					<td> A </td><td> B </td><td> B </td><td> A </td><td> B </td>
					</tr>
					<tr>
					<td> A </td><td> A </td><td> A </td><td> A </td><td> A </td>
					</td>
					</tbody></table>

					<p>
					So to sum up:
					<ol>
					<li class="level1">msOverwrite completely overwrites all blocks with the Src's blocks</li>
					<li class="level1">msFillAir overwrites only those blocks that were air</li>
					<li class="level1">msImprint overwrites with only those blocks that are non-air</li>
					</ol>
					</p>

					<h3>Special strategies</h3>
					<p>For each strategy, evaluate the table rows from top downwards, the first match wins.</p>

					<p>
					<strong>msDifference</strong> - changes all the blocks which are the same to air. Otherwise the source block gets placed.
					</p>
					<table><tbody<tr>
					<th colspan="2"> area block </th><th> </th><th> Notes </th>
					</tr><tr>
					<td> * </td><td> B </td><td> B </td><td> The blocks are different so we use block B </td>
					</tr><tr>
					<td> B </td><td> B </td><td> Air </td><td> The blocks are the same so we get air. </td>
					</tr>
					</tbody></table>


					<p>
					<strong>msLake</strong> - used for merging areas with lava and water lakes, in the appropriate generator.
					</p>
					<table><tbody><tr>
					<th colspan="2"> area block </th><th> </th><th> Notes </th>
					</tr><tr>
					<th> self </th><th> Src </th><th> result </th><th> </th>
					</tr><tr>
					<td> A </td><td> sponge </td><td> A </td><td> Sponge is the NOP block </td>
					</tr><tr>
					<td> *        </td><td> air    </td><td> air    </td><td> Air always gets hollowed out, even under the oceans </td>
					</tr><tr>
					<td> water    </td><td> *      </td><td> water  </td><td> Water is never overwritten </td>
					</tr><tr>
					<td> lava     </td><td> *      </td><td> lava   </td><td> Lava is never overwritten </td>
					</tr><tr>
					<td> *        </td><td> water  </td><td> water  </td><td> Water always overwrites anything </td>
					</tr><tr>
					<td> *        </td><td> lava   </td><td> lava   </td><td> Lava always overwrites anything </td>
					</tr><tr>
					<td> dirt     </td><td> stone  </td><td> stone  </td><td> Stone overwrites dirt </td>
					</tr><tr>
					<td> grass    </td><td> stone  </td><td> stone  </td><td> ... and grass </td>
					</tr><tr>
					<td> mycelium </td><td> stone  </td><td> stone  </td><td> ... and mycelium </td>
					</tr><tr>
					<td> A        </td><td> stone  </td><td> A      </td><td> ... but nothing else </td>
					</tr><tr>
					<td> A        </td><td> *      </td><td> A      </td><td> Everything else is left as it is </td>
					</tr>
					</tbody></table>

					<p>
					<strong>msSpongePrint</strong> - used for most prefab-generators to merge the prefabs. Similar to
					msImprint, but uses the sponge block as the NOP block instead, so that the prefabs may carve out air
					pockets, too.
					</p>
					<table><tbody><tr>
					<th colspan="2"> area block </th><th> </th><th> Notes </th>
					</tr><tr>
					<th> self </th><th> Src </th><th> result </th><th> </th>
					</tr><tr>
					<td> A </td><td> sponge </td><td> A </td><td> Sponge is the NOP block </td>
					</tr><tr>
					<td> * </td><td> B </td><td> B </td><td> Everything else overwrites anything </td>
					</tr>
					</tbody></table>

					<p>
					<strong>msMask</strong> - the blocks that are the same in the other area are kept, all the
					differing blocks are replaced with air. Meta is used in the comparison, too, two blocks of the
					same type but different meta are considered different and thus replaced with air.
					</p>
					<table><tbody><tr>
					<th colspan="2"> area block </th><th> </th><th> Notes </th>
					</tr><tr>
					<th> self </th><th> Src </th><th> result </th><th> </th>
					</tr><tr>
					<td> A </td><td> A </td><td> A </td><td> Same blocks are kept </td>
					</tr><tr>
					<td> A </td><td> non-A </td><td> air </td><td> Differing blocks are replaced with air </td>
					</tr>
					</tbody></table>

					<p>
					<strong>msDifference</strong> - the blocks that are the same in both areas are replaced with air, all the
					differing blocks are kept from the first area. Meta is used in the comparison, too, two blocks of the
					same type but different meta are considered different.
					</p>
					<table><tbody><tr>
					<th colspan="2"> area block </th><th> </th><th> Notes </th>
					</tr><tr>
					<th> self </th><th> Src </th><th> result </th><th> </th>
					</tr><tr>
					<td> A </td><td> A </td><td> air </td><td> Same blocks are replaced with air </td>
					</tr><tr>
					<td> A </td><td> non-A </td><td> A </td><td> Differing blocks are kept from 'self' </td>
					</tr>
					</tbody></table>

					<p>
					<strong>msSimpleCompare</strong> - the blocks that are the same in both areas are replaced with air, all the
					differing blocks are replaced with stone. Meta is used in the comparison, too, two blocks of the
					same type but different meta are considered different.
					</p>
					<table><tbody><tr>
					<th colspan="2"> area block </th><th> </th><th> Notes </th>
					</tr><tr>
					<th> self </th><th> Src </th><th> result </th><th> </th>
					</tr><tr>
					<td> A </td><td> A </td><td> air </td><td> Same blocks are replaced with air </td>
					</tr><tr>
					<td> A </td><td> non-A </td><td> stone </td><td> Differing blocks are replaced with stone </td>
					</tr>
					</tbody></table>
]],
			},
		},
	},
}
