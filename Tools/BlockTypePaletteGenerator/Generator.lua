-- lib/lunajson/src/ is not in default Lua package paths
package.path = 'lib/lunajson/src/?.lua;' .. package.path;

function usage(message)
	if message then
		io.output(io.stderr);
		io.write(message, "\n\n");
	end
	io.write("Generator.lua - Converts Minecraft report format to cuberite bl"..
		"ock type palette format.\n"..
		"\n"..
		"Usage: lua Generator.lua <inputfile> <outputfile>\n"..
		"\n"..
		"<inputfile> and <outputfile> must point to a valid path. <inputfile>"..
		" must be readable and outputfile must be writable. Either can be rep"..
		"laced with `-` (dash character) to point to standard input or output"..
		".");
	os.exit();
end

-- test path
if not pcall(function() require "lunajson.decoder" end) then
	usage("Could not load required libraries, please run `Generator.lua` with"..
		"in its directory and make sure to run `git submodule update`.");
end

local inpath, outpath = ...;
io.input(io.stdin);
io.output(io.stdout);

if select("#", ...) ~= 2 then
	usage();
end
if inpath ~= "-" then
	local handle, err = io.open(inpath, "r");
	io.input(handle or usage(err));
end
if outpath ~= "-" then
	local handle, err = io.open(outpath, "w");
	io.output(handle or usage(err));
end



local decode = (require "lunajson.decoder")();
local encode = (require "lunajson.encoder")();

local input = decode(io.input():read("*a"));
local registry = {};
local max_id = -1;


for blockname,blockdata in pairs(input) do
	for i=1, #(blockdata.states or {}) do
		local state = blockdata.states[i];
		assert(registry[state.id+1] == nil, "Duplicate id is an error state");

		-- needed in the end to verify we got no holes in the array:
		max_id = math.max(max_id, state.id);

		registry[state.id+1] = {
			id = assert(state.id, "id is required."),
			name = assert(blockname, "Block type name is required."),
			-- default = state.default or nil,
			props = state.properties,
		};
	end
end

-- io.stderr:write(#registry, " ", max_id, "\n");

-- The following assertion is not necessary by the current spec, but by required
-- by how lunajson distinguishes objects from arrays.
assert(#registry == max_id+1, "Ensure that registry has contiguous keys");

local out = {
	Metadata = {
		ProtocolBlockTypePaletteVersion = 1
	},
	Palette = registry
};

io.write(encode(out), "\n");
