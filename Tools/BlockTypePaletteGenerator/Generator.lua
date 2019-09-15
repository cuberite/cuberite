-- lib/lunajson/src/ is not in default Lua package paths
package.path = 'lib/lunajson/src/?.lua;' .. package.path;

-- test path
if not pcall(function() require "lunajson.decoder" end) then
	io.stderr:write("Could not load required libraries, please run "..
	"`Generator.lua` within its directory and make sure to run `git submodule"..
	" update --init --recursive`.\n");
	os.exit();
end

local decode = (require "lunajson.decoder")();
local encode = (require "lunajson.encoder")();

local input = decode(io.stdin:read("*a"));
local registry = {};
local registry_coverage = {};
local max_id = 0;


for blockname,blockdata in pairs(input) do
	for i=1, #(blockdata.states or {}) do
		local state = blockdata.states[i];
		assert(registry[state.id+1] == nil, "Duplicate entry is an error state");

		-- needed in the end to verify we got no holes in the array:
		max_id = math.max(max_id, state.id);

		registry[state.id+1] = {
			-- id = state.id or error(""),
			name = assert(blockname, "Block type name is required."),
			-- default = state.default or false,
			props = state.properties,
		}
	end
end

-- io.stderr:write(#registry, " ", max_id, "\n");
assert(#registry == max_id+1, "Ensure that registry has contiguous keys");

out = registry
print(encode(out))
