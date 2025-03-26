set(OPENDAQ_ENABLE_TESTS false)

FetchContent_Declare(
    openDAQ
    GIT_REPOSITORY https://github.com/openDAQ/openDAQ.git
    GIT_TAG v3.19.0-rc
    GIT_PROGRESS   ON
)

FetchContent_MakeAvailable(openDAQ)
