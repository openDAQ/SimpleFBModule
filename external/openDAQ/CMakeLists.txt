set(OPENDAQ_ENABLE_TESTS false)

FetchContent_Declare(
    openDAQ
    GIT_REPOSITORY https://github.com/openDAQ/openDAQ.git
    GIT_TAG v3.20.1
    GIT_PROGRESS   ON
	SYSTEM
    FIND_PACKAGE_ARGS 3.20.1 GLOBAL
)

FetchContent_MakeAvailable(openDAQ)
