
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
				Clear    = { Notes = "Clears the object, resets it to zero size" },
				CopyFrom = { Params = "{{cBlockArea|BlockAreaSrc}}", Notes = "Copies contents from BlockAreaSrc into self"},
				CopyTo   = { Params = "{{cBlockArea|BlockAreaDst}}", Notes = "Copies contents from self into BlockAreaDst"},
				GetBlockLight = { Params = "BlockX, BlockY, BlockZ", Return = "NIBBLETYPE", Notes = "Returns the blocklight at the specified absolute coords"},
			},
			Constants =
			{
				baTypes = { Notes = "Operation should work on block types" },
			},
		},
		
		cBlockEntity =
		{
		}
	},
	
	IgnoreFunctions =
	{
		"globals.assert",
		"globals.collectgarbage",
		"globals.xpcall",
	}
} ;




