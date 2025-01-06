BehaviorTree =
{
    -- Selects from the range of supplied entries with the specified guards
    DynamicGuardSelector =
    {
        {
            -- If health is low: panics and runs away
            Guard =
            {
                -- If health is lower then 0.5 - this equals to half of the HP
                HealthRange =
                {
                    Min = 0.0,
                    Max = 0.5
                },
            },
            Task =
            {
                Sequence =
                {
                    RandomPosition =
                    {
                        UniqueID = "PanicPos",
                        Range = 20.0
                    },
                    MoveToPosition =
                    {
                        UniqueID = "PanicPos",
                    }
                }
            }
        },
        {
            -- Walks to a player in the range holding a specified item
            Guard =
            {
                -- Seeks for a player that holds the item in the range
                BreedingItemGuard =
                {
                    Item = "Wheat",
                    Range = 5.0,
                },
            },
            Task =
            {
                MoveToPosition =
                {
                    UniqueID = "FeederPos",
                },
            }
        },
        {
            -- Basic task, wanders around
            Guard = nil,
            Task =
            {
                Sequence =
                {
                    RandomPosition=
                    {
                        UniqueID = "WanderPos",
                        Range = 10.0,
                    },
                    Succeeder=
                    {
                        MoveToPosition =
                        {
                            UniqueID = "WanderPos",
                        }
                    },
                    RandomWait =
                    {
                        Min = 1,
                        Max = 4,
                    }
                }
            }
        }
    }
}
