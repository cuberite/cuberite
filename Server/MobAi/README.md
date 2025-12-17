# Howto: BehaviorTree for mobs in Cuberite

The behavior of mobs in Cuberite can be modified with behavior trees. Those are
described in lua files. For any preexisting monster type there is a file present.

If there is no file, there will be no behavior ingame. If the table in the root
object is empty, there will be no behavior. So for example you could comment
out the behavior tree or delete it but sill keep the file.

# File setup

The files are lua files with the proper file ending. The root objects *NEEDS*
to be "BehaviorTree". In this object a table containing the behavior needs to be
stored.

```lua
BehaviorTree =
{
 
}
```

## File names

The preexisting file names equal to the minecraft mob names in lowercase and
underscores as separators. E.g. `ender_dragon.lua`.

## Adding Behaviour

There is a wide selection of behaviours available. Many of them have additional
parameters which need to be set. If not supplied, during startup, the

# Behavior types

## Guards

Any behaviours listed in this section are intended to be used as a guard in the 
DynamicGuardSelector.

### BreedingItemGuard

This behaviour is used to select the closest player with a breeding item in
their hand. The position of the selected player is stored with the key
`FeederPos` to be used by MoveToPosition. 

#### Parameters

**Range:** The range to be searched in (Default: 5.0)

**Item:** The item to searched for. **WARNING:** Changing this item won't
change the item needed to breed the entity. May be multiple items separated by
commas.

#### Example

```lua
BreedingItemGuard =
{
    Item = "Wheat",
    Range = 5.0,  -- Todo: actual range
}
```

### HealthRange

Checks for the health of the entity. The parameters are supplied as relative
values.

#### Parameters

**Min:** The minimum health value.

**Max:** The maximum health value.

#### Example

```lua
HealthRange =
{
    Min = 0.0,
    Max = 0.5
}
```

## Selection

This section includes behaviors that can be used in combination with the already
presented guards to decide on the behavior of the entity.

### DynamicGuardSelector
### GuardedTask
### Sequence
### Selector
### Succeder

## Behaviors

The entries in this section are behaviours that actually create an impact on the
entity or its surroundings.

### MoveToPosition
### RandomPosition
### RandomWait


# Reloading the behaviours

Right now the behaviours are loaded during the creation of the entity. So one 
can either restart the server if the behaviour is the default one or spawn a new
one, currently the behaviour can't be changed after the creation of the entity.