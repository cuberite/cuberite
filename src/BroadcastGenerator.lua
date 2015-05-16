
Broadcasts = 
	{
		ChunkData = { 
			args =  { { 'Vector2i', 'a_Pos' }, { 'cChunkDataSerializer &', 'a_Serializer' } },
			pos = 'Chunk'
		}	
	}

local header, err = io.open("Broadcasts.inc", "w")
if (header == nil) then
	print("Cannot open file \"Broadcasts.inc\": " .. err)
	print("Aborting")
	os.exit(1)
end

local forwards, err = io.open("BroadcasterForwards.cpp", "w")
if (forwards == nil) then
	print("Cannot open file \"BroadcasterForwards.inc\": " .. err)
	print("Aborting")
	os.exit(1)
end

forwards:write(
[[
#include "Globals.h"
#include "Broadcaster.h"
#include "Vector2.h"
#include "Protocol/ChunkDataSerializer.h"
#include "World.h"
#include "Chunk.h"
]])

for name, params in pairs(Broadcasts) do
	argsWithType = ""
	args = ""
	first = true
	for i, arg in ipairs(params['args']) do
		if first then
			first = false
		else
			args = args .. ", "
			argsWithType = argsWithType .. ", "
		end
		argsWithType = argsWithType .. arg[1] .. " " .. arg[2]
		args = args .. arg[2]
	end
	header:write(string.format("void Broadcast%s (%s, const cClientHandle * a_Exclude = nullptr);\n", name, argsWithType))

	if params['pos'] == 'Chunk' then
		forwards:write(string.format(
			[[
void cBroadcaster::Broadcast%s (%s, const cClientHandle * a_Exclude)
{
	m_World->DoWithChunk(a_Pos,
		[&](cChunk & a_Chunk) -> bool
		{
			for (auto&& client : a_Chunk.GetAllClients())
			{
				if (client == a_Exclude)
				{
					continue;
				}
				client->Send%s(%s);
			};
			return true;
		});
}
			]], name, argsWithType, name, args))
	else
		forwards:write(string.format(
			[[
void cBroadcaster::Broadcast%s (%s, const cClientHandle * a_Exclude)
{
	m_World->DoWithChunkAt(a_Pos,
		[&](cChunk & a_Chunk) -> bool
		{
			for (auto&& client : a_Chunk.GetAllClients())
			{
				if (client == a_Exclude)
				{
					continue;
				}
				client->Send%s(%s);
			};
			return true;
		});
}
			]], name, argsWithType, name, args))
	end
	header:write("\n")
	forwards:write("\n")
end

