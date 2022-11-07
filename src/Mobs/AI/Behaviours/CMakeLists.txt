
target_sources(
        ${CMAKE_PROJECT_NAME} PRIVATE
        BasicTask.h
        DynamicGuardSelector.h
        GuardedTask.h
        HealthRange.h
        MoveToPosition.h
        RandomPosition.h
        RandomWait.h
        Succeder.h

        DynamicGuardSelector.cpp
        GuardedTask.cpp
        HealthRange.cpp
        MoveToPosition.cpp
        RandomPosition.cpp
        RandomWait.cpp
        Succeder.cpp
)