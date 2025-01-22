set(OPENDAQ_ENABLE_TESTS false)

FetchContent_Declare(
    openDAQ
    GIT_REPOSITORY https://github.com/openDAQ/openDAQ.git
    GIT_TAG origin/main
    GIT_PROGRESS   ON
    SYSTEM
    FIND_PACKAGE_ARGS 3.11.0 GLOBAL
)

FetchContent_MakeAvailable(openDAQ)
