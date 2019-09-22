-- lib/lunajson/src/ is not in default Lua package paths
package.path = 'lib/lunajson/src/?.lua;' .. package.path;


--- Prints usage instructions to stdout.
-- If the optional `message` is passed, output is prepended by message _and_
-- redirected to stderr.
function usage(message)
	if message then
		io.output(io.stderr);
		io.write(message, "\n\n");
	end
	io.write(
		"Usage: lua Generator.lua INPUTFILE OUTPUTFILE\n"..
		"Converts the Minecraft blocks.json report format to the cuberite "..
		"block type palette format.\n"..
		"\n"..
		"INPUTFILE and OUTPUTFILE must point to a valid path. INPUTFILE must "..
		"be readable and OUTPUTFILE must be writable. Either can be replaced "..
		"with `-` (dash character) to point to standard-input or -output.\n");
	os.exit(message and 1 or 0);
end


-- Test whether the script is run in a path where it can load it's libraries
if not pcall(function() require("lunajson.decoder") end) then
	usage("Could not load required libraries, please run `Generator.lua` "..
		"within its directory and make sure to run `git submodule update`.");
end


-- Check/Prepare CLI arguments
local inpath, outpath = ...;
io.input(io.stdin);
io.output(io.stdout);

if select("#", ...) ~= 2 then
	usage("Incorrect number of arguments.");
end

if inpath ~= "-" then
	local handle, err = io.open(inpath, "r");
	io.input(handle or usage(err));
end

if outpath ~= "-" then
	local handle, err = io.open(outpath, "w");
	io.output(handle or usage(err));
end


-- Main program starts here
local decode = (require("lunajson.decoder"))();
local encode = (require("lunajson.encoder"))();

local input = decode(io.input():read("*a"));
local registry = {};
local max_id = -1;


for blockname, blockdata in pairs(input) do
	for i = 1, #(blockdata.states or {}) do
		local state = blockdata.states[i];
		assert(registry[state.id + 1] == nil, "Ensure no duplicate IDs");

		-- needed in the end to verify we got no holes in the array:
		max_id = math.max(max_id, state.id);

		registry[state.id + 1] = {
			id = assert(state.id, "id is required."),
			name = assert(blockname, "Block type name is required."),
			-- default = state.default or nil,  -- may need this later
			props = state.properties,
		};
	end
end


-- The following assertion is not necessary by the current spec, but is required
-- by how lunajson distinguishes objects from arrays. Also if this fails, it is
-- _very_ likely that the input file is faulty.
assert(#registry == max_id + 1, "Ensure that registry has contiguous keys");

local out = {
	Metadata = {
		ProtocolBlockTypePaletteVersion = 1
	},
	Palette = registry
};

io.write(encode(out), "\n");
