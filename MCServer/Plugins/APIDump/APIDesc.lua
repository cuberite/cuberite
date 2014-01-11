-- APIDesc.lua

-- Contains the API objects' descriptions




g_APIDesc =
{
	Classes =
	{
		--[[
		-- What the APIDump plugin understands / how to document stuff:
		ExampleClassName =
		{
			Desc = "Description, exported as the first paragraph of the class page. Usually enclosed within double brackets."

			Functions =
			{
				FunctionName = { Params = "Parameter list", Return = "Return values list", Notes = "Notes" ),
				OverloadedFunctionName =  -- When a function supports multiple parameter variants
				{
					{ Params = "Parameter list 1", Return = "Return values list 1", Notes = "Notes 1" },
					{ Params = "Parameter list 2", Return = "Return values list 2", Notes = "Notes 2" },
				}
			} ,

			Constants =
			{
				ConstantName = { Notes = "Notes about the constant" },
			} ,
			
			ConstantGroups =
			{
				GroupName1 =  -- GroupName1 is used as the HTML anchor name
				{
					Include = {"constant1", "constant2", "const_.*"},  -- Constants to include in this group, array of identifiers, accepts wildcards
					TextBefore = "This text will be written in front of the constant list",
					TextAfter = "This text will be written after the constant list",
					ShowInDescendants = false,  -- If false, descendant classes won't list these constants
				}
			},

			Variables =
			{
				VariableName = { Type = "string", Notes = "Notes about the variable" },
			} ,

			AdditionalInfo =  -- Paragraphs to be exported after the function definitions table
			{
				{
					Header = "Header 1",
					Contents = "Contents of the additional section 1",
				},
				{
					Header = "Header 2",
					Contents = "Contents of the additional section 2",
				}
			},

			Inherits = "ParentClassName",  -- Only present if the class inherits from another API class
		},
		]]--

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
					<li>Read an area from the world / load from file / create anew</li>
					<li>Modify blocks inside cBlockArea</li>
					<li>Write the area back to a world / save to file</li>
				</ul></p>
			]],
			Functions =
			{
				constructor = { Params = "", Return = "cBlockArea", Notes = "Creates a new empty cBlockArea object" },
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
				SetBlockTypeMeta = { Params = "BlockX, BlockY, BlockZ, BlockType, BlockMeta", Return = "", Notes = "Sets the block type and meta at the specified absolute coords" },
				SetOrigin = { Params = "OriginX, OriginY, OriginZ", Return = "", Notes = "Resets the origin for the absolute coords. Only affects how absolute coords are translated into relative coords." },
				SetRelBlockLight = { Params = "RelBlockX, RelBlockY, RelBlockZ, BlockLight", Return = "", Notes = "Sets the blocklight at the specified relative coords" },
				SetRelBlockMeta = { Params = "RelBlockX, RelBlockY, RelBlockZ, BlockMeta", Return = "", Notes = "Sets the block meta at the specified relative coords" },
				SetRelBlockSkyLight = { Params = "RelBlockX, RelBlockY, RelBlockZ, SkyLight", Return = "", Notes = "Sets the skylight at the specified relative coords" },
				SetRelBlockType = { Params = "RelBlockX, RelBlockY, RelBlockZ, BlockType", Return = "", Notes = "Sets the block type at the specified relative coords" },
				SetRelBlockTypeMeta = { Params = "RelBlockX, RelBlockY, RelBlockZ, BlockType, BlockMeta", Return = "", Notes = "Sets the block type and meta at the specified relative coords" },
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
			ConstantGroups =
			{
				BATypes =
				{
					Include = "ba.*",
					TextBefore = [[
						The following constants are used to signalize the datatype to read or write:
					]],
				},
				MergeStrategies =
				{
					Include = "ms.*",
					TextBefore = [[
						The Merge() function can use different strategies to combine the source and destination blocks.
						The following constants are used:
					]],
					TextAfter = "See below for a detailed explanation of the individual merge strategies.",
				},
			},
			AdditionalInfo =
			{
				{
					Header = "Merge strategies",
					Contents =
					[[
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
						</tbody></table>

						<p>
						So to sum up:
						<ol>
						<li class="level1">msOverwrite completely overwrites all blocks with the Src's blocks</li>
						<li class="level1">msFillAir overwrites only those blocks that were air</li>
						<li class="level1">msImprint overwrites with only those blocks that are non-air</li>
						</ol>
						</p>

						<p>
						Special strategies:
						</p>

						<p>
						<strong>msLake</strong> (evaluate top-down, first match wins):
						</p>
						<table><tbody><tr>
						<th colspan="2"> area block </th><th> </th><th> Notes </th>
						</tr><tr>
						<th> this </th><th> Src </th><th> result </th><th> </th>
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
					]],
				},  -- Merge strategies
			},  -- AdditionalInfo
		},  -- cBlockArea

		cBoundingBox =
		{
			Desc = [[
			Represents two sets of coordinates, minimum and maximum for each direction; thus defining an
			axis-aligned cuboid with floating-point boundaries. It supports operations changing the size and
			position of the box, as well as querying whether a point or another BoundingBox is inside the box.</p>
			<p>
			All the points within the coordinate limits (inclusive the edges) are considered "inside" the box.
			However, for intersection purposes, if the intersection is "sharp" in any coord (min1 == max2, i. e.
			zero volume), the boxes are considered non-intersecting.</p>
			]],
			Functions =
			{
				constructor =
				{
					{ Params = "MinX, MaxX, MinY, MaxY, MinZ, MaxZ", Return = "cBoundingBox", Notes = "Creates a new bounding box with the specified edges" },
					{ Params = "{{Vector3d|Min}}, {{Vector3d|Max}}", Return = "cBoundingBox", Notes = "Creates a new bounding box with the coords specified as two vectors" },
					{ Params = "{{Vector3d|Pos}}, Radius, Height", Return = "cBoundingBox", Notes = "Creates a new bounding box from the position given and radius (X/Z) and height. Radius is added from X/Z to calculate the maximum coords and subtracted from X/Z to get the minimum; minimum Y is set to Pos.y and maxumim Y to Pos.y plus Height. This corresponds with how {{cEntity|entities}} are represented in Minecraft." },
					{ Params = "OtherBoundingBox", Return = "cBoundingBox", Notes = "Creates a new copy of the given bounding box. Same result can be achieved by using a simple assignment." },
				},
				CalcLineIntersection = { Params = "{{Vector3d|LineStart}}, {{Vector3d|LinePt2}}", Return = "DoesIntersect, LineCoeff, Face", Notes = "Calculates the intersection of a ray (half-line), given by two of its points, with the bounding box. Returns false if the line doesn't intersect the bounding box, or true, together with coefficient of the intersection (how much of the difference between the two ray points is needed to reach the intersection), and the face of the box which is intersected.<br /><b>TODO</b>: Lua binding for this function is wrong atm." },
				DoesIntersect = { Params = "OtherBoundingBox", Return = "bool", Notes = "Returns true if the two bounding boxes have an intersection of nonzero volume." },
				Expand = { Params = "ExpandX, ExpandY, ExpandZ", Return = "", Notes = "Expands this bounding box by the specified amount in each direction (so the box becomes larger by 2 * Expand in each axis)." },
				IsInside =
				{
					{ Params = "{{Vector3d|Point}}", Return = "bool", Notes = "Returns true if the specified point is inside (including on the edge) of the box." },
					{ Params = "PointX, PointY, PointZ", Return = "bool", Notes = "Returns true if the specified point is inside (including on the edge) of the box." },
					{ Params = "OtherBoundingBox", Return = "bool", Notes = "Returns true if OtherBoundingBox is inside of this box." },
					{ Params = "{{Vector3d|OtherBoxMin}}, {{Vector3d|OtherBoxMax}}", Return = "bool", Notes = "Returns true if the other bounding box, specified by its 2 corners, is inside of this box." },
				},
				Move =
				{
					{ Params = "OffsetX, OffsetY, OffsetZ", Return = "", Notes = "Moves the bounding box by the specified offset in each axis" },
					{ Params = "{{Vector3d|Offset}}", Return = "", Notes = "Moves the bounding box by the specified offset in each axis" },
				},
				Union = { Params = "OtherBoundingBox", Return = "cBoundingBox", Notes = "Returns the smallest bounding box that contains both OtherBoundingBox and this bounding box. Note that unlike the strict geometrical meaning of \"union\", this operation actually returns a cBoundingBox." },
			},
		},

		cChatColor =
		{
			Desc = [[
				A wrapper class for constants representing colors or effects.
			]],

			Functions = {},
			Constants =
			{
				Black         = { Notes = "" },
				Blue          = { Notes = "" },
				Bold          = { Notes = "" },
				Color         = { Notes = "The first character of the color-code-sequence, �" },
				DarkPurple    = { Notes = "" },
				Delimiter     = { Notes = "The first character of the color-code-sequence, �" },
				Gold          = { Notes = "" },
				Gray          = { Notes = "" },
				Green         = { Notes = "" },
				Italic        = { Notes = "" },
				LightBlue     = { Notes = "" },
				LightGray     = { Notes = "" },
				LightGreen    = { Notes = "" },
				LightPurple   = { Notes = "" },
				Navy          = { Notes = "" },
				Plain         = { Notes = "Resets all formatting to normal" },
				Purple        = { Notes = "" },
				Random        = { Notes = "Random letters and symbols animate instead of the text" },
				Red           = { Notes = "" },
				Rose          = { Notes = "" },
				Strikethrough = { Notes = "" },
				Underlined    = { Notes = "" },
				White         = { Notes = "" },
				Yellow        = { Notes = "" },
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
				FillRelCuboid =
				{
					{ Params = "{{cCuboid|RelCuboid}}, BlockType, BlockMeta", Return = "", Notes = "Fills the cuboid, specified in relative coords, by the specified block type and block meta. The cuboid may reach outside of the chunk, only the part intersecting with this chunk is filled." },
					{ Params = "MinRelX, MaxRelX, MinRelY, MaxRelY, MinRelZ, MaxRelZ, BlockType, BlockMeta", Return = "", Notes = "Fills the cuboid, specified in relative coords, by the specified block type and block meta. The cuboid may reach outside of the chunk, only the part intersecting with this chunk is filled." },
				},
				FloorRelCuboid =
				{
					{ Params = "{{cCuboid|RelCuboid}}, BlockType, BlockMeta", Return = "", Notes = "Fills those blocks of the cuboid (specified in relative coords) that are considered non-floor (air, water) with the specified block type and meta. Cuboid may reach outside the chunk, only the part intersecting with this chunk is filled." },
					{ Params = "MinRelX, MaxRelX, MinRelY, MaxRelY, MinRelZ, MaxRelZ, BlockType, BlockMeta", Return = "", Notes = "Fills those blocks of the cuboid (specified in relative coords) that are considered non-floor (air, water) with the specified block type and meta. Cuboid may reach outside the chunk, only the part intersecting with this chunk is filled." },
				},
				GetBiome                  = { Params = "RelX, RelZ", Return = "EMCSBiome", Notes = "Returns the biome at the specified relative coords" },
				GetBlockEntity            = { Params = "RelX, RelY, RelZ", Return = "{{cBlockEntity}} descendant", Notes = "Returns the block entity for the block at the specified coords. Creates it if it doesn't exist. Returns nil if the block has no block entity capability." },
				GetBlockMeta              = { Params = "RelX, RelY, RelZ", Return = "NIBBLETYPE", Notes = "Returns the block meta at the specified relative coords" },
				GetBlockType              = { Params = "RelX, RelY, RelZ", Return = "BLOCKTYPE", Notes = "Returns the block type at the specified relative coords" },
				GetBlockTypeMeta          = { Params = "RelX, RelY, RelZ", Return = "BLOCKTYPE, NIBBLETYPE", Notes = "Returns the block type and meta at the specified relative coords" },
				GetChunkX                 = { Params = "", Return = "number", Notes = "Returns the X coord of the chunk contained." },
				GetChunkZ                 = { Params = "", Return = "number", Notes = "Returns the Z coord of the chunk contained." },
				GetHeight                 = { Params = "RelX, RelZ", Return = "number", Notes = "Returns the height at the specified relative coords" },
				GetMaxHeight              = { Params = "", Return = "number", Notes = "Returns the maximum height contained in the heightmap." },
				IsUsingDefaultBiomes      = { Params = "", Return = "bool", Notes = "Returns true if the chunk is set to use default biome generator" },
				IsUsingDefaultComposition = { Params = "", Return = "bool", Notes = "Returns true if the chunk is set to use default composition generator" },
				IsUsingDefaultFinish      = { Params = "", Return = "bool", Notes = "Returns true if the chunk is set to use default finishers" },
				IsUsingDefaultHeight      = { Params = "", Return = "bool", Notes = "Returns true if the chunk is set to use default height generator" },
				IsUsingDefaultStructures  = { Params = "", Return = "bool", Notes = "Returns true if the chunk is set to use default structures" },
				RandomFillRelCuboid =
				{
					{ Params = "{{cCuboid|RelCuboid}}, BlockType, BlockMeta, RandomSeed, ChanceOutOf10k", Return = "", Notes = "Fills the specified relative cuboid with block type and meta in random locations. RandomSeed is used for the random number genertion (same seed produces same results); ChanceOutOf10k specifies the density (how many out of every 10000 blocks should be filled). Cuboid may reach outside the chunk, only the part intersecting with this chunk is filled." },
					{ Params = "MinRelX, MaxRelX, MinRelY, MaxRelY, MinRelZ, MaxRelZ, BlockType, BlockMeta, RandomSeed, ChanceOutOf10k", Return = "", Notes = "Fills the specified relative cuboid with block type and meta in random locations. RandomSeed is used for the random number genertion (same seed produces same results); ChanceOutOf10k specifies the density (how many out of every 10000 blocks should be filled). Cuboid may reach outside the chunk, only the part intersecting with this chunk is filled." },
				},
				ReadBlockArea             = { Params = "{{cBlockArea|BlockArea}}, MinRelX, MaxRelX, MinRelY, MaxRelY, MinRelZ, MaxRelZ", Return = "", Notes = "Reads data from the chunk into the block area object. Block types and metas are processed." },
				ReplaceRelCuboid =
				{
					{ Params = "{{cCuboid|RelCuboid}}, SrcType, SrcMeta, DstType, DstMeta", Return = "", Notes = "Replaces all SrcType+SrcMeta blocks in the cuboid (specified in relative coords) with DstType+DstMeta blocks. Cuboid may reach outside the chunk, only the part intersecting with this chunk is filled." },
					{ Params = "MinRelX, MaxRelX, MinRelY, MaxRelY, MinRelZ, MaxRelZ, SrcType, SrcMeta, DstType, DstMeta", Return = "", Notes = "Replaces all SrcType+SrcMeta blocks in the cuboid (specified in relative coords) with DstType+DstMeta blocks. Cuboid may reach outside the chunk, only the part intersecting with this chunk is filled." },
				},
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
				WriteBlockArea            = { Params = "{{cBlockArea|BlockArea}}, MinRelX, MinRelY, MinRelZ", Return = "", Notes = "Writes data from the block area into the chunk" },
			},
			AdditionalInfo =
			{
				{
					Header = "Manipulating block entities",
					Contents = [[
						To manipulate block entities while the chunk is generated, first use SetBlockTypeMeta() to set
						the correct block type and meta at the position. Then use the GetBlockEntity() to create and
						return the correct block entity instance. Finally, use {{tolua}}.cast() to cast to the proper
						type.</p>
						Note that you don't need to check if a block entity has previously existed at the place, because
						GetBlockEntity() will automatically re-create the correct type for you.</p>
						<p>
						The following code is taken from the Debuggers plugin, it creates a sign at each chunk's [0, 0]
						coords, with the text being the chunk coords:
<pre class="prettyprint lang-lua">
function OnChunkGenerated(a_World, a_ChunkX, a_ChunkZ, a_ChunkDesc)
	-- Get the topmost block coord:
	local Height = a_ChunkDesc:GetHeight(0, 0);
	
	-- Create a sign there:
	a_ChunkDesc:SetBlockTypeMeta(0, Height + 1, 0, E_BLOCK_SIGN_POST, 0);
	local BlockEntity = a_ChunkDesc:GetBlockEntity(0, Height + 1, 0);
	if (BlockEntity ~= nil) then
		LOG("Setting sign lines...");
		local SignEntity = tolua.cast(BlockEntity, "cSignEntity");
		SignEntity:SetLines("Chunk:", tonumber(a_ChunkX) .. ", " .. tonumber(a_ChunkZ), "", "(Debuggers)");
	end

	-- Update the heightmap:
	a_ChunkDesc:SetHeight(0, 0, Height + 1);
end
</pre>
					]],
				},
			},  -- AdditionalInfo
		},  -- cChunkDesc

		cClientHandle =
		{
			Desc = [[
				A cClientHandle represents the technical aspect of a connected player - their game client
				connection. Internally, it handles all the incoming and outgoing packets, the chunks that are to be
				sent to the client, ping times etc.
			]],

			Functions =
			{
				GetPing = { Params = "", Return = "number", Notes = "Returns the ping time, in ms" },
				GetPlayer = { Params = "", Return = "{{cPlayer|cPlayer}}", Notes = "Returns the player object connected to this client. Note that this may be nil, for example if the player object is not yet spawned." },
				GetUniqueID = { Params = "", Return = "number", Notes = "Returns the UniqueID of the client used to identify the client in the server" },
				GetUsername = { Params = "", Return = "string", Notes = "Returns the username that the client has provided" },
				GetViewDistance = { Params = "", Return = "number", Notes = "Returns the viewdistance (number of chunks loaded for the player in each direction)" },
				Kick = { Params = "Reason", Return = "", Notes = "Kicks the user with the specified reason" },
				SetUsername = { Params = "Name", Return = "", Notes = "Sets the username" },
				SetViewDistance = { Params = "ViewDistance", Return = "", Notes = "Sets the viewdistance (number of chunks loaded for the player in each direction)" },
				SendBlockChange = { Params = "BlockX, BlockY, BlockZ, BlockType, BlockMeta", Return = "", Notes = "Sends a BlockChange packet to the client. This can be used to create fake blocks only for that player." },
			},
			Constants =
			{
				MAX_VIEW_DISTANCE = { Notes = "The maximum value of the view distance" },
				MIN_VIEW_DISTANCE = { Notes = "The minimum value of the view distance" },
			},
		},  -- cClientHandle

		cCraftingGrid =
		{
			Desc = [[
				cCraftingGrid represents the player's crafting grid. It is used in
				{{OnCraftingNoRecipe|OnCraftingNoRecipe}}, {{OnPostCrafting|OnPostCrafting}} and
				{{OnPreCrafting|OnPreCrafting}} hooks. Plugins may use it to inspect the items the player placed
				on their crafting grid.</p>
				<p>
				Also, an object of this type is used in {{cCraftingRecipe}}'s ConsumeIngredients() function for
				specifying the exact number of ingredients to consume in that recipe; plugins may use this to
				apply the crafting recipe.</p>
			]],

			Functions =
			{
				constructor = { Params = "Width, Height", Return = "cCraftingGrid", Notes = "Creates a new CraftingGrid object. This new crafting grid is not related to any player, but may be needed for {{cCraftingRecipe}}'s ConsumeIngredients function." },
				Clear = { Params = "", Return = "", Notes = "Clears the entire grid" },
				ConsumeGrid = { Params = "{{cCraftingGrid|CraftingGrid}}", Return = "", Notes = "Consumes items specified in CraftingGrid from the current contents. Used internally by {{cCraftingRecipe}}'s ConsumeIngredients() function, but available to plugins, too." },
				Dump = { Params = "", Return = "", Notes = "DEBUG build: Dumps the contents of the grid to the log. RELEASE build: no action" },
				GetHeight = { Params = "", Return = "number", Notes = "Returns the height of the grid" },
				GetItem = { Params = "x, y", Return = "{{cItem|cItem}}", Notes = "Returns the item at the specified coords" },
				GetWidth = { Params = "", Return = "number", Notes = "Returns the width of the grid" },
				SetItem =
				{
					{ Params = "x, y, {{cItem|cItem}}", Return = "", Notes = "Sets the item at the specified coords" },
					{ Params = "x, y, ItemType, ItemCount, ItemDamage", Return = "", Notes = "Sets the item at the specified coords" },
				},
			},
		},  -- cCraftingGrid

		cCraftingRecipe =
		{
			Desc = [[
				This class is used to represent a crafting recipe, either a built-in one, or one created dynamically in a plugin. It is used only as a parameter for {{OnCraftingNoRecipe|OnCraftingNoRecipe}}, {{OnPostCrafting|OnPostCrafting}} and {{OnPreCrafting|OnPreCrafting}} hooks. Plugins may use it to inspect or modify a crafting recipe that a player views in their crafting window, either at a crafting table or the survival inventory screen.
</p>
		<p>Internally, the class contains a {{cCraftingGrid}} for the ingredients and a {{cItem}} for the result.
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
				SetIngredient =
				{
					{ Params = "x, y, {{cItem|cItem}}", Return = "", Notes = "Sets the ingredient at the specified coords" },
					{ Params = "x, y, ItemType, ItemCount, ItemDamage", Return = "", Notes = "Sets the ingredient at the specified coords" },
				},
				SetResult =
				{
					{ Params = "{{cItem|cItem}}", Return = "", Notes = "Sets the result item" },
					{ Params = "ItemType, ItemCount, ItemDamage", Return = "", Notes = "Sets the result item" },
				},
			},
		},  -- cCraftingRecipe

		cCuboid =
		{
			Desc = [[
				cCuboid offers some native support for integral-boundary cuboids. A cuboid internally consists of
				two {{Vector3i}}s. By default the cuboid doesn't make any assumptions about the defining points,
				but for most of the operations in the cCuboid class, the p1 member variable is expected to be the
				minima and the p2 variable the maxima. The Sort() function guarantees this condition.</p>
				<p>
				The Cuboid considers both its edges inclusive.</p>
			]],
			Functions =
			{
				constructor =
				{
					{ Params = "OtheCuboid", Return = "cCuboid", Notes = "Creates a new Cuboid object as a copy of OtherCuboid" },
					{ Params = "{{Vector3i|Point1}}, {{Vector3i|Point2}}", Return = "cCuboid", Notes = "Creates a new Cuboid object with the specified points as its corners." },
					{ Params = "X, Y, Z", Return = "cCuboid", Notes = "Creates a new Cuboid object with the specified point as both its corners (the cuboid has a size of 1 in each direction)." },
					{ Params = "X1, Y1, Z1, X2, Y2, Z2", Return = "cCuboid", Notes = "Creates a new Cuboid object with the specified points as its corners." },
				},
				Assign = { Params = "X1, Y1, Z1, X2, Y2, Z2", Return = "", Notes = "Assigns all the coords stored in the cuboid. Sort-state is ignored." },
				DifX = { Params = "", Return = "number", Notes = "Returns the difference between the two X coords (X-size minus 1). Assumes sorted." },
				DifY = { Params = "", Return = "number", Notes = "Returns the difference between the two Y coords (Y-size minus 1). Assumes sorted." },
				DifZ = { Params = "", Return = "number", Notes = "Returns the difference between the two Z coords (Z-size minus 1). Assumes sorted." },
				DoesIntersect = { Params = "OtherCuboid", Return = "bool", Notes = "Returns true if this cuboid has at least one voxel in common with OtherCuboid. Note that edges are considered inclusive. Assumes both sorted." },
				IsCompletelyInside = { Params = "OuterCuboid", Return = "bool", Notes = "Returns true if this cuboid is completely inside (in all directions) in OuterCuboid. Assumes both sorted." },
				IsInside =
				{
					{ Params = "X, Y, Z", Return = "bool", Notes = "Returns true if the specified point (integral coords) is inside this cuboid. Assumes sorted." },
					{ Params = "{{Vector3i|Point}}", Return = "bool", Notes = "Returns true if the specified point (integral coords) is inside this cuboid. Assumes sorted." },
					{ Params = "{{Vector3d|Point}}", Return = "bool", Notes = "Returns true if the specified point (floating-point coords) is inside this cuboid. Assumes sorted." },
				},
				IsSorted = { Params = "", Return = "bool", Notes = "Returns true if this cuboid is sorted" },
				Move = { Params = "OffsetX, OffsetY, OffsetZ", Return = "", Notes = "Adds the specified offsets to each respective coord, effectively moving the Cuboid. Sort-state is ignored." },
				Sort = { Params = "", Return = "" , Notes = "Sorts the internal representation so that p1 contains the lesser coords and p2 contains the greater coords." },
			},
			Variables =
			{
				p1 = { Type = "{{Vector3i}}", Notes = "The first corner. Usually the lesser of the two coords in each set" },
				p2 = { Type = "{{Vector3i}}", Notes = "The second corner. Usually the larger of the two coords in each set" },
			},
		},  -- cCuboid

		cEnchantments =
		{
			Desc = [[
				This class  is the storage for enchantments for a single {{cItem|cItem}} object, through its
				m_Enchantments member variable. Although it is possible to create a standalone object of this class,
				it is not yet used in any API directly.</p>
				<p>
				Enchantments can be initialized either programmatically by calling the individual functions
				(SetLevel()), or by using a string description of the enchantment combination. This string
				description is in the form "id=lvl;id=lvl;...;id=lvl;", where id is either a numerical ID of the
				enchantment, or its textual representation from the table below, and lvl is the desired enchantment
				level. The class can also create its string description from its current contents; however that
				string description will only have the numerical IDs.</p>
				<p>
				See the {{cItem}} class for usage examples.
			]],
			Functions =
			{
				constructor =
				{
					{ Params = "", Return = "cEnchantments", Notes = "Creates a new empty cEnchantments object" },
					{ Params = "StringSpec", Return = "cEnchantments", Notes = "Creates a new cEnchantments object filled with enchantments based on the string description" },
				},
				operator_eq = { Params = "OtherEnchantments", Return = "bool", Notes = "Returns true if this enchantments object has the same enchantments as OtherEnchantments." },
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
				-- Only list these enchantment IDs, as they don't really need any kind of documentation:
				enchAquaAffinity = { Notes = "" },
				enchBaneOfArthropods = { Notes = "" },
				enchBlastProtection = { Notes = "" },
				enchEfficiency = { Notes = "" },
				enchFeatherFalling = { Notes = "" },
				enchFireAspect = { Notes = "" },
				enchFireProtection = { Notes = "" },
				enchFlame = { Notes = "" },
				enchFortune = { Notes = "" },
				enchInfinity = { Notes = "" },
				enchKnockback = { Notes = "" },
				enchLooting = { Notes = "" },
				enchLuckOfTheSea = { Notes = "" },
				enchLure = { Notes = "" },
				enchPower = { Notes = "" },
				enchProjectileProtection = { Notes = "" },
				enchProtection = { Notes = "" },
				enchPunch = { Notes = "" },
				enchRespiration = { Notes = "" },
				enchSharpness = { Notes = "" },
				enchSilkTouch = { Notes = "" },
				enchSmite = { Notes = "" },
				enchThorns = { Notes = "" },
				enchUnbreaking = { Notes = "" },
			},
		},

		cEntity =
		{
			Desc = [[
				A cEntity object represents an object in the world, it has a position and orientation. cEntity is an
				abstract class, and can not be instantiated directly, instead, all entities are implemented as
				subclasses. The cEntity class works as the common interface for the operations that all (most)
				entities support.</p>
				<p>
				All cEntity objects have an Entity Type so it can be determined what kind of entity it is
				efficiently. Entities also have a class inheritance awareness, they know their class name,
				their parent class' name and can decide if there is a class within their inheritance chain.
				Since these functions operate on strings, they are slightly slower than checking the entity type
				directly, on the other hand, they are more specific directly. To check if the entity is a spider,
				you need to call IsMob(), then cast the object to {{cMonster}} and finally compare
				{{cMonster}}:GetMonsterType() to mtSpider. GetClass(), on the other hand, returns "cSpider"
				directly.</p>
				<p>
				Note that you should not store a cEntity object between two hooks' calls, because MCServer may
				despawn / remove that entity in between the calls. If you need to refer to an entity later, use its
				UniqueID and {{cWorld|cWorld}}'s entity manipulation functions DoWithEntityByID(), ForEachEntity()
				or ForEachEntityInChunk() to access the entity again.</p>
			]],
			Functions =
			{
				AddPosition =
				{
					{ Params = "OffsetX, OffsetY, OffsetZ", Return = "", Notes = "Moves the entity by the specified amount in each axis direction" },
					{ Params = "{{Vector3d|Offset}}", Return = "", Notes = "Moves the entity by the specified amount in each direction" },
				},
				AddPosX = { Params = "OffsetX", Return = "", Notes = "Moves the entity by the specified amount in the X axis direction" },
				AddPosY = { Params = "OffsetY", Return = "", Notes = "Moves the entity by the specified amount in the Y axis direction" },
				AddPosZ = { Params = "OffsetZ", Return = "", Notes = "Moves the entity by the specified amount in the Z axis direction" },
				AddSpeed =
				{
					{ Params = "AddX, AddY, AddZ", Return = "", Notes = "Adds the specified amount of speed in each axis direction." },
					{ Params = "{{Vector3d|Add}}", Return = "", Notes = "Adds the specified amount of speed in each axis direction." },
				},
				AddSpeedX = { Params = "AddX", Return = "", Notes = "Adds the specified amount of speed in the X axis direction." },
				AddSpeedY = { Params = "AddY", Return = "", Notes = "Adds the specified amount of speed in the Y axis direction." },
				AddSpeedZ = { Params = "AddZ", Return = "", Notes = "Adds the specified amount of speed in the Z axis direction." },
				Destroy = { Params = "", Return = "", Notes = "Schedules the entity to be destroyed" },
				GetArmorCoverAgainst = { Params = "AttackerEntity, DamageType, RawDamage", Return = "number", Notes = "Returns the number of hitpoints out of RawDamage that the currently equipped armor would cover. See {{TakeDamageInfo}} for more information on attack damage." },
				GetChunkX = { Params = "", Return = "number", Notes = "Returns the X-coord of the chunk in which the entity is placed" },
				GetChunkZ = { Params = "", Return = "number", Notes = "Returns the Z-coord of the chunk in which the entity is placed" },
				GetClass = { Params = "", Return = "string", Notes = "Returns the classname of the entity, such as \"cSpider\" or \"cPickup\"" },
				GetClassStatic = { Params = "", Return = "string", Notes = "Returns the entity classname that this class implements. Each descendant overrides this function. Is static" },
				GetEntityType = { Params = "", Return = "{{cEntity#EntityType|EntityType}}", Notes = "Returns the type of the entity, one of the {{cEntity#EntityType|etXXX}} constants. Note that to check specific entity type, you should use one of the IsXXX functions instead of comparing the value returned by this call." },
				GetEquippedBoots = { Params = "", Return = "{{cItem}}", Notes = "Returns the boots that the entity has equipped. Returns an empty cItem if no boots equipped or not applicable." },
				GetEquippedChestplate = { Params = "", Return = "{{cItem}}", Notes = "Returns the chestplate that the entity has equipped. Returns an empty cItem if no chestplate equipped or not applicable." },
				GetEquippedHelmet = { Params = "", Return = "{{cItem}}", Notes = "Returns the helmet that the entity has equipped. Returns an empty cItem if no helmet equipped or not applicable." },
				GetEquippedLeggings = { Params = "", Return = "{{cItem}}", Notes = "Returns the leggings that the entity has equipped. Returns an empty cItem if no leggings equipped or not applicable." },
				GetEquippedWeapon = { Params = "", Return = "{{cItem}}", Notes = "Returns the weapon that the entity has equipped. Returns an empty cItem if no weapon equipped or not applicable." },
				GetGravity = { Params = "", Return = "number", Notes = "Returns the number that is used as the gravity for physics simulation. 1G (9.78) by default." },
				GetHeadYaw = { Params = "", Return = "number", Notes = "Returns the pitch of the entity's head (FIXME: Rename to GetHeadPitch() )." },
				GetHealth = { Params = "", Return = "number", Notes = "Returns the current health of the entity." },
				GetHeight = { Params = "", Return = "number", Notes = "Returns the height (Y size) of the entity" },
				GetKnockbackAmountAgainst = { Params = "ReceiverEntity", Return = "number", Notes = "Returns the amount of knockback that the currently equipped items would cause when attacking the ReceiverEntity." },
				GetLookVector = { Params = "", Return = "{{Vector3f}}", Notes = "Returns the vector that defines the direction in which the entity is looking" },
				GetMass = { Params = "", Return = "number", Notes = "Returns the mass of the entity. Currently unused." },
				GetMaxHealth = { Params = "", Return = "number", Notes = "Returns the maximum number of hitpoints this entity is allowed to have." },
				GetParentClass = { Params = "", Return = "string", Notes = "Returns the name of the direct parent class for this entity" },
				GetPitch = { Params = "", Return = "number", Notes = "Returns the pitch (nose-down rotation) of the entity" },
				GetPosition = { Params = "", Return = "{{Vector3d}}", Notes = "Returns the entity's pivot position as a 3D vector" },
				GetPosX = { Params = "", Return = "number", Notes = "Returns the X-coord of the entity's pivot" },
				GetPosY = { Params = "", Return = "number", Notes = "Returns the Y-coord of the entity's pivot" },
				GetPosZ = { Params = "", Return = "number", Notes = "Returns the Z-coord of the entity's pivot" },
				GetRawDamageAgainst = { Params = "ReceiverEntity", Return = "number", Notes = "Returns the raw damage that this entity's equipment would cause when attacking the ReceiverEntity. This includes this entity's weapon {{cEnchantments|enchantments}}, but excludes the receiver's armor or potion effects. See {{TakeDamageInfo}} for more information on attack damage." },
				GetRoll = { Params = "", Return = "number", Notes = "Returns the roll (sideways rotation) of the entity. Currently unused." },
				GetRot = { Params = "", Return = "{{Vector3f}}", Notes = "Returns the entire rotation vector (Yaw, Pitch, Roll)" },
				GetRotation = { Params = "", Return = "number", Notes = "Returns the yaw (direction) of the entity. OBSOLETE, use GetYaw() instead." },
				GetSpeed = { Params = "", Return = "{{Vector3d}}", Notes = "Returns the complete speed vector of the entity" },
				GetSpeedX = { Params = "", Return = "number", Notes = "Returns the X-part of the speed vector" },
				GetSpeedY = { Params = "", Return = "number", Notes = "Returns the Y-part of the speed vector" },
				GetSpeedZ = { Params = "", Return = "number", Notes = "Returns the Z-part of the speed vector" },
				GetUniqueID = { Params = "", Return = "number", Notes = "Returns the ID that uniquely identifies the entity within the running server. Note that this ID is not persisted to the data files." },
				GetWidth = { Params = "", Return = "number", Notes = "Returns the width (X and Z size) of the entity." },
				GetWorld = { Params = "", Return = "{{cWorld}}", Notes = "Returns the world where the entity resides" },
				GetYaw = { Params = "", Return = "number", Notes = "Returns the yaw (direction) of the entity." },
				Heal = { Params = "Hitpoints", Return = "", Notes = "Heals the specified number of hitpoints. Hitpoints is expected to be a positive number." },
				IsA = { Params = "ClassName", Return = "bool", Notes = "Returns true if the entity class is a descendant of the specified class name, or the specified class itself" },
				IsBoat = { Params = "", Return = "bool", Notes = "Returns true if the entity is a {{cBoat|boat}}." },
				IsCrouched = { Params = "", Return = "bool", Notes = "Returns true if the entity is crouched. Always false for entities that don't support crouching." },
				IsDestroyed = { Params = "", Return = "bool", Notes = "Returns true if the entity has been destroyed and is awaiting removal from the internal structures." },
				IsFallingBlock = { Params = "", Return = "bool", Notes = "Returns true if the entity represents a {{cFallingBlock}} entity." },
				IsInvisible = { Params = "", Return = "bool", Notes = "Returns true if the entity is invisible" },
				IsMinecart = { Params = "", Return = "bool", Notes = "Returns true if the entity represents a {{cMinecart|minecart}}" },
				IsMob = { Params = "", Return = "bool", Notes = "Returns true if the entity represents any {{cMonster|mob}}." },
				IsOnFire = { Params = "", Return = "bool", Notes = "Returns true if the entity is on fire" },
				IsPickup = { Params = "", Return = "bool", Notes = "Returns true if the entity represents a {{cPickup|pickup}}." },
				IsPlayer = { Params = "", Return = "bool", Notes = "Returns true if the entity represents a {{cPlayer|player}}" },
				IsProjectile = { Params = "", Return = "bool", Notes = "Returns true if the entity is a {{cProjectileEntity}} descendant." },
				IsRclking = { Params = "", Return = "bool", Notes = "Currently unimplemented" },
				IsRiding = { Params = "", Return = "bool", Notes = "Returns true if the entity is attached to (riding) another entity." },
				IsSprinting = { Params = "", Return = "bool", Notes = "Returns true if the entity is sprinting. Entities that cannot sprint return always false" },
				IsTNT = { Params = "", Return = "bool", Notes = "Returns true if the entity represents a {{cTNTEntity|TNT entity}}" },
				KilledBy = { Notes = "FIXME: Remove this from API" },
				SetGravity = { Params = "Gravity", Return = "", Notes = "Sets the number that is used as the gravity for physics simulation. 1G (9.78) by default." },
				SetHeadYaw = { Params = "HeadPitch", Return = "", Notes = "Sets the head pitch (FIXME: Rename to SetHeadPitch() )." },
				SetHealth = { Params = "Hitpoints", Return = "", Notes = "Sets the entity's health to the specified amount of hitpoints. Doesn't broadcast any hurt animation. Doesn't kill the entity if health drops below zero. Use the TakeDamage() function instead for taking damage." },
				SetHeight = { Params = "", Return = "", Notes = "FIXME: Remove this from API" },
				SetMass = { Params = "Mass", Return = "", Notes = "Sets the mass of the entity. Currently unused." },
				SetMaxHealth = { Params = "MaxHitpoints", Return = "", Notes = "Sets the maximum hitpoints of the entity. If current health is above MaxHitpoints, it is capped to MaxHitpoints." },
				SetPitch = { Params = "number", Return = "", Notes = "Sets the pitch (nose-down rotation) of the entity" },
				SetPitchFromSpeed = { Params = "", Return = "", Notes = "Sets the entity pitch to match its speed (entity looking forwards as it moves)" },
				SetPosition =
				{
					{ Params = "PosX, PosY, PosZ", Return = "", Notes = "Sets all three coords of the entity's pivot" },
					{ Params = "{{Vector3d|Vector3d}}", Return = "", Notes = "Sets all three coords of the entity's pivot" },
				},
				SetPosX = { Params = "number", Return = "", Notes = "Sets the X-coord of the entity's pivot" },
				SetPosY = { Params = "number", Return = "", Notes = "Sets the Y-coord of the entity's pivot" },
				SetPosZ = { Params = "number", Return = "", Notes = "Sets the Z-coord of the entity's pivot" },
				SetRoll = { Params = "number", Return = "", Notes = "Sets the roll (sideways rotation) of the entity. Currently unused." },
				SetRot = { Params = "{{Vector3f|Rotation}}", Return = "", Notes = "Sets the entire rotation vector (Yaw, Pitch, Roll)" },
				SetRotation = { Params = "number", Return = "", Notes = "Sets the yaw (direction) of the entity. OBSOLETE, use SetYaw() instead." },
				SetRotationFromSpeed = { Params = "", Return = "", Notes = "Sets the entity's yaw to match its current speed (entity looking forwards as it moves). (FIXME: Rename to SetYawFromSpeed)" },
				SetSpeed =
				{
					{ Params = "SpeedX, SpeedY, SpeedZ", Return = "", Notes = "Sets the current speed of the entity" },
					{ Params = "{{Vector3d|Speed}}", Return = "", Notes = "Sets the current speed of the entity" },
				},
				SetSpeedX = { Params = "SpeedX", Return = "", Notes = "Sets the X component of the entity speed" },
				SetSpeedY = { Params = "SpeedY", Return = "", Notes = "Sets the Y component of the entity speed" },
				SetSpeedZ = { Params = "SpeedZ", Return = "", Notes = "Sets the Z component of the entity speed" },
				SetWidth = { Params = "", Return = "", Notes = "FIXME: Remove this from API" },
				SetYaw = { Params = "number", Return = "", Notes = "Sets the yaw (direction) of the entity." },
				StartBurning = { Params = "NumTicks", Return = "", Notes = "Sets the entity on fire for the specified number of ticks. If entity is on fire already, makes it burn for either NumTicks or the number of ticks left from the previous fire, whichever is larger." },
				SteerVehicle = { Params = "ForwardAmount, SidewaysAmount", Return = "", Notes = "Applies the specified steering to the vehicle this entity is attached to. Ignored if not attached to any entity." },
				StopBurning = { Params = "", Return = "", Notes = "Extinguishes the entity fire, cancels all fire timers." },
				TakeDamage =
				{
					{ Params = "AttackerEntity", Return = "", Notes = "Causes this entity to take damage that AttackerEntity would inflict. Includes their weapon and this entity's armor." },
					{ Params = "DamageType, AttackerEntity, RawDamage, KnockbackAmount", Return = "", Notes = "Causes this entity to take damage of the specified type, from the specified attacker (may be nil). The final damage is calculated from RawDamage using the currently equipped armor." },
					{ Params = "DamageType, ArrackerEntity, RawDamage, FinalDamage, KnockbackAmount", Return = "", Notes = "Causes this entity to take damage of the specified type, from the specified attacker (may be nil). The values are wrapped into a {{TakeDamageInfo}} structure and applied directly." },
				},
				TeleportToCoords = { Params = "PosX, PosY, PosZ", Return = "", Notes = "Teleports the entity to the specified coords." },
				TeleportToEntity = { Params = "DestEntity", Return = "", Notes = "Teleports this entity to the specified destination entity." },
			},
			Constants =
			{
				etBoat = { Notes = "The entity is a {{cBoat}}" },
				etEntity = { Notes = "No further specialization available" },
				etExpOrb = { Notes = "The entity is a {{cExpOrb}}" },
				etFallingBlock = { Notes = "The entity is a {{cFallingBlock}}" },
				etMob = { Notes = "The entity is a {{cMonster}} descendant" },
				etMonster = { Notes = "The entity is a {{cMonster}} descendant" },
				etMinecart = { Notes = "The entity is a {{cMinecart}} descendant" },
				etPlayer = { Notes = "The entity is a {{cPlayer}}" },
				etPickup = { Notes = "The entity is a {{cPickup}}" },
				etProjectile = { Notes = "The entity is a {{cProjectileEntity}} descendant" },
				etTNT = { Notes = "The entity is a {{cTNTEntity}}" },
			},
			ConstantGroups =
			{
				EntityType =
				{
					Include = "et.*",
					TextBefore = "The following constants are used to distinguish between different entity types:",
				},
			},
		},

		cFile =
		{
			Desc = [[
				Provides helper functions for manipulating and querying the filesystem. Most functions are static,
				so they should be called directly on the cFile class itself:
<pre class="prettyprint lang-lua">
cFile:Delete("/usr/bin/virus.exe");
</pre></p>
			]],
			Functions =
			{
				Copy = { Params = "SrcFileName, DstFileName", Return = "bool", Notes = "(STATIC) Copies a single file to a new destination. Returns true if successful. Fails if the destination already exists." },
				CreateFolder = { Params = "FolderName", Return = "bool", Notes = "(STATIC) Creates a new folder. Returns true if successful." },
				Delete = { Params = "FileName", Return = "bool", Notes = "(STATIC) Deletes the specified file. Returns true if successful." },
				Exists = { Params = "FileName", Return = "bool", Notes = "(STATIC) Returns true if the specified file exists." },
				GetFolderContents = { Params = "FolderName", Return = "array table of strings", Notes = "(STATIC) Returns the contents of the specified folder, as an array table of strings. Each filesystem object is listed. Use the IsFile() and IsFolder() functions to determine the object type." },
				GetSize = { Params = "FileName", Return = "number", Notes = "(STATIC) Returns the size of the file, or -1 on failure." },
				IsFile = { Params = "Path", Return = "bool", Notes = "(STATIC) Returns true if the specified path points to an existing file." },
				IsFolder = { Params = "Path", Return = "bool", Notes = "(STATIC) Returns true if the specified path points to an existing folder." },
				Rename = { Params = "OrigPath, NewPath", Return = "bool", Notes = "(STATIC) Renames a file or a folder. Returns true if successful. Undefined result if NewPath already exists." },
			},
		},  -- cFile

		cGroup =
		{
			Desc = [[
				This class represents a group {{cPlayer|players}} can be in. Groups define the permissions players
				have, and optionally the color of their name in the chat.
			]],
			Functions =
			{
				SetName = { Return = "" },
				GetName = { Return = "string" },
				SetColor = { Return = "" },
				GetColor = { Return = "string" },
				AddCommand = { Return = "" },
				HasCommand = { Return = "bool" },
				AddPermission = { Return = "" },
				InheritFrom = { Return = "" },
			},
		},  -- cGroup

		cIniFile =
		{
			Desc = [[
				This class implements a simple name-value storage represented on disk by an INI file. These files
				are suitable for low-volume high-latency human-readable information storage, such as for
				configuration. MCServer itself uses INI files for settings and options.</p>
				<p>
				The INI files follow this basic structure:
<pre class="prettyprint lang-ini">
; Header comment line
[KeyName0]
; Key comment line 0
ValueName0=Value0
ValueName1=Value1

[KeyName1]
; Key comment line 0
; Key comment line 1
ValueName0=SomeOtherValue
</pre>
				The cIniFile object stores all the objects in numbered arrays and provides access to the information
				either based on names (KeyName, ValueName) or zero-based indices.</p>
				<p>
				The objects of this class are created empty. You need to either load a file using ReadFile(), or
				insert values by hand. Then you can store the object's contents to a disk file using WriteFile(), or
				just forget everything by destroying the object. Note that the file operations are quite slow.</p>
				<p>
				For storing high-volume low-latency data, use the {{sqlite3}} class. For storing
				hierarchically-structured data, use the XML format, using the LuaExpat parser in the {{lxp}} class.
			]],
			Functions =
			{
				constructor = { Params = "", Return = "cIniFile", Notes = "Creates a new empty cIniFile object." },
				AddHeaderComment = { Params = "Comment", Return = "", Notes = "Adds a comment to be stored in the file header." },
				AddKeyComment = 
				{
					{ Params = "KeyID, Comment", Return = "", Notes = "Adds a comment to be stored in the file under the specified key" },
					{ Params = "KeyName, Comment", Return = "", Notes = "Adds a comment to be stored in the file under the specified key" },
				},
				AddKeyName = { Params = "KeyName", Returns = "number", Notes = "Adds a new key of the specified name. Returns the KeyID of the new key." },
				AddValue = { Params = "KeyName, ValueName, Value", Return = "", Notes = "Adds a new value of the specified name to the specified key. If another value of the same name exists in the key, both are kept (nonstandard INI file)" },
				AddValueB = { Params = "KeyName, ValueName, Value", Return = "", Notes = "Adds a new bool value of the specified name to the specified key. If another value of the same name exists in the key, both are kept (nonstandard INI file)" },
				AddValueF = { Params = "KeyName, ValueName, Value", Return = "", Notes = "Adds a new float value of the specified name to the specified key. If another value of the same name exists in the key, both are kept (nonstandard INI file)" },
				AddValueI = { Params = "KeyName, ValueName, Value", Return = "", Notes = "Adds a new integer value of the specified name to the specified key. If another value of the same name exists in the key, both are kept (nonstandard INI file)" },
				CaseInsensitive = { Params = "", Return = "", Notes = "Sets key names' and value names' comparisons to case insensitive (default)." },
				CaseSensitive = { Params = "", Return = "", Notes = "Sets key names and value names comparisons to case sensitive." },
				Clear = { Params = "", Return = "", Notes = "Removes all the in-memory data. Note that , like all the other operations, this doesn't affect any file data." },
				DeleteHeaderComment = { Params = "CommentID", Return = "bool" , Notes = "Deletes the specified header comment. Returns true if successful."},
				DeleteHeaderComments = { Params = "", Return = "", Notes = "Deletes all headers comments." },
				DeleteKey = { Params = "KeyName", Return = "bool", Notes = "Deletes the specified key, and all values in that key. Returns true if successful." },
				DeleteKeyComment =
				{
					{ Params = "KeyID, CommentID", Return = "bool", Notes = "Deletes the specified key comment. Returns true if successful." },
					{ Params = "KeyName, CommentID", Return = "bool", Notes = "Deletes the specified key comment. Returns true if successful." },
				},
				DeleteKeyComments =
				{
					{ Params = "KeyID", Return = "bool", Notes = "Deletes all comments for the specified key. Returns true if successful." },
					{ Params = "KeyName", Return = "bool", Notes = "Deletes all comments for the specified key. Returns true if successful." },
				},
				DeleteValue = { Params = "KeyName, ValueName", Return = "bool", Notes = "Deletes the specified value. Returns true if successful." },
				DeleteValueByID = { Params = "KeyID, ValueID", Return = "bool", Notes = "Deletes the specified value. Returns true if successful." },
				FindKey = { Params = "KeyName", Return = "number", Notes = "Returns the KeyID for the specified key name, or the noID constant if the key doesn't exist." },
				FindValue = { Params = "KeyID, ValueName", Return = "numebr", Notes = "Returns the ValueID for the specified value name, or the noID constant if the specified key doesn't contain a value of that name." },
				GetHeaderComment = { Params = "CommentID", Return = "string", Notes = "Returns the specified header comment, or an empty string if such comment doesn't exist" },
				GetKeyComment =
				{
					{ Params = "KeyID, CommentID", Return = "string", Notes = "Returns the specified key comment, or an empty string if such a comment doesn't exist" },
					{ Params = "KeyName, CommentID", Return = "string", Notes = "Returns the specified key comment, or an empty string if such a comment doesn't exist" },
				},
				GetKeyName = { Params = "KeyID", Return = "string", Notes = "Returns the key name for the specified key ID. Inverse for FindKey()." },
				GetNumHeaderComments = { Params = "", Return = "number", Notes = "Retuns the number of header comments." },
				GetNumKeyComments =
				{
					{ Params = "KeyID", Return = "number", Notes = "Returns the number of comments under the specified key" },
					{ Params = "KeyName", Return = "number", Notes = "Returns the number of comments under the specified key" },
				},
				GetNumKeys = { Params = "", Return = "number", Notes = "Returns the total number of keys. This is the range for the KeyID (0 .. GetNumKeys() - 1)" },
				GetNumValues = 
				{
					{ Params = "KeyID", Return = "number", Notes = "Returns the number of values stored under the specified key." },
					{ Params = "KeyName", Return = "number", Notes = "Returns the number of values stored under the specified key." },
				},
				GetValue = 
				{
					{ Params = "KeyName, ValueName", Return = "string", Notes = "Returns the value of the specified name under the specified key. Returns an empty string if the value doesn't exist." },
					{ Params = "KeyID, ValueID", Return = "string", Notes = "Returns the value of the specified name under the specified key. Returns an empty string if the value doesn't exist." },
				},
				GetValueB = { Params = "KeyName, ValueName", Return = "bool", Notes = "Returns the value of the specified name under the specified key, as a bool. Returns false if the value doesn't exist." },
				GetValueF = { Params = "KeyName, ValueName", Return = "number", Notes = "Returns the value of the specified name under the specified key, as a floating-point number. Returns zero if the value doesn't exist." },
				GetValueI = { Params = "KeyName, ValueName", Return = "number", Notes = "Returns the value of the specified name under the specified key, as an integer. Returns zero if the value doesn't exist." },
				GetValueName = 
				{
					{ Params = "KeyID, ValueID", Return = "string", Notes = "Returns the name of the specified value Inverse for FindValue()." },
					{ Params = "KeyName, ValueID", Return = "string", Notes = "Returns the name of the specified value Inverse for FindValue()." },
				},
				GetValueSet = { Params = "KeyName, ValueName, Default", Return = "string", Notes = "Returns the value of the specified name under the specified key. If the value doesn't exist, creates it with the specified default." },
				GetValueSetB = { Params = "KeyName, ValueName, Default", Return = "bool", Notes = "Returns the value of the specified name under the specified key, as a bool. If the value doesn't exist, creates it with the specified default." },
				GetValueSetF = { Params = "KeyName, ValueName, Default", Return = "number", Notes = "Returns the value of the specified name under the specified key, as a floating-point number. If the value doesn't exist, creates it with the specified default." },
				GetValueSetI = { Params = "KeyName, ValueName, Default", Return = "number", Notes = "Returns the value of the specified name under the specified key, as an integer. If the value doesn't exist, creates it with the specified default." },
				ReadFile = { Params = "FileName, [AllowExampleFallback]", Return = "bool", Notes = "Reads the values from the specified file. Previous in-memory contents are lost. If the file cannot be opened, and AllowExample is true, another file, \"filename.example.ini\", is loaded and then saved as \"filename.ini\". Returns true if successful, false if not." },
				SetValue = 
				{
					{ Params = "KeyID, ValueID, NewValue", Return = "bool", Notes = "Overwrites the specified value with a new value. If the specified value doesn't exist, returns false (doesn't add)." },
					{ Params = "KeyName, ValueName, NewValue, [CreateIfNotExists]", Return = "bool", Notes = "Overwrites the specified value with a new value. If CreateIfNotExists is true (default) and the value doesn't exist, it is first created. Returns true if the value was successfully set, false if not (didn't exists, CreateIfNotExists false)." },
				},
				SetValueB = { Params = "KeyName, ValueName, NewValueBool, [CreateIfNotExists]", Return = "bool", Notes = "Overwrites the specified value with a new bool value. If CreateIfNotExists is true (default) and the value doesn't exist, it is first created. Returns true if the value was successfully set, false if not (didn't exists, CreateIfNotExists false)." },
				SetValueF = { Params = "KeyName, ValueName, NewValueFloat, [CreateIfNotExists]", Return = "bool", Notes = "Overwrites the specified value with a new floating-point number value. If CreateIfNotExists is true (default) and the value doesn't exist, it is first created. Returns true if the value was successfully set, false if not (didn't exists, CreateIfNotExists false)." },
				SetValueI = { Params = "KeyName, ValueName, NewValueInt, [CreateIfNotExists]", Return = "bool", Notes = "Overwrites the specified value with a new integer value. If CreateIfNotExists is true (default) and the value doesn't exist, it is first created. Returns true if the value was successfully set, false if not (didn't exists, CreateIfNotExists false)." },
				WriteFile = { Params = "FileName", Return = "bool", Notes = "Writes the current in-memory data into the specified file. Returns true if successful, false if not." },
			},
			Constants =
			{
				noID = { Notes = "" },
			},
			AdditionalInfo =
			{
				{
					Header = "Code example: Reading a simple value",
					Contents = [[
						cIniFile is very easy to use. For example, you can find out what port the server is supposed to
						use according to settings.ini by using this little snippet:
<pre class="prettyprint lang-lua">
local IniFile = cIniFile();
if (IniFile:ReadFile("settings.ini")) then
	ServerPort = IniFile:GetValueI("Server", "Port");
end
</pre>
					]],
				},
				{
					Header = "Code example: Enumerating all objects in a file",
					Contents = [[
						To enumerate all keys in a file, you need to query the total number of keys, using GetNumKeys(),
						and then query each key's name using GetKeyName(). Similarly, to enumerate all values under a
						key, you need to query the total number of values using GetNumValues() and then query each
						value's name using GetValueName().</p>
						<p>
						The following code logs all keynames and their valuenames into the server log:
<pre class="prettyprint lang-lua">
local IniFile = cIniFile();
IniFile:ReadFile("somefile.ini")
local NumKeys = IniFile:GetNumKeys();
for k = 0, NumKeys do
	local NumValues = IniFile:GetNumValues(k);
	LOG("key \"" .. IniFile:GetKeyName(k) .. "\" has " .. NumValues .. " values:");
	for v = 0, NumValues do
		LOG("  value \"" .. IniFile:GetValueName(k, v) .. "\".");
	end
end
</pre>
					]],
				},
			},  -- AdditionalInfo
		},  -- cIniFile

		cInventory =
		{
			Desc = [[This object is used to store the items that a {{cPlayer|cPlayer}} has. It also keeps track of what item the player has currently selected in their hotbar.
Internally, the class uses three {{cItemGrid|cItemGrid}} objects to store the contents:
<li>Armor</li>
<li>Inventory</li>
<li>Hotbar</li>
These ItemGrids are available in the API and can be manipulated by the plugins, too.</p>
				<p>
				When using the raw slot access functions, such as GetSlot() and SetSlot(), the slots are numbered
				consecutively, each ItemGrid has its offset and count. To future-proff your plugins, use the named
				constants instead of hard-coded numbers.
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
				invArmorCount      = { Notes = "Number of slots in the Armor part" },
				invArmorOffset     = { Notes = "Starting slot number of the Armor part" },
				invInventoryCount  = { Notes = "Number of slots in the main inventory part" },
				invInventoryOffset = { Notes = "Starting slot number of the main inventory part" },
				invHotbarCount     = { Notes = "Number of slots in the Hotbar part" },
				invHotbarOffset    = { Notes = "Starting slot number of the Hotbar part" },
				invNumSlots        = { Notes = "Total number of slots in a cInventory" },
			},
			ConstantGroups =
			{
				SlotIndices =
				{
					Include = "inv.*",
					TextBefore = [[
						Rather than hardcoding numbers, use the following constants for slot indices and counts:
					]],
				},
			},
		},  -- cInventory

		cItem =
		{
			Desc = [[
				cItem is what defines an item or stack of items in the game, it contains the item ID, damage,
				quantity and enchantments. Each slot in a {{cInventory}} class or a {{cItemGrid}} class is a cItem
				and each {{cPickup}} contains a cItem. The enchantments are contained in a separate
				{{cEnchantments}} class and are accessible through the m_Enchantments variable.</p>
				<p>
				To test if a cItem object represents an empty item, do not compare the item type nor the item count,
				but rather use the IsEmpty() function.</p>
				<p>
				To translate from a cItem to its string representation, use the {{Globals#functions|global function}}
				ItemToString(), ItemTypeToString() or ItemToFullString(). To translate from a string to a cItem,
				use the StringToItem() global function.
			]],

			Functions =
			{
				constructor =
				{
					{ Params = "", Return = "cItem", Notes = "Creates a new empty cItem object" },
					{ Params = "ItemType, Count, Damage, EnchantmentString", Return = "cItem", Notes = "Creates a new cItem object of the specified type, count (1 by default), damage (0 by default) and enchantments (non-enchanted by default)" },
					{ Params = "cItem", Return = "cItem", Notes = "Creates an exact copy of the cItem object in the parameter" },
				} ,
				AddCount = { Params = "AmountToAdd", Return = "cItem", Notes = "Adds the specified amount to the item count. Returns self (useful for chaining)." },
				Clear = { Params = "", Return = "", Notes = "Resets the instance to an empty item" },
				CopyOne = { Params = "", Return = "cItem", Notes = "Creates a copy of this object, with its count set to 1" },
				DamageItem = { Params = "[Amount]", Return = "bool", Notes = "Adds the specified damage. Returns true when damage reaches max value and the item should be destroyed (but doesn't destroy the item)" },
				Empty = { Params = "", Return = "", Notes = "Resets the instance to an empty item" },
				GetMaxDamage = { Params = "", Return = "number", Notes = "Returns the maximum value for damage that this item can get before breaking; zero if damage is not accounted for for this item type" },
				GetMaxStackSize = { Params = "", Return = "number", Notes = "Returns the maximum stack size for this item." },
				IsDamageable = { Params = "", Return = "bool", Notes = "Returns true if this item does account for its damage" },
				IsEmpty = { Params = "", Return = "bool", Notes = "Returns true if this object represents an empty item (zero count or invalid ID)" },
				IsEqual = { Params = "cItem", Return = "bool", Notes = "Returns true if the item in the parameter is the same as the one stored in the object (type, damage and enchantments)" },
				IsFullStack = { Params = "", Return = "bool", Notes = "Returns true if the item is stacked up to its maximum stacking" },
				IsSameType = { Params = "cItem", Return = "bool", Notes = "Returns true if the item in the parameter is of the same ItemType as the one stored in the object. This is true even if the two items have different enchantments" },
				IsStackableWith = { Params = "cItem", Return = "bool", Notes = "Returns true if the item in the parameter is stackable with the one stored in the object. Two items with different enchantments cannot be stacked" },
			},
			Variables =
			{
				m_Enchantments = { Type = "{{cEnchantments}}", Notes = "The enchantments that this item has" },
				m_ItemCount    = { Type = "number", Notes = "Number of items in this stack" },
				m_ItemDamage   = { Type = "number", Notes = "The damage of the item. Zero means no damage. Maximum damage can be queried with GetMaxDamage()" },
				m_ItemType     = { Type = "number", Notes = "The item type. One of E_ITEM_ or E_BLOCK_ constants" },
			},
			AdditionalInfo =
			{
				{
					Header = "Usage notes",
					Contents = [[
						Note that the object contained in a cItem class is quite complex and quite often new Minecraft
						versions add more stuff. Therefore it is recommended to copy cItem objects using the
						copy-constructor ("local copy = cItem(original);"), this is the only way that guarantees that
						the object will be copied at full, even with future versions of MCServer.
					]],
				},
				{
					Header = "Example code",
					Contents = [[
						The following code shows how to create items in several different ways (adapted from the Debuggers plugin):
<pre class="prettyprint lang-lua">
-- empty item:
local Item1 = cItem();

-- enchanted sword, enchantment given as numeric string (bad style; see Item5):
local Item2 = cItem(E_ITEM_DIAMOND_SWORD, 1, 0, "1=1");

-- 1 undamaged shovel, no enchantment:
local Item3 = cItem(E_ITEM_DIAMOND_SHOVEL);

-- Add the Unbreaking enchantment. Note that Vanilla's levelcap isn't enforced:
Item3.m_Enchantments:SetLevel(cEnchantments.enchUnbreaking, 4);

-- 1 undamaged pickaxe, no enchantment:
local Item4 = cItem(E_ITEM_DIAMOND_PICKAXE);

-- Add multiple enchantments:
Item4.m_Enchantments:SetLevel(cEnchantments.enchUnbreaking, 5);
Item4.m_Enchantments:SetLevel(cEnchantments.enchEfficiency, 3);

-- enchanted chestplate, enchantment given as textual stringdesc (good style)
local Item5 = cItem(E_ITEM_DIAMOND_CHESTPLATE, 1, 0, "thorns=1;unbreaking=3");
</pre>
]],
				},
			},
		},  -- cItem

		cItemGrid =
		{
			Desc = [[This class represents a 2D array of items. It is used as the underlying storage and API for all cases that use a grid of items:
<li>{{cChestEntity|Chest}} contents</li>
<li>(TODO) Chest minecart contents</li>
<li>{{cDispenserEntity|Dispenser}} contents</li>
<li>{{cDropperEntity|Dropper}} contents</li>
<li>{{cFurnaceEntity|Furnace}} contents (?)</li>
<li>{{cHopperEntity|Hopper}} contents</li>
<li>(TODO) Hopper minecart contents</li>
<li>{{cPlayer|Player}} Inventory areas</li>
<li>(TODO) Trapped chest contents</li>
</p>
		<p>The items contained in this object are accessed either by a pair of XY coords, or a slot number (x + Width * y). There are functions available for converting between the two formats.
]],
			Functions =
			{
				AddItem = { Params = "{{cItem|cItem}}, [AllowNewStacks]", Return = "number", Notes = "Adds an item to the storage; if AllowNewStacks is true (default), will also create new stacks in empty slots. Returns the number of items added" },
				AddItems = { Params = "{{cItems|cItems}}, [AllowNewStacks]", Return = "number", Notes = "Same as AddItem, but for several items at once" },
				ChangeSlotCount =
				{
					{ Params = "SlotNum, AddToCount", Return = "number", Notes = "Adds AddToCount to the count of items in the specified slot. If the slot was empty, ignores the call. Returns the new count in the slot, or -1 if invalid SlotNum" },
					{ Params = "X, Y, AddToCount", Return = "number", Notes = "Adds AddToCount to the count of items in the specified slot. If the slot was empty, ignores the call. Returns the new count in the slot, or -1 if invalid slot coords" },
				},
				Clear = { Params = "", Return = "", Notes = "Empties all slots" },
				CopyToItems = { Params = "{{cItems|cItems}}", Return = "", Notes = "Copies all non-empty slots into the cItems object provided; original cItems contents are preserved" },
				DamageItem =
				{
					{ Params = "SlotNum, [DamageAmount]", Return = "bool", Notes = "Adds the specified damage (1 by default) to the specified item, returns true if the item reached its max damage and should be destroyed" },
					{ Params = "X, Y, [DamageAmount]", Return = "bool", Notes = "Adds the specified damage (1 by default) to the specified item, returns true if the item reached its max damage and should be destroyed" },
				},
				EmptySlot =
				{
					{ Params = "SlotNum", Return = "", Notes = "Destroys the item in the specified slot" },
					{ Params = "X, Y", Return = "", Notes = "Destroys the item in the specified slot" },
				},
				GetFirstEmptySlot = { Params = "", Return = "number", Notes = "Returns the SlotNumber of the first empty slot, -1 if all slots are full" },
				GetFirstUsedSlot = { Params = "", Return = "number", Notes = "Returns the SlotNumber of the first non-empty slot, -1 if all slots are empty" },
				GetHeight = { Params = "", Return = "number", Notes = "Returns the Y dimension of the grid" },
				GetLastEmptySlot = { Params = "", Return = "number", Notes = "Returns the SlotNumber of the last empty slot, -1 if all slots are full" },
				GetLastUsedSlot = { Params = "", Return = "number", Notes = "Returns the SlotNumber of the last non-empty slot, -1 if all slots are empty" },
				GetNextEmptySlot = { Params = "StartFrom", Return = "number", Notes = "Returns the SlotNumber of the first empty slot following StartFrom, -1 if all the following slots are full" },
				GetNextUsedSlot = { Params = "StartFrom", Return = "number", Notes = "Returns the SlotNumber of the first non-empty slot following StartFrom, -1 if all the following slots are full" },
				GetNumSlots = { Params = "", Return = "number", Notes = "Returns the total number of slots in the grid (Width * Height)" },
				GetSlot =
				{
					{ Params = "SlotNumber", Return = "{{cItem|cItem}}", Notes = "Returns the item in the specified slot. Note that the item is read-only" },
					{ Params = "X, Y", Return = "{{cItem|cItem}}", Notes = "Returns the item in the specified slot. Note that the item is read-only" },
				},
				GetSlotCoords = { Params = "SlotNum", Return = "number, number", Notes = "Returns the X and Y coords for the specified SlotNumber. Returns \"-1, -1\" on invalid SlotNumber" },
				GetSlotNum = { Params = "X, Y", Return = "number", Notes = "Returns the SlotNumber for the specified slot coords. Returns -1 on invalid coords" },
				GetWidth = { Params = "", Return = "number", Notes = "Returns the X dimension of the grid" },
				HasItems = { Params = "{{cItem|cItem}}", Return = "bool", Notes = "Returns true if there are at least as many items of the specified type as in the parameter" },
				HowManyCanFit = { Params = "{{cItem|cItem}}", Return = "number", Notes = "Returns the number of the specified items that can fit in the storage, including empty slots" },
				HowManyItems = { Params = "{{cItem|cItem}}", Return = "number", Notes = "Returns the number of the specified items that are currently stored" },
				IsSlotEmpty =
				{
					{ Params = "SlotNum", Return = "bool", Notes = "Returns true if the specified slot is empty, or an invalid slot is specified" },
					{ Params = "X, Y", Return = "bool", Notes = "Returns true if the specified slot is empty, or an invalid slot is specified" },
				},
				RemoveOneItem =
				{
					{ Params = "SlotNum", Return = "{{cItem|cItem}}", Notes = "Removes one item from the stack in the specified slot and returns it as a single cItem. Empty slots are skipped and an empty item is returned" },
					{ Params = "X, Y", Return = "{{cItem|cItem}}", Notes = "Removes one item from the stack in the specified slot and returns it as a single cItem. Empty slots are skipped and an empty item is returned" },
				},
				SetSlot =
				{
					{ Params = "SlotNum, {{cItem|cItem}}", Return = "", Notes = "Sets the specified slot to the specified item" },
					{ Params = "X, Y, {{cItem|cItem}}", Return = "", Notes = "Sets the specified slot to the specified item" },
				},
			},
			AdditionalInfo =
			{
				{
					Header = "Code example: Add items to player inventory",
					Contents = [[
						The following code tries to add 32 sticks to a player's main inventory:
<pre class="prettyprint lang-lua">
local Items = cItem(E_ITEM_STICK, 32);
local PlayerMainInventory = Player:GetInventorySlots();  -- PlayerMainInventory is of type cItemGrid
local NumAdded = PlayerMainInventory:AddItem(Items);
if (NumAdded == Items.m_ItemCount) then
  -- All the sticks did fit
  LOG("Added 32 sticks");
else
  -- Some (or all) of the sticks didn't fit
  LOG("Tried to add 32 sticks, but only " .. NumAdded .. " could fit");
end
</pre>
					]],
				},
				{
					Header = "Code example: Damage an item",
					Contents = [[
						The following code damages the helmet in the player's armor and destroys it if it reaches max damage:
<pre class="prettyprint lang-lua">
local PlayerArmor = Player:GetArmorSlots();  -- PlayerArmor is of type cItemGrid
if (PlayerArmor:DamageItem(0)) then  -- Helmet is at SlotNum 0
  -- The helmet has reached max damage, destroy it:
  PlayerArmor:EmptySlot(0);
end
</pre>
					]],
				},
			},  -- AdditionalInfo
		},  -- cItemGrid

		cItems =
		{
			Desc = [[
				This class represents a numbered collection (array) of {{cItem}} objects. The array indices start at
				zero, each consecutive item gets a consecutive index. This class is used for spawning multiple
				pickups or for mass manipulating an inventory.
				]],
			Functions =
			{
				constructor = { Params = "", Return = "cItems", Notes = "Creates a new cItems object" },
				Add =
				{
					{ Params = "{{cItem|cItem}}", Return = "", Notes = "Adds a new item to the end of the collection" },
					{ Params = "ItemType, ItemCount, ItemDamage", Return = "", Notes = "Adds a new item to the end of the collection" },
				},
				Clear = { Params = "", Return = "", Notes = "Removes all items from the collection" },
				Delete = { Params = "Index", Return = "", Notes = "Deletes item at the specified index" },
				Get = { Params = "Index", Return = "{{cItem|cItem}}", Notes = "Returns the item at the specified index" },
				Set =
				{
					{ Params = "Index, {{cItem|cItem}}", Return = "", Notes = "Sets the item at the specified index to the specified item" },
					{ Params = "Index, ItemType, ItemCount, ItemDamage", Return = "", Notes = "Sets the item at the specified index to the specified item" },
				},
				Size = { Params = "", Return = "number", Notes = "Returns the number of items in the collection" },
			},
		},  -- cItems

		cLineBlockTracer =
		{
			Desc = [[Objects of this class provide an easy-to-use interface to tracing lines through individual
blocks in the world. It will call the provided callbacks according to what events it encounters along the
way.</p>
<p>
For the Lua API, there's only one function exported that takes all the parameters necessary to do the
tracing. The Callbacks parameter is a table containing all the functions that will be called upon the
various events. See below for further information.
			]],
			Functions =
			{
				Trace = { Params = "{{cWorld}}, Callbacks, StartX, StartY, StartZ, EndX, EndY, EndZ", Return = "bool", Notes = "(STATIC) Performs the trace on the specified line. Returns true if the entire trace was processed (no callback returned true)" },
			},

			AdditionalInfo =
			{
				{
					Header = "Callbacks",
					Contents = [[
The Callbacks in the Trace() function is a table that contains named functions. MCServer will call
individual functions from that table for the events that occur on the line - hitting a block, going out of
valid world data etc. The following table lists all the available callbacks. If the callback function is
not defined, MCServer skips it. Each function can return a bool value, if it returns true, the tracing is
aborted and Trace() returns false.</p>
<p>
<table><tr><th>Name</th><th>Parameters</th><th>Notes</th></tr>
<tr><td>OnNextBlock</td><td>BlockX, BlockY, BlockZ, BlockType, BlockMeta, EntryFace</td>
	<td>Called when the ray hits a new valid block. The block type and meta is given. EntryFace is one of the
	BLOCK_FACE_ constants indicating which "side" of the block got hit by the ray.</td></tr>
<tr><td>OnNextBlockNoData</td><td>BlockX, BlockY, BlockZ, EntryFace</td>
	<td>Called when the ray hits a new block, but the block is in an unloaded chunk - no valid data is
	available. Only the coords and the entry face are given.</td></tr>
<tr><td>OnOutOfWorld</td><td>X, Y, Z</td>
	<td>Called when the ray goes outside of the world (Y-wise); the coords specify the exact exit point. Note
	that for other paths than lines (considered for future implementations) the path may leave the world and
	go back in again later, in such a case this callback is followed by OnIntoWorld() and further
	OnNextBlock() calls.</td></tr>
<tr><td>OnIntoWorld</td><td>X, Y, Z</td>
	<td>Called when the ray enters the world (Y-wise); the coords specify the exact entry point.</td></tr>
<tr><td>OnNoMoreHits</td><td>&nbsp;</td>
	<td>Called when the path is sure not to hit any more blocks. This is the final callback, no more
	callbacks are called after this function. Unlike the other callbacks, this function doesn't have a return
	value.</td></tr>
<tr><td>OnNoChunk</td><td>&nbsp;</td>
	<td>Called when the ray enters a chunk that is not loaded. This usually means that the tracing is aborted.
	Unlike the other callbacks, this function doesn't have a return value.</td></tr>
</table>
					]],
				},
				{
					Header = "Example",
					Contents = [[
<p>The following example is taken from the Debuggers plugin. It is a command handler function for the
"/spidey" command that creates a line of cobweb blocks from the player's eyes up to 50 blocks away in
the direction they're looking, but only through the air.
<pre class="prettyprint lang-lua">
function HandleSpideyCmd(a_Split, a_Player)
	local World = a_Player:GetWorld();

	local Callbacks = {
		OnNextBlock = function(a_BlockX, a_BlockY, a_BlockZ, a_BlockType, a_BlockMeta)
			if (a_BlockType ~= E_BLOCK_AIR) then
				-- abort the trace
				return true;
			end
			World:SetBlock(a_BlockX, a_BlockY, a_BlockZ, E_BLOCK_COBWEB, 0);
		end
	};

	local EyePos = a_Player:GetEyePosition();
	local LookVector = a_Player:GetLookVector();
	LookVector:Normalize();  -- Make the vector 1 m long

	-- Start cca 2 blocks away from the eyes
	local Start = EyePos + LookVector + LookVector;
	local End = EyePos + LookVector * 50;

	cLineBlockTracer.Trace(World, Callbacks, Start.x, Start.y, Start.z, End.x, End.y, End.z);

	return true;
end
</pre>
</p>
					]],
				},
			},  -- AdditionalInfo
		},  -- cLineBlockTracer

		cLuaWindow =
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
]],
			Functions =
			{
				constructor = { Params = "WindowType, ContentsWidth, ContentsHeight, Title", Return = "", Notes = "Creates a new object of this class" },
				GetContents = { Params = "", Return = "{{cItemGrid|cItemGrid}}", Notes = "Returns the cItemGrid object representing the internal storage in this window" },
				SetOnClosing = { Params = "OnClosingCallback", Return = "", Notes = "Sets the function that the window will call when it is about to be closed by a player" },
				SetOnSlotChanged = { Params = "OnSlotChangedCallback", Return = "", Notes = "Sets the function that the window will call when a slot is changed by a player" },
			},
			AdditionalInfo =
			{
				{
					Header = "Callbacks",
					Contents = [[
						The object calls the following functions at the appropriate time:
					]],
				},
				{
					Header = "OnClosing Callback",
					Contents = [[
						This callback, settable via the SetOnClosing() function, will be called when the player tries to close the window, or the window is closed for any other reason (such as a player disconnecting).</p>
<pre class="prettyprint lang-lua">
function OnWindowClosing(a_Window, a_Player, a_CanRefuse)
</pre>
						<p>
						The a_Window parameter is the cLuaWindow object representing the window, a_Player is the player for whom the window is about to close. a_CanRefuse specifies whether the callback can refuse the closing. If the callback returns true and a_CanRefuse is true, the window is not closed (internally, the server sends a new OpenWindow packet to the client).
					]],
				},
				{
					Header = "OnSlotChanged Callback",
					Contents = [[
						This callback, settable via the SetOnSlotChanged() function, will be called whenever the contents of any slot in the window's contents (i. e. NOT in the player inventory!) changes.</p>
<pre class="prettyprint lang-lua">
function OnWindowSlotChanged(a_Window, a_SlotNum)
</pre>
						<p>The a_Window parameter is the cLuaWindow object representing the window, a_SlotNum is the slot number. There is no reference to a {{cPlayer}}, because the slot change needn't originate from the player action. To get or set the slot, you'll need to retrieve a cPlayer object, for example by calling {{cWorld|cWorld}}:DoWithPlayer().
						</p>
						<p>Any returned values are ignored.
					]],
				},
				{
					Header = "Example",
					Contents = [[
						This example is taken from the Debuggers plugin, used to test the API functionality. It opens a window and refuse to close it 3 times. It also logs slot changes to the server console.
<pre class="prettyprint lang-lua">
-- Callback that refuses to close the window twice, then allows:
local Attempt = 1;
local OnClosing = function(Window, Player, CanRefuse)
	Player:SendMessage("Window closing attempt #" .. Attempt .. "; CanRefuse = " .. tostring(CanRefuse));
	Attempt = Attempt + 1;
	return CanRefuse and (Attempt <= 3);  -- refuse twice, then allow, unless CanRefuse is set to true
end

-- Log the slot changes:
local OnSlotChanged = function(Window, SlotNum)
	LOG("Window \"" .. Window:GetWindowTitle() .. "\" slot " .. SlotNum .. " changed.");
end

-- Set window contents:
-- a_Player is a cPlayer object received from the outside of this code fragment
local Window = cLuaWindow(cWindow.Hopper, 3, 3, "TestWnd");
Window:SetSlot(a_Player, 0, cItem(E_ITEM_DIAMOND, 64));
Window:SetOnClosing(OnClosing);
Window:SetOnSlotChanged(OnSlotChanged);

-- Open the window:
a_Player:OpenWindow(Window);
</pre>
					]],
				},
			},  -- AdditionalInfo
			Inherits = "cWindow",
		},  -- cLuaWindow

		cMonster =
		{
			Desc = [[
				This class is the base class for all computer-controlled mobs in the game.</p>
				<p>
				To spawn a mob in a world, use the {{cWorld}}:SpawnMob() function.
			]],
			Functions =
			{
				FamilyFromType = { Params = "{{cMonster#MobType|MobType}}", Return = "{{cMonster#MobFamily|MobFamily}}", Notes = "(STATIC) Returns the mob family ({{cMonster#MobFamily|mfXXX}} constants) based on the mob type ({{cMonster#MobType|mtXXX}} constants)" },
				GetMobFamily = { Params = "", Return = "{{cMonster#MobFamily|MobFamily}}", Notes = "Returns this mob's family ({{cMonster#MobFamily|mfXXX}} constant)" },
				GetMobType = { Params = "", Return = "{{cMonster#MobType|MobType}}", Notes = "Returns the type of this mob ({{cMonster#MobType|mtXXX}} constant)" },
				GetSpawnDelay = { Params = "{{cMonster#MobFamily|MobFamily}}", Return = "number", Notes = "(STATIC) Returns the spawn delay  - the number of game ticks between spawn attempts - for the specified mob family." },
				MobTypeToString = { Params = "{{cMonster#MobType|MobType}}", Return = "string", Notes = "(STATIC) Returns the string representing the given mob type ({{cMonster#MobType|mtXXX}} constant), or empty string if unknown type." },
				StringToMobType = { Params = "string", Return = "{{cMonster#MobType|MobType}}", Notes = "(STATIC) Returns the mob type ({{cMonster#MobType|mtXXX}} constant) parsed from the string type (\"creeper\"), or mtInvalidType if unrecognized." },
			},
			Constants =
			{
				mfAmbient = { Notes = "Family: ambient (bat)" },
				mfHostile = { Notes = "Family: hostile (blaze, cavespider, creeper, enderdragon, enderman, ghast, giant, magmacube, silverfish, skeleton, slime, spider, witch, wither, zombie, zombiepigman)" },
				mfMaxplusone = { Notes = "The maximum family value, plus one. Returned when monster family not recognized." },
				mfPassive = { Notes = "Family: passive (chicken, cow, horse, irongolem, mooshroom, ocelot, pig, sheep, snowgolem, villager, wolf)" },
				mfWater = { Notes = "Family: water (squid)" },
				mtBat = { Notes = "" },
				mtBlaze = { Notes = "" },
				mtCaveSpider = { Notes = "" },
				mtChicken = { Notes = "" },
				mtCow = { Notes = "" },
				mtCreeper = { Notes = "" },
				mtEnderDragon = { Notes = "" },
				mtEnderman = { Notes = "" },
				mtGhast = { Notes = "" },
				mtGiant = { Notes = "" },
				mtHorse = { Notes = "" },
				mtInvalidType = { Notes = "Invalid monster type. Returned when monster type not recognized" },
				mtIronGolem = { Notes = "" },
				mtMagmaCube = { Notes = "" },
				mtMooshroom = { Notes = "" },
				mtOcelot = { Notes = "" },
				mtPig = { Notes = "" },
				mtSheep = { Notes = "" },
				mtSilverfish = { Notes = "" },
				mtSkeleton = { Notes = "" },
				mtSlime = { Notes = "" },
				mtSnowGolem = { Notes = "" },
				mtSpider = { Notes = "" },
				mtSquid = { Notes = "" },
				mtVillager = { Notes = "" },
				mtWitch = { Notes = "" },
				mtWither = { Notes = "" },
				mtWolf = { Notes = "" },
				mtZombie = { Notes = "" },
				mtZombiePigman = { Notes = "" },
			},
			ConstantGroups =
			{
				MobFamily =
				{
					Include = "mf.*",
					TextBefore = [[
						Mobs are divided into families. The following constants are used for individual family types:
					]],
				},
				MobType =
				{
					Include = "mt.*",
					TextBefore = [[
						The following constants are used for distinguishing between the individual mob types:
					]],
				},
			},
			Inherits = "cPawn",
		},  -- cMonster

		cPawn =
		{
			Desc = [[cPawn is a controllable pawn object, controlled by either AI or a player. cPawn inherits all functions and members of {{cEntity}}
]],
			Functions =
			{
				TeleportToEntity = { Return = "" },
				TeleportTo = { Return = "" },
				Heal = { Return = "" },
				TakeDamage = { Return = "" },
				KilledBy = { Return = "" },
				GetHealth = { Return = "number" },
			},
			Inherits = "cEntity",
		},  -- cPawn

		cPickup =
		{
			Desc = [[
				This class represents a pickup entity (an item that the player or mobs can pick up). It is also
				commonly known as "drops". With this class you could create your own "drop" or modify those
				created automatically.
			]],
			Functions =
			{
				constructor = { Params = "PosX, PosY, PosZ, {{cItem|Item}}, IsPlayerCreated, [SpeedX, SpeedY, SpeedZ]", Return = "cPickup", Notes = "Creates a new pickup at the specified coords. If IsPlayerCreated is true, the pickup has a longer initial collection interval." },
				CollectedBy = { Params = "{{cPlayer}}", Return = "bool", Notes = "Tries to make the player collect the pickup. Returns true if the pickup was collected, at least partially." },
				GetAge = { Params = "", Return = "number", Notes = "Returns the number of ticks that the pickup has existed." },
				GetItem = { Params = "", Return = "{{cItem|cItem}}", Notes = "Returns the item represented by this pickup" },
				IsCollected = { Params = "", Return = "bool", Notes = "Returns true if this pickup has already been collected (is waiting to be destroyed)" },
				IsPlayerCreated = { Params = "", Return = "bool", Notes = "Returns true if the pickup was created by a player" },
			},
			Inherits = "cEntity",
		},  -- cPickup

		cPlayer =
		{
			Desc = [[
				This class describes a player in the server. cPlayer inherits all functions and members of
				{{cPawn|cPawn}}. It handles all the aspects of the gameplay, such as hunger, sprinting, inventory
				etc.
			]],
			Functions =
			{
				AddFoodExhaustion = { Params = "Exhaustion", Return = "", Notes = "Adds the specified number to the food exhaustion. Only positive numbers expected." },
				AddToGroup = { Params = "GroupName", Return = "", Notes = "Temporarily adds the player to the specified group. The assignment is lost when the player disconnects." },
				CalcLevelFromXp = { Params = "XPAmount", Return = "number", Notes = "Returns the level which is reached with the specified amount of XP. Inverse of XpForLevel()." },
				CanFly = { Return = "bool", Notes = "Returns if the player is able to fly." },
				CanUseCommand = { Params = "Command", Return = "bool", Notes = "Returns true if the player is allowed to use the specified command." },
				CloseWindow = { Params = "[CanRefuse]", Return = "", Notes = "Closes the currently open UI window. If CanRefuse is true (default), the window may refuse the closing." },
				CloseWindowIfID = { Params = "WindowID, [CanRefuse]", Return = "", Notes = "Closes the currently open UI window if its ID matches the given ID. If CanRefuse is true (default), the window may refuse the closing." },
				DeltaExperience = { Params = "DeltaXP", Return = "", Notes = "Adds or removes XP from the current XP amount. Won't allow XP to go negative. Returns the new experience, -1 on error (XP overflow)." },
				Feed = { Params = "AddFood, AddSaturation", Return = "bool", Notes = "Tries to add the specified amounts to food level and food saturation level (only positive amounts expected). Returns true if player was hungry and the food was consumed, false if too satiated." },
				FoodPoison = { Params = "NumTicks", Return = "", Notes = "Starts the food poisoning for the specified amount of ticks; if already foodpoisoned, sets FoodPoisonedTicksRemaining to the larger of the two" },
				ForceSetSpeed = { Params = "{{Vector3d|Direction}}", Notes = "Forces the player to move to the given direction." },
				GetAirLevel = { Params = "", Return = "number", Notes = "Returns the air level (number of ticks of air left)." },
				GetClientHandle = { Params = "", Return = "{{cClientHandle}}", Notes = "Returns the client handle representing the player's connection. May be nil (AI players)." },
				GetColor = { Return = "string", Notes = "Returns the full color code to be used for this player (based on the first group). Prefix player messages with this code." },
				GetCurrentXp = { Params = "", Return = "number", Notes = "Returns the current amount of XP" },
				GetEffectiveGameMode = { Params = "", Return = "{{eGameMode|GameMode}}", Notes = "Returns the current resolved game mode of the player. If the player is set to inherit the world's gamemode, returns that instead. See also GetGameMode() and IsGameModeXXX() functions." },
				GetEquippedItem = { Params = "", Return = "{{cItem}}", Notes = "Returns the item that the player is currently holding; empty item if holding nothing." },
				GetEyeHeight = { Return = "number", Notes = "Returns the height of the player's eyes, in absolute coords" },
				GetEyePosition = { Return = "{{Vector3d|EyePositionVector}}", Notes = "Returns the position of the player's eyes, as a {{Vector3d}}" },
				GetFoodExhaustionLevel = { Params = "", Return = "number", Notes = "Returns the food exhaustion level" },
				GetFoodLevel = { Params = "", Return = "number", Notes = "Returns the food level (number of half-drumsticks on-screen)" },
				GetFoodPoisonedTicksRemaining = { Params = "", Return = "", Notes = "Returns the number of ticks left for the food posoning effect" },
				GetFoodSaturationLevel = { Params = "", Return = "number", Notes = "Returns the food saturation (overcharge of the food level, is depleted before food level)" },
				GetFoodTickTimer = { Params = "", Return = "", Notes = "Returns the number of ticks past the last food-based heal or damage action; when this timer reaches 80, a new heal / damage is applied." },
				GetGameMode = { Return = "{{eGameMode|GameMode}}", Notes = "Returns the player's gamemode. The player may have their gamemode unassigned, in which case they inherit the gamemode from the current {{cWorld|world}}.<br /> <b>NOTE:</b> Instead of comparing the value returned by this function to the gmXXX constants, use the IsGameModeXXX() functions. These functions handle the gamemode inheritance automatically."},
				GetGroups = { Return = "array-table of {{cGroup}}", Notes = "Returns all the groups that this player is member of, as a table. The groups are stored in the array part of the table, beginning with index 1."},
				GetIP = { Return = "string", Notes = "Returns the IP address of the player, if available. Returns an empty string if there's no IP to report."},
				GetInventory = { Return = "{{cInventory|Inventory}}", Notes = "Returns the player's inventory"},
				GetMaxSpeed = { Params = "", Return = "number", Notes = "Returns the player's current maximum speed (as reported by the 1.6.1+ protocols)" },
				GetName = { Return = "string", Notes = "Returns the player's name" },
				GetNormalMaxSpeed = { Params = "", Return = "number", Notes = "Returns the player's maximum walking speed (as reported by the 1.6.1+ protocols)" },
				GetResolvedPermissions = { Return = "array-table of string", Notes = "Returns all the player's permissions, as a table. The permissions are stored in the array part of the table, beginning with index 1." },
				GetSprintingMaxSpeed = { Params = "", Return = "number", Notes = "Returns the player's maximum sprinting speed (as reported by the 1.6.1+ protocols)" },
				GetStance = { Return = "number", Notes = "Returns the player's stance (Y-pos of player's eyes)" },
				GetThrowSpeed = { Params = "SpeedCoeff", Return = "{{Vector3d}}", Notes = "Returns the speed vector for an object thrown with the specified speed coeff. Basically returns the normalized look vector multiplied by the coeff, with a slight random variation." },
				GetThrowStartPos = { Params = "", Return = "{{Vector3d}}", Notes = "Returns the position where the projectiles should start when thrown by this player." },
				GetWindow = { Params = "", Return = "{{cWindow}}", Notes = "Returns the currently open UI window. If the player doesn't have any UI window open, returns the inventory window." },
				GetXpLevel = { Params = "", Return = "number", Notes = "Returns the current XP level (based on current XP amount)." },
				GetXpLifetimeTotal = { Params = "", Return = "number", Notes = "Returns the amount of XP that has been accumulated throughout the player's lifetime." },
				GetXpPercentage = { Params = "", Return = "number", Notes = "Returns the percentage of the experience bar - the amount of XP towards the next XP level. Between 0 and 1." },
				HasPermission = { Params = "PermissionString", Return = "bool", Notes = "Returns true if the player has the specified permission" },
				Heal = { Params = "HitPoints", Return = "", Notes = "Heals the player by the specified amount of HPs. Only positive amounts are expected. Sends a health update to the client." },
				IsEating = { Params = "", Return = "bool", Notes = "Returns true if the player is currently eating the item in their hand." },
				IsFlying = { Return = "bool", Notes = "Returns true if the player is flying." },
				IsGameModeAdventure = { Params = "", Return = "bool", Notes = "Returns true if the player is in the gmAdventure gamemode, or has their gamemode unset and the world is a gmAdventure world." },
				IsGameModeCreative = { Params = "", Return = "bool", Notes = "Returns true if the player is in the gmCreative gamemode, or has their gamemode unset and the world is a gmCreative world." },
				IsGameModeSurvival = { Params = "", Return = "bool", Notes = "Returns true if the player is in the gmSurvival gamemode, or has their gamemode unset and the world is a gmSurvival world." },
				IsInGroup = { Params = "GroupNameString", Return = "bool", Notes = "Returns true if the player is a member of the specified group." },
				IsOnGround = { Params = "", Return = "bool", Notes = "Returns true if the player is on ground (not falling, not jumping, not flying)" },
				IsSatiated = { Params = "", Return = "bool", Notes = "Returns true if the player is satiated (cannot eat)." },
				IsSubmerged = { Params = "", Return = "bool", Notes = "Returns true if the player is submerged in water (the player's head is in a water block)" },
				IsSwimming = { Params = "", Return = "bool", Notes = "Returns true if the player is swimming in water (the player's feet are in a water block)" },
				IsVisible = { Params = "", Return = "bool", Notes = "Returns true if the player is visible to other players" },
				LoadPermissionsFromDisk = { Params = "", Return = "", Notes = "Reloads the player's permissions from the disk. This loses any temporary changes made to the player's groups." },
				MoveTo = { Params = "{{Vector3d|NewPosition}}", Return = "Tries to move the player into the specified position." },
				MoveToWorld = { Params = "WorldName", Return = "bool", Return = "Moves the player to the specified world. Returns true if successful." },
				OpenWindow = { Params = "{{cWindow|Window}}", Return = "", Notes = "Opens the specified UI window for the player." },
				RemoveFromGroup = { Params = "GroupName", Return = "", Notes = "Temporarily removes the player from the specified group. This change is lost when the player disconnects." },
				Respawn = { Params = "", Return = "", Notes = "Restores the health, extinguishes fire, makes visible and sends the Respawn packet." },
				SendMessage = { Params = "MessageString", Return = "", Notes = "Sends the specified message to the player." },
				SetCanFly = { Params = "CanFly", Notes = "Sets if the player can fly or not." },
				SetCrouch = { Params = "IsCrouched", Return = "", Notes = "Sets the crouch state, broadcasts the change to other players." },
				SetCurrentExperience = { Params = "XPAmount", Return = "", Notes = "Sets the current amount of experience (and indirectly, the XP level)." },
				SetFlying = { Params = "IsFlying", Notes = "Sets if the player is flying or not." },
				SetFoodExhaustionLevel = { Params = "ExhaustionLevel", Return = "", Notes = "Sets the food exhaustion to the specified level." },
				SetFoodLevel = { Params = "FoodLevel", Return = "", Notes = "Sets the food level (number of half-drumsticks on-screen)" },
				SetFoodPoisonedTicksRemaining = { Params = "FoodPoisonedTicksRemaining", Return = "", Notes = "Sets the number of ticks remaining for food poisoning. Doesn't send foodpoisoning effect to the client, use FoodPoison() for that." },
				SetFoodSaturationLevel = { Params = "FoodSaturationLevel", Return = "", Notes = "Sets the food saturation (overcharge of the food level)." },
				SetFoodTickTimer = { Params = "FoodTickTimer", Return = "", Notes = "Sets the number of ticks past the last food-based heal or damage action; when this timer reaches 80, a new heal / damage is applied." },
				SetGameMode = { Params = "{{eGameMode|NewGameMode}}", Return = "", Notes = "Sets the gamemode for the player. The new gamemode overrides the world's default gamemode, unless it is set to gmInherit." },
				SetName = { Params = "Name", Return = "", Notes = "Sets the player name. This rename will NOT be visible to any players already in the server who are close enough to see this player." },
				SetNormalMaxSpeed = { Params = "NormalMaxSpeed", Return = "", Notes = "Sets the normal (walking) maximum speed (as reported by the 1.6.1+ protocols)" },
				SetSprint = { Params = "IsSprinting", Return = "", Notes = "Sets whether the player is sprinting or not." },
				SetSprintingMaxSpeed = { Params = "SprintingMaxSpeed", Return = "", Notes = "Sets the sprinting maximum speed (as reported by the 1.6.1+ protocols)" },
				SetVisible = { Params = "IsVisible", Return = "", Notes = "Sets the player visibility to other players" },
				TossItem = { Params = "DraggedItem, [Amount], [CreateType], [CreateDamage]", Return = "", Notes = "FIXME: This function will be rewritten, avoid it. It tosses an item, either from the inventory, dragged in hand (while in UI window) or a newly created one." },
				XpForLevel = { Params = "XPLevel", Return = "number", Notes = "Returns the total amount of XP needed for the specified XP level. Inverse of CalcLevelFromXp()." },
			},
			Constants =
			{
				DROWNING_TICKS = { Notes = "Number of ticks per heart of damage when drowning (zero AirLevel)" },
				EATING_TICKS   = { Notes = "Number of ticks required for consuming an item." },
				MAX_AIR_LEVEL  = { Notes = "The maximum air level value. AirLevel gets reset to this value when the player exits water." },
				MAX_FOOD_LEVEL = { Notes = "The maximum food level value. When the food level is at this value, the player cannot eat." },
				MAX_HEALTH     = { Notes = "The maximum health value" },
			},
			Inherits = "cPawn",
		},  -- cPlayer

		cPlugin =
		{
			Desc = [[cPlugin describes a Lua plugin. This page is dedicated to new-style plugins and contain their functions. Each plugin has its own Plugin object.
]],
			Functions =
			{
				Call = { Params = "Function name, [All the parameters divided with commas]", Notes = "This function allows you to call a function from another plugin. It can only use pass: integers, booleans, strings and usertypes (cPlayer, cEntity, cCuboid, etc.)." },
				GetDirectory = { Return = "string", Notes = "Returns the name of the folder where the plugin's files are. (APIDump)" },
				GetLocalDirectory = { Notes = "OBSOLETE use GetLocalFolder instead." },
				GetLocalFolder = { Return = "string", Notes = "Returns the path where the plugin's files are. (Plugins/APIDump)" },
				GetName = { Return = "string", Notes = "Returns the name of the plugin." },
				SetName = { Params = "string", Notes = "Sets the name of the Plugin." },
				GetVersion = { Return = "number", Notes = "Returns the version of the plugin." },
				SetVersion = { Params = "number", Notes = "Sets the version of the plugin." },
				GetFileName = { Return = "string" },
				CreateWebPlugin = { Notes = "{{cWebPlugin|cWebPlugin}}" },
			},
		},  -- cPlugin

		cPluginLua =
		{
			Desc = "",
			Functions = {},
			Inherits = "cPlugin",
		},  -- cPluginLua

		cPluginManager =
		{
			Desc = [[
				This class is used for generic plugin-related functionality. The plugin manager has a list of all
				plugins, can enable or disable plugins, manages hooks and in-game console commands.</p>
				<p>
				There is one instance of cPluginManager in MCServer, to get it, call either
				{{cRoot|cRoot}}:Get():GetPluginManager() or cPluginManager:Get() function.</p>
				<p>
				Note that some functions are "static", that means that they are called using a dot operator instead
				of the colon operator. For example:
<pre class="prettyprint lang-lua">
cPluginManager.AddHook(cPluginManager.HOOK_CHAT, OnChatMessage);
</pre></p>
			]],
			Functions =
			{
				AddHook =
				{
					{ Params = "HookType, [HookFunction]", Return = "", Notes = "(STATIC) Informs the plugin manager that it should call the specified function when the specified hook event occurs. If a function is not specified, a default function name is looked up, based on the hook type" },
					{ Params = "{{cPlugin|Plugin}}, HookType, [HookFunction]", Return = "", Notes = "(STATIC, <b>DEPRECATED</b>) Informs the plugin manager that it should call the specified function when the specified hook event occurs. If a function is not specified, a default function name is looked up, based on the hook type. NOTE: This format is deprecated and the server outputs a warning if it is used!" },
				},
				BindCommand =
				{
					{ Params = "Command, Permission, Callback, HelpString", Return = "", Notes = "(STATIC) Binds an in-game command with the specified callback function, permission and help string. By common convention, providing an empty string for HelpString will hide the command from the /help display." },
					{ Params = "Command, Permission, Callback, HelpString", Return = "", Notes = "Binds an in-game command with the specified callback function, permission and help string. By common convention, providing an empty string for HelpString will hide the command from the /help display." },
				},
				BindConsoleCommand =
				{
					{ Params = "Command, Callback, HelpString", Return = "", Notes = "(STATIC) Binds a console command with the specified callback function and help string. By common convention, providing an empty string for HelpString will hide the command from the \"help\" console command." },
					{ Params = "Command, Callback, HelpString", Return = "", Notes = "Binds a console command with the specified callback function and help string. By common convention, providing an empty string for HelpString will hide the command from the \"help\" console command." },
				},
				DisablePlugin = { Params = "PluginName", Return = "bool", Notes = "Disables a plugin specified by its name. Returns true if the plugin was disabled, false if it wasn't found or wasn't active." },
				ExecuteCommand = { Params = "{{cPlayer|Player}}, CommandStr", Return = "bool", Notes = "Executes the command as if given by the specified Player. Checks permissions. Returns true if executed." },
				FindPlugins = { Params = "", Return = "", Notes = "Refreshes the list of plugins to include all folders inside the Plugins folder (potentially new disabled plugins)" },
				ForceExecuteCommand = { Params = "{{cPlayer|Player}}, CommandStr", Return = "bool", Notes = "Same as ExecuteCommand, but doesn't check permissions" },
				ForEachCommand = { Params = "CallbackFn", Return = "bool", Notes = "Calls the CallbackFn function for each command that has been bound using BindCommand(). The CallbackFn has the following signature: <pre class=\"prettyprint lang-lua\">function(Command, Permission, HelpString)</pre>. If the callback returns true, the enumeration is aborted and this API function returns false; if it returns false or no value, the enumeration continues with the next command, and the API function returns true." },
				ForEachConsoleCommand = { Params = "CallbackFn", Return = "bool", Notes = "Calls the CallbackFn function for each command that has been bound using BindConsoleCommand(). The CallbackFn has the following signature: <pre class=\"prettyprint lang-lua\">function (Command, HelpString)</pre>. If the callback returns true, the enumeration is aborted and this API function returns false; if it returns false or no value, the enumeration continues with the next command, and the API function returns true." },
				Get = { Params = "", Return = "cPluginManager", Notes = "(STATIC) Returns the single instance of the plugin manager" },
				GetAllPlugins = { Params = "", Return = "table", Notes = "Returns a table (dictionary) of all plugins, [name => {{cPlugin}}] pairing." },
				GetCommandPermission = { Params = "Command", Return = "Permission", Notes = "Returns the permission needed for executing the specified command" },
				GetCurrentPlugin = { Params = "", Return = "{{cPlugin}}", Notes = "Returns the {{cPlugin}} object for the calling plugin. This is the same object that the Initialize function receives as the argument." },
				GetNumPlugins = { Params = "", Return = "number", Notes = "Returns the number of plugins, including the disabled ones" },
				GetPlugin = { Params = "PluginName", Return = "{{cPlugin}}", Notes = "Returns a plugin handle of the specified plugin" },
				IsCommandBound = { Params = "Command", Return = "bool", Notes = "Returns true if in-game Command is already bound (by any plugin)" },
				IsConsoleCommandBound = { Params = "Command", Return = "bool", Notes = "Returns true if console Command is already bound (by any plugin)" },
				LoadPlugin = { Params = "PluginFolder", Return = "", Notes = "(<b>DEPRECATED</b>) Loads a plugin from the specified folder. NOTE: Loading plugins may be an unsafe operation and may result in a deadlock or a crash. This API is deprecated and might be removed." },
				ReloadPlugins = { Params = "", Return = "", Notes = "Reloads all active plugins" },
			},
			Constants =
			{
				HOOK_BLOCK_TO_PICKUPS = { Notes = "Called when a block has been dug and is being converted to pickups. The server has provided the default pickups and the plugins may modify them." },
				HOOK_CHAT = { Notes = "Called when a client sends a chat message that is not a command. The plugin may modify the chat message" },
				HOOK_CHUNK_AVAILABLE = { Notes = "Called when a chunk is loaded or generated and becomes available in the {{cWorld|world}}." },
				HOOK_CHUNK_GENERATED = { Notes = "Called after a chunk is generated. A plugin may do last modifications on the generated chunk before it is handed of to the {{cWorld|world}}." },
				HOOK_CHUNK_GENERATING = { Notes = "Called before a chunk is generated. A plugin may override some parts of the generation algorithm." },
				HOOK_CHUNK_UNLOADED = { Notes = "Called after a chunk has been unloaded from a {{cWorld|world}}." },
				HOOK_CHUNK_UNLOADING = { Notes = "Called before a chunk is unloaded from a {{cWorld|world}}. The chunk has already been saved." },
				HOOK_COLLECTING_PICKUP = { Notes = "Called when a player is about to collect a pickup." },
				HOOK_CRAFTING_NO_RECIPE = { Notes = "Called when a player has items in the crafting slots and the server cannot locate any recipe. Plugin may provide a recipe." },
				HOOK_DISCONNECT = { Notes = "Called after the player has disconnected." },
				HOOK_EXECUTE_COMMAND = { Notes = "Called when a client sends a chat message that is recognized as a command, before handing that command to the regular command handler. A plugin may stop the command from being handled. This hook is called even when the player doesn't have permissions for the command." },
				HOOK_EXPLODED = { Notes = "Called after an explosion has been processed in a {{cWorld|world}}." },
				HOOK_EXPLODING = { Notes = "Called before an explosion is processed in a {{cWorld|world}}. A plugin may alter the explosion parameters or cancel the explosion altogether." },
				HOOK_HANDSHAKE = { Notes = "Called when a Handshake packet is received from a client." },
				HOOK_HOPPER_PULLING_ITEM = { Notes = "Called when a hopper is pulling an item from the container above it." },
				HOOK_HOPPER_PUSHING_ITEM = { Notes = "Called when a hopper is pushing an item into the container it is aimed at." },
				HOOK_KILLING = { Notes = "Called when an entity has just been killed. A plugin may resurrect the entity by setting its health to above zero." },
				HOOK_LOGIN = { Notes = "Called when a Login packet is sent to the client, before the client is queued for authentication." },
				HOOK_MAX = { Notes = "The maximum TypeID of a hook. Used internally by MCS to check hook type for validity." },
				HOOK_NUM_HOOKS = { Notes = "Total number of hook types MCS supports. Used internally by MCS to check hook type for validity." },
				HOOK_PLAYER_ANIMATION = { Notes = "Called when a client send the Animation packet." },
				HOOK_PLAYER_BREAKING_BLOCK = { Notes = "Called when a player is about to break a block. A plugin may cancel the event." },
				HOOK_PLAYER_BROKEN_BLOCK = { Notes = "Called after a player has broken a block." },
				HOOK_PLAYER_EATING = { Notes = "Called when the player starts eating a held item. Plugins may abort the eating." },
				HOOK_PLAYER_JOINED = { Notes = "Called when the player entity has been created. It has not yet been fully initialized." },
				HOOK_PLAYER_LEFT_CLICK = { Notes = "Called when the client sends the LeftClick packet." },
				HOOK_PLAYER_MOVING = { Notes = "Called when the player has moved and the movement is now being applied." },
				HOOK_PLAYER_PLACED_BLOCK = { Notes = "Called when the player has just placed a block" },
				HOOK_PLAYER_PLACING_BLOCK = { Notes = "Called when the player is about to place a block. A plugin may cancel the event." },
				HOOK_PLAYER_RIGHT_CLICK = { Notes = "Called when the client sends the RightClick packet." },
				HOOK_PLAYER_RIGHT_CLICKING_ENTITY = { Notes = "Called when the client sends the UseEntity packet." },
				HOOK_PLAYER_SHOOTING = { Notes = "Called when the player releases the mouse button to fire their bow." },
				HOOK_PLAYER_SPAWNED = { Notes = "Called after the player entity has been created. The entity is fully initialized and is spawning in the {{cWorld|world}}." },
				HOOK_PLAYER_TOSSING_ITEM = { Notes = "Called when the player is tossing the held item (keypress Q)" },
				HOOK_PLAYER_USED_BLOCK = { Notes = "Called after the player has right-clicked a block" },
				HOOK_PLAYER_USED_ITEM = { Notes = "Called after the player has right-clicked with a usable item in their hand." },
				HOOK_PLAYER_USING_BLOCK = { Notes = "Called when the player is about to use (right-click) a block" },
				HOOK_PLAYER_USING_ITEM = { Notes = "Called when the player is about to right-click with a usable item in their hand." },
				HOOK_POST_CRAFTING = { Notes = "Called after a valid recipe has been chosen for the current contents of the crafting grid. Plugins may modify the recipe." },
				HOOK_PRE_CRAFTING = { Notes = "Called before a recipe is searched for the current contents of the crafting grid. Plugins may provide a recipe and cancel the built-in search." },
				HOOK_SPAWNED_ENTITY = { Notes = "Called after an entity is spawned in a {{cWorld|world}}. The entity is already part of the world." },
				HOOK_SPAWNED_MONSTER = { Notes = "Called after a mob is spawned in a {{cWorld|world}}. The mob is already part of the world." },
				HOOK_SPAWNING_ENTITY = { Notes = "Called just before an entity is spawned in a {{cWorld|world}}." },
				HOOK_SPAWNING_MONSTER = { Notes = "Called just before a mob is spawned in a {{cWorld|world}}." },
				HOOK_TAKE_DAMAGE = { Notes = "Called when an entity is taking any kind of damage. Plugins may modify the damage value, effects, source or cancel the damage." },
				HOOK_TICK = { Notes = "Called when the main server thread ticks - 20 times a second." },
				HOOK_UPDATED_SIGN = { Notes = "Called after a {{cSignEntity|sign}} text has been updated, either by a player or by any external means." },
				HOOK_UPDATING_SIGN = { Notes = "Called before a {{cSignEntity|sign}} text is updated, either by a player or by any external means." },
				HOOK_WEATHER_CHANGED = { Notes = "Called after the weather has changed." },
				HOOK_WEATHER_CHANGING = { Notes = "Called just before the weather changes" },
				HOOK_WORLD_TICK = { Notes = "Called in each world's tick thread when the game logic is about to tick (20 times a second)." },
			},
		},  -- cPluginManager

		cRoot =
		{
			Desc = [[
				This class represents the root of MCServer's object hierarchy. There is always only one cRoot
				object. It manages and allows querying all the other objects, such as {{cServer}},
				{{cPluginManager}}, individual {{cWorld|worlds}} etc.</p>
				<p>
				To get the singleton instance of this object, you call the cRoot:Get() function. Then you can call
				the individual functions on this object. Note that some of the functions are static and don't need
				the instance, they are to be called directly on the cRoot class, such as cRoot:GetPhysicalRAMUsage()
			]],
			Functions =
			{
				Get = { Params = "", Return = "Root object", Notes = "(STATIC)This function returns the cRoot object." },
				BroadcastChat = { Params = "Message", Return = "", Notes = "Broadcasts a message to every player in the server." },
				FindAndDoWithPlayer = { Params = "PlayerName, CallbackFunction", Return = "", Notes = "Calls the given callback function for the given player." },
				ForEachPlayer = { Params = "CallbackFunction", Return = "", Notes = "Calls the given callback function for each player. The callback function has the following signature: <pre class=\"prettyprint lang-lua\">function Callback({{cPlayer|cPlayer}})</pre>" },
				ForEachWorld = { Params = "CallbackFunction", Return = "", Notes = "Calls the given callback function for each world. The callback function has the following signature: <pre class=\"prettyprint lang-lua\">function Callback({{cWorld|cWorld}})</pre>" },
				GetCraftingRecipes = { Params = "", Return = "{{cCraftingRecipe|cCraftingRecipe}}", Notes = "Returns the CraftingRecipes object" },
				GetDefaultWorld = { Params = "", Return = "{{cWorld|cWorld}}", Notes = "Returns the world object from the default world." },
				GetFurnaceFuelBurnTime = { Params = "{{cItem|Fuel}}", Return = "number", Notes = "(STATIC) Returns the number of ticks for how long the item would fuel a furnace. Returns zero if not a fuel." },
				GetFurnaceRecipe = { Params = "{{cItem|InItem}}", Return = "{{cItem|OutItem}}, NumTicks, {{cItem|InItem}}", Notes = "(STATIC) Returns the furnace recipe for smelting the specified input. If a recipe is found, returns the smelted result, the number of ticks required for the smelting operation, and the input consumed (note that MCServer supports smelting M items into N items and different smelting rates). If no recipe is found, returns no value." },
				GetGroupManager = { Params = "", Return = "{{cGroupManager|cGroupManager}}", Notes = "Returns the cGroupManager object." },
				GetPhysicalRAMUsage = { Params = "", Return = "number", Notes = "Returns the amount of physical RAM that the entire MCServer process is using, in KiB. Negative if the OS doesn't support this query." },
				GetPluginManager = { Params = "", Return = "{{cPluginManager|cPluginManager}}", Notes = "Returns the cPluginManager object." },
				GetPrimaryServerVersion = { Params = "", Return = "number", Notes = "Returns the servers primary server version." },
				GetProtocolVersionTextFromInt = { Params = "Protocol Version", Return = "string", Notes = "Returns the Minecraft version from the given Protocol. If there is no version found, it returns 'Unknown protocol(Parameter)'" },
				GetServer = { Params = "", Return = "{{cServer|cServer}}", Notes = "Returns the cServer object." },
				GetTotalChunkCount = { Params = "", Return = "number", Notes = "Returns the amount of loaded chunks." },
				GetVirtualRAMUsage = { Params = "", Return = "number", Notes = "Returns the amount of virtual RAM that the entire MCServer process is using, in KiB. Negative if the OS doesn't support this query." },
				GetWebAdmin = { Params = "", Return = "{{cWebAdmin|cWebAdmin}}", Notes = "Returns the cWebAdmin object." },
				GetWorld = { Params = "WorldName", Return = "{{cWorld|cWorld}}", Notes = "Returns the cWorld object of the given world. It returns nil if there is no world with the given name." },
				QueueExecuteConsoleCommand = { Params = "Message", Return = "", Notes = "Queues a console command for execution through the cServer class. The command will be executed in the tick thread. The command's output will be sent to console." },
				SaveAllChunks = { Params = "", Return = "", Notes = "Saves all the chunks in all the worlds. Note that the saving is queued on each world's tick thread and this functions returns before the chunks are actually saved." },
				SetPrimaryServerVersion = { Params = "Protocol Version", Return = "", Notes = "Sets the servers PrimaryServerVersion to the given protocol number." }
			},
			AdditionalInfo =
			{
				{
					Header = "Querying a furnace recipe",
					Contents = [[
						To find the furnace recipe for an item, use the following code (adapted from the Debuggers plugin's /fr command):
<pre class="prettyprint lang-lua">
local HeldItem = a_Player:GetEquippedItem();
local Out, NumTicks, In = cRoot:GetFurnaceRecipe(HeldItem);  -- Note STATIC call - no need for a Get()
if (Out ~= nil) then
	-- There is a recipe, list it:
	a_Player:SendMessage(
		"Furnace turns " .. ItemToFullString(In) ..
		" to " .. ItemToFullString(Out) ..
		" in " .. NumTicks .. " ticks (" ..
		tostring(NumTicks / 20) .. " seconds)."
	);
else
	-- No recipe found
	a_Player:SendMessage("There is no furnace recipe that would smelt " .. ItemToString(HeldItem));
end
</pre>
					]],
				},
			},
		},  -- cRoot

		cServer =
		{
			Desc = [[
				This class manages all the client connections internally. In the API layer, it allows to get and set
				the general properties of the server, such as the description and max players.</p>
				<p>
				It used to support broadcasting chat messages to all players, this functionality has been moved to
				{{cRoot}}:BroadcastChat().
				]],
			Functions =
			{
				GetDescription = { Return = "string", Notes = "Returns the server description set in the settings.ini." },
				GetMaxPlayers = { Return = "number", Notes = "Returns the max amount of players who can join the server." },
				SetMaxPlayers = { Params = "number", Notes = "Sets the max amount of players who can join." },
				GetNumPlayers = { Return = "number", Notes = "Returns the amount of players online." },
				GetServerID = { Return = "string", Notes = "Returns the ID of the server?" },
				IsHardcore = { Params = "", Return = "bool", Notes = "Returns true if the server is hardcore (players get banned on death)." },
			},
		},  -- cServer

		cTNTEntity =
		{
			Desc = "This class manages a TNT entity.",
			Functions =
			{
				GetCounterTime = { Return = "number", Notes = "Returns the time until the entity explodes." },
				GetMaxFuseTime = { Return = "number", Notes = "Returns how long the fuse was." },
			},
			Inherits = "cEntity",
		},
		
		cTracer =
		{
			Desc = [[
				A cTracer object is used to trace lines in the world. One thing you can use the cTracer for, is
				tracing what block a player is looking at, but you can do more with it if you want.</p>
				<p>
				The cTracer is still a work in progress.</p>
				<p>
				See also the {{cLineBlockTracer}} class for an alternative approach using callbacks.
			]],
			Functions =
			{
			},
		},  -- cTracer

		cWebAdmin =
		{
			Desc = "",
			Functions =
			{
				GetHTMLEscapedString = { Params = "string", Return = "string", Notes = "Gets the HTML escaped representation of a requested string. This is useful for user input and game data that is not guaranteed to be escaped already." },
			},
		},  -- cWebAdmin

		cWebPlugin =
		{
			Desc = "",
			Functions = {},
		},  -- cWebPlugin

		cWindow =
		{
			Desc = [[
				This class is the common ancestor for all window classes used by MCServer. It is inherited by the
				{{cLuaWindow|cLuaWindow}} class that plugins use for opening custom windows. It is planned to be
				used for window-related hooks in the future. It implements the basic functionality of any
				window.</p>
				<p>
				Note that one cWindow object can be used for multiple players at the same time, and therefore the
				slot contents are player-specific (e. g. crafting grid, or player inventory). Thus the GetSlot() and
				SetSlot() functions need to have the {{cPlayer|cPlayer}} parameter that specifies the player for
				whom the contents are to be queried.</p>
				<p>
				Windows also have numeric properties, these are used to set the progressbars for furnaces or the XP
				costs for enchantment tables.
			]],
			Functions =
			{
				GetSlot = { Params = "{{cPlayer|Player}}, SlotNumber", Return = "{{cItem}}", Notes = "Returns the item at the specified slot for the specified player. Returns nil and logs to server console on error." },
				GetWindowID = { Params = "", Return = "number", Notes = "Returns the ID of the window, as used by the network protocol" },
				GetWindowTitle = { Params = "", Return = "string", Notes = "Returns the window title that will be displayed to the player" },
				GetWindowType = { Params = "", Return = "number", Notes = "Returns the type of the window, one of the constants in the table above" },
				IsSlotInPlayerHotbar = { Params = "SlotNum", Return = "bool", Notes = "Returns true if the specified slot number is in the player hotbar" },
				IsSlotInPlayerInventory = { Params = "SlotNum", Return = "bool", Notes = "Returns true if the specified slot number is in the player's main inventory or in the hotbar. Note that this returns false for armor slots!" },
				IsSlotInPlayerMainInventory = { Params = "SlotNum", Return = "bool", Notes = "Returns true if the specified slot number is in the player's main inventory" },
				SetProperty = { Params = "PropertyID, PropartyValue, {{cPlayer|Player}}", Return = "", Notes = "Sends the UpdateWindowProperty (0x69) packet to the specified player; or to all players who are viewing this window if Player is not specified or nil." },
				SetSlot = { Params = "{{cPlayer|Player}}, SlotNum, {{cItem|cItem}}", Return = "", Notes = "Sets the contents of the specified slot for the specified player. Ignored if the slot number is invalid" },
				SetWindowTitle = { Params = "string", Return = "", Notes = "Sets the window title that will be displayed to the player" },
			},
			Constants =
			{
				wtInventory = { Notes = "An inventory window" },
				wtChest = { Notes = "A {{cChestEntity|chest}} or doublechest window" },
				wtWorkbench = { Notes = "A workbench (crafting table) window" },
				wtFurnace = { Notes = "A {{cFurnaceEntity|furnace}} window" },
				wtDropSpenser = { Notes = "A {{cDropperEntity|dropper}} or a {{cDispenserEntity|dispenser}} window" },
				wtEnchantment = { Notes = "An enchantment table window" },
				wtBrewery = { Notes = "A brewing stand window" },
				wtNPCTrade = { Notes = "A villager trade window" },
				wtBeacon = { Notes = "A beacon window" },
				wtAnvil = { Notes = "An anvil window" },
				wtHopper = { Notes = "A {{cHopperEntity|hopper}} window" },
				wtAnimalChest = { Notes = "A horse or donkey window" },
			},
		},  -- cWindow

		cWorld =
		{
			Desc = [[
				cWorld is the game world. It is the hub of all the information managed by individual classes,
				providing convenient access to them. MCServer supports multiple worlds in any combination of
				world types. You can have two overworlds, three nethers etc. To enumerate all world the server
				provides, use the {{cRoot}}:ForEachWorld() function.</p>
				<p>
				The world data is held in individual chunks. Each chunk consists of 16 (x) * 16 (z) * 256 (y)
				blocks, each block is specified by its block type (8-bit) and block metadata (4-bit).
				Additionally, each block has two light values calculated - skylight (how much daylight it receives)
				and blocklight (how much light from light-emissive blocks it receives), both 4-bit.</p>
				<p>
				Each world runs several separate threads used for various housekeeping purposes, the most important
				of those is the Tick thread. This thread updates the game logic 20 times per second, and it is
				the thread where all the gameplay actions are evaluated. Liquid physics, entity interactions,
				player ovement etc., all are applied in this thread.</p>
				<p>
				Additional threads include the generation thread (generates new chunks as needed, storage thread
				(saves and loads chunk from the disk), lighting thread (updates block light values) and the
				chunksender thread (compresses chunks to send to the clients).</p>
				<p>
				The world provides access to all its {{cPlayer|players}}, {{cEntity|entities}} and {{cBlockEntity|block
				entities}}. Because of multithreading issues, individual objects cannot be retrieved for indefinite
				handling, but rather must be modified in callbacks, within which they are guaranteed to stay valid.</p>
				<p>
				Physics for individual blocks are handled by the simulators. These will fire in each tick for all
				blocks that have been scheduled for simulator update ("simulator wakeup"). The simulators include
				liquid physics, falling blocks, fire spreading and extinguishing and redstone.</p>
				<p>
				Game time is also handled by the world. It provides the time-of-day and the total world age.
			]],

			Functions =
			{
				BroadcastChat = { Params = "Message, [{{cClientHandle|ExcludeClient}}]", Return = "", Notes = "Sends the Message to all players in this world, except the optional ExceptClient" },
				BroadcastSoundEffect = { Params = "SoundName, X, Y, Z, Volume, Pitch, [{{cClientHandle|ExcludeClient}}]", Return = "", Notes = "Sends the specified sound effect to all players in this world, except the optional ExceptClient" },
				BroadcastSoundParticleEffect = { Params = "EffectID, X, Y, Z, EffectData, [{{cClientHandle|ExcludeClient}}]", Return = "", Notes = "Sends the specified effect to all players in this world, except the optional ExceptClient" },
				CastThunderbolt = { Params = "X, Y, Z", Return = "", Notes = "Creates a thunderbolt at the specified coords" },
				ChangeWeather = { Params = "", Return = "", Notes = "Forces the weather to change in the next game tick. Weather is changed according to the normal rules: wSunny <-> wRain <-> wStorm" },
				CreateProjectile = { Params = "X, Y, Z, {{cProjectileEntity|ProjectileKind}}, {{cEntity|Creator}}, [{{Vector3d|Speed}}]", Return = "", Notes = "Creates a new projectile of the specified kind at the specified coords. The projectile's creator is set to Creator (may be nil). Optional speed indicates the initial speed for the projectile." },
				DigBlock = { Params = "X, Y, Z", Return = "", Notes = "Replaces the specified block with air, without dropping the usual pickups for the block. Wakes up the simulators for the block and its neighbors." },
				DoExplosionAt = { Params = "Force, X, Y, Z, CanCauseFire, Source, SourceData", Return = "", Notes = "Creates an explosion of the specified relative force in the specified position. If CanCauseFire is set, the explosion will set blocks on fire, too. The Source parameter specifies the source of the explosion, one of the esXXX constants. The SourceData parameter is specific to each source type, usually it provides more info about the source." },
				DoWithBlockEntityAt = { Params = "X, Y, Z, CallbackFunction, [CallbackData]", Return = "bool", Notes = "If there is a block entity at the specified coords, calls the CallbackFunction with the {{cBlockEntity}} parameter representing the block entity. The CallbackFunction has the following signature: <pre class=\"prettyprint lang-lua\">function Callback({{cBlockEntity|BlockEntity}}, [CallbackData])</pre> The function returns false if there is no block entity, or if there is, it returns the bool value that the callback has returned. Use {{tolua}}.cast() to cast the Callback's BlockEntity parameter to the correct {{cBlockEntity}} descendant." },
				DoWithChestAt = { Params = "X, Y, Z, CallbackFunction, [CallbackData]", Return = "bool", Notes = "If there is a chest at the specified coords, calls the CallbackFunction with the {{cChestEntity}} parameter representing the chest. The CallbackFunction has the following signature: <pre class=\"prettyprint lang-lua\">function Callback({{cChestEntity|ChestEntity}}, [CallbackData])</pre> The function returns false if there is no chest, or if there is, it returns the bool value that the callback has returned." },
				DoWithDispenserAt = { Params = "X, Y, Z, CallbackFunction, [CallbackData]", Return = "bool", Notes = "If there is a dispenser at the specified coords, calls the CallbackFunction with the {{cDispenserEntity}} parameter representing the dispenser. The CallbackFunction has the following signature: <pre class=\"prettyprint lang-lua\">function Callback({{cDispenserEntity|DispenserEntity}}, [CallbackData])</pre> The function returns false if there is no dispenser, or if there is, it returns the bool value that the callback has returned." },
				DoWithDropSpenserAt = { Params = "X, Y, Z, CallbackFunction, [CallbackData]", Return = "bool", Notes = "If there is a dropper or a dispenser at the specified coords, calls the CallbackFunction with the {{cDropSpenserEntity}} parameter representing the dropper or dispenser. The CallbackFunction has the following signature: <pre class=\"prettyprint lang-lua\">function Callback({{cDropSpenserEntity|DropSpenserEntity}}, [CallbackData])</pre> Note that this can be used to access both dispensers and droppers in a similar way. The function returns false if there is neither dispenser nor dropper, or if there is, it returns the bool value that the callback has returned." },
				DoWithDropperAt = { Params = "X, Y, Z, CallbackFunction, [CallbackData]", Return = "bool", Notes = "If there is a dropper at the specified coords, calls the CallbackFunction with the {{cDropperEntity}} parameter representing the dropper. The CallbackFunction has the following signature: <pre class=\"prettyprint lang-lua\">function Callback({{cDropperEntity|DropperEntity}}, [CallbackData])</pre> The function returns false if there is no dropper, or if there is, it returns the bool value that the callback has returned." },
				DoWithEntityByID = { Params = "EntityID, CallbackFunction, [CallbackData]", Return = "bool", Notes = "If an entity with the specified ID exists, calls the callback with the {{cEntity}} parameter representing the entity. The CallbackFunction has the following signature: <pre class=\"prettyprint lang-lua\">function Callback({{cEntity|Entity}}, [CallbackData])</pre> The function returns false if the entity was not found, and it returns the same bool value that the callback has returned if the entity was found." },
				DoWithFurnaceAt = { Params = "X, Y, Z, CallbackFunction, [CallbackData]", Return = "bool", Notes = "If there is a furnace at the specified coords, calls the CallbackFunction with the {{cFurnaceEntity}} parameter representing the furnace. The CallbackFunction has the following signature: <pre class=\"prettyprint lang-lua\">function Callback({{cFurnaceEntity|FurnaceEntity}}, [CallbackData])</pre> The function returns false if there is no furnace, or if there is, it returns the bool value that the callback has returned." },
				DoWithPlayer = { Params = "PlayerName, CallbackFunction, [CallbackData]", Return = "bool", Notes = "If there is a player of the specified name (exact match), calls the CallbackFunction with the {{cPlayer}} parameter representing the player. The CallbackFunction has the following signature: <pre class=\"prettyprint lang-lua\">function Callback({{cPlayer|Player}}, [CallbackData])</pre> The function returns false if the player was not found, or whatever bool value the callback returned if the player was found." },
				FastSetBlock =
				{
					{ Params = "X, Y, Z, BlockType, BlockMeta", Return = "", Notes = "Sets the block at the specified coords, without waking up the simulators or replacing the block entities for the previous block type. Do not use if the block being replaced has a block entity tied to it!" },
					{ Params = "{{Vector3i|BlockCoords}}, BlockType, BlockMeta", Return = "", Notes = "Sets the block at the specified coords, without waking up the simulators or replacing the block entities for the previous block type. Do not use if the block being replaced has a block entity tied to it!" },
				},
				FindAndDoWithPlayer = { Params = "PlayerNameHint, CallbackFunction, [CallbackData]", Return = "bool", Notes = "If there is a player of a name similar to the specified name (weighted-match), calls the CallbackFunction with the {{cPlayer}} parameter representing the player. The CallbackFunction has the following signature: <pre class=\"prettyprint lang-lua\">function Callback({{cPlayer|Player}}, [CallbackData])</pre> The function returns false if the player was not found, or whatever bool value the callback returned if the player was found. Note that the name matching is very loose, so it is a good idea to check the player name in the callback function." },
				ForEachBlockEntityInChunk = { Params = "ChunkX, ChunkZ, CallbackFunction, [CallbackData]", Return = "bool", Notes = "Calls the specified callback for each block entity in the chunk. Returns true if all block entities in the chunk have been processed (including when there are zero block entities), or false if the callback has aborted the enumeration by returning true. The CallbackFunction has the following signature: <pre class=\"prettyprint lang-lua\">function Callback({{cBlockEntity|BlockEntity}}, [CallbackData])</pre> The callback should return false or no value to continue with the next block entity, or true to abort the enumeration. Use {{tolua}}.cast() to cast the Callback's BlockEntity parameter to the correct {{cBlockEntity}} descendant." },
				ForEachChestInChunk = { Params = "ChunkX, ChunkZ, CallbackFunction, [CallbackData]", Return = "bool", Notes = "Calls the specified callback for each chest in the chunk. Returns true if all chests in the chunk have been processed (including when there are zero chests), or false if the callback has aborted the enumeration by returning true. The CallbackFunction has the following signature: <pre class=\"prettyprint lang-lua\">function Callback({{cChestEntity|ChestEntity}}, [CallbackData])</pre> The callback should return false or no value to continue with the next chest, or true to abort the enumeration." },
				ForEachEntity = { Params = "CallbackFunction, [CallbackData]", Return = "bool", Notes = "Calls the specified callback for each entity in the loaded world. Returns true if all the entities have been processed (including when there are zero entities), or false if the callback function has aborted the enumeration by returning true. The callback function has the following signature: <pre class=\"prettyprint lang-lua\">function Callback({{cEntity|Entity}}, [CallbackData])</pre> The callback should return false or no value to continue with the next entity, or true to abort the enumeration." },
				ForEachEntityInChunk = { Params = "ChunkX, ChunkZ, CallbackFunction, [CallbackData]", Return = "bool", Notes = "Calls the specified callback for each entity in the specified chunk. Returns true if all the entities have been processed (including when there are zero entities), or false if the chunk is not loaded or the callback function has aborted the enumeration by returning true. The callback function has the following signature: <pre class=\"prettyprint lang-lua\">function Callback({{cEntity|Entity}}, [CallbackData])</pre> The callback should return false or no value to continue with the next entity, or true to abort the enumeration." },
				ForEachFurnaceInChunk = { Params = "ChunkX, ChunkZ, CallbackFunction, [CallbackData]", Return = "bool", Notes = "Calls the specified callback for each furnace in the chunk. Returns true if all furnaces in the chunk have been processed (including when there are zero furnaces), or false if the callback has aborted the enumeration by returning true. The CallbackFunction has the following signature: <pre class=\"prettyprint lang-lua\">function Callback({{cFurnaceEntity|FurnaceEntity}}, [CallbackData])</pre> The callback should return false or no value to continue with the next furnace, or true to abort the enumeration." },
				ForEachPlayer = { Params = "CallbackFunction, [CallbackData]", Return = "bool", Notes = "Calls the specified callback for each player in the loaded world. Returns true if all the players have been processed (including when there are zero players), or false if the callback function has aborted the enumeration by returning true. The callback function has the following signature: <pre class=\"prettyprint lang-lua\">function Callback({{cPlayer|Player}}, [CallbackData])</pre> The callback should return false or no value to continue with the next player, or true to abort the enumeration." },
				GenerateChunk = { Params = "ChunkX, ChunkZ", Return = "", Notes = "Queues the specified chunk in the chunk generator. Ignored if the chunk is already generated (use RegenerateChunk() to force chunk re-generation)." },
				GetBiomeAt = { Params = "BlockX, BlockZ", Return = "eBiome", Notes = "Returns the biome at the specified coords. Reads the biome from the chunk, if it is loaded, otherwise it uses the chunk generator to provide the biome value." },
				GetBlock =
				{
					{ Params = "BlockX, BlockY, BlockZ", Return = "BLOCKTYPE", Notes = "Returns the block type of the block at the specified coords, or 0 if the appropriate chunk is not loaded." },
					{ Params = "{{Vector3i|BlockCoords}}", Return = "BLOCKTYPE", Notes = "Returns the block type of the block at the specified coords, or 0 if the appropriate chunk is not loaded." },
				},
				GetBlockBlockLight = { Params = "BlockX, BlockY, BlockZ", Return = "number", Notes = "Returns the amount of block light at the specified coords, or 0 if the appropriate chunk is not loaded." },
				GetBlockInfo = { Params = "BlockX, BlockY, BlockZ", Return = "BlockValid, BlockType, BlockMeta, BlockSkyLight, BlockBlockLight", Notes = "Returns the complete block info for the block at the specified coords. The first value specifies if the block is in a valid loaded chunk, the other values are valid only if BlockValid is true." },
				GetBlockMeta =
				{
					{ Params = "BlockX, BlockY, BlockZ", Return = "number", Notes = "Returns the block metadata of the block at the specified coords, or 0 if the appropriate chunk is not loaded." },
					{ Params = "{{Vector3i|BlockCoords}}", Return = "number", Notes = "Returns the block metadata of the block at the specified coords, or 0 if the appropriate chunk is not loaded." },
				},
				GetBlockSkyLight = { Params = "BlockX, BlockY, BlockZ", Return = "number", Notes = "Returns the block skylight of the block at the specified coords, or 0 if the appropriate chunk is not loaded." },
				GetBlockTypeMeta = { Params = "BlockX, BlockY, BlockZ", Return = "BlockValid, BlockType, BlockMeta", Notes = "Returns the block type and metadata for the block at the specified coords. The first value specifies if the block is in a valid loaded chunk, the other values are valid only if BlockValid is true." },
				GetDimension = { Params = "", Return = "eDimension", Notes = "Returns the dimension of the world - dimOverworld, dimNether or dimEnd." },
				GetGameMode = { Params = "", Return = "eGameMode", Notes = "Returns the gamemode of the world - gmSurvival, gmCreative or gmAdventure." },
				GetGeneratorQueueLength = { Params = "", Return = "number", Notes = "Returns the number of chunks that are queued in the chunk generator." },
				GetHeight = { Params = "BlockX, BlockZ", Return = "number", Notes = "Returns the maximum height of the particula block column in the world. If the chunk is not loaded, it waits for it to load / generate. <b>WARNING</b>: Do not use, Use TryGetHeight() instead for a non-waiting version, otherwise you run the risk of a deadlock!" },
				GetIniFileName = { Params = "", Return = "string", Notes = "Returns the name of the world.ini file that the world uses to store the information." },
				GetLightingQueueLength = { Params = "", Return = "number", Notes = "Returns the number of chunks in the lighting thread's queue." },
				GetMaxCactusHeight = { Params = "", Return = "number", Notes = "Returns the configured maximum height to which cacti will grow naturally." },
				GetMaxSugarcaneHeight = { Params = "", Return = "number", Notes = "Returns the configured maximum height to which sugarcane will grow naturally." },
				GetName = { Params = "", Return = "string", Notes = "Returns the name of the world, as specified in the settings.ini file." },
				GetNumChunks = { Params = "", Return = "number", Notes = "Returns the number of chunks currently loaded." },
				GetSignLines = { Params = "BlockX, BlockY, BlockZ", Return = "IsValid, [Line1, Line2, Line3, Line4]", Notes = "Returns true and the lines of a sign at the specified coords, or false if there is no sign at the coords." },
				GetSpawnX = { Params = "", Return = "number", Notes = "Returns the X coord of the default spawn" },
				GetSpawnY = { Params = "", Return = "number", Notes = "Returns the Y coord of the default spawn" },
				GetSpawnZ = { Params = "", Return = "number", Notes = "Returns the Z coord of the default spawn" },
				GetStorageLoadQueueLength = { Params = "", Return = "number", Notes = "Returns the number of chunks queued up for loading" },
				GetStorageSaveQueueLength = { Params = "", Return = "number", Notes = "Returns the number of chunks queued up for saving" },
				GetTicksUntilWeatherChange = { Params = "", Return = "number", Notes = "Returns the number of ticks that will pass before the weather is changed" },
				GetTimeOfDay = { Params = "", Return = "number", Notes = "Returns the number of ticks that have passed from the sunrise, 0 .. 24000." },
				GetWeather = { Params = "", Return = "eWeather", Notes = "Returns the current weather in the world (wSunny, wRain, wStorm). To check for weather, use IsWeatherXXX() functions instead." },
				GetWorldAge = { Params = "", Return = "number", Notes = "Returns the total age of the world, in ticks. The age always grows, cannot be set by plugins and is unrelated to TimeOfDay." },
				GrowCactus = { Params = "BlockX, BlockY, BlockZ, NumBlocksToGrow", Return = "", Notes = "Grows a cactus block at the specified coords, by up to the specified number of blocks. Adheres to the world's maximum cactus growth (GetMaxCactusHeight())." },
				GrowMelonPumpkin = { Params = "BlockX, BlockY, BlockZ, StemType", Return = "", Notes = "Grows a melon or pumpkin, based on the stem type specified (assumed to be in the coords provided). Checks for normal melon / pumpkin growth conditions - stem not having another produce next to it and suitable ground below." },
				GrowRipePlant = { Params = "BlockX, BlockY, BlockZ, IsByBonemeal", Return = "bool", Notes = "Grows the plant at the specified coords. If IsByBonemeal is true, checks first if the specified plant type is bonemealable in the settings. Returns true if the plant was grown, false if not." },
				GrowSugarcane = { Params = "BlockX, BlockY, BlockZ, NumBlocksToGrow", Return = "", Notes = "Grows a sugarcane block at the specified coords, by up to the specified number of blocks. Adheres to the world's maximum sugarcane growth (GetMaxSugarcaneHeight())." },
				GrowTree = { Params = "BlockX, BlockY, BlockZ", Return = "", Notes = "Grows a tree based at the specified coords. If there is a sapling there, grows the tree based on that sapling, otherwise chooses a tree image based on the biome." },
				GrowTreeByBiome = { Params = "BlockX, BlockY, BlockZ", Return = "", Notes = "Grows a tree based at the specified coords. The tree type is picked from types available for the biome at those coords." },
				GrowTreeFromSapling = { Params = "BlockX, BlockY, BlockZ, SaplingMeta", Return = "", Notes = "Grows a tree based at the specified coords. The tree type is determined from the sapling meta (the sapling itself needn't be present)." },
				IsBlockDirectlyWatered = { Params = "BlockX, BlockY, BlockZ", Return = "bool", Notes = "Returns true if the specified block has a water block right next to it (on the X/Z axes)" },
				IsDeepSnowEnabled = { Params = "", Return = "bool", Notes = "Returns whether the configuration has DeepSnow enabled." },
				IsGameModeAdventure = { Params = "", Return = "bool", Notes = "Returns true if the current gamemode is gmAdventure." },
				IsGameModeCreative = { Params = "", Return = "bool", Notes = "Returns true if the current gamemode is gmCreative." },
				IsGameModeSurvival = { Params = "", Return = "bool", Notes = "Returns true if the current gamemode is gmSurvival." },
				IsPVPEnabled = { Params = "", Return = "bool", Notes = "Returns whether PVP is enabled in the world settings." },
				IsWeatherRain = { Params = "", Return = "bool", Notes = "Returns true if the current weather is rain." },
				IsWeatherStorm = { Params = "", Return = "bool", Notes = "Returns true if the current weather is a storm." },
				IsWeatherSunny = { Params = "", Return = "bool", Notes = "Returns true if the current weather is sunny." },
				IsWeatherWet = { Params = "", Return = "bool", Notes = "Returns true if the current weather has any precipitation (rain or storm)." },
				QueueBlockForTick = { Params = "BlockX, BlockY, BlockZ, TicksToWait", Return = "", Notes = "Queues the specified block to be ticked after the specified number of gameticks." },
				QueueSaveAllChunks = { Params = "", Return = "", Notes = "Queues all chunks to be saved in the world storage thread" },
				QueueSetBlock = { Params = "BlockX, BlockY, BlockZ, BlockType, BlockMeta, TickDelay", Return = "", Notes = "Queues the block to be set to the specified blocktype and meta after the specified amount of game ticks. Uses SetBlock() for the actual setting, so simulators are woken up and block entities are handled correctly." },
				QueueTask = { Params = "TaskFunction", Return = "", Notes = "Queues the specified function to be executed in the tick thread. This is the primary means of interaction with a cWorld from the WebAdmin page handlers (see {{WebWorldThreads}}). The function signature is <pre class=\"pretty-print lang-lua\">function()</pre>All return values from the function are ignored. Note that this function is actually called *after* the QueueTask() function returns." },
				RegenerateChunk = { Params = "ChunkX, ChunkZ", Return = "", Notes = "Queues the specified chunk to be re-generated, overwriting the current data. To queue a chunk for generating only if it doesn't exist, use the GenerateChunk() instead." },
				SendBlockTo = { Params = "BlockX, BlockY, BlockZ, {{cPlayer|Player}}", Return = "", Notes = "Sends the block at the specified coords to the specified player's client, as an UpdateBlock packet." },
				SetBlock = { Params = "BlockX, BlockY, BlockZ, BlockType, BlockMeta", Return = "", Notes = "Sets the block at the specified coords, replaces the block entities for the previous block type, creates a new block entity for the new block, if appropriate, and wakes up the simulators. This is the preferred way to set blocks, as opposed to FastSetBlock(), which is only to be used under special circumstances." },
				SetBlockMeta =
				{
					{ Params = "BlockX, BlockY, BlockZ, BlockMeta", Return = "", Notes = "Sets the meta for the block at the specified coords." },
					{ Params = "{{Vector3i|BlockCoords}}, BlockMeta", Return = "", Notes = "Sets the meta for the block at the specified coords." },
				},
				SetNextBlockTick = { Params = "BlockX, BlockY, BlockZ", Return = "", Notes = "Sets the blockticking to start at the specified block in the next tick." },
				SetSignLines = { Params = "X, Y, Z, Line1, Line2, Line3, Line4, [{{cPlayer|Player}}]", Return = "", Notes = "Sets the sign text at the specified coords. The sign-updating hooks are called for the change. The Player parameter is used to indicate the player from whom the change has come, it may be nil. Same as UpdateSign()." },
				SetTicksUntilWeatherChange = { Params = "NumTicks", Return = "", Notes = "Sets the number of ticks after which the weather will be changed." },
				SetTimeOfDay = { Params = "TimeOfDayTicks", Return = "", Notes = "Sets the time of day, expressed as number of ticks past sunrise, in the range 0 .. 24000." },
				SetWeather = { Params = "Weather", Return = "", Notes = "Sets the current weather (wSunny, wRain, wStorm) and resets the TicksUntilWeatherChange to the default value for the new weather. The normal weather-changing hooks are called for the change." },
				SpawnItemPickups =
				{
					{ Params = "{{cItems|Pickups}}, X, Y, Z, FlyAwaySpeed", Return = "", Notes = "Spawns the specified pickups at the position specified. The FlyAway speed is used to initialize the random speed in which the pickups fly away from the spawn position." },
					{ Params = "{{cItems|Pickups}}, X, Y, Z, SpeedX, SpeedY, SpeedZ", Return = "", Notes = "Spawns the specified pickups at the position specified. All the pickups fly away from the spawn position using the specified speed." },
				},
				SpawnMob = { Params = "X, Y, Z, {{cMonster|MonsterType}}", Return = "EntityID", Notes = "Spawns the specified type of mob at the specified coords. Returns the EntityID of the creates entity, or -1 on failure. " },
				SpawnFallingBlock = { Params = "X, Y, Z, BlockType, BlockMeta", Return = "EntityID", Notes = "Spawns an {{cFallingBlock|Falling Block}} entity at the specified coords with the given block type/meta" },
				SpawnExperienceOrb = { Params = "X, Y, Z, Reward", Return = "EntityID", Notes = "Spawns an {{cExpOrb|experience orb}} at the specified coords, with the given reward" },
				SpawnPrimedTNT = { Params = "X, Y, Z, FuseTimeSecs, InitialVelocityCoeff", Return = "", Notes = "Spawns a {{cTNTEntity|primed TNT entity}} at the specified coords, with the given fuse time. The entity gets a random speed multiplied by the InitialVelocityCoeff, 1 being the default value." },
				TryGetHeight = { Params = "BlockX, BlockZ", Return = "IsValid, Height", Notes = "Returns true and height of the highest non-air block if the chunk is loaded, or false otherwise." },
				UnloadUnusedChunks = { Params = "", Return = "", Notes = "Unloads chunks that are no longer needed, and are saved. NOTE: This API is deprecated and will be removed soon." },
				UpdateSign = { Params = "X, Y, Z, Line1, Line2, Line3, Line4, [{{cPlayer|Player}}]", Return = "", Notes = "Sets the sign text at the specified coords. The sign-updating hooks are called for the change. The Player parameter is used to indicate the player from whom the change has come, it may be nil. Same as SetSignLiens()" },
				UseBlockEntity = { Params = "{{cPlayer|Player}}, BlockX, BlockY, BlockZ", Return = "", Notes = "Makes the specified Player use the block entity at the specified coords (open chest UI, etc.) If the cords are in an unloaded chunk or there's no block entity, ignores the call." },
				WakeUpSimulators = { Params = "BlockX, BlockY, BlockZ", Return = "", Notes = "Wakes up the simulators for the specified block." },
				WakeUpSimulatorsInArea = { Params = "MinBlockX, MaxBlockX, MinBlockY, MaxBlockY, MinBlockZ, MaxBlockZ", Return = "", Notes = "Wakes up the simulators for all the blocks in the specified area (edges inclusive)." },
			},
			AdditionalInfo =
			{
				{
					Header = "Using callbacks",
					Contents = [[
						To avoid problems with stale objects, the cWorld class will not let plugins get a direct pointer
						to an {{cEntity|entity}}, {{cBlockEntity|block entity}} or a {{cPlayer|player}}. Such an object
						could be modified or even destroyed by another thread while the plugin holds it, so it would be
						rather unsafe.</p>
						<p>
						Instead, the cWorld provides access to these objects using callbacks. The plugin provides a
						function that is called and receives the object as a parameter; cWorld guarantees that while
						the callback is executing, the object will stay valid. If a plugin needs to "remember" the
						object outside of the callback, it needs to store the entity ID, blockentity coords or player
						name.</p>
						<p>
						The following code examples show how to use the callbacks</p>
						<p>
						This code teleports player Player to another player named ToName in the same world:
<pre class="prettyprint lang-lua">
-- Player is a cPlayer object
-- ToName is a string
-- World is a cWorld object
World:ForEachPlayer(
	function (a_OtherPlayer)
	if (a_OtherPlayer:GetName() == ToName) then
		Player:TeleportToEntity(a_OtherPlayer);
	end
);
</pre></p>
						<p>
						This code fills each furnace in the chunk with 64 coals:
<pre class="prettyprint lang-lua">
-- Player is a cPlayer object
-- World is a cWorld object
World:ForEachFurnaceInChunk(Player:GetChunkX(), Player:GetChunkZ(),
	function (a_Furnace)
		a_Furnace:SetFuelSlot(cItem(E_ITEM_COAL, 64));
	end
);
</pre></p>
						<p>
						This code teleports all spiders up by 100 blocks:
<pre class="prettyprint lang-lua">
-- World is a cWorld object
World:ForEachEntity(
	function (a_Entity)
		if not(a_Entity:IsMob()) then
			return;
		end
		local Monster = tolua.cast(a_Entity, "cMonster");  -- Get the cMonster out of cEntity, now that we know the entity represents one.
		if (Monster:GetMobType() == cMonster.mtSpider) then
			Monster:TeleportToCoords(Monster:GetPosX(), Monster:GetPosY() + 100, Monster:GetPosZ());
		end
	end
);
</pre></p>
					]],
				},
			},  -- AdditionalInfo
		},  -- cWorld

		HTTPFormData =
		{
			Desc = "This class stores data for one form element for a {{HTTPRequest|HTTP request}}.",
			Variables =
			{
				Name = { Type = "string", Notes = "Name of the form element" },
				Type = { Type = "string", Notes = "Type of the data (usually empty)" },
				Value = { Type = "string", Notes = "Value of the form element. Contains the raw data as sent by the browser." },
			},
		},  -- HTTPFormData

		HTTPRequest =
		{
			Desc = [[
				This class encapsulates all the data that is sent to the WebAdmin through one HTTP request. Plugins
				receive this class as a parameter to the function handling the web requests, as registered in the
				FIXME: {{cPluginLua}}:AddWebPage().
			]],
			Constants =
			{
				FormData = { Notes = "Array-table of {{HTTPFormData}}, contains the values of individual form elements submitted by the client" },
				Params = { Notes = "Map-table of parameters given to the request in the URL (?param=value); if a form uses GET method, this is the same as FormData. For each parameter given as \"param=value\", there is an entry in the table with \"param\" as its key and \"value\" as its value." },
				PostParams = { Notes = "Map-table of data posted through a FORM - either a GET or POST method. Logically the same as FormData, but in a map-table format (for each parameter given as \"param=value\", there is an entry in the table with \"param\" as its key and \"value\" as its value)." },
			},

			Variables =
			{
				Method = { Type = "string", Notes = "The HTTP method used to make the request. Usually GET or POST." },
				Path = { Type = "string", Notes = "The Path part of the URL (excluding the parameters)" },
				Username = { Type = "string", Notes = "Name of the logged-in user." },
			},
		},  -- HTTPRequest

		ItemCategory =
		{
			Desc = [[
				This class contains static functions for determining item categories. All of the functions are
				called directly on the class table, unlike most other object, which require an instance first.
			]],
			Functions =
			{
				IsArmor      = { Params = "ItemType", Return = "bool", Notes = "(STATIC) Returns true if the specified item type is any kind of an armor." },
				IsAxe        = { Params = "ItemType", Return = "bool", Notes = "(STATIC) Returns true if the specified item type is any kind of an axe." },
				IsBoots      = { Params = "ItemType", Return = "bool", Notes = "(STATIC) Returns true if the specified item type is any kind of boots." },
				IsChestPlate = { Params = "ItemType", Return = "bool", Notes = "(STATIC) Returns true if the specified item type is any kind of a chestplate." },
				IsHelmet     = { Params = "ItemType", Return = "bool", Notes = "(STATIC) Returns true if the specified item type is any kind of a helmet." },
				IsHoe        = { Params = "ItemType", Return = "bool", Notes = "(STATIC) Returns true if the specified item type is any kind of a hoe." },
				IsLeggings   = { Params = "ItemType", Return = "bool", Notes = "(STATIC) Returns true if the specified item type is any kind of a leggings." },
				IsPickaxe    = { Params = "ItemType", Return = "bool", Notes = "(STATIC) Returns true if the specified item type is any kind of a pickaxe." },
				IsShovel     = { Params = "ItemType", Return = "bool", Notes = "(STATIC) Returns true if the specified item type is any kind of a shovel." },
				IsSword      = { Params = "ItemType", Return = "bool", Notes = "(STATIC) Returns true if the specified item type is any kind of a sword." },
				IsTool       = { Params = "ItemType", Return = "bool", Notes = "(STATIC) Returns true if the specified item type is any kind of a tool (axe, hoe, pickaxe, shovel or FIXME: sword)" },
			},
			AdditionalInfo =
			{
				{
					Header = "Code example",
					Contents = [[
						The following code snippet checks if the player holds a shovel.
<pre class="prettyprint lang-lua">
-- a_Player is a {{cPlayer}} object, possibly received as a hook param
local HeldItem = a_Player:GetEquippedItem();
if (cItemCategory:IsShovel(HeldItem.m_ItemType)) then
	-- It's a shovel
end
</pre>
					]],
				}
			},
		},  -- ItemCategory
		
		lxp =
		{
			Desc = [[
				This class provides an interface to the XML parser,
				{{http://matthewwild.co.uk/projects/luaexpat/|LuaExpat}}. It provides a SAX interface with an
				incremental XML parser.</p>
				<p>
				With an event-based API like SAX the XML document can be fed to the parser in chunks, and the
				parsing begins as soon as the parser receives the first document chunk. LuaExpat reports parsing
				events (such as the start and end of elements) directly to the application through callbacks. The
				parsing of huge documents can benefit from this piecemeal operation.</p>
				<p>
				See the online
				{{http://matthewwild.co.uk/projects/luaexpat/manual.html#parser|LuaExpat documentation}} for details
				on how to work with this parser. The code examples below should provide some basic help, too.
			]],
			Functions =
			{
				new = {Params = "CallbacksTable, [SeparatorChar]", Return = "XMLParser object", Notes = "Creates a new XML parser object, with the specified callbacks table and optional separator character."},
			},
			Constants =
			{
				_COPYRIGHT = { Notes = "" },
				_DESCRIPTION = { Notes = "" },
				_VERSION = { Notes = "" },
			},
			AdditionalInfo =
			{
				{
					Header = "Parser callbacks",
					Contents = [[
						The callbacks table passed to the new() function specifies the Lua functions that the parser
						calls upon various events. The following table lists the most common functions used, for a
						complete list see the online
						{{http://matthewwild.co.uk/projects/luaexpat/manual.html#parser|LuaExpat documentation}}.</p>
						<table>
						<tr><th>Function name</th><th>Parameters</th><th>Notes</th></tr>
						<tr><td>CharacterData</td><td>Parser, string</td><td>Called when the parser recognizes a raw string inside the element</td></tr>
						<tr><td>EndElement</td><td>Parser, ElementName</td><td>Called when the parser detects the ending of an XML element</td></tr>
						<tr><td>StartElement</td><td>Parser, ElementName, AttributesTable</td><td>Called when the parser detects the start of an XML element. The AttributesTable is a Lua table containing all the element's attributes, both in the array section (in the order received) and in the dictionary section.</td></tr>
						</table>
					]],
				},
				{
					Header = "XMLParser object",
					Contents = [[
						The XMLParser object returned by lxp.new provides the functions needed to parse the XML. The
						following list provides the most commonly used ones, for a complete list see the online
						{{http://matthewwild.co.uk/projects/luaexpat/manual.html#parser|LuaExpat documentation}}.
						<ul>
							<li>close() - closes the parser, freeing all memory used by it.</li>
							<li>getCallbacks() - returns the callbacks table for this parser.</li>
							<li>parse(string) - parses more document data. the string contains the next part (or possibly all) of the document. Returns non-nil for success or nil, msg, line, col, pos for error.</li>
							<li>stop() - aborts parsing (can be called from within the parser callbacks).</li>
						</ul>
					]],
				},
				{
					Header = "Code example",
					Contents = [[
						The following code reads an entire XML file and outputs its logical structure into the console:
<pre class="prettyprint lang-lua">
local Depth = 0;

-- Define the callbacks:
local Callbacks = {
	CharacterData = function(a_Parser, a_String)
		LOG(string.rep(" ", Depth) .. "* " .. a_String);
	end
	
	EndElement = function(a_Parser, a_ElementName)
		Depth = Depth - 1;
		LOG(string.rep(" ", Depth) .. "- " .. a_ElementName);
	end

	StartElement = function(a_Parser, a_ElementName, a_Attribs)
		LOG(string.rep(" ", Depth) .. "+ " .. a_ElementName);
		Depth = Depth + 1;
	end
}

-- Create the parser:
local Parser = lxp.new(Callbacks);

-- Parse the XML file:
local f = io.open("file.xml", "rb");
while (true) do
	local block = f:read(128 * 1024);  -- Use a 128KiB buffer for reading
	if (block == nil) then
		-- End of file
		break;
	end
	Parser:parse(block);
end

-- Signalize to the parser that no more data is coming
Parser:parse();

-- Close the parser:
Parser:close();
</pre>
					]],
				},
			},  -- AdditionalInfo
		},  -- lxp
		
		sqlite3 =
		{
			Desc = [[
			]],
			
			Functions =
			{
				complete = { Params = "string", Return = "bool", Notes = "Returns true if the string sql comprises one or more complete SQL statements and false otherwise." },
				open = { Params = "FileName", Return = "DBClass", Notes = [[
					Opens (or creates if it does not exist) an SQLite database with name filename and returns its
					handle as userdata (the returned object should be used for all further method calls in connection
					with this specific database, see
					{{http://lua.sqlite.org/index.cgi/doc/tip/doc/lsqlite3.wiki#database_methods|Database methods}}).
					Example:
<pre class="prettyprint lang-lua">
-- open the database:
myDB = sqlite3.open('MyDatabaseFile.sqlite3')

-- do some database calls...

-- Close the database:
myDB:close()
</pre>
				]], },
				open_memory = { Return = "DBClass", Notes = "Opens an SQLite database in memory and returns its handle as userdata. In case of an error, the function returns nil, an error code and an error message. (In-memory databases are volatile as they are never stored on disk.)" },
				version = { Return = "string", Notes = "Returns a string with SQLite version information, in the form 'x.y[.z]'." },
			},
		},
		
		TakeDamageInfo =
		{
			Desc = [[
				This class contains the amount of damage, and the entity that caused the damage. It is used in the
				{{OnTakeDamage|HOOK_TAKE_DAMAGE}} hook and in the {{cEntity}}'s TakeDamage() function.
			]],
			Variables =
			{
				Attacker = { Type = "{{cEntity}}", Notes = "The entity who is attacking. Only valid if dtAttack." },
				DamageType = { Type = "eDamageType", Notes = "Source of the damage. One of the dtXXX constants." },
				FinalDamage = { Type = "number", Notes = "	The final amount of damage that will be applied to the Receiver. It is the RawDamage minus any Receiver's armor-protection " },
				Knockback = { Type = "{{Vector3d}}", Notes = "Vector specifying the amount and direction of knockback that will be applied to the Receiver " },
				RawDamage = { Type = "number", Notes = "Amount of damage that the attack produces on the Receiver, including the Attacker's equipped weapon, but excluding the Receiver's armor." },
			},
			AdditionalInfo =
			{
				{
					Header = "",
					Contents = [[
						The TDI is passed as the second parameter in the HOOK_TAKE_DAMAGE hook, and can be used to
						modify the damage before it is applied to the receiver:
<pre class="prettyprint lang-lua">
function OnTakeDamage(Receiver, TDI)
	LOG("Damage: Raw ".. TDI.RawDamage .. ", Final:" .. TDI.FinalDamage);

	-- If the attacker is a spider, make it deal 999 points of damage (insta-death spiders):
	if ((TDI.Attacker ~= nil) and TDI.Attacker:IsA("cSpider")) then
		TDI.FinalDamage = 999;
	end
end
</pre>
					]],
				},
			},  -- AdditionalInfo
		},  -- TakeDamageInfo

		tolua =
		{
			Desc = [[
				This class represents the tolua bridge between the Lua API and MCServer. It supports some low
				level operations and queries on the objects. See also the tolua++'s documentation at
				{{http://www.codenix.com/~tolua/tolua++.html#utilities}}. Normally you shouldn't use any of these
				functions except for cast() and type()
			]],
			Functions =
			{
				cast = { Params = "Object, TypeStr", Return = "Object", Notes = "Casts the object to the specified type through the inheritance hierarchy." },
				getpeer = { Params = "", Return = "", Notes = "" },
				inherit = { Params = "", Return = "", Notes = "" },
				releaseownership = { Params = "", Return = "", Notes = "" },
				setpeer = { Params = "", Return = "", Notes = "" },
				takeownership = { Params = "", Return = "", Notes = "" },
				type = { Params = "Object", Return = "TypeStr", Notes = "Returns a string representing the type of the object. This works similar to Lua's built-in type() function, but recognizes the underlying C++ types, too." },
			},
			AdditionalInfo =
			{
				{
					Header = "Usage example",
					Contents =
					[[
						The tolua.cast() function is normally used to cast between related types. For example in the
						hook callbacks you often receive a generic {{cEntity}} object, when in fact you know that the
						object is a {{cMonster}}. You can cast the object to access its cMonster functions:
<pre class="prettyprint lang-lua">
function OnTakeDamage(a_ReceiverEntity, TDI)
	if (a_ReceiverEntity.IsMob()) then
		local Mob = tolua.cast(a_ReceiverEntity, "cMonster");  -- Cast a_ReceiverEntity into a {{cMonster}} instance
		if (Mob:GetMonsterType() == cMonster.mtSheep) then
			local Sheep = tolua.cast(Mob, "cSheep");  -- Cast Mob into a {{cSheep}} instance
			-- Do something sheep-specific
		end
	end
end
</pre>
					]],
				}
			}  -- AdditionalInfo
		},  -- tolua

		Vector3d =
		{
			Desc = [[
				A Vector3d object uses double precision floating point values to describe a point in 3D space.</p>
				<p>
				See also {{Vector3f}} for single-precision floating point 3D coords and {{Vector3i}} for integer
				3D coords.
			]],
			Functions =
			{
				constructor =
				{
					{ Params = "{{Vector3f}}", Return = "Vector3d", Notes = "Creates a new Vector3d object by copying the coords from the given Vector3f." },
					{ Params = "", Return = "Vector3d", Notes = "Creates a new Vector3d object with all its coords set to 0." },
					{ Params = "X, Y, Z", Return = "Vector3d", Notes = "Creates a new Vector3d object with its coords set to the specified values." },
				},
				operator_div = { Params = "number", Return = "Vector3d", Notes = "Returns a new Vector3d with each coord divided by the specified number." },
				operator_mul = { Params = "number", Return = "Vector3d", Notes = "Returns a new Vector3d with each coord multiplied." },
				operator_sub = { Params = "Vector3d", Return = "Vector3d", Notes = "Returns a new Vector3d containing the difference between this object and the specified vector." },
				operator_plus = {Params = "Vector3d", Return = "Vector3d", Notes = "Returns a new Vector3d containing the sum of this vector and the specified vector" },
				Cross = { Params = "Vector3d", Return = "Vector3d", Notes = "Returns a new Vector3d that is a {{http://en.wikipedia.org/wiki/Cross_product|cross product}} of this vector and the specified vector." },
				Dot = { Params = "Vector3d", Return = "number", Notes = "Returns the dot product of this vector and the specified vector." },
				Equals = { Params = "Vector3d", Return = "bool", Notes = "Returns true if this vector is exactly equal to the specified vector." },
				Length = { Params = "", Return = "number", Notes = "Returns the (euclidean) length of the vector." },
				LineCoeffToXYPlane = { Params = "Vector3d, Z", Return = "number", Notes = "Returns the coefficient for the line from the specified vector through this vector to reach the specified Z coord. The result satisfies the following equation: (this + Result * (Param - this)).z = Z. Returns the NO_INTERSECTION constant if there's no intersection." },
				LineCoeffToXZPlane = { Params = "Vector3d, Y", Return = "number", Notes = "Returns the coefficient for the line from the specified vector through this vector to reach the specified Y coord. The result satisfies the following equation: (this + Result * (Param - this)).y = Y. Returns the NO_INTERSECTION constant if there's no intersection." },
				LineCoeffToYZPlane = { Params = "Vector3d, X", Return = "number", Notes = "Returns the coefficient for the line from the specified vector through this vector to reach the specified X coord. The result satisfies the following equation: (this + Result * (Param - this)).x = X. Returns the NO_INTERSECTION constant if there's no intersection." },
				Normalize = { Params = "", Return = "", Notes = "Changes this vector so that it keeps current direction but is exactly 1 unit long. FIXME: Fails for a zero vector." },
				NormalizeCopy = { Params = "", Return = "Vector3d", Notes = "Returns a new vector that has the same directino as this but is exactly 1 unit long. FIXME: Fails for a zero vector." },
				Set = { Params = "X, Y, Z", Return = "", Notes = "Sets all the coords in this object." },
				SqrLength = { Params = "", Return = "number", Notes = "Returns the (euclidean) length of this vector, squared. This operation is slightly less computationally expensive than Length(), while it conserves some properties of Length(), such as comparison. " },
			},
			Constants =
			{
				EPS = { Notes = "The max difference between two coords for which the coords are assumed equal (in LineCoeffToXYPlane() et al)." },
				NO_INTERSECTION = { Notes = "Special return value for the LineCoeffToXYPlane() et al meaning that there's no intersectino with the plane." },
			},
			Variables =
			{
				x = { Type = "number", Notes = "The X coord of the vector." },
				y = { Type = "number", Notes = "The Y coord of the vector." },
				z = { Type = "number", Notes = "The Z coord of the vector." },
			},
		},  -- Vector3d

		Vector3f =
		{
			Desc = [[
				A Vector3f object uses floating point values to describe a point in space.</p>
				<p>
				See also {{Vector3d}} for double-precision floating point 3D coords and {{Vector3i}} for integer
				3D coords.
			]],
			Functions =
			{
				constructor =
				{
					{ Params = "", Return = "Vector3f", Notes = "Creates a new Vector3f object with zero coords" },
					{ Params = "x, y, z", Return = "Vector3f", Notes = "Creates a new Vector3f object with the specified coords" },
					{ Params = "Vector3f", Return = "Vector3f", Notes = "Creates a new Vector3f object as a copy of the specified vector" },
					{ Params = "{{Vector3d}}", Return = "Vector3f", Notes = "Creates a new Vector3f object as a copy of the specified {{Vector3d}}" },
					{ Params = "{{Vector3i}}", Return = "Vector3f", Notes = "Creates a new Vector3f object as a copy of the specified {{Vector3i}}" },
				},
				operator_mul =
				{
					{ Params = "number", Return = "Vector3f", Notes = "Returns a new Vector3f object that has each of its coords multiplied by the specified number" },
					{ Params = "Vector3f", Return = "Vector3f", Notes = "Returns a new Vector3f object that has each of its coords multiplied by the respective coord of the specified vector." },
				},
				operator_plus = { Params = "Vector3f", Return = "Vector3f", Notes = "Returns a new Vector3f object that holds the vector sum of this vector and the specified vector." },
				operator_sub = { Params = "Vector3f", Return = "Vector3f", Notes = "Returns a new Vector3f object that holds the vector differrence between this vector and the specified vector." },
				Cross = { Params = "Vector3f", Return = "Vector3f", Notes = "Returns a new Vector3f object that holds the cross product of this vector and the specified vector." },
				Dot = { Params = "Vector3f", Return = "number", Notes = "Returns the dot product of this vector and the specified vector." },
				Equals = { Params = "Vector3f", Return = "bool", Notes = "Returns true if the specified vector is exactly equal to this vector." },
				Length = { Params = "", Return = "number", Notes = "Returns the (euclidean) length of this vector" },
				Normalize = { Params = "", Return = "", Notes = "Normalizes this vector (makes it 1 unit long while keeping the direction). FIXME: Fails for zero vectors." },
				NormalizeCopy = { Params = "", Return = "Vector3f", Notes = "Returns a copy of this vector that is normalized (1 unit long while keeping the same direction). FIXME: Fails for zero vectors." },
				Set = { Params = "x, y, z", Return = "", Notes = "Sets all the coords of the vector at once." },
				SqrLength = { Params = "", Return = "number", Notes = "Returns the (euclidean) length of this vector, squared. This operation is slightly less computationally expensive than Length(), while it conserves some properties of Length(), such as comparison." },
			},
			Variables =
			{
				x = { Type = "number", Notes = "The X coord of the vector." },
				y = { Type = "number", Notes = "The Y coord of the vector." },
				z = { Type = "number", Notes = "The Z coord of the vector." },
			},
		},  -- Vector3f

		Vector3i =
		{
			Desc = [[
				A Vector3i object uses integer values to describe a point in space.</p>
				<p>
				See also {{Vector3d}} for double-precision floating point 3D coords and {{Vector3f}} for
				single-precision floating point 3D coords.
			]],
			Functions =
			{
				constructor =
				{
					{ Params = "", Return = "Vector3i", Notes = "Creates a new Vector3i object with zero coords." },
					{ Params = "x, y, z", Return = "Vector3i", Notes = "Creates a new Vector3i object with the specified coords." },
					{ Params = "{{Vector3d}}", Return = "Vector3i", Notes = "Creates a new Vector3i object with coords copied and floor()-ed from the specified {{Vector3d}}." },
				},
				Equals = { Params = "Vector3i", Return = "bool", Notes = "Returns true if this vector is exactly the same as the specified vector." },
				Length = { Params = "", Return = "number", Notes = "Returns the (euclidean) length of this vector." },
				Set = { Params = "x, y, z", Return = "", Notes = "Sets all the coords of the vector at once" },
				SqrLength = { Params = "", Return = "number", Notes = "Returns the (euclidean) length of this vector, squared. This operation is slightly less computationally expensive than Length(), while it conserves some properties of Length(), such as comparison." },
			},
			Variables =
			{
				x = { Type = "number", Notes = "The X coord of the vector." },
				y = { Type = "number", Notes = "The Y coord of the vector." },
				z = { Type = "number", Notes = "The Z coord of the vector." },
			},
		},  -- Vector3i
		
		Globals =
		{
			Desc = [[
				These functions are available directly, without a class instance. Any plugin cal call them at any
				time.
			]],
			Functions =
			{
				AddFaceDirection = {Params = "BlockX, BlockY, BlockZ, BlockFace, [IsInverse]", Return = "BlockX, BlockY, BlockZ", Notes = "Returns the coords of a block adjacent to the specified block through the specified {{Globals#BlockFace|face}}"},
				BlockStringToType = {Params = "BlockTypeString", Return = "BLOCKTYPE", Notes = "Returns the block type parsed from the given string"},
				ClickActionToString = {Params = "{{Globals#ClickAction|ClickAction}}", Return = "string", Notes = "Returns a string description of the ClickAction enumerated value"},
				DamageTypeToString = {Params = "{{Globals#DamageType|DamageType}}", Return = "string", Notes = "Converts the {{Globals#DamageType|DamageType}} enumerated value to a string representation "},
				EscapeString = {Params = "string", Return = "string", Notes = "Returns a copy of the string with all quotes and backslashes escaped by a backslash"},
				GetChar = {Params = "String, Pos", Return = "string", Notes = "Returns one character from the string, specified by position "},
				GetTime = {Return = "number", Notes = "Returns the current OS time, as a unix time stamp (number of seconds since Jan 1, 1970)"},
				IsValidBlock = {Params = "BlockType", Return = "bool", Notes = "Returns true if BlockType is a known block type"},
				IsValidItem = {Params = "ItemType", Return = "bool", Notes = "Returns true if ItemType is a known item type"},
				ItemToFullString = {Params = "{{cItem|cItem}}", Return = "string", Notes = "Returns the string representation of the item, in the format 'ItemTypeText:ItemDamage * Count'"},
				ItemToString = {Params = "{{cItem|cItem}}", Return = "string", Notes = "Returns the string representation of the item type"},
				ItemTypeToString = {Params = "ItemType", Return = "string", Notes = "Returns the string representation of ItemType "},
				LOG = {Params = "string", Notes = "Logs a text into the server console using 'normal' severity (gray text) "},
				LOGERROR = {Params = "string", Notes = "Logs a text into the server console using 'error' severity (black text on red background)"},
				LOGINFO = {Params = "string", Notes = "Logs a text into the server console using 'info' severity (yellow text)"},
				LOGWARN = {Params = "string", Notes = "Logs a text into the server console using 'warning' severity (red text); OBSOLETE, use LOGWARNING() instead"},
				LOGWARNING = {Params = "string", Notes = "Logs a text into the server console using 'warning' severity (red text)"},
				NoCaseCompare = {Params = "string, string", Return = "number", Notes = "Case-insensitive string comparison; returns 0 if the strings are the same"},
				ReplaceString = {Params = "full-string, to-be-replaced-string, to-replace-string", Notes = "Replaces *each* occurence of to-be-replaced-string in full-string with to-replace-string"},
				StringSplit = {Params = "string, SeperatorsString", Return = "array table of strings", Notes = "Seperates string into multiple by splitting every time any of the characters in SeperatorsString is encountered."},
				StringSplitAndTrim = {Params = "string, SeperatorsString", Return = "array table of strings", Notes = "Seperates string into multiple by splitting every time any of the characters in SeperatorsString is encountered. Each of the separate strings is trimmed (whitespace removed from the beginning and end of the string)"},
				StringToBiome = {Params = "string", Return = "{{Globals#BiomeTypes|BiomeType}}", Notes = "Converts a string representation to a {{Globals#BiomeTypes|BiomeType}} enumerated value"},
				StringToDamageType = {Params = "string", Return = "{{Globals#DamageType|DamageType}}", Notes = "Converts a string representation to a {{Globals#DamageType|DamageType}} enumerated value."},
				StringToDimension = {Params = "string", Return = "{{Globals#WorldDimension|Dimension}}", Notes = "Converts a string representation to a {{Globals#WorldDimension|Dimension}} enumerated value"},
				StringToItem = {Params = "string, {{cItem|cItem}}", Return = "bool", Notes = "Parses the given string and sets the item; returns true if successful"},
				StringToMobType = {Params = "string", Return = "{{cMonster#MobType|MobType}}", Notes = "Converts a string representation to a {{cMonster#MobType|MobType}} enumerated value"},
				StripColorCodes = {Params = "string", Return = "string", Notes = "Removes all control codes used by MC for colors and styles"},
				TrimString = {Params = "string", Return = "string", Notes = "Trims whitespace at both ends of the string"},
				md5 = {Params = "string", Return = "string", Notes = "converts a string to an md5 hash"},
			},
			ConstantGroups =
			{
				BlockTypes =
				{
					Include = "^E_BLOCK_.*",
					TextBefore = [[
						These constants are used for block types. They correspond directly with MineCraft's data values
						for blocks.
					]],
				},
				ItemTypes =
				{
					Include = "^E_ITEM_.*",
					TextBefore = [[
						These constants are used for item types. They correspond directly with MineCraft's data values
						for items.
					]],
				},
				MetaValues =
				{
					Include = "^E_META_.*",
				},
				BiomeTypes =
				{
					Include = "^bi.*",
					TextBefore = [[
						These constants represent the biomes that the server understands. Note that there is a global
						StringToBiome() function that can convert a string into one of these constants.
					]],
				},
				BlockFaces =
				{
					Include = "^BLOCK_FACE_.*",
					TextBefore = [[
						These constants are used to describe individual faces of the block. They are used when the
						client is interacting with a block, or when the {{cLineBlockTracer}} hits a block, etc.
					]],
				},
				ClickAction =
				{
					Include = "^ca.*",
					TextBefore = [[
						These constants are used to signalize various interactions that the user can do with the
						{{cWindow|UI windows}}. The server translates the protocol events into these constants. Note
						that there is a global ClickActionToString() function that can translate these constants into
						their textual representation.
					]],
				},
				WorldDimension =
				{
					Include = "^dim.*",
					TextBefore = [[
						These constants represent dimension of a world. In MCServer, the dimension is only reflected in
						the world's overall tint - overworld gets sky-like colors and dark shades, the nether gets
						reddish haze and the end gets dark haze. World generator is not directly affected by the
						dimension, same as fluid simulators; those only default to the expected values if not set
						specifically otherwise in the world.ini file.
					]],
				},
				DamageType =
				{
					Include = "^dt.*",
					TextBefore = [[
						These constants are used for specifying the cause of damage to entities. They are used in the
						{{TakeDamageInfo}} structure, as well as in {{cEntity}}'s damage-related API functions.
					]],
				},
				GameMode =
				{
					Include = { "^gm.*", "^eGameMode_.*" },
					TextBefore = [[
						The following constants are used for the gamemode - survival, creative or adventure. Use the
						gmXXX constants, the eGameMode_ constants are deprecated and will be removed from the API.
					]],
				},
				Weather =
				{
					Include = { "^eWeather_.*", "wSunny", "wRain", "wStorm", "wThunderstorm" },
					TextBefore = [[
						These constants represent the weather in the world. Note that unlike vanilla, MCServer allows
						different weathers even in non-overworld {{Globals#WorldDimension|dimensions}}.
					]],
				},
				ExplosionSource =
				{
					Include = "^es.*",
					TextBefore = [[
						These constants are used to differentiate the various sources of explosions. They are used in
						the {{OnExploded|HOOK_EXPLODED}} hook, {{OnExploding|HOOK_EXPLODING}} hook and in the
						{{cWorld}}:DoExplosionAt() function. These constants also dictate the type of the additional
						data provided with the explosions, such as the exploding {{cCreeper|creeper}} entity or the
						{{Vector3i|coords}} of the exploding bed.
					]],
				}
			},
		},  -- Globals
	},


	IgnoreClasses =
	{
		"coroutine",
		"debug",
		"io",
		"math",
		"package",
		"os",
		"string",
		"table",
		"g_Stats",
		"g_TrackedPages",
	},

	IgnoreFunctions =
	{
		"Globals.assert",
		"Globals.collectgarbage",
		"Globals.xpcall",
		"Globals.decoda_output",  -- When running under Decoda, this function gets added to the global namespace
		"sqlite3.__newindex",
		"%a+\.__%a+",        -- AnyClass.__Anything
		"%a+\.\.collector",  -- AnyClass..collector
		"%a+\.new",          -- AnyClass.new
		"%a+.new_local",     -- AnyClass.new_local
		"%a+.delete",        -- AnyClass.delete

		-- Functions global in the APIDump plugin:
		"CreateAPITables",
		"DumpAPIHtml",
		"DumpAPITxt",
		"Initialize",
		"LinkifyString",
		"ListMissingPages",
		"ListUndocumentedObjects",
		"ListUnexportedObjects",
		"LoadAPIFiles",
		"ReadDescriptions",
		"ReadHooks",
		"WriteHtmlClass",
		"WriteHtmlHook",
		"WriteStats",
	},
	
	IgnoreConstants =
	{
		"cChestEntity.__cBlockEntityWindowOwner__",
		"cDropSpenserEntity.__cBlockEntityWindowOwner__",
		"cFurnaceEntity.__cBlockEntityWindowOwner__",
		"cHopperEntity.__cBlockEntityWindowOwner__",
		"cLuaWindow.__cItemGrid__cListener__",
	},
	
	IgnoreVariables =
	{
		"__.*__",  -- tolua exports multiple inheritance this way
	} ,
	
	ExtraPages =
	{
		-- No sorting is provided for these, they will be output in the same order as defined here
		{ FileName = "Writing-a-MCServer-plugin.html", Title = "Writing a MCServer plugin" },
		{ FileName = "SettingUpDecoda.html", Title = "Setting up the Decoda Lua IDE" },
		{ FileName = "WebWorldThreads.html", Title = "Webserver vs World threads" },
	}
} ;




