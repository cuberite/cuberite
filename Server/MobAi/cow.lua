BehaviorTree =
{
    DynamicGuardSelector =
    {
        {
            Guard =
            {
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
