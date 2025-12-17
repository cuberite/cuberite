
target_sources(
        ${CMAKE_PROJECT_NAME} PRIVATE
        BasicTask.h
        BreedingItemGuard.h
        DynamicGuardSelector.h
        GuardedTask.h
        HealthRange.h
        MoveToPosition.h
        RandomPosition.h
        RandomWait.h
        Succeder.h

        BreedingItemGuard.cpp
        DynamicGuardSelector.cpp
        GuardedTask.cpp
        HealthRange.cpp
        MoveToPosition.cpp
        RandomPosition.cpp
        RandomWait.cpp
        Succeder.cpp
)
