return
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
				FunctionName =
				{
					{
						Params =
						{
							{ Name = "BuiltInType", Type = "number"},
							{ Name = "ClassEnum", Type = "cClass#eEnum"},
							{ Name = "GlobalEnum", Type = "eEnum"},
						},
						Returns =
						{
							{ Type = "number" },
							{ Type = "self" },  -- Returns the same object on which it was called
						},
						Notes = "Notes 1"
					},
					{
						Params = {...},
						Returns = {...},
						Notes = "Notes 2",
					},
				},
			} ,

			Constants =
			{
				ConstantName = { Notes = "Notes about the constant" },
			} ,

			ConstantGroups =
			{
				eEnum =  -- also used as the HTML anchor name
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
		--]]

		cBlockInfo =
		{
			Desc = [[
				This class is used to query block properties.
			]],
			Functions =
			{
				CanBeTerraformed =
				{
					IsStatic = true,
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
							Type = "boolean",
						},
					},
					Notes = "Returns true if the block is suitable to be changed by a generator",
				},
				FullyOccupiesVoxel =
				{
					IsStatic = true,
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
							Type = "boolean",
						},
					},
					Notes = "Returns whether the specified block fully occupies its voxel.",
				},
				Get =
				{
					IsStatic = true,
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
							Type = "cBlockInfo",
						},
					},
					Notes = "Returns the {{cBlockInfo}} structure for the specified block type. <b>OBSOLETE</b>, use static functions instead",
				},
				GetHardness =
				{
					IsStatic = true,
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
					Notes = "Returns the block's hardness. The bigger the harder the block.",
				},
				GetBlockHeight =
				{
					IsStatic = true,
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
					Notes = "Returns the block's hitbox height.",
				},
				GetLightValue =
				{
					IsStatic = true,
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
					Notes = "Returns how much light the specified block emits on its own.",
				},
				GetPlaceSound =
				{
					IsStatic = true,
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
							Type = "string",
						},
					},
					Notes = "(<b>DEPRECATED</b>) Not used by cuberite internally and always returns an empty string.",
				},
				GetSpreadLightFalloff =
				{
					IsStatic = true,
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
					Notes = "Returns how much light the specified block type consumes.",
				},
				IsOneHitDig =
				{
					IsStatic = true,
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
							Type = "boolean",
						},
					},
					Notes = "Returns true if the specified block type will be destroyed after a single hit.",
				},
				IsPistonBreakable =
				{
					IsStatic = true,
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
							Type = "boolean",
						},
					},
					Notes = "Returns true if a piston can break the specified block type.",
				},
				IsRainBlocker =
				{
					IsStatic = true,
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
							Type = "boolean",
						},
					},
					Notes = "Returns true if the specified block type blocks rain from passing through.",
				},
				IsSkylightDispersant =
				{
					IsStatic = true,
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
							Type = "boolean",
						},
					},
					Notes = "Returns true if skylight is impeded by passage through a block of the specified type.",
				},
				IsSnowable =
				{
					IsStatic = true,
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
							Type = "boolean",
						},
					},
					Notes = "Returns whether the specified block type can hold snow atop.",
				},
				IsSolid =
				{
					IsStatic = true,
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
							Type = "boolean",
						},
					},
					Notes = "Returns whether the specified block type is solid.",
				},
				IsTransparent =
				{
					IsStatic = true,
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
							Type = "boolean",
						},
					},
					Notes = "Returns whether the specified block is transparent.",
				},
				IsUseableBySpectator =
				{
					IsStatic = true,
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
							Type = "boolean",
						},
					},
					Notes = "Returns whether a spectator can interact with the specified block.",
				},
			},
			Variables =
			{
				m_BlockHeight =
				{
					Type = "number",
					Notes = "The height of the block, a value between 0.0 and 1.0. <b>OBSOLETE</b>, use cBlockInfo:GetBlockHeight() instead.",
				},
				m_CanBeTerraformed =
				{
					Type = "bool",
					Notes = "Is this block suited to be terraformed? <b>OBSOLETE</b>, use cBlockInfo:CanBeTerraformed() instead.",
				},
				m_FullyOccupiesVoxel =
				{
					Type = "bool",
					Notes = "Does this block fully occupy its voxel - is it a 'full' block? <b>OBSOLETE</b>, use cBlockInfo:FullyOccupiesVoxel() instead.",
				},
				m_Hardness =
				{
					Type = "number",
					Notes = "The greater the value the longer the player needs to break the block. <b>OBSOLETE</b>, use cBlockInfo:GetHardness() instead.",
				},
				m_IsSnowable =
				{
					Type = "bool",
					Notes = "Can this block hold snow atop? <b>OBSOLETE</b>, use cBlockInfo:IsSnowable() instead",
				},
				m_IsSolid =
				{
					Type = "bool",
					Notes = "Is this block solid (player cannot walk through)? <b>OBSOLETE</b>, use cBlockInfo:IsSolid() instead.",
				},
				m_LightValue =
				{
					Type = "number",
					Notes = "How much light do the blocks emit on their own? <b>OBSOLETE</b>, use cBlockInfo:GetLightValue() instead.",
				},
				m_OneHitDig =
				{
					Type = "bool",
					Notes = "Is a block destroyed after a single hit? <b>OBSOLETE</b>, use cBlockInfo:IsOneHitDig() instead.",
				},
				m_PistonBreakable =
				{
					Type = "bool",
					Notes = "Can a piston break this block? <b>OBSOLETE</b>, use cBlockInfo:IsPistonBreakable instead.",
				},
				m_SpreadLightFalloff =
				{
					Type = "number",
					Notes = "How much light do the blocks consume? <b>OBSOLETE</b>, use cBlockInfo:GetSpreadLightFalloff() instead.",
				},
				m_Transparent =
				{
					Type = "bool",
					Notes = "Is a block completely transparent? (light doesn't get decreased(?)). <b>OBSOLETE</b>, use cBlockInfo:IsTransparent() instead.",
				},
			},
		},
		cChatColor =
		{
			Desc = [[
				A wrapper class for constants representing colors or effects.
			]],
			Functions =
			{

			},
			Constants =
			{
				Black =
				{
					Notes = "",
				},
				Blue =
				{
					Notes = "",
				},
				Bold =
				{
					Notes = "",
				},
				Color =
				{
					Notes = "The first character of the color-code-sequence, §",
				},
				DarkPurple =
				{
					Notes = "",
				},
				Delimiter =
				{
					Notes = "The first character of the color-code-sequence, §",
				},
				Gold =
				{
					Notes = "",
				},
				Gray =
				{
					Notes = "",
				},
				Green =
				{
					Notes = "",
				},
				Italic =
				{
					Notes = "",
				},
				LightBlue =
				{
					Notes = "",
				},
				LightGray =
				{
					Notes = "",
				},
				LightGreen =
				{
					Notes = "",
				},
				LightPurple =
				{
					Notes = "",
				},
				Navy =
				{
					Notes = "",
				},
				Plain =
				{
					Notes = "Resets all formatting to normal",
				},
				Purple =
				{
					Notes = "",
				},
				Random =
				{
					Notes = "Random letters and symbols animate instead of the text",
				},
				Red =
				{
					Notes = "",
				},
				Rose =
				{
					Notes = "",
				},
				Strikethrough =
				{
					Notes = "",
				},
				Underlined =
				{
					Notes = "",
				},
				White =
				{
					Notes = "",
				},
				Yellow =
				{
					Notes = "",
				},
			},
		},
		cChunkDesc =
		{
			Desc = [[
				The cChunkDesc class is a container for chunk data while the chunk is being generated. As such, it is
				only used as a parameter for the {{OnChunkGenerating|OnChunkGenerating}} and
				{{OnChunkGenerated|OnChunkGenerated}} hooks and cannot be constructed on its own. Plugins can use this
				class in both those hooks to manipulate generated chunks.
				Calls to any setter of this class will not trigger simulator updates (lava, water, redstone).
			]],
			Functions =
			{
				FillBlocks =
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
					Notes = "Fills the entire chunk with the specified blocks",
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
								Name = "BlockType",
								Type = "number",
							},
							{
								Name = "BlockMeta",
								Type = "number",
							},
						},
						Notes = "Fills the cuboid, specified in relative coords, by the specified block type and block meta. The cuboid may reach outside of the chunk, only the part intersecting with this chunk is filled.",
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
								Name = "BlockType",
								Type = "number",
							},
							{
								Name = "BlockMeta",
								Type = "number",
							},
						},
						Notes = "Fills the cuboid, specified in relative coords, by the specified block type and block meta. The cuboid may reach outside of the chunk, only the part intersecting with this chunk is filled.",
					},
				},
				FloorRelCuboid =
				{
					{
						Params =
						{
							{
								Name = "RelCuboid",
								Type = "cCuboid",
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
						Notes = "Fills those blocks of the cuboid (specified in relative coords) that are considered non-floor (air, water) with the specified block type and meta. Cuboid may reach outside the chunk, only the part intersecting with this chunk is filled.",
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
								Name = "BlockType",
								Type = "number",
							},
							{
								Name = "BlockMeta",
								Type = "number",
							},
						},
						Notes = "Fills those blocks of the cuboid (specified in relative coords) that are considered non-floor (air, water) with the specified block type and meta. Cuboid may reach outside the chunk, only the part intersecting with this chunk is filled.",
					},
				},
				GetBiome =
				{
					Params =
					{
						{
							Name = "RelX",
							Type = "number",
						},
						{
							Name = "RelZ",
							Type = "number",
						},
					},
					Returns =
					{
						{
							Type = "EMCSBiome",
						},
					},
					Notes = "Returns the biome at the specified relative coords",
				},
				GetBlockEntity =
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
					},
					Returns =
					{
						{
							Type = "cBlockEntity",
						},
					},
					Notes = "Returns the block entity for the block at the specified coords. Creates it if it doesn't exist. Returns nil if the block has no block entity capability.",
				},
				GetBlockMeta =
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
				GetBlockType =
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
				GetBlockTypeMeta =
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
				GetChunkX =
				{
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the X coord of the chunk contained.",
				},
				GetChunkZ =
				{
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the Z coord of the chunk contained.",
				},
				GetHeight =
				{
					Params =
					{
						{
							Name = "RelX",
							Type = "number",
						},
						{
							Name = "RelZ",
							Type = "number",
						},
					},
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the height at the specified relative coords",
				},
				GetMaxHeight =
				{
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the maximum height contained in the heightmap.",
				},
				GetMinHeight =
				{
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the minimum height value in the heightmap.",
				},
				IsUsingDefaultBiomes =
				{
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Returns true if the chunk is set to use default biome generator",
				},
				IsUsingDefaultComposition =
				{
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Returns true if the chunk is set to use default composition generator",
				},
				IsUsingDefaultFinish =
				{
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Returns true if the chunk is set to use default finishers",
				},
				IsUsingDefaultHeight =
				{
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Returns true if the chunk is set to use default height generator",
				},
				IsUsingDefaultStructures =
				{
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Returns true if the chunk is set to use default structures",
				},
				RandomFillRelCuboid =
				{
					{
						Params =
						{
							{
								Name = "RelCuboid",
								Type = "cCuboid",
							},
							{
								Name = "BlockType",
								Type = "number",
							},
							{
								Name = "BlockMeta",
								Type = "number",
							},
							{
								Name = "RandomSeed",
								Type = "number",
							},
							{
								Name = "ChanceOutOf10k",
								Type = "number",
							},
						},
						Notes = "Fills the specified relative cuboid with block type and meta in random locations. RandomSeed is used for the random number genertion (same seed produces same results); ChanceOutOf10k specifies the density (how many out of every 10000 blocks should be filled). Cuboid may reach outside the chunk, only the part intersecting with this chunk is filled.",
					},
					{
						Params =
						{
							{
								Name = "MinRelX",
								Type = "number",
							},
							{
								Name = "ChanceOutOf10k",
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
								Name = "BlockType",
								Type = "number",
							},
							{
								Name = "BlockMeta",
								Type = "number",
							},
							{
								Name = "RandomSeed",
								Type = "number",
							},
						},
						Notes = "Fills the specified relative cuboid with block type and meta in random locations. RandomSeed is used for the random number genertion (same seed produces same results); ChanceOutOf10k specifies the density (how many out of every 10000 blocks should be filled). Cuboid may reach outside the chunk, only the part intersecting with this chunk is filled.",
					},
				},
				ReadBlockArea =
				{
					Params =
					{
						{
							Name = "BlockArea",
							Type = "cBlockArea",
						},
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
					},
					Notes = "Reads data from the chunk into the block area object. Block types and metas are processed.",
				},
				ReplaceRelCuboid =
				{
					{
						Params =
						{
							{
								Name = "RelCuboid",
								Type = "cCuboid",
							},
							{
								Name = "SrcBlockType",
								Type = "number",
							},
							{
								Name = "SrcBlockMeta",
								Type = "number",
							},
							{
								Name = "DstBlockType",
								Type = "number",
							},
							{
								Name = "DstBlockMeta",
								Type = "number",
							},
						},
						Notes = "Replaces all SrcBlockType + SrcBlockMeta blocks in the cuboid (specified in relative coords) with DstBlockType + DstBlockMeta blocks. Cuboid may reach outside the chunk, only the part intersecting with this chunk is filled.",
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
								Name = "SrcBlockType",
								Type = "number",
							},
							{
								Name = "SrcBlockMeta",
								Type = "number",
							},
							{
								Name = "DstBlockType",
								Type = "number",
							},
							{
								Name = "DstBlockMeta",
								Type = "number",
							},
						},
						Notes = "Replaces all SrcBlockType + SrcBlockMeta blocks in the cuboid (specified in relative coords) with DstBlockType + DstBlockMeta blocks. Cuboid may reach outside the chunk, only the part intersecting with this chunk is filled.",
					},
				},
				SetBiome =
				{
					Params =
					{
						{
							Name = "RelX",
							Type = "number",
						},
						{
							Name = "RelZ",
							Type = "number",
						},
						{
							Name = "Biome",
							Type = "EMCSBiome",
						},
					},
					Notes = "Sets the biome at the specified relative coords.",
				},
				SetBlockMeta =
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
							Name = "BlockMeta",
							Type = "number",
						},
					},
					Notes = "Sets the block meta at the specified relative coords.",
				},
				SetBlockType =
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
							Name = "BlockType",
							Type = "number",
						},
					},
					Notes = "Sets the block type at the specified relative coords",
				},
				SetBlockTypeMeta =
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
				SetHeight =
				{
					Params =
					{
						{
							Name = "RelX",
							Type = "number",
						},
						{
							Name = "RelZ",
							Type = "number",
						},
						{
							Name = "Height",
							Type = "number",
						},
					},
					Notes = "Sets the height at the specified relative coords",
				},
				SetUseDefaultBiomes =
				{
					Params =
					{
						{
							Name = "ShouldUseDefaultBiomes",
							Type = "boolean",
						},
					},
					Notes = "Sets the chunk to use default biome generator or not",
				},
				SetUseDefaultComposition =
				{
					Params =
					{
						{
							Name = "ShouldUseDefaultComposition",
							Type = "boolean",
						},
					},
					Notes = "Sets the chunk to use default composition generator or not",
				},
				SetUseDefaultFinish =
				{
					Params =
					{
						{
							Name = "ShouldUseDefaultFinish",
							Type = "boolean",
						},
					},
					Notes = "Sets the chunk to use default finishers or not",
				},
				SetUseDefaultHeight =
				{
					Params =
					{
						{
							Name = "ShouldUseDefaultHeight",
							Type = "boolean",
						},
					},
					Notes = "Sets the chunk to use default height generator or not",
				},
				SetUseDefaultStructures =
				{
					Params =
					{
						{
							Name = "ShouldUseDefaultStructures",
							Type = "boolean",
						},
					},
					Notes = "Sets the chunk to use default structures or not",
				},
				UpdateHeightmap =
				{
					Notes = "Updates the heightmap to match current contents. The plugins should do that if they modify the contents and don't modify the heightmap accordingly; Cuberite expects (and checks in Debug mode) that the heightmap matches the contents when the cChunkDesc is returned from a plugin.",
				},
				WriteBlockArea =
				{
					Params =
					{
						{
							Name = "BlockArea",
							Type = "cBlockArea",
						},
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
							Name = "MergeStrategy",
							Type = "cBlockArea",
							IsOptional = true,
						},
					},
					Notes = "Writes data from the block area into the chunk",
				},
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
			},
		},
		cClientHandle =
		{
			Desc = [[
				A cClientHandle represents the technical aspect of a connected player - their game client
				connection. Internally, it handles all the incoming and outgoing packets, the chunks that are to be
				sent to the client, ping times etc.
			]],
			Functions =
			{
				GenerateOfflineUUID =
				{
					IsStatic = true,
					Params =
					{
						{
							Name = "Username",
							Type = "string",
						},
					},
					Returns =
					{
						{
							Type = "string",
						},
					},
					Notes = "Generates an UUID based on the player name provided. This is used for the offline (non-auth) mode, when there's no UUID source. Each username generates a unique and constant UUID, so that when the player reconnects with the same name, their UUID is the same. Returns a 32-char UUID (no dashes).",
				},
				GetClientBrand =
				{
					Returns =
					{
						{
							Type = "string",
						},
					},
					Notes = "Returns the brand that the client has sent in their MC|Brand plugin message.",
				},
				GetForgeMods =
				{
					Returns =
					{
						{
							Type = "table",
						},
					},
					Notes = "Returns the Forge mods installed on the client.",
				},
				GetIPString =
				{
					Returns =
					{
						{
							Type = "string",
						},
					},
					Notes = "Returns the IP address of the connection, as a string. Only the address part is returned, without the port number.",
				},
				GetLocale =
				{
					Returns =
					{
						{
							Name = "Locale",
							Type = "string",
						},
					},
					Notes = "Returns the locale string that the client sends as part of the protocol handshake. Can be used to provide localized strings.",
				},
				GetPing =
				{
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the ping time, in ms",
				},
				GetPlayer =
				{
					Returns =
					{
						{
							Type = "cPlayer",
						},
					},
					Notes = "Returns the player object connected to this client. Note that this may be nil, for example if the player object is not yet spawned.",
				},
				GetProtocolVersion =
				{
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the protocol version number of the protocol that the client is talking. Returns zero if the protocol version is not (yet) known.",
				},
				GetRequestedViewDistance =
				{
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the view distance that the player request, not the used view distance.",
				},
				GetUniqueID =
				{
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the UniqueID of the client used to identify the client in the server",
				},
				GetUsername =
				{
					Returns =
					{
						{
							Type = "string",
						},
					},
					Notes = "Returns the username that the client has provided",
				},
				GetUUID =
				{
					Returns =
					{
						{
							Type = "string",
						},
					},
					Notes = "Returns the authentication-based UUID of the client. This UUID should be used to identify the player when persisting any player-related data. Returns a 32-char UUID (no dashes)",
				},
				GetViewDistance =
				{
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the viewdistance (number of chunks loaded for the player in each direction)",
				},
				HasPluginChannel =
				{
					Params =
					{
						{
							Name = "ChannelName",
							Type = "string",
						},
					},
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Returns true if the client has registered to receive messages on the specified plugin channel.",
				},
				IsForgeClient =
				{
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Returns true if the client is modded with Forge.",
				},
				IsUUIDOnline =
				{
					IsStatic = true,
					Params =
					{
						{
							Name = "UUID",
							Type = "cUUID",
						},
					},
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Returns true if the UUID is generated by online auth, false if it is an offline-generated UUID. We use Version-3 UUIDs for offline UUIDs, online UUIDs are Version-4, thus we can tell them apart. Accepts both 32-char and 36-char UUIDs (with and without dashes).",
				},
				Kick =
				{
					Params =
					{
						{
							Name = "Reason",
							Type = "string",
						},
					},
					Notes = "Kicks the user with the specified reason",
				},
				SendBlockChange =
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
					Notes = "Sends a BlockChange packet to the client. This can be used to create fake blocks only for that player.",
				},
				SendEntityAnimation =
				{
					Params =
					{
						{
							Name = "Entity",
							Type = "cEntity",
						},
						{
							Name = "AnimationNumber",
							Type = "number",
						},
					},
					Notes = "Sends the specified animation of the specified entity to the client. The AnimationNumber is protocol-specific.",
				},
				SendHideTitle =
				{
					Notes = "Hides the title. This makes the title and subtitle disappear, but if you call SendTitleTimes() the same title and subtitle will appear again."
				},
				SendPluginMessage =
				{
					Params =
					{
						{
							Name = "Channel",
							Type = "string",
						},
						{
							Name = "Message",
							Type = "string",
						},
					},
					Notes = "Sends the plugin message on the specified channel.",
				},
				SendResetTitle =
				{
					Notes = "Resets and hides the title but not the subtitle."
				},
				SendSetSubTitle =
				{
					Params =
					{
						{
							Name = "SubTitle",
							Type = "cCompositeChat",
						},
					},
					Notes = "Sends the subtitle to the client. Doesn't make the client display it yet, use SendTitleTimes() to show both the title and the subtitle."
				},
				SendSetRawSubTitle =
				{
					Params =
					{
						{
							Name = "SubTitle",
							Type = "string",
						},
					},
					Notes = "Sends the raw subtitle to the client. Doesn't make the client display it yet, use SendTitleTimes() to show both the title and the subtitle."
				},
				SendSetTitle =
				{
					Params =
					{
						{
							Name = "Title",
							Type = "cCompositeChat",
						},
					},
					Notes = "Sends the title to the client. Doesn't make the client display it yet, use SendTitleTimes() to show both the title and the subtitle."
				},
				SendSetRawTitle =
				{
					Params =
					{
						{
							Name = "Title",
							Type = "string",
						},
					},
					Notes = "Sends the raw title to the client. Doesn't make the client display it yet, use SendTitleTimes() to show both the title and the subtitle."
				},
				SendSoundEffect =
				{
					Params =
					{
						{
							Name = "SoundName",
							Type = "string",
						},
						{
							Name = "Position",
							Type = "Vector3d",
						},
						{
							Name = "Volume",
							Type = "number",
						},
						{
							Name = "Pitch",
							Type = "number",
						},
					},
					Notes = "Sends a sound effect request to the client. The sound is played at the specified coords, with the specified volume (a float, 1.0 is full volume, can be more) and pitch (0-255, 63 is 100%)",
				},
				SendSoundEffect =
				{
					Params =
					{
						{
							Name = "SoundName",
							Type = "string",
						},
						{
							Name = "X",
							Type = "number",
						},
						{
							Name = "Y",
							Type = "number",
						},
						{
							Name = "Z",
							Type = "number",
						},
						{
							Name = "Volume",
							Type = "number",
						},
						{
							Name = "Pitch",
							Type = "number",
						},
					},
					Notes = "Sends a sound effect request to the client. The sound is played at the specified coords, with the specified volume (a float, 1.0 is full volume, can be more) and pitch (0-255, 63 is 100%) (DEPRECATED, use vector-parametered version instead)",
				},
				SendTitleTimes =
				{
					Params =
					{
						{
							Name = "FadeInTicks",
							Type = "number",
						},
						{
							Name = "DisplayTicks",
							Type = "number",
						},
						{
							Name = "FadeOutTicks",
							Type = "number",
						},
					},
					Notes = "Sends the request to display the title and subtitle, previously set with SendSetTitle, SendSetRawTitle, SendSetSubTitle and SendSetRawSubTitle, to the client."
				},
				SendTimeUpdate =
				{
					Params =
					{
						{
							Name = "WorldAge",
							Type = "number",
						},
						{
							Name = "TimeOfDay",
							Type = "number",
						},
						{
							Name = "DoDaylightCycle",
							Type = "boolean",
						},
					},
					Notes = "Sends the specified time update to the client. WorldAge is the total age of the world, in ticks. TimeOfDay is the current day's time, in ticks (0 - 24000). DoDaylightCycle is a bool that specifies whether the client should automatically move the sun (true) or keep it in the same place (false).",
				},
				SetClientBrand =
				{
					Params =
					{
						{
							Name = "ClientBrand",
							Type = "string",
						},
					},
					Notes = "Sets the value of the client's brand. Normally this value is received from the client by a MC|Brand plugin message, this function lets plugins overwrite the value.",
				},
				SetLocale =
				{
					Params =
					{
						{
							Name = "Locale",
							Type = "string",
						},
					},
					Notes = "Sets the locale that Cuberite keeps on record. Initially the locale is initialized in protocol handshake, this function allows plugins to override the stored value (but only server-side and only until the user disconnects).",
				},
				SetUsername =
				{
					Params =
					{
						{
							Name = "Name",
							Type = "string",
						},
					},
					Notes = "Sets the username",
				},
				SetViewDistance =
				{
					Params =
					{
						{
							Name = "ViewDistance",
							Type = "number",
						},
					},
					Notes = "Sets the viewdistance (number of chunks loaded for the player in each direction)",
				},
			},
			Constants =
			{
				MAX_VIEW_DISTANCE =
				{
					Notes = "The maximum value of the view distance",
				},
				MIN_VIEW_DISTANCE =
				{
					Notes = "The minimum value of the view distance",
				},
			},
		},
		cColor =
		{
			Desc = [[
				Encapsulates a RGB color, e.g. for armor.
			]],
			Functions =
			{
				Clear =
				{
					Notes = "Resets the color to uninitialized."
				},
				constructor =
				{
					{
						Returns = { {Type="cColor"} },
						Notes = "Creates an uninitialized cColor. Each component must be between 0 and 255, inclusive.",
					},
					{
						Params =
						{
							{
								Name = "Red",
								Type = "number",
							},
							{
								Name = "Green",
								Type = "number",
							},
							{
								Name = "Blue",
								Type = "number",
							},
						},
						Returns = { {Type="cColor"} },
						Notes = "Creates the specified cColor. All components must be between 0 and 255, inclusive.",
					},
				},
				GetColor =
				{
					Returns =
					{
						{
							Name = "Red",
							Type = "number",
						},
						{
							Name = "Green",
							Type = "number",
						},
						{
							Name = "Blue",
							Type = "number",
						},
					},
					Notes = "Returns the color's red, green, and blue components, respectively."
				},
				GetRed =
				{
					Returns =
					{
						{
							Name = "Red",
							Type = "number",
						},
					},
					Notes = "Returns the color's red component."
				},
				GetGreen =
				{
					Returns =
					{
						{
							Name = "Green",
							Type = "number",
						},
					},
					Notes = "Returns the color's green component."
				},
				GetBlue =
				{
					Returns =
					{
						{
							Name = "Blue",
							Type = "number",
						},
					},
					Notes = "Returns the color's blue component."
				},
				IsValid =
				{
					Returns =
					{
						{
							Type = "boolean"
						},
					},
					Notes = "True if the color is valid, false if the color has not been set yet."
				},
				SetColor =
				{
					Params =
					{
						{
							Name = "Red",
							Type = "number"
						},
						{
							Name = "Green",
							Type = "number"
						},
						{
							Name = "Blue",
							Type = "number"
						},
					},
					Notes = "Sets the color's red, green, and blue components. Values range from 0 to 255."
				},
				SetRed =
				{
					Params =
					{
						{
							Name = "Red",
							Type = "number",
						},
					},
					Notes = "Sets the color's red component. Must be between 0 and 255, inclusive."
				},
				SetGreen =
				{
					Params =
					{
						{
							Name = "Green",
							Type = "number",
						},
					},
					Notes = "Sets the color's green component. Must be between 0 and 255, inclusive."
				},
				SetBlue =
				{
					Params =
					{
						{
							Name = "Blue",
							Type = "number",
						},
					},
					Notes = "Sets the color's blue component. Must be between 0 and 255, inclusive."
				},
			},
			Constants =
			{
				COLOR_LIMIT =
				{
					Notes = "The upper bound (exclusive) for a color component",
				},
				COLOR_MAX =
				{
					Notes = "The maximum value for a color component",
				},
				COLOR_MIN =
				{
					Notes = "The minimum value for a color component",
				},
				COLOR_NONE =
				{
					Notes = "A constant denoting the color is invalid (note: use IsValid)",
				},
			},
		},
		cCompositeChat =
		{
			Desc = [[
				Encapsulates a chat message that can contain various formatting, URLs, commands executed on click
				and commands suggested on click. The chat message can be sent by the regular chat-sending functions,
				{{cPlayer}}:SendMessage(), {{cWorld}}:BroadcastChat() and {{cRoot}}:BroadcastChat().</p>
				<p>
				Note that most of the functions in this class are so-called chaining modifiers - they modify the
				object and then return the object itself, so that they can be chained one after another. See the
				Chaining example below for details.</p>
				<p>
				Each part of the composite chat message takes a "Style" parameter, this is a string that describes
				the formatting. It uses the following strings, concatenated together:
				<table>
				<tr><th>String</th><th>Style</th></tr>
				<tr><td>b</td><td>Bold text</td></tr>
				<tr><td>i</td><td>Italic text</td></tr>
				<tr><td>u</td><td>Underlined text</td></tr>
				<tr><td>s</td><td>Strikethrough text</td></tr>
				<tr><td>o</td><td>Obfuscated text</td></tr>
				<tr><td>@X</td><td>color X (X is 0 - 9 or a - f, same as dye meta</td></tr>
				</table>
				The following picture, taken from MineCraft Wiki, illustrates the color codes:</p>
				<img src="https://hydra-media.cursecdn.com/minecraft.gamepedia.com/4/4c/Colors.png?version=34a0f56789a95326e1f7d82047b12232" />
			]],
			Functions =
			{
				AddRunCommandPart =
				{
					Params =
					{
						{
							Name = "Text",
							Type = "string",
						},
						{
							Name = "Command",
							Type = "string",
						},
						{
							Name = "Style",
							Type = "string",
							IsOptional = true,
						},
					},
					Returns =
					{
						{
							Type = "self",
						},
					},
					Notes = "Adds a text which, when clicked, runs the specified command. Chaining.",
				},
				AddShowAchievementPart =
				{
					Params =
					{
						{
							Name = "PlayerName",
							Type = "string",
						},
						{
							Name = "AchievementName",
							Type = "string",
						},
						{
							Name = "Style",
							Type = "string",
							IsOptional = true,
						},
					},
					Notes = "Adds a text that represents the 'Achievement get' message.",
				},
				AddSuggestCommandPart =
				{
					Params =
					{
						{
							Name = "Text",
							Type = "string",
						},
						{
							Name = "Command",
							Type = "string",
						},
						{
							Name = "Style",
							Type = "string",
							IsOptional = true,
						},
					},
					Returns =
					{
						{
							Type = "self",
						},
					},
					Notes = "Adds a text which, when clicked, puts the specified command into the player's chat input area. Chaining.",
				},
				AddTextPart =
				{
					Params =
					{
						{
							Name = "Text",
							Type = "string",
						},
						{
							Name = "Style",
							Type = "string",
							IsOptional = true,
						},
					},
					Returns =
					{
						{
							Type = "self",
						},
					},
					Notes = "Adds a regular text. Chaining.",
				},
				AddUrlPart =
				{
					Params =
					{
						{
							Name = "Text",
							Type = "string",
						},
						{
							Name = "Url",
							Type = "string",
						},
						{
							Name = "Style",
							Type = "string",
							IsOptional = true,
						},
					},
					Returns =
					{
						{
							Type = "self",
						},
					},
					Notes = "Adds a text which, when clicked, opens up a browser at the specified URL. Chaining.",
				},
				Clear =
				{
					Returns =
					{
						{
							Type = "self",
						},
					},
					Notes = "Removes all parts from this object",
				},
				constructor =
				{
					{
						Returns = { {Type = "cCompositeChat"} },
						Notes = "Creates an empty chat message",
					},
					{
						Params =
						{
							{
								Name = "Text",
								Type = "string",
							},
							{
								Name = "MessageType",
								Type = "eMessageType",
								IsOptional = true,
							},
						},
						Returns = { {Type = "cCompositeChat"} },
						Notes = "Creates a chat message containing the specified text, parsed by the ParseText() function. This allows easy migration from old chat messages.",
					},
				},
				CreateJsonString =
				{
					Params =
					{
						{
							Name = "AddPrefixes",
							Type = "boolean",
							IsOptional = true,
						},
					},
					Returns =
					{
						{
							Type = "string",
						},
					},
					Notes = "Returns the entire object serialized into JSON, as it would be sent to a client. AddPrefixes specifies whether the chat prefixes should be prepended to the message, true by default.",
				},
				ExtractText =
				{
					Returns =
					{
						{
							Type = "string",
						},
					},
					Notes = "Returns the text from the parts that comprises the human-readable data. Used for older protocols that don't support composite chat, and for console-logging.",
				},
				GetAdditionalMessageTypeData =
				{
					Returns =
					{
						{
							Type = "string",
						},
					},
					Notes = "Returns the AdditionalData associated with the message, such as the sender's name for mtPrivateMessage",
				},
				GetMessageType =
				{
					Returns =
					{
						{
							Type = "eMessageType",
						},
					},
					Notes = "Returns the MessageType (mtXXX constant) that is associated with this message. When sent to a player, the message will be formatted according to this message type and the player's settings (adding \"[INFO]\" prefix etc.)",
				},
				ParseText =
				{
					Params =
					{
						{
							Name = "Text",
							Type = "string",
						},
					},
					Returns =
					{
						{
							Type = "self",
						},
					},
					Notes = "Adds text, while recognizing http and https URLs and old-style formatting codes (\"@2\"). Chaining.",
				},
				SetMessageType =
				{
					Params =
					{
						{
							Name = "MessageType",
							Type = "eMessageType",
						},
						{
							Name = "AdditionalData",
							Type = "string",
							IsOptional = true,
						},
					},
					Returns =
					{
						{
							Type = "self",
						},
					},
					Notes = "Sets the MessageType (mtXXX constant) that is associated with this message. Also sets the additional data (string) associated with the message, which is specific for the message type - such as the sender's name for mtPrivateMessage. When sent to a player, the message will be formatted according to this message type and the player's settings (adding \"[INFO]\" prefix etc.). Chaining.",
				},
				UnderlineUrls =
				{
					Returns =
					{
						{
							Type = "self",
						},
					},
					Notes = "Makes all URL parts contained in the message underlined. Doesn't affect parts added in the future. Chaining.",
				},
			},
			AdditionalInfo =
			{
				{
					Header = "Chaining example",
					Contents = [[
						Sending a chat message that is composed of multiple different parts has been made easy thanks to
						chaining. Consider the following example that shows how a message containing all kinds of parts
						is sent (adapted from the Debuggers plugin):
<pre class="prettyprint lang-lua">
function OnPlayerJoined(a_Player)
	-- Send an example composite chat message to the player:
	a_Player:SendMessage(cCompositeChat()
		:AddTextPart("Hello, ")
		:AddUrlPart(a_Player:GetName(), "https://cuberite.org", "u@2")  -- Colored underlined link
		:AddSuggestCommandPart(", and welcome.", "/help", "u")       -- Underlined suggest-command
		:AddRunCommandPart(" SetDay", "/time set 0")                 -- Regular text that will execute command when clicked
		:SetMessageType(mtJoin)                                      -- It is a join-message
	)
end</pre>
					]],
				},
			},
		},
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
				Clear =
				{
					Notes = "Clears the entire grid",
				},
				constructor =
				{
					Params =
					{
						{
							Name = "Width",
							Type = "number",
						},
						{
							Name = "Height",
							Type = "number",
						},
					},
					Returns =
					{
						{
							Type = "cCraftingGrid",
						},
					},
					Notes = "Creates a new CraftingGrid object. This new crafting grid is not related to any player, but may be needed for {{cCraftingRecipe}}'s ConsumeIngredients function.",
				},
				ConsumeGrid =
				{
					Params =
					{
						{
							Name = "CraftingGrid",
							Type = "cCraftingGrid",
						},
					},
					Notes = "Consumes items specified in CraftingGrid from the current contents. Used internally by {{cCraftingRecipe}}'s ConsumeIngredients() function, but available to plugins, too.",
				},
				Dump =
				{
					Notes = "DEBUG build: Dumps the contents of the grid to the log. RELEASE build: no action",
				},
				GetHeight =
				{
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the height of the grid",
				},
				GetItem =
				{
					Params =
					{
						{
							Name = "x",
							Type = "number",
						},
						{
							Name = "y",
							Type = "number",
						},
					},
					Returns =
					{
						{
							Type = "cItem",
						},
					},
					Notes = "Returns the item at the specified coords",
				},
				GetWidth =
				{
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the width of the grid",
				},
				SetItem =
				{
					{
						Params =
						{
							{
								Name = "x",
								Type = "number",
							},
							{
								Name = "y",
								Type = "number",
							},
							{
								Name = "cItem",
								Type = "cItem",
							},
						},
						Notes = "Sets the item at the specified coords",
					},
					{
						Params =
						{
							{
								Name = "x",
								Type = "number",
							},
							{
								Name = "y",
								Type = "number",
							},
							{
								Name = "ItemType",
								Type = "number",
							},
							{
								Name = "ItemCount",
								Type = "number",
							},
							{
								Name = "ItemDamage",
								Type = "number",
							},
						},
						Notes = "Sets the item at the specified coords",
					},
				},
			},
		},
		cCraftingRecipe =
		{
			Desc = [[
				This class is used to represent a crafting recipe, either a built-in one, or one created dynamically in a plugin. It is used only as a parameter for {{OnCraftingNoRecipe|OnCraftingNoRecipe}}, {{OnPostCrafting|OnPostCrafting}} and {{OnPreCrafting|OnPreCrafting}} hooks. Plugins may use it to inspect or modify a crafting recipe that a player views in their crafting window, either at a crafting table or the survival inventory screen.
</p>
		<p>Internally, the class contains a {{cCraftingGrid}} for the ingredients and a {{cItem}} for the result.
]],
			Functions =
			{
				Clear =
				{
					Notes = "Clears the entire recipe, both ingredients and results",
				},
				ConsumeIngredients =
				{
					Params =
					{
						{
							Name = "CraftingGrid",
							Type = "cCraftingGrid",
						},
					},
					Notes = "Consumes ingredients specified in the given {{cCraftingGrid|cCraftingGrid}} class",
				},
				Dump =
				{
					Notes = "DEBUG build: dumps ingredients and result into server log. RELEASE build: no action",
				},
				GetIngredient =
				{
					Params =
					{
						{
							Name = "x",
							Type = "number",
						},
						{
							Name = "y",
							Type = "number",
						},
					},
					Returns =
					{
						{
							Type = "cItem",
						},
					},
					Notes = "Returns the ingredient stored in the recipe at the specified coords",
				},
				GetIngredientsHeight =
				{
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the height of the ingredients' grid",
				},
				GetIngredientsWidth =
				{
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the width of the ingredients' grid",
				},
				GetResult =
				{
					Returns =
					{
						{
							Type = "cItem",
						},
					},
					Notes = "Returns the result of the recipe",
				},
				SetIngredient =
				{
					{
						Params =
						{
							{
								Name = "x",
								Type = "number",
							},
							{
								Name = "y",
								Type = "number",
							},
							{
								Name = "cItem",
								Type = "cItem",
							},
						},
						Notes = "Sets the ingredient at the specified coords",
					},
					{
						Params =
						{
							{
								Name = "x",
								Type = "number",
							},
							{
								Name = "y",
								Type = "number",
							},
							{
								Name = "ItemType",
								Type = "number",
							},
							{
								Name = "ItemCount",
								Type = "number",
							},
							{
								Name = "ItemDamage",
								Type = "number",
							},
						},
						Notes = "Sets the ingredient at the specified coords",
					},
				},
				SetResult =
				{
					{
						Params =
						{
							{
								Name = "cItem",
								Type = "cItem",
							},
						},
						Notes = "Sets the result item",
					},
					{
						Params =
						{
							{
								Name = "ItemType",
								Type = "number",
							},
							{
								Name = "ItemCount",
								Type = "number",
							},
							{
								Name = "ItemDamage",
								Type = "number",
							},
						},
						Notes = "Sets the result item",
					},
				},
			},
		},
		cCryptoHash =
		{
			Desc = [[
				Provides functions for generating cryptographic hashes.</p>
				<p>
				Note that all functions in this class are super-static, so they are to be called in the dot convention:
<pre class="prettyprint lang-lua">
local Hash = cCryptoHash.sha1HexString("DataToHash")
</pre></p>
				<p>Each cryptographic hash has two variants, one returns the hash as a raw binary string, the other returns the hash as a hex-encoded string twice as long as the binary string.
			]],
			Functions =
			{
				md5 =
				{
					IsStatic = true,
					IsGlobal = true,
					Params =
					{
						{
							Name = "Data",
							Type = "string",
						},
					},
					Returns =
					{
						{
							Type = "string",
						},
					},
					Notes = "Calculates the md5 hash of the data, returns it as a raw (binary) string of 16 characters.",
				},
				md5HexString =
				{
					IsStatic = true,
					IsGlobal = true,
					Params =
					{
						{
							Name = "Data",
							Type = "string",
						},
					},
					Returns =
					{
						{
							Type = "string",
						},
					},
					Notes = "Calculates the md5 hash of the data, returns it as a hex-encoded string of 32 characters.",
				},
				sha1 =
				{
					IsStatic = true,
					IsGlobal = true,
					Params =
					{
						{
							Name = "Data",
							Type = "string",
						},
					},
					Returns =
					{
						{
							Type = "string",
						},
					},
					Notes = "Calculates the sha1 hash of the data, returns it as a raw (binary) string of 20 characters.",
				},
				sha1HexString =
				{
					IsStatic = true,
					IsGlobal = true,
					Params =
					{
						{
							Name = "Data",
							Type = "string",
						},
					},
					Returns =
					{
						{
							Type = "string",
						},
					},
					Notes = "Calculates the sha1 hash of the data, returns it as a hex-encoded string of 40 characters.",
				},
			},
		},
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
				Add =
				{
					Params =
					{
						{
							Name = "Other",
							Type = "cEnchantments",
						},
					},
					Notes = "Adds the enchantments contained in Other into this object. Existing enchantments are preserved, unless Other specifies a different level, in which case the level is changed to the Other's one.",
				},
				AddFromString =
				{
					Params =
					{
						{
							Name = "StringSpec",
							Type = "string",
						},
					},
					Notes = "Adds the enchantments in the string description into the object. If a specified enchantment already existed, it is overwritten.",
				},
				Clear =
				{
					Notes = "Removes all enchantments",
				},
				constructor =
				{
					{
						Returns =
						{
							{
								Type = "cEnchantments",
							},
						},
						Notes = "Creates a new empty cEnchantments object",
					},
					{
						Params =
						{
							{
								Name = "StringSpec",
								Type = "string",
							},
						},
						Returns =
						{
							{
								Type = "cEnchantments",
							},
						},
						Notes = "Creates a new cEnchantments object filled with enchantments based on the string description",
					},
				},
				Count =
				{
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Get the count of enchantments contained within the class",
				},
				GetLevel =
				{
					Params =
					{
						{
							Name = "EnchantmentNumID",
							Type = "number",
						},
					},
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the level of the specified enchantment stored in this object; 0 if not stored",
				},
				CanAddEnchantment =
				{
					Params =
					{
						{
							Name = "EnchantmentNumID",
							Type = "number",
						},
					},
					Returns =
					{
						{
							Type = "boolean"
						},
					},
					Notes = "Returns true if the specified enchantment is not mutually exclusive with any of the enchantments stored by the object.",
				},
				IsEmpty =
				{
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Returns true if the object stores no enchantments",
				},
				operator_eq =
				{
					Params =
					{
						{
							Name = "OtherEnchantments",
							Type = "cEnchantments",
						},
					},
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Returns true if this enchantments object has the same enchantments as OtherEnchantments.",
				},
				SetLevel =
				{
					Params =
					{
						{
							Name = "EnchantmentNumID",
							Type = "number",
						},
						{
							Name = "Level",
							Type = "number",
						},
					},
					Notes = "Sets the level for the specified enchantment, adding it if not stored before, or removing it if Level < = 0",
				},
				StringToEnchantmentID =
				{
					IsStatic = true,
					Params =
					{
						{
							Name = "EnchantmentName",
							Type = "string",
						},
					},
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the enchantment numerical ID, -1 if not understood. Case insensitive. Also understands plain numbers.",
				},
				ToString =
				{
					Returns =
					{
						{
							Type = "string",
						},
					},
					Notes = "Returns the string description of all the enchantments stored in this object, in numerical-ID form",
				},
			},
			Constants =
			{
				enchAquaAffinity =
				{
					Notes = "",
				},
				enchBaneOfArthropods =
				{
					Notes = "",
				},
				enchBlastProtection =
				{
					Notes = "",
				},
				enchEfficiency =
				{
					Notes = "",
				},
				enchFeatherFalling =
				{
					Notes = "",
				},
				enchFireAspect =
				{
					Notes = "",
				},
				enchFireProtection =
				{
					Notes = "",
				},
				enchFlame =
				{
					Notes = "",
				},
				enchFortune =
				{
					Notes = "",
				},
				enchInfinity =
				{
					Notes = "",
				},
				enchKnockback =
				{
					Notes = "",
				},
				enchLooting =
				{
					Notes = "",
				},
				enchLuckOfTheSea =
				{
					Notes = "",
				},
				enchLure =
				{
					Notes = "",
				},
				enchPower =
				{
					Notes = "",
				},
				enchProjectileProtection =
				{
					Notes = "",
				},
				enchProtection =
				{
					Notes = "",
				},
				enchPunch =
				{
					Notes = "",
				},
				enchRespiration =
				{
					Notes = "",
				},
				enchSharpness =
				{
					Notes = "",
				},
				enchSilkTouch =
				{
					Notes = "",
				},
				enchSmite =
				{
					Notes = "",
				},
				enchThorns =
				{
					Notes = "",
				},
				enchUnbreaking =
				{
					Notes = "",
				},
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
				Note that you should not store a cEntity object between two hooks' calls, because Cuberite may
				despawn / remove that entity in between the calls. If you need to refer to an entity later, use its
				UniqueID and {{cWorld|cWorld}}'s entity manipulation functions DoWithEntityByID(), ForEachEntity()
				or ForEachEntityInChunk() to access the entity again.</p>
			]],
			Functions =
			{
				AddPosition =
				{
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
						Notes = "Moves the entity by the specified amount in each axis direction",
					},
					{
						Params =
						{
							{
								Name = "Offset",
								Type = "Vector3d",
							},
						},
						Notes = "Moves the entity by the specified amount in each direction",
					},
				},
				AddPosX =
				{
					Params =
					{
						{
							Name = "OffsetX",
							Type = "number",
						},
					},
					Notes = "Moves the entity by the specified amount in the X axis direction",
				},
				AddPosY =
				{
					Params =
					{
						{
							Name = "OffsetY",
							Type = "number",
						},
					},
					Notes = "Moves the entity by the specified amount in the Y axis direction",
				},
				AddPosZ =
				{
					Params =
					{
						{
							Name = "OffsetZ",
							Type = "number",
						},
					},
					Notes = "Moves the entity by the specified amount in the Z axis direction",
				},
				AddSpeed =
				{
					{
						Params =
						{
							{
								Name = "AddX",
								Type = "number",
							},
							{
								Name = "AddY",
								Type = "number",
							},
							{
								Name = "AddZ",
								Type = "number",
							},
						},
						Notes = "Adds the specified amount of speed in each axis direction.",
					},
					{
						Params =
						{
							{
								Name = "Add",
								Type = "Vector3d",
							},
						},
						Notes = "Adds the specified amount of speed in each axis direction.",
					},
				},
				AddSpeedX =
				{
					Params =
					{
						{
							Name = "AddX",
							Type = "number",
						},
					},
					Notes = "Adds the specified amount of speed in the X axis direction.",
				},
				AddSpeedY =
				{
					Params =
					{
						{
							Name = "AddY",
							Type = "number",
						},
					},
					Notes = "Adds the specified amount of speed in the Y axis direction.",
				},
				AddSpeedZ =
				{
					Params =
					{
						{
							Name = "AddZ",
							Type = "number",
						},
					},
					Notes = "Adds the specified amount of speed in the Z axis direction.",
				},
				ApplyArmorDamage =
				{
					Params =
					{
						{
							Name = "DamageBlocked",
							Type = "number",
						},
					},
					Notes = "Lowers armor durability, as if the armor blocked the given amount of damage.",
				},
				ArmorCoversAgainst =
				{
					Params =
					{
						{
							Name = "DamageType",
							Type = "eDamageType",
						},
					},
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Returns whether armor will protect against the specified damage type",
				},
				Destroy =
				{
					Params =
					{
						{
							Name = "ShouldBroadcast <b>(DEPRECATED)</b>",
							Type = "boolean",
							IsOptional = true,
						},
					},
					Notes = "Schedules the entity to be destroyed; broadcasts the DestroyEntity packet",
				},
				DoesPreventBlockPlacement =
				{
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Returns true if this entity doesn't allow blocks to be placed intersecting the entity.",
				},
				GetAirLevel =
				{
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the air level (number of ticks of air left). Note, this function is only updated with mobs or players.",
				},
				GetArmorCoverAgainst =
				{
					Params =
					{
						{
							Name = "AttackerEntity",
							Type = "cEntity",
						},
						{
							Name = "DamageType",
							Type = "eDamageType",
						},
						{
							Name = "RawDamage",
							Type = "number",
						},
					},
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the number of hitpoints out of RawDamage that the currently equipped armor would cover. See {{TakeDamageInfo}} for more information on attack damage.",
				},
				GetBoundingBox =
				{
					Returns =
					{
						{
							Type = "cBoundingBox",
						},
					},
					Notes = "Returns the bounding box of the entity, which has width and height corresponding to the entity, and is aligned with the block grid.",
				},
				GetChunkX =
				{
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the X-coord of the chunk in which the entity is placed",
				},
				GetChunkZ =
				{
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the Z-coord of the chunk in which the entity is placed",
				},
				GetClass =
				{
					Returns =
					{
						{
							Type = "string",
						},
					},
					Notes = "Returns the classname of the entity, such as \"cSpider\" or \"cPickup\"",
				},
				GetClassStatic =
				{
					IsStatic = true,
					Returns =
					{
						{
							Type = "string",
						},
					},
					Notes = "Returns the entity classname that this class implements. Each descendant overrides this function.",
				},
				GetEnchantmentBlastKnockbackReduction =
				{
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns explosion knock back reduction percent from blast protection level.",
				},
				GetEnchantmentCoverAgainst =
				{
					Params =
					{
						{
							Name = "AttackerEntity",
							Type = "cEntity",
						},
						{
							Name = "DamageType",
							Type = "eDamageType",
						},
						{
							Name = "RawDamage",
							Type = "number",
						},
					},
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the number of hitpoints out of RawDamage that the enchantments on the currently equipped armor would cover. See {{TakeDamageInfo}} for more information on attack damage.",
				},
				GetEntityType =
				{
					Returns =
					{
						{
							Name = "EntityType",
							Type = "cEntity#eEntityType",
						},
					},
					Notes = "Returns the type of the entity, one of the {{cEntity#eEntityType|etXXX}} constants. Note that to check specific entity type, you should use one of the IsXXX functions instead of comparing the value returned by this call.",
				},
				GetEquippedBoots =
				{
					Returns =
					{
						{
							Type = "cItem",
						},
					},
					Notes = "Returns the boots that the entity has equipped. Returns an empty cItem if no boots equipped or not applicable.",
				},
				GetEquippedChestplate =
				{
					Returns =
					{
						{
							Type = "cItem",
						},
					},
					Notes = "Returns the chestplate that the entity has equipped. Returns an empty cItem if no chestplate equipped or not applicable.",
				},
				GetEquippedHelmet =
				{
					Returns =
					{
						{
							Type = "cItem",
						},
					},
					Notes = "Returns the helmet that the entity has equipped. Returns an empty cItem if no helmet equipped or not applicable.",
				},
				GetEquippedLeggings =
				{
					Returns =
					{
						{
							Type = "cItem",
						},
					},
					Notes = "Returns the leggings that the entity has equipped. Returns an empty cItem if no leggings equipped or not applicable.",
				},
				GetEquippedWeapon =
				{
					Returns =
					{
						{
							Type = "cItem",
						},
					},
					Notes = "Returns the weapon that the entity has equipped. Returns an empty cItem if no weapon equipped or not applicable.",
				},
				GetOffHandEquipedItem =
				{
					Returns =
					{
						{
							Type = "cItem",
						},
					},
					Notes = "Returns the item that the entity has equipped on off-hand. Returns an empty cItem if no item equipped or not applicable.",
				},
				GetGravity =
				{
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the number that is used as the gravity for physics simulation. 1G (9.78) by default.",
				},
				GetHeadYaw =
				{
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the pitch of the entity's head (FIXME: Rename to GetHeadPitch() ).",
				},
				GetHealth =
				{
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the current health of the entity.",
				},
				GetHeight =
				{
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the height (Y size) of the entity",
				},
				GetInvulnerableTicks =
				{
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the number of ticks that this entity will be invulnerable for. This is used for after-hit recovery - the entities are invulnerable for half a second after being hit.",
				},
				GetKnockbackAmountAgainst =
				{
					Params =
					{
						{
							Name = "ReceiverEntity",
							Type = "cEntity",
						},
					},
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the amount of knockback that the currently equipped items would cause when attacking the ReceiverEntity.",
				},
				GetLookVector =
				{
					Returns =
					{
						{
							Type = "Vector3f",
						},
					},
					Notes = "Returns the vector that defines the direction in which the entity is looking",
				},
				GetMass =
				{
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the mass of the entity. Currently unused.",
				},
				GetMaxHealth =
				{
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the maximum number of hitpoints this entity is allowed to have.",
				},
				GetParentClass =
				{
					Returns =
					{
						{
							Type = "string",
						},
					},
					Notes = "Returns the name of the direct parent class for this entity",
				},
				GetPitch =
				{
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the pitch (nose-down rotation) of the entity. Measured in degrees, normal values range from -90 to +90. +90 means looking down, 0 means looking straight ahead, -90 means looking up.",
				},
				GetPosition =
				{
					Returns =
					{
						{
							Type = "Vector3d",
						},
					},
					Notes = "Returns the entity's pivot position as a 3D vector",
				},
				GetPosX =
				{
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the X-coord of the entity's pivot",
				},
				GetPosY =
				{
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the Y-coord of the entity's pivot",
				},
				GetPosZ =
				{
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the Z-coord of the entity's pivot",
				},
				GetRawDamageAgainst =
				{
					Params =
					{
						{
							Name = "ReceiverEntity",
							Type = "cEntity",
						},
					},
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the raw damage that this entity's equipment would cause when attacking the ReceiverEntity. This includes this entity's weapon {{cEnchantments|enchantments}}, but excludes the receiver's armor or potion effects. See {{TakeDamageInfo}} for more information on attack damage.",
				},
				GetRoll =
				{
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the roll (sideways rotation) of the entity. Currently unused.",
				},
				GetSpeed =
				{
					Returns =
					{
						{
							Type = "Vector3d",
						},
					},
					Notes = "Returns the complete speed vector of the entity",
				},
				GetSpeedX =
				{
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the X-part of the speed vector",
				},
				GetSpeedY =
				{
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the Y-part of the speed vector",
				},
				GetSpeedZ =
				{
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the Z-part of the speed vector",
				},
				GetTicksAlive =
				{
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the number of ticks that this entity has been alive for.",
				},
				GetUniqueID =
				{
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the ID that uniquely identifies the entity within the running server. Note that this ID is not persisted to the data files.",
				},
				GetWidth =
				{
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the width (X and Z size) of the entity.",
				},
				GetWorld =
				{
					Returns =
					{
						{
							Type = "cWorld",
						},
					},
					Notes = "Returns the world where the entity resides",
				},
				GetYaw =
				{
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the yaw (direction) of the entity. Measured in degrees, values range from -180 to +180. 0 means ZP, 90 means XM, -180 means ZM, -90 means XP.",
				},
				HandleSpeedFromAttachee =
				{
					Params =
					{
						{
							Name = "ForwardAmount",
							Type = "number",
						},
						{
							Name = "SidewaysAmount",
							Type = "number",
						},
					},
					Notes = "Updates the entity's speed based on the attachee exerting the specified force forward and sideways. Used for entities being driven by other entities attached to them - usually players driving minecarts and boats.",
				},
				Heal =
				{
					Params =
					{
						{
							Name = "Hitpoints",
							Type = "number",
						},
					},
					Notes = "Heals the specified number of hitpoints. Hitpoints is expected to be a positive number.",
				},
				IsA =
				{
					Params =
					{
						{
							Name = "ClassName",
							Type = "string",
						},
					},
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Returns true if the entity class is a descendant of the specified class name, or the specified class itself",
				},
				IsArrow =
				{
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Returns true if the entity is an arrow.",
				},
				IsBoat =
				{
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Returns true if the entity is a {{cBoat|boat}}.",
				},
				IsCrouched =
				{
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Returns true if the entity is crouched. Always false for entities that don't support crouching.",
				},
				IsDestroyed =
				{
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "(<b>DEPRECATED</b>) Please use cEntity:IsTicking().",
				},
				IsEnderCrystal =
				{
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Returns true if the entity is an ender crystal.",
				},
				IsExpOrb =
				{
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Returns true if the entity represents an experience orb",
				},
				IsFallingBlock =
				{
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Returns true if the entity represents a {{cFallingBlock}} entity.",
				},
				IsFireproof =
				{
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Returns true if the entity takes no damage from being on fire.",
				},
				IsFloater =
				{
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Returns true if the entity represents a fishing rod floater",
				},
				IsInvisible =
				{
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Returns true if the entity is invisible",
				},
				IsInFire =
				{
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Returns true if any part of the entity is in a fire block",
				},
				IsInLava =
				{
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Returns true if any part of the entity is in a lava block",
				},
				IsInWater =
				{
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Returns true if any part of the entity is in a water block",
				},
				IsHeadInWater =
				{
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Returns true if the entity's head is in a water block",
				},
				IsItemFrame =
				{
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Returns true if the entity is an item frame.",
				},
				IsLeashKnot =
				{
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Returns true if the entity is a leash knot.",
				},
				IsMinecart =
				{
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Returns true if the entity represents a {{cMinecart|minecart}}",
				},
				IsMob =
				{
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Returns true if the entity represents any {{cMonster|mob}}.",
				},
				IsOnFire =
				{
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Returns true if the entity is on fire",
				},
				IsOnGround =
				{
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Returns true if the entity is on ground (not falling, not jumping, not flying)",
				},
				IsPainting =
				{
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Returns if this entity is a painting.",
				},
				IsPawn =
				{
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Returns true if the entity is a {{cPawn}} descendant.",
				},
				IsPickup =
				{
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Returns true if the entity represents a {{cPickup|pickup}}.",
				},
				IsPlayer =
				{
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Returns true if the entity represents a {{cPlayer|player}}",
				},
				IsProjectile =
				{
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Returns true if the entity is a {{cProjectileEntity}} descendant.",
				},
				IsRclking =
				{
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Currently unimplemented",
				},
				IsRiding =
				{
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Returns true if the entity is attached to (riding) another entity.",
				},
				IsSprinting =
				{
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Returns true if the entity is sprinting. Entities that cannot sprint return always false",
				},
				IsSubmerged =
				{
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Returns true if the entity's head is in a water block <b>Currently deprecated in favour of IsHeadInWater()</b>",
				},
				IsSwimming =
				{
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Returns true if any part of the entity is in a water block. Note, this function is only updated with mobs or players. <b>Currently deprecated in favour of IsInWater()</b>",
				},
				IsTicking =
				{
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Returns true if the entity is valid and ticking. Returns false if the entity is not ticking and is about to leave its current world either via teleportation or destruction. If this returns false, you must stop using the cEntity pointer you have.",
				},
				IsTNT =
				{
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Returns true if the entity represents a {{cTNTEntity|TNT entity}}",
				},
				Killed =
				{
					Params =
					{
						{
							Name = "Victim",
							Type = "cEntity",
						},
					},
					Notes = "This entity has killed another entity (the Victim). For players, adds the scoreboard statistics about the kill.",
				},
				MoveToWorld =
				{
					{
						Params =
						{
							{
								Name = "World",
								Type = "cWorld",
							},
							{
								Name = "ShouldSendRespawn",
								Type = "boolean",
								IsOptional = true,
							},
						},
						Returns =
						{
							{
								Type = "boolean",
							},
						},
						Notes = "Removes the entity from this world and starts moving it to the specified world's spawn point. Note that to avoid deadlocks, the move is asynchronous - the entity is moved into a queue and will be moved from that queue into the destination world at some (unpredictable) time in the future. ShouldSendRespawn is used only for players, it specifies whether the player should be sent a Respawn packet upon leaving the world (The client handles respawns only between different dimensions).",
					},
					{
						Params =
						{
							{
								Name = "WorldName",
								Type = "string",
							},
							{
								Name = "ShouldSendRespawn",
								Type = "boolean",
								IsOptional = true,
							},
						},
						Returns =
						{
							{
								Type = "boolean",
							},
						},
						Notes = "Removes the entity from this world and starts moving it to the specified world's spawn point. Note that to avoid deadlocks, the move is asynchronous - the entity is moved into a queue and will be moved from that queue into the destination world at some (unpredictable) time in the future. ShouldSendRespawn is used only for players, it specifies whether the player should be sent a Respawn packet upon leaving the world (The client handles respawns only between different dimensions).",
					},
					{
						Params =
						{
							{
								Name = "World",
								Type = "cWorld",
							},
							{
								Name = "ShouldSendRespawn",
								Type = "boolean",
							},
							{
								Name = "Position",
								Type = "Vector3d",
							},
						},
						Returns =
						{
							{
								Type = "boolean",
							},
						},
						Notes = "Removes the entity from this world and starts moving it to the specified world. Note that to avoid deadlocks, the move is asynchronous - the entity is moved into a queue and will be moved from that queue into the destination world at some (unpredictable) time in the future. ShouldSendRespawn is used only for players, it specifies whether the player should be sent a Respawn packet upon leaving the world (The client handles respawns only between different dimensions). The Position parameter specifies the location that the entity should be placed in, in the new world.",
					},
					{
						Params =
						{
							{
								Name = "World",
								Type = "cWorld",
							},
							{
								Name = "Position",
								Type = "Vector3d",
							},
							{
								Name = "ShouldSetPortalCooldown",
								Type = "boolean",
								IsOptional = true,
							},
							{
								Name = "ShouldSendRespawn",
								Type = "boolean",
								IsOptional = true,
							},
						},
						Returns =
						{
							{
								Type = "boolean",
							},
						},
						Notes = "Removes the entity from this world and starts moving it to the specified world. Note that to avoid deadlocks, the move is asynchronous - the entity is moved into a queue and will be moved from that queue into the destination world at some (unpredictable) time in the future. If ShouldSetPortalCooldown is false (default), doesn't set any portal cooldown, if it is true, the default portal cooldown is applied to the entity. ShouldSendRespawn is used only for players, it specifies whether the player should be sent a Respawn packet upon leaving the world (The client handles respawns only between different dimensions). The Position parameter specifies the location that the entity should be placed in, in the new world.",
					},
				},
				ScheduleMoveToWorld =
				{
					Params =
					{
						{
							Name = "World",
							Type = "cWorld",
						},
						{
							Name = "NewPosition",
							Type = "Vector3d",
						},
						{
							Name = "ShouldSetPortalCooldown",
							Type = "boolean",
							IsOptional = true,
						},
						{
							Name = "ShouldSendRespawn",
							Type = "boolean",
							IsOptional  = true,
						},
					},
					Notes = "Schedules a MoveToWorld call to occur on the next Tick of the entity. If ShouldSetPortalCooldown is false (default), doesn't set any portal cooldown, if it is true, the default portal cooldown is applied to the entity. If ShouldSendRespawn is false, no respawn packet is sent, if it is true (default) then a respawn packet is sent to the client. <b>OBSOLETE</b>, use MoveToWorld instead.",
				},
				SetGravity =
				{
					Params =
					{
						{
							Name = "Gravity",
							Type = "number",
						},
					},
					Notes = "Sets the number that is used as the gravity for physics simulation. 1G (9.78) by default.",
				},
				SetHeadYaw =
				{
					Params =
					{
						{
							Name = "HeadPitch",
							Type = "number",
						},
					},
					Notes = "Sets the head pitch (FIXME: Rename to SetHeadPitch() ).",
				},
				SetHealth =
				{
					Params =
					{
						{
							Name = "Hitpoints",
							Type = "number",
						},
					},
					Notes = "Sets the entity's health to the specified amount of hitpoints. Doesn't broadcast any hurt animation. Doesn't kill the entity if health drops below zero. Use the TakeDamage() function instead for taking damage.",
				},
				SetInvulnerableTicks =
				{
					Params =
					{
						{
							Name = "NumTicks",
							Type = "number",
						},
					},
					Notes = "Sets the amount of ticks for which the entity will not receive any damage from other entities.",
				},
				SetIsFireproof =
				{
					Params =
					{
						{
							Name = "IsFireproof",
							Type = "boolean",
						},
					},
					Notes = "Sets whether the entity receives damage from being on fire.",
				},
				SetMass =
				{
					Params =
					{
						{
							Name = "Mass",
							Type = "number",
						},
					},
					Notes = "Sets the mass of the entity. Currently unused.",
				},
				SetMaxHealth =
				{
					Params =
					{
						{
							Name = "MaxHitpoints",
							Type = "number",
						},
					},
					Notes = "Sets the maximum hitpoints of the entity. If current health is above MaxHitpoints, it is capped to MaxHitpoints.",
				},
				SetPitch =
				{
					Params =
					{
						{
							Name = "Pitch",
							Type = "number",
						},
					},
					Notes = "Sets the pitch (nose-down rotation) of the entity",
				},
				SetPitchFromSpeed =
				{
					Notes = "Sets the entity pitch to match its speed (entity looking forwards as it moves)",
				},
				SetPosition =
				{
					{
						Params =
						{
							{
								Name = "PosX",
								Type = "number",
							},
							{
								Name = "PosY",
								Type = "number",
							},
							{
								Name = "PosZ",
								Type = "number",
							},
						},
						Notes = "Sets all three coords of the entity's pivot",
					},
					{
						Params =
						{
							{
								Name = "Vector3d",
								Type = "Vector3d",
							},
						},
						Notes = "Sets all three coords of the entity's pivot",
					},
				},
				SetPosX =
				{
					Params =
					{
						{
							Name = "PosX",
							Type = "number",
						},
					},
					Notes = "Sets the X-coord of the entity's pivot",
				},
				SetPosY =
				{
					Params =
					{
						{
							Name = "PosY",
							Type = "number",
						},
					},
					Notes = "Sets the Y-coord of the entity's pivot",
				},
				SetPosZ =
				{
					Params =
					{
						{
							Name = "PosZ",
							Type = "number",
						},
					},
					Notes = "Sets the Z-coord of the entity's pivot",
				},
				SetRoll =
				{
					Params =
					{
						{
							Name = "Roll",
							Type = "number",
						},
					},
					Notes = "Sets the roll (sideways rotation) of the entity. Currently unused.",
				},
				SetSpeed =
				{
					{
						Params =
						{
							{
								Name = "SpeedX",
								Type = "number",
							},
							{
								Name = "SpeedY",
								Type = "number",
							},
							{
								Name = "SpeedZ",
								Type = "number",
							},
						},
						Notes = "Sets the current speed of the entity",
					},
					{
						Params =
						{
							{
								Name = "Speed",
								Type = "Vector3d",
							},
						},
						Notes = "Sets the current speed of the entity",
					},
				},
				SetSpeedX =
				{
					Params =
					{
						{
							Name = "SpeedX",
							Type = "number",
						},
					},
					Notes = "Sets the X component of the entity speed",
				},
				SetSpeedY =
				{
					Params =
					{
						{
							Name = "SpeedY",
							Type = "number",
						},
					},
					Notes = "Sets the Y component of the entity speed",
				},
				SetSpeedZ =
				{
					Params =
					{
						{
							Name = "SpeedZ",
							Type = "number",
						},
					},
					Notes = "Sets the Z component of the entity speed",
				},
				SetYaw =
				{
					Params =
					{
						{
							Name = "Yaw",
							Type = "number",
						},
					},
					Notes = "Sets the yaw (direction) of the entity.",
				},
				SetYawFromSpeed =
				{
					Notes = "Sets the entity's yaw to match its current speed (entity looking forwards as it moves).",
				},
				StartBurning =
				{
					Params =
					{
						{
							Name = "NumTicks",
							Type = "number",
						},
					},
					Notes = "Sets the entity on fire for the specified number of ticks. If entity is on fire already, makes it burn for either NumTicks or the number of ticks left from the previous fire, whichever is larger.",
				},
				SteerVehicle =
				{
					Params =
					{
						{
							Name = "ForwardAmount",
							Type = "number",
						},
						{
							Name = "SidewaysAmount",
							Type = "number",
						},
					},
					Notes = "Applies the specified steering to the vehicle this entity is attached to. Ignored if not attached to any entity.",
				},
				StopBurning =
				{
					Notes = "Extinguishes the entity fire, cancels all fire timers.",
				},
				TakeDamage =
				{
					{
						Params =
						{
							{
								Name = "AttackerEntity",
								Type = "cEntity",
							},
						},
						Notes = "Causes this entity to take damage that AttackerEntity would inflict. Includes their weapon and this entity's armor.",
					},
					{
						Params =
						{
							{
								Name = "DamageType",
								Type = "eDamageType",
							},
							{
								Name = "AttackerEntity",
								Type = "cEntity",
							},
							{
								Name = "RawDamage",
								Type = "number",
							},
							{
								Name = "KnockbackAmount",
								Type = "number",
							},
						},
						Notes = "Causes this entity to take damage of the specified type, from the specified attacker (may be nil). The final damage is calculated from RawDamage using the currently equipped armor.",
					},
					{
						Params =
						{
							{
								Name = "DamageType",
								Type = "eDamageType",
							},
							{
								Name = "AttackerEntity",
								Type = "cEntity",
							},
							{
								Name = "RawDamage",
								Type = "number",
							},
							{
								Name = "FinalDamage",
								Type = "number",
							},
							{
								Name = "KnockbackAmount",
								Type = "number",
							},
						},
						Notes = "Causes this entity to take damage of the specified type, from the specified attacker (may be nil). The values are wrapped into a {{TakeDamageInfo}} structure and applied directly.",
					},
				},
				TeleportToCoords =
				{
					Params =
					{
						{
							Name = "PosX",
							Type = "number",
						},
						{
							Name = "PosY",
							Type = "number",
						},
						{
							Name = "PosZ",
							Type = "number",
						},
					},
					Notes = "Teleports the entity to the specified coords. Asks plugins if the teleport is allowed.",
				},
				TeleportToEntity =
				{
					Params =
					{
						{
							Name = "DestEntity",
							Type = "cEntity",
						},
					},
					Notes = "Teleports this entity to the specified destination entity. Asks plugins if the teleport is allowed.",
				},
			},
			Constants =
			{
				INVALID_ID =
				{
					Notes = "Special value of an entity ID, indicating a failure. Used primarily in functions that create entities when the entity cannot be created.",
				},
				etBoat =
				{
					Notes = "The entity is a {{cBoat}}",
				},
				etEnderCrystal =
				{
					Notes = "The entity is a {{cEnderCrystal}}",
				},
				etEntity =
				{
					Notes = "No further specialization available",
				},
				etExpOrb =
				{
					Notes = "The entity is a {{cExpOrb}}",
				},
				etFallingBlock =
				{
					Notes = "The entity is a {{cFallingBlock}}",
				},
				etFloater =
				{
					Notes = "The entity is a fishing rod floater",
				},
				etItemFrame =
				{
					Notes = "The entity is an item frame",
				},
				etLeashKnot =
				{
					Notes = "The entity is a leash knot",
				},
				etMinecart =
				{
					Notes = "The entity is a {{cMinecart}} descendant",
				},
				etMob =
				{
					Notes = "The entity is a {{cMonster}} descendant",
				},
				etMonster =
				{
					Notes = "The entity is a {{cMonster}} descendant",
				},
				etPainting =
				{
					Notes = "The entity is a {{cPainting}}",
				},
				etPickup =
				{
					Notes = "The entity is a {{cPickup}}",
				},
				etPlayer =
				{
					Notes = "The entity is a {{cPlayer}}",
				},
				etProjectile =
				{
					Notes = "The entity is a {{cProjectileEntity}} descendant",
				},
				etTNT =
				{
					Notes = "The entity is a {{cTNTEntity}}",
				},
			},
			ConstantGroups =
			{
				eEntityType =
				{
					Include = "et.*",
					TextBefore = "The following constants are used to distinguish between different entity types:",
				},
			},
		},
		cEntityEffect =
		{
			Functions =
			{
				GetPotionColor =
				{
					IsStatic = true,
					Params =
					{
						{
							Name = "ItemDamage",
							Type = "number",
						},
					},
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the potion color (used by the client for visuals), based on the potion's damage value",
				},
				GetPotionEffectDuration =
				{
					IsStatic = true,
					Params =
					{
						{
							Name = "ItemDamage",
							Type = "number",
						},
					},
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the effect duration, in ticks, based on the potion's damage value",
				},
				GetPotionEffectIntensity =
				{
					IsStatic = true,
					Params =
					{
						{
							Name = "ItemDamage",
							Type = "number",
						},
					},
					Returns =
					{
						{
							Name = "short",
							Type = "number",
						},
					},
					Notes = "Retrieves the intensity level from the potion's damage value. Returns 0 for level I potions, 1 for level II potions.",
				},
				GetPotionEffectType =
				{
					IsStatic = true,
					Params =
					{
						{
							Name = "ItemDamage",
							Type = "number",
						},
					},
					Returns =
					{
						{
							Name = "eType",
							Type = "cEntityEffect#eType",
						},
					},
					Notes = "Translates the potion's damage value into the entity effect that the potion gives",
				},
				IsPotionDrinkable =
				{
					IsStatic = true,
					Params =
					{
						{
							Name = "ItemDamage",
							Type = "number",
						},
					},
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Returns true if the potion with the given damage is drinkable",
				},
			},  -- Functions
			ConstantGroups =
			{
				eType =
				{
					Include = { "eff.*" },
				},
			},  -- ConstantGroups
		},  -- cEntityEffect

		cFile =
		{
			Desc = [[
				Provides helper functions for manipulating and querying the filesystem. Most functions are static,
				so they should be called directly on the cFile class itself:
<pre class="prettyprint lang-lua">
cFile:DeleteFile("/usr/bin/virus.exe");
</pre></p>
			]],
			Functions =
			{
				ChangeFileExt =
				{
					IsStatic = true,
					Params =
					{
						{
							Name = "FileName",
							Type = "string",
						},
						{
							Name = "NewExt",
							Type = "string",
						},
					},
					Returns =
					{
						{
							Type = "string",
						},
					},
					Notes = "Returns FileName with its extension changed to NewExt. NewExt may begin with a dot, but needn't, the result is the same in both cases (the first dot, if present, is ignored). FileName may contain path elements, extension is recognized as the last dot after the last path separator in the string.",
				},
				Copy =
				{
					IsStatic = true,
					Params =
					{
						{
							Name = "SrcFileName",
							Type = "string",
						},
						{
							Name = "DstFileName",
							Type = "string",
						},
					},
					Returns =
					{
						{
							Name = "IsSuccess",
							Type = "boolean",
						},
					},
					Notes = "Copies a single file to a new destination. Returns true if successful. Fails if the destination already exists.",
				},
				CreateFolder =
				{
					IsStatic = true,
					Params =
					{
						{
							Name = "FolderPath",
							Type = "string",
						},
					},
					Returns =
					{
						{
							Name = "IsSuccess",
							Type = "boolean",
						},
					},
					Notes = "Creates a new folder. Returns true if successful. Only a single level can be created at a time, use CreateFolderRecursive() to create multiple levels of folders at once.",
				},
				CreateFolderRecursive =
				{
					IsStatic = true,
					Params =
					{
						{
							Name = "FolderPath",
							Type = "string",
						},
					},
					Returns =
					{
						{
							Name = "IsSuccess",
							Type = "boolean",
						},
					},
					Notes = "Creates a new folder, creating its parents if needed. Returns true if successful.",
				},
				Delete =
				{
					IsStatic = true,
					Params =
					{
						{
							Name = "Path",
							Type = "string",
						},
					},
					Returns =
					{
						{
							Name = "IsSuccess",
							Type = "boolean",
						},
					},
					Notes = "Deletes the specified file or folder. Returns true if successful. Only deletes folders that are empty.<br/><b>NOTE</b>: If you already know if the object is a file or folder, use DeleteFile() or DeleteFolder() explicitly.",
				},
				DeleteFile =
				{
					IsStatic = true,
					Params =
					{
						{
							Name = "FilePath",
							Type = "string",
						},
					},
					Returns =
					{
						{
							Name = "IsSuccess",
							Type = "boolean",
						},
					},
					Notes = "Deletes the specified file. Returns true if successful.",
				},
				DeleteFolder =
				{
					IsStatic = true,
					Params =
					{
						{
							Name = "FolderPath",
							Type = "string",
						},
					},
					Returns =
					{
						{
							Name = "IsSuccess",
							Type = "boolean",
						},
					},
					Notes = "Deletes the specified file or folder. Returns true if successful. Only deletes folders that are empty.",
				},
				DeleteFolderContents =
				{
					IsStatic = true,
					Params =
					{
						{
							Name = "FolderPath",
							Type = "string",
						},
					},
					Returns =
					{
						{
							Name = "IsSuccess",
							Type = "boolean",
						},
					},
					Notes = "Deletes everything from the specified folder, recursively. The specified folder stays intact. Returns true if successful.",
				},
				Exists =
				{
					IsStatic = true,
					Params =
					{
						{
							Name = "Path",
							Type = "string",
						},
					},
					Returns =
					{
						{
							Name = "Exists",
							Type = "boolean",
						},
					},
					Notes = "Returns true if the specified file or folder exists.<br/><b>OBSOLETE</b>, use IsFile() or IsFolder() instead",
				},
				GetExecutableExt =
				{
					IsStatic = true,
					Returns =
					{
						{
							Type = "string",
						},
					},
					Notes = "Returns the customary executable extension (including the dot) used by the current platform (\".exe\" on Windows, empty string on Linux). ",
				},
				GetFolderContents =
				{
					IsStatic = true,
					Params =
					{
						{
							Name = "FolderName",
							Type = "string",
						},
					},
					Returns =
					{
						{
							Type = "table",
						},
					},
					Notes = "Returns the contents of the specified folder, as an array table of strings. Each filesystem object is listed. Use the IsFile() and IsFolder() functions to determine the object type. Note that \".\" and \"..\" are NOT returned. The order of the names is arbitrary (as returned by OS, no sorting).",
				},
				GetLastModificationTime =
				{
					IsStatic = true,
					Params =
					{
						{
							Name = "Path",
							Type = "string",
						},
					},
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the last modification time (in current timezone) of the specified file or folder. Returns zero if file not found / not accessible. The returned value is in the same units as values returned by os.time().",
				},
				GetPathSeparator =
				{
					IsStatic = true,
					Returns =
					{
						{
							Type = "string",
						},
					},
					Notes = "Returns the primary path separator used by the current platform. Returns \"\\\" on Windows and \"/\" on Linux. Note that the platform or CRT may support additional path separators, those are not reported.",
				},
				GetSize =
				{
					IsStatic = true,
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
							Type = "number",
						},
					},
					Notes = "Returns the size of the file, or -1 on failure.",
				},
				IsFile =
				{
					IsStatic = true,
					Params =
					{
						{
							Name = "Path",
							Type = "string",
						},
					},
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Returns true if the specified path points to an existing file.",
				},
				IsFolder =
				{
					IsStatic = true,
					Params =
					{
						{
							Name = "Path",
							Type = "string",
						},
					},
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Returns true if the specified path points to an existing folder.",
				},
				ReadWholeFile =
				{
					IsStatic = true,
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
							Type = "string",
						},
					},
					Notes = "Returns the entire contents of the specified file. Returns an empty string if the file cannot be opened.",
				},
				Rename =
				{
					IsStatic = true,
					Params =
					{
						{
							Name = "OrigPath",
							Type = "string",
						},
						{
							Name = "NewPath",
							Type = "string",
						},
					},
					Returns =
					{
						{
							Name = "IsSuccess",
							Type = "boolean",
						},
					},
					Notes = "Renames a file or a folder. Returns true if successful. Undefined result if NewPath already exists.",
				},
			},
		},
		cFloater =
		{
			Desc = [[
				Manages the floater created when a player uses their fishing rod.
			]],
			Functions =
			{
				CanPickup =
				{
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Returns true if the floater gives an item when the player right clicks.",
				},
				GetAttachedMobID =
				{
					Returns =
					{
						{
							Name = "EntityID",
							Type = "number",
						},
					},
					Notes = "Returns the EntityID of a mob that this floater is attached to. Returns -1 if not attached to any mob.",
				},
				GetOwnerID =
				{
					Returns =
					{
						{
							Name = "EntityID",
							Type = "number",
						},
					},
					Notes = "Returns the EntityID of the player who owns the floater.",
				},
				GetBitePos =
				{
					Returns =
					{
							Name = "BitePosition",
							Type = "Vector3d",
					},
					Notes = "Returns the position of the floater just before a fish bites. If a fish hasn't bitten the floater, this function returns the position the floater was cast from.",
				},
			},
			Inherits = "cEntity",
		},
		cHangingEntity =
		{
			Functions =
			{
				GetFacing =
				{
					Returns =
					{
						{
							Name = "BlockFace",
							Type = "eBlockFace",
						},
					},
					Notes = "Returns the direction in which the entity is facing.",
				},
				SetFacing =
				{
					Params =
					{
						{
							Name = "BlockFace",
							Type = "eBlockFace",
						},
					},
					Notes = "Set the direction in which the entity is facing.",
				},
			},
			Inherits = "cEntity",
		},
		cIniFile =
		{
			Desc = [[
				This class implements a simple name-value storage represented on disk by an INI file. These files
				are suitable for low-volume high-latency human-readable information storage, such as for
				configuration. Cuberite itself uses INI files for settings and options.</p>
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
				AddHeaderComment =
				{
					Params =
					{
						{
							Name = "Comment",
							Type = "string",
						},
					},
					Notes = "Adds a comment to be stored in the file header.",
				},
				AddKeyComment =
				{
					{
						Params =
						{
							{
								Name = "KeyID",
								Type = "number",
							},
							{
								Name = "Comment",
								Type = "string",
							},
						},
						Notes = "Adds a comment to be stored in the file under the specified key",
					},
					{
						Params =
						{
							{
								Name = "KeyName",
								Type = "string",
							},
							{
								Name = "Comment",
								Type = "string",
							},
						},
						Notes = "Adds a comment to be stored in the file under the specified key",
					},
				},
				AddKeyName =
				{
					Params =
					{
						{
							Name = "KeyName",
							Type = "string",
						},
					},
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Adds a new key of the specified name. Returns the KeyID of the new key.",
				},
				AddValue =
				{
					Params =
					{
						{
							Name = "KeyName",
							Type = "string",
						},
						{
							Name = "ValueName",
							Type = "string",
						},
						{
							Name = "Value",
							Type = "string",
						},
					},
					Notes = "Adds a new value of the specified name to the specified key. If another value of the same name exists in the key, both are kept (nonstandard INI file)",
				},
				AddValueB =
				{
					Params =
					{
						{
							Name = "KeyName",
							Type = "string",
						},
						{
							Name = "ValueName",
							Type = "string",
						},
						{
							Name = "Value",
							Type = "boolean",
						},
					},
					Notes = "Adds a new bool value of the specified name to the specified key. If another value of the same name exists in the key, both are kept (nonstandard INI file)",
				},
				AddValueF =
				{
					Params =
					{
						{
							Name = "KeyName",
							Type = "string",
						},
						{
							Name = "ValueName",
							Type = "string",
						},
						{
							Name = "Value",
							Type = "number",
						},
					},
					Notes = "Adds a new float value of the specified name to the specified key. If another value of the same name exists in the key, both are kept (nonstandard INI file)",
				},
				AddValueI =
				{
					Params =
					{
						{
							Name = "KeyName",
							Type = "string",
						},
						{
							Name = "ValueName",
							Type = "string",
						},
						{
							Name = "Value",
							Type = "number",
						},
					},
					Notes = "Adds a new integer value of the specified name to the specified key. If another value of the same name exists in the key, both are kept (nonstandard INI file)",
				},
				CaseInsensitive =
				{
					Notes = "Sets key names' and value names' comparisons to case insensitive (default).",
				},
				CaseSensitive =
				{
					Notes = "Sets key names and value names comparisons to case sensitive.",
				},
				Clear =
				{
					Notes = "Removes all the in-memory data. Note that , like all the other operations, this doesn't affect any file data.",
				},
				constructor =
				{
					Returns =
					{
						{
							Type = "cIniFile",
						},
					},
					Notes = "Creates a new empty cIniFile object.",
				},
				DeleteHeaderComment =
				{
					Params =
					{
						{
							Name = "CommentID",
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
					Notes = "Deletes the specified header comment. Returns true if successful.",
				},
				DeleteHeaderComments =
				{
					Notes = "Deletes all headers comments.",
				},
				DeleteKey =
				{
					Params =
					{
						{
							Name = "KeyName",
							Type = "string",
						},
					},
					Returns =
					{
						{
							Name = "IsSuccess",
							Type = "boolean",
						},
					},
					Notes = "Deletes the specified key, and all values in that key. Returns true if successful.",
				},
				DeleteKeyComment =
				{
					{
						Params =
						{
							{
								Name = "KeyID",
								Type = "number",
							},
							{
								Name = "CommentID",
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
						Notes = "Deletes the specified key comment. Returns true if successful.",
					},
					{
						Params =
						{
							{
								Name = "KeyName",
								Type = "string",
							},
							{
								Name = "CommentID",
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
						Notes = "Deletes the specified key comment. Returns true if successful.",
					},
				},
				DeleteKeyComments =
				{
					{
						Params =
						{
							{
								Name = "KeyID",
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
						Notes = "Deletes all comments for the specified key. Returns true if successful.",
					},
					{
						Params =
						{
							{
								Name = "KeyName",
								Type = "string",
							},
						},
						Returns =
						{
							{
								Name = "IsSuccess",
								Type = "boolean",
							},
						},
						Notes = "Deletes all comments for the specified key. Returns true if successful.",
					},
				},
				DeleteValue =
				{
					Params =
					{
						{
							Name = "KeyName",
							Type = "string",
						},
						{
							Name = "ValueName",
							Type = "string",
						},
					},
					Returns =
					{
						{
							Name = "IsSuccess",
							Type = "boolean",
						},
					},
					Notes = "Deletes the specified value. Returns true if successful.",
				},
				DeleteValueByID =
				{
					Params =
					{
						{
							Name = "KeyID",
							Type = "number",
						},
						{
							Name = "ValueID",
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
					Notes = "Deletes the specified value. Returns true if successful.",
				},
				FindKey =
				{
					Params =
					{
						{
							Name = "KeyName",
							Type = "string",
						},
					},
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the KeyID for the specified key name, or the noID constant if the key doesn't exist.",
				},
				FindValue =
				{
					Params =
					{
						{
							Name = "KeyID",
							Type = "number",
						},
						{
							Name = "ValueName",
							Type = "string",
						},
					},
					Returns =
					{
						{
							Name = "ValueID",
							Type = "number",
						},
					},
					Notes = "Returns the ValueID for the specified value name, or the noID constant if the specified key doesn't contain a value of that name.",
				},
				Flush =
				{
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Writes the data stored in the object to the file that was last associated with the object (ReadFile() or WriteFile()). Returns true on success, false on failure.",
				},
				GetHeaderComment =
				{
					Params =
					{
						{
							Name = "CommentID",
							Type = "number",
						},
					},
					Returns =
					{
						{
							Type = "string",
						},
					},
					Notes = "Returns the specified header comment, or an empty string if such comment doesn't exist",
				},
				GetKeyComment =
				{
					{
						Params =
						{
							{
								Name = "KeyID",
								Type = "number",
							},
							{
								Name = "CommentID",
								Type = "number",
							},
						},
						Returns =
						{
							{
								Type = "string",
							},
						},
						Notes = "Returns the specified key comment, or an empty string if such a comment doesn't exist",
					},
					{
						Params =
						{
							{
								Name = "KeyName",
								Type = "string",
							},
							{
								Name = "CommentID",
								Type = "number",
							},
						},
						Returns =
						{
							{
								Type = "string",
							},
						},
						Notes = "Returns the specified key comment, or an empty string if such a comment doesn't exist",
					},
				},
				GetKeyName =
				{
					Params =
					{
						{
							Name = "KeyID",
							Type = "number",
						},
					},
					Returns =
					{
						{
							Type = "string",
						},
					},
					Notes = "Returns the key name for the specified key ID. Inverse for FindKey().",
				},
				GetNumHeaderComments =
				{
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Retuns the number of header comments.",
				},
				GetNumKeyComments =
				{
					{
						Params =
						{
							{
								Name = "KeyID",
								Type = "number",
							},
						},
						Returns =
						{
							{
								Type = "number",
							},
						},
						Notes = "Returns the number of comments under the specified key",
					},
					{
						Params =
						{
							{
								Name = "KeyName",
								Type = "string",
							},
						},
						Returns =
						{
							{
								Type = "number",
							},
						},
						Notes = "Returns the number of comments under the specified key",
					},
				},
				GetNumKeys =
				{
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the total number of keys. This is the range for the KeyID (0 .. GetNumKeys() - 1)",
				},
				GetNumValues =
				{
					{
						Params =
						{
							{
								Name = "KeyID",
								Type = "number",
							},
						},
						Returns =
						{
							{
								Type = "number",
							},
						},
						Notes = "Returns the number of values stored under the specified key.",
					},
					{
						Params =
						{
							{
								Name = "KeyName",
								Type = "string",
							},
						},
						Returns =
						{
							{
								Type = "number",
							},
						},
						Notes = "Returns the number of values stored under the specified key.",
					},
				},
				GetValue =
				{
					{
						Params =
						{
							{
								Name = "KeyName",
								Type = "string",
							},
							{
								Name = "ValueName",
								Type = "string",
							},
							{
								Name = "DefaultValue",
								Type = "string",
								IsOptional = true,
							},
						},
						Returns =
						{
							{
								Type = "string",
							},
						},
						Notes = "Returns the value of the specified name under the specified key. Returns DefaultValue (empty string if not given) if the value doesn't exist.",
					},
					{
						Params =
						{
							{
								Name = "KeyID",
								Type = "number",
							},
							{
								Name = "ValueID",
								Type = "number",
							},
							{
								Name = "DefaultValue",
								Type = "string",
								IsOptional = true,
							},
						},
						Returns =
						{
							{
								Type = "string",
							},
						},
						Notes = "Returns the value of the specified name under the specified key. Returns DefaultValue (empty string if not given) if the value doesn't exist.",
					},
				},
				GetValueB =
				{
					Params =
					{
						{
							Name = "KeyName",
							Type = "string",
						},
						{
							Name = "ValueName",
							Type = "string",
						},
						{
							Name = "DefaultValue",
							Type = "boolean",
							IsOptional = true,
						},
					},
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Returns the value of the specified name under the specified key, as a bool. Returns DefaultValue (false if not given) if the value doesn't exist.",
				},
				GetValueF =
				{
					Params =
					{
						{
							Name = "KeyName",
							Type = "string",
						},
						{
							Name = "ValueName",
							Type = "string",
						},
						{
							Name = "DefaultValue",
							Type = "number",
							IsOptional = true,
						},
					},
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the value of the specified name under the specified key, as a floating-point number. Returns DefaultValue (zero if not given) if the value doesn't exist.",
				},
				GetValueI =
				{
					Params =
					{
						{
							Name = "KeyName",
							Type = "string",
						},
						{
							Name = "ValueName",
							Type = "string",
						},
						{
							Name = "DefaultValue",
							Type = "number",
							IsOptional = true,
						},
					},
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the value of the specified name under the specified key, as an integer. Returns DefaultValue (zero if not given) if the value doesn't exist.",
				},
				GetValueName =
				{
					{
						Params =
						{
							{
								Name = "KeyID",
								Type = "number",
							},
							{
								Name = "ValueID",
								Type = "number",
							},
						},
						Returns =
						{
							{
								Type = "string",
							},
						},
						Notes = "Returns the name of the value specified by its ID. Inverse for FindValue().",
					},
					{
						Params =
						{
							{
								Name = "KeyName",
								Type = "string",
							},
							{
								Name = "ValueID",
								Type = "number",
							},
						},
						Returns =
						{
							{
								Type = "string",
							},
						},
						Notes = "Returns the name of the value specified by its ID. Inverse for FindValue().",
					},
				},
				GetValueSet =
				{
					Params =
					{
						{
							Name = "KeyName",
							Type = "string",
						},
						{
							Name = "ValueName",
							Type = "string",
						},
						{
							Name = "DefaultValue",
							Type = "string",
							IsOptional = true,
						},
					},
					Returns =
					{
						{
							Type = "string",
						},
					},
					Notes = "Returns the value of the specified name under the specified key. If the value doesn't exist, creates it with the specified default (empty string if not given).",
				},
				GetValueSetB =
				{
					Params =
					{
						{
							Name = "KeyName",
							Type = "string",
						},
						{
							Name = "ValueName",
							Type = "string",
						},
						{
							Name = "DefaultValue",
							Type = "boolean",
							IsOptional = true,
						},
					},
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Returns the value of the specified name under the specified key, as a bool. If the value doesn't exist, creates it with the specified default (false if not given).",
				},
				GetValueSetF =
				{
					Params =
					{
						{
							Name = "KeyName",
							Type = "string",
						},
						{
							Name = "ValueName",
							Type = "string",
						},
						{
							Name = "DefaultValue",
							Type = "number",
							IsOptional = true,
						},
					},
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the value of the specified name under the specified key, as a floating-point number. If the value doesn't exist, creates it with the specified default (zero if not given).",
				},
				GetValueSetI =
				{
					Params =
					{
						{
							Name = "KeyName",
							Type = "string",
						},
						{
							Name = "ValueName",
							Type = "string",
						},
						{
							Name = "DefaultValue",
							Type = "number",
							IsOptional = true,
						},
					},
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the value of the specified name under the specified key, as an integer. If the value doesn't exist, creates it with the specified default (zero if not given).",
				},
				HasValue =
				{
					Params =
					{
						{
							Name = "KeyName",
							Type = "string",
						},
						{
							Name = "ValueName",
							Type = "string",
						},
					},
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Returns true if the specified value is present.",
				},
				ReadFile =
				{
					Params =
					{
						{
							Name = "FileName",
							Type = "string",
						},
						{
							Name = "AllowExampleFallback",
							Type = "boolean",
							IsOptional = true,
						},
					},
					Returns =
					{
						{
							Name = "IsSuccess",
							Type = "boolean",
						},
					},
					Notes = "Reads the values from the specified file. Previous in-memory contents are lost. If the file cannot be opened, and AllowExample is true, another file, \"filename.example.ini\", is loaded and then saved as \"filename.ini\". Returns true if successful, false if not.",
				},
				SetValue =
				{
					{
						Params =
						{
							{
								Name = "KeyID",
								Type = "number",
							},
							{
								Name = "ValueID",
								Type = "number",
							},
							{
								Name = "NewValue",
								Type = "string",
							},
						},
						Returns =
						{
							{
								Name = "IsSuccess",
								Type = "boolean",
							},
						},
						Notes = "Overwrites the specified value with a new value. If the specified value doesn't exist, returns false (doesn't add).",
					},
					{
						Params =
						{
							{
								Name = "KeyName",
								Type = "string",
							},
							{
								Name = "ValueName",
								Type = "string",
							},
							{
								Name = "NewValue",
								Type = "string",
							},
							{
								Name = "CreateIfNotExists",
								Type = "boolean",
								IsOptional = true,
							},
						},
						Returns =
						{
							{
								Name = "IsSuccess",
								Type = "boolean",
							},
						},
						Notes = "Overwrites the specified value with a new value. If CreateIfNotExists is true (default) and the value doesn't exist, it is first created. Returns true if the value was successfully set, false if not (didn't exists, CreateIfNotExists false).",
					},
				},
				SetValueB =
				{
					Params =
					{
						{
							Name = "KeyName",
							Type = "string",
						},
						{
							Name = "ValueName",
							Type = "string",
						},
						{
							Name = "NewValue",
							Type = "boolean",
						},
						{
							Name = "CreateIfNotExists",
							Type = "boolean",
							IsOptional = true,
						},
					},
					Returns =
					{
						{
							Name = "IsSuccess",
							Type = "boolean",
						},
					},
					Notes = "Overwrites the specified value with a new bool value. If CreateIfNotExists is true (default) and the value doesn't exist, it is first created. Returns true if the value was successfully set, false if not (didn't exists, CreateIfNotExists false).",
				},
				SetValueF =
				{
					Params =
					{
						{
							Name = "KeyName",
							Type = "string",
						},
						{
							Name = "ValueName",
							Type = "string",
						},
						{
							Name = "NewValue",
							Type = "number",
						},
						{
							Name = "CreateIfNotExists",
							Type = "boolean",
							IsOptional = true,
						},
					},
					Returns =
					{
						{
							Name = "IsSuccess",
							Type = "boolean",
						},
					},
					Notes = "Overwrites the specified value with a new floating-point number value. If CreateIfNotExists is true (default) and the value doesn't exist, it is first created. Returns true if the value was successfully set, false if not (didn't exists, CreateIfNotExists false).",
				},
				SetValueI =
				{
					Params =
					{
						{
							Name = "KeyName",
							Type = "string",
						},
						{
							Name = "ValueName",
							Type = "string",
						},
						{
							Name = "NewValue",
							Type = "number",
						},
						{
							Name = "CreateIfNotExists",
							Type = "boolean",
							IsOptional = true,
						},
					},
					Returns =
					{
						{
							Name = "IsSuccess",
							Type = "boolean",
						},
					},
					Notes = "Overwrites the specified value with a new integer value. If CreateIfNotExists is true (default) and the value doesn't exist, it is first created. Returns true if the value was successfully set, false if not (didn't exists, CreateIfNotExists false).",
				},
				WriteFile =
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
							Name = "IsSuccess",
							Type = "boolean",
						},
					},
					Notes = "Writes the current in-memory data into the specified file. Returns true if successful, false if not.",
				},
			},
			Constants =
			{
				noID =
				{
					Notes = "",
				},
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
for k = 0, (NumKeys - 1) do
	local NumValues = IniFile:GetNumValues(k);
	LOG("key \"" .. IniFile:GetKeyName(k) .. "\" has " .. NumValues .. " values:");
	for v = 0, (NumValues - 1) do
		LOG("  value \"" .. IniFile:GetValueName(k, v) .. "\".");
	end
end
</pre>
					]],
				},
			},
		},
		cInventory =
		{
			Desc = [[
This object is used to store the items that a {{cPlayer|cPlayer}} has. It also keeps track of what item the player has currently selected in their hotbar.
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
				AddItem =
				{
					Params =
					{
						{
							Name = "cItem",
							Type = "cItem",
						},
						{
							Name = "AllowNewStacks",
							Type = "boolean",
							IsOptional = true,
						},
					},
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Adds an item to the storage; if AllowNewStacks is true (default), will also create new stacks in empty slots. Fills existing stacks first and fills the hotbar before the main inventory. Returns the number of items added",
				},
				AddItems =
				{
					Params =
					{
						{
							Name = "cItems",
							Type = "cItems",
						},
						{
							Name = "AllowNewStacks",
							Type = "boolean",
							IsOptional = true,
						},
					},
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Same as AddItem, but for several items at once",
				},
				ChangeSlotCount =
				{
					Params =
					{
						{
							Name = "SlotNum",
							Type = "number",
						},
						{
							Name = "AddToCount",
							Type = "number",
						},
					},
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Adds AddToCount to the count of items in the specified slot. If the slot was empty, ignores the call. Returns the new count in the slot, or -1 if invalid SlotNum",
				},
				Clear =
				{
					Notes = "Empties all slots",
				},
				CopyToItems =
				{
					Params =
					{
						{
							Name = "cItems",
							Type = "cItems",
						},
					},
					Notes = "Copies all non-empty slots into the cItems object provided; original cItems contents are preserved",
				},
				DamageEquippedItem =
				{
					Params =
					{
						{
							Name = "DamageAmount",
							Type = "number",
							IsOptional = true,
						},
					},
					Returns =
					{
						{
							Name = "HasDestroyed",
							Type = "boolean",
						},
					},
					Notes = "Adds the specified damage (1 by default) to the currently equipped item. Removes the item and returns true if the item reached its max damage and was destroyed.",
				},
				DamageItem =
				{
					Params =
					{
						{
							Name = "SlotNum",
							Type = "number",
						},
						{
							Name = "DamageAmount",
							Type = "number",
							IsOptional = true,
						},
					},
					Returns =
					{
						{
							Name = "HasDestroyed",
							Type = "boolean",
						},
					},
					Notes = "Adds the specified damage (1 by default) to the specified item. Removes the item and returns true if the item reached its max damage and was destroyed.",
				},
				FindItem =
				{
					Params =
					{
						{
							Name = "RecipeItem",
							Type = "cItem",
						},
					},
					Returns =
					{
						{
							Type = "cItem",
						},
					},
					Notes = "Finds an item in the shield, hotbar and inventory slots matching `ItemType` and `ItemDamage`. The actual item is returned, if none is found `nullptr`. This can be used to validate that the player has a specific type of item.",
				},
				GetArmorGrid =
				{
					Returns =
					{
						{
							Type = "cItemGrid",
						},
					},
					Notes = "Returns the ItemGrid representing the armor grid (1 x 4 slots)",
				},
				GetArmorSlot =
				{
					Params =
					{
						{
							Name = "ArmorSlotNum",
							Type = "number",
						},
					},
					Returns =
					{
						{
							Type = "cItem",
						},
					},
					Notes = "Returns the specified armor slot contents. Note that the returned item is read-only",
				},
				GetEquippedBoots =
				{
					Returns =
					{
						{
							Type = "cItem",
						},
					},
					Notes = "Returns the item in the \"boots\" slot of the armor grid. Note that the returned item is read-only",
				},
				GetEquippedChestplate =
				{
					Returns =
					{
						{
							Type = "cItem",
						},
					},
					Notes = "Returns the item in the \"chestplate\" slot of the armor grid. Note that the returned item is read-only",
				},
				GetEquippedHelmet =
				{
					Returns =
					{
						{
							Type = "cItem",
						},
					},
					Notes = "Returns the item in the \"helmet\" slot of the armor grid. Note that the returned item is read-only",
				},
				GetEquippedItem =
				{
					Returns =
					{
						{
							Type = "cItem",
						},
					},
					Notes = "Returns the currently selected item from the hotbar. Note that the returned item is read-only. Also note that the returned item is bound to the actual inventory slot - if a player moves another item into the slot, this object will update to the new item. Use a {{cItem}} constructor to make a copy if you need to store the contents of the slot.",
				},
				GetEquippedLeggings =
				{
					Returns =
					{
						{
							Type = "cItem",
						},
					},
					Notes = "Returns the item in the \"leggings\" slot of the armor grid. Note that the returned item is read-only",
				},
				GetEquippedSlotNum =
				{
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the hotbar slot number for the currently selected item",
				},
				GetHotbarGrid =
				{
					Returns =
					{
						{
							Type = "cItemGrid",
						},
					},
					Notes = "Returns the ItemGrid representing the hotbar grid (9 x 1 slots)",
				},
				GetHotbarSlot =
				{
					Params =
					{
						{
							Name = "HotBarSlotNum",
							Type = "number",
						},
					},
					Returns =
					{
						{
							Type = "cItem",
						},
					},
					Notes = "Returns the specified hotbar slot contents. Note that the returned item is read-only",
				},
				GetShieldSlot =
				{
					Returns =
					{
						{
							Type = "cItem",
						},
					},
					Notes = "Returns current item in shield slot.",
				},
				GetInventoryGrid =
				{
					Returns =
					{
						{
							Type = "cItemGrid",
						},
					},
					Notes = "Returns the ItemGrid representing the main inventory (9 x 3 slots)",
				},
				GetInventorySlot =
				{
					Params =
					{
						{
							Name = "InventorySlotNum",
							Type = "number",
						},
					},
					Returns =
					{
						{
							Type = "cItem",
						},
					},
					Notes = "Returns the specified main inventory slot contents. Note that the returned item is read-only",
				},
				GetOwner =
				{
					Returns =
					{
						{
							Type = "cPlayer",
						},
					},
					Notes = "Returns the player whose inventory this object represents",
				},
				GetSlot =
				{
					Params =
					{
						{
							Name = "SlotNum",
							Type = "number",
						},
					},
					Returns =
					{
						{
							Type = "cItem",
						},
					},
					Notes = "Returns the contents of the specified slot. Note that the returned item is read-only",
				},
				HasItems =
				{
					Params =
					{
						{
							Name = "cItem",
							Type = "cItem",
						},
					},
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Returns true if there are at least as many items of the specified type as in the parameter",
				},
				HowManyCanFit =
				{
					{
						Params =
						{
							{
								Name = "ItemStack",
								Type = "cItem",
							},
							{
								Name = "AllowNewStacks",
								Type = "boolean",
								IsOptional = true,
							},
						},
						Returns =
						{
							{
								Type = "number",
							},
						},
						Notes = "Returns number of items out of a_ItemStack that can fit in the storage. If AllowNewStacks is false, only considers slots already containing the specified item. AllowNewStacks defaults to true if not given.",
					},
					{
						Params =
						{
							{
								Name = "ItemStack",
								Type = "cItem",
							},
							{
								Name = "BeginSlotNum",
								Type = "number",
							},
							{
								Name = "EndSlotNum",
								Type = "number",
							},
							{
								Name = "AllowNewStacks",
								Type = "boolean",
								IsOptional = true,
							},
						},
						Returns =
						{
							{
								Type = "number",
							},
						},
						Notes = "Returns how many items of the specified type would fit into the slot range specified. If AllowNewStacks is false, only considers slots already containing the specified item. AllowNewStacks defaults to true if not given.",
					},
				},
				HowManyItems =
				{
					Params =
					{
						{
							Name = "cItem",
							Type = "cItem",
						},
					},
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the number of the specified items that are currently stored",
				},
				RemoveItem =
				{
					Params =
					{
						{
							Name = "cItem",
							Type = "cItem",
						},
					},
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Removes the specified item from the inventory, as many as possible, up to the item's m_ItemCount. Returns the number of items that were removed.",
				},
				RemoveOneEquippedItem =
				{
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Removes one item from the hotbar's currently selected slot. Returns true on success.",
				},
				ReplaceOneEquippedItem =
				{
					Params =
					{
						{
							Name = "Item",
							Type = "cItem",
						},
						{
							Name = "TryOtherSlots",
							Type = "boolean",
							IsOptional = true,
						},
					},
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Removes one item from the the current equipped item stack, and attempts to add the specified item stack back to the same slot. If it is not possible to place the item in the same slot, optionally (default true) tries to place the specified item elsewhere in the inventory. Returns the number of items successfully added. If the currently equipped slot is empty, its contents are simply set to the given Item.",
				},
				SendEquippedSlot =
				{
					Notes = "Sends the equipped item slot to the client",
				},
				SetArmorSlot =
				{
					Params =
					{
						{
							Name = "ArmorSlotNum",
							Type = "number",
						},
						{
							Name = "cItem",
							Type = "cItem",
						},
					},
					Notes = "Sets the specified armor slot contents",
				},
				SetEquippedSlotNum =
				{
					Params =
					{
						{
							Name = "EquippedSlotNum",
							Type = "number",
						},
					},
					Notes = "Sets the currently selected hotbar slot number",
				},
				SetHotbarSlot =
				{
					Params =
					{
						{
							Name = "HotbarSlotNum",
							Type = "number",
						},
						{
							Name = "cItem",
							Type = "cItem",
						},
					},
					Notes = "Sets the specified hotbar slot contents",
				},
				SetInventorySlot =
				{
					Params =
					{
						{
							Name = "InventorySlotNum",
							Type = "number",
						},
						{
							Name = "cItem",
							Type = "cItem",
						},
					},
					Notes = "Sets the specified main inventory slot contents",
				},
				SetShieldSlot =
				{
					Params =
					{
						{
							Name = "Item",
							Type = "cItem",
						},
					},
					Notes = "Sets the shield slot content",
				},
				SetSlot =
				{
					Params =
					{
						{
							Name = "SlotNum",
							Type = "number",
						},
						{
							Name = "cItem",
							Type = "cItem",
						},
					},
					Notes = "Sets the specified slot contents",
				},
				SetEquippedItem =
				{
					Params =
					{
						{
							Name = "Item",
							Type = "cItem",
						},
					},
					Notes = "Sets current item in the equipped hotbar slot",
				},
			},
			Constants =
			{
				invArmorCount =
				{
					Notes = "Number of slots in the Armor part",
				},
				invArmorOffset =
				{
					Notes = "Starting slot number of the Armor part",
				},
				invHotbarCount =
				{
					Notes = "Number of slots in the Hotbar part",
				},
				invHotbarOffset =
				{
					Notes = "Starting slot number of the Hotbar part",
				},
				invInventoryCount =
				{
					Notes = "Number of slots in the main inventory part",
				},
				invInventoryOffset =
				{
					Notes = "Starting slot number of the main inventory part",
				},
				invShieldCount =
				{
					Notes = "Number of slots in the Shield part",
				},
				invShieldOffset =
				{
					Notes = "Starting slot number of the Shield part",
				},
				invNumSlots =
				{
					Notes = "Total number of slots in a cInventory",
				},
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
		},
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
				AddCount =
				{
					Params =
					{
						{
							Name = "AmountToAdd",
							Type = "number",
						},
					},
					Returns =
					{
						{
							Type = "cItem",
						},
					},
					Notes = "Adds the specified amount to the item count. Returns self (useful for chaining).",
				},
				AddEnchantment =
				{
					Params =
					{
						{
							Name = "Enchantment ID",
							Type = "number",
						},
						{
							Name = "Level",
							Type = "number",
						},
						{
							Name = "FromBook",
							Type = "boolean",
						},
					},
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Adds the given enchantment at the given level to this item, following anvil enchantment combining rules. Returns the XP level cost of the addition. FromBook specifies whether to use the XP multiplier for books or the multiplier used for other items, if true it uses the multiplier for books.",
				},
				AddEnchantmentsFromItem =
				{
					Params =
					{
						{
							Name = "Additive",
							Type = "cItem",
						},
					},
					Returns =
					{
						{
							Name = "LevelCost",
							Type = "number",
						},
					},
					Notes = "Adds the enchantments from the specified item to this item, returning the cost as if this were an anvil.",
				},
				Clear =
				{
					Notes = "Resets the instance to an empty item",
				},
				constructor =
				{
					{
						Returns =
						{
							{
								Type = "cItem",
							},
						},
						Notes = "Creates a new empty cItem object",
					},
					{
						Params =
						{
							{
								Name = "ItemType",
								Type = "number",
							},
							{
								Name = "Count",
								Type = "number",
								IsOptional = true,
							},
							{
								Name = "Damage",
								Type = "number",
								IsOptional = true,
							},
							{
								Name = "EnchantmentString",
								Type = "string",
								IsOptional = true,
							},
							{
								Name = "CustomName",
								Type = "string",
								IsOptional = true,
							},
							{
								Name = "Lore",
								Type = "table",
								IsOptional = true,
							},
						},
						Returns =
						{
							{
								Type = "cItem",
							},
						},
						Notes = "Creates a new cItem object of the specified type, count (1 by default), damage (0 by default), enchantments (non-enchanted by default), CustomName (empty by default) and Lore (string, empty by default)",
					},
					{
						Params =
						{
							{
								Name = "cItem",
								Type = "cItem",
							},
						},
						Returns =
						{
							{
								Type = "cItem",
							},
						},
						Notes = "Creates an exact copy of the cItem object in the parameter",
					},
				},
				CopyOne =
				{
					Returns =
					{
						{
							Type = "cItem",
						},
					},
					Notes = "Creates a copy of this object, with its count set to 1",
				},
				DamageItem =
				{
					Params =
					{
						{
							Name = "Amount",
							Type = "number",
							IsOptional = true,
						},
					},
					Returns =
					{
						{
							Name = "HasReachedMaxDamage",
							Type = "boolean",
						},
					},
					Notes = "Adds the specified damage. Returns true when damage reaches max value and the item should be destroyed (but doesn't destroy the item)",
				},
				Empty =
				{
					Notes = "Resets the instance to an empty item",
				},
				EnchantByXPLevels =
				{
					Params =
					{
						{
							Name = "NumXPLevels",
							Type = "number",
						},
					},
					Returns =
					{
						{
							Name = "HasEnchanted",
							Type = "boolean",
						},
					},
					Notes = "Randomly enchants the item using the specified number of XP levels. Returns true if the item was enchanted, false if not (not enchantable / too many enchantments already).",
				},
				GetEnchantability =
				{
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the enchantability of the item. Returns zero if the item doesn't have enchantability.",
				},
				GetMaxDamage =
				{
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the maximum value for damage that this item can get before breaking; zero if damage is not accounted for for this item type",
				},
				GetMaxStackSize =
				{
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the maximum stack size for this item.",
				},
				IsBothNameAndLoreEmpty =
				{
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Returns if both the custom name and lore are not set.",
				},
				IsCustomNameEmpty =
				{
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Returns if the custom name is empty.",
				},
				IsDamageable =
				{
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Returns true if this item does account for its damage",
				},
				IsEmpty =
				{
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Returns true if this object represents an empty item (zero count or invalid ItemType)",
				},
				IsEnchantable =
				{
					IsStatic = true,
					Params =
					{
						{
							Name = "ItemType",
							Type = "number",
						},
						{
							Name = "FromBook",
							Type = "boolean",
						},
					},
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Returns true if the specified item type is enchantable. If FromBook is true, the function is used in the anvil inventory with book enchantments. So it checks the \"only book enchantments\" too. Example: You can only enchant a hoe with a book.",
				},
				IsEqual =
				{
					Params =
					{
						{
							Name = "OtherItem",
							Type = "cItem",
						},
					},
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Returns true if the item in the parameter is the same as the one stored in the object (type, damage, lore, name and enchantments)",
				},
				IsFullStack =
				{
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Returns true if the item is stacked up to its maximum stacking",
				},
				IsLoreEmpty =
				{
					Notes = "Returns if the lore of the cItem is empty.",
					Returns =
					{
						{
							Type = "boolean",
						},
					},
				},
				IsSameType =
				{
					Params =
					{
						{
							Name = "OtherItem",
							Type = "cItem",
						},
					},
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Returns true if the item in the parameter is of the same ItemType as the one stored in the object. This is true even if the two items have different enchantments",
				},
			},
			Variables =
			{
				m_CustomName =
				{
					Type = "string",
					Notes = "The custom name for an item.",
				},
				m_Enchantments =
				{
					Type = "{{cEnchantments|cEnchantments}}}",
					Notes = "The enchantments of the item.",
				},
				m_ItemCount =
				{
					Type = "number",
					Notes = "Number of items in this stack",
				},
				m_ItemDamage =
				{
					Type = "number",
					Notes = "The damage of the item. Zero means no damage. Maximum damage can be queried with GetMaxDamage()",
				},
				m_ItemType =
				{
					Type = "number",
					Notes = "The item type. One of E_ITEM_ or E_BLOCK_ constants",
				},
				m_LoreTable =
				{
					Type = "table",
					Notes = "The lore for an item. Represented as an array table of lines.",
				},
				m_RepairCost =
				{
					Type = "number",
					Notes = "The repair cost of the item. The anvil need this value",
				},
			},
			AdditionalInfo =
			{
				{
					Header = "Usage notes",
					Contents = [[
						Note that the object contained in a cItem class is quite complex and quite often new Minecraft
						versions add more stuff. Therefore it is recommended to copy cItem objects using the
						copy-constructor ("local copy = cItem(original);"), this is the only way that guarantees that
						the object will be copied at full, even with future versions of Cuberite.
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
		},
		cItemFrame =
		{
			Functions =
			{
				GetItem =
				{
					Returns =
					{
						{
							Type = "cItem",
							IsConst = true,
						},
					},
					Notes = "Returns the item in the frame (readonly object, do not modify)",
				},
				GetItemRotation =
				{
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the rotation from the item in the frame",
				},
				SetItem =
				{
					Params =
					{
						{
							Name = "Item",
							Type = "cItem",
						},
					},
					Notes = "Set the item in the frame",
				},
				SetItemRotation =
				{
					Params =
					{
						{
							Name = "ItemRotation",
							Type = "number",
						},
					},
					Notes = "Set the rotation from the item in the frame",
				},
			},
		},
		cItemGrid =
		{
			Desc = [[
This class represents a 2D array of items. It is used as the underlying storage and API for all cases that use a grid of items:
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
				AddItem =
				{
					Params =
					{
						{
							Name = "ItemStack",
							Type = "cItem",
						},
						{
							Name = "AllowNewStacks",
							Type = "boolean",
							IsOptional = true,
						},
						{
							Name = "PrioritySlot",
							Type = "number",
							IsOptional = true,
						},
					},
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Adds as many items out of ItemStack as can fit. If AllowNewStacks is set to false, only existing stacks can be topped up. If AllowNewStacks is set to true (default), empty slots can be used for the rest. If PrioritySlot is set to a non-negative value, then the corresponding slot will be used first (if empty or compatible with added items). If PrioritySlot is set to -1 (default), regular order applies. Returns the number of items that fit.",
				},
				AddItems =
				{
					Params =
					{
						{
							Name = "ItemStackList",
							Type = "cItems",
						},
						{
							Name = "AllowNewStacks",
							Type = "boolean",
							IsOptional = true,
						},
						{
							Name = "PrioritySlot",
							Type = "number",
							IsOptional = true,
						},
					},
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Same as AddItem, but works on an entire list of item stacks. The a_ItemStackList is modified to reflect the leftover items. If a_AllowNewStacks is set to false, only existing stacks can be topped up. If AllowNewStacks is set to true, empty slots can be used for the rest. If PrioritySlot is set to a non-negative value, then the corresponding slot will be used first (if empty or compatible with added items). If PrioritySlot is set to -1 (default), regular order applies. Returns the total number of items that fit.",
				},
				ChangeSlotCount =
				{
					{
						Params =
						{
							{
								Name = "SlotNum",
								Type = "number",
							},
							{
								Name = "AddToCount",
								Type = "number",
							},
						},
						Returns =
						{
							{
								Type = "number",
							},
						},
						Notes = "Adds AddToCount to the count of items in the specified slot. If the slot was empty, ignores the call. Returns the new count in the slot, or -1 if invalid SlotNum",
					},
					{
						Params =
						{
							{
								Name = "X",
								Type = "number",
							},
							{
								Name = "Y",
								Type = "number",
							},
							{
								Name = "AddToCount",
								Type = "number",
							},
						},
						Returns =
						{
							{
								Type = "number",
							},
						},
						Notes = "Adds AddToCount to the count of items in the specified slot. If the slot was empty, ignores the call. Returns the new count in the slot, or -1 if invalid slot coords",
					},
				},
				Clear =
				{
					Notes = "Empties all slots",
				},
				CopyToItems =
				{
					Params =
					{
						{
							Name = "cItems",
							Type = "cItems",
						},
					},
					Notes = "Copies all non-empty slots into the {{cItems}} object provided; original cItems contents are preserved as well.",
				},
				DamageItem =
				{
					{
						Params =
						{
							{
								Name = "SlotNum",
								Type = "number",
							},
							{
								Name = "DamageAmount",
								Type = "number",
								IsOptional = true,
							},
						},
						Returns =
						{
							{
								Name = "HasReachedMaxDamage",
								Type = "boolean",
							},
						},
						Notes = "Adds the specified damage (1 by default) to the specified item, returns true if the item reached its max damage and should be destroyed (but doesn't destroy the item).",
					},
					{
						Params =
						{
							{
								Name = "X",
								Type = "number",
							},
							{
								Name = "Y",
								Type = "number",
							},
							{
								Name = "DamageAmount",
								Type = "number",
								IsOptional = true,
							},
						},
						Returns =
						{
							{
								Name = "HasReachedMaxDamage",
								Type = "boolean",
							},
						},
						Notes = "Adds the specified damage (1 by default) to the specified item, returns true if the item reached its max damage and should be destroyed (but doesn't destroy the item).",
					},
				},
				EmptySlot =
				{
					{
						Params =
						{
							{
								Name = "SlotNum",
								Type = "number",
							},
						},
						Notes = "Destroys the item in the specified slot",
					},
					{
						Params =
						{
							{
								Name = "X",
								Type = "number",
							},
							{
								Name = "Y",
								Type = "number",
							},
						},
						Notes = "Destroys the item in the specified slot",
					},
				},
				FindItem =
				{
					Params =
					{
						{
							Name = "RecipeItem",
							Type = "cItem",
						},
					},
					Returns =
					{
						{
							Type = "cItem",
						},
					},
					Notes = "Finds an item within the grid matching `ItemType` and `ItemDamage`. The actual item is returned, if none is found `nullptr`.",
				},
				GetFirstEmptySlot =
				{
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the SlotNumber of the first empty slot, -1 if all slots are full",
				},
				GetFirstUsedSlot =
				{
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the SlotNumber of the first non-empty slot, -1 if all slots are empty",
				},
				GetHeight =
				{
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the Y dimension of the grid",
				},
				GetLastEmptySlot =
				{
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the SlotNumber of the last empty slot, -1 if all slots are full",
				},
				GetLastUsedSlot =
				{
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the SlotNumber of the last non-empty slot, -1 if all slots are empty",
				},
				GetNextEmptySlot =
				{
					Params =
					{
						{
							Name = "StartFrom",
							Type = "number",
						},
					},
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the SlotNumber of the first empty slot following StartFrom, -1 if all the following slots are full",
				},
				GetNextUsedSlot =
				{
					Params =
					{
						{
							Name = "StartFrom",
							Type = "number",
						},
					},
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the SlotNumber of the first non-empty slot following StartFrom, -1 if all the following slots are full",
				},
				GetNumSlots =
				{
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the total number of slots in the grid (Width * Height)",
				},
				GetSlot =
				{
					{
						Params =
						{
							{
								Name = "SlotNumber",
								Type = "number",
							},
						},
						Returns =
						{
							{
								Type = "cItem",
							},
						},
						Notes = "Returns the item in the specified slot. Note that the item is read-only",
					},
					{
						Params =
						{
							{
								Name = "X",
								Type = "number",
							},
							{
								Name = "Y",
								Type = "number",
							},
						},
						Returns =
						{
							{
								Type = "cItem",
							},
						},
						Notes = "Returns the item in the specified slot. Note that the item is read-only",
					},
				},
				GetSlotCoords =
				{
					Params =
					{
						{
							Name = "SlotNum",
							Type = "number",
						},
					},
					Returns =
					{
						{
							Type = "number",
						},
						{
							Type = "number",
						},
					},
					Notes = "Returns the X and Y coords for the specified SlotNumber. Returns \"-1, -1\" on invalid SlotNumber",
				},
				GetSlotNum =
				{
					Params =
					{
						{
							Name = "X",
							Type = "number",
						},
						{
							Name = "Y",
							Type = "number",
						},
					},
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the SlotNumber for the specified slot coords. Returns -1 on invalid coords",
				},
				GetWidth =
				{
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the X dimension of the grid",
				},
				HasItems =
				{
					Params =
					{
						{
							Name = "ItemStack",
							Type = "cItem",
						},
					},
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Returns true if there are at least as many items of the specified type as in the parameter",
				},
				HowManyCanFit =
				{
					Params =
					{
						{
							Name = "ItemStack",
							Type = "cItem",
						},
						{
							Name = "AllowNewStacks",
							Type = "boolean",
							IsOptional = true,
						},
					},
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the number of the specified items that can fit in the storage. If AllowNewStacks is true (default), includes empty slots in the returned count.",
				},
				HowManyItems =
				{
					Params =
					{
						{
							Name = "Item",
							Type = "cItem",
						},
					},
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the number of the specified item that are currently stored",
				},
				IsSlotEmpty =
				{
					{
						Params =
						{
							{
								Name = "SlotNum",
								Type = "number",
							},
						},
						Returns =
						{
							{
								Type = "boolean",
							},
						},
						Notes = "Returns true if the specified slot is empty, or an invalid slot is specified",
					},
					{
						Params =
						{
							{
								Name = "X",
								Type = "number",
							},
							{
								Name = "Y",
								Type = "number",
							},
						},
						Returns =
						{
							{
								Type = "boolean",
							},
						},
						Notes = "Returns true if the specified slot is empty, or an invalid slot is specified",
					},
				},
				RemoveItem =
				{
					Params =
					{
						{
							Name = "ItemStack",
							Type = "cItem",
						},
					},
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Removes the specified item from the grid, as many as possible, up to ItemStack's m_ItemCount. Returns the number of items that were removed.",
				},
				RemoveOneItem =
				{
					{
						Params =
						{
							{
								Name = "SlotNum",
								Type = "number",
							},
						},
						Returns =
						{
							{
								Type = "cItem",
							},
						},
						Notes = "Removes one item from the stack in the specified slot and returns it as a single cItem. Empty slots are skipped and an empty item is returned",
					},
					{
						Params =
						{
							{
								Name = "X",
								Type = "number",
							},
							{
								Name = "Y",
								Type = "number",
							},
						},
						Returns =
						{
							{
								Type = "cItem",
							},
						},
						Notes = "Removes one item from the stack in the specified slot and returns it as a single cItem. Empty slots are skipped and an empty item is returned",
					},
				},
				SetSlot =
				{
					{
						Params =
						{
							{
								Name = "SlotNum",
								Type = "number",
							},
							{
								Name = "cItem",
								Type = "cItem",
							},
						},
						Notes = "Sets the specified slot to the specified item",
					},
					{
						Params =
						{
							{
								Name = "SlotNum",
								Type = "number",
							},
							{
								Name = "ItemType",
								Type = "number",
							},
							{
								Name = "ItemCount",
								Type = "number",
							},
							{
								Name = "ItemDamage",
								Type = "number",
							},
						},
						Notes = "Sets the specified slot to the specified item",
					},
					{
						Params =
						{
							{
								Name = "X",
								Type = "number",
							},
							{
								Name = "Y",
								Type = "number",
							},
							{
								Name = "cItem",
								Type = "cItem",
							},
						},
						Notes = "Sets the specified slot to the specified item",
					},
					{
						Params =
						{
							{
								Name = "X",
								Type = "number",
							},
							{
								Name = "Y",
								Type = "number",
							},
							{
								Name = "ItemType",
								Type = "number",
							},
							{
								Name = "ItemCount",
								Type = "number",
							},
							{
								Name = "ItemDamage",
								Type = "number",
							},
						},
						Notes = "Sets the specified slot to the specified item",
					},
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
local PlayerInventory = Player:GetInventory();
local PlayerMainInventory = PlayerInventory:GetInventoryGrid();  -- PlayerMainInventory is of type cItemGrid
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
local PlayerInventory = Player:GetInventory();
local PlayerArmor = PlayerInventory:GetArmorGrid();  -- PlayerArmor is of type cItemGrid
if (PlayerArmor:DamageItem(0)) then  -- Helmet is at SlotNum 0
  -- The helmet has reached max damage, destroy it:
  PlayerArmor:EmptySlot(0);
end
</pre>
					]],
				},
			},
		},
		cItems =
		{
			Desc = [[
				This class represents a numbered collection (array) of {{cItem}} objects. The array indices start at
				zero, each consecutive item gets a consecutive index. This class is used for spawning multiple
				pickups or for mass manipulating an inventory.
				]],
			Functions =
			{
				Add =
				{
					{
						Params =
						{
							{
								Name = "cItem",
								Type = "cItem",
							},
						},
						Notes = "Adds a new item to the end of the collection",
					},
					{
						Params =
						{
							{
								Name = "ItemType",
								Type = "number",
							},
							{
								Name = "ItemCount",
								Type = "number",
							},
							{
								Name = "ItemDamage",
								Type = "number",
							},
						},
						Notes = "Adds a new item to the end of the collection",
					},
				},
				AddItemGrid =
				{
					Params =
					{
						{
							Name = "ItemGrid",
							Type = "cItemGrid",
						},
					},
					Notes = "Adds a copy of each item in the specified {{cItemGrid|ItemGrid}}.",
				},
				Clear =
				{
					Notes = "Removes all items from the collection",
				},
				constructor =
				{
					Returns =
					{
						{
							Type = "cItems",
						},
					},
					Notes = "Creates a new empty cItems object",
				},
				Contains =
				{
					Params =
					{
						{
							Name = "Item",
							Type = "cItem",
						},
					},
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Returns true if the collection contains an item that is fully equivalent to the parameter",
				},
				ContainsType =
				{
					Params =
					{
						{
							Name = "Item",
							Type = "cItem",
						},
					},
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Returns true if the collection contains an item that is the same type as the parameter",
				},
				Delete =
				{
					Params =
					{
						{
							Name = "Index",
							Type = "number",
						},
					},
					Notes = "Deletes item at the specified index",
				},
				Get =
				{
					Params =
					{
						{
							Name = "Index",
							Type = "number",
						},
					},
					Returns =
					{
						{
							Type = "cItem",
						},
					},
					Notes = "Returns the item at the specified index",
				},
				Set =
				{
					{
						Params =
						{
							{
								Name = "Index",
								Type = "number",
							},
							{
								Name = "cItem",
								Type = "cItem",
							},
						},
						Notes = "Sets the item at the specified index to the specified item",
					},
					{
						Params =
						{
							{
								Name = "Index",
								Type = "number",
							},
							{
								Name = "ItemType",
								Type = "number",
							},
							{
								Name = "ItemCount",
								Type = "number",
							},
							{
								Name = "ItemDamage",
								Type = "number",
							},
						},
						Notes = "Sets the item at the specified index to the specified item",
					},
				},
				Size =
				{
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the number of items in the collection",
				},
			},
		},
		cJson =
		{
			Desc = [[
				Exposes the Json parser and serializer available in the server. Plugins can parse Json strings into
				Lua tables, and serialize Lua tables into Json strings easily.
			]],
			Functions =
			{
				Parse =
				{
					IsStatic = true,
					Params =
					{
						{
							Name = "InputString",
							Type = "string",
						},
					},
					Returns =
					{
						{
							Type = "table",
						},
					},
					Notes = "Parses the Json in the input string into a Lua table. Returns nil and detailed error message if parsing fails.",
				},
				Serialize =
				{
					IsStatic = true,
					Params =
					{
						{
							Name = "table",
							Type = "table",
						},
						{
							Name = "options",
							Type = "table",
							IsOptional = true,
						},
					},
					Returns =
					{
						{
							Type = "string",
						},
					},
					Notes = "Serializes the input table into a Json string. The options table, if present, is used to adjust the formatting of the serialized string, see below for details. <br/>Returns nil and error message if the table cannot be serialized (eg. contains both an array part and a dictionary part).",
				},
			},
			AdditionalInfo =
			{
				{
					Header = "Serializer options",
					Contents = [[
						The "options" parameter given to the cJson:Serialize() function is a dictionary-table of "option
						name" -> "option value". The serializer warns if any unknown options are used; the following
						options are recognized:</p>
						<ul>
						<li><b>commentStyle</b> - either "All" or "None", specifies whether comments are written to the
						output. Currently unused since comments cannot be represented in a Lua table</li>
						<li><b>indentation</b> - the string that is repeated for each level of indentation of the output.
						If empty, the Json is compressed (without linebreaks).</li>
						<li><b>enableYAMLCompatibility</b> - bool manipulating the whitespace around the colons.</li>
						<li><b>dropNullPlaceholders</b> - bool specifying whether null placeholders should be dropped
						from the output</li>
						</ul>
					]],
				},
				{
					Header = "Code example: Parsing a Json string",
					Contents = [==[
						The following code, adapted from the Debuggers plugin, parses a simple Json string and verifies
						the results:
<pre class="prettyprint lang-lua">
local t1 = cJson:Parse([[{"a": 1, "b": "2", "c": [3, "4", 5]}]])
assert(t1.a == 1)
assert(t1.b == "2")
assert(t1.c[1] == 3)
assert(t1.c[2] == "4")
assert(t1.c[3] == 5)
</pre>
					]==],
				},
				{
					Header = "Code example: Serializing into a Json string",
					Contents = [[
						The following code, adapted from the Debuggers plugin, serializes a simple Lua table into a
						string, using custom indentation:
<pre class="prettyprint lang-lua">
local s1 = cJson:Serialize({a = 1, b = "2", c = {3, "4", 5}}, {indentation = "  "})
LOG("Serialization result: " .. (s1 or "<nil>"))
</pre>
					]],
				},
			},
		},
		cLuaWindow =
		{
			Desc = [[
This class is used by plugins wishing to display a custom window to the player, unrelated to block entities or entities near the player. The window can be of any type and have any contents that the plugin defines. Callbacks for when the player modifies the window contents and when the player closes the window can be set.
</p>
		<p>This class inherits from the {{cWindow|cWindow}} class, so all cWindow's functions and constants can be used, in addition to the cLuaWindow-specific functions listed below.
</p>
		<p>The contents of this window are represented by a {{cWindow|cWindow}}:GetSlot() etc. or {{cPlayer|cPlayer}}:GetInventory() to access the player inventory.
</p>
		<p>When creating a new cLuaWindow object, you need to specify both the window type and the contents' width and height. Note that Cuberite accepts any combination of these, but opening a window for a player may crash their client if the contents' dimensions don't match the client's expectations.
</p>
		<p>To open the window for a player, call {{cPlayer|cPlayer}}:OpenWindow(). Multiple players can open window of the same cLuaWindow object. All players see the same items in the window's contents (like chest, unlike crafting table).
]],
			Functions =
			{
				constructor =
				{
					Params =
					{
						{
							Name = "WindowType",
							Type = "cWindow#WindowType",
						},
						{
							Name = "ContentsWidth",
							Type = "number",
						},
						{
							Name = "ContentsHeight",
							Type = "number",
						},
						{
							Name = "Title",
							Type = "string",
						},
					},
					Notes = "Creates a new object of this class. The window is not shown to any player yet.",
				},
				GetContents =
				{
					Returns =
					{
						{
							Type = "cItemGrid",
						},
					},
					Notes = "Returns the cItemGrid object representing the internal storage in this window",
				},
				SetOnClicked =
				{
					Params =
					{
						{
							Name = "OnClickedCallback",
							Type = "function",
						},
					},
					Notes = "Sets the function that the window will call when it is about to process a click from a player. See {{#additionalinfo_1|below}} for the signature of the callback function.",
				},
				SetOnClosing =
				{
					Params =
					{
						{
							Name = "OnClosingCallback",
							Type = "function",
						},
					},
					Notes = "Sets the function that the window will call when it is about to be closed by a player",
				},
				SetOnSlotChanged =
				{
					Params =
					{
						{
							Name = "OnSlotChangedCallback",
							Type = "function",
						},
					},
					Notes = "Sets the function that the window will call when a slot is changed by a player",
				},
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
					Header = "OnClicked Callback",
					Contents = [[
						This callback, settable via the SetOnClicked() function, will be called when the player clicks a slot in the window. The callback can cancel the click.</p>
<pre class="prettyprint lang-lua">
function OnWindowClicked(a_Window, a_Player, a_SlotNum, a_ClickAction, a_ClickedItem)
</pre>
						<p>
						The a_Window parameter is the cLuaWindow object representing the window, a_Player is the player who made the click, a_SlotNum is the slot the player clicked, a_ClickAction is the type of click the player made, and a_ClickedItem is the item the player clicked on, if applicable. If the function returns true, the click is cancelled (internally, the server resends the window slots to the player to keep the player in sync).
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
						This example is taken from the Debuggers plugin, used to test the API functionality. It opens a window and refuse to close it 3 times. It also logs slot changes to the server console and prevents shift-clicking in the window.
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

-- Prevent shift-clicking:
local OnClicked = function(Window, ClickingPlayer, SlotNum, ClickAction, ClickedItem)
	if ClickAction == caShiftLeftClick then
		return true
	end
end

-- Set window contents:
-- a_Player is a cPlayer object received from the outside of this code fragment
local Window = cLuaWindow(cWindow.wtHopper, 3, 3, "TestWnd");
Window:SetSlot(a_Player, 0, cItem(E_ITEM_DIAMOND, 64));
Window:SetOnClicked(OnClicked);
Window:SetOnClosing(OnClosing);
Window:SetOnSlotChanged(OnSlotChanged);

-- Open the window:
a_Player:OpenWindow(Window);
</pre>
					]],
				},
			},
			Inherits = "cWindow",
		},
		cMap =
		{
			Desc = [[
				This class encapsulates a single in-game colored map.</p>
				<p>
				The contents (i.e. pixel data) of a cMap are dynamically updated by each
				tracked {{cPlayer}} instance. Furthermore, a cMap maintains and periodically
				updates	a list of map decorators, which are objects drawn on the map that
				can freely move (e.g. Player and item frame pointers).
			]],
			Functions =
			{
				EraseData =
				{
					Notes = "Erases all pixel data.",
				},
				GetCenterX =
				{
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the X coord of the map's center.",
				},
				GetCenterZ =
				{
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the Y coord of the map's center.",
				},
				GetDimension =
				{
					Returns =
					{
						{
							Type = "eDimension",
						},
					},
					Notes = "Returns the dimension of the associated world.",
				},
				GetHeight =
				{
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the height of the map.",
				},
				GetID =
				{
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the numerical ID of the map. (The item damage value)",
				},
				GetName =
				{
					Returns =
					{
						{
							Type = "string",
						},
					},
					Notes = "Returns the name of the map.",
				},
				GetNumPixels =
				{
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the number of pixels in this map.",
				},
				GetPixel =
				{
					Params =
					{
						{
							Name = "PixelX",
							Type = "number",
						},
						{
							Name = "PixelZ",
							Type = "number",
						},
					},
					Returns =
					{
						{
							Name = "ColorID",
							Type = "number",
						},
					},
					Notes = "Returns the color of the specified pixel.",
				},
				GetPixelWidth =
				{
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the width of a single pixel in blocks.",
				},
				GetScale =
				{
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the scale of the map. Range: [0,4]",
				},
				GetWidth =
				{
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the width of the map.",
				},
				GetWorld =
				{
					Returns =
					{
						{
							Type = "cWorld",
						},
					},
					Notes = "Returns the associated world.",
				},
				Resize =
				{
					Params =
					{
						{
							Name = "Width",
							Type = "number",
						},
						{
							Name = "Height",
							Type = "number",
						},
					},
					Notes = "Resizes the map. WARNING: This will erase the pixel data.",
				},
				SetPixel =
				{
					Params =
					{
						{
							Name = "PixelX",
							Type = "number",
						},
						{
							Name = "PixelZ",
							Type = "number",
						},
						{
							Name = "ColorID",
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
					Notes = "Sets the color of the specified pixel. Returns false on error (Out of range).",
				},
				SetPosition =
				{
					Params =
					{
						{
							Name = "CenterX",
							Type = "number",
						},
						{
							Name = "CenterZ",
							Type = "number",
						},
					},
					Notes = "Relocates the map. The pixel data will not be modified.",
				},
				SetScale =
				{
					Params =
					{
						{
							Name = "Scale",
							Type = "number",
						},
					},
					Notes = "Rescales the map. The pixel data will not be modified.",
				},
			},
			Constants =
			{
				E_BASE_COLOR_BLUE =
				{
					Notes = "",
				},
				E_BASE_COLOR_BROWN =
				{
					Notes = "",
				},
				E_BASE_COLOR_DARK_BROWN =
				{
					Notes = "",
				},
				E_BASE_COLOR_DARK_GRAY =
				{
					Notes = "",
				},
				E_BASE_COLOR_DARK_GREEN =
				{
					Notes = "",
				},
				E_BASE_COLOR_GRAY_1 =
				{
					Notes = "",
				},
				E_BASE_COLOR_GRAY_2 =
				{
					Notes = "",
				},
				E_BASE_COLOR_LIGHT_BROWN =
				{
					Notes = "",
				},
				E_BASE_COLOR_LIGHT_GRAY =
				{
					Notes = "",
				},
				E_BASE_COLOR_LIGHT_GREEN =
				{
					Notes = "",
				},
				E_BASE_COLOR_PALE_BLUE =
				{
					Notes = "",
				},
				E_BASE_COLOR_RED =
				{
					Notes = "",
				},
				E_BASE_COLOR_TRANSPARENT =
				{
					Notes = "",
				},
				E_BASE_COLOR_WHITE =
				{
					Notes = "",
				},
			},
		},
		cMapManager =
		{
			Desc = [[
				This class is associated with a single {{cWorld}} instance and manages a list of maps.
			]],
			Functions =
			{
				DoWithMap =
				{
					Params =
					{
						{
							Name = "MapID",
							Type = "number",
						},
						{
							Name = "CallbackFunction",
							Type = "function",
						},
					},
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "If a map with the specified ID exists, calls the CallbackFunction for that map. The CallbackFunction has the following signature: <pre class=\"prettyprint lang-lua\">function Callback({{cMap|Map}})</pre> Returns true if the map was found and the callback called, false if map not found.",
				},
				GetNumMaps =
				{
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the number of registered maps.",
				},
			},
		},
		cMojangAPI =
		{
			Desc = [[
				Provides interface to various API functions that Mojang provides through their servers. Note that
				some of these calls will wait for a response from the network, and so shouldn't be used while the
				server is fully running (or at least when there are players connected) to avoid percepted lag.</p>
				<p>
				All the functions are static, call them using the <code>cMojangAPI:Function()</code> convention.</p>
				<p>
				Mojang uses two formats for UUIDs, short and dashed. Cuberite will accept either format for any
				functions taking a UUID. The MakeUUIDShort() and MakeUUIDDashed() functions are provided for plugins
				to use for conversion between the two formats.</p>
				<p>
				This class will cache values returned by the API service. The cache will hold the values for 7 days
				by default, after that, they will no longer be available. This is in order to not let the server get
				banned from using the API service, since they are rate-limited to 600 queries per 10 minutes. The
				cache contents also gets updated whenever a player successfully joins, since that makes the server
				contact the API service, too, and retrieve the relevant data.</p>
			]],
			Functions =
			{
				AddPlayerNameToUUIDMapping =
				{
					IsStatic = true,
					Params =
					{
						{
							Name = "PlayerName",
							Type = "string",
						},
						{
							Name = "UUID",
							Type = "cUUID",
						},
					},
					Notes = "Adds the specified PlayerName-to-UUID mapping into the cache, with current timestamp.",
				},
				GetPlayerNameFromUUID =
				{
					IsStatic = true,
					Params =
					{
						{
							Name = "UUID",
							Type = "cUUID",
						},
						{
							Name = "UseOnlyCached",
							Type = "boolean",
							IsOptional = true,
						},
					},
					Returns =
					{
						{
							Name = "PlayerName",
							Type = "string",
						},
					},
					Notes = "Returns the playername that corresponds to the given UUID, or an empty string on error. If UseOnlyCached is false (the default), queries the Mojang servers if the UUID is not in the cache. The UUID can be either short or dashed. <br /><b>WARNING</b>: Do NOT use this function with UseOnlyCached set to false while the server is running. Only use it when the server is starting up (inside the Initialize() method), otherwise you will lag the server severely.",
				},
				GetUUIDFromPlayerName =
				{
					IsStatic = true,
					Params =
					{
						{
							Name = "PlayerName",
							Type = "string",
						},
						{
							Name = "UseOnlyCached",
							Type = "boolean",
							IsOptional = true,
						},
					},
					Returns =
					{
						{
							Name = "UUID",
							Type = "string",
						},
					},
					Notes = "Returns the (short) UUID that corresponds to the given playername, or an empty string on error. If UseOnlyCached is false (the default), queries the Mojang servers if the playername is not in the cache. <br /><b>WARNING</b>: Do NOT use this function with UseOnlyCached set to false while the server is running. Only use it when the server is starting up (inside the Initialize() method), otherwise you will lag the server severely.",
				},
				GetUUIDsFromPlayerNames =
				{
					IsStatic = true,
					Params =
					{
						{
							Name = "PlayerNames",
							Type = "string",
						},
						{
							Name = "UseOnlyCached",
							Type = "boolean",
							IsOptional = true,
						},
					},
					Returns =
					{
						{
							Type = "table",
						},
					},
					Notes = "Returns a table that contains the map, 'PlayerName' -> '(short) UUID', for all valid playernames in the input array-table. PlayerNames not recognized will not be set in the returned map. If UseOnlyCached is false (the default), queries the Mojang servers for the results that are not in the cache. <br /><b>WARNING</b>: Do NOT use this function with UseOnlyCached set to false while the server is running. Only use it when the server is starting up (inside the Initialize() method), otherwise you will lag the server severely.",
				},
				MakeUUIDDashed =
				{
					IsStatic = true,
					Params =
					{
						{
							Name = "UUID",
							Type = "cUUID",
						},
					},
					Returns =
					{
						{
							Name = "DashedUUID",
							Type = "string",
						},
					},
					Notes = "Converts the UUID to a dashed format (\"01234567-8901-2345-6789-012345678901\"). An alias for cUUID:ToLongString()",
				},
				MakeUUIDShort =
				{
					IsStatic = true,
					Params =
					{
						{
							Name = "UUID",
							Type = "cUUID",
						},
					},
					Returns =
					{
						{
							Name = "ShortUUID",
							Type = "string",
						},
					},
					Notes = "Converts the UUID to a short format (without dashes, \"01234567890123456789012345678901\"). An alias for cUUID:ToShortString()",
				},
			},
		},
		cMonster =
		{
			Desc = [[
				This class is the base class for all computer-controlled mobs in the game.</p>
				<p>
				To spawn a mob in a world, use the {{cWorld}}:SpawnMob() function.
			]],
			Functions =
			{
                                BurnsInDaylight =
                                {
                                        Returns =
                                        {
                                                {
                                                        Type = "boolean",
                                                },
                                        },
                                        Notes = "Returns whether the mob burns in daylight.",
                                },
				CanBeLeashed =
				{
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Returns whether the mob can be leashed.",
				},
				FamilyFromType =
				{
					IsStatic = true,
					Params =
					{
						{
							Name = "MobType",
							Type = "eMonsterType",
						},
					},
					Returns =
					{
						{
							Name = "MobFamily",
							Type = "cMonster#eFamily",
						},
					},
					Notes = "Returns the mob family ({{cMonster#eFamily|mfXXX}} constants) based on the mob type ({{Globals#eMonsterType|mtXXX}} constants)",
				},
				GetAge =
				{
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the age of the monster",
				},
				GetCustomName =
				{
					Returns =
					{
						{
							Type = "string",
						},
					},
					Notes = "Gets the custom name of the monster. If no custom name is set, the function returns an empty string.",
				},
				GetLeashedTo =
				{
					Returns =
					{
						{
							Name = "LeashedTo",
							Type = "cEntity",
						},
					},
					Notes = "Returns the entity to where this mob is leashed, returns nil if it's not leashed",
				},
				GetMobFamily =
				{
					Returns =
					{
						{
							Name = "MobFamily",
							Type = "cMonster#eFamily",
						},
					},
					Notes = "Returns this mob's family ({{cMonster#eFamily|mfXXX}} constant)",
				},
				GetMobType =
				{
					Returns =
					{
						{
							Name = "MobType",
							Type = "eMonsterType",
						},
					},
					Notes = "Returns the type of this mob ({{Globals#eMonsterType|mtXXX}} constant)",
				},
				GetRelativeWalkSpeed =
				{
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the relative walk speed of this mob. Standard is 1.0",
				},
				GetSpawnDelay =
				{
					IsStatic = true,
					Params =
					{
						{
							Name = "MobFamily",
							Type = "cMonster#eFamily",
						},
					},
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the spawn delay  - the number of game ticks between spawn attempts - for the specified mob family.",
				},
				HasCustomName =
				{
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Returns true if the monster has a custom name.",
				},
				IsBaby =
				{
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Returns true if the monster is a baby",
				},
				IsCustomNameAlwaysVisible =
				{
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Is the custom name of this monster always visible? If not, you only see the name when you sight the mob.",
				},
				IsLeashed =
				{
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Returns whether the monster is leashed to an entity.",
				},
				LeashTo =
				{
					Params =
					{
						{
							Name = "Entity",
							Type = "cEntity",
						}
					},
					Notes = "Leash the monster to an entity.",
				},
				MobTypeToString =
				{
					IsStatic = true,
					Params =
					{
						{
							Name = "MobType",
							Type = "eMonsterType",
						},
					},
					Returns =
					{
						{
							Type = "string",
						},
					},
					Notes = "Returns the string representing the given mob type ({{Globals#eMonsterType|mtXXX}} constant), or empty string if unknown type.",
				},
				MobTypeToVanillaName =
				{
					IsStatic = true,
					Params =
					{
						{
							Name = "MobType",
							Type = "eMonsterType",
						},
					},
					Returns =
					{
						{
							Type = "string",
						},
					},
					Notes = "Returns the vanilla name of the given mob type, or empty string if unknown type.",
				},
				MoveToPosition =
				{
					Params =
					{
						{
							Name = "Position",
							Type = "Vector3d",
						},
					},
					Notes = "Start moving (using a pathfinder) towards the specified position",
				},
				SetAge =
				{
					Params =
					{
						{
							Name = "Age",
							Type = "number",
						},
					},
					Notes = "Sets the age of the monster",
				},
				SetBurnsInDaylight =
				{
					Params =
					{
						{
							Name = "BurnsInDaylight",
							Type = "boolean",
						},
					},
					Notes = "Sets whether the mob burns in daylight. Only evaluated at next burn-decision tick",
				},
				SetCanBeLeashed =
				{
					Params =
					{
						{
							Name = "CanBeLeashed",
							Type = "boolean",
						}
					},
					Notes = "Sets whether the mob can be leashed, for extensibility in plugins"
				},
				SetCustomName =
				{
					Params =
					{
						{
							Name = "CustomName",
							Type = "string",
						},
					},
					Notes = "Sets the custom name of the monster. You see the name over the monster. If you want to disable the custom name, simply set an empty string.",
				},
				SetCustomNameAlwaysVisible =
				{
					Params =
					{
						{
							Name = "IsCustomNameAlwaysVisible",
							Type = "boolean",
						},
					},
					Notes = "Sets the custom name visiblity of this monster. If it's false, you only see the name when you sight the mob. If it's true, you always see the custom name.",
				},
				SetRelativeWalkSpeed =
				{
					Params =
					{
						{
							Name = "RelativeWalkSpeed",
							Type = "number",
						},
					},
					Notes = "Sets the relative walk speed of this mob. The default relative speed is 1.0.",
				},
				StringToMobType =
				{
					IsStatic = true,
					Params =
					{
						{
							Name = "MobTypeString",
							Type = "string",
						},
					},
					Returns =
					{
						{
							Name = "MobType",
							Type = "eMonsterType",
						},
					},
					Notes = "Returns the mob type ({{Globals#eMonsterType|mtXXX}} constant) parsed from the string type (\"creeper\"), or mtInvalidType if unrecognized.",
				},
				Unleash =
				{
					Params =
					{
						{
							Name = "ShouldDropLeashPickup",
							Type = "boolean",
						},
					},
					Notes = "Unleash the monster.",
				},
			},
			Constants =
			{
				mfAmbient =
				{
					Notes = "Family: ambient (bat)",
				},
				mfHostile =
				{
					Notes = "Family: hostile (blaze, cavespider, creeper, enderdragon, enderman, ghast, giant, magmacube, silverfish, skeleton, slime, spider, witch, wither, zombie, zombiepigman)",
				},
				mfMaxplusone =
				{
					Notes = "The maximum family value, plus one. Returned when monster family not recognized.",
				},
				mfPassive =
				{
					Notes = "Family: passive (chicken, cow, horse, irongolem, mooshroom, ocelot, pig, sheep, snowgolem, villager, wolf)",
				},
				mfWater =
				{
					Notes = "Family: water (squid)",
				},
			},
			ConstantGroups =
			{
				eFamily =
				{
					Include = "mf.*",
					TextBefore = [[
						Mobs are divided into families. The following constants are used for individual family types:
					]],
				},
			},
			Inherits = "cPawn",
		},
		cObjective =
		{
			Desc = [[
				This class represents a single scoreboard objective.
			]],
			Functions =
			{
				AddScore =
				{
					Params =
					{
						{
							Name = "string",
							Type = "string",
						},
						{
							Name = "number",
							Type = "number",
						},
					},
					Returns =
					{
						{
							Name = "Score",
							Type = "<unknown>",
						},
					},
					Notes = "Adds a value to the score of the specified player and returns the new value.",
				},
				GetDisplayName =
				{
					Returns =
					{
						{
							Type = "string",
						},
					},
					Notes = "Returns the display name of the objective. This name will be shown to the connected players.",
				},
				GetName =
				{
					Returns =
					{
						{
							Type = "string",
						},
					},
					Notes = "Returns the internal name of the objective.",
				},
				GetScore =
				{
					Params =
					{
						{
							Name = "string",
							Type = "string",
						},
					},
					Returns =
					{
						{
							Name = "Score",
							Type = "<unknown>",
						},
					},
					Notes = "Returns the score of the specified player.",
				},
				GetType =
				{
					Returns =
					{
						{
							Name = "eType",
							Type = "<unknown>",
						},
					},
					Notes = "Returns the type of the objective. (i.e what is being tracked)",
				},
				Reset =
				{
					Notes = "Resets the scores of the tracked players.",
				},
				ResetScore =
				{
					Params =
					{
						{
							Name = "string",
							Type = "string",
						},
					},
					Notes = "Reset the score of the specified player.",
				},
				SetDisplayName =
				{
					Params =
					{
						{
							Name = "string",
							Type = "string",
						},
					},
					Notes = "Sets the display name of the objective.",
				},
				SetScore =
				{
					Params =
					{
						{
							Name = "string",
							Type = "string",
						},
						{
							Name = "Score",
							Type = "<unknown>",
						},
					},
					Notes = "Sets the score of the specified player.",
				},
				SubScore =
				{
					Params =
					{
						{
							Name = "string",
							Type = "string",
						},
						{
							Name = "number",
							Type = "number",
						},
					},
					Returns =
					{
						{
							Name = "Score",
							Type = "<unknown>",
						},
					},
					Notes = "Subtracts a value from the score of the specified player and returns the new value.",
				},
			},
			Constants =
			{
				otAchievement =
				{
					Notes = "",
				},
				otDeathCount =
				{
					Notes = "",
				},
				otDummy =
				{
					Notes = "",
				},
				otHealth =
				{
					Notes = "",
				},
				otPlayerKillCount =
				{
					Notes = "",
				},
				otStat =
				{
					Notes = "",
				},
				otStatBlockMine =
				{
					Notes = "",
				},
				otStatEntityKill =
				{
					Notes = "",
				},
				otStatEntityKilledBy =
				{
					Notes = "",
				},
				otStatItemBreak =
				{
					Notes = "",
				},
				otStatItemCraft =
				{
					Notes = "",
				},
				otStatItemUse =
				{
					Notes = "",
				},
				otTotalKillCount =
				{
					Notes = "",
				},
			},
		},
		cPainting =
		{
			Desc = "This class represents a painting in the world. These paintings are special and different from Vanilla in that they can be critical-hit.",
			Functions =
			{
				GetDirection =
				{
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the direction the painting faces. Directions: ZP - 0, ZM - 2, XM - 1, XP - 3. Note that these are not the BLOCK_FACE constants.",
				},
				GetName =
				{
					Returns =
					{
						{
							Type = "string",
						},
					},
					Notes = "Returns the name of the painting",
				},
			},
		},
		cPawn =
		{
			Desc = "cPawn is a controllable pawn object, controlled by either AI or a player. cPawn inherits all functions and members of {{cEntity}}\
",
			Functions =
			{
				AddEntityEffect =
				{
					Params =
					{
						{
							Name = "EffectType",
							Type = "cEntityEffect#eType",
						},
						{
							Name = "EffectDurationTicks",
							Type = "number",
						},
						{
							Name = "EffectIntensity",
							Type = "number",
						},
						{
							Name = "DistanceModifier",
							Type = "number",
						},
					},
					Notes = "Applies an entity effect. Checks with plugins if they allow the addition. EffectIntensity is the level of the effect (0 = Potion I, 1 = Potion II, etc). DistanceModifier is the scalar multiplied to the potion duration (only applies to splash potions).",
				},
				ClearEntityEffects =
				{
					Notes = "Removes all currently applied entity effects",
				},
				HasEntityEffect =
				{
					Params =
					{
						{
							Name = "EffectType",
							Type = "cEntityEffect#eType",
						},
					},
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Returns true, if the supplied entity effect type is currently applied",
				},
				RemoveEntityEffect =
				{
					Params =
					{
						{
							Name = "EffectType",
							Type = "cEntityEffect#eType",
						},
					},
					Notes = "Removes a currently applied entity effect",
				},
			},
			Inherits = "cEntity",
		},
		cExpOrb =
		{
			Desc = [[
				This class represents an experience orb. This entity can be spawned by using {{cWorld#SpawnExperienceOrb_1|cWorld:SpawnExperienceOrb}}.
				It's also spawned when a monster is killed or a player is mining coal. The default lifetime of an experience orb is 5 mins.
			]],
			Functions =
			{
				GetAge =
				{
					Returns =
					{
						{
							Type = "number"
						},
					},
					Notes = "Returns the number of ticks that this experience orb has existed.",
				},
				SetAge =
				{
					Params =
					{
						{
							Type = "number",
						},
					},
					Notes = "Sets the experience orb's age, in ticks.",
				},
				GetReward =
				{
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the experience amount.",
				},
				SetReward =
				{
					Params =
					{
						{
							Type = "number",
						},
					},
					Notes = "Sets the experience amount.",
				},
			},
			Inherits = "cEntity",
		},
		cFallingBlock =
		{
			Desc = [[
				This class represents a falling block. This entity can be spawned by using {{cWorld#SpawnFallingBlock_1|cWorld:SpawnFallingBlock}}.
			]],
			Functions =
			{
				GetBlockType =
				{
					Returns =
					{
						{
							Name = "BlockType",
							Type = "number",
						}
					},
					Notes = "Returns the block type of the falling block.",
				},
				GetBlockMeta =
				{
					Returns =
					{
						{
							Name = "BlockMeta",
							Type = "number",
						}
					},
					Notes = "Returns the block meta of the falling block.",
				},
			},
			Inherits = "cEntity",
		},
		cBoat =
		{
			Desc = [[
				This class represents a boat. This entity can be spawned by using {{cWorld#SpawnBoat_1|cWorld:SpawnBoat}}.
			]],
			Functions =
			{
				GetMaterial =
				{
					Returns =
					{
						{
							Name = "Material",
							Type = "cBoat#eMaterial",
						},
					},
					Notes = "Returns the material of the boat.",
				},
				MaterialToString =
				{
					IsStatic = true,
					Params =
					{
						{
							Name = "Material",
							Type = "cBoat#eMaterial",
						},
					},
					Returns =
					{
						{
							Name = "Material",
							Type = "string",
						},
					},
					Notes = "Returns the boat material as a string.",
				},
				ItemToMaterial =
				{
					IsStatic = true,
					Params =
					{
						{
							Name = "Item",
							Type = "cItem",
						},
					},
					Returns =
					{
						{
							Name = "Material",
							Type = "cBoat#eMaterial",
						},
					},
					Notes = "Returns the eMaterial that should be used for a boat created from the specified item. Returns bmOak if not a boat item.",
				},
				MaterialToItem =
				{
					IsStatic = true,
					Params =
					{
						{
							Name = "Material",
							Type = "cBoat#eMaterial",
						},
					},
					Returns =
					{
						{
							Name = "Item",
							Type = "cItem",
						},
					},
					Notes = "Returns the boat item of the boat material",
				},
				StringToMaterial =
				{
					IsStatic = true,
					Params =
					{
						{
							Name = "Material",
							Type = "string",
						},
					},
					Returns =
					{
						{
							Name = "Material",
							Type = "cBoat#eMaterial",
						},
					},
					Notes = "Returns the boat material for the passed string. Returns oak if not valid.",
				},
				SetMaterial =
				{
					Params =
					{
						{
							Name = "Material",
							Type = "cBoat#eMaterial",
						},
					},
					Notes = "Set the material of the boat.",
				},
			},
			Constants =
			{
				bmOak =
				{
					Notes = "",
				},
				bmSpruce =
				{
					Notes = "",
				},
				bmBirch =
				{
					Notes = "",
				},
				bmJungle =
				{
					Notes = "",
				},
				bmAcacia =
				{
					Notes = "",
				},
				bmDarkOak =
				{
					Notes = "",
				},
			},
			ConstantGroups =
			{
				eMaterial =
				{
					Include = "bm.*",
					TextBefore = "These constans are the different wood materials of the boat.",
				},
			},
			Inherits = "cEntity",
		},
		cPickup =
		{
			Desc = [[
				This class represents a pickup entity (an item that the player or mobs can pick up). It is also
				commonly known as "drops". With this class you could create your own "drop" or modify those
				created automatically. The default lifetime of a pickup is 5 mins.
			]],
			Functions =
			{
				CanCombine =
				{
					Returns =
					{
						{
							Type = "boolean"
						}
					},
					Notes = "Returns whether this pickup is allowed to combine with other similar pickups.",
				},
				CollectedBy =
				{
					Params =
					{
						{
							Name = "Player",
							Type = "cPlayer",
						},
					},
					Returns =
					{
						{
							Name = "WasCollected",
							Type = "boolean",
						},
					},
					Notes = "Tries to make the player collect the pickup. Returns true if the pickup was collected, at least partially.",
				},
				GetAge =
				{
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the number of ticks that the pickup has existed.",
				},
				GetItem =
				{
					Returns =
					{
						{
							Type = "cItem",
						},
					},
					Notes = "Returns the item represented by this pickup",
				},
				GetLifetime =
				{
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the total length of this pickup's lifespan, in ticks.",
				},
				IsCollected =
				{
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Returns true if this pickup has already been collected (is waiting to be destroyed)",
				},
				IsPlayerCreated =
				{
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Returns true if the pickup was created by a player",
				},
				SetAge =
				{
					Params =
					{
						{
							Name = "AgeTicks",
							Type = "number",
						},
					},
					Notes = "Sets the pickup's age, in ticks.",
				},
				SetCanCombine =
				{
					Params =
					{
						{
							Name = "CanCombine",
							Type = "boolean",
						},
					},
					Notes = "Sets whether this pickup is allowed to combine with other similar pickups.",
				},
				SetLifetime =
				{
					Params =
					{
						{
							Name = "LifeTimeInTicks",
							Type = "number",
						},
					},
					Notes = "Sets the total lifespan of this pickup before it despawns, in ticks. Does not reset the age of the pickup, use SetAge(0). If new lifetime is less than the current age, pickup will despawn.",
				},
			},
			Inherits = "cEntity",
		},
		cPlayer =
		{
			Desc = [[
				This class describes a player in the server. cPlayer inherits all functions and members of
				{{cPawn|cPawn}}. It handles all the aspects of the gameplay, such as hunger, sprinting, inventory
				etc.
			]],
			Functions =
			{
				AddFoodExhaustion =
				{
					Params =
					{
						{
							Name = "Exhaustion",
							Type = "number",
						},
					},
					Notes = "Adds the specified number to the food exhaustion. Only positive numbers expected.",
				},
				CalcLevelFromXp =
				{
					IsStatic = true,
					Params =
					{
						{
							Name = "XPAmount",
							Type = "number",
						},
					},
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the level which is reached with the specified amount of XP. Inverse of XpForLevel().",
				},
				CanFly =
				{
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Returns if the player is able to fly.",
				},
				CanMobsTarget =
				{
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Returns if the player can be targeted by mobs.",
				},
				CloseWindow =
				{
					Params =
					{
						{
							Name = "CanRefuse",
							Type = "boolean",
							IsOptional = true,
						},
					},
					Notes = "Closes the currently open UI window. If CanRefuse is true (default), the window may refuse the closing.",
				},
				CloseWindowIfID =
				{
					Params =
					{
						{
							Name = "WindowID",
							Type = "number",
						},
						{
							Name = "CanRefuse",
							Type = "boolean",
							IsOptional = true,
						},
					},
					Notes = "Closes the currently open UI window if its ID matches the given ID. If CanRefuse is true (default), the window may refuse the closing.",
				},
				DeltaExperience =
				{
					Params =
					{
						{
							Name = "DeltaXP",
							Type = "number",
						},
					},
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Adds or removes XP from the current XP amount. Won't allow XP to go negative. Returns the new experience, -1 on error (XP overflow).",
				},
				Feed =
				{
					Params =
					{
						{
							Name = "AddFood",
							Type = "number",
						},
						{
							Name = "AddSaturation",
							Type = "number",
						},
					},
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Tries to add the specified amounts to food level and food saturation level (only positive amounts expected). Returns true if player was hungry and the food was consumed, false if too satiated.",
				},
				ForceSetSpeed =
				{
					Params =
					{
						{
							Name = "Direction",
							Type = "Vector3d",
						},
					},
					Notes = "Forces the player to move to the given direction.",
				},
				Freeze =
				{
					Params =
					{
						{
							Name = "Location",
							Type = "Vector3d",
						},
					},
					Notes = "Teleports the player to \"Location\" and prevents them from moving, locking them in place until unfreeze() is called",
				},
				GetClientHandle =
				{
					Returns =
					{
						{
							Type = "cClientHandle",
						},
					},
					Notes = "Returns the client handle representing the player's connection. May be nil (AI players).",
				},
				GetColor =
				{
					Returns =
					{
						{
							Type = "string",
						},
					},
					Notes = "Returns the full color code to be used for this player's messages (based on their rank). Prefix player messages with this code.",
				},
				GetDraggingItem =
				{
					Returns =
					{
						{
							Type = "cItem",
						},
					},
					Notes = "Returns the item the player is dragging in a UI window."
				},
				GetPrefix =
				{
					Returns =
					{
						{
							Type = "string",
						},
					},
					Notes = "Returns the prefix to player names for messages (based on their rank), may contain @ format codes.",
				},
				GetSuffix =
				{
					Returns =
					{
						{
							Type = "string",
						},
					},
					Notes = "Returns the suffix to player names for messages (based on their rank), may contain @ format codes.",
				},
				GetCurrentXp =
				{
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the current amount of XP",
				},
				GetCustomName =
				{
					Returns =
					{
						{
							Type = "string",
						},
					},
					Notes = "Returns the custom name of this player. If the player hasn't a custom name, it will return an empty string.",
				},
				GetEffectiveGameMode =
				{
					Returns =
					{
						{
							Name = "GameMode",
							Type = "eGameMode",
						},
					},
					Notes = "(OBSOLETE) Returns the current resolved game mode of the player. If the player is set to inherit the world's gamemode, returns that instead. See also GetGameMode() and IsGameModeXXX() functions. Note that this function is the same as GetGameMode(), use that function instead.",
				},
				GetEquippedItem =
				{
					Returns =
					{
						{
							Type = "cItem",
						},
					},
					Notes = "Returns the item that the player is currently holding; empty item if holding nothing.",
				},
				GetEyeHeight =
				{
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the height of the player's eyes, in absolute coords",
				},
				GetEyePosition =
				{
					Returns =
					{
						{
							Name = "EyePositionVector",
							Type = "Vector3d",
						},
					},
					Notes = "Returns the position of the player's eyes, as a {{Vector3d}}",
				},
				GetFloaterID =
				{
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the Entity ID of the fishing hook floater that belongs to the player. Returns -1 if no floater is associated with the player. FIXME: Undefined behavior when the player has used multiple fishing rods simultanously.",
				},
				GetFlyingMaxSpeed =
				{
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the maximum flying speed, relative to the default game flying speed. Defaults to 1, but plugins may modify it for faster or slower flying.",
				},
				GetFoodExhaustionLevel =
				{
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the food exhaustion level",
				},
				GetFoodLevel =
				{
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the food level (number of half-drumsticks on-screen)",
				},
				GetFoodSaturationLevel =
				{
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the food saturation (overcharge of the food level, is depleted before food level)",
				},
				GetFoodTickTimer =
				{
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the number of ticks past the last food-based heal or damage action; when this timer reaches 80, a new heal / damage is applied.",
				},
				GetGameMode =
				{
					Returns =
					{
						{
							Name = "GameMode",
							Type = "eGameMode",
						},
					},
					Notes = "Returns the player's gamemode. The player may have their gamemode unassigned, in which case they inherit the gamemode from the current {{cWorld|world}}.<br /> <b>NOTE:</b> Instead of comparing the value returned by this function to the gmXXX constants, use the IsGameModeXXX() functions. These functions handle the gamemode inheritance automatically.",
				},
				GetInventory =
				{
					Returns =
					{
						{
							Name = "Inventory",
							Type = "cInventory",
						},
					},
					Notes = "Returns the player's inventory",
				},
				GetIP =
				{
					Returns =
					{
						{
							Type = "string",
						},
					},
					Notes = "Returns the IP address of the player, if available. Returns an empty string if there's no IP to report.",
				},
				GetLastBedPos =
				{
					Returns =
					{
						{
							Type = "Vector3i",
						},
					},
					Notes = "Returns the position of the last bed the player has slept in, or the world's spawn if no such position was recorded.",
				},
				GetMaxSpeed =
				{
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the player's current maximum speed, relative to the game default speed. Takes into account the sprinting / flying status.",
				},
				GetMainHand =
				{
					Returns =
					{
						{
							Type = "eMainHand",
						},
					},
					Notes = "Returns the player's main hand.",
				},
				GetName =
				{
					Returns =
					{
						{
							Type = "string",
						},
					},
					Notes = "Returns the player's name",
				},
				GetNormalMaxSpeed =
				{
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the player's maximum walking speed, relative to the game default speed. Defaults to 1, but plugins may modify it for faster or slower walking.",
				},
				GetPermissions =
				{
					Returns =
					{
						{
							Type = "table",
						},
					},
					Notes = "Returns an array-table of all permissions (strings) that the player has assigned to them through their rank.",
				},
				GetPlayerListName =
				{
					Returns =
					{
						{
							Type = "string",
						},
					},
					Notes = "Returns the name that is used in the playerlist.",
				},
				GetRestrictions =
				{
					Returns =
					{
						{
							Type = "table",
						},
					},
					Notes = "Returns an array-table of all the restrictions that the player has assigned to them.",
				},
				GetSprintingMaxSpeed =
				{
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the player's maximum sprinting speed, relative to the game default speed. Defaults to 1.3, but plugins may modify it for faster or slower sprinting.",
				},
				GetSkinParts =
				{
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the player's current set of skin part flags.  This is a bitwise OR of various {{Globals#eSkinPart|eSkinPart}} constants.  Note that HasSkinPart may be easier to use in most situations.",
				},
				GetStance =
				{
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the player's stance (Y-pos of player's eyes)",
				},
				GetTeam =
				{
					Returns =
					{
						{
							Type = "cTeam",
						},
					},
					Notes = "Returns the team that the player belongs to, or nil if none.",
				},
				GetThrowSpeed =
				{
					Params =
					{
						{
							Name = "SpeedCoeff",
							Type = "number",
						},
					},
					Returns =
					{
						{
							Type = "Vector3d",
						},
					},
					Notes = "Returns the speed vector for an object thrown with the specified speed coeff. Basically returns the normalized look vector multiplied by the coeff, with a slight random variation.",
				},
				GetThrowStartPos =
				{
					Returns =
					{
						{
							Type = "Vector3d",
						},
					},
					Notes = "Returns the position where the projectiles should start when thrown by this player.",
				},
				GetUUID =
				{
					Returns =
					{
						{
							Type = "string",
						},
					},
					Notes = "Returns the (short) UUID that the player is using. Could be empty string for players that don't have a Mojang account assigned to them (in the future, bots for example).",
				},
				GetWindow =
				{
					Returns =
					{
						{
							Type = "cWindow",
						},
					},
					Notes = "Returns the currently open UI window. If the player doesn't have any UI window open, returns the inventory window.",
				},
				GetXpLevel =
				{
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the current XP level (based on current XP amount).",
				},
				GetXpLifetimeTotal =
				{
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the amount of XP that has been accumulated throughout the player's lifetime.",
				},
				GetXpPercentage =
				{
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the percentage of the experience bar - the amount of XP towards the next XP level. Between 0 and 1.",
				},
				HasCustomName =
				{
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Returns true if the player has a custom name.",
				},
				HasPermission =
				{
					Params =
					{
						{
							Name = "PermissionString",
							Type = "string",
						},
					},
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Returns true if the player has the specified permission",
				},
				HasSkinPart =
				{
					Params =
					{
						{
							Name = "Part",
							Type = "eSkinPart",
						},
					},
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Returns true if the player has the specified skin part enabled",
				},
				Heal =
				{
					Params =
					{
						{
							Name = "HitPoints",
							Type = "number",
						},
					},
					Notes = "Heals the player by the specified amount of HPs. Only positive amounts are expected. Sends a health update to the client.",
				},
				IsEating =
				{
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Returns true if the player is currently eating the item in their hand.",
				},
				IsFireproof =
				{
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Returns true if a player is fireproof. This is when the flag has been explicitly set, or the player is in creative or spectator mode.",
				},
				IsFishing =
				{
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Returns true if the player is currently fishing",
				},
				IsFlying =
				{
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Returns true if the player is flying.",
				},
				IsFrozen =
				{
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Returns true if the player is frozen. See Freeze()",
				},
				IsGameModeAdventure =
				{
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Returns true if the player is in the gmAdventure gamemode, or has their gamemode unset and the world is a gmAdventure world.",
				},
				IsGameModeCreative =
				{
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Returns true if the player is in the gmCreative gamemode, or has their gamemode unset and the world is a gmCreative world.",
				},
				IsGameModeSpectator =
				{
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Returns true if the player is in the gmSpectator gamemode, or has their gamemode unset and the world is a gmSpectator world.",
				},
				IsGameModeSurvival =
				{
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Returns true if the player is in the gmSurvival gamemode, or has their gamemode unset and the world is a gmSurvival world.",
				},
				IsInBed =
				{
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Returns true if the player is currently lying in a bed.",
				},
				IsSatiated =
				{
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Returns true if the player is satiated (cannot eat).",
				},
				IsVisible =
				{
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Returns true if the player is visible to other players",
				},
				LoadRank =
				{
					Notes = "Reloads the player's rank, message visuals and permissions from the {{cRankManager}}, based on the player's current rank.",
				},
				OpenWindow =
				{
					Params =
					{
						{
							Name = "Window",
							Type = "cWindow",
						},
					},
					Notes = "Opens the specified UI window for the player.",
				},
				PermissionMatches =
				{
					IsStatic = true,
					Params =
					{
						{
							Name = "Permission",
							Type = "string",
						},
						{
							Name = "Template",
							Type = "string",
						},
					},
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Returns true if the specified permission matches the specified template. The template may contain asterisk as a wildcard for any word.",
				},
				PlaceBlock =
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
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Places a block while impersonating the player. The {{OnPlayerPlacingBlock|HOOK_PLAYER_PLACING_BLOCK}} hook is called before the placement, and if it succeeds, the block is placed and the {{OnPlayerPlacedBlock|HOOK_PLAYER_PLACED_BLOCK}} hook is called. Returns true iff the block is successfully placed. Assumes that the block is in a currently loaded chunk.",
				},
				ReplaceOneEquippedItemTossRest =
				{
					Params =
					{
						{
							Name = "Item",
							Type = "cItem",
						},
					},
					Notes = "Removes one item from the the current equipped item stack, and attempts to add the specified item stack back to the same slot. If it is not possible to place the item in the same slot, tries to place the specified item elsewhere in the inventory. If this is not possible, then any remaining items are tossed. If the currently equipped slot is empty, its contents are simply set to the given Item.",
				},
				Respawn =
				{
					Notes = "Restores the health, extinguishes fire, makes visible and sends the Respawn packet.",
				},
				SendAboveActionBarMessage =
				{
					Params =
					{
						{
							Name = "Message",
							Type = "string",
						},
					},
					Notes = "Sends the specified message to the player (shows above action bar).",
				},
				SendBlocksAround =
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
							Name = "BlockRange",
							Type = "number",
							IsOptional = true,
						},
					},
					Notes = "Sends all the world's blocks in BlockRange from the specified coords to the player, as a BlockChange packet. Range defaults to 1 (only one block sent).",
				},
				SendMessage =
				{
					Params =
					{
						{
							Name = "Message",
							Type = "string",
						},
					},
					Notes = "Sends the specified message to the player.",
				},
				SendMessageRaw =
				{
					Params =
					{
						{
							Name = "Json",
							Type = "string",
						},
						{
							Name = "eChatType",
							Type = "number",
							IsOptional = true,
						}
					},
					Notes = "Sends the specified json string to the player. The optional value eChatType (default ctChatBox) can be ctChatBox, ctSystem or ctAboveActionBar. You can use {{cJson}} to build a json string.",
				},
				SendMessageFailure =
				{
					Params =
					{
						{
							Name = "Message",
							Type = "string",
						},
					},
					Notes = "Prepends Rose [INFO] / colours entire text (depending on ShouldUseChatPrefixes()) and sends message to player. For a command that failed to run because of insufficient permissions, etc.",
				},
				SendMessageFatal =
				{
					Params =
					{
						{
							Name = "Message",
							Type = "string",
						},
					},
					Notes = "Prepends Red [FATAL] / colours entire text (depending on ShouldUseChatPrefixes()) and sends message to player. For something serious, such as a plugin crash, etc.",
				},
				SendMessageInfo =
				{
					Params =
					{
						{
							Name = "Message",
							Type = "string",
						},
					},
					Notes = "Prepends Yellow [INFO] / colours entire text (depending on ShouldUseChatPrefixes()) and sends message to player. Informational message, such as command usage, etc.",
				},
				SendMessagePrivateMsg =
				{
					Params =
					{
						{
							Name = "Message",
							Type = "string",
						},
						{
							Name = "SenderName",
							Type = "string",
						},
					},
					Notes = "Prepends Light Blue [MSG: *SenderName*] / prepends SenderName and colours entire text (depending on ShouldUseChatPrefixes()) and sends message to player. For private messaging.",
				},
				SendMessageSuccess =
				{
					Params =
					{
						{
							Name = "Message",
							Type = "string",
						},
					},
					Notes = "Prepends Green [INFO] / colours entire text (depending on ShouldUseChatPrefixes()) and sends message to player. Success notification.",
				},
				SendMessageWarning =
				{
					Params =
					{
						{
							Name = "Message",
							Type = "string",
						},
					},
					Notes = "Prepends Rose [WARN] / colours entire text (depending on ShouldUseChatPrefixes()) and sends message to player. Denotes that something concerning, such as plugin reload, is about to happen.",
				},
				SendRotation =
				{
					Params =
					{
						{
							Name = "YawDegrees",
							Type = "number",
						},
						{
							Name = "PitchDegrees",
							Type = "number",
						},
					},
					Notes = "Sends the specified rotation to the player, forcing them to look that way",
				},
				SendSystemMessage =
				{
					Params =
					{
						{
							Name = "Message",
							Type = "string",
						},
					},
					Notes = "Sends the specified message to the player.",
				},
				SetBedPos =
				{
					Params =
					{
						{
							Name = "Position",
							Type = "Vector3i",
						},
						{
							Name = "World",
							Type = "cWorld",
							IsOptional = true,
						},
					},
					Notes = "Sets the position and world of the player's respawn point, which is also known as the bed position. The player will respawn at this position and world upon death. If the world is not specified, it is set to the player's current world.",
				},
				SetCanFly =
				{
					Params =
					{
						{
							Name = "CanFly",
							Type = "boolean",
						},
					},
					Notes = "Sets if the player can fly or not.",
				},
				SetCrouch =
				{
					Params =
					{
						{
							Name = "IsCrouched",
							Type = "boolean",
						},
					},
					Notes = "Sets the crouch state, broadcasts the change to other players.",
				},
				SetCurrentExperience =
				{
					Params =
					{
						{
							Name = "XPAmount",
							Type = "number",
						},
					},
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Sets the current amount of experience (and indirectly, the XP level).",
				},
				SetCustomName =
				{
					Params =
					{
						{
							Name = "CustomName",
							Type = "string",
						},
					},
					Notes = "Sets the custom name for this player. If you want to disable the custom name, simply set an empty string. The custom name will be used in the tab-list, in the player nametag and in the tab-completion.",
				},
				SetDraggingItem =
				{
					Params =
					{
						{
							Name = "Item",
							Type = "cItem",
						},
					},
					Notes = "Sets the item that the player is dragging in a UI window. If no UI window is open, this function does nothing."
				},
				SetFlying =
				{
					Params =
					{
						{
							Name = "IsFlying",
							Type = "boolean",
						},
					},
					Notes = "Sets if the player is flying or not.",
				},
				SetFlyingMaxSpeed =
				{
					Params =
					{
						{
							Name = "FlyingMaxSpeed",
							Type = "number",
						},
					},
					Notes = "Sets the flying maximum speed, relative to the game default speed. The default value is 1. Sends the updated speed to the client.",
				},
				SetFoodExhaustionLevel =
				{
					Params =
					{
						{
							Name = "ExhaustionLevel",
							Type = "number",
						},
					},
					Notes = "Sets the food exhaustion to the specified level.",
				},
				SetFoodLevel =
				{
					Params =
					{
						{
							Name = "FoodLevel",
							Type = "number",
						},
					},
					Notes = "Sets the food level (number of half-drumsticks on-screen)",
				},
				SetFoodSaturationLevel =
				{
					Params =
					{
						{
							Name = "FoodSaturationLevel",
							Type = "number",
						},
					},
					Notes = "Sets the food saturation (overcharge of the food level).",
				},
				SetFoodTickTimer =
				{
					Params =
					{
						{
							Name = "FoodTickTimer",
							Type = "number",
						},
					},
					Notes = "Sets the number of ticks past the last food-based heal or damage action; when this timer reaches 80, a new heal / damage is applied.",
				},
				SetGameMode =
				{
					Params =
					{
						{
							Name = "NewGameMode",
							Type = "eGameMode",
						},
					},
					Notes = "Sets the gamemode for the player. The new gamemode overrides the world's default gamemode, unless it is set to gmInherit.",
				},
				SetIsFishing =
				{
					Params =
					{
						{
							Name = "IsFishing",
							Type = "boolean",
						},
						{
							Name = "FloaterEntityID",
							Type = "number",
							IsOptional = true,
						},
					},
					Notes = "Sets the 'IsFishing' flag for the player. The floater entity ID is expected for the true variant, it can be omitted when IsFishing is false. FIXME: Undefined behavior when multiple fishing rods are used simultanously",
				},
				SetMainHand =
				{
					Params =
					{
						{
							Name = "Hand",
							Type = "eMainHand",
						},
					},
					Notes = "Sets the main hand of the player.",
				},
				SetName =
				{
					Params =
					{
						{
							Name = "Name",
							Type = "string",
						},
					},
					Notes = "Sets the player name. This rename will NOT be visible to any players already in the server who are close enough to see this player.",
				},
				SetNormalMaxSpeed =
				{
					Params =
					{
						{
							Name = "NormalMaxSpeed",
							Type = "number",
						},
					},
					Notes = "Sets the normal (walking) maximum speed, relative to the game default speed. The default value is 1. Sends the updated speed to the client, if appropriate.",
				},
				SetSprint =
				{
					Params =
					{
						{
							Name = "IsSprinting",
							Type = "boolean",
						},
					},
					Notes = "Sets whether the player is sprinting or not.",
				},
				SetSkinParts =
				{
					Params =
					{
						{
							Name = "Parts",
							Type = "number",
						},
					},
					Notes = "Sets the skin part flags of the player.  The value should be a bitwise OR of several {{Globals#eSkinPart|eSkinPart}} constants.",
				},
				SetSprintingMaxSpeed =
				{
					Params =
					{
						{
							Name = "SprintingMaxSpeed",
							Type = "number",
						},
					},
					Notes = "Sets the sprinting maximum speed, relative to the game default speed. The default value is 1.3. Sends the updated speed to the client, if appropriate.",
				},
				SetTeam =
				{
					Params =
					{
						{
							Name = "Team",
							Type = "cTeam",
						},
					},
					Notes = "Moves the player to the specified team.",
				},
				SetVisible =
				{
					Params =
					{
						{
							Name = "IsVisible",
							Type = "boolean",
						},
					},
					Notes = "Sets the player visibility to other players",
				},
				SpectateEntity =
				{
					Params =
					{
						{
							Name = "Target",
							Type = "cEntity",
						},
					},
					Notes = "Spectates the target entity. Does not change the player's gamemode to spectator mode. When called with self or nil as the target, resets the spectation.",
				},
				TossEquippedItem =
				{
					Params =
					{
						{
							Name = "Amount",
							Type = "number",
							IsOptional = true,
						},
					},
					Notes = "Tosses the item that the player has selected in their hotbar. Amount defaults to 1.",
				},
				TossHeldItem =
				{
					Params =
					{
						{
							Name = "Amount",
							Type = "number",
							IsOptional = true,
						},
					},
					Notes = "Tosses the item held by the cursor, when the player is in a UI window. Amount defaults to 1.",
				},
				TossPickup =
				{
					Params =
					{
						{
							Name = "Item",
							Type = "cItem",
						},
					},
					Notes = "Tosses a pickup newly created from the specified item.",
				},
				Unfreeze =
				{
					Notes = "Allows the player to move again, canceling the effects of Freeze()",
				},
				XpForLevel =
				{
					IsStatic = true,
					Params =
					{
						{
							Name = "XPLevel",
							Type = "number",
						},
					},
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the total amount of XP needed for the specified XP level. Inverse of CalcLevelFromXp().",
				},
			},
			Constants =
			{
				EATING_TICKS =
				{
					Notes = "Number of ticks required for consuming an item.",
				},
				MAX_FOOD_LEVEL =
				{
					Notes = "The maximum food level value. When the food level is at this value, the player cannot eat.",
				},
				MAX_HEALTH =
				{
					Notes = "The maximum health value",
				},
			},
			Inherits = "cPawn",
		},  -- cPlayer

		cRoot =
		{
			Desc = [[
				This class represents the root of Cuberite's object hierarchy. There is always only one cRoot
				object. It manages and allows querying all the other objects, such as {{cServer}},
				{{cPluginManager}}, individual {{cWorld|worlds}} etc.</p>
				<p>
				To get the singleton instance of this object, you call the cRoot:Get() function. Then you can call
				the individual functions on this object. Note that some of the functions are static and don't need
				the instance, they are to be called directly on the cRoot class, such as cRoot:GetPhysicalRAMUsage()
			]],
			Functions =
			{
				BroadcastChat =
				{
					{
						Params =
						{
							{
								Name = "MessageText",
								Type = "string",
							},
							{
								Name = "MessageType",
								Type = "eMessageType",
							},
						},
						Notes = "Broadcasts a message to all players, with its message type set to MessageType (default: mtCustom).",
					},
					{
						Params =
						{
							{
								Name = "CompositeChat",
								Type = "cCompositeChat",
							},
						},
						Notes = "Broadcasts a {{cCompositeChat|composite chat message}} to all players.",
					},
				},
				BroadcastChatDeath =
				{
					Params =
					{
						{
							Name = "MessageText",
							Type = "string",
						},
					},
					Notes = "Broadcasts the specified message to all players, with its message type set to mtDeath. Use for when a player has died.",
				},
				BroadcastChatFailure =
				{
					Params =
					{
						{
							Name = "MessageText",
							Type = "string",
						},
					},
					Notes = "Broadcasts the specified message to all players, with its message type set to mtFailure. Use for a command that failed to run because of insufficient permissions, etc.",
				},
				BroadcastChatFatal =
				{
					Params =
					{
						{
							Name = "MessageText",
							Type = "string",
						},
					},
					Notes = "Broadcasts the specified message to all players, with its message type set to mtFatal. Use for a plugin that crashed, or similar.",
				},
				BroadcastChatInfo =
				{
					Params =
					{
						{
							Name = "MessageText",
							Type = "string",
						},
					},
					Notes = "Broadcasts the specified message to all players, with its message type set to mtInfo. Use for informational messages, such as command usage.",
				},
				BroadcastChatJoin =
				{
					Params =
					{
						{
							Name = "MessageText",
							Type = "string",
						},
					},
					Notes = "Broadcasts the specified message to all players, with its message type set to mtJoin. Use for players joining the server.",
				},
				BroadcastChatLeave =
				{
					Params =
					{
						{
							Name = "MessageText",
							Type = "string",
						},
					},
					Notes = "Broadcasts the specified message to all players, with its message type set to mtLeave. Use for players leaving the server.",
				},
				BroadcastChatSuccess =
				{
					Params =
					{
						{
							Name = "MessageText",
							Type = "string",
						},
					},
					Notes = "Broadcasts the specified message to all players, with its message type set to mtSuccess. Use for success messages.",
				},
				BroadcastChatWarning =
				{
					Params =
					{
						{
							Name = "MessageText",
							Type = "string",
						},
					},
					Notes = "Broadcasts the specified message to all players, with its message type set to mtWarning. Use for concerning events, such as plugin reload etc.",
				},
				DoWithPlayerByUUID =
				{
					Params =
					{
						{
							Name = "PlayerUUID",
							Type = "cUUID",
						},
						{
							Name = "CallbackFunction",
							Type = "function",
						},
					},
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "If there is the player with the uuid, calls the CallbackFunction with the {{cPlayer}} parameter representing the player. The CallbackFunction has the following signature: <pre class=\"prettyprint lang-lua\">function Callback({{cPlayer|Player}})</pre> The function returns false if the player was not found, or whatever bool value the callback returned if the player was found.",
				},
				FindAndDoWithPlayer =
				{
					Params =
					{
						{
							Name = "PlayerName",
							Type = "string",
						},
						{
							Name = "CallbackFunction",
							Type = "function",
						},
					},
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Calls the given callback function for the player with the name best matching the name string provided.<br>This function is case-insensitive and will match partial names.<br>Returns false if player not found or there is ambiguity, true otherwise. The CallbackFunction has the following signature: <pre class=\"prettyprint lang-lua\">function Callback({{cPlayer|Player}})</pre>",
				},
				ForEachPlayer =
				{
					Params =
					{
						{
							Name = "CallbackFunction",
							Type = "function",
						},
					},
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Calls the given callback function for each player. The callback function has the following signature: <pre class=\"prettyprint lang-lua\">function Callback({{cPlayer|cPlayer}})</pre>",
				},
				ForEachWorld =
				{
					Params =
					{
						{
							Name = "CallbackFunction",
							Type = "function",
						},
					},
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Calls the given callback function for each world. The callback function has the following signature: <pre class=\"prettyprint lang-lua\">function Callback({{cWorld|cWorld}})</pre>. Returns false if a callback aborts, otherwise true.",
				},
				Get =
				{
					IsStatic = true,
					Returns =
					{
						{
							Type = "cRoot",
						},
					},
					Notes = "Returns the one and only cRoot object.",
				},
				GetBrewingRecipe =
				{
					IsStatic = true,
					Params =
					{
						{
							Name = "Bottle",
							Type = "cItem",
						},
						{
							Name = "Ingredient",
							Type = "cItem",
						},
					},
					Returns =
					{
						{
							Type = "cItem",
						},
					},
					Notes = "Returns the result item, if a recipe has been found to brew the Ingredient into Bottle. If no recipe is found, returns no value.",
				},
				GetBuildCommitID =
				{
					IsStatic = true,
					Returns =
					{
						{
							Type = "string",
						},
					},
					Notes = "For official builds (Travis CI / Jenkins) it returns the exact commit hash used for the build. For unofficial local builds, returns the approximate commit hash (since the true one cannot be determined), formatted as \"approx: &lt;CommitHash&gt;\".",
				},
				GetBuildDateTime =
				{
					IsStatic = true,
					Returns =
					{
						{
							Type = "string",
						},
					},
					Notes = "For official builds (Travic CI / Jenkins) it returns the date and time of the build. For unofficial local builds, returns the approximate datetime of the commit (since the true one cannot be determined), formatted as \"approx: &lt;DateTime-iso8601&gt;\".",
				},
				GetBuildID =
				{
					IsStatic = true,
					Returns =
					{
						{
							Type = "string",
						},
					},
					Notes = "For official builds (Travis CI / Jenkins) it returns the unique ID of the build, as recognized by the build system. For unofficial local builds, returns the string \"Unknown\".",
				},
				GetBuildSeriesName =
				{
					IsStatic = true,
					Returns =
					{
						{
							Type = "string",
						},
					},
					Notes = "For official builds (Travis CI / Jenkins) it returns the series name of the build (for example \"Cuberite Windows x64 Master\"). For unofficial local builds, returns the string \"local build\".",
				},
				GetCraftingRecipes =
				{
					Returns =
					{
						{
							Type = "cCraftingRecipe",
						},
					},
					Notes = "Returns the CraftingRecipes object",
				},
				GetDefaultWorld =
				{
					Returns =
					{
						{
							Type = "cWorld",
						},
					},
					Notes = "Returns the world object from the default world.",
				},
				GetFurnaceFuelBurnTime =
				{
					IsStatic = true,
					Params =
					{
						{
							Name = "Fuel",
							Type = "cItem",
						},
					},
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the number of ticks for how long the item would fuel a furnace. Returns zero if not a fuel.",
				},
				GetFurnaceRecipe =
				{
					IsStatic = true,
					Params =
					{
						{
							Name = "InItem",
							Type = "cItem",
						},
					},
					Returns =
					{
						{
							Name = "OutItem",
							Type = "cItem",
						},
						{
							Name = "NumTicks",
							Type = "number",
						},
						{
							Name = "InItem",
							Type = "cItem",
						},
					},
					Notes = "Returns the furnace recipe for smelting the specified input. If a recipe is found, returns the smelted result, the number of ticks required for the smelting operation, and the input consumed (note that Cuberite supports smelting M items into N items and different smelting rates). If no recipe is found, returns no value.",
				},
				GetPhysicalRAMUsage =
				{
					IsStatic = true,
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the amount of physical RAM that the entire Cuberite process is using, in KiB. Negative if the OS doesn't support this query.",
				},
				GetPluginManager =
				{
					Returns =
					{
						{
							Type = "cPluginManager",
						},
					},
					Notes = "Returns the cPluginManager object.",
				},
				GetProtocolVersionTextFromInt =
				{
					IsStatic = true,
					Params =
					{
						{
							Name = "ProtocolVersionNumber",
							Type = "number",
						},
					},
					Returns =
					{
						{
							Type = "string",
						},
					},
					Notes = "Returns the Minecraft client version from the given Protocol version number. If there is no version found, it returns 'Unknown protocol (Number)'",
				},
				GetServer =
				{
					Returns =
					{
						{
							Type = "cServer",
						},
					},
					Notes = "Returns the cServer object.",
				},
				GetServerUpTime =
				{
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the uptime of the server in seconds.",
				},
				GetTotalChunkCount =
				{
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the amount of loaded chunks.",
				},
				GetVirtualRAMUsage =
				{
					IsStatic = true,
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the amount of virtual RAM that the entire Cuberite process is using, in KiB. Negative if the OS doesn't support this query.",
				},
				GetWebAdmin =
				{
					Returns =
					{
						{
							Type = "cWebAdmin",
						},
					},
					Notes = "Returns the cWebAdmin object.",
				},
				GetWorld =
				{
					Params =
					{
						{
							Name = "WorldName",
							Type = "string",
						},
					},
					Returns =
					{
						{
							Type = "cWorld",
						},
					},
					Notes = "Returns the cWorld object of the given world. It returns nil if there is no world with the given name.",
				},
				QueueExecuteConsoleCommand =
				{
					Params =
					{
						{
							Name = "Message",
							Type = "string",
						},
					},
					Notes = "Queues a console command for execution through the cServer class. The command will be executed in the tick thread. The command's output will be sent to console.",
				},
				SaveAllChunks =
				{
					Notes = "Saves all the chunks in all the worlds. Note that the saving is queued on each world's tick thread and this functions returns before the chunks are actually saved.",
				},
				SetSavingEnabled =
				{
					Params =
					{
						{
							Name = "SavingEnabled",
							Type = "boolean",
						},
					},
					Notes = "Sets whether saving chunk data is enabled for all worlds. If disabled, dirty chunks will stay in memory forever, which can cause performance and stability issues.",
				},
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
		},
		cScoreboard =
		{
			Desc = [[
				This class manages the objectives and teams of a single world.
			]],
			Functions =
			{
				AddPlayerScore =
				{
					Params =
					{
						{
							Name = "Name",
							Type = "string",
						},
						{
							Name = "Type",
							Type = "<unknown>",
						},
						{
							Name = "Value",
							Type = "<unknown>",
						},
					},
					Notes = "Adds a value to all player scores of the specified objective type.",
				},
				ForEachObjective =
				{
					Params =
					{
						{
							Name = "CallbackFunction",
							Type = "function",
						},
					},
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Calls the specified callback for each objective in the scoreboard. Returns true if all objectives have been processed (including when there are zero objectives), or false if the callback function has aborted the enumeration by returning true. The callback function has the following signature: <pre class=\"prettyprint lang-lua\">function Callback({{cObjective|Objective}})</pre> The callback should return false or no value to continue with the next objective, or true to abort the enumeration.",
				},
				ForEachTeam =
				{
					Params =
					{
						{
							Name = "CallbackFunction",
							Type = "function",
						},
					},
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Calls the specified callback for each team in the scoreboard. Returns true if all teams have been processed (including when there are zero teams), or false if the callback function has aborted the enumeration by returning true. The callback function has the following signature: <pre class=\"prettyprint lang-lua\">function Callback({{cObjective|Objective}})</pre> The callback should return false or no value to continue with the next team, or true to abort the enumeration.",
				},
				GetNumObjectives =
				{
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the nuber of registered objectives.",
				},
				GetNumTeams =
				{
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the number of registered teams.",
				},
				GetObjective =
				{
					Params =
					{
						{
							Name = "string",
							Type = "string",
						},
					},
					Returns =
					{
						{
							Type = "cObjective",
						},
					},
					Notes = "Returns the objective with the specified name.",
				},
				GetObjectiveIn =
				{
					Params =
					{
						{
							Name = "DisplaySlot",
							Type = "<unknown>",
						},
					},
					Returns =
					{
						{
							Type = "cObjective",
						},
					},
					Notes = "Returns the objective in the specified display slot. Can be nil.",
				},
				GetTeam =
				{
					Params =
					{
						{
							Name = "TeamName",
							Type = "string",
						},
					},
					Returns =
					{
						{
							Type = "cTeam",
						},
					},
					Notes = "Returns the team with the specified name.",
				},
				GetTeamNames =
				{
					Returns =
					{
						{
							Type = "table",
						},
					},
					Notes = "Returns the names of all teams, as an array-table of strings",
				},
				RegisterObjective =
				{
					Params =
					{
						{
							Name = "Name",
							Type = "string",
						},
						{
							Name = "DisplayName",
							Type = "string",
						},
						{
							Name = "Type",
							Type = "<unknown>",
						},
					},
					Returns =
					{
						{
							Type = "cObjective",
						},
					},
					Notes = "Registers a new scoreboard objective. Returns the {{cObjective}} instance, nil on error.",
				},
				RegisterTeam =
				{
					Params =
					{
						{
							Name = "Name",
							Type = "string",
						},
						{
							Name = "DisplayName",
							Type = "string",
						},
						{
							Name = "Prefix",
							Type = "<unknown>",
						},
						{
							Name = "Suffix",
							Type = "<unknown>",
						},
					},
					Returns =
					{
						{
							Type = "cTeam",
						},
					},
					Notes = "Registers a new team. Returns the {{cTeam}} instance, nil on error.",
				},
				RemoveObjective =
				{
					Params =
					{
						{
							Name = "Name",
							Type = "string",
						},
					},
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Removes the objective with the specified name. Returns true if operation was successful.",
				},
				RemoveTeam =
				{
					Params =
					{
						{
							Name = "TeamName",
							Type = "string",
						},
					},
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Removes the team with the specified name. Returns true if operation was successful.",
				},
				SetDisplay =
				{
					Params =
					{
						{
							Name = "Name",
							Type = "string",
						},
						{
							Name = "DisplaySlot",
							Type = "<unknown>",
						},
					},
					Notes = "Updates the currently displayed objective.",
				},
			},
			Constants =
			{
				dsCount =
				{
					Notes = "",
				},
				dsList =
				{
					Notes = "",
				},
				dsName =
				{
					Notes = "",
				},
				dsSidebar =
				{
					Notes = "",
				},
			},
		},
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
				DoesAllowMultiLogin =
				{
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Returns true if players can log in multiple times from the same account (normally used for debugging), false if only one player per name is allowed.",
				},
				GetDescription =
				{
					Returns =
					{
						{
							Type = "string",
						},
					},
					Notes = "Returns the server description set in the settings.ini.",
				},
				GetShutdownMessage =
				{
					Returns =
					{
						{
							Type = "string",
						},
					},
					Notes = "Returns the shutdown message set in the settings.ini.",
				},
				GetMaxPlayers =
				{
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the max amount of players who can join the server.",
				},
				GetNumPlayers =
				{
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the amount of players online.",
				},
				GetServerID =
				{
					Returns =
					{
						{
							Type = "string",
						},
					},
					Notes = "Returns the ID of the server?",
				},
				IsHardcore =
				{
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Returns true if the server is hardcore (players get banned on death).",
				},
				IsPlayerInQueue =
				{
					Params =
					{
						{
							Name = "Username",
							Type = "string",
						},
					},
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Returns true if the specified player is queued to be transferred to a World.",
				},
				RegisterForgeMod =
				{
					Params =
					{
						{
							Name = "ModName",
							Type = "string",
						},
						{
							Name = "ModVersion",
							Type = "string",
						},
						{
							Name = "ProtocolVersionNumber",
							Type = "number",
						},
					},
					Notes = "Add a Forge mod name/version to the server ping list.",
				},
				SetMaxPlayers =
				{
					Params =
					{
						{
							Name = "MaxPlayers",
							Type = "number",
						},
					},
					Notes = "Sets the max amount of players who can join.",
				},
				ShouldAuthenticate =
				{
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Returns true iff the server is set to authenticate players (\"online mode\").",
				},
				UnregisterForgeMod =
				{
					Params =
					{
						{
							Name = "ModName",
							Type = "string",
						},
						{
							Name = "ProtocolVersionNumber",
							Type = "number",
						},
					},
					Notes = "Remove a Forge mod name/version from the server ping list.",
				},
			},
		},
		cStringCompression =
		{
			Desc = [[
				Provides functions to compress or decompress string
				<p>
				All functions in this class are static, so they should be called in the dot convention:
<pre class="prettyprint lang-lua">
local CompressedString = cStringCompression.CompressStringGZIP("DataToCompress")
</pre>
			]],
			Functions =
			{
				CompressStringGZIP =
				{
					IsStatic = true,
					Params =
					{
						{
							Name = "Data",
							Type = "string",
						},
					},
					Returns =
					{
						{
							Type = "string",
						},
					},
					Notes = "Compresses data in a string using GZIP",
				},
				CompressStringZLIB =
				{
					IsStatic = true,
					Params =
					{
						{
							Name = "Data",
							Type = "string",
						},
						{
							Name = "factor",
							Type = "number",
						},
					},
					Returns =
					{
						{
							Type = "string",
						},
					},
					Notes = "Compresses data in a string using ZLIB. Factor 0 is no compression and factor 9 is maximum compression.",
				},
				InflateString =
				{
					IsStatic = true,
					Params =
					{
						{
							Name = "Data",
							Type = "string",
						},
					},
					Returns =
					{
						{
							Type = "string",
						},
					},
					Notes = "Uncompresses a string using Inflate",
				},
				UncompressStringGZIP =
				{
					IsStatic = true,
					Params =
					{
						{
							Name = "Data",
							Type = "string",
						},
					},
					Returns =
					{
						{
							Type = "string",
						},
					},
					Notes = "Uncompress a string using GZIP",
				},
				UncompressStringZLIB =
				{
					IsStatic = true,
					Params =
					{
						{
							Name = "Data",
							Type = "string",
						},
						{
							Name = "UncompressedLength",
							Type = "number",
						},
					},
					Returns =
					{
						{
							Type = "string",
						},
					},
					Notes = "Uncompresses Data using ZLIB",
				},
			},
		},
		cTeam =
		{
			Desc = [[
				This class manages a single player team.
			]],
			Functions =
			{
				AddPlayer =
				{
					Params =
					{
						{
							Name = "PlayerName",
							Type = "string",
						},
					},
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Adds a player to this team. Returns true if the operation was successful.",
				},
				AllowsFriendlyFire =
				{
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Returns whether team friendly fire is allowed.",
				},
				CanSeeFriendlyInvisible =
				{
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Returns whether players can see invisible teammates.",
				},
				GetDisplayName =
				{
					Returns =
					{
						{
							Type = "string",
						},
					},
					Notes = "Returns the display name of the team.",
				},
				GetName =
				{
					Returns =
					{
						{
							Type = "string",
						},
					},
					Notes = "Returns the internal name of the team.",
				},
				GetNumPlayers =
				{
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the number of registered players.",
				},
				GetPrefix =
				{
					Returns =
					{
						{
							Type = "string",
						},
					},
					Notes = "Returns the prefix prepended to the names of the members of this team.",
				},
				GetSuffix =
				{
					Returns =
					{
						{
							Type = "string",
						},
					},
					Notes = "Returns the suffix appended to the names of the members of this team.",
				},
				HasPlayer =
				{
					Params =
					{
						{
							Name = "PlayerName",
							Type = "string",
						},
					},
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Returns whether the specified player is a member of this team.",
				},
				RemovePlayer =
				{
					Params =
					{
						{
							Name = "PlayerName",
							Type = "string",
						},
					},
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Removes the player with the specified name from this team. Returns true if the operation was successful.",
				},
				Reset =
				{
					Notes = "Removes all players from this team.",
				},
				SetCanSeeFriendlyInvisible =
				{
					Params =
					{
						{
							Name = "CanSeeFriendlyInvisible",
							Type = "boolean",
						},
					},
					Notes = "Set whether players can see invisible teammates.",
				},
				SetDisplayName =
				{
					Params =
					{
						{
							Name = "DisplayName",
							Type = "string",
						},
					},
					Notes = "Sets the display name of this team. (i.e. what will be shown to the players)",
				},
				SetFriendlyFire =
				{
					Params =
					{
						{
							Name = "AllowFriendlyFire",
							Type = "boolean",
						},
					},
					Notes = "Sets whether team friendly fire is allowed.",
				},
				SetPrefix =
				{
					Params =
					{
						{
							Name = "Prefix",
							Type = "string",
						},
					},
					Notes = "Sets the prefix prepended to the names of the members of this team.",
				},
				SetSuffix =
				{
					Params =
					{
						{
							Name = "Suffix",
							Type = "string",
						},
					},
					Notes = "Sets the suffix appended to the names of the members of this team.",
				},
			},
		},
		cTNTEntity =
		{
			Desc = "This class manages a TNT entity.",
			Functions =
			{
				Explode =
				{
					Notes = "Explode the tnt.",
				},
				GetFuseTicks =
				{
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the fuse ticks - the number of game ticks until the tnt explodes.",
				},
				SetFuseTicks =
				{
					Params =
					{
						{
							Name = "TicksUntilExplosion",
							Type = "number",
						},
					},
					Notes = "Set the fuse ticks until the tnt will explode.",
				},
			},
			Inherits = "cEntity",
		},
		cUrlParser =
		{
			Desc = [[
			Provides a parser for generic URLs that returns the individual components of the URL.</p>
			<p>
			Note that all functions are static. Call them by using "cUrlParser:Parse(...)" etc.
			]],
			Functions =
			{
				GetDefaultPort =
				{
					IsStatic = true,
					Params =
					{
						{
							Name = "Scheme",
							Type = "string",
						},
					},
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the default port that should be used for the given scheme (protocol). Returns zero if the scheme is not known.",
				},
				IsKnownScheme =
				{
					IsStatic = true,
					Params =
					{
						{
							Name = "Scheme",
							Type = "string",
						},
					},
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Returns true if the scheme (protocol) is recognized by the parser.",
				},
				Parse =
				{
					IsStatic = true,
					Params =
					{
						{
							Name = "URL",
							Type = "string",
						},
					},
					Returns =
					{
						{
							Name = "Scheme",
							Type = "string",
						},
						{
							Name = "Username",
							Type = "string",
						},
						{
							Name = "Password",
							Type = "string",
						},
						{
							Name = "Host",
							Type = "string",
						},
						{
							Name = "Port",
							Type = "number",
						},
						{
							Name = "Path",
							Type = "string",
						},
						{
							Name = "Query",
							Type = "string",
						},
						{
							Name = "Fragment",
							Type = "string",
						},
					},
					Notes = "Returns the individual parts of the URL. Parts that are not explicitly specified in the URL are empty, the default port for the scheme is used. If parsing fails, the function returns nil and an error message.",
				},
				ParseAuthorityPart =
				{
					IsStatic = true,
					Params =
					{
						{
							Name = "AuthPart",
							Type = "string",
						},
					},
					Returns =
					{
						{
							Name = "Username",
							Type = "string",
						},
						{
							Name = "Password",
							Type = "string",
						},
						{
							Name = "Host",
							Type = "string",
						},
						{
							Name = "Port",
							Type = "number",
						},
					},
					Notes = "Parses the Authority part of the URL. Parts that are not explicitly specified in the AuthPart are returned empty, the port is returned zero. If parsing fails, the function returns nil and an error message.",
				},
				UrlDecode =
				{
					IsStatic = true,
					Params =
					{
						{
							Name = "Text",
							Type = "string",
						},
					},
					Returns =
					{
						{
							Name = "Decoded",
							Type = "string",
						},
					},
					Notes = "Returns the Text, URL-decoded. Returns nil if there is a problem while decoding (invalid input).",
				},
				UrlEncode =
				{
					IsStatic = true,
					Params =
					{
						{
							Name = "Text",
							Type = "string",
						},
					},
					Returns =
					{
						{
							Name = "Encoded",
							Type = "string",
						},
					},
					Notes = "Returns the Text, URL-encoded.",
				},
			},
			AdditionalInfo =
			{
				{
					Header = "Code example",
					Contents = [[
						The following code fragment uses the cUrlParser to parse an URL string into its components, and
						prints those components out:
<pre class="prettyprint lang-lua">
local Scheme, Username, Password, Host, Port, Path, Query, Fragment = cUrlParser:Parse(
	"http://anonymous:user@example.com@ftp.cuberite.org:9921/releases/2015/?sort=date#files"
)
if not(Scheme) then
	-- Parsing failed, the second returned value (in Username) is the error message:
	LOG("  Error: " .. (Username or "<nil>"))
else
	LOG("  Scheme   = " .. Scheme)    -- "http"
	LOG("  Username = " .. Username)  -- "anonymous"
	LOG("  Password = " .. Password)  -- "user@example.com"
	LOG("  Host     = " .. Host)      -- "ftp.cuberite.org"
	LOG("  Port     = " .. Port)      -- 9921
	LOG("  Path     = " .. Path)      -- "releases/2015/"
	LOG("  Query    = " .. Query)     -- "sort=date"
	LOG("  Fragment = " .. Fragment)  -- "files"
end
</pre>
					]],
				},
			},
		},
		cUUID =
		{
			Desc = [[
				Class representing a Universally Unique Identifier.
				Note that all Cuberite's API functions that take a cUUID parameter will also
				accept a string in its place, as long as that string can be converted to a cUUID
				(using the {{#FromString_1|cUUID:FromString}} function).
			]],
			Functions =
			{
				constructor =
				{
					Returns =
					{
						{
							Type = "cUUID",
						},
					},
					Notes = "Constructs a nil-valued UUID (all zeros)",
				},
				Compare =
				{
					Params =
					{
						{
							Name = "Other",
							Type = "cUUID",
						},
					},
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = [[
						Compares this UUID with the specified Other UUID, Returns:
							0 when equal to Other,
							< 0 when less than Other,
							> 0 when greater than Other
					]],
				},
				IsNil =
				{
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Returns true if this contains the \"nil\" UUID with all bits set to 0",
				},
				FromString =
				{
					Params =
					{
						{
							Name = "StringUUID",
							Type = "string",
						},
					},
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Tries to interpret the string as a short or long form UUID and assign from it.	On error, returns false and does not set the value.",
				},
				ToShortString =
				{
					Returns =
					{
						{
							Type = "string",
						},
					},
					Notes = "Converts the UUID to a short form string (i.e without dashes).",
				},
				ToLongString =
				{
					Returns =
					{
						{
							Type = "string",
						},
					},
					Notes = "Converts the UUID to a long form string (i.e with dashes).",
				},
				Version =
				{
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the version number of the UUID.",
				},
				Variant =
				{
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the variant number of the UUID",
				},
				GenerateVersion3 =
				{
					IsStatic = true,
					Params =
					{
						{
							Name = "Name",
							Type = "string",
						},
					},
					Returns =
					{
						{
							Type = "cUUID",
						},
					},
					Notes = "Generates a version 3, variant 1 UUID based on the md5 hash of Name."
				},
			},
		},
		cWebPlugin =
		{
			Desc = "",
			Functions =
			{

			},
		},
		cWindow =
		{
			Desc = [[
				This class is the common ancestor for all window classes used by Cuberite. It is inherited by the
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
				GetSlot =
				{
					Params =
					{
						{
							Name = "Player",
							Type = "cPlayer",
						},
						{
							Name = "SlotNumber",
							Type = "number",
						},
					},
					Returns =
					{
						{
							Type = "cItem",
						},
					},
					Notes = "Returns the item at the specified slot for the specified player. Returns nil and logs to server console on error.",
				},
				GetWindowID =
				{
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the ID of the window, as used by the network protocol",
				},
				GetWindowTitle =
				{
					Returns =
					{
						{
							Type = "string",
						},
					},
					Notes = "Returns the window title that will be displayed to the player",
				},
				GetWindowType =
				{
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the type of the window, one of the constants in the table above",
				},
				GetWindowTypeName =
				{
					Returns =
					{
						{
							Type = "string",
						},
					},
					Notes = "Returns the textual representation of the window's type, such as \"minecraft:chest\".",
				},
				IsSlotInPlayerHotbar =
				{
					Params =
					{
						{
							Name = "SlotNum",
							Type = "number",
						},
					},
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Returns true if the specified slot number is in the player hotbar",
				},
				IsSlotInPlayerInventory =
				{
					Params =
					{
						{
							Name = "SlotNum",
							Type = "number",
						},
					},
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Returns true if the specified slot number is in the player's main inventory or in the hotbar. Note that this returns false for armor slots!",
				},
				IsSlotInPlayerMainInventory =
				{
					Params =
					{
						{
							Name = "SlotNum",
							Type = "number",
						},
					},
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Returns true if the specified slot number is in the player's main inventory",
				},
				SetProperty =
				{
					Params =
					{
						{
							Name = "PropertyID",
							Type = "number",
						},
						{
							Name = "PropertyValue",
							Type = "number",
						},
						{
							Name = "Player",
							Type = "cPlayer",
							IsOptional = true,
						},
					},
					Notes = "Updates a numerical property associated with the window. Typically used for furnace progressbars. Sends the UpdateWindowProperty packet to the specified Player, or to all current clients of the window if Player is not specified.",
				},
				SetSlot =
				{
					Params =
					{
						{
							Name = "Player",
							Type = "cPlayer",
						},
						{
							Name = "SlotNum",
							Type = "number",
						},
						{
							Name = "cItem",
							Type = "cItem",
						},
					},
					Notes = "Sets the contents of the specified slot for the specified player. Ignored if the slot number is invalid",
				},
				SetWindowTitle =
				{
					Params =
					{
						{
							Name = "WindowTitle",
							Type = "string",
						},
					},
					Notes = "Sets the window title that will be displayed to the player",
				},
			},
			Constants =
			{
				wtAnimalChest =
				{
					Notes = "A horse or donkey window",
				},
				wtAnvil =
				{
					Notes = "An anvil window",
				},
				wtBeacon =
				{
					Notes = "A beacon window",
				},
				wtBrewery =
				{
					Notes = "A brewing stand window",
				},
				wtChest =
				{
					Notes = "A {{cChestEntity|chest}} or doublechest window",
				},
				wtDropSpenser =
				{
					Notes = "A {{cDropperEntity|dropper}} or a {{cDispenserEntity|dispenser}} window",
				},
				wtEnchantment =
				{
					Notes = "An enchantment table window",
				},
				wtFurnace =
				{
					Notes = "A {{cFurnaceEntity|furnace}} window",
				},
				wtHopper =
				{
					Notes = "A {{cHopperEntity|hopper}} window",
				},
				wtInventory =
				{
					Notes = "An inventory window",
				},
				wtNPCTrade =
				{
					Notes = "A villager trade window",
				},
				wtWorkbench =
				{
					Notes = "A workbench (crafting table) window",
				},
			},  -- Constants

			ConstantGroups =
			{
				WindowType =
				{
					Include = { "wt.*" },
				}
			},  -- ConstantGroups
		},  -- cWindow

		EffectID =
		{
			Desc = [[
				An enumeration of sound and particle effects which can be used with
				{{cWorld#BroadcastSoundParticleEffect|BroadcastSoundParticleEffect}}.
			]],
			Constants =
			{
				SFX_RANDOM_DISPENSER_DISPENSE =
				{
					Notes = "Sound of droper/dispenser releasing an item",
				},
				SFX_RANDOM_DISPENSER_DISPENSE_FAIL =
				{
					Notes = "Sound of a droper/dispenser activated without items",
				},
				SFX_RANDOM_DISPENSER_SHOOT =
				{
					Notes = "Sound of a dispenser shooting",
				},
				SFX_RANDOM_ENDER_EYE_LAUNCH =
				{
					Notes = "Sound of an ender eye launch"
				},
				SFX_RANDOM_FIREWORK_SHOT =
				{
					Notes = "Sound of a firework shot",
				},
				SFX_RANDOM_IRON_DOOR_OPEN =
				{
					Notes = "Sound of an iron door opening",
				},
				SFX_RANDOM_WOODEN_DOOR_OPEN =
				{
					Notes = "Sound of a wooden door opening"
				},
				SFX_RANDOM_WOODEN_TRAPDOOR_OPEN =
				{
					Notes = "Sound of a wooden trapdoor opening"
				},
				SFX_RANDOM_FENCE_GATE_OPEN =
				{
					Notes = "Sound of a fence gate opening",
				},
				SFX_RANDOM_FIRE_EXTINGUISH =
				{
					Notes = "Sound of a fire extinguishing",
				},
				SFX_RANDOM_PLAY_MUSIC_DISC =
				{
					Notes = "Starts playing a music disc. Needs an accompanting music disc ID",
				},
				SFX_RANDOM_IRON_DOOR_CLOSE =
				{
					Notes = "Sound of an iron door closing",
				},
				SFX_RANDOM_WOODEN_DOOR_CLOSE =
				{
					Notes = "Sound of a wooden door closing",
				},
				SFX_RANDOM_WOODEN_TRAPDOOR_CLOSE =
				{
					Notes = "Sound of a trapdoor closing",
				},
				SFX_RANDOM_FENCE_GATE_CLOSE =
				{
					Notes = "Sound of a fence gate closing",
				},
				SFX_MOB_GHAST_WARN =
				{
					Notes = "Sound of a ghast warning cry",
				},
				SFX_MOB_GHAST_SHOOT =
				{
					Notes = "Sound of a ghast shooting",
				},
				SFX_MOB_ENDERDRAGON_SHOOT =
				{
					Notes = "Sound of the enderdragon shooting",
				},
				SFX_MOB_BLAZE_SHOOT =
				{
					Notes = "Sound of a blaze shooting",
				},
				SFX_MOB_ZOMBIE_WOOD =
				{
					Notes = "Sound of a zombie attacking a wooden door",
				},
				SFX_MOB_ZOMBIE_METAL =
				{
					Notes = "Sound of a zombie attacking a metal door",
				},
				SFX_MOB_ZOMBIE_WOOD_BREAK =
				{
					Notes = "Sound of a zombie breaking a wooden door",
				},
				SFX_MOB_WITHER_BREAK_BLOCK =
				{
					Notes = "Sound of a wither breaking blocks",
				},
				SFX_MOB_WITHER_SPAWN =
				{
					Notes = "Sound of a wither spawning",
				},
				SFX_MOB_WITHER_SHOOT =
				{
					Notes = "Sound of a wither shooting",
				},
				SFX_MOB_BAT_TAKEOFF =
				{
					Notes = "Sound of a bat taking off",
				},
				SFX_MOB_ZOMBIE_INFECT =
				{
					Notes = "Sound of a zombie infecting a villager",
				},
				SFX_MOB_ZOMBIE_UNFECT =
				{
					Notes = "Sound of a zombie villager converting to villager",
				},
				SFX_MOB_ENDERDRAGON_DEATH =
				{
					Notes = "Sound of the dragon releasing dragon breath",
				},
				SFX_RANDOM_ANVIL_BREAK =
				{
					Notes = "Sound of an anvil breaking",
				},
				SFX_RANDOM_ANVIL_USE =
				{
					Notes = "Sound of using an anvil",
				},
				SFX_RANDOM_ANVIL_LAND =
				{
					Notes = "Sound of a falling anvil landing",
				},
				SFX_RANDOM_PORTAL_TRAVEL =
				{
					Notes = "Sound of travelling through a portal",
				},
				SFX_RANDOM_CHORUS_FLOWER_GROW =
				{
					Notes = "Sound of a growing chorus flower",
				},
				SFX_RANDOM_CHORUS_FLOWER_DEATH =
				{
					Notes = "Sound of a dieing chorus flower",
				},
				SFX_RANDOM_BREWING_STAND_BREW =
				{
					Notes = "Sound of an active brewing stand",
				},
				SFX_RANDOM_IRON_TRAPDOOR_OPEN =
				{
					Notes = "Sound of an iron trapdoor opening",
				},
				SFX_RANDOM_IRON_TRAPDOOR_CLOSE =
				{
					Notes = "Sound of an iron trapdoor closing",
				},
				PARTICLE_SMOKE =
				{
					Notes = "Spawns 10 smoke particles, e.g. from a fire.  Needs a {{SmokeDirection|SmokeDirection}}",
				},
				PARTICLE_BLOCK_BREAK =
				{
					Notes = "Block break particle and sound.  Needs a BlockID",
				},
				PARTICLE_SPLASH_POTION =
				{
					Notes = "Splash potion particles and glass break sound.  Needs a PotionID",
				},
				PARTICLE_EYE_OF_ENDER =
				{
					Notes = "Eye of ender entity break particles and sound",
				},
				PARTICLE_MOBSPAWN =
				{
					Notes = "Mob spawn particle effect: smoke and flames",
				},
				PARTICLE_HAPPY_VILLAGER =
				{
					Notes = "Happy villager/bonemeal particles.  Number of particles may be given or 0 for default of 15",
				},
				PARTICLE_DRAGON_BREATH =
				{
					Notes = "Dragon breath particle effect",
				},
				PARTICLE_END_GATEWAY_SPAWN =
				{
					Notes = "End gateway spawn particle effect",
				},
				PARTICLE_ENDERDRAGON_GROWL =
				{
					Notes = "Ender dragon growl particle effect",
				},
			},
		},
		SmokeDirection =
		{
			Desc = [[
				An enumeration of the direction spawned smoke will drift in as it floats up.
			]],
			Constants =
			{
				SOUTH_EAST =
				{
					Notes = "Smoke drifts south-east",
				},
				SOUTH =
				{
					Notes = "Smoke drifts south",
				},
				SOUTH_WEST =
				{
					Notes = "Smoke drifts south-west",
				},
				EAST =
				{
					Notes = "Smoke drifts east",
				},
				CENTRE =
				{
					Notes = "Smoke does not drift",
				},
				WEST =
				{
					Notes = "Smoke drifts west",
				},
				NORTH_EAST =
				{
					Notes = "Smoke drifts north-east",
				},
				NORTH =
				{
					Notes = "Smoke drifts north",
				},
				NORTH_WEST =
				{
					Notes = "Smoke drifts west",
				},
			}
		},
		Globals =
		{
			Desc = [[
				These functions are available directly, without a class instance. Any plugin can call them at any
				time.
			]],
			Functions =
			{
				AddFaceDirection =
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
								Name = "BlockFace",
								Type = "eBlockFace",
							},
							{
								Name = "IsInverse",
								Type = "boolean",
								IsOptional = true,
							},
						},
						Returns =
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
						Notes = "Returns the coords of a block adjacent to the specified block through the specified {{Globals#BlockFaces|face}}",
					},
					{
						Params =
						{
							{
								Name = "BlockPos",
								Type = "Vector3i",
							},
							{
								Name = "BlockFace",
								Type = "eBlockFace",
							},
							{
								Name = "IsInverse",
								Type = "boolean",
								IsOptional = true,
							},
						},
						Returns =
						{
							{
								Name = "BlockPos",
								Type = "Vector3i",
							},
						},
						Notes = "Returns the coords of a block adjacent to the specified block through the specified {{Globals#BlockFaces|face}}",
					},
				},
				Base64Decode =
				{
					Params =
					{
						{
							Name = "Input",
							Type = "string",
						},
					},
					Returns =
					{
						{ Type = "string" },
					},
					Notes = "Decodes a Base64-encoded string into the raw data.",
				},
				Base64Encode =
				{
					Params =
					{
						{
							Name = "Input",
							Type = "string",
						},
					},
					Returns =
					{
						{ Type = "string" },
					},
					Notes = "Encodes a string into Base64.",
				},
				BiomeToString =
				{
					Params =
					{
						{
							Name = "Biome",
							Type = "EMCSBiome",
						},
					},
					Returns =
					{
						{ Type = "string" },
					},
					Notes = "Translates biome enum into biome string. Returns empty string on failure (unknown biome).",
				},
				BlockFaceToString =
				{
					Params =
					{
						{
							Name = "eBlockFace",
							Type = "eBlockFace",
						},
					},
					Returns =
					{
						{
							Type = "string",
						},
					},
					Notes = "Returns the string representation of the {{Globals#BlockFaces|eBlockFace}} constant. Uses the axis-direction-based names, such as BLOCK_FACE_XP.",
				},
				BlockStringToType =
				{
					Params =
					{
						{
							Name = "BlockTypeString",
							Type = "string",
						},
					},
					Returns =
					{
						{
							Name = "BLOCKTYPE",
							Type = "number",
						},
					},
					Notes = "Returns the block type parsed from the given string",
				},
				Clamp =
				{
					Params =
					{
						{
							Name = "Number",
							Type = "number",
						},
						{
							Name = "Min",
							Type = "number",
						},
						{
							Name = "Max",
							Type = "number",
						},
					},
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Clamp the number to the specified range.",
				},
				ClickActionToString =
				{
					Params =
					{
						{
							Name = "ClickAction",
							Type = "eClickAction",
						},
					},
					Returns =
					{
						{
							Type = "string",
						},
					},
					Notes = "Returns a string description of the ClickAction enumerated value",
				},
				DamageTypeToString =
				{
					Params =
					{
						{
							Name = "DamageType",
							Type = "eDamageType",
						},
					},
					Returns =
					{
						{
							Type = "string",
						},
					},
					Notes = "Converts the {{Globals#eDamageType|DamageType}} to a string representation ",
				},
				DimensionToString =
				{
					Params =
					{
						{
							Name = "Dimension",
							Type = "eDimension",
						},
					},
					Returns =
					{
						{
							Type = "string",
						},
					},
					Notes = "Converts an {{Globals#eDimension|eDimension}} to a string value. Returns Overworld on failure.",
				},
				EscapeString =
				{
					Params =
					{
						{
							Name = "Input",
							Type = "string",
						},
					},
					Returns =
					{
						{
							Type = "string",
						},
					},
					Notes = "Returns a copy of the string with all quotes and backslashes escaped by a backslash",
				},
				GetIniItemSet =
				{
					Params =
					{
						{
							Name = "IniFile",
							Type = "cIniFile",
						},
						{
							Name = "SectionName",
							Type = "string",
						},
						{
							Name = "KeyName",
							Type = "string",
						},
						{
							Name = "DefaultValue",
							Type = "string",
						},
					},
					Returns =
					{
						{
							Type = "cItem",
						},
					},
					Notes = "Returns the item that has been read from the specified INI file value. If the value is not present in the INI file, the DefaultValue is stored in the file and parsed as the result. Returns empty item if the value cannot be parsed. ",
				},
				GetSnowStartHeight =
				{
					Params =
					{
						{
							Name = "Biome",
							Type = "EMCSBiome",
						},
					},
					Returns =
					{
						{ Type = "number" },
					},
					Notes = "Returns the height at which snow will start falling in the {{Globals#EMCSBiome|Biome}}. Check functions IsBiomeCold and IsBiomeVeryCold for more informations.",
				},
				GetTime =
				{
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Returns the current OS time, as a unix time stamp (number of seconds since Jan 1, 1970)",
				},
				IsBiomeCold =
				{
					Params =
					{
						{
							Name = "Biome",
							Type = "EMCSBiome",
						},
					},
					Returns =
					{
						{ Type = "boolean" },
					},
					Notes = "Returns true if the biome is cold (has snow and snowfall at higher elevations but not at regular heights). Doesn't report Very Cold biomes, use IsBiomeVeryCold() for those.",
				},
				IsBiomeNoDownfall =
				{
					Params =
					{
						{
							Name = "Biome",
							Type = "EMCSBiome",
						},
					},
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Returns true if the biome is 'dry', that is, there is no precipitation during rains and storms.",
				},
				IsBiomeOcean =
				{
					Params =
					{
						{
							Name = "Biome",
							Type = "EMCSBiome",
						},
					},
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Returns true if the biome is an ocean biome.",
				},
				IsBiomeVeryCold =
				{
					Params =
					{
						{
							Name = "Biome",
							Type = "EMCSBiome",
						},
					},
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Returns true if the biome is very cold (has snow on ground everywhere, turns top water to ice, has snowfall instead of rain everywhere). Doesn't report mildly cold biomes (where it snows above certain elevation), use IsBiomeCold() for those.",
				},
				IsBiomeMountain =
				{
					Params =
					{
						{
							Name = "Biome",
							Type = "EMCSBiome",
						}
					},
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Returns true if the biome is mountainous (mutations of the extreme hills biome)."
				},
				IsBiomeMesa =
				{
					Params =
					{
						{
							Name = "Biome",
							Type = "EMCSBiome",
						}
					},
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Returns true if the biome is a type of Mesa (mutations of the Mesa biome)."
				},
				IsValidBlock =
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
							Type = "boolean",
						},
					},
					Notes = "Returns true if BlockType is a known block type",
				},
				IsValidItem =
				{
					Params =
					{
						{
							Name = "ItemType",
							Type = "number",
						},
					},
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Returns true if ItemType is a known item type",
				},
				ItemToFullString =
				{
					Params =
					{
						{
							Name = "cItem",
							Type = "cItem",
						},
					},
					Returns =
					{
						{
							Type = "string",
						},
					},
					Notes = "Returns the string representation of the item, in the format 'ItemTypeText:ItemDamage * Count'",
				},
				ItemToString =
				{
					Params =
					{
						{
							Name = "cItem",
							Type = "cItem",
						},
					},
					Returns =
					{
						{
							Type = "string",
						},
					},
					Notes = "Returns the string representation of the item type",
				},
				ItemTypeToString =
				{
					Params =
					{
						{
							Name = "ItemType",
							Type = "number",
						},
					},
					Returns =
					{
						{
							Type = "string",
						},
					},
					Notes = "Returns the string representation of ItemType ",
				},
				LOG =
				{
					{
						Params =
						{
							{
								Name = "Message",
								Type = "string",
							},
						},
						Notes = "Logs a text into the server console and logfile using 'normal' severity (gray text)",
					},
					{
						Params =
						{
							{
								Name = "Message",
								Type = "cCompositeChat",
							},
						},
						Notes = "Logs the {{cCompositeChat}}'s human-readable text into the server console. The severity is converted from the CompositeChat's MessageType.",
					},
				},
				LOGERROR =
				{
					{
						Params =
						{
							{
								Name = "Message",
								Type = "string",
							},
						},
						Notes = "Logs a text into the server console and logfile using 'error' severity (black text on red background)",
					},
					{
						Params =
						{
							{
								Name = "Message",
								Type = "cCompositeChat",
							},
						},
						Notes = "Logs the {{cCompositeChat}}'s human-readable text into the server console and logfile using 'error' severity (black text on red background)",
					},
				},
				LOGINFO =
				{
					{
						Params =
						{
							{
								Name = "Message",
								Type = "string",
							},
						},
						Notes = "Logs a text into the server console and logfile using 'info' severity (yellow text)",
					},
					{
						Params =
						{
							{
								Name = "Message",
								Type = "cCompositeChat",
							},
						},
						Notes = "Logs the {{cCompositeChat}}'s human-readable text into the server console and logfile using 'info' severity (yellow text)",
					},
				},
				LOGWARN =
				{
					{
						Params =
						{
							{
								Name = "Message",
								Type = "string",
							},
						},
						Notes = "Logs a text into the server console and logfile using 'warning' severity (red text); OBSOLETE, use LOGWARNING() instead",
					},
					{
						Params =
						{
							{
								Name = "Message",
								Type = "cCompositeChat",
							},
						},
						Notes = "Logs the {{cCompositeChat}}'s human-readable text into the server console and logfile using 'warning' severity (red text); OBSOLETE, use LOGWARNING() instead",
					},
				},
				LOGWARNING =
				{
					{
						Params =
						{
							{
								Name = "Message",
								Type = "string",
							},
						},
						Notes = "Logs a text into the server console and logfile using 'warning' severity (red text)",
					},
					{
						Params =
						{
							{
								Name = "Message",
								Type = "cCompositeChat",
							},
						},
						Notes = "Logs the {{cCompositeChat}}'s human-readable text into the server console and logfile using 'warning' severity (red text)",
					},
				},
				md5 =
				{
					Params =
					{
						{
							Name = "Data",
							Type = "string",
						},
					},
					Returns =
					{
						{
							Type = "string",
						},
					},
					Notes = "<b>OBSOLETE</b>, use the {{cCryptoHash}} functions instead.<br>Converts a string to a raw binary md5 hash.",
				},
				MirrorBlockFaceY =
				{
					Params =
					{
						{
							Name = "eBlockFace",
							Type = "eBlockFace",
						},
					},
					Returns =
					{
						{
							Type = "eBlockFace",
						},
					},
					Notes = "Returns the {{Globals#BlockFaces|eBlockFace}} that corresponds to the given {{Globals#BlockFaces|eBlockFace}} after mirroring it around the Y axis (or rotating 180 degrees around it).",
				},
				NoCaseCompare =
				{
					Params =
					{
						{
							Name = "Value1",
							Type = "string",
						},
						{
							Name = "Value2",
							Type = "string",
						},
					},
					Returns =
					{
						{
							Type = "number",
						},
					},
					Notes = "Case-insensitive string comparison; returns 0 if the strings are the same, -1 if Value1 is smaller and 1 if Value2 is smaller",
				},
				NormalizeAngleDegrees =
				{
					Params =
					{
						{
							Name = "AngleDegrees",
							Type = "number",
						},
					},
					Returns =
					{
						{
							Name = "AngleDegrees",
							Type = "number",
						},
					},
					Notes = "Returns the angle, wrapped into the [-180, +180) range.",
				},
				ReplaceString =
				{
					Params =
					{
						{
							Name = "full-string",
							Type = "string",
						},
						{
							Name = "to-be-replaced-string",
							Type = "string",
						},
						{
							Name = "to-replace-string",
							Type = "string",
						},
					},
					Returns =
					{
						{
							Type = "string",
						},
					},
					Notes = "Replaces *each* occurence of to-be-replaced-string in full-string with to-replace-string",
				},
				ReverseBlockFace =
				{
					Params =
					{
						{
							Name = "BlockFace",
							Type = "eBlockFace",
						},
					},
					Returns =
					{
						{
							Type = "eBlockFace",
						},
					},
					Notes = "Returns the reversed {{Globals#eBlockFace|eBlockFace}}.",
				},
				RotateBlockFaceCCW =
				{
					Params =
					{
						{
							Name = "eBlockFace",
							Type = "eBlockFace",
						},
					},
					Returns =
					{
						{
							Type = "eBlockFace",
						},
					},
					Notes = "Returns the {{Globals#BlockFaces|eBlockFace}} that corresponds to the given {{Globals#BlockFaces|eBlockFace}} after rotating it around the Y axis 90 degrees counter-clockwise.",
				},
				RotateBlockFaceCW =
				{
					Params =
					{
						{
							Name = "eBlockFace",
							Type = "eBlockFace",
						},
					},
					Returns =
					{
						{
							Type = "eBlockFace",
						},
					},
					Notes = "Returns the {{Globals#BlockFaces|eBlockFace}} that corresponds to the given {{Globals#BlockFaces|eBlockFace}} after rotating it around the Y axis 90 degrees clockwise.",
				},
				StringSplit =
				{
					Params =
					{
						{
							Name = "Input",
							Type = "string",
						},
						{
							Name = "SeperatorsString",
							Type = "string",
						},
					},
					Returns =
					{
						{
							Type = "table",
						},
					},
					Notes = "Seperates string into multiple by splitting every time any of the characters in SeperatorsString is encountered. Returns and array-table of strings.",
				},
				StringSplitAndTrim =
				{
					Params =
					{
						{
							Name = "Input",
							Type = "string",
						},
						{
							Name = "SeperatorsString",
							Type = "string",
						},
					},
					Returns =
					{
						{
							Type = "table",
						},
					},
					Notes = "Seperates string into multiple by splitting every time any of the characters in SeperatorsString is encountered. Each of the separate strings is trimmed (whitespace removed from the beginning and end of the string). Returns an array-table of strings.",
				},
				StringSplitWithQuotes =
				{
					Params =
					{
						{
							Name = "Input",
							Type = "string",
						},
						{
							Name = "SeperatorsString",
							Type = "string",
						},
					},
					Returns =
					{
						{
							Type = "table",
						},
					},
					Notes = "Seperates string into multiple by splitting every time any of the characters in SeperatorsString is encountered. Whitespace wrapped with single or double quotes will be ignored. Returns an array-table of strings.",
				},
				StringToBiome =
				{
					Params =
					{
						{
							Name = "Input",
							Type = "string",
						},
					},
					Returns =
					{
						{
							Name = "BiomeType",
							Type = "EMCSBiome",
						},
					},
					Notes = "Converts a string representation to a {{Globals#BiomeTypes|BiomeType}} enumerated value. Returns biInvalidBiome if the input is not a recognized biome.",
				},
				StringToDamageType =
				{
					Params =
					{
						{
							Name = "Input",
							Type = "string",
						},
					},
					Returns =
					{
						{
							Name = "DamageType",
							Type = "eDamageType",
						},
					},
					Notes = "Converts a string representation to a {{Globals#DamageType|DamageType}} enumerated value. Returns -1 if the inupt is not a recognized damage type.",
				},
				StringToDimension =
				{
					Params =
					{
						{
							Name = "Input",
							Type = "string",
						},
					},
					Returns =
					{
						{
							Name = "Dimension",
							Type = "eDimension",
						},
					},
					Notes = "Converts a string representation to a {{Globals#eDimension|eDimension}} enumerated value. Returns dimNotSet if the input is not a recognized dimension.",
				},
				StringToItem =
				{
					Params =
					{
						{
							Name = "StringToParse",
							Type = "string",
						},
						{
							Name = "DestItem",
							Type = "cItem",
						},
					},
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Parses the item specification in the given string and sets it into DestItem; returns true if successful",
				},
				StringToMobType =
				{
					Params =
					{
						{
							Name = "MobTypeString",
							Type = "string",
						},
					},
					Returns =
					{
						{
							Name = "MobType",
							Type = "eMonsterType",
						},
					},
					Notes = "(<b>DEPRECATED!</b>) Please use cMonster:StringToMobType(). Converts a string representation to an {{Globals#eMonsterType|eMonsterType}} enumerated value",
				},
				StripColorCodes =
				{
					Params =
					{
						{
							Name = "Input",
							Type = "string",
						},
					},
					Returns =
					{
						{
							Type = "string",
						},
					},
					Notes = "Removes all control codes used by MC for colors and styles",
				},
				TrimString =
				{
					Params =
					{
						{
							Name = "Input",
							Type = "string",
						},
					},
					Returns =
					{
						{
							Type = "string",
						},
					},
					Notes = "Removes whitespace at both ends of the string",
				},
			},
			Constants =
			{
				caLeftClick =
				{
					Notes = "Left click on a slot",
				},
				caRightClick =
				{
					Notes = "Right click on a slot",
				},
				caShiftLeftClick =
				{
					Notes = "Shift + left click on a slot",
				},
				caShiftRightClick =
				{
					Notes = "Shift + right click on a slot",
				},
				caNumber1 =
				{
					Notes = "Number key 1",
				},
				caNumber2 =
				{
					Notes = "Number key 2",
				},
				caNumber3 =
				{
					Notes = "Number key 3",
				},
				caNumber4 =
				{
					Notes = "Number key 4",
				},
				caNumber5 =
				{
					Notes = "Number key 5",
				},
				caNumber6 =
				{
					Notes = "Number key 6",
				},
				caNumber7 =
				{
					Notes = "Number key 7",
				},
				caNumber8 =
				{
					Notes = "Number key 8",
				},
				caNumber9 =
				{
					Notes = "Number key 9",
				},
				caMiddleClick =
				{
					Notes = "Middle click, only valid for creative players",
				},
				caDropKey =
				{
					Notes = "Drop a single item",
				},
				caCtrlDropKey =
				{
					Notes = "Drop a full stack",
				},
				caLeftClickOutside =
				{
					Notes = "Left click outside of inventory",
				},
				caRightClickOutside =
				{
					Notes = "Right click outside of inventory",
				},
				caLeftClickOutsideHoldNothing =
				{
					Notes = "Left click outside inventory holding nothing",
				},
				caRightClickOutsideHoldNothing =
				{
					Notes = "Right click outside inventory holding nothing",
				},
				caLeftPaintBegin =
				{
					Notes = "Begining of left click paint drag action",
				},
				caRightPaintBegin =
				{
					Notes = "Begining of right click paint drag action",
				},
				caMiddlePaintBegin =
				{
					Notes = "Begining of middle click paint drag action, only valid for creative players",
				},
				caLeftPaintProgress =
				{
					Notes = "Add slot for left click paint drag action",
				},
				caRightPaintProgress =
				{
					Notes = "Add slot for right click paint drag action",
				},
				caMiddlePaintProgress =
				{
					Notes = "Add slot for middle click paint drag action, only valid for creative players",
				},
				caLeftPaintEnd =
				{
					Notes = "End of left click paint drag action",
				},
				caRightPaintEnd =
				{
					Notes = "End of right click paint drag action",
				},
				caMiddlePaintEnd =
				{
					Notes = "End of middle click paint drag action, only valid for creative players",
				},
				caDblClick =
				{
					Notes = "Double click action",
				},
				caUnknown =
				{
					Notes = "Unknown click action"
				},
				dtAdmin =
				{
					Notes = "Damage applied by an admin command"
				},
				dtArrow =
				{
					Notes = "Damage received by being attacked by a projectile, possibly from a mob"
				},
				dtArrowAttack =
				{
					Notes = "Damage received by being attacked by a projectile, possibly from a mob"
				},
				dtAttack =
				{
					Notes = "Damage recieved by being attacked by a mob"
				},
				dtBurning =
				{
					Notes = "Damage from being on fire"
				},
				dtCacti =
				{
					Notes = "Damage from contact with a cactus block"
				},
				dtCactus =
				{
					Notes = "Damage from contact with a cactus block"
				},
				dtCactusContact =
				{
					Notes = "Damage from contact with a cactus block"
				},
				dtCactuses =
				{
					Notes = "Damage from contact with a cactus block"
				},
				dtDrown =
				{
					Notes = "Damage received by drowning in water / lava"
				},
				dtDrowning =
				{
					Notes = "Damage received by drowning in water / lava"
				},
				dtEnderPearl =
				{
					Notes = "Damage received by throwing an ender pearl and being teleported by it"
				},
				dtEntityAttack =
				{
					Notes = "Damage recieved by being attacked by a mob"
				},
				dtEnvironment =
				{
					Notes = "Damage dealt to mobs from environment: enderman in rain, snow golem in desert"
				},
				dtExplosion =
				{
					Notes = "Damage applied by an explosion"
				},
				dtFall =
				{
					Notes = "Damage from falling down. Dealt when hitting the ground"
				},
				dtFalling =
				{
					Notes = "Damage from falling down. Dealt when hitting the ground"
				},
				dtFireContact =
				{
					Notes = "Damage received by standing inside a fire block"
				},
				dtHunger =
				{
					Notes = "Damage received from hunger"
				},
				dtInFire =
				{
					Notes = "Damage received by standing inside a fire block"
				},
				dtInVoid =
				{
					Notes = "Damage received by falling into the Void (Y < 0)"
				},
				dtLava =
				{
					Notes = "Damage received by a contact with a lava block"
				},
				dtLavaContact =
				{
					Notes = "Damage received by a contact with a lava block"
				},
				dtLightning =
				{
					Notes = "Damage from being hit by a lightning strike"
				},
				dtMob =
				{
					Notes = "Damage received by being attacked by a mob"
				},
				dtMobAttack =
				{
					Notes = "Damage received by being attacked by a mob"
				},
				dtOnFire =
				{
					Notes = "Damage from being on fire"
				},
				dtPawnAttack =
				{
					Notes = "Damage received by being attacked by a mob"
				},
				dtPlugin =
				{
					Notes = "Damage applied by an admin command"
				},
				dtPoison =
				{
					Notes = "Damage applied by the poison effect"
				},
				dtPoisoning =
				{
					Notes = "Damage applied by the poison effect"
				},
				dtPotionOfHarming =
				{
					Notes = "Damage applied by the potion of harming"
				},
				dtProjectile =
				{
					Notes = "Damage received by being attacked by a projectile, possibly from a mob"
				},
				dtRangedAttack =
				{
					Notes = "Damage received by being attacked by a projectile, possibly from a mob"
				},
				dtStarvation =
				{
					Notes = "Damage received from hunger"
				},
				dtStarving =
				{
					Notes = "Damage received from hunger"
				},
				dtSuffocating =
				{
					Notes = "Damage from suffocating inside a block"
				},
				dtSuffocation =
				{
					Notes = "Damage from suffocating inside a block"
				},
				dtWither =
				{
					Notes = "Damage from the wither effect"
				},
				dtWithering =
				{
					Notes = "Damage from the wither effect"
				},
				E_BLOCK_ACACIA_DOOR =
				{
					Notes = "The blocktype for acacia door"
				},
				E_BLOCK_ACACIA_FENCE =
				{
					Notes = "The blocktype for acacia fence"
				},
				E_BLOCK_ACACIA_FENCE_GATE =
				{
					Notes = "The blocktype for acacia fence gate"
				},
				E_BLOCK_ACACIA_WOOD_STAIRS =
				{
					Notes = "The blocktype for acacia wood stairs"
				},
				E_BLOCK_ACTIVATOR_RAIL =
				{
					Notes = "The blocktype for activator rail"
				},
				E_BLOCK_ACTIVE_COMPARATOR =
				{
					Notes = "The blocktype for active comparator"
				},
				E_BLOCK_AIR =
				{
					Notes = "The blocktype for air"
				},
				E_BLOCK_ANVIL =
				{
					Notes = "The blocktype for anvil"
				},
				E_BLOCK_ANVIL_HIGH_DAMAGE =
				{
					Notes = "The blocktype for anvil high damage"
				},
				E_BLOCK_ANVIL_LOW_DAMAGE =
				{
					Notes = "The blocktype for anvil low damage"
				},
				E_BLOCK_ANVIL_NO_DAMAGE =
				{
					Notes = "The blocktype for anvil no damage"
				},
				E_BLOCK_ANVIL_X =
				{
					Notes = "The blocktype for anvil x"
				},
				E_BLOCK_ANVIL_Z =
				{
					Notes = "The blocktype for anvil z"
				},
				E_BLOCK_BARRIER =
				{
					Notes = "The blocktype for barrier"
				},
				E_BLOCK_BEACON =
				{
					Notes = "The blocktype for beacon"
				},
				E_BLOCK_BED =
				{
					Notes = "The blocktype for bed"
				},
				E_BLOCK_BEDROCK =
				{
					Notes = "The blocktype for bedrock"
				},
				E_BLOCK_BED_BED_HEAD =
				{
					Notes = "The blocktype for bed bed head"
				},
				E_BLOCK_BED_OCCUPIED =
				{
					Notes = "The blocktype for bed occupied"
				},
				E_BLOCK_BED_XM =
				{
					Notes = "The blocktype for bed xm"
				},
				E_BLOCK_BED_XP =
				{
					Notes = "The blocktype for bed xp"
				},
				E_BLOCK_BED_ZM =
				{
					Notes = "The blocktype for bed zm"
				},
				E_BLOCK_BED_ZP =
				{
					Notes = "The blocktype for bed zp"
				},
				E_BLOCK_BEETROOTS =
				{
					Notes = "The blocktype for beetroots"
				},
				E_BLOCK_BIG_FLOWER =
				{
					Notes = "The blocktype for big flower"
				},
				E_BLOCK_BIRCH_DOOR =
				{
					Notes = "The blocktype for birch door"
				},
				E_BLOCK_BIRCH_FENCE =
				{
					Notes = "The blocktype for birch fence"
				},
				E_BLOCK_BIRCH_FENCE_GATE =
				{
					Notes = "The blocktype for birch fence gate"
				},
				E_BLOCK_BIRCH_WOOD_STAIRS =
				{
					Notes = "The blocktype for birch wood stairs"
				},
				E_BLOCK_BLACK_GLAZED_TERRACOTTA =
				{
					Notes = "The blocktype for black terracotta"
				},
				E_BLOCK_BLACK_SHULKER_BOX =
				{
					Notes = "The blocktype for black shulker box"
				},
				E_BLOCK_BLOCK_OF_COAL =
				{
					Notes = "The blocktype for block of coal"
				},
				E_BLOCK_BLOCK_OF_REDSTONE =
				{
					Notes = "The blocktype for block of redstone"
				},
				E_BLOCK_BLUE_GLAZED_TERRACOTTA =
				{
					Notes = "The blocktype for blue glazed terracotta"
				},
				E_BLOCK_BLUE_SHULKER_BOX =
				{
					Notes = "The blocktype for blue shulker box"
				},
				E_BLOCK_BONE_BLOCK =
				{
					Notes = "The blocktype for bone block"
				},
				E_BLOCK_BOOKCASE =
				{
					Notes = "The blocktype for bookcase"
				},
				E_BLOCK_BREWING_STAND =
				{
					Notes = "The blocktype for brewing stand"
				},
				E_BLOCK_BRICK =
				{
					Notes = "The blocktype for brick"
				},
				E_BLOCK_BRICK_STAIRS =
				{
					Notes = "The blocktype for brick stairs"
				},
				E_BLOCK_BROWN_GLAZED_TERRACOTTA =
				{
					Notes = "The blocktype for brown glazed terracotta"
				},
				E_BLOCK_BROWN_MUSHROOM =
				{
					Notes = "The blocktype for brown mushroom"
				},
				E_BLOCK_BROWN_SHULKER_BOX =
				{
					Notes = "The blocktype for brown shulker box"
				},
				E_BLOCK_BURNING_FURNACE =
				{
					Notes = "The blocktype for burning furnace"
				},
				E_BLOCK_BUTTON_PRESSED =
				{
					Notes = "The blocktype for button pressed"
				},
				E_BLOCK_BUTTON_XM =
				{
					Notes = "The blocktype for button xm"
				},
				E_BLOCK_BUTTON_XP =
				{
					Notes = "The blocktype for button xp"
				},
				E_BLOCK_BUTTON_YM =
				{
					Notes = "The blocktype for button ym"
				},
				E_BLOCK_BUTTON_YP =
				{
					Notes = "The blocktype for button yp"
				},
				E_BLOCK_BUTTON_ZM =
				{
					Notes = "The blocktype for button zm"
				},
				E_BLOCK_BUTTON_ZP =
				{
					Notes = "The blocktype for button zp"
				},
				E_BLOCK_CACTUS =
				{
					Notes = "The blocktype for cactus"
				},
				E_BLOCK_CAKE =
				{
					Notes = "The blocktype for cake"
				},
				E_BLOCK_CARPET =
				{
					Notes = "The blocktype for carpet"
				},
				E_BLOCK_CARROTS =
				{
					Notes = "The blocktype for carrots"
				},
				E_BLOCK_CAULDRON =
				{
					Notes = "The blocktype for cauldron"
				},
				E_BLOCK_CHAIN_COMMAND_BLOCK =
				{
					Notes = "The blocktype for chain command block"
				},
				E_BLOCK_CHEST =
				{
					Notes = "The blocktype for chest"
				},
				E_BLOCK_CHORUS_FLOWER =
				{
					Notes = "The blocktype for chorus flower"
				},
				E_BLOCK_CHORUS_PLANT =
				{
					Notes = "The blocktype for chorus plant"
				},
				E_BLOCK_CLAY =
				{
					Notes = "The blocktype for clay"
				},
				E_BLOCK_COAL_ORE =
				{
					Notes = "The blocktype for coal ore"
				},
				E_BLOCK_COBBLESTONE =
				{
					Notes = "The blocktype for cobblestone"
				},
				E_BLOCK_COBBLESTONE_STAIRS =
				{
					Notes = "The blocktype for cobblestone stairs"
				},
				E_BLOCK_COBBLESTONE_WALL =
				{
					Notes = "The blocktype for cobblestone wall"
				},
				E_BLOCK_COBWEB =
				{
					Notes = "The blocktype for cobweb"
				},
				E_BLOCK_COCOA_POD =
				{
					Notes = "The blocktype for cocoa pod"
				},
				E_BLOCK_COMMAND_BLOCK =
				{
					Notes = "The blocktype for command block"
				},
				E_BLOCK_CONCRETE =
				{
					Notes = "The blocktype for concrete"
				},
				E_BLOCK_CONCRETE_POWDER =
				{
					Notes = "The blocktype for concrete powder"
				},
				E_BLOCK_CRAFTING_TABLE =
				{
					Notes = "The blocktype for crafting table"
				},
				E_BLOCK_CROPS =
				{
					Notes = "The blocktype for crops"
				},
				E_BLOCK_CYAN_GLAZED_TERRACOTTA =
				{
					Notes = "The blocktype for cyan glazed terracotta"
				},
				E_BLOCK_CYAN_SHULKER_BOX =
				{
					Notes = "The blocktype for cyan shulker box"
				},
				E_BLOCK_DANDELION =
				{
					Notes = "The blocktype for dandelion"
				},
				E_BLOCK_DARK_OAK_DOOR =
				{
					Notes = "The blocktype for dark oak door"
				},
				E_BLOCK_DARK_OAK_FENCE =
				{
					Notes = "The blocktype for dark oak fence"
				},
				E_BLOCK_DARK_OAK_FENCE_GATE =
				{
					Notes = "The blocktype for dark oak fence gate"
				},
				E_BLOCK_DARK_OAK_WOOD_STAIRS =
				{
					Notes = "The blocktype for dark oak wood stairs"
				},
				E_BLOCK_DAYLIGHT_SENSOR =
				{
					Notes = "The blocktype for daylight sensor"
				},
				E_BLOCK_DEAD_BUSH =
				{
					Notes = "The blocktype for dead bush"
				},
				E_BLOCK_DETECTOR_RAIL =
				{
					Notes = "The blocktype for detector rail"
				},
				E_BLOCK_DIAMOND_BLOCK =
				{
					Notes = "The blocktype for diamond block"
				},
				E_BLOCK_DIAMOND_ORE =
				{
					Notes = "The blocktype for diamond ore"
				},
				E_BLOCK_DIRT =
				{
					Notes = "The blocktype for dirt"
				},
				E_BLOCK_DISPENSER =
				{
					Notes = "The blocktype for dispenser"
				},
				E_BLOCK_DOUBLE_RED_SANDSTONE_SLAB =
				{
					Notes = "The blocktype for double red sandstone slab"
				},
				E_BLOCK_DOUBLE_STONE_SLAB =
				{
					Notes = "The blocktype for double stone slab"
				},
				E_BLOCK_DOUBLE_WOODEN_SLAB =
				{
					Notes = "The blocktype for double wooden slab"
				},
				E_BLOCK_DRAGON_EGG =
				{
					Notes = "The blocktype for dragon egg"
				},
				E_BLOCK_DROPPER =
				{
					Notes = "The blocktype for dropper"
				},
				E_BLOCK_EMERALD_BLOCK =
				{
					Notes = "The blocktype for emerald block"
				},
				E_BLOCK_EMERALD_ORE =
				{
					Notes = "The blocktype for emerald ore"
				},
				E_BLOCK_ENCHANTMENT_TABLE =
				{
					Notes = "The blocktype for enchantment table"
				},
				E_BLOCK_ENDER_CHEST =
				{
					Notes = "The blocktype for ender chest"
				},
				E_BLOCK_END_BRICKS =
				{
					Notes = "The blocktype for end bricks"
				},
				E_BLOCK_END_GATEWAY =
				{
					Notes = "The blocktype for end gateway"
				},
				E_BLOCK_END_PORTAL =
				{
					Notes = "The blocktype for end portal"
				},
				E_BLOCK_END_PORTAL_FRAME =
				{
					Notes = "The blocktype for end portal frame"
				},
				E_BLOCK_END_ROD =
				{
					Notes = "The blocktype for end rod"
				},
				E_BLOCK_END_STONE =
				{
					Notes = "The blocktype for end stone"
				},
				E_BLOCK_FARMLAND =
				{
					Notes = "The blocktype for farmland"
				},
				E_BLOCK_FENCE =
				{
					Notes = "The blocktype for fence"
				},
				E_BLOCK_FENCE_GATE =
				{
					Notes = "The blocktype for fence gate"
				},
				E_BLOCK_FIRE =
				{
					Notes = "The blocktype for fire"
				},
				E_BLOCK_FLOWER =
				{
					Notes = "The blocktype for flower"
				},
				E_BLOCK_FLOWER_POT =
				{
					Notes = "The blocktype for flower pot"
				},
				E_BLOCK_FROSTED_ICE =
				{
					Notes = "The blocktype for frosted ice"
				},
				E_BLOCK_FURNACE =
				{
					Notes = "The blocktype for furnace"
				},
				E_BLOCK_GLASS =
				{
					Notes = "The blocktype for glass"
				},
				E_BLOCK_GLASS_PANE =
				{
					Notes = "The blocktype for glass pane"
				},
				E_BLOCK_GLOWSTONE =
				{
					Notes = "The blocktype for glowstone"
				},
				E_BLOCK_GOLD_BLOCK =
				{
					Notes = "The blocktype for gold block"
				},
				E_BLOCK_GOLD_ORE =
				{
					Notes = "The blocktype for gold ore"
				},
				E_BLOCK_GRASS =
				{
					Notes = "The blocktype for grass"
				},
				E_BLOCK_GRASS_PATH =
				{
					Notes = "The blocktype for grass path"
				},
				E_BLOCK_GRAVEL =
				{
					Notes = "The blocktype for gravel"
				},
				E_BLOCK_GRAY_GLAZED_TERRACOTTA =
				{
					Notes = "The blocktype for gray glazed terracotta"
				},
				E_BLOCK_GRAY_SHULKER_BOX =
				{
					Notes = "The blocktype for gray shulker box"
				},
				E_BLOCK_GREEN_GLAZED_TERRACOTTA =
				{
					Notes = "The blocktype for green glazed terracotta"
				},
				E_BLOCK_GREEN_SHULKER_BOX =
				{
					Notes = "The blocktype for green shulker box"
				},
				E_BLOCK_HARDENED_CLAY =
				{
					Notes = "The blocktype for hardened clay"
				},
				E_BLOCK_HAY_BALE =
				{
					Notes = "The blocktype for hay bale"
				},
				E_BLOCK_HEAD =
				{
					Notes = "The blocktype for head"
				},
				E_BLOCK_HEAVY_WEIGHTED_PRESSURE_PLATE =
				{
					Notes = "The blocktype for heavy weighted pressure plate"
				},
				E_BLOCK_HOPPER =
				{
					Notes = "The blocktype for hopper"
				},
				E_BLOCK_HUGE_BROWN_MUSHROOM =
				{
					Notes = "The blocktype for huge brown mushroom"
				},
				E_BLOCK_HUGE_RED_MUSHROOM =
				{
					Notes = "The blocktype for huge red mushroom"
				},
				E_BLOCK_ICE =
				{
					Notes = "The blocktype for ice"
				},
				E_BLOCK_INACTIVE_COMPARATOR =
				{
					Notes = "The blocktype for inactive comparator"
				},
				E_BLOCK_INVERTED_DAYLIGHT_SENSOR =
				{
					Notes = "The blocktype for inverted daylight sensor"
				},
				E_BLOCK_IRON_BARS =
				{
					Notes = "The blocktype for iron bars"
				},
				E_BLOCK_IRON_BLOCK =
				{
					Notes = "The blocktype for iron block"
				},
				E_BLOCK_IRON_DOOR =
				{
					Notes = "The blocktype for iron door"
				},
				E_BLOCK_IRON_ORE =
				{
					Notes = "The blocktype for iron ore"
				},
				E_BLOCK_IRON_TRAPDOOR =
				{
					Notes = "The blocktype for iron trapdoor"
				},
				E_BLOCK_JACK_O_LANTERN =
				{
					Notes = "The blocktype for jack o lantern"
				},
				E_BLOCK_JUKEBOX =
				{
					Notes = "The blocktype for jukebox"
				},
				E_BLOCK_JUNGLE_DOOR =
				{
					Notes = "The blocktype for jungle door"
				},
				E_BLOCK_JUNGLE_FENCE =
				{
					Notes = "The blocktype for jungle fence"
				},
				E_BLOCK_JUNGLE_FENCE_GATE =
				{
					Notes = "The blocktype for jungle fence gate"
				},
				E_BLOCK_JUNGLE_WOOD_STAIRS =
				{
					Notes = "The blocktype for jungle wood stairs"
				},
				E_BLOCK_LADDER =
				{
					Notes = "The blocktype for ladder"
				},
				E_BLOCK_LAPIS_BLOCK =
				{
					Notes = "The blocktype for lapis block"
				},
				E_BLOCK_LAPIS_ORE =
				{
					Notes = "The blocktype for lapis ore"
				},
				E_BLOCK_LAVA =
				{
					Notes = "The blocktype for lava"
				},
				E_BLOCK_LEAVES =
				{
					Notes = "The blocktype for leaves"
				},
				E_BLOCK_LEVER =
				{
					Notes = "The blocktype for lever"
				},
				E_BLOCK_LIGHT_BLUE_GLAZED_TERRACOTTA =
				{
					Notes = "The blocktype for light blue glazed terracotta"
				},
				E_BLOCK_LIGHT_BLUE_SHULKER_BOX =
				{
					Notes = "The blocktype for light blue shulker box"
				},
				E_BLOCK_LIGHT_GRAY_GLAZED_TERRACOTTA =
				{
					Notes = "The blocktype for light gray glazed terracotta"
				},
				E_BLOCK_LIGHT_GRAY_SHULKER_BOX =
				{
					Notes = "The blocktype for light gray shulker box"
				},
				E_BLOCK_LIGHT_WEIGHTED_PRESSURE_PLATE =
				{
					Notes = "The blocktype for light weighted pressure plate"
				},
				E_BLOCK_LILY_PAD =
				{
					Notes = "The blocktype for lily pad"
				},
				E_BLOCK_LIME_GLAZED_TERRACOTTA =
				{
					Notes = "The blocktype for lime glazed terracotta"
				},
				E_BLOCK_LIME_SHULKER_BOX =
				{
					Notes = "The blocktype for lime shulker box"
				},
				E_BLOCK_LIT_FURNACE =
				{
					Notes = "The blocktype for lit furnace"
				},
				E_BLOCK_LOG =
				{
					Notes = "The blocktype for log"
				},
				E_BLOCK_MAGENTA_GLAZED_TERRACOTTA =
				{
					Notes = "The blocktype for magenta glazed terracotta"
				},
				E_BLOCK_MAGENTA_SHULKER_BOX =
				{
					Notes = "The blocktype for magenta shulker box"
				},
				E_BLOCK_MAGMA =
				{
					Notes = "The blocktype for magma"
				},
				E_BLOCK_MAX_TYPE_ID =
				{
					Notes = "The blocktype for max type id"
				},
				E_BLOCK_MELON =
				{
					Notes = "The blocktype for melon"
				},
				E_BLOCK_MELON_STEM =
				{
					Notes = "The blocktype for melon stem"
				},
				E_BLOCK_MINECART_TRACKS =
				{
					Notes = "The blocktype for minecart tracks"
				},
				E_BLOCK_MOB_SPAWNER =
				{
					Notes = "The blocktype for mob spawner"
				},
				E_BLOCK_MOSSY_COBBLESTONE =
				{
					Notes = "The blocktype for mossy cobblestone"
				},
				E_BLOCK_MYCELIUM =
				{
					Notes = "The blocktype for mycelium"
				},
				E_BLOCK_NETHERRACK =
				{
					Notes = "The blocktype for netherrack"
				},
				E_BLOCK_NETHER_BRICK =
				{
					Notes = "The blocktype for nether brick"
				},
				E_BLOCK_NETHER_BRICK_FENCE =
				{
					Notes = "The blocktype for nether brick fence"
				},
				E_BLOCK_NETHER_BRICK_STAIRS =
				{
					Notes = "The blocktype for nether brick stairs"
				},
				E_BLOCK_NETHER_PORTAL =
				{
					Notes = "The blocktype for nether portal"
				},
				E_BLOCK_NETHER_QUARTZ_ORE =
				{
					Notes = "The blocktype for nether quartz ore"
				},
				E_BLOCK_NETHER_WART =
				{
					Notes = "The blocktype for nether wart"
				},
				E_BLOCK_NETHER_WART_BLOCK =
				{
					Notes = "The blocktype for nether wart block"
				},
				E_BLOCK_NEW_LEAVES =
				{
					Notes = "The blocktype for new leaves"
				},
				E_BLOCK_NEW_LOG =
				{
					Notes = "The blocktype for new log"
				},
				E_BLOCK_NOTE_BLOCK =
				{
					Notes = "The blocktype for note block"
				},
				E_BLOCK_NUMBER_OF_TYPES =
				{
					Notes = "The blocktype for number of types"
				},
				E_BLOCK_OAK_DOOR =
				{
					Notes = "The blocktype for oak door"
				},
				E_BLOCK_OAK_FENCE_GATE =
				{
					Notes = "The blocktype for oak fence gate"
				},
				E_BLOCK_OAK_WOOD_STAIRS =
				{
					Notes = "The blocktype for oak wood stairs"
				},
				E_BLOCK_OBSERVER =
				{
					Notes = "The blocktype for observer"
				},
				E_BLOCK_OBSIDIAN =
				{
					Notes = "The blocktype for obsidian"
				},
				E_BLOCK_ORANGE_GLAZED_TERRACOTTA =
				{
					Notes = "The blocktype for orange glazed terracota"
				},
				E_BLOCK_ORANGE_SHULKER_BOX =
				{
					Notes = "The blocktype for orange shulker box"
				},
				E_BLOCK_PACKED_ICE =
				{
					Notes = "The blocktype for packed ice"
				},
				E_BLOCK_PINK_GLAZED_TERRACOTTA =
				{
					Notes = "The blocktype for pink glazed terracotta"
				},
				E_BLOCK_PINK_SHULKER_BOX =
				{
					Notes = "The blocktype for pink shulker box"
				},
				E_BLOCK_PISTON =
				{
					Notes = "The blocktype for piston"
				},
				E_BLOCK_PISTON_EXTENSION =
				{
					Notes = "The blocktype for piston extension"
				},
				E_BLOCK_PISTON_MOVED_BLOCK =
				{
					Notes = "The blocktype for piston moved block"
				},
				E_BLOCK_PLANKS =
				{
					Notes = "The blocktype for planks"
				},
				E_BLOCK_POTATOES =
				{
					Notes = "The blocktype for potatoes"
				},
				E_BLOCK_POWERED_RAIL =
				{
					Notes = "The blocktype for powered rail"
				},
				E_BLOCK_PRISMARINE_BLOCK =
				{
					Notes = "The blocktype for prismarine block"
				},
				E_BLOCK_PUMPKIN =
				{
					Notes = "The blocktype for pumpkin"
				},
				E_BLOCK_PUMPKIN_STEM =
				{
					Notes = "The blocktype for pumpkin stem"
				},
				E_BLOCK_PURPLE_GLAZED_TERRACOTTA =
				{
					Notes = "The blocktype for purple glazed terracotta"
				},
				E_BLOCK_PURPLE_SHULKER_BOX =
				{
					Notes = "The blocktype for purple shulker box"
				},
				E_BLOCK_PURPUR_BLOCK =
				{
					Notes = "The blocktype for purpur block"
				},
				E_BLOCK_PURPUR_DOUBLE_SLAB =
				{
					Notes = "The blocktype for purpur double slab"
				},
				E_BLOCK_PURPUR_PILLAR =
				{
					Notes = "The blocktype for purpur pillar"
				},
				E_BLOCK_PURPUR_SLAB =
				{
					Notes = "The blocktype for purpur slab"
				},
				E_BLOCK_PURPUR_STAIRS =
				{
					Notes = "The blocktype for purpur stairs"
				},
				E_BLOCK_QUARTZ_BLOCK =
				{
					Notes = "The blocktype for quartz block"
				},
				E_BLOCK_QUARTZ_STAIRS =
				{
					Notes = "The blocktype for quartz stairs"
				},
				E_BLOCK_RAIL =
				{
					Notes = "The blocktype for rail"
				},
				E_BLOCK_REDSTONE_LAMP_OFF =
				{
					Notes = "The blocktype for redstone lamp off"
				},
				E_BLOCK_REDSTONE_LAMP_ON =
				{
					Notes = "The blocktype for redstone lamp on"
				},
				E_BLOCK_REDSTONE_ORE =
				{
					Notes = "The blocktype for redstone ore"
				},
				E_BLOCK_REDSTONE_ORE_GLOWING =
				{
					Notes = "The blocktype for redstone ore glowing"
				},
				E_BLOCK_REDSTONE_REPEATER_OFF =
				{
					Notes = "The blocktype for redstone repeater off"
				},
				E_BLOCK_REDSTONE_REPEATER_ON =
				{
					Notes = "The blocktype for redstone repeater on"
				},
				E_BLOCK_REDSTONE_TORCH_OFF =
				{
					Notes = "The blocktype for redstone torch off"
				},
				E_BLOCK_REDSTONE_TORCH_ON =
				{
					Notes = "The blocktype for redstone torch on"
				},
				E_BLOCK_REDSTONE_WIRE =
				{
					Notes = "The blocktype for redstone wire"
				},
				E_BLOCK_RED_GLAZED_TERRACOTTA =
				{
					Notes = "The blocktype for red glazed terracotta"
				},
				E_BLOCK_RED_MUSHROOM =
				{
					Notes = "The blocktype for red mushroom"
				},
				E_BLOCK_RED_NETHER_BRICK =
				{
					Notes = "The blocktype for red nether brick"
				},
				E_BLOCK_RED_ROSE =
				{
					Notes = "The blocktype for red rose"
				},
				E_BLOCK_RED_SANDSTONE =
				{
					Notes = "The blocktype for red sandstone"
				},
				E_BLOCK_RED_SANDSTONE_SLAB =
				{
					Notes = "The blocktype for red sandstone slab"
				},
				E_BLOCK_RED_SANDSTONE_STAIRS =
				{
					Notes = "The blocktype for red sandstone stairs"
				},
				E_BLOCK_RED_SHULKER_BOX =
				{
					Notes = "The blocktype for red shulker box"
				},
				E_BLOCK_REEDS =
				{
					Notes = "The blocktype for reeds"
				},
				E_BLOCK_REPEATING_COMMAND_BLOCK =
				{
					Notes = "The blocktype for repeating command block"
				},
				E_BLOCK_SAND =
				{
					Notes = "The blocktype for sand"
				},
				E_BLOCK_SANDSTONE =
				{
					Notes = "The blocktype for sandstone"
				},
				E_BLOCK_SANDSTONE_STAIRS =
				{
					Notes = "The blocktype for sandstone stairs"
				},
				E_BLOCK_SAPLING =
				{
					Notes = "The blocktype for sapling"
				},
				E_BLOCK_SEA_LANTERN =
				{
					Notes = "The blocktype for sea lantern"
				},
				E_BLOCK_SIGN_POST =
				{
					Notes = "The blocktype for sign post"
				},
				E_BLOCK_SILVERFISH_EGG =
				{
					Notes = "The blocktype for silverfish egg"
				},
				E_BLOCK_SLIME_BLOCK =
				{
					Notes = "The blocktype for slime block"
				},
				E_BLOCK_SNOW =
				{
					Notes = "The blocktype for snow"
				},
				E_BLOCK_SNOW_BLOCK =
				{
					Notes = "The blocktype for snow block"
				},
				E_BLOCK_SOULSAND =
				{
					Notes = "The blocktype for soulsand"
				},
				E_BLOCK_SPONGE =
				{
					Notes = "The blocktype for sponge"
				},
				E_BLOCK_SPRUCE_DOOR =
				{
					Notes = "The blocktype for spruce door"
				},
				E_BLOCK_SPRUCE_FENCE =
				{
					Notes = "The blocktype for spruce fence"
				},
				E_BLOCK_SPRUCE_FENCE_GATE =
				{
					Notes = "The blocktype for spruce fence gate"
				},
				E_BLOCK_SPRUCE_WOOD_STAIRS =
				{
					Notes = "The blocktype for spruce wood stairs"
				},
				E_BLOCK_STAINED_CLAY =
				{
					Notes = "The blocktype for stained clay"
				},
				E_BLOCK_STAINED_GLASS =
				{
					Notes = "The blocktype for stained glass"
				},
				E_BLOCK_STAINED_GLASS_PANE =
				{
					Notes = "The blocktype for stained glass pane"
				},
				E_BLOCK_STAIRS_UPSIDE_DOWN =
				{
					Notes = "The blocktype for stairs upside down"
				},
				E_BLOCK_STAIRS_XM =
				{
					Notes = "The blocktype for stairs xm"
				},
				E_BLOCK_STAIRS_XP =
				{
					Notes = "The blocktype for stairs xp"
				},
				E_BLOCK_STAIRS_ZM =
				{
					Notes = "The blocktype for stairs zm"
				},
				E_BLOCK_STAIRS_ZP =
				{
					Notes = "The blocktype for stairs zp"
				},
				E_BLOCK_STANDING_BANNER =
				{
					Notes = "The blocktype for standing banner"
				},
				E_BLOCK_STATIONARY_LAVA =
				{
					Notes = "The blocktype for stationary lava"
				},
				E_BLOCK_STATIONARY_WATER =
				{
					Notes = "The blocktype for stationary water"
				},
				E_BLOCK_STICKY_PISTON =
				{
					Notes = "The blocktype for sticky piston"
				},
				E_BLOCK_STONE =
				{
					Notes = "The blocktype for stone"
				},
				E_BLOCK_STONE_BRICKS =
				{
					Notes = "The blocktype for stone bricks"
				},
				E_BLOCK_STONE_BRICK_STAIRS =
				{
					Notes = "The blocktype for stone brick stairs"
				},
				E_BLOCK_STONE_BUTTON =
				{
					Notes = "The blocktype for stone button"
				},
				E_BLOCK_STONE_PRESSURE_PLATE =
				{
					Notes = "The blocktype for stone pressure plate"
				},
				E_BLOCK_STONE_SLAB =
				{
					Notes = "The blocktype for stone slab"
				},
				E_BLOCK_STRUCTURE_BLOCK =
				{
					Notes = "The blocktype for structure block"
				},
				E_BLOCK_STRUCTURE_VOID =
				{
					Notes = "The blocktype for structure void"
				},
				E_BLOCK_SUGARCANE =
				{
					Notes = "The blocktype for sugarcane"
				},
				E_BLOCK_TALL_GRASS =
				{
					Notes = "The blocktype for tall grass"
				},
				E_BLOCK_TERRACOTTA =
				{
					Notes = "The blocktype for terracotta (synonym for E_BLOCK_STAINED_CLAY)",
				},
				E_BLOCK_TNT =
				{
					Notes = "The blocktype for tnt"
				},
				E_BLOCK_TORCH =
				{
					Notes = "The blocktype for torch"
				},
				E_BLOCK_TRAPDOOR =
				{
					Notes = "The blocktype for trapdoor"
				},
				E_BLOCK_TRAPPED_CHEST =
				{
					Notes = "The blocktype for trapped chest"
				},
				E_BLOCK_TRIPWIRE =
				{
					Notes = "The blocktype for tripwire"
				},
				E_BLOCK_TRIPWIRE_HOOK =
				{
					Notes = "The blocktype for tripwire hook"
				},
				E_BLOCK_UNFINISHED =
				{
					Notes = "Internal blocktype for unfinished block handlers",
				},
				E_BLOCK_VINES =
				{
					Notes = "The blocktype for vines"
				},
				E_BLOCK_WALLSIGN =
				{
					Notes = "The blocktype for wallsign"
				},
				E_BLOCK_WALL_BANNER =
				{
					Notes = "The blocktype for wall banner"
				},
				E_BLOCK_WATER =
				{
					Notes = "The blocktype for water"
				},
				E_BLOCK_WHITE_GLAZED_TERRACOTTA =
				{
					Notes = "The blocktype for white glazed terracotta"
				},
				E_BLOCK_WHITE_SHULKER_BOX =
				{
					Notes = "The blocktype for white shulker box"
				},
				E_BLOCK_WOODEN_BUTTON =
				{
					Notes = "The blocktype for wooden button"
				},
				E_BLOCK_WOODEN_DOOR =
				{
					Notes = "The blocktype for wooden door"
				},
				E_BLOCK_WOODEN_PRESSURE_PLATE =
				{
					Notes = "The blocktype for wooden pressure plate"
				},
				E_BLOCK_WOODEN_SLAB =
				{
					Notes = "The blocktype for wooden slab"
				},
				E_BLOCK_WOODEN_STAIRS =
				{
					Notes = "The blocktype for wooden stairs"
				},
				E_BLOCK_WOOL =
				{
					Notes = "The blocktype for wool"
				},
				E_BLOCK_WORKBENCH =
				{
					Notes = "The blocktype for workbench"
				},
				E_BLOCK_YELLOW_FLOWER =
				{
					Notes = "The blocktype for yellow flower"
				},
				E_BLOCK_YELLOW_GLAZED_TERRACOTTA =
				{
					Notes = "The blocktype for yellow glazed terracotta"
				},
				E_BLOCK_YELLOW_SHULKER_BOX =
				{
					Notes = "The blocktype for yellow shulker box"
				},
				E_ITEM_11_DISC =
				{
					Notes = "The itemtype for 11 disc"
				},
				E_ITEM_13_DISC =
				{
					Notes = "The itemtype for 13 disc"
				},
				E_ITEM_ACACIA_BOAT =
				{
					Notes = "The itemtype for acacia boat"
				},
				E_ITEM_ACACIA_DOOR =
				{
					Notes = "The itemtype for acacia door"
				},
				E_ITEM_ARMOR_STAND =
				{
					Notes = "The itemtype for armor stand"
				},
				E_ITEM_ARROW =
				{
					Notes = "The itemtype for arrow"
				},
				E_ITEM_BAKED_POTATO =
				{
					Notes = "The itemtype for baked potato"
				},
				E_ITEM_BANNER =
				{
					Notes = "The itemtype for banner"
				},
				E_ITEM_BED =
				{
					Notes = "The itemtype for bed"
				},
				E_ITEM_BEETROOT =
				{
					Notes = "The itemtype for beetroot"
				},
				E_ITEM_BEETROOT_SEEDS =
				{
					Notes = "The itemtype for beetroot seeds"
				},
				E_ITEM_BEETROOT_SOUP =
				{
					Notes = "The itemtype for betroot soup"
				},
				E_ITEM_BIRCH_BOAT =
				{
					Notes = "The itemtype for birch boat"
				},
				E_ITEM_BIRCH_DOOR =
				{
					Notes = "The itemtype for birch door"
				},
				E_ITEM_BLAZE_POWDER =
				{
					Notes = "The itemtype for blaze powder"
				},
				E_ITEM_BLAZE_ROD =
				{
					Notes = "The itemtype for blaze rod"
				},
				E_ITEM_BLOCKS_DISC =
				{
					Notes = "The itemtype for blocks disc"
				},
				E_ITEM_BOAT =
				{
					Notes = "The itemtype for boat"
				},
				E_ITEM_BONE =
				{
					Notes = "The itemtype for bone"
				},
				E_ITEM_BOOK =
				{
					Notes = "The itemtype for book"
				},
				E_ITEM_BOOK_AND_QUILL =
				{
					Notes = "The itemtype for book and quill"
				},
				E_ITEM_BOTTLE_O_ENCHANTING =
				{
					Notes = "The itemtype for bottle o enchanting"
				},
				E_ITEM_BOW =
				{
					Notes = "The itemtype for bow"
				},
				E_ITEM_BOWL =
				{
					Notes = "The itemtype for bowl"
				},
				E_ITEM_BREAD =
				{
					Notes = "The itemtype for bread"
				},
				E_ITEM_BREWING_STAND =
				{
					Notes = "The itemtype for brewing stand"
				},
				E_ITEM_BUCKET =
				{
					Notes = "The itemtype for bucket"
				},
				E_ITEM_CAKE =
				{
					Notes = "The itemtype for cake"
				},
				E_ITEM_CARROT =
				{
					Notes = "The itemtype for carrot"
				},
				E_ITEM_CARROT_ON_STICK =
				{
					Notes = "The itemtype for carrot on stick"
				},
				E_ITEM_CAT_DISC =
				{
					Notes = "The itemtype for cat disc"
				},
				E_ITEM_CAULDRON =
				{
					Notes = "The itemtype for cauldron"
				},
				E_ITEM_CHAIN_BOOTS =
				{
					Notes = "The itemtype for chain boots"
				},
				E_ITEM_CHAIN_CHESTPLATE =
				{
					Notes = "The itemtype for chain chestplate"
				},
				E_ITEM_CHAIN_HELMET =
				{
					Notes = "The itemtype for chain helmet"
				},
				E_ITEM_CHAIN_LEGGINGS =
				{
					Notes = "The itemtype for chain leggings"
				},
				E_ITEM_CHEST_MINECART =
				{
					Notes = "The itemtype for chest minecart"
				},
				E_ITEM_CHIRP_DISC =
				{
					Notes = "The itemtype for chirp disc"
				},
				E_ITEM_CHORUS_FRUIT =
				{
					Notes = "The itemtype for chorus fruit"
				},
				E_ITEM_CLAY =
				{
					Notes = "The itemtype for clay"
				},
				E_ITEM_CLAY_BRICK =
				{
					Notes = "The itemtype for clay brick"
				},
				E_ITEM_CLOCK =
				{
					Notes = "The itemtype for clock"
				},
				E_ITEM_COAL =
				{
					Notes = "The itemtype for coal"
				},
				E_ITEM_COMPARATOR =
				{
					Notes = "The itemtype for comparator"
				},
				E_ITEM_COMPASS =
				{
					Notes = "The itemtype for compass"
				},
				E_ITEM_COOKED_CHICKEN =
				{
					Notes = "The itemtype for cooked chicken"
				},
				E_ITEM_COOKED_FISH =
				{
					Notes = "The itemtype for cooked fish"
				},
				E_ITEM_COOKED_MUTTON =
				{
					Notes = "The itemtype for cooked mutton"
				},
				E_ITEM_COOKED_PORKCHOP =
				{
					Notes = "The itemtype for cooked porkchop"
				},
				E_ITEM_COOKED_RABBIT =
				{
					Notes = "The itemtype for cooked rabbit"
				},
				E_ITEM_COOKIE =
				{
					Notes = "The itemtype for cookie"
				},
				E_ITEM_DARK_OAK_BOAT =
				{
					Notes = "The itemtype for dark oak boat"
				},
				E_ITEM_DARK_OAK_DOOR =
				{
					Notes = "The itemtype for dark oak door"
				},
				E_ITEM_DIAMOND =
				{
					Notes = "The itemtype for diamond"
				},
				E_ITEM_DIAMOND_AXE =
				{
					Notes = "The itemtype for diamond axe"
				},
				E_ITEM_DIAMOND_BOOTS =
				{
					Notes = "The itemtype for diamond boots"
				},
				E_ITEM_DIAMOND_CHESTPLATE =
				{
					Notes = "The itemtype for diamond chestplate"
				},
				E_ITEM_DIAMOND_HELMET =
				{
					Notes = "The itemtype for diamond helmet"
				},
				E_ITEM_DIAMOND_HOE =
				{
					Notes = "The itemtype for diamond hoe"
				},
				E_ITEM_DIAMOND_HORSE_ARMOR =
				{
					Notes = "The itemtype for diamond horse armor"
				},
				E_ITEM_DIAMOND_LEGGINGS =
				{
					Notes = "The itemtype for diamond leggings"
				},
				E_ITEM_DIAMOND_PICKAXE =
				{
					Notes = "The itemtype for diamond pickaxe"
				},
				E_ITEM_DIAMOND_SHOVEL =
				{
					Notes = "The itemtype for diamond shovel"
				},
				E_ITEM_DIAMOND_SWORD =
				{
					Notes = "The itemtype for diamond sword"
				},
				E_ITEM_DRAGON_BREATH =
				{
					Notes = "The itemtype for dragon breath"
				},
				E_ITEM_DYE =
				{
					Notes = "The itemtype for dye"
				},
				E_ITEM_EGG =
				{
					Notes = "The itemtype for egg"
				},
				E_ITEM_ELYTRA =
				{
					Notes = "The itemtype for elytra"
				},
				E_ITEM_EMERALD =
				{
					Notes = "The itemtype for emerald"
				},
				E_ITEM_EMPTY =
				{
					Notes = "The itemtype for empty"
				},
				E_ITEM_EMPTY_MAP =
				{
					Notes = "The itemtype for empty map"
				},
				E_ITEM_ENCHANTED_BOOK =
				{
					Notes = "The itemtype for enchanted book"
				},
				E_ITEM_END_CRYSTAL =
				{
					Notes = "The itemtype for end crystal"
				},
				E_ITEM_ENDER_PEARL =
				{
					Notes = "The itemtype for ender pearl"
				},
				E_ITEM_EYE_OF_ENDER =
				{
					Notes = "The itemtype for eye of ender"
				},
				E_ITEM_FAR_DISC =
				{
					Notes = "The itemtype for far disc"
				},
				E_ITEM_FEATHER =
				{
					Notes = "The itemtype for feather"
				},
				E_ITEM_FERMENTED_SPIDER_EYE =
				{
					Notes = "The itemtype for fermented spider eye"
				},
				E_ITEM_FIREWORK_ROCKET =
				{
					Notes = "The itemtype for firework rocket"
				},
				E_ITEM_FIREWORK_STAR =
				{
					Notes = "The itemtype for firework star"
				},
				E_ITEM_FIRE_CHARGE =
				{
					Notes = "The itemtype for fire charge"
				},
				E_ITEM_FIRST =
				{
					Notes = "The itemtype for first"
				},
				E_ITEM_FIRST_DISC =
				{
					Notes = "The itemtype for first disc"
				},
				E_ITEM_FISHING_ROD =
				{
					Notes = "The itemtype for fishing rod"
				},
				E_ITEM_FLINT =
				{
					Notes = "The itemtype for flint"
				},
				E_ITEM_FLINT_AND_STEEL =
				{
					Notes = "The itemtype for flint and steel"
				},
				E_ITEM_FLOWER_POT =
				{
					Notes = "The itemtype for flower pot"
				},
				E_ITEM_FURNACE_MINECART =
				{
					Notes = "The itemtype for furnace minecart"
				},
				E_ITEM_GHAST_TEAR =
				{
					Notes = "The itemtype for ghast tear"
				},
				E_ITEM_GLASS_BOTTLE =
				{
					Notes = "The itemtype for glass bottle"
				},
				E_ITEM_GLISTERING_MELON =
				{
					Notes = "The itemtype for glistering melon"
				},
				E_ITEM_GLOWSTONE_DUST =
				{
					Notes = "The itemtype for glowstone dust"
				},
				E_ITEM_GOLD =
				{
					Notes = "The itemtype for gold"
				},
				E_ITEM_GOLDEN_APPLE =
				{
					Notes = "The itemtype for golden apple"
				},
				E_ITEM_GOLDEN_CARROT =
				{
					Notes = "The itemtype for golden carrot"
				},
				E_ITEM_GOLD_AXE =
				{
					Notes = "The itemtype for gold axe"
				},
				E_ITEM_GOLD_BOOTS =
				{
					Notes = "The itemtype for gold boots"
				},
				E_ITEM_GOLD_CHESTPLATE =
				{
					Notes = "The itemtype for gold chestplate"
				},
				E_ITEM_GOLD_HELMET =
				{
					Notes = "The itemtype for gold helmet"
				},
				E_ITEM_GOLD_HOE =
				{
					Notes = "The itemtype for gold hoe"
				},
				E_ITEM_GOLD_HORSE_ARMOR =
				{
					Notes = "The itemtype for gold horse armor"
				},
				E_ITEM_GOLD_LEGGINGS =
				{
					Notes = "The itemtype for gold leggings"
				},
				E_ITEM_GOLD_NUGGET =
				{
					Notes = "The itemtype for gold nugget"
				},
				E_ITEM_GOLD_PICKAXE =
				{
					Notes = "The itemtype for gold pickaxe"
				},
				E_ITEM_GOLD_SHOVEL =
				{
					Notes = "The itemtype for gold shovel"
				},
				E_ITEM_GOLD_SWORD =
				{
					Notes = "The itemtype for gold sword"
				},
				E_ITEM_GUNPOWDER =
				{
					Notes = "The itemtype for gunpowder"
				},
				E_ITEM_HEAD =
				{
					Notes = "The itemtype for head"
				},
				E_ITEM_IRON =
				{
					Notes = "The itemtype for iron"
				},
				E_ITEM_IRON_AXE =
				{
					Notes = "The itemtype for iron axe"
				},
				E_ITEM_IRON_BOOTS =
				{
					Notes = "The itemtype for iron boots"
				},
				E_ITEM_IRON_CHESTPLATE =
				{
					Notes = "The itemtype for iron chestplate"
				},
				E_ITEM_IRON_DOOR =
				{
					Notes = "The itemtype for iron door"
				},
				E_ITEM_IRON_HELMET =
				{
					Notes = "The itemtype for iron helmet"
				},
				E_ITEM_IRON_HOE =
				{
					Notes = "The itemtype for iron hoe"
				},
				E_ITEM_IRON_HORSE_ARMOR =
				{
					Notes = "The itemtype for iron horse armor"
				},
				E_ITEM_IRON_LEGGINGS =
				{
					Notes = "The itemtype for iron leggings"
				},
				E_ITEM_IRON_NUGGET =
				{
					Notes = "The itemtype for iron nugget"
				},
				E_ITEM_IRON_PICKAXE =
				{
					Notes = "The itemtype for iron pickaxe"
				},
				E_ITEM_IRON_SHOVEL =
				{
					Notes = "The itemtype for iron shovel"
				},
				E_ITEM_IRON_SWORD =
				{
					Notes = "The itemtype for iron sword"
				},
				E_ITEM_ITEM_FRAME =
				{
					Notes = "The itemtype for item frame"
				},
				E_ITEM_JUNGLE_BOAT =
				{
					Notes = "The itemtype for jungle boat"
				},
				E_ITEM_JUNGLE_DOOR =
				{
					Notes = "The itemtype for jungle door"
				},
				E_ITEM_LAST =
				{
					Notes = "The itemtype for last"
				},
				E_ITEM_LAST_DISC =
				{
					Notes = "The itemtype for last disc"
				},
				E_ITEM_LAST_DISC_PLUS_ONE =
				{
					Notes = "The itemtype for last disc plus one"
				},
				E_ITEM_LAVA_BUCKET =
				{
					Notes = "The itemtype for lava bucket"
				},
				E_ITEM_LEAD =
				{
					Notes = "The itemtype for lead"
				},
				E_ITEM_LEASH =
				{
					Notes = "The itemtype for lead (E_ITEM_LEAD synonym)"
				},
				E_ITEM_LEATHER =
				{
					Notes = "The itemtype for leather"
				},
				E_ITEM_LEATHER_BOOTS =
				{
					Notes = "The itemtype for leather boots"
				},
				E_ITEM_LEATHER_CAP =
				{
					Notes = "The itemtype for leather cap"
				},
				E_ITEM_LEATHER_PANTS =
				{
					Notes = "The itemtype for leather pants"
				},
				E_ITEM_LEATHER_TUNIC =
				{
					Notes = "The itemtype for leather tunic"
				},
				E_ITEM_LINGERING_POTION =
				{
					Notes = "The itemtype for lingering potion"
				},
				E_ITEM_MAGMA_CREAM =
				{
					Notes = "The itemtype for magma cream"
				},
				E_ITEM_MALL_DISC =
				{
					Notes = "The itemtype for mall disc"
				},
				E_ITEM_MAP =
				{
					Notes = "The itemtype for map"
				},
				E_ITEM_MAX_CONSECUTIVE_TYPE_ID =
				{
					Notes = "The itemtype for max consecutive type id"
				},
				E_ITEM_MELLOHI_DISC =
				{
					Notes = "The itemtype for mellohi disc"
				},
				E_ITEM_MELON_SEEDS =
				{
					Notes = "The itemtype for melon seeds"
				},
				E_ITEM_MELON_SLICE =
				{
					Notes = "The itemtype for melon slice"
				},
				E_ITEM_MILK =
				{
					Notes = "The itemtype for milk"
				},
				E_ITEM_MINECART =
				{
					Notes = "The itemtype for minecart"
				},
				E_ITEM_MINECART_WITH_COMMAND_BLOCK =
				{
					Notes = "The itemtype for minecart with command block"
				},
				E_ITEM_MINECART_WITH_HOPPER =
				{
					Notes = "The itemtype for minecart with hopper"
				},
				E_ITEM_MINECART_WITH_TNT =
				{
					Notes = "The itemtype for minecart with tnt"
				},
				E_ITEM_MUSHROOM_SOUP =
				{
					Notes = "The itemtype for mushroom soup"
				},
				E_ITEM_NAME_TAG =
				{
					Notes = "The itemtype for name tag"
				},
				E_ITEM_NETHER_BRICK =
				{
					Notes = "The itemtype for nether brick"
				},
				E_ITEM_NETHER_QUARTZ =
				{
					Notes = "The itemtype for nether quartz"
				},
				E_ITEM_NETHER_STAR =
				{
					Notes = "The itemtype for nether star"
				},
				E_ITEM_NETHER_WART =
				{
					Notes = "The itemtype for nether wart"
				},
				E_ITEM_NUMBER_OF_CONSECUTIVE_TYPES =
				{
					Notes = "The itemtype for number of consecutive types"
				},
				E_ITEM_PAINTING =
				{
					Notes = "The itemtype for painting"
				},
				E_ITEM_PAPER =
				{
					Notes = "The itemtype for paper"
				},
				E_ITEM_POISONOUS_POTATO =
				{
					Notes = "The itemtype for poisonous potato"
				},
				E_ITEM_POPPED_CHORUS_FRUIT =
				{
					Notes = "The itemtype for popped chorus fruit"
				},
				E_ITEM_POTATO =
				{
					Notes = "The itemtype for potato"
				},
				E_ITEM_POTION =
				{
					Notes = "The itemtype for potion"
				},
				E_ITEM_POTIONS =
				{
					Notes = "The itemtype for potion (obsolete, use E_ITEM_POTION instead)"
				},
				E_ITEM_PRISMARINE_CRYSTALS =
				{
					Notes = "The itemtype for prismarine crystals"
				},
				E_ITEM_PRISMARINE_SHARD =
				{
					Notes = "The itemtype for prismarine shard"
				},
				E_ITEM_PUMPKIN_PIE =
				{
					Notes = "The itemtype for pumpkin pie"
				},
				E_ITEM_PUMPKIN_SEEDS =
				{
					Notes = "The itemtype for pumpkin seeds"
				},
				E_ITEM_RABBITS_FOOT =
				{
					Notes = "The itemtype for rabbits foot"
				},
				E_ITEM_RABBIT_HIDE =
				{
					Notes = "The itemtype for rabbit hide"
				},
				E_ITEM_RABBIT_STEW =
				{
					Notes = "The itemtype for rabbit stew"
				},
				E_ITEM_RAW_BEEF =
				{
					Notes = "The itemtype for raw beef"
				},
				E_ITEM_RAW_CHICKEN =
				{
					Notes = "The itemtype for raw chicken"
				},
				E_ITEM_RAW_FISH =
				{
					Notes = "The itemtype for raw fish"
				},
				E_ITEM_RAW_MUTTON =
				{
					Notes = "The itemtype for raw mutton"
				},
				E_ITEM_RAW_PORKCHOP =
				{
					Notes = "The itemtype for raw porkchop"
				},
				E_ITEM_RAW_RABBIT =
				{
					Notes = "The itemtype for raw rabbit"
				},
				E_ITEM_REDSTONE_DUST =
				{
					Notes = "The itemtype for redstone dust"
				},
				E_ITEM_REDSTONE_REPEATER =
				{
					Notes = "The itemtype for redstone repeater"
				},
				E_ITEM_RED_APPLE =
				{
					Notes = "The itemtype for red apple"
				},
				E_ITEM_ROTTEN_FLESH =
				{
					Notes = "The itemtype for rotten flesh"
				},
				E_ITEM_SADDLE =
				{
					Notes = "The itemtype for saddle"
				},
				E_ITEM_SEEDS =
				{
					Notes = "The itemtype for seeds"
				},
				E_ITEM_SHEARS =
				{
					Notes = "The itemtype for shears"
				},
				E_ITEM_SHIELD =
				{
					Notes = "The itemtype for shield"
				},
				E_ITEM_SHULKER_SHELL =
				{
					Notes = "The itemtype for shulker shell"
				},
				E_ITEM_SIGN =
				{
					Notes = "The itemtype for sign"
				},
				E_ITEM_SLIMEBALL =
				{
					Notes = "The itemtype for slimeball"
				},
				E_ITEM_SNOWBALL =
				{
					Notes = "The itemtype for snowball"
				},
				E_ITEM_SPAWN_EGG =
				{
					Notes = "The itemtype for spawn egg"
				},
				E_ITEM_SPECTRAL_ARROW =
				{
					Notes = "The itemtype for spectral arrow"
				},
				E_ITEM_SPIDER_EYE =
				{
					Notes = "The itemtype for spider eye"
				},
				E_ITEM_SPLASH_POTION =
				{
					Notes = "The itemtype for splash potion"
				},
				E_ITEM_SPRUCE_BOAT =
				{
					Notes = "The itemtype for spruce boat"
				},
				E_ITEM_SPRUCE_DOOR =
				{
					Notes = "The itemtype for spruce door"
				},
				E_ITEM_STAL_DISC =
				{
					Notes = "The itemtype for stal disc"
				},
				E_ITEM_STEAK =
				{
					Notes = "The itemtype for steak"
				},
				E_ITEM_STICK =
				{
					Notes = "The itemtype for stick"
				},
				E_ITEM_STONE_AXE =
				{
					Notes = "The itemtype for stone axe"
				},
				E_ITEM_STONE_HOE =
				{
					Notes = "The itemtype for stone hoe"
				},
				E_ITEM_STONE_PICKAXE =
				{
					Notes = "The itemtype for stone pickaxe"
				},
				E_ITEM_STONE_SHOVEL =
				{
					Notes = "The itemtype for stone shovel"
				},
				E_ITEM_STONE_SWORD =
				{
					Notes = "The itemtype for stone sword"
				},
				E_ITEM_STRAD_DISC =
				{
					Notes = "The itemtype for strad disc"
				},
				E_ITEM_STRING =
				{
					Notes = "The itemtype for string"
				},
				E_ITEM_SUGAR =
				{
					Notes = "The itemtype for sugar"
				},
				E_ITEM_SUGARCANE =
				{
					Notes = "The itemtype for sugarcane"
				},
				E_ITEM_SUGAR_CANE =
				{
					Notes = "The itemtype for sugar cane"
				},
				E_ITEM_TIPPED_ARROW =
				{
					Notes = "The itemtype for tipped arrow"
				},
				E_ITEM_TOTEM_OF_UNDYING =
				{
					Notes = "The itemtype for totem of undying"
				},
				E_ITEM_WAIT_DISC =
				{
					Notes = "The itemtype for wait disc"
				},
				E_ITEM_WARD_DISC =
				{
					Notes = "The itemtype for ward disc"
				},
				E_ITEM_WATER_BUCKET =
				{
					Notes = "The itemtype for water bucket"
				},
				E_ITEM_WHEAT =
				{
					Notes = "The itemtype for wheat"
				},
				E_ITEM_WOODEN_AXE =
				{
					Notes = "The itemtype for wooden axe"
				},
				E_ITEM_WOODEN_DOOR =
				{
					Notes = "The itemtype for wooden door"
				},
				E_ITEM_WOODEN_HOE =
				{
					Notes = "The itemtype for wooden hoe"
				},
				E_ITEM_WOODEN_PICKAXE =
				{
					Notes = "The itemtype for wooden pickaxe"
				},
				E_ITEM_WOODEN_SHOVEL =
				{
					Notes = "The itemtype for wooden shovel"
				},
				E_ITEM_WOODEN_SWORD =
				{
					Notes = "The itemtype for wooden sword"
				},
				E_ITEM_WRITTEN_BOOK =
				{
					Notes = "The itemtype for written book"
				},
				BLOCK_FACE_BOTTOM =
				{
					Notes = "Please use BLOCK_FACE_YM instead. Interacting with the bottom face of the block.",
				},
				BLOCK_FACE_EAST =
				{
					Notes = "Please use BLOCK_FACE_XM instead. Interacting with the eastern face of the block.",
				},
				BLOCK_FACE_MAX =
				{
					Notes = "Used for range checking - highest legal value for an {{Globals#eBlockFace|eBlockFace}}",
				},
				BLOCK_FACE_MIN =
				{
					Notes = "Used for range checking - lowest legal value for an {{Globals#eBlockFace|eBlockFace}}",
				},
				BLOCK_FACE_NONE =
				{
					Notes = "Interacting with no block face - swinging the item in the air",
				},
				BLOCK_FACE_NORTH =
				{
					Notes = "Please use BLOCK_FACE_ZM instead. Interacting with the northern face of the block.",
				},
				BLOCK_FACE_SOUTH =
				{
					Notes = "Please use BLOCK_FACE_ZP instead. Interacting with the southern face of the block.",
				},
				BLOCK_FACE_TOP =
				{
					Notes = "Please use BLOCK_FACE_YP instead. Interacting with the top face of the block.",
				},
				BLOCK_FACE_WEST =
				{
					Notes = "Please use BLOCK_FACE_XP instead. Interacting with the western face of the block.",
				},
				BLOCK_FACE_XM =
				{
					Notes = "Interacting with the X- face of the block",
				},
				BLOCK_FACE_XP =
				{
					Notes = "Interacting with the X+ face of the block",
				},
				BLOCK_FACE_YM =
				{
					Notes = "Interacting with the Y- face of the block",
				},
				BLOCK_FACE_YP =
				{
					Notes = "Interacting with the Y+ face of the block",
				},
				BLOCK_FACE_ZM =
				{
					Notes = "Interacting with the Z- face of the block",
				},
				BLOCK_FACE_ZP =
				{
					Notes = "Interacting with the Z+ face of the block",
				},
				DIG_STATUS_CANCELLED =
				{
					Notes = "The player has let go of the mine block key before finishing mining the block",
				},
				DIG_STATUS_DROP_HELD =
				{
					Notes = "The player has dropped a single item using the Drop Item key (default: Q)",
				},
				DIG_STATUS_DROP_STACK =
				{
					Notes = "The player has dropped a full stack of items using the Drop Item key (default: Q) while holding down a specific modifier key (in windows, control)",
				},
				DIG_STATUS_FINISHED =
				{
					Notes = "The player thinks that it has finished mining a block",
				},
				DIG_STATUS_SHOOT_EAT =
				{
					Notes = "The player has finished shooting a bow or finished eating",
				},
				DIG_STATUS_STARTED =
				{
					Notes = "The player has started digging",
				},
				DIG_STATUS_SWAP_ITEM_IN_HAND =
				{
					Notes = "The player has swapped their held item with the item in their offhand slot (1.9)",
				},
				E_META_BIG_FLOWER_TOP =
				{
					Notes = "The metadata of a big flower block that indicates it is the top block.",
				},
				E_META_CONCRETE_BLACK =
				{
					Notes = "A flag in the metadata of concete that indicates that the concrete is black.",
				},
				E_META_CONCRETE_BLUE =
				{
					Notes = "A flag in the metadata of concete that indicates that the concrete is blue.",
				},
				E_META_CONCRETE_BROWN =
				{
					Notes = "A flag in the metadata of concete that indicates that the concrete is brown.",
				},
				E_META_CONCRETE_CYAN =
				{
					Notes = "A flag in the metadata of concete that indicates that the concrete is cyan.",
				},
				E_META_CONCRETE_GRAY =
				{
					Notes = "A flag in the metadata of concete that indicates that the concrete is gray.",
				},
				E_META_CONCRETE_GREEN =
				{
					Notes = "A flag in the metadata of concete that indicates that the concrete is green.",
				},
				E_META_CONCRETE_LIGHTBLUE =
				{
					Notes = "A flag in the metadata of concete that indicates that the concrete is light blue.",
				},
				E_META_CONCRETE_LIGHTGRAY =
				{
					Notes = "A flag in the metadata of concete that indicates that the concrete is light gray.",
				},
				E_META_CONCRETE_LIGHTGREEN =
				{
					Notes = "A flag in the metadata of concete that indicates that the concrete is light green.",
				},
				E_META_CONCRETE_MAGENTA =
				{
					Notes = "A flag in the metadata of concete that indicates that the concrete is magenta.",
				},
				E_META_CONCRETE_ORANGE =
				{
					Notes = "A flag in the metadata of concete that indicates that the concrete is orange.",
				},
				E_META_CONCRETE_PINK =
				{
					Notes = "A flag in the metadata of concete that indicates that the concrete is pink.",
				},
				E_META_CONCRETE_POWDER_BLACK =
				{
					Notes = "A flag in the metadata of concete powder that indicates that the concrete powder is black.",
				},
				E_META_CONCRETE_POWDER_BLUE =
				{
					Notes = "A flag in the metadata of concete powder that indicates that the concrete powder is blue.",
				},
				E_META_CONCRETE_POWDER_BROWN =
				{
					Notes = "A flag in the metadata of concete powder that indicates that the concrete powder is brown.",
				},
				E_META_CONCRETE_POWDER_CYAN =
				{
					Notes = "A flag in the metadata of concete powder that indicates that the concrete powder is cyan.",
				},
				E_META_CONCRETE_POWDER_GRAY =
				{
					Notes = "A flag in the metadata of concete powder that indicates that the concrete powder is gray.",
				},
				E_META_CONCRETE_POWDER_GREEN =
				{
					Notes = "A flag in the metadata of concete powder that indicates that the concrete powder is green.",
				},
				E_META_CONCRETE_POWDER_LIGHTBLUE =
				{
					Notes = "A flag in the metadata of concete powder that indicates that the concrete powder is light blue.",
				},
				E_META_CONCRETE_POWDER_LIGHTGRAY =
				{
					Notes = "A flag in the metadata of concete powder that indicates that the concrete powder is light gray.",
				},
				E_META_CONCRETE_POWDER_LIGHTGREEN =
				{
					Notes = "A flag in the metadata of concete powder that indicates that the concrete powder is light green.",
				},
				E_META_CONCRETE_POWDER_MAGENTA =
				{
					Notes = "A flag in the metadata of concete powder that indicates that the concrete powder is magenta.",
				},
				E_META_CONCRETE_POWDER_ORANGE =
				{
					Notes = "A flag in the metadata of concete powder that indicates that the concrete powder is orange.",
				},
				E_META_CONCRETE_POWDER_PINK =
				{
					Notes = "A flag in the metadata of concete powder that indicates that the concrete powder is pink.",
				},
				E_META_CONCRETE_POWDER_PURPLE =
				{
					Notes = "A flag in the metadata of concete powder that indicates that the concrete powder is purple.",
				},
				E_META_CONCRETE_POWDER_RED =
				{
					Notes = "A flag in the metadata of concete powder that indicates that the concrete powder is red.",
				},
				E_META_CONCRETE_POWDER_WHITE =
				{
					Notes = "A flag in the metadata of concete powder that indicates that the concrete powder is white.",
				},
				E_META_CONCRETE_POWDER_YELLOW =
				{
					Notes = "A flag in the metadata of concete powder that indicates that the concrete powder is yellow.",
				},
				E_META_CONCRETE_PURPLE =
				{
					Notes = "A flag in the metadata of concete that indicates that the concrete is purple.",
				},
				E_META_CONCRETE_RED =
				{
					Notes = "A flag in the metadata of concete that indicates that the concrete is red.",
				},
				E_META_CONCRETE_WHITE =
				{
					Notes = "A flag in the metadata of concete that indicates that the concrete is white.",
				},
				E_META_CONCRETE_YELLOW =
				{
					Notes = "A flag in the metadata of concete that indicates that the concrete is yellow.",
				},
				E_META_DROPSPENSER_ACTIVATED =
				{
					Notes = "A flag in the metadata of droppers and dispensers that indicates that the dropper or dispenser is currently activated. If this flag is set, the block must be unpowered first and powered again to shoot the next item.",
				},
				E_META_DROPSPENSER_FACING_MASK =
				{
					Notes = "A mask that indicates the bits of the metadata that specify the facing of droppers and dispensers.",
				},
				E_META_DROPSPENSER_FACING_XM =
				{
					Notes = "A flag in the metadata of droppers and dispensers that indicates that the dropper or dispenser is looking in the negative X direction.",
				},
				E_META_DROPSPENSER_FACING_XP =
				{
					Notes = "A flag in the metadata of droppers and dispensers that indicates that the dropper or dispenser is looking in the positive X direction.",
				},
				E_META_DROPSPENSER_FACING_YM =
				{
					Notes = "A flag in the metadata of droppers and dispensers that indicates that the dropper or dispenser is looking in the negative Y direction.",
				},
				E_META_DROPSPENSER_FACING_YP =
				{
					Notes = "A flag in the metadata of droppers and dispensers that indicates that the dropper or dispenser is looking in the positive Y direction.",
				},
				E_META_DROPSPENSER_FACING_ZM =
				{
					Notes = "A flag in the metadata of droppers and dispensers that indicates that the dropper or dispenser is looking in the negative Z direction.",
				},
				E_META_DROPSPENSER_FACING_ZP =
				{
					Notes = "A flag in the metadata of droppers and dispensers that indicates that the dropper or dispenser is looking in the positive Z direction.",
				},
				E_META_END_PORTAL_FRAME_EYE =
				{
					Notes = "A flag in the metadata of end portal frames that indicates that the portal frame has an eye in it.",
				},
				E_META_END_PORTAL_FRAME_NO_EYE =
				{
					Notes = "The lack of the flag in the metadata of end portal frames indicating that the portal frame has an eye in it.",
				},
				E_META_END_PORTAL_FRAME_XM =
				{
					Notes = "A flag in the metadata of end portal frames that indicates that the portal frame is facing the negative X direction.",
				},
				E_META_END_PORTAL_FRAME_XP =
				{
					Notes = "A flag in the metadata of end portal frames that indicates that the portal frame is facing the positive X direction.",
				},
				E_META_END_PORTAL_FRAME_ZM =
				{
					Notes = "A flag in the metadata of end portal frames that indicates that the portal frame is facing the negative Z direction.",
				},
				E_META_END_PORTAL_FRAME_ZP =
				{
					Notes = "A flag in the metadata of end portal frames that indicates that the portal frame is facing the positive Z direction.",
				},
				E_META_END_PORTAL_FRAME_XM_EYE =
				{
					Notes = "A flag in the metadata of end portal frames that indicates that the portal frame is facing the negative X direction and has an ender eye in it.",
				},
				E_META_END_PORTAL_FRAME_XP_EYE =
				{
					Notes = "A flag in the metadata of end portal frames that indicates that the portal frame is facing the positive X direction and has an ender eye in it.",
				},
				E_META_END_PORTAL_FRAME_ZM_EYE =
				{
					Notes = "A flag in the metadata of end portal frames that indicates that the portal frame is facing the negative Z direction and has an ender eye in it.",
				},
				E_META_END_PORTAL_FRAME_ZP_EYE =
				{
					Notes = "A flag in the metadata of end portal frames that indicates that the portal frame is facing the positive Z direction and has an ender eye in it.",
				},
				E_META_HEAD_CREEPER =
				{
					Notes = "A flag in the metadata of heads that indicates that the head is a creeper head.",
				},
				E_META_HEAD_DRAGON =
				{
					Notes = "A flag in the metadata of heads that indicates that the head is a dragon head.",
				},
				E_META_HEAD_PLAYER =
				{
					Notes = "A flag in the metadata of heads that indicates that the head is a player head.",
				},
				E_META_HEAD_SKELETON =
				{
					Notes = "A flag in the metadata of heads that indicates that the head is a skeleton head.",
				},
				E_META_HEAD_WITHER =
				{
					Notes = "A flag in the metadata of heads that indicates that the head is a wither head.",
				},
				E_META_HEAD_ZOMBIE =
				{
					Notes = "A flag in the metadata of heads that indicates that the head is a zombie head.",
				},
				E_META_REDSTONE_REPEATER_FACING_ZM =
				{
					Notes = "A flag in the metadata of redstone repeaters that indicates that the repeater is looking in the negative Z direction.",
				},
				E_META_REDSTONE_REPEATER_FACING_XP =
				{
					Notes = "A flag in the metadata of redstone repeaters that indicates that the repeater is looking in the positive X direction.",
				},
				E_META_REDSTONE_REPEATER_FACING_ZP =
				{
					Notes = "A flag in the metadata of redstone repeaters that indicates that the repeater is looking in the positive Z direction.",
				},
				E_META_REDSTONE_REPEATER_FACING_XM =
				{
					Notes = "A flag in the metadata of redstone repeaters that indicates that the repeater is looking in the negative X direction.",
				},
				E_META_REDSTONE_REPEATER_FACING_MASK =
				{
					Notes = "A mask that indicates the bits of the metadata that specify the facing of redstone repeaters.",
				},
				E_META_SPAWN_EGG_WITHER_SKELETON =
				{
					Notes = ""
				},
				E_META_SILVERFISH_EGG_CHISELED_STONE_BRICK =
				{
					Notes = "A flag in the metadata of the silverfish egg that the block is made from chiseled stone bricks"
				},
				E_META_SILVERFISH_EGG_CRACKED_STONE_BRICK =
				{
					Notes = "A flag in the metadata of the silverfish egg that the block is made from cracked stone bricks"
				},
				E_META_SILVERFISH_EGG_MOSSY_STONE_BRICK =
				{
					Notes =  "A flag in the metadata of the silverfish egg that the block is made from mossy stone bricks"
				},
				E_META_SPONGE_DRY =
				{
					Notes = "A flag in the metadata of sponges that indicates that the sponge is dry.",
				},
				E_META_SPONGE_WET =
				{
					Notes = "A flag in the metadata of sponges that indicates that the sponge is wet.",
				},
				esBed =
				{
					Notes = "A bed explosion. The SourceData param is the {{Vector3i|position}} of the bed.",
				},
				esEnderCrystal =
				{
					Notes = "An ender crystal entity explosion. The SourceData param is the {{cEntity|ender crystal entity}} object.",
				},
				esGhastFireball =
				{
					Notes = "A ghast fireball explosion. The SourceData param is the {{cGhastFireballEntity|ghast fireball entity}} object.",
				},
				esMonster =
				{
					Notes = "A monster explosion (creeper). The SourceData param is the {{cMonster|monster entity}} object.",
				},
				esOther =
				{
					Notes = "Any other explosion. The SourceData param is unused.",
				},
				esPlugin =
				{
					Notes = "An explosion started by a plugin, without any further information. The SourceData param is unused. ",
				},
				esPrimedTNT =
				{
					Notes = "A TNT explosion. The SourceData param is the {{cTNTEntity|TNT entity}} object.",
				},
				esWitherBirth =
				{
					Notes = "An explosion at a wither's birth. The SourceData param is the {{cMonster|wither entity}} object.",
				},
				esWitherSkull =
				{
					Notes = "A wither skull explosion. The SourceData param is the {{cWitherSkullEntity|wither skull entity}} object.",
				},

				-- eMonsterType:
				mtBat =
				{
					Notes = "",
				},
				mtBlaze =
				{
					Notes = "",
				},
				mtCaveSpider =
				{
					Notes = "",
				},
				mtChicken =
				{
					Notes = "",
				},
				mtCow =
				{
					Notes = "",
				},
				mtCreeper =
				{
					Notes = "",
				},
				mtEnderDragon =
				{
					Notes = "",
				},
				mtEnderman =
				{
					Notes = "",
				},
				mtGhast =
				{
					Notes = "",
				},
				mtGiant =
				{
					Notes = "",
				},
				mtHorse =
				{
					Notes = "",
				},
				mtInvalidType =
				{
					Notes = "Invalid monster type. Returned when monster type not recognized",
				},
				mtIronGolem =
				{
					Notes = "",
				},
				mtMagmaCube =
				{
					Notes = "",
				},
				mtMooshroom =
				{
					Notes = "",
				},
				mtOcelot =
				{
					Notes = "",
				},
				mtPig =
				{
					Notes = "",
				},
				mtSheep =
				{
					Notes = "",
				},
				mtSilverfish =
				{
					Notes = "",
				},
				mtSkeleton =
				{
					Notes = "",
				},
				mtSlime =
				{
					Notes = "",
				},
				mtSnowGolem =
				{
					Notes = "",
				},
				mtSpider =
				{
					Notes = "",
				},
				mtSquid =
				{
					Notes = "",
				},
				mtVillager =
				{
					Notes = "",
				},
				mtWitch =
				{
					Notes = "",
				},
				mtWither =
				{
					Notes = "",
				},
				mtWitherSkeleton =
				{
					Notes = "",
				},
				mtWolf =
				{
					Notes = "",
				},
				mtZombie =
				{
					Notes = "",
				},
				mtZombiePigman =
				{
					Notes = "",
				},
				mtZombieVillager =
				{
					Notes = "",
				},

				-- eMessageType:
				mtCustom =
				{
					Notes = "Send raw data without any processing",
				},
				mtDeath =
				{
					Notes = "Denotes death of player",
				},
				mtError =
				{
					Notes = "Something could not be done (i.e. command not executed due to insufficient privilege)",
				},
				mtFail =
				{
					Notes = "Something could not be done (i.e. command not executed due to insufficient privilege)",
				},
				mtFailure =
				{
					Notes = "Something could not be done (i.e. command not executed due to insufficient privilege)",
				},
				mtFatal =
				{
					Notes = "Something catastrophic occured (i.e. plugin crash)",
				},
				mtInfo =
				{
					Notes = "Informational message (i.e. command usage)",
				},
				mtInformation =
				{
					Notes = "Informational message (i.e. command usage)",
				},
				mtJoin =
				{
					Notes = "A player has joined the server",
				},
				mtLeave =
				{
					Notes = "A player has left the server",
				},
				mtMaxPlusOne =
				{
					Notes = "The first invalid type, used for checking on LuaAPI boundaries",
				},
				mtPM =
				{
					Notes = "Player to player messaging identifier",
				},
				mtPrivateMessage =
				{
					Notes = "Player to player messaging identifier",
				},
				mtSuccess =
				{
					Notes = "Something executed successfully",
				},
				mtWarning =
				{
					Notes = "Something concerning (i.e. reload) is about to happen",
				},
				mtWitherSkeleton =
				{
					Notes = ""
				},
				hMain =
				{
					Notes = "The main hand",
				},
				hOff =
				{
					Notes = "The off hand",
				},
				mhLeft =
				{
					Notes = "The left hand is the main hand",
				},
				mhRight =
				{
					Notes = "The right hand is the main hand",
				},
				SKULL_TYPE_CREEPER =
				{
					Notes = "A creeper skull",
				},
				SKULL_TYPE_DRAGON =
				{
					Notes = "A dragon skull",
				},
				SKULL_TYPE_PLAYER =
				{
					Notes = "A player skull",
				},
				SKULL_TYPE_SKELETON =
				{
					Notes = "A skeleton skull",
				},
				SKULL_TYPE_WITHER =
				{
					Notes = "A wither skull",
				},
				SKULL_TYPE_ZOMBIE =
				{
					Notes = "A zombie skull",
				},
				spCape =
				{
					Notes = "The cape skin part",
				},
				spJacket =
				{
					Notes = "The jacket skin part",
				},
				spLeftSleeve =
				{
					Notes = "The left sleeve skin part",
				},
				spRightSleeve =
				{
					Notes = "The right sleeve skin part",
				},
				spLeftPants =
				{
					Notes = "The left pants leg skin part",
				},
				spRightPants =
				{
					Notes = "The right pants leg skin part",
				},
				spHat =
				{
					Notes = "The hat/head skin part",
				},
				spMask =
				{
					Notes = "A mask of all valid skin parts combined",
				},
			},
			ConstantGroups =
			{
				eBlockFace =
				{
					Include = "^BLOCK_FACE_.*",
					TextBefore = [[
						These constants are used to describe individual faces of the block. They are used when the
						client is interacting with a block in the {{OnPlayerBreakingBlock|HOOK_PLAYER_BREAKING_BLOCK}},
						{{OnPlayerBrokenBlock|HOOK_PLAYER_BROKEN_BLOCK}}, {{OnPlayerLeftClick|HOOK_PLAYER_LEFT_CLICK}},
						{{OnPlayerPlacedBlock|HOOK_PLAYER_PLACED_BLOCK}}, {{OnPlayerPlacingBlock|HOOK_PLAYER_PLACING_BLOCK}},
						{{OnPlayerRightClick|HOOK_PLAYER_RIGHT_CLICK}}, {{OnPlayerUsedBlock|HOOK_PLAYER_USED_BLOCK}},
						{{OnPlayerUsedItem|HOOK_PLAYER_USED_ITEM}}, {{OnPlayerUsingBlock|HOOK_PLAYER_USING_BLOCK}},
						and {{OnPlayerUsingItem|HOOK_PLAYER_USING_ITEM}} hooks, or when the {{cLineBlockTracer}} hits a
						block, etc.
					]],
				},
				eBlockType =
				{
					Include = "^E_BLOCK_.*",
					TextBefore = [[
						These constants are used for block types. They correspond directly with MineCraft's data values
						for blocks.
					]],
				},
				eClickAction =
				{
					Include = "^ca.*",
					TextBefore = [[
						These constants are used to signalize various interactions that the user can do with the
						{{cWindow|UI windows}}. The server translates the protocol events into these constants. Note
						that there is a global ClickActionToString() function that can translate these constants into
						their textual representation.
					]],
				},
				eDamageType =
				{
					Include = "^dt.*",
					TextBefore = [[
						These constants are used for specifying the cause of damage to entities. They are used in the
						{{TakeDamageInfo}} structure, as well as in {{cEntity}}'s damage-related API functions.
					]],
				},
				DigStatuses =
				{
					Include = "^DIG_STATUS_.*",
					TextBefore = [[
						These constants are used to describe digging statuses, but in reality cover several more cases.
						They are used with {{OnPlayerLeftClick|HOOK_PLAYER_LEFT_CLICK}}.
					]],
				},
				eDimension =
				{
					Include = "^dim.*",
					TextBefore = [[
						These constants represent dimension of a world. In Cuberite, the dimension is only reflected in
						the world's overall tint - overworld gets sky-like colors and dark shades, the nether gets
						reddish haze and the end gets dark haze. World generator is not directly affected by the
						dimension, same as fluid simulators; those only default to the expected values if not set
						specifically otherwise in the world.ini file.
					]],
				},
				eExplosionSource =
				{
					Include = "^es.*",
					TextBefore = [[
						These constants are used to differentiate the various sources of explosions. They are used in
						the {{OnExploded|HOOK_EXPLODED}} hook, {{OnExploding|HOOK_EXPLODING}} hook and in the
						{{cWorld}}:DoExplosionAt() function. These constants also dictate the type of the additional
						data provided with the explosions, such as the exploding creeper {{cEntity|entity}} or the
						{{Vector3i|coords}} of the exploding bed.
					]],
				},
				eGameMode =
				{
					Include =
					{
						"^gm.*",
						"^eGameMode_.*",
					},
					TextBefore = [[
						The following constants are used for the gamemode - survival, creative or adventure. Use the
						gmXXX constants, the eGameMode_ constants are deprecated and will be removed from the API.
					]],
				},
				eHand =
				{
					Include =
					{
						"hMain",
						"hOff",
					},
					TextBefore = [[
						These constants represent the main and off hand.  Currently, these constants are not used, but
						are provided for future use when dual-wielding is functional.  An action or item can be in the
						main hand or the off hand.  The main hand can be either the left or the right hand - use
						{{cPlayer}}:GetMainHand() to determine which (see {{Globals#eMainHand|eMainHand}}).
					]],
				},
				eMainHand =
				{
					Include =
					{
						"^mh.*",
					},
					TextBefore = [[
						These constants identify which hand is the main hand.  The main hand can either be the left hand
						or the right hand.  Note that this is only visual, as the client behaves the same regardless of the
						main hand setting.  See {{cPlayer}}:GetMainHand().
					]],
				},
				EMCSBiome =
				{
					Include = "^bi.*",
					TextBefore = [[
						These constants represent the biomes that the server understands. Note that there is a global
						StringToBiome() function that can convert a string into one of these constants.
					]],
				},
				eMessageType =
				{
					-- Need to be specified explicitly, because there's also eMonsterType using the same "mt" prefix
					Include =
					{
						"mtCustom",
						"mtDeath",
						"mtError",
						"mtFail",
						"mtFailure",
						"mtFatal",
						"mtInfo",
						"mtInformation",
						"mtJoin",
						"mtLeave",
						"mtMaxPlusOne",
						"mtPrivateMessage",
						"mtPM",
						"mtSuccess",
						"mtWarning",
					},
					TextBefore = [[
						These constants are used together with messaging functions and classes, they specify the type of
						message being sent. The server can be configured to modify the message text (add prefixes) based
						on the message's type.
					]],
				},
				eMobHeadType =
				{
					Include = "SKULL_TYPE_.*",
				},
				eMobHeadRotation =
				{
					Include = "SKULL_ROTATION_.*",
				},
				eMonsterType =
				{
					Include =
					{
						"mtInvalidType",
						"mtBat",
						"mtBlaze",
						"mtCaveSpider",
						"mtChicken",
						"mtCow",
						"mtCreeper",
						"mtEnderDragon",
						"mtEnderman",
						"mtGhast",
						"mtGiant",
						"mtGuardian",
						"mtHorse",
						"mtIronGolem",
						"mtMagmaCube",
						"mtMooshroom",
						"mtOcelot",
						"mtPig",
						"mtRabbit",
						"mtSheep",
						"mtSilverfish",
						"mtSkeleton",
						"mtSlime",
						"mtSnowGolem",
						"mtSpider",
						"mtSquid",
						"mtVillager",
						"mtWitch",
						"mtWither",
						"mtWitherSkeleton",
						"mtWolf",
						"mtZombie",
						"mtZombiePigman",
						"mtZombieVillager",
						"mtMax",
					},
					TextBefore = [[
						The following constants are used for distinguishing between the individual mob types:
					]],
				},
				eShrapnelLevel =
				{
					Include = "^sl.*",
					TextBefore = [[
						The following constants define the block types that are propelled outwards after an explosion.
					]],
				},
				eSkinPart =
				{
					Include =
					{
						"^sp.*",
					},
					TextBefore = [[
						These constants represent various skin part flags.
					]],
				},
				eSpreadSource =
				{
					Include = "^ss.*",
					TextBefore = [[
						These constants are used to differentiate the various sources of spreads, such as grass growing.
						They are used in the {{OnBlockSpread|HOOK_BLOCK_SPREAD}} hook.
					]],
				},
				eWeather =
				{
					Include =
					{
						"^eWeather_.*",
						"wSunny",
						"wRain",
						"wStorm",
						"wThunderstorm",
					},
					TextBefore = [[
						These constants represent the weather in the world. Note that unlike vanilla, Cuberite allows
						different weathers even in non-overworld {{Globals#eDimension|dimensions}}.
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
			},
		},
		ItemCategory =
		{
			Desc = [[
				This class contains static functions for determining item categories. All of the functions are
				called directly on the class table, unlike most other object, which require an instance first.
			]],
			Functions =
			{
				IsArmor =
				{
					IsStatic = true,
					Params =
					{
						{
							Name = "ItemType",
							Type = "number",
						},
					},
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Returns true if the specified item type is any kind of an armor.",
				},
				IsAxe =
				{
					IsStatic = true,
					Params =
					{
						{
							Name = "ItemType",
							Type = "number",
						},
					},
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Returns true if the specified item type is any kind of an axe.",
				},
				IsBoots =
				{
					IsStatic = true,
					Params =
					{
						{
							Name = "ItemType",
							Type = "number",
						},
					},
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Returns true if the specified item type is any kind of boots.",
				},
				IsChestPlate =
				{
					IsStatic = true,
					Params =
					{
						{
							Name = "ItemType",
							Type = "number",
						},
					},
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Returns true if the specified item type is any kind of a chestplate.",
				},
				IsHelmet =
				{
					IsStatic = true,
					Params =
					{
						{
							Name = "ItemType",
							Type = "number",
						},
					},
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Returns true if the specified item type is any kind of a helmet.",
				},
				IsHoe =
				{
					IsStatic = true,
					Params =
					{
						{
							Name = "ItemType",
							Type = "number",
						},
					},
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Returns true if the specified item type is any kind of a hoe.",
				},
				IsHorseArmor=
				{
					IsStatic = true,
					Params =
					{
						{
							Name = "ItemType",
							Type = "number",
						},
					},
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Returns true if the specified item type is any kind of a horse armor.",
				},
				IsLeggings =
				{
					IsStatic = true,
					Params =
					{
						{
							Name = "ItemType",
							Type = "number",
						},
					},
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Returns true if the specified item type is any kind of a leggings.",
				},
				IsMinecart =
				{
					IsStatic = true,
					Params =
					{
						{
							Name = "ItemType",
							Type = "number",
						},
					},
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Returns true if the specified item type is any kind of a minecart.",
				},
				IsPickaxe =
				{
					IsStatic = true,
					Params =
					{
						{
							Name = "ItemType",
							Type = "number",
						},
					},
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Returns true if the specified item type is any kind of a pickaxe.",
				},
				IsShovel =
				{
					IsStatic = true,
					Params =
					{
						{
							Name = "ItemType",
							Type = "number",
						},
					},
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Returns true if the specified item type is any kind of a shovel.",
				},
				IsSword =
				{
					IsStatic = true,
					Params =
					{
						{
							Name = "ItemType",
							Type = "number",
						},
					},
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Returns true if the specified item type is any kind of a sword.",
				},
				IsTool =
				{
					IsStatic = true,
					Params =
					{
						{
							Name = "ItemType",
							Type = "number",
						},
					},
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Returns true if the specified item type is any kind of a tool (axe, hoe, pickaxe, shovel or FIXME: sword)",
				},
			},
			AdditionalInfo =
			{
				{
					Header = "Code example",
					Contents = [[
						The following code snippet checks if the player holds a shovel.
<pre class="prettyprint lang-lua">
-- a_Player is a {{cPlayer}} object, possibly received as a hook param
local HeldItem = a_Player:GetEquippedItem()
if (ItemCategory.IsShovel(HeldItem.m_ItemType)) then
	-- It's a shovel
end
</pre>
					]],
				},
			},
		},
		lxp =
		{
			Desc = [[
				This class provides an interface to the XML parser,
				{{https://matthewwild.co.uk/projects/luaexpat/|LuaExpat}}. It provides a SAX interface with an
				incremental XML parser.</p>
				<p>
				With an event-based API like SAX the XML document can be fed to the parser in chunks, and the
				parsing begins as soon as the parser receives the first document chunk. LuaExpat reports parsing
				events (such as the start and end of elements) directly to the application through callbacks. The
				parsing of huge documents can benefit from this piecemeal operation.</p>
				<p>
				See the online
				{{https://matthewwild.co.uk/projects/luaexpat/manual.html#parser|LuaExpat documentation}} for details
				on how to work with this parser. The code examples below should provide some basic help, too.
			]],
			Functions =
			{
				new =
				{
					Params =
					{
						{
							Name = "CallbacksTable",
							Type = "table",
						},
						{
							Name = "SeparatorChar",
							Type = "string",
							IsOptional = true,
						},
					},
					Returns =
					{
						{
							Name = "XMLParser object",
							Type = "table",
						},
					},
					Notes = "Creates a new XML parser object, with the specified callbacks table and optional separator character.",
				},
			},
			Constants =
			{
				_COPYRIGHT =
				{
					Notes = "",
				},
				_DESCRIPTION =
				{
					Notes = "",
				},
				_VERSION =
				{
					Notes = "",
				},
			},
			AdditionalInfo =
			{
				{
					Header = "Parser callbacks",
					Contents = [[
						The callbacks table passed to the new() function specifies the Lua functions that the parser
						calls upon various events. The following table lists the most common functions used, for a
						complete list see the online
						{{https://matthewwild.co.uk/projects/luaexpat/manual.html#parser|LuaExpat documentation}}.</p>
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
						{{https://matthewwild.co.uk/projects/luaexpat/manual.html#parser|LuaExpat documentation}}.
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
			},
		},
		sqlite3 =
		{
			Desc = [[
			]],
			Functions =
			{
				complete =
				{
					IsStatic = true,
					IsGlobal = true,  -- Emulate a global function without a self parameter - this is called with a dot convention
					Params =
					{
						{
							Name = "SQL",
							Type = "string",
						},
					},
					Returns =
					{
						{
							Type = "boolean",
						},
					},
					Notes = "Returns true if the input string comprises one or more complete SQL statements.",
				},
				open =
				{
					IsStatic = true,
					IsGlobal = true,  -- Emulate a global function without a self parameter - this is called with a dot convention
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
							Name = "DBClass",
							Type = "SQLite DB object",
						},
					},
					Notes = [[
					Opens (or creates if it does not exist) an SQLite database with name filename and returns its
					handle (the returned object should be used for all further method calls in connection
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
				]],
				},
				open_memory =
				{
					IsStatic = true,
					IsGlobal = true,  -- Emulate a global function without a self parameter - this is called with a dot convention
					Returns =
					{
						{
							Name = "DBClass",
							Type = "SQLite DB object",
						},
					},
					Notes = "Opens an SQLite database in memory and returns its handle. In case of an error, the function returns nil, an error code and an error message. (In-memory databases are volatile as they are never stored on disk.)",
				},
				version =
				{
					IsStatic = true,
					IsGlobal = true,  -- Emulate a global function without a self parameter - this is called with a dot convention
					Returns =
					{
						{
							Type = "string",
						},
					},
					Notes = "Returns a string with SQLite version information, in the form 'x.y[.z]'.",
				},
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
				Attacker =
				{
					Type = "{{cEntity}}",
					Notes = "The entity who is attacking. Only valid if dtAttack.",
				},
				DamageType =
				{
					Type = "eDamageType",
					Notes = "Source of the damage. One of the dtXXX constants.",
				},
				FinalDamage =
				{
					Type = "number",
					Notes = "The final amount of damage that will be applied to the Receiver. It is the RawDamage minus any Receiver's armor-protection.",
				},
				Knockback =
				{
					Type = "{{Vector3d}}",
					Notes = "Vector specifying the amount and direction of knockback that will be applied to the Receiver ",
				},
				RawDamage =
				{
					Type = "number",
					Notes = "Amount of damage that the attack produces on the Receiver, including the Attacker's equipped weapon, but excluding the Receiver's armor.",
				},
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
			},
		},
		tolua =
		{
			Desc = [[
				This class represents the tolua bridge between the Lua API and Cuberite. It supports some low
				level operations and queries on the objects. See also the tolua++'s documentation at
				{{https://www8.cs.umu.se/kurser/TDBD12/VT04/lab/lua/tolua++.html#utilities}}. Normally you shouldn't use any of these
				functions except for type()
			]],
			Functions =
			{
				cast =
				{
					IsStatic = true,
					IsGlobal = true,  -- Emulate a global function without a self parameter - this is called with a dot convention
					Params =
					{
						{
							Name = "Object",
							Type = "any",
						},
						{
							Name = "TypeStr",
							Type = "string",
						},
					},
					Returns =
					{
						{
							Name = "Object",
							Type = "any",
						},
					},
					Notes = "Casts the object to the specified type.<br/><b>Note:</b> This is a potentially unsafe operation and it could crash the server. There is normally no need to use this function at all, so don't use it unless you know exactly what you're doing.",
				},
				getpeer =
				{
					Notes = "",
				},
				inherit =
				{
					Notes = "",
				},
				releaseownership =
				{
					Notes = "",
				},
				setpeer =
				{
					Notes = "",
				},
				takeownership =
				{
					Notes = "",
				},
				type =
				{
					IsStatic = true,
					IsGlobal = true,  -- Emulate a global function without a self parameter - this is called with a dot convention
					Params =
					{
						{
							Name = "Object",
							Type = "any",
						},
					},
					Returns =
					{
						{
							Name = "TypeStr",
							Type = "string",
						},
					},
					Notes = "Returns a string representing the type of the object. This works similar to Lua's built-in type() function, but recognizes the underlying C++ classes, too.",
				},
			},
		},
	},
	ExtraPages =
	{
		{
			FileName = "Writing-a-Cuberite-plugin.html",
			Title = "Writing a Cuberite plugin",
		},
		{
			FileName = "InfoFile.html",
			Title = "Using the Info.lua file",
		},
		{
			FileName = "SettingUpDecoda.html",
			Title = "Setting up the Decoda Lua IDE",
		},
		{
			FileName = "SettingUpZeroBrane.html",
			Title = "Setting up the ZeroBrane Studio Lua IDE",
		},
		{
			FileName = "UsingChunkStays.html",
			Title = "Using ChunkStays",
		},
		{
			FileName = "WebWorldThreads.html",
			Title = "Webserver vs World threads",
		},
	},
	IgnoreClasses =
	{
		"^coroutine$",
		"^g_TrackedPages$",
		"^debug$",
		"^io$",
		"^math$",
		"^package$",
		"^os$",
		"^string$",
		"^table$",
		"^g_Stats$",
	},
	IgnoreFunctions =
	{
		"Globals.assert",
		"%a+%.delete",
		"CreateAPITables",
		"DumpAPIHtml",
		"DumpAPITxt",
		"Initialize",
		"LinkifyString",
		"ListMissingPages",
		"ListUndocumentedObjects",
		"ListUnexportedObjects",
		"LoadAPIFiles",
		"Globals.collectgarbage",
		"ReadDescriptions",
		"ReadHooks",
		"WriteHtmlClass",
		"WriteHtmlHook",
		"WriteStats",
		"Globals.xpcall",
		"Globals.decoda_output",
		"sqlite3.__newindex",
		"%a+%.__%a+",
		"%a+%.%.collector",
		"%a+%.new",
		"%a+%.new_local",
	},
	IgnoreConstants =
	{
		"cChestEntity.__cBlockEntityWindowOwner__",
		"cDropSpenserEntity.__cBlockEntityWindowOwner__",
		"cFurnaceEntity.__cBlockEntityWindowOwner__",
		"cHopperEntity.__cBlockEntityWindowOwner__",
		"cLuaWindow.__cItemGrid__cListener__",
		"Globals._CuberiteInternal_.*",
		"Globals.esMax",
		"Globals.E_BLOCK_*",
	},
	IgnoreVariables =
	{
		"__.*__",
	},
}
