This generator crafts an intermediate index format to be read by cuberite

# Running

Run `lua ./Generator.lua`, pass `blocks.json` as first argument to the script
and the desired output location as 2nd argument.

Make sure to run the Generator from within its directory (`cd` into the path
where `Generator.lua` is.)

## Examples

```bash
SERVER=/path/to/server.jar
java -cp "$SERVER" net.minecraft.data.Main --reports &&
lua Generator.lua \
	generated/reports/blocks.json \
	../../Server/Protocol/1.13/ProtocolBlockTypePalette.json 
```

```bash
SERVER=/path/to/server.jar
java -cp "$SERVER" net.minecraft.data.Main --reports &&
lua Generator.lua - -\
	< generated/reports/blocks.json \
	> ../Server/Protocol/1.13/ProtocolBlockTypePalette.json
```

## Output format

The Format is a `JSON` document containing an object with at least two keys at
the top level: `Metadata` and `Palette`.

`Metadata` contains document metadata, namely a key `"ProtocolBlockType": 1`.

`Palette` contains an array of objects. Each of these objects has at least the
keys `id`, `name` and an optional `props` key that contains the individual
properties of the current state. These properties are a KV dict of pure strings.

The order of the array or object elements is not significant. `id` is unique.

```json
{
  "Metadata": {
    "ProtocolBlockType": 1
  },
  "Palette": [{
      "id": 0,
      "name": "minecraft:air"
    }, {
      "id": 1,
      "name": "minecraft:stone"
    }, {
      "id": 221,
      "name": "minecraft:dark_oak_leaves",
      "props": {
        "persistent": "false",
        "distance": "4"
      }
    }
  ]
}
```